@echo off
echo ===================================================
echo   Compiling and Starting CodeForge Backend Server
echo ===================================================

taskkill /F /IM server.exe >nul 2>&1
taskkill /F /IM codeforge.exe >nul 2>&1

g++ main.cpp -o server.exe -std=c++17 -O1 -pthread -lws2_32 -lwsock32
if %ERRORLEVEL% equ 0 (
    echo.
    echo Backend compiled successfully! Starting server on http://localhost:8080...
    echo.
    server.exe
) else (
    echo.
    echo [ERROR] Compilation failed! Check the errors above.
    pause
)