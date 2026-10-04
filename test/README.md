# D5 Test Suite

Bo test nay duoc viet cho project Library Management hien tai cua nhom, theo API backend/C++ dang co.

## Bo test dang co

`run_d5_tests.ps1` kiem tra tu dong:

- `getBooks` va response khong lo password.
- MC1: `getBook` voi Book_ID ton tai, khong ton tai, rong.
- MC2: `getBooksByYearRange` voi full range, exact year, empty range, reversed range, thieu tham so.
- Loan Slip: `getLoanSlipByLoanId` voi Loan_ID hop le/khong hop le.
- Reservation read: `getReservationsByBookCode`.
- Trending: kiem tra business rule `borrowed * 2 > total` tren ket qua backend.

Script khong sua `data/library.json`.

## Chay

Tu thu muc goc project:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\test\run_d5_tests.ps1
```

Hoac:

```cmd
.\test\run_tests.bat
```

Ket qua se duoc luu o:

```text
 test/test_results.csv
```

## Benchmark

Chay:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\test\benchmark_mc2.ps1
```

Ket qua:

```text
test/benchmark_mc2_results.csv
```

Benchmark nay chi do MC2 tren dataset hien tai. Theo de D5, nhom van can benchmark co quy mo du lon (mac dinh toi thieu 10.000 va 100.000 ban ghi neu chua co muc tieu khac duoc giang vien xac nhan), va phai co doi chung naive/linear scan.

## Vi tri bo test

```text
project-root/
├── bin/
│   └── ThuVien.exe
├── data/
│   └── library.json
└── test/
    ├── run_d5_tests.ps1
    ├── run_tests.bat
    ├── benchmark_mc2.ps1
    ├── README.md
    ├── test_results.csv       (tu sinh sau khi chay)
    └── benchmark_mc2_results.csv (tu sinh sau khi benchmark)
```

Bo test tap trung vao API/C++ backend va khong dua vao UI de ket luan backend dung hay sai.
