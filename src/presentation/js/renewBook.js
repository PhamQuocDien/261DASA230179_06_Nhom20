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
    const resultBox = document.querySelector("#renewBookResult");

    // Kiểm tra giao diện
    if (!button || !input || !resultBox) {
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

        // Kiểm tra input
        if (!loanId) {
            resultBox.innerHTML = `
                <p style="color: red;">
                    Vui lòng nhập Loan_ID.
                </p>
            `;
            input.focus();
            return;
        }

        // Khóa nút trong lúc xử lý
        button.disabled = true;
        resultBox.innerHTML = `<p>Đang xử lý...</p>`;

        try {
            const response = await sendApiRequest({
                action: "renewBook",
                loanId: loanId
            });

            // API trả về lỗi
            if (!response.success) {
                resultBox.innerHTML = `
                    <p style="color: red;">
                        ${escapeHtml(
                            response.error ||
                            response.message ||
                            "Không thể gia hạn."
                        )}
                    </p>
                `;
                return;
            }

            // Hiển thị kết quả thành công
            const data = response.data || {};

            resultBox.innerHTML = `
                <div style="
                    padding: 15px;
                    border: 1px solid #4ade80;
                    border-radius: 8px;
                    background: rgba(34, 197, 94, 0.1);
                ">
                    <p style="
                        color: #4ade80;
                        margin-top: 0;
                    ">
                        <b>${escapeHtml(response.message)}</b>
                    </p>

                    <p style="margin-bottom: 5px;">
                        <b>Loan_ID:</b>
                        ${escapeHtml(data.loanId)}
                    </p>

                    <p style="margin-bottom: 5px;">
                        <b>Member_ID:</b>
                        ${escapeHtml(data.memberId)}
                    </p>

                    <p style="margin-bottom: 5px;">
                        <b>Book_ID:</b>
                        ${escapeHtml(data.bookId)}
                    </p>

                    <p style="margin-bottom: 5px;">
                        <b>Ngày mượn:</b>
                        ${escapeHtml(data.borrowDate)}
                    </p>

                    <p style="margin-bottom: 5px;">
                        <b>Hạn trả mới:</b>
                        ${escapeHtml(data.dueDate)}
                    </p>

                    <p style="margin-bottom: 0;">
                        <b>Số lần gia hạn:</b>
                        ${escapeHtml(data.renewalCount)}
                    </p>
                </div>
            `;

            // Xóa ô input sau khi thành công
            input.value = "";
        } catch (error) {
            console.error("RenewBook error:", error);
            resultBox.innerHTML = `
                <p style="color: red;">
                    Không thể kết nối đến hệ thống.
                </p>
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
}
