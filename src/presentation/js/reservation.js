// =====================================
// RESERVATION
// DANG KY CHO MUON SACH
// =====================================


// =====================================
// API URL
// =====================================

const API_URL =
    new URL(
        "../../api/api.php",
        import.meta.url
    );


// =====================================
// GOI API
// =====================================

async function callReservationApi(request) {

    try {

        const response =
            await fetch(
                API_URL,
                {
                    method: "POST",

                    headers: {
                        "Content-Type":
                            "application/json"
                    },

                    credentials: "include",

                    body: JSON.stringify(request)
                }
            );


        const result =
            await response.json();

        return result;

    }
    catch (error) {

        console.error(
            "Reservation API error:",
            error
        );

        return {
            success: false,
            error:
                "Khong the ket noi den API."
        };
    }
}


// =====================================
// ESCAPE HTML
// =====================================

function escapeHtml(value) {

    return String(value ?? "")
        .replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;")
        .replace(/'/g, "&#039;");
}


// =====================================
// HIEN THI KET QUA DANG KY
// =====================================

function showReservationResult(
    container,
    result
) {

    if (!result.success) {

        container.innerHTML = `
            <p>
                ${escapeHtml(
                    result.error ||
                    "Dang ky that bai."
                )}
            </p>
        `;

        return;
    }


    const reservation =
        result.data;


    container.innerHTML = `
        <div>

            <p>
                <strong>
                    Dang ky thanh cong.
                </strong>
            </p>

            <p>
                Reservation_ID:
                ${escapeHtml(
                    reservation.reservationId
                )}
            </p>

            <p>
                Member_ID:
                ${escapeHtml(
                    reservation.memberId
                )}
            </p>

            <p>
                BookCode:
                ${escapeHtml(
                    reservation.bookCode
                )}
            </p>

            <p>
                Ngay dang ky:
                ${escapeHtml(
                    reservation.reservationDate
                )}
            </p>

            <p>
                Trang thai:
                ${escapeHtml(
                    reservation.status
                )}
            </p>

        </div>
    `;
}


// =====================================
// HIEN THI DANH SACH CHO
// =====================================

function showReservationQueue(
    container,
    result
) {

    if (!result.success) {

        container.innerHTML = `
            <p>
                ${escapeHtml(
                    result.error ||
                    "Khong the tra cuu."
                )}
            </p>
        `;

        return;
    }


    const reservations =
        result.data || [];


    if (reservations.length === 0) {

        container.innerHTML = `
            <p>
                Khong co thanh vien nao dang cho.
            </p>
        `;

        return;
    }


    let html = `
        <table class="reservation-table">

            <thead>

                <tr>

                    <th>STT</th>

                    <th>
                        Reservation_ID
                    </th>

                    <th>
                        Member_ID
                    </th>

                    <th>
                        BookCode
                    </th>

                    <th>
                        Ngay dang ky
                    </th>

                    <th>
                        Trang thai
                    </th>

                    <th>
                        Thao tac
                    </th>

                </tr>

            </thead>

            <tbody>
    `;


    reservations.forEach(
        (reservation, index) => {

            html += `
                <tr>

                    <td>
                        ${index + 1}
                    </td>

                    <td>
                        ${escapeHtml(
                            reservation.reservationId
                        )}
                    </td>

                    <td>
                        ${escapeHtml(
                            reservation.memberId
                        )}
                    </td>

                    <td>
                        ${escapeHtml(
                            reservation.bookCode
                        )}
                    </td>

                    <td>
                        ${escapeHtml(
                            reservation.reservationDate
                        )}
                    </td>

                    <td>
                        ${escapeHtml(
                            reservation.status
                        )}
                    </td>

                    <td>

                        ${
                            reservation.status ===
                            "WAITING"
                                ? `
                                    <button
                                        type="button"
                                        class="btn-cancel-reservation"
                                        data-reservation-id="${escapeHtml(
                                            reservation.reservationId
                                        )}"
                                        data-book-code="${escapeHtml(
                                            reservation.bookCode
                                        )}">
                                        Hủy đăng ký
                                    </button>
                                `
                                : `
                                    <span>
                                        Không thể hủy
                                    </span>
                                `
                        }

                    </td>

                </tr>
            `;
        }
    );


    html += `
            </tbody>

        </table>
    `;


    container.innerHTML =
        html;
}


// =====================================
// KHOI DONG RESERVATION
// =====================================

export function initReservation() {

    const reservationMemberId =
        document.getElementById(
            "reservationMemberId"
        );


    const reservationBookId =
        document.getElementById(
            "reservationBookId"
        );


    const reservationPassword =
        document.getElementById(
            "reservationPassword"
        );


    const btnReservation =
        document.getElementById(
            "btnReservation"
        );


    const reservationResult =
        document.getElementById(
            "reservationResult"
        );


    const reservationLookupBookId =
        document.getElementById(
            "reservationLookupBookId"
        );


    const btnReservationLookup =
        document.getElementById(
            "btnReservationLookup"
        );


    const btnNextReservation =
        document.getElementById(
            "btnNextReservation"
        );


    const reservationQueueResult =
        document.getElementById(
            "reservationQueueResult"
        );


    // =================================
    // KIEM TRA ELEMENT
    // =================================

    if (
        !reservationMemberId ||
        !reservationBookId ||
        !reservationPassword ||
        !btnReservation ||
        !reservationResult ||
        !reservationLookupBookId ||
        !btnReservationLookup ||
        !btnNextReservation ||
        !reservationQueueResult
    ) {

        console.error(
            "Khong tim thay day du element Reservation."
        );

        return;
    }


    // Tránh đăng ký sự kiện nhiều lần
    if (btnReservation.dataset.initialized === "true") {

        return;
    }

    btnReservation.dataset.initialized = "true";


    const cancelReservationModal =
        document.getElementById(
            "cancelReservationModal"
        );


    const cancelReservationModalBook =
        document.getElementById(
            "cancelReservationModalBook"
        );


    const cancelReservationPassword =
        document.getElementById(
            "cancelReservationPassword"
        );


    const cancelReservationModalMessage =
        document.getElementById(
            "cancelReservationModalMessage"
        );


    const btnCancelReservationCancel =
        document.getElementById(
            "btnCancelReservationCancel"
        );


    const btnCancelReservationConfirm =
        document.getElementById(
            "btnCancelReservationConfirm"
        );


    // Reservation_ID dang cho xac nhan huy
    let pendingCancelReservationId = "";

    // BookCode de tai lai danh sach cho
    let pendingCancelBookCode = "";


    function openCancelReservationModal(
        reservationId,
        bookCode
    ) {

        pendingCancelReservationId =
            reservationId;

        pendingCancelBookCode =
            bookCode;


        if (cancelReservationModalBook) {
            cancelReservationModalBook.textContent =
                bookCode;
        }

        if (cancelReservationModalMessage) {
            cancelReservationModalMessage.textContent =
                "";
        }

        if (cancelReservationPassword) {
            cancelReservationPassword.value =
                "";
        }

        if (cancelReservationModal) {
            cancelReservationModal.style.display =
                "flex";
        }

        if (cancelReservationPassword) {
            cancelReservationPassword.focus();
        }
    }


    function closeCancelReservationModal() {

        if (cancelReservationModal) {
            cancelReservationModal.style.display =
                "none";
        }

        if (cancelReservationPassword) {
            cancelReservationPassword.value =
                "";
        }

        pendingCancelReservationId = "";
        pendingCancelBookCode = "";
    }


    async function handleCancelReservationConfirm() {

        const password =
            cancelReservationPassword
                ? cancelReservationPassword.value
                : "";

        if (password === "") {

            if (cancelReservationModalMessage) {
                cancelReservationModalMessage.textContent =
                    "Vui long nhap mat khau thanh vien.";
            }

            return;
        }


        if (btnCancelReservationConfirm) {
            btnCancelReservationConfirm.disabled =
                true;
        }


        // -----------------------------
        // GOI API HUY
        // Mat khau chi gui len backend,
        // backend tu kiem tra chu so huu
        // -----------------------------

        const result =
            await callReservationApi(
                {
                    action:
                        "cancelReservation",

                    reservationId:
                        pendingCancelReservationId,

                    password:
                        password
                }
            );


        // Xoa mat khau khoi o nhap ngay sau khi gui
        if (cancelReservationPassword) {
            cancelReservationPassword.value =
                "";
        }


        if (btnCancelReservationConfirm) {
            btnCancelReservationConfirm.disabled =
                false;
        }


        // -----------------------------
        // THAT BAI
        // Chi hien thi message backend
        // -----------------------------

        if (!result.success) {

            if (cancelReservationModalMessage) {
                cancelReservationModalMessage.textContent =
                    result.error ||
                    "Khong the huy dang ky.";
            }

            return;
        }


        const bookCode =
            pendingCancelBookCode;

        closeCancelReservationModal();


        console.log(
            "Da huy dang ky thanh cong."
        );


        // -----------------------------
        // LOAD LAI DANH SACH
        // -----------------------------

        await loadReservationQueue(
            bookCode,
            reservationQueueResult
        );
    }


    // =================================
    // MODAL HUY CHO
    // =================================

    if (btnCancelReservationCancel) {
        btnCancelReservationCancel.addEventListener(
            "click",
            closeCancelReservationModal
        );
    }

    if (btnCancelReservationConfirm) {
        btnCancelReservationConfirm.addEventListener(
            "click",
            handleCancelReservationConfirm
        );
    }

    if (cancelReservationPassword) {
        cancelReservationPassword.addEventListener(
            "keydown",
            event => {
                if (event.key === "Enter") {
                    handleCancelReservationConfirm();
                }
            }
        );
    }


    // =================================
    // DANG KY CHO
    // =================================

    btnReservation.addEventListener(
        "click",
        async () => {

            const memberId =
                reservationMemberId.value.trim();


            const bookCode =
                reservationBookId.value.trim();


            const password =
                reservationPassword.value;


            // -----------------------------
            // KIEM TRA MEMBER_ID
            // -----------------------------

            if (!memberId) {

                reservationResult.innerHTML = `
                    <p>
                        Vui long nhap Member_ID.
                    </p>
                `;

                return;
            }


            // -----------------------------
            // KIEM TRA BOOKCODE
            // -----------------------------

            if (!bookCode) {

                reservationResult.innerHTML = `
                    <p>
                        Vui long nhap BookCode.
                    </p>
                `;

                return;
            }


            // -----------------------------
            // KIEM TRA MAT KHAU
            // -----------------------------

            if (!password) {

                reservationResult.innerHTML = `
                    <p>
                        Vui long nhap mat khau thanh vien.
                    </p>
                `;

                reservationPassword.focus();

                return;
            }


            reservationResult.innerHTML = `
                <p>
                    Dang xu ly...
                </p>
            `;


            // -----------------------------
            // GOI API
            // -----------------------------

            const result =
                await callReservationApi(
                    {
                        action:
                            "enqueueReservation",

                        memberId:
                            memberId,

                        bookCode:
                            bookCode,

                        password:
                            password
                    }
                );


            // -----------------------------
            // HIEN THI KET QUA
            // -----------------------------

            showReservationResult(
                reservationResult,
                result
            );


            // -----------------------------
            // NEU THANH CONG
            // -----------------------------

            if (result.success) {

                reservationMemberId.value =
                    "";

                reservationBookId.value =
                    "";

                reservationPassword.value =
                    "";


                // Tu dong dien BookCode
                // vao o tra cuu

                reservationLookupBookId.value =
                    bookCode;


                // Cap nhat danh sach cho

                await loadReservationQueue(
                    bookCode,
                    reservationQueueResult
                );
            }
        }
    );


    // =================================
    // TRA CUU DANH SACH CHO
    // =================================

    btnReservationLookup.addEventListener(
        "click",
        async () => {

            const bookCode =
                reservationLookupBookId.value.trim();


            if (!bookCode) {

                reservationQueueResult.innerHTML = `
                    <p>
                        Vui long nhap BookCode.
                    </p>
                `;

                return;
            }


            await loadReservationQueue(
                bookCode,
                reservationQueueResult
            );
        }
    );


    // =================================
    // HUY DANG KY
    // =================================

    reservationQueueResult.addEventListener(
        "click",
        async (event) => {

            const cancelButton =
                event.target.closest(
                    ".btn-cancel-reservation"
                );


            if (!cancelButton) {
                return;
            }


            const reservationId =
                cancelButton.dataset.reservationId;


            const bookCode =
                cancelButton.dataset.bookCode;


            if (
                !reservationId ||
                !bookCode
            ) {

                return;
            }


            // -----------------------------
            // MO MODAL XAC THUC
            // -----------------------------

            openCancelReservationModal(
                reservationId,
                bookCode
            );
        }
    );


    // =================================
    // XU LY LUOT CHO TIEP THEO
    // =================================

    btnNextReservation.addEventListener(
        "click",
        async () => {

            const bookCode =
                reservationLookupBookId.value.trim();


            // -----------------------------
            // KIEM TRA BOOKCODE
            // -----------------------------

            if (!bookCode) {

                reservationQueueResult.innerHTML = `
                    <p>
                        Vui long nhap BookCode.
                    </p>
                `;

                return;
            }


            // -----------------------------
            // VO HIEU HOA NUT
            // -----------------------------

            btnNextReservation.disabled =
                true;


            btnNextReservation.textContent =
                "Dang xu ly...";


            // -----------------------------
            // GOI API
            // -----------------------------

            const result =
                await callReservationApi(
                    {
                        action:
                            "nextReservation",

                        bookCode:
                            bookCode
                    }
                );


            // -----------------------------
            // XU LY THAT BAI
            // -----------------------------

            if (!result.success) {

                reservationResult.innerHTML = `
                    <p>
                        ${escapeHtml(
                            result.error ||
                            "Khong co nguoi cho tiep theo."
                        )}
                    </p>
                `;


                btnNextReservation.disabled =
                    false;


                btnNextReservation.textContent =
                    "Xử lý lượt chờ tiếp theo";


                return;
            }


            // -----------------------------
            // XU LY THANH CONG
            // -----------------------------

            const reservation =
                result.data;


            reservationResult.innerHTML = `
                <div>

                    <p>
                        <strong>
                            Da xu ly luot cho tiep theo.
                        </strong>
                    </p>

                    <p>
                        Reservation_ID:
                        ${escapeHtml(
                            reservation.reservationId
                        )}
                    </p>

                    <p>
                        Member_ID:
                        ${escapeHtml(
                            reservation.memberId
                        )}
                    </p>

                    <p>
                        BookCode:
                        ${escapeHtml(
                            reservation.bookCode
                        )}
                    </p>

                    <p>
                        Trang thai:
                        ${escapeHtml(
                            reservation.status
                        )}
                    </p>

                </div>
            `;


            // -----------------------------
            // LOAD LAI HANG CHO
            // -----------------------------

            await loadReservationQueue(
                bookCode,
                reservationQueueResult
            );


            // -----------------------------
            // KHOI PHUC NUT
            // -----------------------------

            btnNextReservation.disabled =
                false;


            btnNextReservation.textContent =
                "Xử lý lượt chờ tiếp theo";
        }
    );
}


// =====================================
// LOAD DANH SACH CHO
// =====================================

async function loadReservationQueue(
    bookCode,
    container
) {

    container.innerHTML = `
        <p>
            Dang tra cuu...
        </p>
    `;


    const result =
        await callReservationApi(
            {
                action:
                    "getReservationsByBookCode",

                bookCode:
                    bookCode
            }
        );


    showReservationQueue(
        container,
        result
    );
}
