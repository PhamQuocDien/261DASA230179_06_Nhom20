// =====================================
// MAIN SCRIPT
// BUOI 50
// FILE DIEU PHOI CHINH
// =====================================

import {
    loadComponents
} from "./js/componentLoader.js";

import {
    initBook
} from "./js/book.js";

import {
    initLoanSlip
} from "./js/loanSlip.js";

import {
    initMemberRegister
} from "./js/memberRegister.js";

import {
    initReturnBook
} from "./js/returnBook.js";

import {
    initFine
} from "./js/fine.js";

import {
    initNavigation
} from "./js/navigation.js";


// =====================================
// KHOI DONG UNG DUNG
// =====================================

document.addEventListener(
    "DOMContentLoaded",
    async () => {

        console.log(
            "=== KHOI DONG THU VIEN ==="
        );


        // =============================
        // LOAD HTML COMPONENTS
        // =============================

        const componentsLoaded =
            await loadComponents();


        if (!componentsLoaded) {

            console.error(
                "Khong the khoi dong do component loi."
            );

            return;
        }


        // =============================
        // KHOI DONG BOOK
        // =============================

        initBook();


        // =============================
        // KHOI DONG LOAN SLIP
        // =============================

        initLoanSlip();


        // =============================
        // KHOI DONG DANG KY THANH VIEN
        // =============================

        initMemberRegister();


        // =============================
        // KHOI DONG TRA SACH
        // =============================

        initReturnBook();


        // =============================
        // KHOI DONG TRA CUU TIEN PHAT
        // =============================

        initFine();


        // =============================
        // KHOI DONG NAVIGATION
        // =============================

        initNavigation();


        // =============================
        // KHOI DONG HOAN TAT
        // =============================

        console.log(
            "=== THU VIEN DA KHOI DONG ==="
        );
    }
);
