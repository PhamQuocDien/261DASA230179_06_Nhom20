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
# Tổng Hợp Trạng Thái và Luồng Nghiệp Vụ

## 1. Bảng Trạng Thái Thực Thể

Dưới đây là danh sách các thực thể trong hệ thống và những trạng thái tương ứng:

| Thực thể (Entity) | Trạng thái (Status) | Ghi chú |
| :--- | :--- | :--- |
| **BookCopy** | `AVAILABLE`, `BORROWED` | Trạng thái của từng cuốn sách vật lý. |
| **Loan** | `BORROWING`, `RETURNED` | Trạng thái của một lượt mượn. |
| **Reservation**| `WAITING` | Trạng thái đặt trước sách. |
| **Fine** | `UNPAID`, `PAID` | Trạng thái nộp phạt. |
| **Member** | `ACTIVE` | Trạng thái hoạt động của thành viên. |

---

## 2. Giải Thích Luồng Nghiệp Vụ (Flow)

### 📌 Luồng Mượn Sách (Borrow)
Khi một thành viên mượn sách thành công, các trạng thái sẽ thay đổi như sau:
* **BookCopy:** `AVAILABLE` ➔ `BORROWED`
* **Loan:** (Tạo mới) ➔ `BORROWING`

### 📌 Luồng Trả Sách (Return)
Khi thành viên đem sách đến trả, hệ thống cập nhật:
* **BookCopy:** `BORROWED` ➔ `AVAILABLE`
* **Loan:** `BORROWING` ➔ `RETURNED`

---

## 3. Quy Tắc Đặc Biệt (Business Rules)

Theo quy tắc đã chốt dành cho Service quản lý phiếu mượn:

**Đối với `LoanSlipService`:**
* 🟢 **CHỈ hiển thị** các Loan đang có `status = BORROWING`.
* 🔴 **KHÔNG hiển thị** các Loan đã có `status = RETURNED`.
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

---

# 2. Giải thích từng file

## A. `docs/`

**Chứa:**
- D2, D3, D4.
- Tài liệu yêu cầu.
- Sơ đồ.
- Tài liệu thiết kế.
- Hình ảnh minh họa.

**Làm:**
- Lưu tài liệu của project.

---

# 3. `src/main.cpp`

**Chứa:**
- Hàm `main()`.
- Khởi tạo chương trình.
- Khởi tạo các thành phần cần thiết.

**Làm:**
- Điểm bắt đầu của chương trình C++.
- Điều phối việc khởi chạy hệ thống.

**Không làm:**
- Không chứa toàn bộ CRUD.
- Không chứa toàn bộ DSA.
- Không xử lý giao diện.

---

# 4. `src/presentation/`

Phần giao diện người dùng.

## `index.html`

**Chứa:**
- Form.
- Button.
- Table.
- Khu vực hiển thị sách.
- Khu vực hiển thị thành viên.
- Khu vực hiển thị phiếu mượn.

**Làm:**
- Tạo giao diện mà người dùng nhìn thấy và thao tác.

## `style.css`

**Chứa:**
- CSS.
- Layout.
- Font.
- Màu sắc.
- Button.
- Table.
- Form.

**Làm:**
- Trang trí và bố trí giao diện.

## `script.js`

**Chứa:**
- JavaScript.
- Event.
- Hàm lấy dữ liệu từ form.
- Hàm gọi API.
- Hàm nhận kết quả.

**Làm:**

```text
Người dùng bấm nút
        ↓
script.js lấy dữ liệu
        ↓
gọi api.php
        ↓
nhận kết quả
        ↓
cập nhật index.html
```

**Không làm:**
- Không triển khai HashTable.
- Không triển khai MergeSort.
- Không chứa business logic chính.

---

# 5. `src/api/api.php`

**Chứa:**
- Các API/action.
- Nhận request.
- Kiểm tra input.
- Gọi Service.
- Trả kết quả.

**Làm:**
- Là cầu nối giữa giao diện và DSA Core.

Ví dụ action:

```text
listBooks
getBook
addBook
updateBook
deleteBook
listLoans
getLoanSlip
```

**Không làm:**
- Không chứa HashTable.
- Không chứa MergeSort.
- Không chứa toàn bộ business logic.

---

# 6. `src/dsa_core/models/`

## `Book.h`

**Chứa:**
```text
bookId
title
author
category
year
quantity
availableQuantity
```

**Làm:**
- Định nghĩa kiểu dữ liệu `Book`.

## `Member.h`

**Chứa:**
```text
memberId
name
email
phone
```

**Làm:**
- Định nghĩa kiểu dữ liệu `Member`.

## `Loan.h`

**Chứa:**
```text
loanId
memberId
bookId
borrowDate
dueDate
returnDate
status
```

**Làm:**
- Định nghĩa dữ liệu một lượt/phiếu mượn.

## `Reservation.h`

**Chứa:**
```text
reservationId
memberId
bookId
reservationDate
status
```

**Làm:**
- Định nghĩa dữ liệu đặt trước sách.

## `Fine.h`

**Chứa:**
```text
fineId
loanId
memberId
amount
reason
status
```

**Làm:**
- Định nghĩa dữ liệu tiền phạt.

---

# 7. `src/dsa_core/structures/`

## `Node.h`

**Chứa:**
- `Node<T>`.
- `data`.
- `next`.

**Làm:**
- Node cơ bản cho Linked List và cấu trúc dựa trên Node.

## `DynamicArray.h`

**Chứa:**
- Mảng động.
- Bộ nhớ.
- Kích thước.
- Số lượng phần tử.

**Làm:**
- `add()`
- `remove()`
- `get()`
- `set()`
- `resize()`
- `size()`

Có thể dùng cho:

```text
DynamicArray<Book>
DynamicArray<Member>
DynamicArray<Loan>
```

## `LinkedList.h`

**Chứa:**
- Linked List.
- Node.
- Head/tail nếu cần.

**Làm:**
- `insert()`
- `remove()`
- `find()`
- `traverse()`

Chỉ dùng khi requirement cần Linked List.

## `HashTable.h`

**Chứa:**
- Hash function.
- Bucket.
- Key/value.
- Xử lý collision.
- Các thao tác Hash Table.

**Làm:**
- `insert(key, value)`
- `find(key)`
- `remove(key)`
- `contains(key)`

Ví dụ:

```text
"B001" → Book
"M001" → Member
"L001" → Loan
```

**Lưu ý:** Nếu `find()` đã thực hiện tra cứu bằng hash thì không cần tạo `HashSearch.h` riêng chỉ để tách file.

---

# 8. `src/dsa_core/algorithms/`

## `Search/LinearSearch.h`

**Chứa:**
- Thuật toán Linear Search.

**Làm:**
- Duyệt tuần tự dữ liệu để tìm phần tử.

## `Sort/MergeSort.h`

**Chứa:**
- Thuật toán Merge Sort.

**Làm:**
- Sắp xếp dữ liệu theo tiêu chí như năm, tên sách hoặc ngày mượn.

## `Sort/QuickSort.h`

**Chứa:**
- Thuật toán Quick Sort.

**Làm:**
- Sắp xếp dữ liệu khi requirement phù hợp.

**Lưu ý:** Không dùng thuật toán chỉ để “cho đủ DSA”.

---

# 9. `src/dsa_core/repositories/`

Repository quản lý và truy xuất dữ liệu trong DSA Core.

## `BookRepository.h`

**Chứa:**
- Tập dữ liệu Book.
- Các thao tác dữ liệu Book.

**Làm:**
- `add()`
- `findById()`
- `update()`
- `remove()`
- `getAll()`

Luồng:

```text
BookService
    ↓
BookRepository
    ↓
HashTable / DynamicArray
```

## `MemberRepository.h`

**Chứa:** dữ liệu Member.

**Làm:** thêm, tìm, sửa, xóa và lấy danh sách Member.

## `LoanRepository.h`

**Chứa:** dữ liệu Loan.

**Làm:** thêm, tìm, sửa, xóa và lấy danh sách Loan.

## `ReservationRepository.h`

**Chứa:** dữ liệu Reservation.

**Làm:** thêm, tìm, sửa, xóa và lấy danh sách Reservation.

## `FineRepository.h`

**Chứa:** dữ liệu Fine.

**Làm:** thêm, tìm, cập nhật, xóa và lấy danh sách Fine.

---

# 10. `src/dsa_core/services/`

Service xử lý nghiệp vụ.

## `BookService.h`

**Chứa:** logic nghiệp vụ Book.

**Làm:**
- Thêm sách.
- Tìm sách.
- Sửa sách.
- Xóa sách.
- Liệt kê sách.
- Tìm kiếm/sắp xếp khi cần.

Luồng:

```text
API
 ↓
BookService
 ↓
BookRepository
 ↓
DSA
```

## `MemberService.h`

**Chứa:** logic nghiệp vụ Member.

**Làm:** thêm, tìm, sửa, xóa và liệt kê Member.

## `LoanService.h`

**Chứa:** logic nghiệp vụ mượn/trả.

**Làm:**
- Tạo lượt mượn.
- Kiểm tra điều kiện mượn.
- Trả sách.
- Cập nhật trạng thái.
- Gia hạn nếu có requirement.

## `ReservationService.h`

**Chứa:** logic nghiệp vụ Reservation.

**Làm:** đặt trước, hủy đặt, kiểm tra trạng thái và xử lý quy tắc đặt trước.

## `FineService.h`

**Chứa:** logic nghiệp vụ Fine.

**Làm:** tính phạt, lấy thông tin phạt và cập nhật trạng thái.

## `LoanSlipService.h`

**Chứa:** logic chuẩn bị dữ liệu cho phiếu mượn.

**Kết hợp:**

```text
Member
+
Loan
+
Book
```

**Làm:** chuẩn bị dữ liệu:

```text
Loan ID
Member ID / Name
Book ID / Title
Borrow Date
Due Date
Return Date
Status
```

Luồng:

```text
LoanSlipService
       ↓
Dữ liệu phiếu mượn
       ↓
API
       ↓
Presentation
```

---

# 11. `src/persistence/`

## `JsonDatabase.h`

**Chứa:**
- Khai báo các hàm đọc/ghi JSON.

Ví dụ:

```cpp
load()
save()
```

**Làm:**
- Định nghĩa interface của tầng Persistence.

## `JsonDatabase.cpp`

**Chứa:**
- Phần cài đặt `JsonDatabase.h`.

**Làm:**
- Đọc `data/library.json`.
- Chuyển JSON thành dữ liệu chương trình.
- Ghi dữ liệu chương trình ra JSON.

Luồng:

```text
library.json
     ↕
JsonDatabase
     ↕
RAM
```

---

# 12. `data/library.json`

**Chứa:** dữ liệu thực tế của hệ thống thư viện.

Ví dụ:

```json
{
  "books": [],
  "members": [],
  "loans": [],
  "reservations": [],
  "fines": []
}
```

**Làm:**
- Lưu dữ liệu lâu dài của hệ thống.

---

# 13. `README.md`

**Chứa:**
- Giới thiệu project.
- Cấu trúc thư mục.
- Vai trò từng file.
- Kiến trúc.
- Hướng dẫn chạy nếu cần.

**Làm:**
- Giúp thành viên và người chấm hiểu project nhanh.

---

# 14. `.gitignore`

**Chứa:** các file/thư mục không cần đưa lên Git.

Ví dụ:

```gitignore
.vs/
build/
bin/
obj/
*.exe
*.o
*.obj
*.log
```

**Làm:**
- Giữ repository sạch.
- Không commit file build hoặc file tạm.

---

# 15. Luồng hệ thống

```text
USER
 ↓
presentation/index.html
 ↓
presentation/script.js
 ↓
api/api.php
 ↓
dsa_core/services/
 ↓
dsa_core/repositories/
 ↓
dsa_core/structures/
      +
dsa_core/algorithms/
 ↓
RAM
 ↕
persistence/JsonDatabase
 ↕
data/library.json
```

## Ghi nhớ

```text
models
→ Dữ liệu là gì?

structures
→ Dữ liệu được tổ chức thế nào?

algorithms
→ Tìm kiếm / sắp xếp thế nào?

repositories
→ Quản lý dữ liệu thế nào?

services
→ Nghiệp vụ hoạt động thế nào?

api
→ Web giao tiếp với Core thế nào?

presentation
→ Người dùng nhìn và thao tác thế nào?

persistence
→ Dữ liệu được lưu/đọc thế nào?

library.json
→ Dữ liệu được lưu ở đâu?
```

# 16. Nguyên tắc

- Mỗi file có một trách nhiệm chính.
- Không đưa DSA vào Presentation.
- `api.php` chỉ làm cầu nối.
- `repositories` và `persistence` tách riêng.
- Chỉ sử dụng cấu trúc dữ liệu/thuật toán khi có requirement thực tế.
- Không tạo file chỉ để làm project nhiều file hơn.
