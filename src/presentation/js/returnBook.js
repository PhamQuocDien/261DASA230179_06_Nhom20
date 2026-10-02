import { sendApiRequest } from "./api.js";

function todayString() {
    const now = new Date();
    return `${now.getFullYear()}-${String(now.getMonth() + 1).padStart(2, "0")}-${String(now.getDate()).padStart(2, "0")}`;
}

function escapeHtml(value) {
    return String(value ?? "")
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}

export function initReturnBook() {
    const button = document.querySelector("#btnReturnBook");
    const input = document.querySelector("#returnLoanId");
    const dateInput = document.querySelector("#returnDate");
    const qualityInput = document.querySelector("#returnQuality");
    const confirmationInput = document.querySelector("#returnConfirmation");
    const resultBox = document.querySelector("#returnBookResult");

    if (!button || !input || !dateInput || !qualityInput || !confirmationInput || !resultBox) {
        console.warn("ReturnBook: Không tìm thấy giao diện trả sách.");
        return;
    }
    if (button.dataset.initialized === "true") return;
    button.dataset.initialized = "true";

    dateInput.value = todayString();

    const submit = async () => {
        const loanId = input.value.trim();
        const returnDate = dateInput.value;
        const quality = qualityInput.value;
        const confirmationCode = confirmationInput.value.trim();

        if (!loanId) {
            resultBox.innerHTML = `<p style="color:red;">Vui lòng nhập Loan_ID.</p>`;
            input.focus();
            return;
        }
        if (!returnDate) {
            resultBox.innerHTML = `<p style="color:red;">Vui lòng chọn ngày trả.</p>`;
            return;
        }

        if (!confirmationCode) {
            resultBox.innerHTML = `<p style="color:red;">Thiếu mã xác nhận.</p>`;
            confirmationInput.focus();
            return;
        }

        if (confirmationCode !== "261DASA230179_06") {
            resultBox.innerHTML = `<p style="color:red;">Sai mã xác nhận.</p>`;
            confirmationInput.focus();
            return;
        }

        button.disabled = true;
        resultBox.innerHTML = `<p>Đang xử lý...</p>`;

        try {
            const response = await sendApiRequest({
                action: "returnBook",
                loanId,
                returnDate,
                quality,
                confirmationCode
            });

            if (!response.success) {
                resultBox.innerHTML = `<p style="color:red;">${escapeHtml(response.error || response.message || "Không thể trả sách.")}</p>`;
                return;
            }

            const data = response.data || {};
            const total = Number(data.totalFee || 0);
            resultBox.innerHTML = `
                <div style="padding:10px;">
                    <p style="color:green;"><b>${escapeHtml(response.message || "Trả sách thành công.")}</b></p>
                    <p><b>Loan_ID:</b> ${escapeHtml(data.loanId)}</p>
                    <p><b>Số ngày trễ:</b> ${Number(data.lateDays || 0)}</p>
                    <p><b>Phí trễ:</b> ${Number(data.lateFee || 0).toLocaleString("vi-VN")} VNĐ</p>
                    <p><b>Phí hư hỏng:</b> ${Number(data.damageFee || 0).toLocaleString("vi-VN")} VNĐ</p>
                    <p><b>Tổng tiền phạt:</b> ${total.toLocaleString("vi-VN")} VNĐ</p>
                </div>
            `;
            input.value = "";
            confirmationInput.value = "";
        } catch (error) {
            console.error(error);
            resultBox.innerHTML = `<p style="color:red;">Không thể kết nối API.</p>`;
        } finally {
            button.disabled = false;
        }
    };

    button.addEventListener("click", submit);
    input.addEventListener("keydown", event => {
        if (event.key === "Enter") submit();
    });
}
