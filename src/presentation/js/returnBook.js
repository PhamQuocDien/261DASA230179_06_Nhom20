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
    const resultBox = document.querySelector("#returnBookResult");

    if (!button || !input || !dateInput || !qualityInput || !resultBox) {
        console.warn("ReturnBook: khong tim thay giao dien tra sach.");
        return;
    }
    if (button.dataset.initialized === "true") return;
    button.dataset.initialized = "true";

    dateInput.value = todayString();

    const submit = async () => {
        const loanId = input.value.trim();
        const returnDate = dateInput.value;
        const quality = qualityInput.value;

        if (!loanId) {
            resultBox.innerHTML = `<p style="color:red;">Vui long nhap Loan_ID.</p>`;
            input.focus();
            return;
        }
        if (!returnDate) {
            resultBox.innerHTML = `<p style="color:red;">Vui long chon ngay tra.</p>`;
            return;
        }

        button.disabled = true;
        resultBox.innerHTML = `<p>Dang xu ly...</p>`;

        try {
            const response = await sendApiRequest({
                action: "returnBook",
                loanId,
                returnDate,
                quality
            });

            if (!response.success) {
                resultBox.innerHTML = `<p style="color:red;">${escapeHtml(response.error || response.message || "Khong the tra sach.")}</p>`;
                return;
            }

            const data = response.data || {};
            const total = Number(data.totalFee || 0);
            resultBox.innerHTML = `
                <div style="padding:10px;">
                    <p style="color:green;"><b>${escapeHtml(response.message || "Tra sach thanh cong.")}</b></p>
                    <p><b>Loan_ID:</b> ${escapeHtml(data.loanId)}</p>
                    <p><b>So ngay tre:</b> ${Number(data.lateDays || 0)}</p>
                    <p><b>Phi tre:</b> ${Number(data.lateFee || 0).toLocaleString("vi-VN")} VNĐ</p>
                    <p><b>Phi hu hong:</b> ${Number(data.damageFee || 0).toLocaleString("vi-VN")} VNĐ</p>
                    <p><b>Tong tien phat:</b> ${total.toLocaleString("vi-VN")} VNĐ</p>
                </div>
            `;
            input.value = "";
        } catch (error) {
            console.error(error);
            resultBox.innerHTML = `<p style="color:red;">Khong the ket noi API.</p>`;
        } finally {
            button.disabled = false;
        }
    };

    button.addEventListener("click", submit);
    input.addEventListener("keydown", event => {
        if (event.key === "Enter") submit();
    });
}
