document.addEventListener("DOMContentLoaded", () => {
    const btn = document.querySelector("button"); // Nút "Tra cứu"
    const input = document.querySelector("input"); // Ô nhập Loan_ID
    const resultBox = document.querySelector(".result-box") || document.body;

    if (btn && input) {
        btn.addEventListener("click", async () => {
            const loanId = input.value.trim();
            if (!loanId) {
                alert("Vui lòng nhập Loan_ID!");
                return;
            }

            try {
                const response = await fetch("../api/api.php", {
                    method: "POST",
                    headers: { "Content-Type": "application/json" },
                    body: JSON.stringify({ action: "getFine", loanId: loanId })
                });
                const data = await response.json();

                if (data.success) {
                    resultBox.innerHTML = `
                        <div style="color: green; padding: 10px;">
                            <p><b>Mã phạt:</b> ${data.fineId}</p>
                            <p><b>Số tiền:</b> ${Number(data.amount).toLocaleString()} VNĐ</p>
                            <p><b>Lý do:</b> ${data.reason}</p>
                            <p><b>Trạng thái:</b> ${data.status}</p>
                        </div>
                    `;
                } else {
                    resultBox.innerHTML = `<p style="color: red;">${data.message}</p>`;
                }
            } catch (err) {
                console.error(err);
                alert("Lỗi kết nối máy chủ C++!");
            }
        });
    }
});
