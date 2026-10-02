import { sendApiRequest } from "./api.js";

function escapeHtml(value) {
    return String(value ?? "")
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}

export function initBorrowBook() {
    const btn = document.querySelector("#btnBorrowBook");
    const memberInput = document.querySelector("#borrowMemberId");
    const bookInput = document.querySelector("#borrowBookId");
    const passwordInput = document.querySelector("#borrowPassword");
    const resultBox = document.querySelector("#borrowBookResult");

    // Kiểm tra giao diện
    if (!btn || !memberInput || !bookInput || !passwordInput || !resultBox) {
        console.warn("BorrowBook: Không tìm thấy giao diện mượn sách.");
        return;
    }

    // Tránh đăng ký sự kiện nhiều lần
    if (btn.dataset.initialized === "true") {
        return;
    }

    btn.dataset.initialized = "true";

    const handleBorrow = async () => {
        const memberId = memberInput.value.trim();
        const bookCode = bookInput.value.trim();
        const password = passwordInput.value;

        // Kiểm tra dữ liệu nhập
        if (!memberId || !bookCode || !password) {
            resultBox.innerHTML = `
                <div class="alert alert-error">
                    Vui lòng nhập đủ Member_ID, BookCode và mật khẩu thành viên.
                </div>
            `;
            return;
        }

        // Khóa nút trong lúc xử lý
        btn.disabled = true;

        resultBox.innerHTML = `
            <div class="loading-state">
                <span class="loading-spinner"></span>
                <p>Đang xử lý...</p>
            </div>
        `;

        try {
            const response = await sendApiRequest({
                action: "borrowBook",
                memberId: memberId,
                bookCode: bookCode,
                password: password
            });

            // API trả về lỗi
            if (!response.success) {
                resultBox.innerHTML = `
                    <div class="alert alert-error">
                        ${escapeHtml(
                            response.error ||
                            response.message ||
                            "Lỗi khi mượn sách."
                        )}
                    </div>
                `;
                return;
            }

            // Dữ liệu phiếu mượn
            const loan = response.data || {};

            resultBox.innerHTML = `
                <div class="alert alert-success">
                    <p class="alert-title">${escapeHtml(
                        response.message ||
                        "Mượn sách thành công!"
                    )}</p>

                    <p class="alert-line">
                        <strong>Mã biên lai:</strong>
                        ${escapeHtml(loan.loanId)}
                    </p>

                    <p class="alert-line">
                        <strong>Mã độc giả:</strong>
                        ${escapeHtml(loan.memberId)}
                    </p>

                    <p class="alert-line">
                        <strong>Mã cuốn vật lý:</strong>
                        ${escapeHtml(loan.bookId)}
                    </p>

                    <p class="alert-line">
                        <strong>Ngày mượn:</strong>
                        ${escapeHtml(loan.borrowDate)}
                    </p>

                    <p class="alert-line">
                        <strong>Hạn trả:</strong>
                        ${escapeHtml(loan.dueDate)}
                    </p>
                </div>
            `;

            // Xóa mã sách sau khi mượn thành công
            bookInput.value = "";
            passwordInput.value = "";

            // Đưa con trỏ về ô nhập mã sách
            bookInput.focus();

        } catch (error) {
            console.error("BorrowBook error:", error);

            resultBox.innerHTML = `
                <div class="alert alert-error">
                    Không thể kết nối đến hệ thống C++.
                </div>
            `;

        } finally {
            // Mở lại nút
            btn.disabled = false;
        }
    };

    // Bấm nút Mượn sách
    btn.addEventListener("click", handleBorrow);

    // Nhấn Enter ở ô mật khẩu
    passwordInput.addEventListener("keydown", event => {
        if (event.key === "Enter") {
            handleBorrow();
        }
    });

    // Nhấn Enter ở ô mã sách
    bookInput.addEventListener("keydown", event => {
        if (event.key === "Enter") {
            handleBorrow();
        }
    });
}
