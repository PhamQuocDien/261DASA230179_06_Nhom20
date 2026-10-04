$ErrorActionPreference = 'Stop'
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$Exe = Join-Path $ProjectRoot 'bin\ThuVien.exe'
$Runs = 20

if (-not (Test-Path $Exe)) { throw "Khong tim thay bin\ThuVien.exe" }

function Invoke-ApiRaw {
    param([hashtable]$Request)
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
    if ([string]::IsNullOrWhiteSpace($stdout)) { throw "API khong tra JSON. stderr=$stderr" }
    return $stdout
}

$libraryPath = Join-Path $ProjectRoot 'data\library.json'
$data = Get-Content $libraryPath -Raw | ConvertFrom-Json
$books = @($data.books)
$years = @($books | ForEach-Object { if ($null -ne $_.year) { [int]$_.year } })
if ($years.Count -eq 0) { throw 'Khong co Book co Year de benchmark.' }
$yearStart = ($years | Measure-Object -Minimum).Minimum
$yearEnd = ($years | Measure-Object -Maximum).Maximum

$rows = @()

foreach ($i in 1..$Runs) {
    $sw = [Diagnostics.Stopwatch]::StartNew()
    $null = Invoke-ApiRaw @{action='getBooksByYearRange';yearStart=[int]$yearStart;yearEnd=[int]$yearEnd}
    $sw.Stop()
    $rows += [pscustomobject]@{Run=$i;Operation='MC2';YearStart=$yearStart;YearEnd=$yearEnd;ElapsedMicroseconds=[math]::Round($sw.Elapsed.TotalMilliseconds * 1000,2)}
}

$out = Join-Path $PSScriptRoot 'benchmark_mc2_results.csv'
$rows | Export-Csv -Path $out -NoTypeInformation -Encoding UTF8
$avg = ($rows.ElapsedMicroseconds | Measure-Object -Average).Average
$min = ($rows.ElapsedMicroseconds | Measure-Object -Minimum).Minimum
$max = ($rows.ElapsedMicroseconds | Measure-Object -Maximum).Maximum

Write-Host '============================================'
Write-Host 'MC2 BENCHMARK - CURRENT DATASET'
Write-Host '============================================'
Write-Host "Books : $($books.Count)"
Write-Host "Range : $yearStart -> $yearEnd"
Write-Host "Runs  : $Runs"
Write-Host "Average (us): $([math]::Round($avg,2))"
Write-Host "Min (us)    : $min"
Write-Host "Max (us)    : $max"
Write-Host "CSV         : $out"
Write-Host ''
Write-Host 'Luu y: day la benchmark tren dataset hien tai, chua thay the yeu cau 10,000/100,000 ban ghi cua D5.' -ForegroundColor Yellow
