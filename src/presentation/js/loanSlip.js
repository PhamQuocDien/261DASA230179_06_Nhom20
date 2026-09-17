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

        return null;
    }


    console.log(
        "Danh sach phieu muon:",
        result.data
    );


    return result.data;
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
        label;


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


    const title =
        document.createElement("h4");

    title.textContent =
        "Phiếu mượn";


    const loanId =
        document.createElement("span");

    loanId.className =
        "loan-slip-loan-id";

    loanId.textContent =
        loan.loanId || "-";


    cardHeader.appendChild(
        title
    );

    cardHeader.appendChild(
        loanId
    );


    // =================================
    // NOI DUNG PHIEU
    // =================================

    const cardBody =
        document.createElement("div");

    cardBody.className =
        "loan-slip-card-body";


    cardBody.appendChild(
        createLoanInfo(
            "Loan_ID",
            loan.loanId
        )
    );


    cardBody.appendChild(
        createLoanInfo(
            "Member_ID",
            loan.memberId
        )
    );


    cardBody.appendChild(
        createLoanInfo(
            "Book_ID",
            loan.bookId
        )
    );


    cardBody.appendChild(
        createLoanInfo(
            "Ngày mượn",
            loan.borrowDate
        )
    );


    cardBody.appendChild(
        createLoanInfo(
            "Ngày đến hạn",
            loan.dueDate
        )
    );


    cardBody.appendChild(
        createLoanInfo(
            "Ngày trả",
            loan.returnDate
        )
    );


    cardBody.appendChild(
        createLoanInfo(
            "Số lần gia hạn",
            loan.renewalCount
        )
    );


    // =================================
    // TRẠNG THÁI
    // =================================

    const statusInfo =
        document.createElement("div");

    statusInfo.className =
        "loan-slip-info";


    const statusLabel =
        document.createElement("span");

    statusLabel.className =
        "loan-slip-label";

    statusLabel.textContent =
        "Trạng thái";


    const statusValue =
        document.createElement("span");

    statusValue.className =
        "loan-slip-status";


    statusValue.textContent =
        loan.status || "-";


    // =================================
    // GAN CLASS THEO TRANG THAI
    // =================================

    if (
        loan.status &&
        loan.status.toUpperCase() ===
        "BORROWING"
    ) {

        statusValue.classList.add(
            "borrowing"
        );

    } else if (
        loan.status &&
        loan.status.toUpperCase() ===
        "RETURNED"
    ) {

        statusValue.classList.add(
            "returned"
        );

    } else {

        statusValue.classList.add(
            "other"
        );
    }


    statusInfo.appendChild(
        statusLabel
    );

    statusInfo.appendChild(
        statusValue
    );


    cardBody.appendChild(
        statusInfo
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


    return card;
}


// =====================================
// HIEN THI DANH SACH PHIEU MUON
// =====================================

function renderLoanSlips(
    loanSlipList,
    loanSlips
) {

    loanSlipList.innerHTML = "";


    // =================================
    // KIEM TRA DU LIEU
    // =================================

    if (!Array.isArray(loanSlips)) {

        loanSlipList.textContent =
            "Du lieu phieu muon khong hop le.";

        return;
    }


    // =================================
    // KHONG CO PHIEU
    // =================================

    if (loanSlips.length === 0) {

        loanSlipList.textContent =
            "Member_ID nay chua co sach dang muon.";

        return;
    }


    // =================================
    // TAO TUNG CARD
    // =================================

    loanSlips.forEach(
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
            "#loanSlipList"
        );


    // =================================
    // KIEM TRA HTML
    // =================================

    if (
        !memberIdInput ||
        !btnLoadLoanSlips ||
        !loanSlipList
    ) {

        console.warn(
            "Chua tim thay day du HTML Loan Slip."
        );

        return;
    }


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

                return;
            }


            // =============================
            // GOI API
            // =============================

            const loanSlips =
                await getLoanSlipsByMember(
                    memberId
                );


            // =============================
            // API LOI
            // =============================

            if (loanSlips === null) {

                loanSlipList.textContent =
                    "Khong the tai phieu muon.";

                return;
            }


            // =============================
            // HIEN THI
            // =============================

            renderLoanSlips(
                loanSlipList,
                loanSlips
            );
        }
    );


    console.log(
        "Loan Slip module da san sang."
    );
}