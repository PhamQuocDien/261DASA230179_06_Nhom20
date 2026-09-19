import { sendApiRequest } from "./api.js";
let currentBookCopies = [];
function getBookElements() {
    return {
        bookCodeInput: document.querySelector("#bookCode"),
        titleInput: document.querySelector("#title"),
        authorInput: document.querySelector("#author"),
        categoryInput: document.querySelector("#category"),
        yearInput: document.querySelector("#year"),
        quantityInput: document.querySelector("#quantity"),
        btnCreate: document.querySelector("#btnCreate"),
        btnRead: document.querySelector("#btnRead"),
        btnUpdate: document.querySelector("#btnUpdate"),
        btnDelete: document.querySelector("#btnDelete"),
        btnReset: document.querySelector("#btnReset"),
        bookTableBody: document.querySelector("#bookTableBody")
    };
}
function clearForm(elements) {
    elements.bookCodeInput.value = "";
    elements.titleInput.value = "";
    elements.authorInput.value = "";
    elements.categoryInput.value = "";
    elements.yearInput.value = "";
    elements.quantityInput.value = "";
    currentBookCopies = [];
    elements.bookCodeInput.focus();
}
function setBookFormData(elements, book) {
    if (!book) {
        return;
    }
    elements.bookCodeInput.value = book.bookCode ?? "";
    elements.titleInput.value = book.title ?? "";
    elements.authorInput.value = book.author ?? "";
    elements.categoryInput.value = book.category ?? "";
    elements.yearInput.value = book.year ?? "";
    currentBookCopies = Array.isArray(book.copies) ? book.copies : [];
    elements.quantityInput.value = currentBookCopies.length;
}
function getBookFormData(elements, copies = currentBookCopies) {
    const bookCode = elements.bookCodeInput.value.trim();
    const title = elements.titleInput.value.trim();
    const author = elements.authorInput.value.trim();
    const category = elements.categoryInput.value.trim();
    const yearText = elements.yearInput.value.trim();
    let year = null;
    if (yearText !== "") {
        year = Number(yearText);
    }
    return { bookCode: bookCode, title: title, author: author, category: category, year: year, copies: copies };
}
function validateBook(book) {
    if (book.bookCode === "") {
        console.warn("Vui long nhap ma sach.");
        return false;
    }
    if (book.title === "") {
        console.warn("Vui long nhap ten sach.");
        return false;
    }
    if (book.author === "") {
        console.warn("Vui long nhap tac gia.");
        return false;
    }
    if (book.category === "") {
        console.warn("Vui long nhap the loai.");
        return false;
    }
    if (book.year === null || Number.isNaN(book.year)) {
        console.warn("Vui long nhap nam xuat ban hop le.");
        return false;
    }
    if (book.year < 0) {
        console.warn("Nam xuat ban khong duoc am.");
        return false;
    }
    return true;
}
async function getBooks() {
    const result = await sendApiRequest({ action: "getBooks" });
    if (!result.success) {
        console.error("Khong the lay danh sach Book:", result.error);
        return null;
    }
    console.log("Danh sach Book:", result.data);
    return result.data;
}
async function getBookByCode(elements, bookCode) {
    if (!bookCode) {
        console.warn("Vui long nhap ma sach.");
        showBookMessage("❌ Vui lòng nhập mã sách.", "error");
        return null;
    }
    const result = await sendApiRequest({ action: "getBook", bookCode: bookCode });
    if (!result.success) {
        console.error("Khong tim thay Book:", result.error);
        showBookMessage("❌ Tìm sách thất bại: " + (result.error ?? "Không tìm thấy sách."), "error");
        return null;
    }
    console.log("Book tim thay:", result.data);
    setBookFormData(elements, result.data);
    showBookMessage("✅ Tìm thấy sách.", "success");
    return result.data;
}
async function createBook(book, quantity) {
    const result = await sendApiRequest({ action: "createBook", book: book, quantity: quantity });
    if (!result.success) {
        console.error("Create Book that bai:", result.error);
        showBookMessage("❌ Thêm sách thất bại: " + (result.error ?? "Lỗi không xác định."), "error");
        return null;
    }
    console.log("Create Book thanh cong:", result.data);
    const copies = Array.isArray(result.data?.copies) ? result.data.copies : [];
    showBookMessage("✅ Thêm đầu sách thành công! Đã tạo " + copies.length + " cuốn vật lý.", "success");
    return result.data;
}
async function updateBook(book, quantity) {
    const result = await sendApiRequest({ action: "updateBook", book: book, quantity: quantity });
    if (!result.success) {
        console.error("Update Book that bai:", result.error);
        showBookMessage("❌ Cập nhật sách thất bại: " + (result.error ?? "Lỗi không xác định."), "error");
        return null;
    }
    console.log("Update Book thanh cong:", result.data);
    currentBookCopies = Array.isArray(result.data.copies) ? result.data.copies : currentBookCopies;
    showBookMessage("✅ Cập nhật sách thành công!", "success");
    return result.data;
}
async function deleteBook(bookCode) {
    if (!bookCode) {
        console.warn("Vui long nhap ma sach.");
        showBookMessage("❌ Vui lòng nhập mã sách.", "error");
        return null;
    }
    const result = await sendApiRequest({ action: "deleteBook", bookCode: bookCode });
    if (!result.success) {
        console.error("Delete Book that bai:", result.error);
        showBookMessage("❌ Xóa sách thất bại: " + (result.error ?? "Lỗi không xác định."), "error");
        return null;
    }
    console.log("Delete Book thanh cong:", result.data);
    showBookMessage("✅ Xóa sách thành công!", "success");
    return result.data;
}
function renderBooks(elements, books) {
    elements.bookTableBody.innerHTML = "";
    if (!Array.isArray(books)) {
        console.warn("Du lieu Book khong phai Array.");
        const row = document.createElement("tr");
        const cell = document.createElement("td");
        cell.colSpan = 5;
        cell.textContent = "Du lieu sach khong hop le.";
        row.appendChild(cell);
        elements.bookTableBody.appendChild(row);
        return;
    }
    if (books.length === 0) {
        const row = document.createElement("tr");
        const cell = document.createElement("td");
        cell.colSpan = 5;
        cell.textContent = "Chua co sach nao.";
        row.appendChild(cell);
        elements.bookTableBody.appendChild(row);
        return;
    }
    books.forEach(book => {
        const row = document.createElement("tr");
        const bookCodeCell = document.createElement("td");
        const titleCell = document.createElement("td");
        const authorCell = document.createElement("td");
        const categoryCell = document.createElement("td");
        const yearCell = document.createElement("td");
        bookCodeCell.textContent = book.bookCode ?? "";
        titleCell.textContent = book.title ?? "";
        authorCell.textContent = book.author ?? "";
        categoryCell.textContent = book.category ?? "";
        yearCell.textContent = book.year ?? "";
        row.appendChild(bookCodeCell);
        row.appendChild(titleCell);
        row.appendChild(authorCell);
        row.appendChild(categoryCell);
        row.appendChild(yearCell);
        elements.bookTableBody.appendChild(row);
    });
    console.log("Da hien thi", books.length, "Book len bang.");
}
function showBookMessage(message, type) {
    const element = document.querySelector("#bookMessage");
    if (!element) {
        return;
    }
    element.textContent = message;
    element.className = "book-message " + type;
}
function clearBookMessage() {
    const element = document.querySelector("#bookMessage");
    if (!element) {
        return;
    }
    element.textContent = "";
    element.className = "book-message";
}
async function loadBooks(elements) {
    console.log("=== LOAD BOOK LIST ===");
    const books = await getBooks();
    if (books === null) {
        console.warn("Khong the tai danh sach Book.");
        return;
    }
    renderBooks(elements, books);
}
export function initBook() {
    console.log("=== INIT BOOK MODULE ===");
    const elements = getBookElements();
    if (!elements.bookCodeInput || !elements.titleInput || !elements.authorInput || !elements.categoryInput || !elements.yearInput || !elements.quantityInput || !elements.btnCreate || !elements.btnRead || !elements.btnUpdate || !elements.btnDelete || !elements.btnReset || !elements.bookTableBody) {
        console.error("Khong tim thay day du HTML cua Book.");
        return;
    }
    elements.btnCreate.addEventListener("click", async () => {
        const book = getBookFormData(elements, []);
        const quantityText = elements.quantityInput.value.trim();
        const quantity = Number(quantityText);
        console.log("=== CREATE BOOK ===");
        console.log("Du lieu Book:", book);
        console.log("So luong cuon vat ly:", quantity);
        if (!validateBook(book)) {
            showBookMessage("❌ Dữ liệu sách không hợp lệ.", "error");
            return;
        }
        if (quantityText === "" || !Number.isInteger(quantity) || quantity <= 0) {
            showBookMessage("❌ Số lượng cuốn vật lý phải là số nguyên lớn hơn 0.", "error");
            return;
        }
        if (quantity > 999) {
            showBookMessage("❌ Số lượng cuốn vật lý không được vượt quá 999.", "error");
            return;
        }
        const result = await createBook(book, quantity);
        if (result !== null) {
            clearForm(elements);
            await loadBooks(elements);
        }
    });
    elements.btnRead.addEventListener("click", async () => {
        const bookCode = elements.bookCodeInput.value.trim();
        console.log("=== READ BOOK ===");
        if (bookCode === "") {
            console.warn("Vui long nhap ma sach.");
            showBookMessage("❌ Vui lòng nhập mã sách.", "error");
            return;
        }
        await getBookByCode(elements, bookCode);
    });
    elements.btnUpdate.addEventListener("click", async () => {
        const book = getBookFormData(elements);
        const quantityText = elements.quantityInput.value.trim();
        const quantity = Number(quantityText);
        console.log("=== UPDATE BOOK ===");
        console.log("So luong hien tai:", currentBookCopies.length);
        console.log("So luong sau khi sua:", quantity);
        if (!validateBook(book)) {
            showBookMessage("❌ Dữ liệu sách không hợp lệ.", "error");
            return;
        }
        if (quantityText === "" || !Number.isInteger(quantity) || quantity <= 0) {
            showBookMessage("❌ Số lượng cuốn vật lý phải là số nguyên lớn hơn 0.", "error");
            return;
        }
        if (quantity > 999) {
            showBookMessage("❌ Số lượng cuốn vật lý không được vượt quá 999.", "error");
            return;
        }
        if (currentBookCopies.length > 0 && quantity < currentBookCopies.length) {
            showBookMessage("❌ Không thể giảm số lượng trong phần Sửa. Vui lòng dùng chức năng Xóa.", "error");
            return;
        }
        const result = await updateBook(book, quantity);
        if (result !== null) {
            setBookFormData(elements, result);
            await loadBooks(elements);
        }
    });
    elements.btnDelete.addEventListener("click", async () => {
        const bookCode = elements.bookCodeInput.value.trim();
        console.log("=== DELETE BOOK ===");
        if (bookCode === "") {
            console.warn("Vui long nhap ma sach.");
            showBookMessage("❌ Vui lòng nhập mã sách.", "error");
            return;
        }
        const result = await deleteBook(bookCode);
        if (result !== null) {
            clearForm(elements);
            await loadBooks(elements);
        }
    });
    elements.btnReset.addEventListener("click", () => {
        console.log("=== RESET FORM ===");
        clearForm(elements);
        clearBookMessage();
    });
    loadBooks(elements);
    console.log("Book module da san sang.");
}
