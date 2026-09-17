// =====================================
// COMPONENT LOADER
// BUOI 50
// Nap cac HTML component vao index.html
// =====================================


// =====================================
// HAM LOAD MOT COMPONENT
// =====================================

async function loadComponent(
    elementId,
    filePath
) {
    try {

        const element =
            document.querySelector("#" + elementId);


        // =============================
        // KIEM TRA ELEMENT
        // =============================

        if (!element) {

            console.error(
                "Khong tim thay element:",
                elementId
            );

            return false;
        }


        // =============================
        // FETCH FILE HTML
        // =============================

        const response =
            await fetch(filePath);


        // =============================
        // KIEM TRA RESPONSE
        // =============================

        if (!response.ok) {

            throw new Error(
                "HTTP error: "
                + response.status
            );
        }


        // =============================
        // DOC NOI DUNG HTML
        // =============================

        const html =
            await response.text();


        // =============================
        // DUA HTML VAO ELEMENT
        // =============================

        element.innerHTML = html;


        console.log(
            "Da load component:",
            filePath
        );


        return true;
    }
    catch (error) {

        console.error(
            "Khong the load component:",
            filePath,
            error
        );


        return false;
    }
}


// =====================================
// LOAD TAT CA COMPONENT
// =====================================

export async function loadComponents() {

    console.log(
        "=== LOAD COMPONENTS ==="
    );


    // =================================
    // HEADER
    // =================================

    const headerLoaded =
        await loadComponent(
            "header",
            "./components/header.html"
        );


    // =================================
    // SIDEBAR
    // =================================

    const sidebarLoaded =
        await loadComponent(
            "sidebar",
            "./components/sidebar.html"
        );


    // =================================
    // HOME TABS
    // =================================

    const homeTabsLoaded =
        await loadComponent(
            "homeTabs",
            "./components/home-tabs.html"
        );


    // =================================
    // ALL BOOKS
    // =================================

    const allBooksLoaded =
        await loadComponent(
            "allBooks",
            "./components/all-books.html"
        );


    // =================================
    // TRENDING BOOKS
    // =================================

    const trendingBooksLoaded =
        await loadComponent(
            "trendingBooks",
            "./components/trending-books.html"
        );


    // =================================
    // BOOK MANAGEMENT
    // QUAN LY SACH
    // =================================

    const bookManagementLoaded =
        await loadComponent(
            "bookManagement",
            "./components/book-management.html"
        );


    // =================================
    // SEARCH BOOK
    // =================================

    const searchBookLoaded =
        await loadComponent(
            "searchBook",
            "./components/search-book.html"
        );


    // =================================
    // SEARCH BOOK BY YEAR
    // =================================

    const searchBookByYearLoaded =
        await loadComponent(
            "searchBookByYear",
            "./components/search-book-by-year.html"
        );


    // =================================
    // BORROW BOOK
    // =================================

    const borrowBookLoaded =
        await loadComponent(
            "borrowBook",
            "./components/borrow-book.html"
        );


    // =================================
    // RESERVATION
    // DANG KY CHO MUON SACH
    // =================================

    const reservationLoaded =
        await loadComponent(
            "reservation",
            "./components/reservation.html"
        );


    // =================================
    // RETURN BOOK
    // =================================

    const returnBookLoaded =
        await loadComponent(
            "returnBook",
            "./components/return-book.html"
        );


    // =================================
    // LOAN SLIP
    // PHIEU MUON
    // =================================

    const loanSlipLoaded =
        await loadComponent(
            "loanSlip",
            "./components/loan-slip.html"
        );


    // =================================
    // FINE
    // =================================

    const fineLoaded =
        await loadComponent(
            "fine",
            "./components/fine.html"
        );


    // =================================
    // FOOTER
    // =================================

    const footerLoaded =
        await loadComponent(
            "footer",
            "./components/footer.html"
        );


    // =================================
    // KIEM TRA TOAN BO
    // =================================

    if (
        headerLoaded &&
        sidebarLoaded &&
        homeTabsLoaded &&
        allBooksLoaded &&
        trendingBooksLoaded &&
        bookManagementLoaded &&
        searchBookLoaded &&
        searchBookByYearLoaded &&
        borrowBookLoaded &&
        reservationLoaded &&
        returnBookLoaded &&
        loanSlipLoaded &&
        fineLoaded &&
        footerLoaded
    ) {

        console.log(
            "Tat ca component da load thanh cong."
        );


        return true;
    }


    console.warn(
        "Co component chua load thanh cong."
    );


    return false;
}