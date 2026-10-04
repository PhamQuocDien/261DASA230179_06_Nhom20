@echo off
setlocal
cd /d "%~dp0.."
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0test\run_d5_tests.ps1"
set EXITCODE=%ERRORLEVEL%
echo.
if %EXITCODE% EQU 0 (
    echo D5 TESTS: PASS
) else (
    echo D5 TESTS: FAIL
)
exit /b %EXITCODE%
