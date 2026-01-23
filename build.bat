@echo off
echo ===================================
echo Building Chess Game (MVC)...
echo ===================================

REM Đường dẫn SFML - THAY ĐỔI THEO MÁY BẠN
set SFML_DIR=D:\SFML-2.6.1

REM Kiểm tra SFML có tồn tại không
if not exist "%SFML_DIR%\include" (
    echo ERROR: SFML not found at %SFML_DIR%
    echo Please install SFML and update SFML_DIR in this script
    echo See SFML_INSTALL_GUIDE.md for instructions
    pause
    exit /b 1
)

echo Compiling main.cpp...
g++ -std=c++17 main.cpp -o ChessGame.exe ^
  -I"%SFML_DIR%\include" ^
  -L"%SFML_DIR%\lib" ^
    -lsfml-graphics-s -lsfml-window-s -lsfml-system-s ^
  -DSFML_STATIC ^
    -lopengl32 -lwinmm -lgdi32 -lfreetype -lshell32 -lsetupapi -limm32 -lole32 -luuid

if %ERRORLEVEL% EQU 0 (
    echo ===================================
    echo Build successful!
    echo ===================================
    echo.
    echo Copying SFML DLL files...
    copy "%SFML_DIR%\bin\sfml-graphics-2.dll" . >nul 2>&1
    copy "%SFML_DIR%\bin\sfml-window-2.dll" . >nul 2>&1
    copy "%SFML_DIR%\bin\sfml-system-2.dll" . >nul 2>&1
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
    echo Make sure:
    echo 1. SFML is installed at %SFML_DIR%
    echo 2. MinGW GCC is in your PATH
    echo.
    echo See SFML_INSTALL_GUIDE.md for help
    pause
    exit /b 1
)

pause
