# Hệ Thống Quản Lý Thư Viện (Library Management System)

> 🌐 **Demo trực tuyến:**
> https://thuvienmini.onrender.com/presentation/index.html

Dự án này là một hệ thống quản lý thư viện được viết bằng C++, áp dụng kiến trúc phần mềm đa tầng (Multi-tier Architecture) và tích hợp các Cấu trúc dữ liệu & Thuật toán (DSA) tự cài đặt để tối ưu hóa hiệu suất truy vấn.

Hệ thống hỗ trợ hai chế độ hoạt động: Giao diện dòng lệnh (Console Mode) dành cho người dùng cuối và Giao diện lập trình ứng dụng (API Mode) thông qua giao tiếp JSON.
## 1.Hướng dẫn chạy trên máy
HƯỚNG DẪN CÀI ĐẶT VÀ CHẠY PROJECT
261DASA230179_06_Nhom20
Tài liệu này dùng cho thành viên trong nhóm muốn tải project từ GitHub về máy Windows, cài PHP từ file `php.rar` do nhóm cung cấp, cài các thành phần cần thiết và chạy hệ thống quản lý thư viện.
---
1. Yêu cầu trước khi bắt đầu
Máy tính cần có:
Windows 10/11.
Visual Studio Code.
Git hoặc có thể tải project dưới dạng ZIP từ GitHub.
C++ compiler `g++` (khuyến nghị dùng MSYS2 UCRT64).
PHP 8.5.x từ file `php.rar` của nhóm.
Không cần dùng Live Server để chạy toàn bộ project.
---
2. Tải project từ GitHub
Repository của nhóm:
```text
https://github.com/PhamQuocDien/261DASA230179\\\_06\\\_Nhom20
```
Cách 1: Clone bằng Git
Mở CMD/PowerShell và chạy:
```bash
git clone https://github.com/PhamQuocDien/261DASA230179\\\_06\\\_Nhom20.git
```
Sau khi tải xong sẽ có thư mục:
```text
261DASA230179\\\_06\\\_Nhom20
```
Cách 2: Download ZIP
Trên GitHub chọn:
```text
Code → Download ZIP
```
Sau đó giải nén ra một thư mục dễ nhớ.
---
3. Mở project bằng Visual Studio Code
Mở VS Code → `File → Open Folder...` → chọn thư mục:
```text
261DASA230179\\\_06\\\_Nhom20
```
Trong Explorer cần thấy các phần chính như:
```text
261DASA230179\\\_06\\\_Nhom20
├── src
├── bin
├── third\\\_party
└── ...
```
---
4. Cài C++ compiler (g++)
Project có C++, vì vậy máy phải có `g++` để build `ThuVien.exe`.
Nếu máy đã cài MSYS2/UCRT64 thì kiểm tra bằng:
```cmd
g++ --version
```
Nếu hiện phiên bản `g++` thì có thể tiếp tục.
Nếu máy chưa có `g++`, cài MSYS2 và sử dụng compiler UCRT64.
> Lưu ý: `php.rar` chỉ cung cấp PHP, không cung cấp compiler C++.
---
5. Cài PHP từ file `php.rar` của nhóm
Nhóm cung cấp file:
```text
php.rar
```
Giải nén file này thành thư mục:
```text
C:\\\\php
```
Sau khi giải nén, kiểm tra trong `C:\\\\php` có ít nhất các file/thư mục PHP cần thiết, đặc biệt là:
```text
C:\\\\php\\\\php.exe
C:\\\\php\\\\php.ini
C:\\\\php\\\\ext\\\\
```
---
6. Thêm PHP vào PATH của Windows
Để có thể gõ lệnh `php` trong CMD/PowerShell, cần thêm:
```text
C:\\\\php
```
vào biến môi trường `Path`.
Các bước
Mở Start và tìm `Environment Variables`.
Chọn `Edit the system environment variables`.
Chọn `Environment Variables...`.
Ở phần `User variables` tìm `Path` → `Edit`.
Chọn `New`.
Nhập:
```text
C:\\\\php
```
Nhấn OK cho đến khi đóng hết cửa sổ.
Đóng các Terminal đang mở và mở Terminal mới.
---
7. Kiểm tra PHP
Mở CMD hoặc PowerShell mới và chạy:
```cmd
php -v
```
Project đã được kiểm tra với PHP 8.5.10.
Nếu lệnh `php -v` không chạy được thì PATH chưa được cấu hình đúng.
---
8. Bật OpenSSL cho PHP
Project cần HTTPS để đọc dữ liệu từ GitHub.
Kiểm tra:
```cmd
php -m | findstr /i openssl
```
Nếu kết quả có:
```text
openssl
```
thì đã OK.
Nếu không có:
Mở file:
```text
C:\\\\php\\\\php.ini
```
Tìm dòng:
```ini
;extension=openssl
```
Bỏ dấu `;` để thành:
```ini
extension=openssl
```
Lưu file.
Đóng Terminal cũ và mở Terminal mới.
Kiểm tra lại:
```cmd
php -m | findstr /i openssl
```
---
9. Kiểm tra `nlohmann/json`
Project đã có thư viện JSON trong project, nên không cần tự tải thêm nếu thư mục sau vẫn có đầy đủ:
```text
third\\\_party\\\\nlohmann\\\\json.hpp
```
Kiểm tra trong thư mục project.
---
10. Build C++ thành `bin\\\\ThuVien.exe`
Mở Terminal tại thư mục gốc project, ví dụ:
```text
C:\\\\Users\\\\<ten>\\\\Downloads\\\\261DASA230179\\\_06\\\_Nhom20
```
Nếu Terminal là PowerShell
Chạy:
```powershell
$files = Get-ChildItem .\\\\src -Recurse -Filter \\\*.cpp | ForEach-Object { $\\\_.FullName }; g++ -std=c++17 -Isrc -Ithird\\\_party $files -o .\\\\bin\\\\ThuVien.exe
```
Sau khi build thành công cần có:
```text
bin
└── ThuVien.exe
```
Kiểm tra:
```powershell
dir .\\\\bin\\\\ThuVien.exe
```
> Không dùng task `C/C++: g++.exe build active file` nếu muốn build toàn bộ project, vì task mặc định đó thường chỉ build file C++ đang mở.
---
11. Cấu hình `GITHUB\\\_TOKEN`
Project có chức năng đọc/sync `library.json` từ GitHub nên môi trường local có thể cần biến môi trường `GITHUB\\\_TOKEN`.
Mỗi thành viên nên tự tạo token của tài khoản GitHub của mình nếu project yêu cầu quyền truy cập.
Với CMD
```cmd
set GITHUB\\\_TOKEN=TOKEN\\\_CUA\\\_BAN
```
Kiểm tra mà không in token:
```cmd
if defined GITHUB\\\_TOKEN (echo GITHUB\\\_TOKEN OK) else (echo GITHUB\\\_TOKEN CHUA DUOC DAT)
```
Với PowerShell
```powershell
$env:GITHUB\\\_TOKEN="TOKEN\\\_CUA\\\_BAN"
```
Kiểm tra:
```powershell
if ($env:GITHUB\\\_TOKEN) { "GITHUB\\\_TOKEN OK" } else { "GITHUB\\\_TOKEN CHUA DUOC DAT" }
```
> \\\*\\\*Không gửi token lên nhóm chat, không commit token vào GitHub, không ghi token trực tiếp vào source code.\\\*\\\*
> Cách `set` hoặc `$env:` chỉ áp dụng cho Terminal hiện tại. Nếu đóng Terminal, cần đặt lại biến trong Terminal mới.
---
12. Chạy PHP server
Đứng tại thư mục gốc của project:
```text
261DASA230179\\\_06\\\_Nhom20
```
Project hiện có:
```text
src\\\\index.php
```
và `src\\\\index.php` chuyển tới:
```text
/presentation/index.html
```
Vì vậy chạy PHP với `src` làm web root:
```cmd
php -S 0.0.0.0:8000 -t src
```
Khi thấy:
```text
PHP 8.5.x Development Server (http://0.0.0.0:8000) started
```
thì server đã chạy.
---
13. Mở website
Mở trình duyệt:
```text
http://localhost:8000/
```
Luồng hoạt động:
```text
http://localhost:8000/
        ↓
src/index.php
        ↓
src/presentation/index.html
        ↓
JavaScript
        ↓
src/api/api.php
        ↓
ThuVien.exe
        ↓
library.json
```
---
14. Không cần dùng Go Live
Nút `Go Live` của Live Server chủ yếu phục vụ frontend HTML/CSS/JS.
Project này còn có PHP API và C++ backend nên để chạy toàn bộ hệ thống hãy dùng:
```cmd
php -S 0.0.0.0:8000 -t src
```
Không cần mở `ThuVien.exe` riêng nếu `api.php` đã được cấu hình để gọi backend C++.
---
15. Khi nào cần build lại C++?
Có thay đổi C++
Nếu sửa các file `.cpp` hoặc `.h` liên quan đến C++ thì build lại:
```powershell
$files = Get-ChildItem .\\\\src -Recurse -Filter \\\*.cpp | ForEach-Object { $\\\_.FullName }; g++ -std=c++17 -Isrc -Ithird\\\_party $files -o .\\\\bin\\\\ThuVien.exe
```
Chỉ sửa frontend/PHP
Nếu chỉ sửa:
```text
.html
.css
.js
.php
```
thì không cần build lại C++.
Chỉ cần refresh website.
---
16. Kiểm tra lỗi khi chạy
Nếu website không load dữ liệu, mở:
```text
F12 → Console
```
và xem lỗi màu đỏ.
Có thể kiểm tra API bằng log của PHP server.
Các lỗi thường gặp:
`php is not recognized`
PHP chưa được thêm vào `Path` hoặc Terminal chưa được mở lại.
`Unable to find the wrapper "https"`
OpenSSL của PHP chưa được bật.
`nlohmann/json.hpp: No such file or directory`
Lệnh build thiếu:
```text
-Ithird\\\_party
```
hoặc thư mục `third\\\_party\\\\nlohmann\\\\json.hpp` không tồn tại.
`The requested resource "/" was not found`
Bạn đang chạy PHP ở sai web root. Hãy chạy từ thư mục gốc project:
```cmd
php -S 0.0.0.0:8000 -t src
```
`API khong tra ve JSON`
Mở Console xem response thật của `api.php`. Warning PHP hoặc lỗi GitHub có thể làm response không còn là JSON thuần.
---
17. Quy trình chạy project mỗi lần
Lần đầu trên máy mới
```text
1. Tải project từ GitHub
2. Mở project bằng VS Code
3. Cài g++/MSYS2
4. Giải nén php.rar → C:\\\\php
5. Thêm C:\\\\php vào PATH
6. Bật OpenSSL
7. Kiểm tra php -v
8. Cấu hình GITHUB\\\_TOKEN nếu project yêu cầu
9. Build → bin\\\\ThuVien.exe
10. Chạy PHP server
11. Mở http://localhost:8000/
```
Những lần sau
Nếu không thay đổi môi trường:
```text
1. Mở project
2. Build lại nếu có thay đổi C++
3. Chạy:
   php -S 0.0.0.0:8000 -t src
4. Mở:
   http://localhost:8000/
```
---
18. Cấu trúc chạy mong muốn
```text
261DASA230179\\\_06\\\_Nhom20
│
├── src
│   ├── index.php
│   ├── api
│   ├── presentation
│   ├── dsa\\\_core
│   └── persistence
│
├── third\\\_party
│   └── nlohmann
│       └── json.hpp
│
├── bin
│   └── ThuVien.exe
│
└── ...
```
---
19. Lưu ý bảo mật
Không chia sẻ `GITHUB\\\_TOKEN` cho người khác.
Không commit token vào GitHub.
Không ghi token trực tiếp trong `api.php` hoặc JavaScript.
Nếu token bị lộ, hãy thu hồi token đó trên GitHub và tạo token mới.
---
20. Tóm tắt lệnh quan trọng
Kiểm tra g++
```cmd
g++ --version
```
Kiểm tra PHP
```cmd
php -v
```
Kiểm tra OpenSSL
```cmd
php -m | findstr /i openssl
```
Build C++
```powershell
$files = Get-ChildItem .\\\\src -Recurse -Filter \\\*.cpp | ForEach-Object { $\\\_.FullName }; g++ -std=c++17 -Isrc -Ithird\\\_party $files -o .\\\\bin\\\\ThuVien.exe
```
Chạy PHP
```cmd
php -S 0.0.0.0:8000 -t src
```
Website
```text
http://localhost:8000/
```
## 2. Cấu trúc project

```text
261DASA230179_06_Nhom20/
│
├── docs/
│
├── src/
│   ├── index.php
│   ├── main.cpp
│   │
│   ├── api/
│   │   └── api.php
│   │
│   ├── presentation/
│   │   ├── index.html
│   │   ├── style.css
│   │   ├── script.js
│   │   │
│   │   ├── components/
│   │   │   ├── header.html
│   │   │   ├── sidebar.html
│   │   │   ├── home-tabs.html
│   │   │   ├── all-books.html
│   │   │   ├── trending-books.html
│   │   │   ├── book-management.html
│   │   │   ├── search-book.html
│   │   │   ├── search-book-by-year.html
│   │   │   ├── borrow-book.html
│   │   │   ├── reservation.html
│   │   │   ├── return-book.html
│   │   │   ├── loan-slip.html
│   │   │   └── fine.html
│   │   │
│   │   └── js/
│   │       ├── componentLoader.js
│   │       ├── navigation.js
│   │       ├── api.js
│   │       ├── book.js
│   │       └── loanSlip.js
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
│   │   │   ├── BookRepository.cpp
│   │   │   ├── MemberRepository.h
│   │   │   ├── MemberRepository.cpp
│   │   │   ├── LoanRepository.h
│   │   │   ├── LoanRepository.cpp
│   │   │   ├── ReservationRepository.h
│   │   │   └── FineRepository.h
│   │   │
│   │   └── services/
│   │       ├── BookService.h
│   │       ├── BookService.cpp
│   │       ├── MemberService.h
│   │       ├── LoanService.h
│   │       ├── ReservationService.h
│   │       ├── FineService.h
│   │       └── LoanSlipService.h
│   │
│   └── persistence/
│       ├── JsonDatabase.h
│       ├── JsonDatabase.cpp
│       ├── JsonMapper.h
│       └── JsonMapper.cpp
│
├── data/
│   └── library.json
│
├── README.md
└── .gitignore
## 1. Cấu Trúc Thư Mục Và Giải Thích File

Dự án được chia thành các module độc lập, đảm bảo tính đóng gói và dễ bảo trì.

### 1.1. Tầng Dữ Liệu Lưu Trữ (Persistence Layer)

Tầng này chịu trách nhiệm giao tiếp với hệ thống file, chuyển đổi qua lại giữa Object trong C++ và định dạng JSON.

| **Tên File**         | **Chức Năng Cốt Lõi**                                                                                                                                                                    |
| -------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `JsonDatabase.h/cpp` | Cung cấp cơ chế đọc/ghi file `library.json` an toàn thông qua khối `try-catch`. Đóng vai trò như một Database Engine thao tác trực tiếp lên ổ đĩa.                                       |
| `JsonMapper.h/cpp`   | Lớp chuyển đổi (Serialization/Deserialization). Chứa các hàm ánh xạ dữ liệu từ Cấu trúc C++ (như `Book`, `Member`, `Loan`) sang JSON (để lưu trữ/trả API) và ngược lại (để nạp vào RAM). |

### 1.2. Tầng Cấu Trúc Dữ Liệu Cốt Lõi (DSA Core)

Nơi chứa các cấu trúc dữ liệu nền tảng tự xây dựng để phục vụ cho các logic nghiệp vụ phức tạp.

| **Tên File**  | **Chức Năng Cốt Lõi**                                                                                                                                                                                                             |
| ------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `HashTable.h` | Cấu trúc Bảng băm (Hash Table) sử dụng kỹ thuật Chaining (mảng các danh sách liên kết) để giải quyết đụng độ (collision). Hỗ trợ các thao tác `insert`, `find`, `remove`, `contains` với độ phức tạp thời gian trung bình $O(1)$. |

### 1.3. Tầng Lớp Thực Thể (Models)

Định nghĩa các khuôn mẫu dữ liệu (Data Objects) phản ánh thực tế trong thư viện.

*(Ghi chú: Nội dung file dựa trên các hàm`JsonMapper`và Service)*

| **Tên File**    | **Chức Năng Cốt Lõi**                                                                                                 |
| --------------- | --------------------------------------------------------------------------------------------------------------------- |
| `Book.h`        | Quản lý thông tin đầu sách (Mã sách, tên, tác giả) và danh sách các cuốn sách vật lý (Book Copies) thuộc đầu sách đó. |
| `Member.h`      | Thông tin người dùng thư viện (Mã, tên, email, sđt, trạng thái thẻ).                                                  |
| `Loan.h`        | Lưu trữ thông tin một phiên mượn sách cơ bản (Ngày mượn, hạn trả, ngày trả thực tế, trạng thái).                      |
| `Reservation.h` | Lưu trữ lịch sử/thông tin đặt trước sách của thành viên.                                                              |
| `Fine.h`        | Lưu trữ thông tin phạt (do trễ hạn, làm hỏng sách).                                                                   |

### 1.4. Tầng Kho Dữ Liệu (Repositories)

Tầng này đóng vai trò như bộ nhớ đệm (In-memory Database). Khi chương trình khởi chạy, toàn bộ dữ liệu từ JSON được nạp vào đây.

| **Tên File**                  | **Chức Năng Cốt Lõi**                                                                                       |
| ----------------------------- | ----------------------------------------------------------------------------------------------------------- |
| `BookRepository.h/cpp`        | Cung cấp các thao tác CRUD (Thêm, Sửa, Xóa, Lấy danh sách, Tìm theo ID) trực tiếp trên mảng vector `books`. |
| `MemberRepository.h/cpp`      | Cung cấp các thao tác CRUD trên mảng vector `members`.                                                      |
| `LoanRepository.h/cpp`        | Cung cấp các thao tác CRUD trên mảng vector `loans`.                                                        |
| `ReservationRepository.h/cpp` | Cung cấp các thao tác CRUD trên mảng vector `reservations`.                                                 |
| `FineRepository.h/cpp`        | Cung cấp các thao tác CRUD trên mảng vector `fines`.                                                        |

### 1.5. Tầng Dịch Vụ Nghiệp Vụ (Services)

Nơi chứa toàn bộ Logic tính toán, liên kết nhiều bảng và xử lý quy trình kinh doanh của thư viện.

| **Tên File**            | **Chức Năng Cốt Lõi**                                                                                                                                                                                                                                                                       |
| ----------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `BookService.h/cpp`     | Xử lý logic nghiệp vụ về Sách. Nổi bật nhất là hàm `addBook`: Khi nhận yêu cầu thêm 1 đầu sách mới cùng số lượng (VD: 5 cuốn), Service sẽ tự động sinh ra 5 ID sách vật lý riêng biệt (hậu tố `001` đến `005`) trước khi đưa xuống Repository để lưu.                                       |
| `LoanSlipService.h/cpp` | Dịch vụ tổng hợp Phiếu Mượn. File này kết nối dữ liệu từ 3 Repository (Loan, Member, Book) để tạo ra `LoanSlip` hoàn chỉnh (chứa đầy đủ tên người mượn, tên sách). Đồng thời, **áp dụng HashTable** để lập chỉ mục (Index) các khoản mượn theo `Member_ID`, giúp tốc độ truy vấn cực nhanh. |
| `MergeSort.h`           | Chứa thuật toán sắp xếp. Cụ thể là hàm `sortBooksByYear` dùng để sắp xếp danh sách các cuốn sách theo năm xuất bản.                                                                                                                                                                         |

### 1.6. File Khởi Chạy (Entry Point)

| **Tên File** | **Chức Năng Cốt Lõi**                                                                                                                                                                                                                  |
| ------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `main.cpp`   | Trái tim của ứng dụng. Khởi tạo Database, load dữ liệu nạp vào Repository, khởi tạo các Service. Phân luồng chạy chương trình thành 2 chế độ: API (nhận JSON/trả JSON qua luồng I/O) hoặc Console (Giao diện CLI tương tác trực tiếp). |

## 2. Giải Thích Luồng Nghiệp Vụ Chi Tiết

Hệ thống không thao tác dữ liệu một cách trực tiếp mà tuân thủ luồng quy trình nghiêm ngặt: **Request -> Controller (main) -> Service -> Repository -> Persistence**.

### 2.1. Nghiệp Vụ Quản Lý Sách (Book Service Logic)

1. **Thêm Sách Mới (`createBook`):**
   - Hệ thống không chỉ lưu thông tin chung (Title, Author) mà còn yêu cầu nhập `quantity` (số lượng cuốn vật lý).
   - `BookService` tự động lặp vòng `for` để sinh mã ID duy nhất cho từng cuốn sách (ví dụ mã đầu sách là `B01`, sinh ra 3 cuốn sẽ có ID: `B01001`, `B01002`, `B01003`).
   - Sau khi lưu vào biến RAM (`BookRepository`), hệ thống gọi ngay `JsonDatabase` để cập nhật xuống file cứng `library.json`. Đảm bảo dữ liệu không bị mất khi tắt app.

### 2.2. Nghiệp Vụ Phiếu Mượn Và Tối Ưu Hóa (Loan Slip Service Logic)

Đây là nghiệp vụ phức tạp nhất nhằm giải quyết bài toán: *"Làm sao để tìm kiếm toàn bộ lịch sử mượn sách của một người dùng cụ thể với tốc độ nhanh nhất?"*

1. **Lập chỉ mục bằng Bảng băm (Hash Table):**
   - Thay vì mỗi lần tìm kiếm phải quét toàn bộ danh sách hàng ngàn lượt mượn (độ phức tạp $O(n)$), `LoanSlipService` khởi tạo một Bảng băm `memberLoanIndex`.
   - Bảng băm này map `Member_ID` (Khóa - Key) với một `vector<string>` chứa danh sách các `Loan_ID` (Giá trị - Value).
   - Việc lập chỉ mục diễn ra một lần duy nhất lúc khởi động (`buildMemberLoanIndex()`).
2. **Truy xuất Phiếu Mượn (Loan Slip):**
   - Khi có yêu cầu tra cứu từ người dùng, hệ thống truyền `memberId` vào Hash Table. Ngay lập tức ($O(1)$), hệ thống lấy được danh sách `Loan_ID`.
   - Từ các `Loan_ID` này, hàm `createLoanSlip()` sẽ đi gom thông tin Tên sách (từ `BookRepository`) và Tên người dùng (từ `MemberRepository`) để đúc thành một Phiếu mượn `LoanSlip` hoàn chỉnh mang ý nghĩa thực tế để hiển thị ra màn hình hoặc trả về JSON.
   - Các phiếu mượn được sắp xếp theo thời gian (`borrowDate`) tăng dần trước khi trả về.

### 2.3. Hai Chế Độ Hoạt Động Của Hệ Thống

Chương trình được thiết kế linh hoạt cho hai môi trường:

| **Đặc điểm**          | **Giao Diện Dòng Lệnh (Console Mode)**                   | **Giao Diện API (API Mode)**                          |
| --------------------- | -------------------------------------------------------- | ----------------------------------------------------- |
| **Cách kích hoạt**    | Chạy file thực thi trực tiếp (VD: `./library`)           | Chạy với tham số `--api` (VD: `./library --api`)      |
| **Giao tiếp đầu vào** | Người dùng gõ trực tiếp từ bàn phím                      | Hệ thống khác gửi chuỗi JSON thô qua `stdin`          |
| **Đầu ra hiển thị**   | Các đoạn văn bản có khung viền đẹp mắt (Phieu Muon Sach) | Chuỗi JSON chuẩn hóa qua `stdout`                     |
| **Mục đích sử dụng**  | Nhân viên thư viện dùng trực tiếp                        | Dùng làm Backend Core kết nối với Frontend Web/Mobile |

## 3. Tổng Hợp Các Chức Năng Chính (API Endpoints)

Nếu hệ thống chạy ở chế độ API, nó xử lý các `action` sau từ cục JSON request:

1. **`getBooks`**: Lấy toàn bộ kho sách của thư viện.
2. **`getBook`**: Tìm và trả về chi tiết một đầu sách kèm các mã cuốn vật lý.
3. `createBook`: Nhập đầu sách mới và khởi tạo số lượng cuốn vật lý.
4. **`updateBook`**: Cập nhật thông tin tiêu đề, tác giả... của đầu sách đang có.
5. **`deleteBook`**: Xóa hoàn toàn một đầu sách khỏi cơ sở dữ liệu.
6. **`getLoanSlipsByMember`**: Trích xuất lịch sử mượn sách (các phiếu mượn) của một thành viên bất kỳ.
7. **`getLoanSlipByLoanId`**: In chi tiết nội dung của một mã phiếu mượn cụ thể.
