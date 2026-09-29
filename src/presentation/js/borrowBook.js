```js
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
    const resultBox = document.querySelector("#borrowBookResult");

    // Kiểm tra giao diện
    if (!btn || !memberInput || !bookInput || !resultBox) {
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

        // Kiểm tra dữ liệu nhập
        if (!memberId || !bookCode) {
            resultBox.innerHTML = `
                <p style="color: red;">
                    Vui lòng nhập đủ thông tin Member_ID và BookCode.
                </p>
            `;
            return;
        }

        // Khóa nút trong lúc xử lý
        btn.disabled = true;

        resultBox.innerHTML = `
            <p>Đang xử lý...</p>
        `;

        try {
            const response = await sendApiRequest({
                action: "borrowBook",
                memberId: memberId,
                bookCode: bookCode
            });

            // API trả về lỗi
            if (!response.success) {
                resultBox.innerHTML = `
                    <p style="color: red;">
                        ${escapeHtml(
                            response.error ||
                            response.message ||
                            "Lỗi khi mượn sách."
                        )}
                    </p>
                `;
                return;
            }

            // Dữ liệu phiếu mượn
            const loan = response.data || {};

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
                        <b>
                            ${escapeHtml(
                                response.message ||
                                "Mượn sách thành công!"
                            )}
                        </b>
                    </p>

                    <p style="margin-bottom: 5px;">
                        <b>Mã biên lai:</b>
                        ${escapeHtml(loan.loanId)}
                    </p>

                    <p style="margin-bottom: 5px;">
                        <b>Mã độc giả:</b>
                        ${escapeHtml(loan.memberId)}
                    </p>

                    <p style="margin-bottom: 5px;">
                        <b>Mã cuốn vật lý:</b>
                        ${escapeHtml(loan.bookId)}
                    </p>

                    <p style="margin-bottom: 5px;">
                        <b>Ngày mượn:</b>
                        ${escapeHtml(loan.borrowDate)}
                    </p>

                    <p style="margin-bottom: 0;">
                        <b>Hạn trả:</b>
                        ${escapeHtml(loan.dueDate)}
                    </p>
                </div>
            `;

            // Xóa mã sách sau khi mượn thành công
            bookInput.value = "";

            // Đưa con trỏ về ô nhập mã sách
            bookInput.focus();

        } catch (error) {
            console.error("BorrowBook error:", error);

            resultBox.innerHTML = `
                <p style="color: red;">
                    Không thể kết nối đến hệ thống C++.
                </p>
            `;

        } finally {
            // Mở lại nút
            btn.disabled = false;
        }
    };

    // Bấm nút Mượn sách
    btn.addEventListener("click", handleBorrow);

    // Nhấn Enter ở ô mã sách
    bookInput.addEventListener("keydown", event => {
        if (event.key === "Enter") {
            handleBorrow();
        }
    });
}
```
