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
// presentation/
// └── js/
//     └── api.js
//
// api.php nam tai:
//
// src/
// └── api/
//     └── api.php
//
// Vi vay can di len 2 cap.

const API_URL =
    new URL(
        "../../api/api.php",
        import.meta.url
    ).href;


// =====================================
// GUI REQUEST DEN API
// =====================================

export async function sendApiRequest(
    requestData
) {
    try {

        console.log(
            "=== API REQUEST ==="
        );

        console.log(
            "Request:",
            requestData
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
        // DOC JSON
        // =================================

        const result =
            await response.json();


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
                "Khong the ket noi API."
        };
    }
}