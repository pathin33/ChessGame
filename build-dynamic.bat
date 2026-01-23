@echo off
echo ===================================
echo Building Chess Game (MVC) - Dynamic Linking...
echo ===================================

REM Đường dẫn SFML - CHỈ CẦN NẾU DÙNG NATIVE MinGW (không khuyến nghị)
REM Khuyến nghị: Dùng MSYS2 thay thế (xem MSYS2_GUIDE.md)
set SFML_DIR=D:\SFML-2.6.1

REM Kiểm tra SFML có tồn tại không
if not exist "%SFML_DIR%\include" (
    echo ERROR: SFML not found at %SFML_DIR%
    echo Please install SFML and update SFML_DIR in this script
    echo See SFML_INSTALL_GUIDE.md for instructions
    pause
    exit /b 1
)

echo Compiling main.cpp with dynamic linking...
g++ -std=c++17 main.cpp -o ChessGame.exe ^
  -I"%SFML_DIR%\include" ^
  -L"%SFML_DIR%\lib" ^
  -lsfml-graphics -lsfml-window -lsfml-system ^
  -static-libgcc -static-libstdc++

if %ERRORLEVEL% EQU 0 (
    echo ===================================
    echo Build successful!
    echo ===================================
    echo.
    echo Copying SFML DLL files...
    copy "%SFML_DIR%\bin\sfml-graphics-2.dll" . >nul 2>&1
    copy "%SFML_DIR%\bin\sfml-window-2.dll" . >nul 2>&1
    copy "%SFML_DIR%\bin\sfml-system-2.dll" . >nul 2>&1
    copy "%SFML_DIR%\bin\openal32.dll" . >nul 2>&1
    echo.
    echo ===================================
    echo Done! Run the game with:
    echo   ChessGame.exe
    echo ===================================
) else (
    echo ===================================
    echo Build failed!
    echo ===================================
    echo.
    echo Trying alternative build method...
    echo See build-alternative.bat
    pause
    exit /b 1
)

pause
