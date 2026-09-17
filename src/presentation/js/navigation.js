// =====================================
// NAVIGATION MODULE
// BUOI 50
// DIEU HUONG GIUA CAC CHUC NANG
// + 2 TAB TRANG CHU
// =====================================


// =====================================
// CAC KHU VUC CHUC NANG
// =====================================

const sectionIds = [
    "homeTabs",
    "allBooks",
    "trendingBooks",
    "bookManagement",
    "searchBook",
    "searchBookByYear",
    "borrowBook",
    "reservation",
    "returnBook",
    "loanSlip",
    "fine"
];


// =====================================
// AN TAT CA KHU VUC
// =====================================

function hideAllSections() {

    sectionIds.forEach(id => {

        const section =
            document.querySelector("#" + id);

        if (section) {
            section.style.display = "none";
        }
    });
}


// =====================================
// HIEN MOT KHU VUC
// =====================================

function showSection(id) {

    const section =
        document.querySelector("#" + id);

    if (section) {
        section.style.display = "block";
    }
}


// =====================================
// BO ACTIVE TAT CA MENU
// =====================================

function removeActiveMenu() {

    const menuItems =
        document.querySelectorAll(
            ".sidebar-item"
        );

    menuItems.forEach(item => {
        item.classList.remove("active");
    });
}


// =====================================
// DAT MENU ACTIVE
// =====================================

function setActiveMenu(menuId) {

    removeActiveMenu();

    const menu =
        document.querySelector(
            "#" + menuId
        );

    if (menu) {
        menu.classList.add("active");
    }
}


// =====================================
// BO ACTIVE TAT CA TAB TRANG CHU
// =====================================

function removeActiveHomeTab() {

    const tabs =
        document.querySelectorAll(
            ".home-tab"
        );

    tabs.forEach(tab => {
        tab.classList.remove("active");
    });
}


// =====================================
// DAT TAB TRANG CHU ACTIVE
// =====================================

function setActiveHomeTab(tabId) {

    removeActiveHomeTab();

    const tab =
        document.querySelector(
            "#" + tabId
        );

    if (tab) {
        tab.classList.add("active");
    }
}


// =====================================
// HIEN TAB TAT CA SACH
// =====================================

function showAllBooksTab() {

    showSection("allBooks");

    const trendingBooks =
        document.querySelector(
            "#trendingBooks"
        );

    if (trendingBooks) {
        trendingBooks.style.display = "none";
    }

    setActiveHomeTab(
        "tabAllBooks"
    );
}


// =====================================
// HIEN TAB SACH DANG DUOC QUAN TAM
// =====================================

function showTrendingBooksTab() {

    showSection("trendingBooks");

    const allBooks =
        document.querySelector(
            "#allBooks"
        );

    if (allBooks) {
        allBooks.style.display = "none";
    }

    setActiveHomeTab(
        "tabTrendingBooks"
    );
}


// =====================================
// KHOI TAO 2 TAB TRANG CHU
// =====================================

function initHomeTabs() {

    const tabAllBooks =
        document.querySelector(
            "#tabAllBooks"
        );

    const tabTrendingBooks =
        document.querySelector(
            "#tabTrendingBooks"
        );


    // =================================
    // KIEM TRA
    // =================================

    if (
        !tabAllBooks ||
        !tabTrendingBooks
    ) {

        console.warn(
            "Khong tim thay day du tab trang chu."
        );

        return;
    }


    // =================================
    // TAB TAT CA SACH
    // =================================

    tabAllBooks.addEventListener(
        "click",
        () => {

            showAllBooksTab();

        }
    );


    // =================================
    // TAB SACH DANG DUOC QUAN TAM
    // =================================

    tabTrendingBooks.addEventListener(
        "click",
        () => {

            showTrendingBooksTab();

        }
    );


    console.log(
        "Home tabs da san sang."
    );
}


// =====================================
// HIEN TRANG CHU
// =====================================

function showHome() {

    // An tat ca
    hideAllSections();


    // Hien thanh tab trang chu
    showSection("homeTabs");


    // Mac dinh hien tab Tat ca sach
    showAllBooksTab();


    // Dat Trang chu active
    setActiveMenu(
        "menuAllBooks"
    );
}


// =====================================
// KHOI TAO NAVIGATION
// =====================================

export function initNavigation() {

    console.log(
        "=== INIT NAVIGATION MODULE ==="
    );


    // =================================
    // KHOI TAO 2 TAB TRANG CHU
    // =================================

    initHomeTabs();


    // =================================
    // MENU TRANG CHU
    // =================================

    const menuAllBooks =
        document.querySelector(
            "#menuAllBooks"
        );

    if (menuAllBooks) {

        menuAllBooks.addEventListener(
            "click",
            () => {

                showHome();

            }
        );
    }


    // =================================
    // MENU QUAN LY SACH
    // =================================

    const menuBookManagement =
        document.querySelector(
            "#menuBookManagement"
        );

    if (menuBookManagement) {

        menuBookManagement.addEventListener(
            "click",
            () => {

                hideAllSections();

                showSection(
                    "bookManagement"
                );

                setActiveMenu(
                    "menuBookManagement"
                );

            }
        );
    }


    // =================================
    // MENU TIM SACH
    // =================================

    const menuSearchBook =
        document.querySelector(
            "#menuSearchBook"
        );

    if (menuSearchBook) {

        menuSearchBook.addEventListener(
            "click",
            () => {

                hideAllSections();

                showSection(
                    "searchBook"
                );

                setActiveMenu(
                    "menuSearchBook"
                );

            }
        );
    }


    // =================================
    // MENU TIM THEO NAM
    // =================================

    const menuSearchBookByYear =
        document.querySelector(
            "#menuSearchBookByYear"
        );

    if (menuSearchBookByYear) {

        menuSearchBookByYear.addEventListener(
            "click",
            () => {

                hideAllSections();

                showSection(
                    "searchBookByYear"
                );

                setActiveMenu(
                    "menuSearchBookByYear"
                );

            }
        );
    }


    // =================================
    // MENU MUON SACH
    // =================================

    const menuBorrowBook =
        document.querySelector(
            "#menuBorrowBook"
        );

    if (menuBorrowBook) {

        menuBorrowBook.addEventListener(
            "click",
            () => {

                hideAllSections();

                showSection(
                    "borrowBook"
                );

                setActiveMenu(
                    "menuBorrowBook"
                );

            }
        );
    }


    // =================================
    // MENU DANG KY CHO MUON
    // =================================

    const menuReservation =
        document.querySelector(
            "#menuReservation"
        );

    if (menuReservation) {

        menuReservation.addEventListener(
            "click",
            () => {

                hideAllSections();

                showSection(
                    "reservation"
                );

                setActiveMenu(
                    "menuReservation"
                );

            }
        );
    }


    // =================================
    // MENU TRA SACH
    // =================================

    const menuReturnBook =
        document.querySelector(
            "#menuReturnBook"
        );

    if (menuReturnBook) {

        menuReturnBook.addEventListener(
            "click",
            () => {

                hideAllSections();

                showSection(
                    "returnBook"
                );

                setActiveMenu(
                    "menuReturnBook"
                );

            }
        );
    }


    // =================================
    // MENU PHIEU MUON
    // CHI XEM CAC SACH DANG MUON
    // =================================

    const menuLoanSlip =
        document.querySelector(
            "#menuLoanSlip"
        );

    if (menuLoanSlip) {

        menuLoanSlip.addEventListener(
            "click",
            () => {

                hideAllSections();

                showSection(
                    "loanSlip"
                );

                setActiveMenu(
                    "menuLoanSlip"
                );

            }
        );
    }


    // =================================
    // MENU TIEN PHAT
    // =================================

    const menuFine =
        document.querySelector(
            "#menuFine"
        );

    if (menuFine) {

        menuFine.addEventListener(
            "click",
            () => {

                hideAllSections();

                showSection(
                    "fine"
                );

                setActiveMenu(
                    "menuFine"
                );

            }
        );
    }

    const homeManagementButton =
        document.querySelector(
            "#homeManagementButton"
        );

    if (homeManagementButton) {

        homeManagementButton.addEventListener(
            "click",
            () => {

                hideAllSections();

                showSection(
                    "bookManagement"
                );

                setActiveMenu(
                    "menuBookManagement"
                );
            }
        );
    }
    // =================================
    // TRANG CHU MAC DINH
    // =================================

    showHome();


    console.log(
        "Navigation module da san sang."
    );
}