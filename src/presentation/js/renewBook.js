// =====================================
// RENEW BOOK MODULE
// GIA HẠN MƯỢN SÁCH
// =====================================

import { sendApiRequest } from "./api.js";

function escapeHtml(value) {
    return String(value ?? "")
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}

export function initRenewBook() {
    const button = document.querySelector("#btnRenewBook");
    const input = document.querySelector("#renewLoanId");
    const passwordInput = document.querySelector("#renewPassword");
    const resultBox = document.querySelector("#renewBookResult");

    // Kiểm tra giao diện
    if (!button || !input || !passwordInput || !resultBox) {
        console.warn("RenewBook: Không tìm thấy giao diện gia hạn.");
        return;
    }

    // Tránh đăng ký sự kiện nhiều lần
    if (button.dataset.initialized === "true") {
        return;
    }
    button.dataset.initialized = "true";

    const handleRenew = async () => {
        const loanId = input.value.trim();
        const password = passwordInput.value;

        // Kiểm tra input
        if (!loanId || !password) {
            resultBox.innerHTML = `
                <div class="alert alert-error">
                    Vui lòng nhập Loan_ID và mật khẩu thành viên.
                </div>
            `;
            input.focus();
            return;
        }

        // Khóa nút trong lúc xử lý
        button.disabled = true;
        resultBox.innerHTML = `
            <div class="loading-state">
                <span class="loading-spinner"></span>
                <p>Đang xử lý...</p>
            </div>
        `;

        try {
            const response = await sendApiRequest({
                action: "renewBook",
                loanId: loanId,
                password: password
            });

            // API trả về lỗi
            if (!response.success) {
                resultBox.innerHTML = `
                    <div class="alert alert-error">
                        ${escapeHtml(
                            response.error ||
                            response.message ||
                            "Không thể gia hạn."
                        )}
                    </div>
                `;
                return;
            }

            // Hiển thị kết quả thành công
            const data = response.data || {};

            resultBox.innerHTML = `
                <div class="alert alert-success">
                    <p class="alert-title">${escapeHtml(response.message)}</p>

                    <p class="alert-line">
                        <strong>Loan_ID:</strong>
                        ${escapeHtml(data.loanId)}
                    </p>

                    <p class="alert-line">
                        <strong>Member_ID:</strong>
                        ${escapeHtml(data.memberId)}
                    </p>

                    <p class="alert-line">
                        <strong>Book_ID:</strong>
                        ${escapeHtml(data.bookId)}
                    </p>

                    <p class="alert-line">
                        <strong>Ngày mượn:</strong>
                        ${escapeHtml(data.borrowDate)}
                    </p>

                    <p class="alert-line">
                        <strong>Hạn trả mới:</strong>
                        ${escapeHtml(data.dueDate)}
                    </p>

                    <p class="alert-line">
                        <strong>Số lần gia hạn:</strong>
                        ${escapeHtml(data.renewalCount)}
                    </p>
                </div>
            `;

            // Xóa ô input sau khi thành công
            input.value = "";
            passwordInput.value = "";
        } catch (error) {
            console.error("RenewBook error:", error);
            resultBox.innerHTML = `
                <div class="alert alert-error">
                    Không thể kết nối đến hệ thống.
                </div>
            `;
        } finally {
            button.disabled = false;
        }
    };

    // Bấm nút Gia hạn
    button.addEventListener("click", handleRenew);

    // Nhấn Enter trong ô input
    input.addEventListener("keydown", event => {
        if (event.key === "Enter") {
            handleRenew();
        }
    });

    // Nhấn Enter trong ô mật khẩu
    passwordInput.addEventListener("keydown", event => {
        if (event.key === "Enter") {
            handleRenew();
        }
    });
}
