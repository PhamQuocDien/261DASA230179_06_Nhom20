import { sendApiRequest } from "./api.js";

function escapeHtml(value) {
    return String(value ?? "")
        .replaceAll("&", "&")
        .replaceAll("<", "<")
        .replaceAll(">", ">")
        .replaceAll('"', """)
        .replaceAll("'", "'");
}

export function initBorrowBook() {
    const btn = document.querySelector("#btnBorrowBook");
    const memberInput = document.querySelector("#borrowMemberId");
    const bookInput = document.querySelector("#borrowBookId");
    const resultBox = document.querySelector("#borrowBookResult");

    if (!btn || !memberInput || !bookInput || !resultBox) {
        console.warn("BorrowBook: Khong tim thay giao dien muon sach.");
        return;
    }

    if (btn.dataset.initialized === "true") {
        return;
    }
    btn.dataset.initialized = "true";

    const handleBorrow = async () => {
        const memberId = memberInput.value.trim();
        const bookCode = bookInput.value.trim();

        if (!memberId || !bookCode) {
            resultBox.innerHTML = `
