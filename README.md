# 261DASA230179_06-
Đồ án nhóm môn DSA (DASA230179) - Hệ thống Quản lý Thư viện.
# 261DASA230179_06_Nhom20

## 1. Giới thiệu

Đây là repository của nhóm **261DASA230179_06_Nhom20** cho project hệ thống quản lý thư viện.

Project được tổ chức theo hướng tách biệt:

- **Presentation**: giao diện người dùng.
- **API**: cầu nối giữa giao diện và phần xử lý phía sau.
- **DSA Core**: mô hình dữ liệu, cấu trúc dữ liệu, thuật toán, repository và service.
- **Persistence**: đọc/ghi dữ liệu.
- **Data**: dữ liệu JSON của hệ thống.

---

## 2. Cấu trúc project

```text
261DASA230179_06_Nhom20/
│
├── docs/
│
├── src/
│   ├── main.cpp
│   │
│   ├── presentation/
│   │   ├── index.html
│   │   ├── style.css
│   │   └── script.js
│   │
│   ├── api/
│   │   └── api.php
│   │
│   ├── dsa_core/
│   │   ├── models/
│   │   │   ├── Book.h
│   │   │   ├── Member.h
│   │   │   ├── Loan.h
│   │   │   ├── Reservation.h
│   │   │   └── Fine.h
│   │   │
│   │   ├── structures/
│   │   │   ├── Node.h
│   │   │   ├── DynamicArray.h
│   │   │   ├── LinkedList.h
│   │   │   └── HashTable.h
│   │   │
│   │   ├── algorithms/
│   │   │   ├── Search/
│   │   │   │   └── LinearSearch.h
│   │   │   └── Sort/
│   │   │       ├── MergeSort.h
│   │   │       └── QuickSort.h
│   │   │
│   │   ├── repositories/
│   │   │   ├── BookRepository.h
│   │   │   ├── MemberRepository.h
│   │   │   ├── LoanRepository.h
│   │   │   ├── ReservationRepository.h
│   │   │   └── FineRepository.h
│   │   │
│   │   └── services/
│   │       ├── BookService.h
│   │       ├── MemberService.h
│   │       ├── LoanService.h
│   │       ├── ReservationService.h
│   │       ├── FineService.h
│   │       └── LoanSlipService.h
│   │
│   └── persistence/
│       ├── JsonDatabase.h
│       └── JsonDatabase.cpp
│
├── data/
│   └── library.json
│
├── README.md
└── .gitignore
```

---
