$ErrorActionPreference = 'Stop'
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$Exe = Join-Path $ProjectRoot 'bin\ThuVien.exe'

if (-not (Test-Path $Exe)) {
    Write-Host "[ERROR] Khong tim thay $Exe" -ForegroundColor Red
    Write-Host "Hay chay script tu thu muc goc cua project va build bin\ThuVien.exe truoc." -ForegroundColor Yellow
    exit 1
}

$script:Passed = 0
$script:Failed = 0
$script:Results = @()

function Invoke-Api {
    param([Parameter(Mandatory)][hashtable]$Request)
    $json = $Request | ConvertTo-Json -Compress -Depth 20
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $Exe
    $psi.Arguments = '--api'
    $psi.WorkingDirectory = $ProjectRoot
    $psi.UseShellExecute = $false
    $psi.CreateNoWindow = $true
    $psi.RedirectStandardInput = $true
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError = $true
    $p = New-Object System.Diagnostics.Process
    $p.StartInfo = $psi
    [void]$p.Start()
    $p.StandardInput.WriteLine($json)
    $p.StandardInput.Close()
    $stdout = $p.StandardOutput.ReadToEnd()
    $stderr = $p.StandardError.ReadToEnd()
    $p.WaitForExit()
    if ([string]::IsNullOrWhiteSpace($stdout)) {
        throw "API khong tra ve JSON. stderr=$stderr"
    }
    try {
        return ($stdout | ConvertFrom-Json)
    }
    catch {
        throw "Response khong phai JSON hop le: $stdout`nstderr=$stderr"
    }
}

function Assert-True {
    param([bool]$Condition,[string]$Message)
    if (-not $Condition) { throw $Message }
}

function Assert-Equal {
    param($Actual,$Expected,[string]$Message)
    if ($Actual -ne $Expected) { throw "$Message | expected=[$Expected] actual=[$Actual]" }
}

function Run-Test {
    param([string]$Name,[scriptblock]$Body)
    try {
        & $Body
        $script:Passed++
        $script:Results += [pscustomobject]@{Test=$Name;Result='PASS';Message=''}
        Write-Host "[PASS] $Name" -ForegroundColor Green
    }
    catch {
        $script:Failed++
        $script:Results += [pscustomobject]@{Test=$Name;Result='FAIL';Message=$_.Exception.Message}
        Write-Host "[FAIL] $Name" -ForegroundColor Red
        Write-Host "       $($_.Exception.Message)" -ForegroundColor Yellow
    }
}

Write-Host '============================================' -ForegroundColor Cyan
Write-Host '        D5 AUTOMATED API TEST SUITE' -ForegroundColor Cyan
Write-Host '============================================' -ForegroundColor Cyan
Write-Host "Project: $ProjectRoot"
Write-Host "Executable: $Exe"
Write-Host ''

$booksResponse = $null
$books = @()

Run-Test 'READ - getBooks returns success and data array' {
    $script:booksResponse = Invoke-Api @{action='getBooks'}
    Assert-True ($booksResponse.success -eq $true) 'getBooks phai success=true.'
    Assert-True ($null -ne $booksResponse.data) 'getBooks phai co data.'
    Assert-True ($booksResponse.data -is [System.Array]) 'getBooks.data phai la array.'
    $script:books = @($booksResponse.data)
}

Run-Test 'READ - getBooks response does not expose password' {
    $raw = $booksResponse | ConvertTo-Json -Depth 20 -Compress
    Assert-True ($raw -notmatch '"password"') 'Response getBooks khong duoc chua password.'
}

$sampleBook = $books | Select-Object -First 1
$sampleBookCode = if ($null -ne $sampleBook) { [string]$sampleBook.bookCode } else { '' }

Run-Test 'MC1 - getBook finds an existing Book_ID' {
    Assert-True (-not [string]::IsNullOrWhiteSpace($sampleBookCode)) 'Khong lay duoc bookCode mau tu getBooks.'
    $r = Invoke-Api @{action='getBook';bookCode=$sampleBookCode}
    Assert-Equal $r.success $true 'getBook bookCode hop le.'
    Assert-Equal ([string]$r.data.bookCode) $sampleBookCode 'Book_ID/BookCode tra ve phai dung ban ghi.'
}

Run-Test 'MC1 - getBook rejects a non-existing Book_ID' {
    $r = Invoke-Api @{action='getBook';bookCode='__D5_BOOK_NOT_FOUND_999999__'}
    Assert-Equal $r.success $false 'Book khong ton tai phai bi tu choi.'
}

Run-Test 'MC1 - getBook rejects empty Book_ID' {
    $r = Invoke-Api @{action='getBook';bookCode=''}
    Assert-Equal $r.success $false 'bookCode rong phai bi tu choi.'
}

$yearedBooks = @($books | Where-Object { $_.year -is [int] -or $_.year -is [long] -or $_.year -as [int] })
$yearValues = @($yearedBooks | ForEach-Object { [int]$_.year })
$minYear = if ($yearValues.Count -gt 0) { ($yearValues | Measure-Object -Minimum).Minimum } else { 0 }
$maxYear = if ($yearValues.Count -gt 0) { ($yearValues | Measure-Object -Maximum).Maximum } else { 0 }

Run-Test 'MC2 - full year range returns only books inside the requested range' {
    Assert-True ($yearValues.Count -gt 0) 'Khong co Book co Year de test MC2.'
    $r = Invoke-Api @{action='getBooksByYearRange';yearStart=[int]$minYear;yearEnd=[int]$maxYear}
    Assert-Equal $r.success $true 'Range hop le phai success=true.'
    Assert-Equal ([int]$r.yearStart) ([int]$minYear) 'yearStart response sai.'
    Assert-Equal ([int]$r.yearEnd) ([int]$maxYear) 'yearEnd response sai.'
    foreach ($b in @($r.data)) {
        Assert-True ([int]$b.year -ge [int]$minYear -and [int]$b.year -le [int]$maxYear) "Book $($b.bookCode) nam ngoai range."
    }
}

if ($yearValues.Count -gt 0) {
    $selectedYear = [int]$yearValues[0]
    Run-Test "MC2 - exact year $selectedYear handles books sharing the same Year" {
        $expected = @($books | Where-Object { [int]$_.year -eq $selectedYear } | ForEach-Object { [string]$_.bookCode } | Sort-Object)
        $r = Invoke-Api @{action='getBooksByYearRange';yearStart=$selectedYear;yearEnd=$selectedYear}
        Assert-Equal $r.success $true 'Year range hop le phai success=true.'
        $actual = @($r.data | ForEach-Object { [string]$_.bookCode } | Sort-Object)
        Assert-Equal (($actual -join '|')) (($expected -join '|')) 'Ket qua exact-year phai khop du lieu dau vao.'
    }
}

Run-Test 'MC2 - empty range returns an empty result' {
    Assert-True ($yearValues.Count -gt 0) 'Khong co Year de tao empty range.'
    $r = Invoke-Api @{action='getBooksByYearRange';yearStart=([int]$maxYear + 1);yearEnd=([int]$maxYear + 2)}
    Assert-Equal $r.success $true 'Empty range van la request hop le.'
    Assert-Equal (@($r.data).Count) 0 'Range khong co nam phu hop phai tra ve rong.'
}

Run-Test 'MC2 - reversed range is rejected' {
    $r = Invoke-Api @{action='getBooksByYearRange';yearStart=2025;yearEnd=2020}
    Assert-Equal $r.success $false 'yearStart > yearEnd phai bi tu choi.'
}

Run-Test 'MC2 - missing range parameters are rejected' {
    $r = Invoke-Api @{action='getBooksByYearRange';yearStart=2020}
    Assert-Equal $r.success $false 'Thieu yearEnd phai bi tu choi.'
}

Run-Test 'Loan Slip - valid loan ID returns the matching slip' {
    $libraryPath = Join-Path $ProjectRoot 'data\library.json'
    Assert-True (Test-Path $libraryPath) 'Khong tim thay data/library.json.'
    $data = Get-Content $libraryPath -Raw | ConvertFrom-Json
    $loan = @($data.loans) | Select-Object -First 1
    Assert-True ($null -ne $loan) 'Khong co loan mau trong library.json.'
    $r = Invoke-Api @{action='getLoanSlipByLoanId';loanId=[string]$loan.loanId}
    Assert-Equal $r.success $true 'Loan_ID ton tai phai tao duoc loan slip.'
    Assert-Equal ([string]$r.data.loanId) ([string]$loan.loanId) 'Loan slip tra ve sai Loan_ID.'
}

Run-Test 'Loan Slip - invalid loan ID is rejected' {
    $r = Invoke-Api @{action='getLoanSlipByLoanId';loanId='__D5_LOAN_NOT_FOUND_999999__'}
    Assert-Equal $r.success $false 'Loan_ID khong ton tai phai bi tu choi.'
}

Run-Test 'Reservation Read - getReservationsByBookCode accepts a valid Book_ID' {
    Assert-True (-not [string]::IsNullOrWhiteSpace($sampleBookCode)) 'Khong co bookCode mau.'
    $r = Invoke-Api @{action='getReservationsByBookCode';bookCode=$sampleBookCode}
    Assert-Equal $r.success $true 'Book_ID hop le phai tra duoc danh sach reservation.'
    foreach ($item in @($r.data)) {
        Assert-Equal ([string]$item.bookCode) $sampleBookCode 'Reservation tra ve phai thuoc dung Book_ID.'
        Assert-Equal ([string]$item.status) 'WAITING' 'Reservation read phai loc WAITING.'
    }
}

Run-Test 'Trending - only books above the 50 percent borrowing threshold are returned' {
    $r = Invoke-Api @{action='getTrendingBooks'}
    Assert-Equal $r.success $true 'getTrendingBooks phai success=true.'
    foreach ($item in @($r.data)) {
        $borrowed = [int]$item.borrowedCopies
        $total = [int]$item.totalCopies
        Assert-True ($total -gt 0) "Trending book $($item.bookCode) co totalCopies <= 0."
        Assert-True ($borrowed * 2 -gt $total) "Trending book $($item.bookCode) khong dat borrowed*2 > total."
    }
}

Write-Host ''
Write-Host '============================================' -ForegroundColor Cyan
Write-Host "TOTAL : $($script:Passed + $script:Failed)" -ForegroundColor Cyan
Write-Host "PASS  : $script:Passed" -ForegroundColor Green
Write-Host "FAIL  : $script:Failed" -ForegroundColor Red
Write-Host '============================================' -ForegroundColor Cyan

$reportPath = Join-Path $PSScriptRoot 'test_results.csv'
$script:Results | Export-Csv -Path $reportPath -NoTypeInformation -Encoding UTF8
Write-Host "Report: $reportPath"

if ($script:Failed -gt 0) { exit 1 }
exit 0
