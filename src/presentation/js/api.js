
// =====================================
// API MODULE
// BUOI 50
// Giao tiep giua Frontend va api.php
// =====================================


// =====================================
// API URL
// =====================================

// api.js nam tai:
//
// src/
// └── presentation/
//     └── js/
//         └── api.js
//
// api.php nam tai:
//
// src/
// └── api/
//     └── api.php
//
// Vi vay can di len 2 cap:
//
// js/
//   ↓ ../
// presentation/
//   ↓ ../
// src/
//
// Sau do vao:
//
// api/
// └── api.php

const API_URL =
    new URL(
        "../../api/api.php",
        import.meta.url
    ).href;


// =====================================
// AN MAT KHAU KHI LOG
// =====================================

function toSafeLogData(requestData) {

    if (
        !requestData ||
        typeof requestData !== "object"
    ) {
        return requestData;
    }

    const safeData = { ...requestData };

    if (safeData.password !== undefined) {
        safeData.password = "********";
    }

    return safeData;
}


// =====================================
// GUI REQUEST DEN API
// =====================================

export async function sendApiRequest(
    requestData
) {
    try {

        // Khong ghi mat khau ra console.
        console.log(
            "=== API REQUEST ==="
        );

        console.log(
            "Request:",
            toSafeLogData(requestData)
        );

        console.log(
            "API URL:",
            API_URL
        );


        const response =
            await fetch(
                API_URL,
                {
                    method: "POST",

                    headers: {
                        "Content-Type":
                            "application/json"
                    },

                    // Gui kem session de backend
                    // nhan dung trang thai dang nhap.
                    credentials: "include",

                    body:
                        JSON.stringify(
                            requestData
                        )
                }
            );


        // =================================
        // KIEM TRA HTTP
        // =================================

        if (!response.ok) {
            throw new Error(
                "HTTP error: "
                + response.status
            );
        }


        // =================================
        // DOC RESPONSE
        // =================================

        const responseText =
            await response.text();


        // =================================
        // KIEM TRA JSON
        // =================================

        let result;

        try {

            result =
                JSON.parse(
                    responseText
                );

        }
        catch (jsonError) {

            console.error(
                "API khong tra ve JSON."
            );

            console.error(
                "Response:",
                responseText
            );

            throw new Error(
                "API khong tra ve JSON."
            );
        }


        console.log(
            "=== API RESPONSE ==="
        );

        console.log(
            "Response:",
            result
        );


        // =================================
        // API BAO LOI
        // =================================

        if (!result.success) {

            console.error(
                "API Error:",
                result.error
            );

            return result;
        }


        // =================================
        // API THANH CONG
        // =================================

        return result;
    }
    catch (error) {

        console.error(
            "Khong the ket noi API:",
            error
        );


        return {
            success: false,
            error:
                error.message ||
                "Khong the ket noi API."
        };
    }
}

