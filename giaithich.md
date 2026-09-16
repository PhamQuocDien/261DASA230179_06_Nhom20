# Giải thích cấu trúc project Library Management System

## 1. Luồng tổng thể

Project nên được hiểu theo luồng từ ngoài vào trong:

```text
HTML/CSS/JS
    ↓
api.php
    ↓
main.cpp / C++ logic
    ↓
Service
    ↓
Repository
    ↓
Data Structure / Model
    ↓
JsonDatabase
    ↓
data/library.json
```

Luồng dữ liệu chính:

```text
data/library.json
        ↓
   JsonDatabase
        ↓
   nlohmann::json
        ↓
Book / Member / Loan...
```

Tư duy chính là C++ thao tác dữ liệu JSON thông qua `nlohmann::json` và lớp `JsonDatabase`.

---

## 2. `docs/`

```text
docs/
```

Đây là nơi chứa tài liệu dự án, không phải code chạy.

Có thể lưu:

```text
docs/
├── BaoCao.docx
├── UML/
├── DacTa.md
├── HuongDan.md
└── screenshots/
```

Dùng để lưu:

- đặc tả yêu cầu
- sơ đồ UML
- kiến trúc hệ thống
- ảnh minh chứng
- báo cáo D3/D4

---

## 3. `src/main.cpp`

```text
src/
└── main.cpp
```

Đây là điểm bắt đầu của chương trình C++ và chứa:

```cpp
int main()
```

Nhiệm vụ chính:

```text
Khởi động chương trình
       ↓
Khởi tạo database
       ↓
Khởi tạo các Service
       ↓
Hiển thị menu / nhận request
       ↓
Gọi Service xử lý
```

Ví dụ:

```cpp
int main() {
    JsonDatabase db("data/library.json");

    BookRepository bookRepo(db);
    BookService bookService(bookRepo);

    bookService.addBook(...);
}
```

### Không nên nhét toàn bộ logic vào `main.cpp`

Không nên gom tất cả:

- thêm sách
- xóa sách
- tìm sách
- mượn sách
- trả sách
- đọc JSON
- sắp xếp

vào `main.cpp`.

`main.cpp` chủ yếu dùng để điều phối.

---

## 4. `presentation/`

```text
presentation/
├── index.html
├── style.css
└── script.js
```

Đây là giao diện người dùng.

### `index.html`

Tạo cấu trúc giao diện, ví dụ:

```text
Trang quản lý thư viện
├── Danh sách sách
├── Thành viên
├── Mượn sách
├── Trả sách
└── Phiếu in
```

Ví dụ:

```html
<button id="btnAddBook">Thêm sách</button>
<div id="bookList"></div>
```

Không đặt logic C++ vào file này.

### `style.css`

Chỉ lo phần giao diện:

- màu sắc
- kích thước
- bố cục
- button
- table
- form
- responsive

### `script.js`

Là cầu nối phía trình duyệt:

```text
Người dùng click "Thêm sách"
             ↓
       script.js
             ↓
          api.php
             ↓
       C++ backend
```

Nó gửi request và nhận dữ liệu từ backend.

---

## 5. `api/api.php`

```text
api/
└── api.php
```

Đây là API trung gian giữa giao diện web và C++.

Ví dụ:

```text
index.html
    ↓
script.js
    ↓
api.php
    ↓
C++
```

Một request có thể chứa:

```text
action=ADD_BOOK
bookCode=LSVN001
title=Lịch sử Việt Nam
```

`api.php` nhận request rồi chuyển tiếp sang backend theo cách nhóm thiết kế.

Nói đơn giản:

> `api.php` là lớp trung gian/phiên dịch giữa Web và C++.

---

## 6. `dsa_core/`

```text
dsa_core/
├── models/
├── structures/
├── algorithms/
├── repositories/
└── services/
```

Đây là trung tâm DSA + dữ liệu của project.

Có thể hiểu:

```text
models       = dữ liệu là gì?
structures   = dữ liệu được lưu bằng cấu trúc gì?
algorithms   = xử lý bằng thuật toán gì?
repositories = lấy/lưu dữ liệu như thế nào?
services     = nghiệp vụ hoạt động ra sao?
```

---

# 7. `models/`

```text
models/
├── Book.h
├── Member.h
├── Loan.h
├── Reservation.h
└── Fine.h
```

Đây là các đối tượng nghiệp vụ của hệ thống.

## `Book.h`

Đại diện cho sách.

Ví dụ:

```cpp
class Book {
private:
    string bookCode;
    string title;
    string author;
    int year;
};
```

Có thể hình dung:

```text
Book
├── bookCode
├── title
├── author
├── year
└── copies
```

Trong dữ liệu JSON, `copies` có thể là một array chứa các object bản sao sách với `bookId` và `status`.

Ví dụ:

```json
{
    "books": [
        {
            "bookId": "LSVN001",
            "status": "AVAILABLE"
        },
        {
            "bookId": "LSVN002",
            "status": "BORROWED"
        }
    ]
}
```

## `Member.h`

Đại diện cho độc giả/thành viên thư viện.

Có thể có:

```text
Member
├── memberId
├── name
├── className
└── ...
```

Dùng cho:

- thêm thành viên
- tìm thành viên
- xóa thành viên
- kiểm tra thành viên

## `Loan.h`

Đại diện cho phiếu mượn / giao dịch mượn sách.

Ví dụ:

```text
Loan
├── loanId
├── memberId
├── bookId
├── borrowDate
├── dueDate
├── returnDate
└── status
```

Một quy tắc quan trọng là trạng thái giao dịch và trạng thái bản sao phải nhất quán, ví dụ:

```text
Loan.status = BORROWING
        ↕
BookCopy.status = BORROWED
```

## `Reservation.h`

Đại diện cho việc đặt trước sách.

Ví dụ:

```text
Reservation
├── reservationId
├── memberId
├── bookId
├── reservationDate
└── status
```

Luồng:

```text
sách đang được mượn
        ↓
thành viên đặt trước
        ↓
sách trả lại
        ↓
xử lý người đặt trước
```

## `Fine.h`

Đại diện cho tiền phạt.

Ví dụ:

```text
Fine
├── fineId
├── memberId
├── loanId
├── amount
└── status
```

Có thể dùng cho:

- quá hạn
- mất sách
- hư sách
- thanh toán tiền phạt

---

# 8. `structures/`

```text
structures/
├── Node.h
├── DynamicArray.h
├── LinkedList.h
└── HashTable.h
```

Đây là phần DSA thực sự.

## `Node.h`

Nút cơ bản của Linked List.

Ví dụ:

```cpp
template <typename T>
struct Node {
    T data;
    Node<T>* next;
};
```

Tư duy:

```text
[data | next] → [data | next] → [data | next]
```

## `DynamicArray.h`

Tự cài mảng động.

Ví dụ:

```text
capacity = 5

[Book][Book][Book][ ][ ]
```

Khi đầy:

```text
capacity = 10

[Book][Book][Book][Book][ ][ ][ ][ ][ ][ ]
```

Có thể có các thao tác:

```text
add()
remove()
get()
set()
size()
resize()
```

## `LinkedList.h`

Cài danh sách liên kết.

Ví dụ:

```text
head
 ↓
Node → Node → Node → nullptr
```

Có thể dùng cho `Loan`, `Reservation`, `Fine` tùy thiết kế.

## `HashTable.h`

Cài bảng băm.

Tư duy:

```text
memberId
   ↓
hash()
   ↓
bucket
   ↓
Member
```

Có thể có:

```cpp
Member* find(string memberId);
void insert(Member member);
void remove(string memberId);
```

---

# 9. `algorithms/Search/LinearSearch.h`

```text
Search/
└── LinearSearch.h
```

Cài Linear Search.

Ví dụ tìm `LSVN004`:

```text
Book 1
Book 2
Book 3
Book 4  ← tìm thấy
Book 5
```

Duyệt:

```text
1 → 2 → 3 → 4
```

Cho tới khi tìm thấy hoặc hết dữ liệu.

Có thể có hàm:

```cpp
linearSearch(...)
```

---

# 10. `algorithms/Sort/`

```text
Sort/
├── MergeSort.h
└── QuickSort.h
```

## `MergeSort.h`

Cài Merge Sort.

Tư duy:

```text
[5 2 8 1]
     ↓
[5 2] [8 1]
 ↓      ↓
[5][2][8][1]
     ↓
[2 5] [1 8]
     ↓
[1 2 5 8]
```

Dùng khi cần sắp xếp dữ liệu.

## `QuickSort.h`

Cài Quick Sort.

Ví dụ:

```text
pivot = 5
```

Chia thành:

```text
nhỏ hơn 5 | 5 | lớn hơn 5
```

sau đó đệ quy.

---

# 11. `repositories/`

```text
repositories/
├── BookRepository.h
├── MemberRepository.h
├── LoanRepository.h
├── ReservationRepository.h
└── FineRepository.h
```

Repository là lớp chịu trách nhiệm truy cập/lưu dữ liệu.

Ví dụ:

```text
BookService
     ↓
BookRepository
     ↓
JsonDatabase
     ↓
library.json
```

## `BookRepository.h`

Quản lý việc lấy/lưu `Book`.

Ví dụ:

```cpp
addBook()
getBook()
getAllBooks()
updateBook()
deleteBook()
```

## `MemberRepository.h`

Tương tự nhưng cho `Member`.

## `LoanRepository.h`

Tương tự nhưng cho `Loan`.

Ví dụ:

```cpp
createLoan()
getLoan()
updateLoanStatus()
```

## `ReservationRepository.h`

Quản lý `Reservation`.

## `FineRepository.h`

Quản lý `Fine`.

---

# 12. `services/`

```text
services/
├── BookService.h
├── MemberService.h
├── LoanService.h
├── ReservationService.h
├── FineService.h
└── LoanSlipService.h
```

Service là nơi xử lý nghiệp vụ.

Điểm cần nhớ:

```text
Repository
= Lấy/lưu dữ liệu

Service
= Quyết định phải làm gì
```

## `BookService.h`

Xử lý nghiệp vụ sách:

- thêm sách
- sửa sách
- xóa sách
- tìm sách
- kiểm tra sách có tồn tại
- kiểm tra còn bản sao

Luồng:

```text
BookService
     ↓
BookRepository
     ↓
JsonDatabase
```

## `MemberService.h`

Xử lý nghiệp vụ thành viên:

- đăng ký thành viên
- xóa thành viên
- tìm thành viên
- kiểm tra thành viên tồn tại

## `LoanService.h`

Một trong các file nghiệp vụ quan trọng nhất.

Xử lý:

- mượn sách
- trả sách
- gia hạn
- kiểm tra điều kiện mượn
- cập nhật trạng thái

Luồng ví dụ:

```text
Member có tồn tại?
       ↓
Book có tồn tại?
       ↓
BookCopy còn AVAILABLE?
       ↓
Tạo Loan
       ↓
Loan = BORROWING
       ↓
BookCopy = BORROWED
```

## `ReservationService.h`

Xử lý:

- đặt sách
- hủy đặt
- xem danh sách đặt
- xử lý khi sách được trả

## `FineService.h`

Xử lý:

- tính tiền phạt
- tạo Fine
- kiểm tra Fine
- thanh toán
- cập nhật trạng thái

Ví dụ:

```text
ngày trả > hạn trả
       ↓
tính số ngày trễ
       ↓
tính tiền phạt
       ↓
Fine
```

## `LoanSlipService.h`

Phù hợp với phần Phiếu In.

Nó không trực tiếp tạo giao dịch mượn mà tạo thông tin phiếu từ `Loan`.

Luồng:

```text
Loan
 ↓
LoanSlipService
 ↓
Phiếu in
```

Phiếu có thể chứa:

```text
--------------------------------
        PHIẾU MƯỢN SÁCH
--------------------------------
Mã thành viên: TV001
Tên: Nguyễn Văn A

Mã sách: LSVN001
Tên sách: Lịch sử Việt Nam

Ngày mượn: 16/09/2026
Ngày trả:   23/09/2026

Trạng thái: BORROWING
--------------------------------
```

Nếu cần in phiếu theo ngày trả, Service này là nơi phù hợp để gom và sắp xếp dữ liệu trước khi đưa ra giao diện.

---

# 13. `persistence/`

```text
persistence/
├── JsonDatabase.h
└── JsonDatabase.cpp
```

Đây là tầng làm việc trực tiếp với file JSON.

## `JsonDatabase.h`

Định nghĩa lớp database.

Ví dụ:

```cpp
class JsonDatabase {
public:
    bool load();
    bool save();

    json& data();
};
```

Nó có thể chịu trách nhiệm:

```text
load library.json
save library.json
truy cập JSON
```

## `JsonDatabase.cpp`

Triển khai thật cho `JsonDatabase.h`.

Có thể thực hiện:

```text
mở file
↓
đọc JSON
↓
parse
↓
lưu vào nlohmann::json
```

Khi lưu:

```text
nlohmann::json
↓
dump()
↓
library.json
```

Có thể dùng:

```cpp
std::cout << book.dump(4);
```

để in JSON có indent 4 khoảng trắng.

---

# 14. `data/library.json`

```text
data/
└── library.json
```

Đây là dữ liệu thật của thư viện.

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

Đây là data, không phải code.

Luồng dữ liệu:

```text
library.json
      ↓
JsonDatabase
      ↓
nlohmann::json
      ↓
Book / Member / Loan ...
```

Một ví dụ chi tiết hơn với array chứa object:

```json
{
    "books": [
        {
            "bookId": "LSVN001",
            "status": "AVAILABLE"
        },
        {
            "bookId": "LSVN002",
            "status": "BORROWED"
        }
    ]
}
```

---

# 15. `README.md`

```text
README.md
```

Đây là file hướng dẫn project.

Nên ghi:

1. Project là gì
2. Thành viên nhóm
3. Cách chạy
4. Cấu trúc project
5. Công nghệ sử dụng
6. DSA sử dụng
7. Cách build

Ví dụ:

```md
# Library Management System

## Technologies
- C++
- HTML/CSS/JS
- PHP
- nlohmann/json

## DSA
- Dynamic Array
- Linked List
- Hash Table
- Linear Search
- Merge Sort
- Quick Sort
```

---

# 16. `.gitignore`

```text
.gitignore
```

Dùng để nói với Git những file/folder không cần upload lên GitHub.

Ví dụ:

```gitignore
.vscode/
build/
*.exe
*.log
```

---

# 17. Sơ đồ tổng thể cần nhớ

```text
                     WEB
                      │
              ┌───────┴───────┐
              │               │
          index.html      script.js
              │               │
              └───────┬───────┘
                      ↓
                   api.php
                      ↓
                  C++ / main
                      ↓
                   SERVICE
                      ↓
                  REPOSITORY
                      ↓
          ┌───────────┴───────────┐
          ↓                       ↓
      STRUCTURE                 MODEL
          ↓                       ↓
 DynamicArray                 Book
 LinkedList                   Member
 HashTable                    Loan
          │                   Reservation
          │                   Fine
          └───────────┬───────────┘
                      ↓
                JsonDatabase
                      ↓
                nlohmann::json
                      ↓
                library.json
```

---

# 18. Thứ tự nên code

Không nên mở `main.cpp` rồi code toàn bộ ngay.

Nên đi theo thứ tự:

```text
Bước 1
MODEL
Book.h
Member.h
Loan.h
Reservation.h
Fine.h

        ↓

Bước 2
DSA
Node.h
DynamicArray.h
LinkedList.h
HashTable.h

        ↓

Bước 3
ALGORITHM
LinearSearch.h
MergeSort.h
QuickSort.h

        ↓

Bước 4
PERSISTENCE
JsonDatabase.h
JsonDatabase.cpp

        ↓

Bước 5
REPOSITORY
BookRepository
MemberRepository
LoanRepository
...

        ↓

Bước 6
SERVICE
BookService
MemberService
LoanService
...

        ↓

Bước 7
LoanSlipService

        ↓

Bước 8
main.cpp

        ↓

Bước 9
api.php

        ↓

Bước 10
HTML/CSS/JS
```

Lý do: tầng trên phụ thuộc tầng dưới. Ví dụ `BookService` cần `BookRepository`, còn `BookRepository` cần `JsonDatabase`.

---

# 19. Phân biệt 3 lớp quan trọng

Đừng nhầm ba thứ sau:

```text
Book
↓
"Cuốn sách là gì?"

BookRepository
↓
"Lấy/lưu Book ở đâu?"

BookService
↓
"Có được phép thực hiện nghiệp vụ với Book không?"
```

Tương tự:

```text
Loan
LoanRepository
LoanService
```

Đây là cách chia project thành từng tầng thay vì gom toàn bộ code vào một file.

---

# 20. File nên bắt đầu code

Nên bắt đầu từ:

```text
Book.h
    ↓
Member.h
    ↓
Loan.h
    ↓
Reservation.h
    ↓
Fine.h
    ↓
JsonDatabase
```

Sau đó mới đi lên Repository, Service và cuối cùng là `main.cpp`, API và giao diện.
