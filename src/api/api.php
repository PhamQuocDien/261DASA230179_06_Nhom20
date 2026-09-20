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
    if ($idAdmin !== 'QuanLyAdmin' || $password !== 'QuanLyThuVien') {
        sendResponse(false, null, 'ID quan ly hoac mat khau khong dung.');
    }
    session_regenerate_id(true);
    $_SESSION['adminAuthenticated'] = true;
    sendResponse(true, ['authenticated' => true]);
}
if ($action === 'registerMember') {
    $name = trim($request['name'] ?? '');
    $email = trim($request['email'] ?? '');
    $phone = trim($request['phone'] ?? '');
    if ($name === '') {
        sendResponse(false, null, 'Vui long nhap ho va ten.');
    }
    if ($email === '') {
        sendResponse(false, null, 'Vui long nhap email.');
    }
    if ($phone === '') {
        sendResponse(false, null, 'Vui long nhap so dien thoai.');
    }
    $request['name'] = $name;
    $request['email'] = $email;
    $request['phone'] = $phone;
}
$adminActions = ['createBook', 'updateBook', 'addBookCopy', 'deleteBook'];
if (in_array($action, $adminActions, true)) {
    if (empty($_SESSION['adminAuthenticated'])) {
        sendResponse(false, null, 'Chua dang nhap quyen quan ly.');
    }
}
$request['adminAuthorized'] = !empty($_SESSION['adminAuthenticated']);
$cppResponse = callCppApi($request);
if (!$cppResponse['processOk']) {
    sendResponse(false, null, $cppResponse['error']);
}
sendRawJson($cppResponse['json']);
function callCppApi(array $request): array {
    $projectRoot = dirname(__DIR__, 2);
    $cppExe = $projectRoot . DIRECTORY_SEPARATOR . 'bin' . DIRECTORY_SEPARATOR . 'ThuVien.exe';
    if (!is_file($cppExe)) {
        return ['processOk' => false, 'json' => '', 'error' => 'Khong tim thay C++ executable: ' . $cppExe];
    }
    $command = [$cppExe, '--api'];
    $descriptorSpec = [0 => ['pipe', 'r'], 1 => ['pipe', 'w'], 2 => ['pipe', 'w']];
    $pipes = [];
    $process = proc_open($command, $descriptorSpec, $pipes, $projectRoot, null, ['bypass_shell' => true]);
    if (!is_resource($process)) {
        return ['processOk' => false, 'json' => '', 'error' => 'Khong the khoi dong C++ executable.'];
    }
    $jsonRequest = json_encode($request, JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES);
    if ($jsonRequest === false) {
        fclose($pipes[0]);
        fclose($pipes[1]);
        fclose($pipes[2]);
        proc_close($process);
        return ['processOk' => false, 'json' => '', 'error' => 'Khong the encode JSON Request.'];
    }
    fwrite($pipes[0], $jsonRequest . PHP_EOL);
    fclose($pipes[0]);
    $stdout = stream_get_contents($pipes[1]);
    fclose($pipes[1]);
    $stderr = stream_get_contents($pipes[2]);
    fclose($pipes[2]);
    $exitCode = proc_close($process);
    if ($exitCode !== 0 && trim($stdout) === '') {
        $error = 'C++ API tra ve exit code ' . $exitCode . '.';
        if (trim($stderr) !== '') {
            $error .= ' ' . trim($stderr);
        }
        return ['processOk' => false, 'json' => '', 'error' => $error];
    }
    if ($stdout === false || trim($stdout) === '') {
        return ['processOk' => false, 'json' => '', 'error' => 'C++ khong tra ve JSON Response.'];
    }
    $decoded = json_decode($stdout, true);
    if (!is_array($decoded)) {
        $error = 'JSON Response tu C++ khong hop le.';
        if (trim($stderr) !== '') {
            $error .= ' ' . trim($stderr);
        }
        return ['processOk' => false, 'json' => '', 'error' => $error];
    }
    return ['processOk' => true, 'json' => $stdout, 'error' => ''];
}
function sendResponse(bool $success, mixed $data, ?string $error = null): never {
    $response = ['success' => $success];
    if ($success) {
        $response['data'] = $data;
    } else {
        $response['error'] = $error ?? 'Unknown error.';
    }
    echo json_encode($response, JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES);
    exit;
}
function sendRawJson(string $json): never {
    echo $json;
    exit;
}