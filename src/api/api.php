<?php

declare(strict_types=1);

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

$cppResponse = callCppApi($request);

if (!$cppResponse['processOk']) {
    sendResponse(false, null, $cppResponse['error']);
}

sendRawJson($cppResponse['json']);


// =====================================
// GOI CHINH EXECUTABLE C++ CUA PROJECT
// =====================================

function callCppApi(array $request): array
{
    // api.php nam tai: src/api/api.php
    // project root: dirname(__DIR__, 2)
    $projectRoot = dirname(__DIR__, 2);

    // DAT OUTPUT CUA PROJECT VISUAL STUDIO VAO:
    // project-root/bin/ThuVien.exe
    $cppExe = $projectRoot
        . DIRECTORY_SEPARATOR
        . 'bin'
        . DIRECTORY_SEPARATOR
        . 'ThuVien.exe';

    if (!is_file($cppExe)) {
        return [
            'processOk' => false,
            'json' => '',
            'error' => 'Khong tim thay C++ executable: ' . $cppExe
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

    $process = proc_open(
        $command,
        $descriptorSpec,
        $pipes,
        $projectRoot,
        null,
        [
            'bypass_shell' => true
        ]
    );

    if (!is_resource($process)) {
        return [
            'processOk' => false,
            'json' => '',
            'error' => 'Khong the khoi dong C++ executable.'
        ];
    }

    $jsonRequest = json_encode(
        $request,
        JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES
    );

    if ($jsonRequest === false) {
        fclose($pipes[0]);
        fclose($pipes[1]);
        fclose($pipes[2]);
        proc_close($process);

        return [
            'processOk' => false,
            'json' => '',
            'error' => 'Khong the encode JSON Request.'
        ];
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

        return [
            'processOk' => false,
            'json' => '',
            'error' => $error
        ];
    }

    if ($stdout === false || trim($stdout) === '') {
        return [
            'processOk' => false,
            'json' => '',
            'error' => 'C++ khong tra ve JSON Response.'
        ];
    }

    $decoded = json_decode($stdout, true);

    if (!is_array($decoded)) {
        $error = 'JSON Response tu C++ khong hop le.';

        if (trim($stderr) !== '') {
            $error .= ' ' . trim($stderr);
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


function sendResponse(
    bool $success,
    mixed $data,
    ?string $error = null
): never {
    $response = [
        'success' => $success
    ];

    if ($success) {
        $response['data'] = $data;
    } else {
        $response['error'] = $error ?? 'Unknown error.';
    }

    echo json_encode(
        $response,
        JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES
    );

    exit;
}


function sendRawJson(string $json): never
{
    echo $json;
    exit;
}