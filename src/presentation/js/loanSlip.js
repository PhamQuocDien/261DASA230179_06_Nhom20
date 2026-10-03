// =====================================
// LOAN SLIP MODULE
// BUOI 50
// HIEN THI PHIEU MUON TU API
// =====================================

import {
    sendApiRequest
} from "./api.js";


// =====================================
// LAY DANH SACH PHIEU MUON
// =====================================

async function getLoanSlipsByMember(
    memberId
) {

    if (!memberId) {

        console.warn(
            "Vui long nhap Member_ID."
        );

        return null;
    }


    const result =
        await sendApiRequest({
            action:
                "getLoanSlipsByMember",

            memberId:
                memberId
        });


    if (!result.success) {

        console.error(
            "Khong the lay phieu muon:",
            result.error
        );

        return result;
    }


    console.log(
        "Danh sach phieu muon:",
        result.data
    );


    return result;
}


// =====================================
// XOA PHIEU MUON
// Backend tu xac thuc chu so huu phieu
// va tu quyet dinh co the xoa hay khong
// =====================================

async function deleteLoan(
    loanId,
    password
) {

    return await sendApiRequest({
        action:
            "deleteLoan",

        loanId:
            loanId,

        password:
            password
    });
}


// =====================================
// CHUYEN SANG DANG KY THANH VIEN
// =====================================

function openMemberRegistration(
    memberId
) {

    console.log(
        "Member_ID khong ton tai:",
        memberId
    );


    // =================================
    // LUU MEMBER_ID CAN DANG KY
    // =================================

    sessionStorage.setItem(
        "pendingMemberId",
        memberId
    );


    // =================================
    // TIM NUT DANG KY THANH VIEN
    // =================================

    const navigationElements =
        Array.from(
            document.querySelectorAll(
                "button, a, [role='button']"
            )
        );


    const memberRegistrationElement =
        navigationElements.find(
            element => {

                const text =
                    element.textContent
                        .trim()
                        .toLowerCase();

                return text.includes(
                    "đăng ký thành viên"
                );
            }
        );


    // =================================
    // NEU TIM THAY NUT
    // =================================

    if (memberRegistrationElement) {

        console.log(
            "Dang chuyen sang trang Dang ky thanh vien."
        );


        memberRegistrationElement.click();

        return;
    }


    // =================================
    // NEU CHUA TIM THAY
    // GIAO CHO NAVIGATION XU LY
    // =================================

    console.warn(
        "Chua tim thay nut Dang ky thanh vien."
    );


    window.dispatchEvent(
        new CustomEvent(
            "openMemberRegistration",
            {
                detail: {
                    memberId:
                        memberId
                }
            }
        )
    );
}


// =====================================
// HAM TAO DONG THONG TIN
// =====================================

function createLoanInfo(
    label,
    value
) {

    const info =
        document.createElement("div");

    info.className =
        "loan-slip-info";


    const labelElement =
        document.createElement("span");

    labelElement.className =
        "loan-slip-label";

    labelElement.textContent =
        label + ":";


    const valueElement =
        document.createElement("span");

    valueElement.className =
        "loan-slip-value";


    valueElement.textContent =
        value === null ||
            value === undefined ||
            value === ""
            ? "-"
            : value;


    info.appendChild(
        labelElement
    );

    info.appendChild(
        valueElement
    );


    return info;
}


// =====================================
// HIEN THI MOT PHIEU MUON
// =====================================

function createLoanSlipCard(
    loan
) {

    const card =
        document.createElement("div");

    card.className =
        "loan-slip-card";


    // =================================
    // HEADER PHIEU
    // =================================

    const cardHeader =
        document.createElement("div");

    cardHeader.className =
        "loan-slip-card-header";


    const isBorrowing =
        loan.status &&
        loan.status.toUpperCase() ===
        "BORROWING";


    const loanId =
        document.createElement("span");

    loanId.className =
        "loan-slip-loan-id";

    loanId.textContent =
        loan.loanId || "-";


    const statusBadge =
        document.createElement("span");

    statusBadge.className =
        "loan-slip-status " +
        (isBorrowing
            ? "borrowing"
            : "returned");

    statusBadge.textContent =
        isBorrowing
        ? "ĐANG MƯỢN"
        : "ĐÃ TRẢ";


    cardHeader.appendChild(
        loanId
    );

    cardHeader.appendChild(
        statusBadge
    );


    // =================================
    // NOI DUNG PHIEU
    // =================================

    const cardBody =
        document.createElement("div");

    cardBody.className =
        "loan-slip-card-body";


    const title =
        document.createElement("div");

    title.className =
        "loan-slip-title";

    title.textContent =
        loan.bookTitle || "-";

    cardBody.appendChild(
        title
    );


    cardBody.appendChild(
        createLoanInfo(
            "Mã sách",
            loan.bookCode
        )
    );


    cardBody.appendChild(
        createLoanInfo(
            "Ngày mượn",
            loan.borrowDate
        )
    );


    // Chỉ hiện hạn trả hoặc ngày trả
    // cho đúng trạng thái phiếu

    if (isBorrowing) {

        cardBody.appendChild(
            createLoanInfo(
                "Hạn trả",
                loan.dueDate
            )
        );

    } else {

        cardBody.appendChild(
            createLoanInfo(
                "Ngày trả",
                loan.returnDate
            )
        );

    }


    // =================================
    // THAO TAC: CHI XOA PHIEU
    // Gia han nam o man hinh rieng
    // =================================

    const cardFooter =
        document.createElement("div");

    cardFooter.className =
        "loan-slip-card-footer";


    const deleteButton =
        document.createElement("button");

    deleteButton.type =
        "button";

    deleteButton.className =
        "loan-slip-delete-btn";

    deleteButton.dataset.loanId =
        loan.loanId || "";

    deleteButton.textContent =
        "Xóa phiếu";

    cardFooter.appendChild(
        deleteButton
    );


    // =================================
    // GHEP CARD
    // =================================

    card.appendChild(
        cardHeader
    );

    card.appendChild(
        cardBody
    );

    card.appendChild(
        cardFooter
    );


    return card;
}


// =====================================
// HIEN THI DANH SACH PHIEU MUON
// =====================================

function renderLoanSlips(
    loanSlipList,
    loanSlips,
    returnedLoanSlipList
) {

    loanSlipList.innerHTML = "";

    returnedLoanSlipList.innerHTML = "";


    // =================================
    // KIEM TRA DU LIEU
    // =================================

    if (!Array.isArray(loanSlips)) {

        loanSlipList.textContent =
            "Du lieu phieu muon khong hop le.";

        returnedLoanSlipList.textContent =
            "Du lieu phieu muon khong hop le.";

        return;
    }


    // =================================
    // PHAN LOAI PHIEU
    // =================================

    const borrowingLoanSlips =
        loanSlips.filter(
            loan =>
                loan.status &&
                loan.status.toUpperCase() ===
                "BORROWING"
        );


    const returnedLoanSlips =
        loanSlips.filter(
            loan =>
                loan.status &&
                loan.status.toUpperCase() ===
                "RETURNED"
        );


    // =================================
    // DANH SACH DANG MUON
    // =================================

    if (borrowingLoanSlips.length === 0) {

        loanSlipList.textContent =
            "Member_ID nay khong co sach dang muon.";

    } else {

        borrowingLoanSlips.forEach(
            loan => {

                const card =
                    createLoanSlipCard(
                        loan
                    );

                loanSlipList.appendChild(
                    card
                );
            }
        );
    }


    // =================================
    // DANH SACH DA TRA
    // =================================

    if (returnedLoanSlips.length === 0) {

        returnedLoanSlipList.textContent =
            "Member_ID nay chua co sach da tra.";

    } else {

        returnedLoanSlips.forEach(
            loan => {

                const card =
                    createLoanSlipCard(
                        loan
                    );

                returnedLoanSlipList.appendChild(
                    card
                );
            }
        );
    }
}


// =====================================
// KHOI TAO LOAN SLIP MODULE
// =====================================

export function initLoanSlip() {

    console.log(
        "=== INIT LOAN SLIP MODULE ==="
    );


    const memberIdInput =
        document.querySelector(
            "#memberId"
        );


    const btnLoadLoanSlips =
        document.querySelector(
            "#btnLoadLoanSlips"
        );


    const loanSlipList =
        document.querySelector(
            "#borrowingLoanSlipList"
        );


    const returnedLoanSlipList =
        document.querySelector(
            "#returnedLoanSlipList"
        );


    const tabBorrowing =
        document.querySelector(
            "#tabBorrowing"
        );


    const tabReturned =
        document.querySelector(
            "#tabReturned"
        );


    // =================================
    // KIEM TRA HTML
    // =================================

    if (
        !memberIdInput ||
        !btnLoadLoanSlips ||
        !loanSlipList ||
        !returnedLoanSlipList ||
        !tabBorrowing ||
        !tabReturned
    ) {

        console.warn(
            "Chua tim thay day du HTML Loan Slip."
        );

        return;
    }
const deleteLoanModal =
        document.querySelector(
            "#deleteLoanModal"
        );


    const deleteLoanModalLoanId =
        document.querySelector(
            "#deleteLoanModalLoanId"
        );


    const deleteLoanPassword =
        document.querySelector(
            "#deleteLoanPassword"
        );


    const deleteLoanModalMessage =
        document.querySelector(
            "#deleteLoanModalMessage"
        );


    const btnDeleteLoanCancel =
        document.querySelector(
            "#btnDeleteLoanCancel"
        );


    const btnDeleteLoanConfirm =
        document.querySelector(
            "#btnDeleteLoanConfirm"
        );


    // Loan_ID dang cho xac nhan xoa
    let pendingDeleteLoanId = "";


    function openDeleteLoanModal(
        loanId
    ) {

        pendingDeleteLoanId =
            loanId;

        if (deleteLoanModalLoanId) {
            deleteLoanModalLoanId.textContent =
                loanId;
        }

        if (deleteLoanModalMessage) {
            deleteLoanModalMessage.textContent =
                "";
        }

        if (deleteLoanPassword) {
            deleteLoanPassword.value =
                "";
        }

        if (deleteLoanModal) {
            deleteLoanModal.style.display =
                "flex";
        }

        if (deleteLoanPassword) {
            deleteLoanPassword.focus();
        }
    }


    function closeDeleteLoanModal() {

        if (deleteLoanModal) {
            deleteLoanModal.style.display =
                "none";
        }

        if (deleteLoanPassword) {
            deleteLoanPassword.value =
                "";
        }

        pendingDeleteLoanId = "";
    }


    async function handleDeleteLoanConfirm() {

        const password =
            deleteLoanPassword
                ? deleteLoanPassword.value
                : "";

        if (password === "") {

            if (deleteLoanModalMessage) {
                deleteLoanModalMessage.textContent =
                    "Vui long nhap mat khau thanh vien.";
            }

            return;
        }


        if (btnDeleteLoanConfirm) {
            btnDeleteLoanConfirm.disabled =
                true;
        }


        const result =
            await deleteLoan(
                pendingDeleteLoanId,
                password
            );


        // Xoa mat khau khoi o nhap ngay sau khi gui
        if (deleteLoanPassword) {
            deleteLoanPassword.value =
                "";
        }


        if (btnDeleteLoanConfirm) {
            btnDeleteLoanConfirm.disabled =
                false;
        }


        // Chi hien thi message tu backend tra ve
        if (!result.success) {

            if (deleteLoanModalMessage) {
                deleteLoanModalMessage.textContent =
                    result.error ||
                    "Khong the xoa phieu muon.";
            }

            return;
        }


        closeDeleteLoanModal();


        console.log(
            "Xoa phieu muon thanh cong."
        );


        // Tai lai danh sach phieu cua thanh vien
        const memberId =
            memberIdInput.value.trim();

        if (memberId !== "") {

            const reload =
                await getLoanSlipsByMember(
                    memberId
                );

            if (reload && reload.success) {

                renderLoanSlips(
                    loanSlipList,
                    reload.data,
                    returnedLoanSlipList
                );
            }
        }
    }


    // =================================
    // MODAL XOA PHIEU
    // =================================

    if (btnDeleteLoanCancel) {
        btnDeleteLoanCancel.addEventListener(
            "click",
            closeDeleteLoanModal
        );
    }

    if (btnDeleteLoanConfirm) {
        btnDeleteLoanConfirm.addEventListener(
            "click",
            handleDeleteLoanConfirm
        );
    }

    if (deleteLoanPassword) {
        deleteLoanPassword.addEventListener(
            "keydown",
            event => {
                if (event.key === "Enter") {
                    handleDeleteLoanConfirm();
                }
            }
        );
    }

    // Mo modal khi bam nut Xoa phieu tren card
    loanSlipList.addEventListener(
        "click",
        event => {

            const deleteButton =
                event.target.closest(
                    ".loan-slip-delete-btn"
                );

            if (!deleteButton) {
                return;
            }

            openDeleteLoanModal(
                deleteButton.dataset.loanId
            );
        }
    );


    returnedLoanSlipList.addEventListener(
        "click",
        event => {

            const deleteButton =
                event.target.closest(
                    ".loan-slip-delete-btn"
                );

            if (!deleteButton) {
                return;
            }

            openDeleteLoanModal(
                deleteButton.dataset.loanId
            );
        }
    );


    // =================================
    // CLICK TAB SACH DANG MUON
    // =================================

    tabBorrowing.addEventListener(
        "click",
        () => {

            loanSlipList.style.display =
                "block";

            returnedLoanSlipList.style.display =
                "none";


            tabBorrowing.classList.add(
                "active"
            );

            tabReturned.classList.remove(
                "active"
            );
        }
    );


    // =================================
    // CLICK TAB SACH DA TRA
    // =================================

    tabReturned.addEventListener(
        "click",
        () => {

            loanSlipList.style.display =
                "none";

            returnedLoanSlipList.style.display =
                "block";


            tabReturned.classList.add(
                "active"
            );

            tabBorrowing.classList.remove(
                "active"
            );
        }
    );


    // =================================
    // CLICK XEM PHIEU
    // =================================

    btnLoadLoanSlips.addEventListener(
        "click",
        async () => {

            const memberId =
                memberIdInput
                    .value
                    .trim();


            console.log(
                "=== GET LOAN SLIPS ==="
            );


            console.log(
                "Member_ID:",
                memberId
            );


            // =============================
            // KIEM TRA MEMBER_ID
            // =============================

            if (memberId === "") {

                console.warn(
                    "Vui long nhap Member_ID."
                );

                loanSlipList.textContent =
                    "Vui long nhap Member_ID.";

                returnedLoanSlipList.textContent =
                    "Vui long nhap Member_ID.";

                return;
            }


            // =============================
            // GOI API
            // =============================

            const result =
                await getLoanSlipsByMember(
                    memberId
                );


            // =============================
            // API LOI
            // =============================

            if (result === null) {

                loanSlipList.textContent =
                    "Khong the tai phieu muon.";

                returnedLoanSlipList.textContent =
                    "Khong the tai phieu muon.";

                return;
            }


            // =============================
            // MEMBER KHONG TON TAI
            // =============================

            if (
                !result.success &&
                result.error ===
                "Member_ID khong ton tai."
            ) {

                loanSlipList.textContent =
                    "Member_ID khong ton tai.";

                returnedLoanSlipList.textContent =
                    "Dang chuyen sang trang Dang ky thanh vien...";


                openMemberRegistration(
                    memberId
                );

                return;
            }


            // =============================
            // API LOI KHAC
            // =============================

            if (!result.success) {

                loanSlipList.textContent =
                    "Khong the tai phieu muon.";

                returnedLoanSlipList.textContent =
                    result.error ||
                    "Khong the tai phieu muon.";

                return;
            }


            // =============================
            // HIEN THI
            // =============================

            renderLoanSlips(
                loanSlipList,
                result.data,
                returnedLoanSlipList
            );
        }
    );


    console.log(
        "Loan Slip module da san sang."
    );
}