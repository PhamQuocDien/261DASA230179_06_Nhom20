import { sendApiRequest } from "./api.js";
function isAdminAuthenticated() {
    return sessionStorage.getItem("adminAuthenticated") === "true";
}
function showAdminLogin() {
    const modal = document.querySelector("#adminLoginModal");
    const adminId = document.querySelector("#adminId");
    const adminPassword = document.querySelector("#adminPassword");
    const adminLoginMessage = document.querySelector("#adminLoginMessage");
    if (!modal) {
        console.warn("Khong tim thay form dang nhap quan ly.");
        return;
    }
    modal.style.display = "flex";
    if (adminId) {
        adminId.value = "";
        adminId.focus();
    }
    if (adminPassword) {
        adminPassword.value = "";
    }
    if (adminLoginMessage) {
        adminLoginMessage.textContent = "";
    }
}
function hideAdminLogin() {
    const modal = document.querySelector("#adminLoginModal");
    if (modal) {
        modal.style.display = "none";
    }
}
function showBookManagement() {
    hideAllSections();
    showSection("bookManagement");
    setActiveMenu("menuBookManagement");
}
async function loginAdmin() {
    const adminId = document.querySelector("#adminId");
    const adminPassword = document.querySelector("#adminPassword");
    const adminLoginMessage = document.querySelector("#adminLoginMessage");
    if (!adminId || !adminPassword) {
        console.warn("Khong tim thay du lieu dang nhap.");
        return false;
    }
    const id = adminId.value.trim();
    const password = adminPassword.value;
    if (id === "") {
        if (adminLoginMessage) {
            adminLoginMessage.textContent = "Vui long nhap ID quan ly.";
        }
        adminId.focus();
        return false;
    }
    if (password === "") {
        if (adminLoginMessage) {
            adminLoginMessage.textContent = "Vui long nhap mat khau.";
        }
        adminPassword.focus();
        return false;
    }
    const result = await sendApiRequest({ action: "loginAdmin", idAdmin: id, password: password });
    if (!result.success) {
        console.error("Dang nhap quan ly that bai:", result.error);
        if (adminLoginMessage) {
            adminLoginMessage.textContent = result.error || "Thong tin dang nhap khong dung.";
        }
        adminPassword.value = "";
        adminPassword.focus();
        return false;
    }
    sessionStorage.setItem("adminAuthenticated", "true");
    console.log("Dang nhap quan ly thanh cong.");
    hideAdminLogin();
    showBookManagement();
    return true;
}
function showMemberRegistration(memberId) {
    hideAllSections();
    showSection("memberRegistration");
    setActiveMenu("menuMemberRegistration");
    const memberName = document.querySelector("#memberName");
    if (memberName) {
        memberName.focus();
    }
    console.log("Da chuyen sang trang Dang ky thanh vien.", memberId || "");
}
const sectionIds = ["homeTabs", "allBooks", "trendingBooks", "bookManagement", "searchBook", "searchBookByYear", "borrowBook", "reservation", "returnBook", "loanSlip", "fine", "memberRegistration"];
let homeBooks = [];
let currentHomePage = 1;
const homeBooksPerPage = 10;
let currentHomeFilterType = "all";
function escapeHomeBookValue(value) {
    return String(value ?? "").replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/"/g, "&quot;").replace(/'/g, "&#039;");
}
function showBookIdsModal(book) {
    const bookIds = Array.isArray(book.bookIds) ? book.bookIds : [];
    const oldModal = document.querySelector("#bookIdsModal");
    if (oldModal) {
        oldModal.remove();
    }
    if (bookIds.length === 0) {
        return;
    }
    const modal = document.createElement("div");
    modal.id = "bookIdsModal";
    modal.style.position = "fixed";
    modal.style.top = "0";
    modal.style.left = "0";
    modal.style.width = "100%";
    modal.style.height = "100%";
    modal.style.background = "rgba(0, 0, 0, 0.5)";
    modal.style.display = "flex";
    modal.style.alignItems = "center";
    modal.style.justifyContent = "center";
    modal.style.zIndex = "9999";
    const bookIdsHtml = bookIds.map((bookId, index) => `<div style="padding: 10px 12px; border-bottom: 1px solid #e2e8f0; color: #0f172a; background: #ffffff; font-size: 14px; font-weight: 600;">${index + 1}. ${escapeHomeBookValue(bookId)}</div>`).join("");
    modal.innerHTML = `
        <div style="background: white; width: min(500px, 90%); max-height: 80vh; border-radius: 10px; padding: 20px; box-sizing: border-box; box-shadow: 0 10px 30px rgba(0,0,0,0.2); display: flex; flex-direction: column;">
            <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 15px;">
                <div>
                    <h3 style="margin: 0; color: #0f172a; font-size: 20px; font-weight: 800;">Danh sách Book_ID</h3>
                    <p style="margin: 5px 0 0; color: #475569; font-size: 14px;">${escapeHomeBookValue(book.title)}</p>
                </div>
                <button type="button" id="closeBookIdsModal" style="border: none; background: transparent; color: #475569; font-size: 24px; cursor: pointer; line-height: 1;">×</button>
            </div>
            <div style="overflow-y: auto; border: 1px solid #cbd5e1; border-radius: 6px; background: #ffffff;">
                ${bookIdsHtml}
            </div>
        </div>
    `;
    document.body.appendChild(modal);
    const closeButton = document.querySelector("#closeBookIdsModal");
    if (closeButton) {
        closeButton.addEventListener("click", () => {
            modal.remove();
        });
    }
    modal.addEventListener("click", event => {
        if (event.target === modal) {
            modal.remove();
        }
    });
    const closeOnEscape = event => {
        if (event.key === "Escape") {
            modal.remove();
            document.removeEventListener("keydown", closeOnEscape);
        }
    };
    document.addEventListener("keydown", closeOnEscape);
}
function updateHomeBooksHeader(title, description) {
    const titleElement = document.querySelector("#allBooks .section-header h2");
    const descriptionElement = document.querySelector("#allBooks .section-header p");
    if (titleElement) {
        titleElement.textContent = title;
    }
    if (descriptionElement) {
        descriptionElement.textContent = description;
    }
}
function updateHomeBookTableHeader(filterType) {
    const bookIdHeader = document.querySelector("#bookIdHeader");
    const bookQuantityHeader = document.querySelector("#bookQuantityHeader");
    if (bookIdHeader) {
        if (filterType === "available" || filterType === "borrowed") {
            bookIdHeader.textContent = "Chi tiết Book_ID";
            bookIdHeader.style.display = "table-cell";
        } else {
            bookIdHeader.style.display = "none";
        }
    }
    if (bookQuantityHeader) {
        if (filterType === "available") {
            bookQuantityHeader.textContent = "Số lượng có thể mượn";
            bookQuantityHeader.style.display = "table-cell";
        } else if (filterType === "borrowed") {
            bookQuantityHeader.textContent = "Số lượng đang được mượn";
            bookQuantityHeader.style.display = "table-cell";
        } else {
            bookQuantityHeader.style.display = "none";
        }
    }
}
function renderHomeBookPage() {
    const bookTableBody = document.querySelector("#bookTableBody");
    if (!bookTableBody) {
        console.warn("Khong tim thay #bookTableBody.");
        return;
    }
    updateHomeBookTableHeader(currentHomeFilterType);
    if (!Array.isArray(homeBooks) || homeBooks.length === 0) {
        const colspan = currentHomeFilterType === "available" || currentHomeFilterType === "borrowed" ? 7 : 5;
        bookTableBody.innerHTML = `<tr><td colspan="${colspan}" style="text-align: center;">Không có sách phù hợp.</td></tr>`;
        renderHomeBookPagination();
        return;
    }
    const totalPages = Math.ceil(homeBooks.length / homeBooksPerPage);
    if (currentHomePage > totalPages) {
        currentHomePage = totalPages;
    }
    if (currentHomePage < 1) {
        currentHomePage = 1;
    }
    const startIndex = (currentHomePage - 1) * homeBooksPerPage;
    const endIndex = startIndex + homeBooksPerPage;
    const pageBooks = homeBooks.slice(startIndex, endIndex);
    bookTableBody.innerHTML = pageBooks.map(book => {
        if (currentHomeFilterType === "available" || currentHomeFilterType === "borrowed") {
            const bookIds = Array.isArray(book.bookIds) ? book.bookIds : [];
            const bookIdHtml = bookIds.length > 0 ? `<button type="button" class="book-id-detail-button" data-book-code="${escapeHomeBookValue(book.bookCode)}">Xem ${bookIds.length} Book_ID</button>` : "-";
            const quantity = currentHomeFilterType === "available" ? (Number.isInteger(book.availableQuantity) ? book.availableQuantity : 0) : (Number.isInteger(book.borrowedQuantity) ? book.borrowedQuantity : 0);
            return `<tr><td>${bookIdHtml}</td><td>${escapeHomeBookValue(book.bookCode)}</td><td>${escapeHomeBookValue(book.title)}</td><td>${escapeHomeBookValue(book.author)}</td><td>${escapeHomeBookValue(book.category)}</td><td>${escapeHomeBookValue(book.year)}</td><td>${quantity}</td></tr>`;
        }
        return `<tr><td>${escapeHomeBookValue(book.bookCode)}</td><td>${escapeHomeBookValue(book.title)}</td><td>${escapeHomeBookValue(book.author)}</td><td>${escapeHomeBookValue(book.category)}</td><td>${escapeHomeBookValue(book.year)}</td></tr>`;
    }).join("");
    document.querySelectorAll(".book-id-detail-button").forEach(button => {
        button.addEventListener("click", () => {
            const bookCode = button.dataset.bookCode;
            const book = homeBooks.find(item => String(item.bookCode) === String(bookCode));
            if (book) {
                showBookIdsModal(book);
            }
        });
    });
    renderHomeBookPagination();
}
function renderHomeBookPagination() {
    const pagination = document.querySelector("#bookPagination");
    if (!pagination) {
        return;
    }
    pagination.innerHTML = "";
    if (!Array.isArray(homeBooks) || homeBooks.length === 0) {
        pagination.style.display = "none";
        return;
    }
    const totalPages = Math.ceil(homeBooks.length / homeBooksPerPage);
    if (totalPages <= 1) {
        pagination.style.display = "none";
        return;
    }
    pagination.style.display = "flex";
    const previousButton = document.createElement("button");
    previousButton.type = "button";
    previousButton.className = "book-pagination-button";
    previousButton.textContent = "‹";
    previousButton.title = "Trang trước";
    previousButton.disabled = currentHomePage === 1;
    previousButton.addEventListener("click", () => {
        if (currentHomePage > 1) {
            currentHomePage--;
            renderHomeBookPage();
        }
    });
    pagination.appendChild(previousButton);
    const pages = [];
    if (totalPages <= 7) {
        for (let i = 1; i <= totalPages; i++) {
            pages.push(i);
        }
    } else {
        pages.push(1);
        if (currentHomePage > 4) {
            pages.push("...");
        }
        const startPage = Math.max(2, currentHomePage - 1);
        const endPage = Math.min(totalPages - 1, currentHomePage + 1);
        for (let i = startPage; i <= endPage; i++) {
            pages.push(i);
        }
        if (currentHomePage < totalPages - 3) {
            pages.push("...");
        }
        pages.push(totalPages);
    }
    pages.forEach(page => {
        if (page === "...") {
            const ellipsis = document.createElement("span");
            ellipsis.className = "book-pagination-ellipsis";
            ellipsis.textContent = "...";
            pagination.appendChild(ellipsis);
            return;
        }
        const pageButton = document.createElement("button");
        pageButton.type = "button";
        pageButton.className = "book-pagination-button";
        pageButton.textContent = String(page);
        if (page === currentHomePage) {
            pageButton.classList.add("active");
        }
        pageButton.addEventListener("click", () => {
            currentHomePage = page;
            renderHomeBookPage();
        });
        pagination.appendChild(pageButton);
    });
    const nextButton = document.createElement("button");
    nextButton.type = "button";
    nextButton.className = "book-pagination-button";
    nextButton.textContent = "›";
    nextButton.title = "Trang sau";
    nextButton.disabled = currentHomePage === totalPages;
    nextButton.addEventListener("click", () => {
        if (currentHomePage < totalPages) {
            currentHomePage++;
            renderHomeBookPage();
        }
    });
    pagination.appendChild(nextButton);
}
function renderHomeBooks(books, filterType = "all") {
    homeBooks = Array.isArray(books) ? books : [];
    currentHomeFilterType = filterType;
    currentHomePage = 1;
    renderHomeBookPage();
}
async function loadHomeBooks(filterType) {
    let action = "getBooks";
    if (filterType === "available") {
        action = "getAvailableBooks";
    } else if (filterType === "borrowed") {
        action = "getBorrowedBooks";
    }
    const result = await sendApiRequest({ action: action });
    if (!result.success) {
        console.error("Khong the tai danh sach sach:", result.error);
        return;
    }
    if (!Array.isArray(result.data)) {
        console.error("Du lieu sach tra ve khong hop le.");
        return;
    }
    renderHomeBooks(result.data, filterType);
    console.log("Da tai du lieu trang chu:", filterType, result.data);
}
function hideAllSections() {
    sectionIds.forEach(id => {
        const section = document.querySelector("#" + id);
        if (section) {
            section.style.display = "none";
        }
    });
}
function showSection(id) {
    const section = document.querySelector("#" + id);
    if (section) {
        section.style.display = "block";
    }
}
function removeActiveMenu() {
    const menuItems = document.querySelectorAll(".sidebar-item");
    menuItems.forEach(item => {
        item.classList.remove("active");
    });
}
function setActiveMenu(menuId) {
    removeActiveMenu();
    const menu = document.querySelector("#" + menuId);
    if (menu) {
        menu.classList.add("active");
    }
}
function removeActiveHomeTab() {
    const tabs = document.querySelectorAll(".home-tab");
    tabs.forEach(tab => {
        tab.classList.remove("active");
    });
}
function setActiveHomeTab(tabId) {
    removeActiveHomeTab();
    const tab = document.querySelector("#" + tabId);
    if (tab) {
        tab.classList.add("active");
    }
}
function showAllBooksTab() {
    showSection("allBooks");
    const trendingBooks = document.querySelector("#trendingBooks");
    if (trendingBooks) {
        trendingBooks.style.display = "none";
    }
    updateHomeBooksHeader("Tất cả sách trong thư viện", "Danh sách toàn bộ sách hiện có trong thư viện");
    setActiveHomeTab("tabAllBooks");
    loadHomeBooks("all");
}
function showAvailableBooksTab() {
    showSection("allBooks");
    const trendingBooks = document.querySelector("#trendingBooks");
    if (trendingBooks) {
        trendingBooks.style.display = "none";
    }
    updateHomeBooksHeader("Sách có thể mượn", "Danh sách đầu sách có ít nhất một cuốn sách có thể mượn");
    setActiveHomeTab("tabAvailableBooks");
    loadHomeBooks("available");
}
function showBorrowedBooksTab() {
    showSection("allBooks");
    const trendingBooks = document.querySelector("#trendingBooks");
    if (trendingBooks) {
        trendingBooks.style.display = "none";
    }
    updateHomeBooksHeader("Sách đang được mượn", "Danh sách đầu sách có ít nhất một cuốn sách đang được mượn");
    setActiveHomeTab("tabBorrowedBooks");
    loadHomeBooks("borrowed");
}
function showTrendingBooksTab() {
    showSection("trendingBooks");
    const allBooks = document.querySelector("#allBooks");
    if (allBooks) {
        allBooks.style.display = "none";
    }
    setActiveHomeTab("tabTrendingBooks");
}
function initHomeTabs() {
    const tabAllBooks = document.querySelector("#tabAllBooks");
    const tabTrendingBooks = document.querySelector("#tabTrendingBooks");
    const tabAvailableBooks = document.querySelector("#tabAvailableBooks");
    const tabBorrowedBooks = document.querySelector("#tabBorrowedBooks");
    if (!tabAllBooks || !tabTrendingBooks || !tabAvailableBooks || !tabBorrowedBooks) {
        console.warn("Khong tim thay day du tab trang chu.");
        return;
    }
    tabAllBooks.addEventListener("click", () => { showAllBooksTab(); });
    tabAvailableBooks.addEventListener("click", () => { showAvailableBooksTab(); });
    tabBorrowedBooks.addEventListener("click", () => { showBorrowedBooksTab(); });
    tabTrendingBooks.addEventListener("click", () => { showTrendingBooksTab(); });
    console.log("Home tabs da san sang.");
}
function showHome() {
    hideAllSections();
    showSection("homeTabs");
    showAllBooksTab();
    setActiveMenu("menuAllBooks");
}
export function initNavigation() {
    console.log("=== INIT NAVIGATION MODULE ===");
    initHomeTabs();
    const menuAllBooks = document.querySelector("#menuAllBooks");
    if (menuAllBooks) {
        menuAllBooks.addEventListener("click", () => { showHome(); });
    }
    const menuBookManagement = document.querySelector("#menuBookManagement");
    if (menuBookManagement) {
        menuBookManagement.addEventListener("click", () => {
            if (!isAdminAuthenticated()) {
                showAdminLogin();
                return;
            }
            showBookManagement();
        });
    }
    const menuMemberRegistration = document.querySelector("#menuMemberRegistration");
    if (menuMemberRegistration) {
        menuMemberRegistration.addEventListener("click", () => { showMemberRegistration(); });
    }
    const menuSearchBook = document.querySelector("#menuSearchBook");
    if (menuSearchBook) {
        menuSearchBook.addEventListener("click", () => {
            hideAllSections();
            showSection("searchBook");
            setActiveMenu("menuSearchBook");
        });
    }
    const menuSearchBookByYear = document.querySelector("#menuSearchBookByYear");
    if (menuSearchBookByYear) {
        menuSearchBookByYear.addEventListener("click", () => {
            hideAllSections();
            showSection("searchBookByYear");
            setActiveMenu("menuSearchBookByYear");
        });
    }
    const menuBorrowBook = document.querySelector("#menuBorrowBook");
    if (menuBorrowBook) {
        menuBorrowBook.addEventListener("click", () => {
            hideAllSections();
            showSection("borrowBook");
            setActiveMenu("menuBorrowBook");
        });
    }
    const menuReservation = document.querySelector("#menuReservation");
    if (menuReservation) {
        menuReservation.addEventListener("click", () => {
            hideAllSections();
            showSection("reservation");
            setActiveMenu("menuReservation");
        });
    }
    const menuReturnBook = document.querySelector("#menuReturnBook");
    if (menuReturnBook) {
        menuReturnBook.addEventListener("click", () => {
            hideAllSections();
            showSection("returnBook");
            setActiveMenu("menuReturnBook");
        });
    }
    const menuLoanSlip = document.querySelector("#menuLoanSlip");
    if (menuLoanSlip) {
        menuLoanSlip.addEventListener("click", () => {
            hideAllSections();
            showSection("loanSlip");
            setActiveMenu("menuLoanSlip");
        });
    }
    const menuFine = document.querySelector("#menuFine");
    if (menuFine) {
        menuFine.addEventListener("click", () => {
            hideAllSections();
            showSection("fine");
            setActiveMenu("menuFine");
        });
    }
    const homeManagementButton = document.querySelector("#homeManagementButton");
    if (homeManagementButton) {
        homeManagementButton.addEventListener("click", () => {
            if (!isAdminAuthenticated()) {
                showAdminLogin();
                return;
            }
            showBookManagement();
        });
    }
    const btnAdminLogin = document.querySelector("#btnAdminLogin");
    if (btnAdminLogin) {
        btnAdminLogin.addEventListener("click", async () => { await loginAdmin(); });
    }
    const btnAdminCancel = document.querySelector("#btnAdminCancel");
    if (btnAdminCancel) {
        btnAdminCancel.addEventListener("click", () => { hideAdminLogin(); });
    }
    const adminPassword = document.querySelector("#adminPassword");
    if (adminPassword) {
        adminPassword.addEventListener("keydown", async event => {
            if (event.key === "Enter") {
                await loginAdmin();
            }
        });
    }
    window.addEventListener("openMemberRegistration", event => {
        const memberId = event.detail && event.detail.memberId ? event.detail.memberId : "";
        if (!memberId) {
            return;
        }
        showMemberRegistration(memberId);
    });
    showHome();
    console.log("Navigation module da san sang.");
}
