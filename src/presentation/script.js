// =====================================
// MAIN SCRIPT
// BUOI 50
// FILE DIEU PHOI CHINH
// =====================================

import {
    loadComponents
} from "./js/componentLoader.js";

import {
    sendApiRequest
} from "./js/api.js";

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


        // =================================
        // LOAD HTML COMPONENTS
        // =================================

        const componentsLoaded =
            await loadComponents();


        if (!componentsLoaded) {

            console.warn( "Mot so component HTML bi loi, nhung van tiep tuc khoi dong JavaScript." );
        }


        // =================================
        // KHOI DONG BOOK
        // =================================

        initBook();


        // =================================
        // KHOI DONG LOAN SLIP
        // =================================

        initLoanSlip();


        // =================================
        // KHOI DONG DANG KY THANH VIEN
        // =================================

        initMemberRegister();


        // =================================
        // KHOI DONG TRA SACH
        // =================================

        initReturnBook();


        // =================================
        // KHOI DONG TRA CUU TIEN PHAT
        // =================================

        initFine();


        // =================================
        // KHOI DONG DANG KY CHO MUON
        // =================================

        initReservation();


        // =================================
        // KHOI DONG NAVIGATION
        // =================================

        initNavigation();
        // =================================
        // MC1 - TIM KIEM SACH
        // =================================

        initSearchBook();

        // =================================
        // MC2
        // TIM KIEM SACH THEO KHOANG NAM
        // =================================

        initSearchBookByYear();


        // =================================
        // HOAN TAT
        // =================================

        console.log(
            "=== THU VIEN DA KHOI DONG ==="
        );
    }
);

// =====================================================
// MC1 - TIM KIEM THEO BOOK CODE HOAC TEN SACH
// =====================================================

function initSearchBook() {

    const searchType =
        document.getElementById("searchBookType");

    const input =
        document.getElementById("searchBookKeyword");

    const button =
        document.getElementById("btnSearchBook");

    const resultContainer =
        document.getElementById("searchBookResult");


    // =================================
    // KIEM TRA GIAO DIEN
    // =================================

    if (
        !searchType ||
        !input ||
        !button ||
        !resultContainer
    ) {

        console.warn(
            "Khong tim thay giao dien tim kiem sach."
        );

        return;
    }


    // =================================
    // DOI PLACEHOLDER KHI CHON CACH TIM
    // =================================

    searchType.addEventListener(
        "change",
        () => {

            if (
                searchType.value === "bookCode"
            ) {

                input.placeholder =
                    "Nhập Book Code...";

            }
            else if (
                searchType.value === "title"
            ) {

                input.placeholder =
                    "Nhập tên sách...";
            }

            input.value = "";

            resultContainer.innerHTML = `
                <p>
                    Chưa có kết quả tìm kiếm.
                </p>
            `;
        }
    );


    // =================================
    // NUT TIM KIEM
    // =================================

    button.addEventListener(
        "click",
        async () => {

            const keyword =
                input.value.trim();

            if (keyword === "") {

                resultContainer.innerHTML = `
                    <p>
                        Vui lòng nhập từ khóa.
                    </p>
                `;

                return;
            }


            resultContainer.innerHTML = `
                <p>
                    Đang tìm kiếm...
                </p>
            `;


            try {

                let response;


                // =================================
                // TIM THEO BOOK CODE
                // =================================

                if (
                    searchType.value === "bookCode"
                ) {

                    response =
                        await sendApiRequest({

                            action: "getBook",

                            bookCode: keyword
                        });


                    if (
                        !response ||
                        !response.success
                    ) {

                        resultContainer.innerHTML = `
                            <p>
                                ${
                                    response &&
                                    response.error
                                        ? escapeHtml(
                                            response.error
                                        )
                                        : "Không tìm thấy sách."
                                }
                            </p>
                        `;

                        return;
                    }


                    const book =
                        response.data;


                    resultContainer.innerHTML = `

                        <div>

                            <p>
                                <strong>Book Code:</strong>
                                ${escapeHtml(
                                    book.bookCode || ""
                                )}
                            </p>

                            <p>
                                <strong>Tên sách:</strong>
                                ${escapeHtml(
                                    book.title || ""
                                )}
                            </p>

                            <p>
                                <strong>Tác giả:</strong>
                                ${escapeHtml(
                                    book.author || ""
                                )}
                            </p>

                            <p>
                                <strong>Thể loại:</strong>
                                ${escapeHtml(
                                    book.category || ""
                                )}
                            </p>

                            <p>
                                <strong>Năm xuất bản:</strong>
                                ${book.year ?? ""}
                            </p>

                        </div>
                    `;

                    return;
                }


                // =================================
                // TIM THEO TEN SACH
                // =================================

                response =
                    await sendApiRequest({

                        action:
                            "searchBookByTitle",

                        keyword:
                            keyword
                    });


                if (
                    !response ||
                    !response.success
                ) {

                    resultContainer.innerHTML = `
                        <p>
                            ${
                                response &&
                                response.error
                                    ? escapeHtml(
                                        response.error
                                    )
                                    : "Không tìm thấy sách."
                            }
                        </p>
                    `;

                    return;
                }


                const books =
                    Array.isArray(response.data)
                        ? response.data
                        : [];


                // =================================
                // KHONG CO KET QUA
                // =================================

                if (books.length === 0) {

                    resultContainer.innerHTML = `
                        <p>
                            Không tìm thấy sách phù hợp.
                        </p>
                    `;

                    return;
                }


                // =================================
                // TAO BANG KET QUA
                // =================================

                let html = `

                    <p>
                        Tìm thấy
                        <strong>${books.length}</strong>
                        sách.
                    </p>

                    <table>

                        <thead>

                            <tr>

                                <th>
                                    STT
                                </th>

                                <th>
                                    Book Code
                                </th>

                                <th>
                                    Tên sách
                                </th>

                                <th>
                                    Tác giả
                                </th>

                                <th>
                                    Thể loại
                                </th>

                                <th>
                                    Năm xuất bản
                                </th>

                            </tr>

                        </thead>

                        <tbody>
                `;


                books.forEach(
                    (book, index) => {

                        html += `

                            <tr>

                                <td>
                                    ${index + 1}
                                </td>

                                <td>
                                    ${escapeHtml(
                                        book.bookCode || ""
                                    )}
                                </td>

                                <td>
                                    ${escapeHtml(
                                        book.title || ""
                                    )}
                                </td>

                                <td>
                                    ${escapeHtml(
                                        book.author || ""
                                    )}
                                </td>

                                <td>
                                    ${escapeHtml(
                                        book.category || ""
                                    )}
                                </td>

                                <td>
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


                resultContainer.innerHTML =
                    html;

            }
            catch (error) {

                console.error(
                    "MC1 ERROR:",
                    error
                );

                resultContainer.innerHTML = `
                    <p>
                        Không thể kết nối đến hệ thống.
                    </p>
                `;
            }
        }
    );
}
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

    const button =
        document.getElementById(
            "btnSearchBookByYear"
        );

    const resultContainer =
        document.getElementById(
            "searchBookByYearResult"
        );


    // =================================
    // KIEM TRA COMPONENT MC2
    // =================================

    if (
        !fromYear ||
        !toYear ||
        !yearSortOrder ||
        !button ||
        !resultContainer
    ) {

        console.warn(
            "Khong tim thay giao dien MC2."
        );

        return;
    }


    console.log(
        "=== MC2 DA KHOI DONG ==="
    );


    // =================================
    // SU KIEN NUT TIM KIEM
    // =================================

    button.addEventListener(
        "click",
        async () => {

            // =============================
            // LAY DU LIEU NHAP
            // =============================

            const yearStart =
                parseInt(
                    fromYear.value,
                    10
                );

            const yearEnd =
                parseInt(
                    toYear.value,
                    10
                );

            const sortOrder =
                yearSortOrder.value;


            // =============================
            // KIEM TRA NAM
            // =============================

            if (
                Number.isNaN(yearStart) ||
                Number.isNaN(yearEnd)
            ) {

                resultContainer.innerHTML = `
                    <p>
                        Vui long nhap day du nam bat dau
                        va nam ket thuc.
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


            // =============================
            // HIEN THI DANG TIM
            // =============================

            resultContainer.innerHTML = `
                <p>
                    Dang tim kiem...
                </p>
            `;


            try {

                // =====================================
                // GUI REQUEST DEN API.PHP
                // =====================================

                const response =
                    await sendApiRequest({

                        action:
                            "getBooksByYearRange",

                        yearStart:
                            yearStart,

                        yearEnd:
                            yearEnd
                    });


                console.log(
                    "MC2 response:",
                    response
                );


                // =====================================
                // API LOI
                // =====================================

                if (
                    !response ||
                    !response.success
                ) {

                    resultContainer.innerHTML = `
                        <p>
                            ${
                                response &&
                                response.error
                                    ? escapeHtml(
                                        response.error
                                    )
                                    : "Khong the tim kiem sach."
                            }
                        </p>
                    `;

                    return;
                }


                // =====================================
                // LAY DANH SACH
                // =====================================

                let books =
                    Array.isArray(response.data)
                        ? response.data
                        : [];


                // =====================================
                // SAP XEP
                // =====================================

                books.sort(
                    (a, b) => {

                        const yearA =
                            Number(a.year) || 0;

                        const yearB =
                            Number(b.year) || 0;


                        // Neu khac nam
                        if (
                            yearA !== yearB
                        ) {

                            return sortOrder === "asc"
                                ? yearA - yearB
                                : yearB - yearA;
                        }


                        // Neu cung nam
                        // thi sap xep BookCode

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
                // KHONG CO KET QUA
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
                // TAO BANG
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
                            width: 100%;
                            border-collapse: collapse;
                            margin-top: 15px;
                        "
                    >

                        <thead>

                            <tr>

                                <th style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                ">
                                    STT
                                </th>

                                <th style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                ">
                                    Book Code
                                </th>

                                <th style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                ">
                                    Ten sach
                                </th>

                                <th style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                ">
                                    Tac gia
                                </th>

                                <th style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                ">
                                    The loai
                                </th>

                                <th style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                ">
                                    Nam xuat ban
                                </th>

                            </tr>

                        </thead>

                        <tbody>
                `;


                // =====================================
                // HIEN THI TUNG SACH
                // =====================================

                books.forEach(
                    (book, index) => {

                        html += `

                            <tr>

                                <td style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                    text-align: center;
                                ">
                                    ${index + 1}
                                </td>

                                <td style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                ">
                                    ${escapeHtml(
                                        book.bookCode || ""
                                    )}
                                </td>

                                <td style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                ">
                                    ${escapeHtml(
                                        book.title || ""
                                    )}
                                </td>

                                <td style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                ">
                                    ${escapeHtml(
                                        book.author || ""
                                    )}
                                </td>

                                <td style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                ">
                                    ${escapeHtml(
                                        book.category || ""
                                    )}
                                </td>

                                <td style="
                                    border: 1px solid #ccc;
                                    padding: 10px;
                                    text-align: center;
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
                // HIEN THI LEN GIAO DIEN
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
                        Khong the ket noi den he thong.
                    </p>
                `;
            }
        }
    );
}


// =====================================================
// HAM BAO VE HTML
// =====================================================

function escapeHtml(value) {

    return String(value)
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}
