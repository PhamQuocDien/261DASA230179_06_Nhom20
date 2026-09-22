// =====================================
// MAIN SCRIPT
// BUOI 50
// FILE DIEU PHOI CHINH
// + MC2: TIM KIEM SACH THEO KHOANG NAM
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
    initReservation
} from "./js/reservation.js";

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
        // KHOI DONG DANG KY CHO MUON
        // =============================

        initReservation();


        // =============================
        // KHOI DONG NAVIGATION
        // =============================

        initNavigation();


        // =====================================================
        // MC2 - TIM KIEM SACH THEO KHOANG NAM
        // =====================================================

        initSearchBookByYear();


        // =============================
        // KHOI DONG HOAN TAT
        // =============================

        console.log(
            "=== THU VIEN DA KHOI DONG ==="
        );
    }
);


// =====================================================
// MC2 - TIM KIEM SACH THEO KHOANG NAM
// =====================================================

function initSearchBookByYear() {

    const fromYear =
        document.getElementById("fromYear");

    const toYear =
        document.getElementById("toYear");

    const yearSortOrder =
        document.getElementById("yearSortOrder");

    const btnSearchBookByYear =
        document.getElementById("btnSearchBookByYear");

    const resultContainer =
        document.getElementById(
            "searchBookByYearResult"
        );


    // -------------------------------------
    // KIEM TRA HTML
    // -------------------------------------

    if (!fromYear ||
        !toYear ||
        !yearSortOrder ||
        !btnSearchBookByYear ||
        !resultContainer) {

        console.warn(
            "Khong tim thay giao dien MC2."
        );

        return;
    }


    // -------------------------------------
    // GAN SU KIEN CHO NUT TIM KIEM
    // -------------------------------------

    btnSearchBookByYear.addEventListener(
        "click",
        async function () {

            const yearStart =
                parseInt(fromYear.value);

            const yearEnd =
                parseInt(toYear.value);

            const sortOrder =
                yearSortOrder.value;


            // =================================
            // KIEM TRA DU LIEU
            // =================================

            if (
                isNaN(yearStart) ||
                isNaN(yearEnd)
            ) {

                resultContainer.innerHTML = `
                    <p>
                        Vui long nhap day du
                        nam bat dau va nam ket thuc.
                    </p>
                `;

                return;
            }


            if (yearStart > yearEnd) {

                resultContainer.innerHTML = `
                    <p>
                        Nam bat dau phai nho hon
                        hoac bang nam ket thuc.
                    </p>
                `;

                return;
            }


            // =================================
            // HIEN THI DANG TIM
            // =================================

            resultContainer.innerHTML = `
                <p>
                    Dang tim kiem...
                </p>
            `;


            try {

                // =====================================
                // GUI REQUEST DEN C++ BACKEND
                // =====================================

                const response =
                    await fetch(
                        "/api",
                        {
                            method: "POST",

                            headers: {
                                "Content-Type":
                                    "application/json"
                            },

                            body: JSON.stringify({

                                action:
                                    "getBooksByYearRange",

                                yearStart:
                                    yearStart,

                                yearEnd:
                                    yearEnd
                            })
                        }
                    );


                // =====================================
                // KIEM TRA HTTP
                // =====================================

                if (!response.ok) {

                    throw new Error(
                        "Khong ket noi duoc backend."
                    );
                }


                // =====================================
                // DOC JSON
                // =====================================

                const result =
                    await response.json();


                // =====================================
                // BACKEND BAO LOI
                // =====================================

                if (!result.success) {

                    resultContainer.innerHTML = `
                        <p>
                            ${escapeHtml(
                                result.error ||
                                "Khong the tim kiem sach."
                            )}
                        </p>
                    `;

                    return;
                }


                // =====================================
                // LAY DANH SACH SACH
                // =====================================

                let books =
                    Array.isArray(result.data)
                        ? result.data
                        : [];


                // =====================================
                // SAP XEP KET QUA
                // =====================================

                books.sort(
                    (a, b) => {

                        const yearA =
                            Number(a.year) || 0;

                        const yearB =
                            Number(b.year) || 0;


                        if (yearA !== yearB) {

                            return sortOrder === "asc"
                                ? yearA - yearB
                                : yearB - yearA;
                        }


                        // Neu trung nam
                        // sap xep theo BookCode

                        const codeA =
                            String(
                                a.bookCode || ""
                            );

                        const codeB =
                            String(
                                b.bookCode || ""
                            );


                        return codeA.localeCompare(
                            codeB
                        );
                    }
                );


                // =====================================
                // KHONG CO SACH
                // =====================================

                if (books.length === 0) {

                    resultContainer.innerHTML = `
                        <p>
                            Khong tim thay sach tu nam
                            <strong>${yearStart}</strong>
                            den nam
                            <strong>${yearEnd}</strong>.
                        </p>
                    `;

                    return;
                }


                // =====================================
                // TAO BANG KET QUA
                // =====================================

                let html = `

                    <p>
                        Tim thay
                        <strong>${books.length}</strong>
                        sach tu nam
                        <strong>${yearStart}</strong>
                        den
                        <strong>${yearEnd}</strong>.
                    </p>

                    <table
                        style="
                            width:100%;
                            border-collapse:collapse;
                            margin-top:15px;
                        "
                    >

                        <thead>

                            <tr>

                                <th style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                ">
                                    STT
                                </th>

                                <th style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                ">
                                    Book Code
                                </th>

                                <th style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                ">
                                    Ten sach
                                </th>

                                <th style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                ">
                                    Tac gia
                                </th>

                                <th style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                ">
                                    The loai
                                </th>

                                <th style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                ">
                                    Nam
                                </th>

                            </tr>

                        </thead>

                        <tbody>
                `;


                // =====================================
                // DUYET SACH
                // =====================================

                books.forEach(
                    (book, index) => {

                        html += `

                            <tr>

                                <td style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                    text-align:center;
                                ">
                                    ${index + 1}
                                </td>

                                <td style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                ">
                                    ${escapeHtml(
                                        book.bookCode || ""
                                    )}
                                </td>

                                <td style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                ">
                                    ${escapeHtml(
                                        book.title || ""
                                    )}
                                </td>

                                <td style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                ">
                                    ${escapeHtml(
                                        book.author || ""
                                    )}
                                </td>

                                <td style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                ">
                                    ${escapeHtml(
                                        book.category || ""
                                    )}
                                </td>

                                <td style="
                                    border:1px solid #ccc;
                                    padding:10px;
                                    text-align:center;
                                ">
                                    ${book.year ?? ""}
                                </td>

                            </tr>

                        `;
                    }
                );


                html += `

                        </tbody>

                    </table>

                `;


                // =====================================
                // HIEN THI
                // =====================================

                resultContainer.innerHTML =
                    html;

            }
            catch (error) {

                console.error(
                    "MC2 ERROR:",
                    error
                );


                resultContainer.innerHTML = `

                    <p>
                        Khong the ket noi den
                        he thong backend.
                    </p>

                `;
            }

        }
    );
}


// =====================================================
// HAM BAO VE HIEN THI HTML
// =====================================================

function escapeHtml(value) {

    return String(value)
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}
