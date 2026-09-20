import { sendApiRequest } from "./api.js";

export function initMemberRegister() {
    const form = document.querySelector("#memberRegisterForm");
    const message = document.querySelector("#memberRegisterMessage");
    const result = document.querySelector("#memberRegisterResult");

    if (!form || !message || !result) {
        console.error("Khong tim thay giao dien dang ky thanh vien.");
        return;
    }

    form.addEventListener("submit", async (event) => {
        event.preventDefault();

        message.textContent = "Dang xu ly...";
        message.className = "member-register-message";
        result.style.display = "none";

        const name = document.querySelector("#memberName").value.trim();
        const email = document.querySelector("#memberEmail").value.trim();
        const phone = document.querySelector("#memberPhone").value.trim();

        if (!name || !email || !phone) {
            message.textContent = "Vui lòng nhập đầy đủ họ tên, email và số điện thoại.";
            message.className = "member-register-message error";
            return;
        }

        const response = await sendApiRequest({
            action: "registerMember",
            name,
            email,
            phone
        });

        if (!response.success) {
            message.textContent = response.error || response.message || "Đăng ký thành viên thất bại.";
            message.className = "member-register-message error";
            return;
        }

        const member = response.data;
        message.textContent = "Đăng ký thành viên thành công.";
        message.className = "member-register-message success";

        result.innerHTML = `
            <h3>Thông tin thành viên mới</h3>
            <p><strong>Member_ID:</strong> ${escapeHtml(member.memberId)}</p>
            <p><strong>Họ tên:</strong> ${escapeHtml(member.name)}</p>
            <p><strong>Email:</strong> ${escapeHtml(member.email)}</p>
            <p><strong>Số điện thoại:</strong> ${escapeHtml(member.phone)}</p>
            <p><strong>Trạng thái:</strong> ${escapeHtml(member.status)}</p>
        `;
        result.style.display = "block";

        form.reset();
    });
}

function escapeHtml(value) {
    return String(value)
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");
}
