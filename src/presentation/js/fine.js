import { sendApiRequest } from "./api.js";

function escapeHtml(value) {
    return String(value ?? "")
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}

export function initFine() {
    const button = document.querySelector("#btnSearchFine");
    const input = document.querySelector("#fineLoanId");
    const resultBox = document.querySelector("#fineResult");

    if (!button || !input || !resultBox) {
        console.warn("Fine: khong tim thay giao dien tra cuu tien phat.");
        return;
    }

    if (button.dataset.initialized === "true") {
        return;
    }
    button.dataset.initialized = "true";

    const search = async () => {
        const loanId = input.value.trim();
        if (!loanId) {
            resultBox.innerHTML = `<p style="color:red;">Vui long nhap Loan_ID.</p>`;
            input.focus();
            return;
        }

        button.disabled = true;
        resultBox.innerHTML = `<p>Dang tra cuu...</p>`;

        try {
            const response = await sendApiRequest({
                action: "getFine",
                loanId
            });

            if (!response.success) {
                resultBox.innerHTML = `<p style="color:red;">${escapeHtml(response.error || response.message || "Khong tim thay tien phat.")}</p>`;
                return;
            }

            const data = response.data || {};
            resultBox.innerHTML = `
                <div style="padding:10px;">
                    <p style="color:green;"><b>Tim thay thong tin tien phat.</b></p>
                    <p><b>Ma phat:</b> ${escapeHtml(data.fineId)}</p>
                    <p><b>Loan_ID:</b> ${escapeHtml(data.loanId)}</p>
                    <p><b>Member_ID:</b> ${escapeHtml(data.memberId)}</p>
                    <p><b>So tien:</b> ${Number(data.amount || 0).toLocaleString("vi-VN")} VNĐ</p>
                    <p><b>Ly do:</b> ${escapeHtml(data.reason)}</p>
                    <p><b>Trang thai:</b> ${escapeHtml(data.status)}</p>
                </div>
            `;
        } catch (error) {
            console.error(error);
            resultBox.innerHTML = `<p style="color:red;">Khong the ket noi API.</p>`;
        } finally {
            button.disabled = false;
        }
    };

    button.addEventListener("click", search);
    input.addEventListener("keydown", event => {
        if (event.key === "Enter") {
            search();
        }
    });
}

