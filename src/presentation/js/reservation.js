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
                <strong>Dang ky thanh cong.</strong>
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
                    <th>Reservation_ID</th>
                    <th>Member_ID</th>
                    <th>BookCode</th>
                    <th>Ngay dang ky</th>
                    <th>Trang thai</th>
                    <th>Thao tac</th>
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

    const reservationQueueResult =
        document.getElementById(
            "reservationQueueResult"
        );


    if (
        !reservationMemberId ||
        !reservationBookId ||
        !btnReservation ||
        !reservationResult ||
        !reservationLookupBookId ||
        !btnReservationLookup ||
        !reservationQueueResult
    ) {

        console.error(
            "Khong tim thay day du element Reservation."
        );

        return;
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


            if (!memberId) {

                reservationResult.innerHTML = `
                    <p>
                        Vui long nhap Member_ID.
                    </p>
                `;

                return;
            }


            if (!bookCode) {

                reservationResult.innerHTML = `
                    <p>
                        Vui long nhap BookCode.
                    </p>
                `;

                return;
            }


            reservationResult.innerHTML = `
                <p>
                    Dang xu ly...
                </p>
            `;


            const result =
                await callReservationApi(
                    {
                        action:
                            "enqueueReservation",

                        memberId:
                            memberId,

                        bookCode:
                            bookCode
                    }
                );


            showReservationResult(
                reservationResult,
                result
            );


            if (result.success) {

                reservationMemberId.value =
                    "";

                reservationBookId.value =
                    "";

                // Tu dong cap nhat danh sach cho
                reservationLookupBookId.value =
                    bookCode;

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


            if (!reservationId ||
                !bookCode) {

                return;
            }


            const confirmed =
                confirm(
                    "Ban co chac chan muon huy dang ky nay?"
                );


            if (!confirmed) {
                return;
            }


            cancelButton.disabled =
                true;

            cancelButton.textContent =
                "Dang huy...";


            const result =
                await callReservationApi(
                    {
                        action:
                            "cancelReservation",

                        reservationId:
                            reservationId
                    }
                );


            if (!result.success) {

                alert(
                    result.error ||
                    "Khong the huy dang ky."
                );

                cancelButton.disabled =
                    false;

                cancelButton.textContent =
                    "Hủy đăng ký";

                return;
            }


            // Huy thanh cong
            alert(
                "Da huy dang ky thanh cong."
            );


            // Load lai danh sach cho
            await loadReservationQueue(
                bookCode,
                reservationQueueResult
            );
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
