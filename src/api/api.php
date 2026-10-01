<?php
declare(strict_types=1);

session_start();

header('Content-Type: application/json; charset=utf-8');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST, OPTIONS');
header('Access-Control-Allow-Headers: Content-Type');

if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    http_response_code(204);
    exit;
}

if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    sendResponse(false, null, 'Method khong duoc ho tro. Chi su dung POST.');
}

$rawInput = file_get_contents('php://input');

if ($rawInput === false || trim($rawInput) === '') {
    sendResponse(false, null, 'Request body rong.');
}

$request = json_decode($rawInput, true);

if (!is_array($request)) {
    sendResponse(false, null, 'JSON Request khong hop le.');
}

$action = $request['action'] ?? '';

if ($action === 'loginAdmin') {

    $idAdmin = $request['idAdmin'] ?? '';
    $password = $request['password'] ?? '';

    if (
        $idAdmin !== 'QuanLyAdmin' ||
        $password !== 'QuanLyThuVien'
    ) {
        sendResponse(
            false,
            null,
            'ID quan ly hoac mat khau khong dung.'
        );
    }

    session_regenerate_id(true);

    $_SESSION['adminAuthenticated'] = true;

    sendResponse(
        true,
        ['authenticated' => true]
    );
}

if ($action === 'registerMember') {

    $name = trim(
        $request['name'] ?? ''
    );

    $email = trim(
        $request['email'] ?? ''
    );

    $phone = trim(
        $request['phone'] ?? ''
    );

    if ($name === '') {
        sendResponse(
            false,
            null,
            'Vui long nhap ho va ten.'
        );
    }

    if ($email === '') {
        sendResponse(
            false,
            null,
            'Vui long nhap email.'
        );
    }

    if ($phone === '') {
        sendResponse(
            false,
            null,
            'Vui long nhap so dien thoai.'
        );
    }

    $request['name'] = $name;
    $request['email'] = $email;
    $request['phone'] = $phone;
}

$adminActions = [
    'createBook',
    'updateBook',
    'addBookCopy',
    'deleteBook'
];

if (in_array($action, $adminActions, true)) {

    if (empty($_SESSION['adminAuthenticated'])) {
        sendResponse(
            false,
            null,
            'Chua dang nhap quyen quan ly.'
        );
    }
}


// =====================================
// DONG BO library.json TU GITHUB XUONG RENDER
// =====================================

$githubState = syncLibraryFromGitHub();

if (!$githubState['success']) {
    sendResponse(
        false,
        null,
        $githubState['error']
    );
}

$request['adminAuthorized'] =
    !empty($_SESSION['adminAuthenticated']);


// =====================================
// GOI C++ API
// =====================================

$cppResponse = callCppApi($request);

if (!$cppResponse['processOk']) {
    sendResponse(
        false,
        null,
        $cppResponse['error']
    );
}


// =====================================
// DONG BO library.json TU RENDER LEN GITHUB
// CHI CAP NHAT KHI C++ THAY DOI FILE
// =====================================

$syncResult = syncLibraryToGitHubIfChanged(
    $githubState['content'],
    $githubState['sha']
);

if (!$syncResult['success']) {
    sendResponse(
        false,
        null,
        $syncResult['error']
    );
}


// =====================================
// TRA RESPONSE VE FRONTEND
// =====================================

sendRawJson(
    $cppResponse['json']
);


// =====================================
// GITHUB TOKEN
// =====================================

function getGitHubToken(): string
{
    $token = getenv('GITHUB_TOKEN');

    if (
        $token === false ||
        trim($token) === ''
    ) {
        throw new RuntimeException(
            'Chua cau hinh GITHUB_TOKEN tren Render.'
        );
    }

    return trim($token);
}


// =====================================
// GITHUB FILE URL
// =====================================

function getGitHubContentsUrl(): string
{
    return
        'https://api.github.com/repos/'
        . 'PhamQuocDien/'
        . '261DASA230179_06_Nhom20/'
        . 'contents/data/library.json';
}


// =====================================
// GITHUB API REQUEST
// =====================================

function githubApiRequest(
    string $method,
    string $url,
    ?array $body = null
): array {

    $token = getGitHubToken();

    $headers = [
        'Accept: application/vnd.github+json',
        'Authorization: Bearer ' . $token,
        'X-GitHub-Api-Version: 2022-11-28',
        'User-Agent: 261DASA230179-06-Nhom20'
    ];

    $options = [
        'http' => [
            'method' => $method,
            'header' => implode(
                "\r\n",
                $headers
            ),
            'ignore_errors' => true,
            'timeout' => 20
        ]
    ];

    if ($body !== null) {

        $options['http']['header'] .=
            "\r\nContent-Type: application/json";

        $jsonBody = json_encode(
            $body,
            JSON_UNESCAPED_UNICODE |
            JSON_UNESCAPED_SLASHES
        );

        if ($jsonBody === false) {
            throw new RuntimeException(
                'Khong the encode GitHub request.'
            );
        }

        $options['http']['content'] =
            $jsonBody;
    }

    $context =
        stream_context_create(
            $options
        );

    $responseBody =
        @file_get_contents(
            $url,
            false,
            $context
        );

    $statusCode = 0;

    if (
        isset($http_response_header) &&
        is_array($http_response_header)
    ) {

        foreach (
            $http_response_header
            as $headerLine
        ) {

            if (
                preg_match(
                    '/^HTTP\/\S+\s+(\d+)/',
                    $headerLine,
                    $matches
                )
            ) {

                $statusCode =
                    (int)$matches[1];
            }
        }
    }

    return [
        'status' =>
            $statusCode,

        'body' =>
            $responseBody === false
                ? ''
                : $responseBody
    ];
}


// =====================================
// GITHUB -> RENDER
// =====================================

function syncLibraryFromGitHub(): array
{
    try {

        $response =
            githubApiRequest(
                'GET',
                getGitHubContentsUrl()
                . '?ref=main'
            );

    }
    catch (Throwable $e) {

        return [
            'success' => false,
            'error' => $e->getMessage()
        ];
    }

    if (
        $response['status'] < 200 ||
        $response['status'] >= 300
    ) {

        $error =
            'Khong doc duoc library.json tu GitHub. HTTP '
            . $response['status']
            . '.';

        if (
            trim($response['body']) !== ''
        ) {

            $error .=
                ' ' .
                trim($response['body']);
        }

        return [
            'success' => false,
            'error' => $error
        ];
    }

    $githubData =
        json_decode(
            $response['body'],
            true
        );

    if (!is_array($githubData)) {

        return [
            'success' => false,
            'error' =>
                'GitHub tra ve JSON Response khong hop le.'
        ];
    }

    $encodedContent =
        $githubData['content'] ?? '';

    $sha =
        $githubData['sha'] ?? '';

    if (
        !is_string($encodedContent) ||
        !is_string($sha) ||
        $encodedContent === '' ||
        $sha === ''
    ) {

        return [
            'success' => false,
            'error' =>
                'GitHub khong tra ve day du content hoac sha cua library.json.'
        ];
    }

    $encodedContent =
        str_replace(
            ["\r", "\n"],
            '',
            $encodedContent
        );

    $content =
        base64_decode(
            $encodedContent,
            true
        );

    if ($content === false) {

        return [
            'success' => false,
            'error' =>
                'Khong giai ma duoc library.json tu GitHub.'
        ];
    }

    json_decode(
        $content,
        true
    );

    if (
        json_last_error() !==
        JSON_ERROR_NONE
    ) {

        return [
            'success' => false,
            'error' =>
                'library.json tren GitHub khong phai JSON hop le.'
        ];
    }

    $projectRoot =
        dirname(
            __DIR__,
            2
        );

    $localFile =
        $projectRoot
        . DIRECTORY_SEPARATOR
        . 'data'
        . DIRECTORY_SEPARATOR
        . 'library.json';

    $dataDirectory =
        dirname($localFile);

    if (
        !is_dir($dataDirectory) &&
        !mkdir(
            $dataDirectory,
            0775,
            true
        ) &&
        !is_dir($dataDirectory)
    ) {

        return [
            'success' => false,
            'error' =>
                'Khong tao duoc thu muc data tren Render.'
        ];
    }

    if (
        @file_put_contents(
            $localFile,
            $content,
            LOCK_EX
        ) === false
    ) {

        return [
            'success' => false,
            'error' =>
                'Khong ghi duoc library.json vao filesystem Render.'
        ];
    }

    return [
        'success' => true,
        'content' => $content,
        'sha' => $sha
    ];
}


// =====================================
// RENDER -> GITHUB
// =====================================

function syncLibraryToGitHubIfChanged(
    string $originalContent,
    string $originalSha
): array {

    $projectRoot =
        dirname(
            __DIR__,
            2
        );

    $localFile =
        $projectRoot
        . DIRECTORY_SEPARATOR
        . 'data'
        . DIRECTORY_SEPARATOR
        . 'library.json';

    $localContent =
        @file_get_contents(
            $localFile
        );

    if ($localContent === false) {

        return [
            'success' => false,
            'error' =>
                'Khong doc duoc library.json sau khi C++ xu ly.'
        ];
    }

    // Khong thay doi -> khong tao commit GitHub.
    if (
        $localContent ===
        $originalContent
    ) {

        return [
            'success' => true,
            'changed' => false
        ];
    }

    json_decode(
        $localContent,
        true
    );

    if (
        json_last_error() !==
        JSON_ERROR_NONE
    ) {

        return [
            'success' => false,
            'error' =>
                'C++ da tao ra library.json khong hop le.'
        ];
    }

    $body = [
        'message' =>
            'Cap nhat data/library.json tu Render',

        'content' =>
            base64_encode(
                $localContent
            ),

        'sha' =>
            $originalSha,

        'branch' =>
            'main'
    ];

    try {

        $response =
            githubApiRequest(
                'PUT',
                getGitHubContentsUrl(),
                $body
            );

    }
    catch (Throwable $e) {

        return [
            'success' => false,
            'error' =>
                $e->getMessage()
        ];
    }

    if (
        $response['status'] < 200 ||
        $response['status'] >= 300
    ) {

        $error =
            'Khong the luu library.json len GitHub. HTTP '
            . $response['status']
            . '.';

        if (
            trim($response['body']) !== ''
        ) {

            $error .=
                ' ' .
                trim($response['body']);
        }

        return [
            'success' => false,
            'error' => $error
        ];
    }

    return [
        'success' => true,
        'changed' => true
    ];
}


// =====================================
// GOI C++ API
// =====================================

function callCppApi(array $request): array
{
    $projectRoot = dirname(__DIR__, 2);

    $cppExe =
        $projectRoot
        . DIRECTORY_SEPARATOR
        . 'bin'
        . DIRECTORY_SEPARATOR
        . 'ThuVien.exe';

    if (!is_file($cppExe)) {

        return [
            'processOk' => false,
            'json' => '',
            'error' =>
                'Khong tim thay C++ executable: '
                . $cppExe
        ];
    }

    $command = [
        $cppExe,
        '--api'
    ];

    $descriptorSpec = [
        0 => ['pipe', 'r'],
        1 => ['pipe', 'w'],
        2 => ['pipe', 'w']
    ];

    $pipes = [];

    $process =
        proc_open(
            $command,
            $descriptorSpec,
            $pipes,
            $projectRoot,
            null,
            ['bypass_shell' => true]
        );

    if (!is_resource($process)) {

        return [
            'processOk' => false,
            'json' => '',
            'error' =>
                'Khong the khoi dong C++ executable.'
        ];
    }

    $jsonRequest =
        json_encode(
            $request,
            JSON_UNESCAPED_UNICODE |
            JSON_UNESCAPED_SLASHES
        );

    if ($jsonRequest === false) {

        fclose($pipes[0]);
        fclose($pipes[1]);
        fclose($pipes[2]);

        proc_close($process);

        return [
            'processOk' => false,
            'json' => '',
            'error' =>
                'Khong the encode JSON Request.'
        ];
    }

    fwrite(
        $pipes[0],
        $jsonRequest . PHP_EOL
    );

    fclose($pipes[0]);

    $stdout =
        stream_get_contents(
            $pipes[1]
        );

    fclose($pipes[1]);

    $stderr =
        stream_get_contents(
            $pipes[2]
        );

    fclose($pipes[2]);

    $exitCode =
        proc_close(
            $process
        );

    if (
        $exitCode !== 0 &&
        trim($stdout) === ''
    ) {

        $error =
            'C++ API tra ve exit code '
            . $exitCode
            . '.';

        if (
            trim($stderr) !== ''
        ) {

            $error .=
                ' '
                . trim($stderr);
        }

        return [
            'processOk' => false,
            'json' => '',
            'error' => $error
        ];
    }

    if (
        $stdout === false ||
        trim($stdout) === ''
    ) {

        return [
            'processOk' => false,
            'json' => '',
            'error' =>
                'C++ khong tra ve JSON Response.'
        ];
    }

    $decoded =
        json_decode(
            $stdout,
            true
        );

    if (!is_array($decoded)) {

        $error =
            'JSON Response tu C++ khong hop le.';

        if (
            trim($stderr) !== ''
        ) {

            $error .=
                ' '
                . trim($stderr);
        }

        return [
            'processOk' => false,
            'json' => '',
            'error' => $error
        ];
    }

    return [
        'processOk' => true,
        'json' => $stdout,
        'error' => ''
    ];
}


// =====================================
// RESPONSE
// =====================================

function sendResponse(
    bool $success,
    mixed $data,
    ?string $error = null
): never {

    $response = [
        'success' => $success
    ];

    if ($success) {

        $response['data'] =
            $data;

    }
    else {

        $response['error'] =
            $error ??
            'Unknown error.';
    }

    echo json_encode(
        $response,
        JSON_UNESCAPED_UNICODE |
        JSON_UNESCAPED_SLASHES
    );

    exit;
}

function sendRawJson(
    string $json
): never {

    echo $json;

    exit;
}
