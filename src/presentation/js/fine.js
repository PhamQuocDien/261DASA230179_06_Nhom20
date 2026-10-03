import { sendApiRequest } from "./api.js";

function escapeHtml(value) {
    return String(value ?? "")
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}

// Fine dang xem tren man hinh.
// Chi luu Fine_ID de gui len API,
// JS khong tu quyet dinh trang thai tien phat.
let currentFineId = "";

export function initFine() {
    const button = document.querySelector("#btnSearchFine");
    const input = document.querySelector("#fineLoanId");
    const resultBox = document.querySelector("#fineResult");

    const payFineActions = document.querySelector("#payFineActions");
    const btnPayFine = document.querySelector("#btnPayFine");

    const payFineModal = document.querySelector("#payFineModal");
    const payFineModalFineId = document.querySelector("#payFineModalFineId");
    const payFineModalMessage = document.querySelector("#payFineModalMessage");
    const payFineAdminId = document.querySelector("#payFineAdminId");
    const payFinePassword = document.querySelector("#payFinePassword");
    const btnPayFineCancel = document.querySelector("#btnPayFineCancel");
    const btnPayFineConfirm = document.querySelector("#btnPayFineConfirm");

    if (!button || !input || !resultBox) {
        console.warn("Fine: không tìm thấy giao diện tra cứu tiền phạt.");
        return;
    }

    if (button.dataset.initialized === "true") {
        return;
    }
    button.dataset.initialized = "true";

    // -----------------------------------------
    // AN / HIEN NUT XAC NHAN
    // -----------------------------------------

    const showPayFineButton = (visible) => {
        if (!payFineActions) {
            return;
        }
        payFineActions.style.display = visible
            ? "block"
            : "none";
    };

    const openPayFineModal = () => {
        if (!payFineModal) {
            return;
        }

        if (payFineModalFineId) {
            payFineModalFineId.textContent = currentFineId;
        }
        if (payFineAdminId) {
            payFineAdminId.value = "";
        }
        if (payFinePassword) {
            payFinePassword.value = "";
        }
        if (payFineModalMessage) {
            payFineModalMessage.textContent = "";
        }

        payFineModal.style.display = "flex";

        if (payFineAdminId) {
            payFineAdminId.focus();
        }
    };

    const closePayFineModal = () => {
        if (!payFineModal) {
            return;
        }

        payFineModal.style.display = "none";

        // Xoa mat khau ngay khi dong modal.
        if (payFinePassword) {
            payFinePassword.value = "";
        }
        if (payFineAdminId) {
            payFineAdminId.value = "";
        }
    };

    const search = async () => {
        const loanId = input.value.trim();
        if (!loanId) {
            resultBox.innerHTML = `<p style="color:red;">Vui lòng nhập Loan_ID.</p>`;
            input.focus();
            return;
        }

        button.disabled = true;
        resultBox.innerHTML = `<p>Đang tra cứu...</p>`;
        showPayFineButton(false);
        currentFineId = "";

        try {
            const response = await sendApiRequest({
                action: "getFine",
                loanId
            });

            if (!response.success) {
                resultBox.innerHTML = `<p style="color:red;">${escapeHtml(response.error || response.message || "Không tìm thấy tiền phạt.")}</p>`;
                return;
            }

            const data = response.data || {};
            currentFineId = data.fineId || "";

            const status = String(data.status ?? "");

            // Trang thai do backend quyet dinh,
            // JS chi hien thi lai.
            const statusText = status === "PAID"
                ? "PAID (Đã thanh toán)"
                : status;

            resultBox.innerHTML = `
                <div style="padding:10px;">
                    <p style="color:green;"><b>tìm thấy thông tin tiền phạt.</b></p>
                    <p><b>Mã phạt:</b> ${escapeHtml(data.fineId)}</p>
                    <p><b>Loan_ID:</b> ${escapeHtml(data.loanId)}</p>
                    <p><b>Member_ID:</b> ${escapeHtml(data.memberId)}</p>
                    <p><b>Số tiền:</b> ${Number(data.amount || 0).toLocaleString("vi-VN")} VNĐ</p>
                    <p><b>Lý do:</b> ${escapeHtml(data.reason)}</p>
                    <p><b>Trạng thái:</b> ${escapeHtml(statusText)}</p>
                </div>
            `;

            // Chi hien nut khi backend bao chua thanh toan.
            if (currentFineId && status !== "PAID") {
                showPayFineButton(true);
            }
        } catch (error) {
            console.error(error);
            resultBox.innerHTML = `<p style="color:red;">Không thể kết nối API.</p>`;
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

    // -----------------------------------------
    // XAC NHAN DA TRA TIEN
    //
    // JS chi gui Fine_ID + Admin_ID + mat khau.
    // Backend tu xac thuc Admin va tu doi
    // trang thai Fine.
    // -----------------------------------------

    const handlePayFineConfirm = async () => {
        const adminId = payFineAdminId
            ? payFineAdminId.value.trim()
            : "";
        const password = payFinePassword
            ? payFinePassword.value
            : "";

        if (!adminId) {
            if (payFineModalMessage) {
                payFineModalMessage.textContent = "Vui lòng nhập Admin ID.";
            }
            if (payFineAdminId) {
                payFineAdminId.focus();
            }
            return;
        }

        if (!password) {
            if (payFineModalMessage) {
                payFineModalMessage.textContent = "Vui lòng nhập mật khẩu quản lý.";
            }
            if (payFinePassword) {
                payFinePassword.focus();
            }
            return;
        }

        // Xoa mat khau khoi o nhap ngay sau khi gui,
        // khong luu o frontend.
        if (payFinePassword) {
            payFinePassword.value = "";
        }

        if (btnPayFineConfirm) {
            btnPayFineConfirm.disabled = true;
        }

        try {
            const response = await sendApiRequest({
                action: "payFine",
                fineId: currentFineId,
                idAdmin: adminId,
                password
            });

            // Sai Admin_ID / mat khau hoac backend loi:
            // chi hien thi message backend tra ve.
            if (!response.success) {
                if (payFineModalMessage) {
                    payFineModalMessage.textContent =
                        response.error ||
                        response.message ||
                        "Không xác nhận được thanh toán.";
                }
                return;
            }

            closePayFineModal();

            // Tra cuu lai de render dung trang thai
            // moi do backend quyet dinh.
            await search();
        } catch (error) {
            console.error(error);
            if (payFineModalMessage) {
                payFineModalMessage.textContent = "Không thể kết nối API.";
            }
        } finally {
            if (btnPayFineConfirm) {
                btnPayFineConfirm.disabled = false;
            }
        }
    };

    if (btnPayFine) {
        btnPayFine.addEventListener("click", openPayFineModal);
    }

    if (btnPayFineCancel) {
        btnPayFineCancel.addEventListener("click", closePayFineModal);
    }

    if (btnPayFineConfirm) {
        btnPayFineConfirm.addEventListener("click", handlePayFineConfirm);
    }

    if (payFinePassword) {
        payFinePassword.addEventListener("keydown", event => {
            if (event.key === "Enter") {
                handlePayFineConfirm();
            }
        });
    }
}

