@echo off
chcp 65001 > nul
title Maze Game Compilation

echo ========================================
echo      20x20 Maze Game - Raylib Version
echo ========================================

set RAYLIB_PATH=C:\raylib
set MINGW_PATH=C:\mingw64
set OUTPUT_NAME=maze_game_20x20

echo [1/3] Checking environment...
if not exist "%RAYLIB_PATH%\include\raylib.h" (
    echo Error: Raylib not found!
    echo Please install Raylib to: %RAYLIB_PATH%
    pause
    exit /b 1
)

if not exist "%MINGW_PATH%\bin\g++.exe" (
    echo Error: G++ compiler not found!
    echo Please install MinGW-w64 to: %MINGW_PATH%
    pause
    exit /b 1
)

echo [2/3] Compiling project...
"%MINGW_PATH%\bin\g++.exe" ^
    -o %OUTPUT_NAME%.exe ^
    src\main.cpp src\Maze.cpp src\Player.cpp ^
    -I include ^
    -I "%RAYLIB_PATH%\include" ^
    -L "%RAYLIB_PATH%\lib" ^
    -lraylib ^
    -lgdi32 ^
    -lwinmm ^
    -luser32 ^
    -lshell32 ^
    -static ^
    -std=c++11 ^
    -Wall ^
    -Wno-missing-braces ^
    -O2

if %errorlevel% equ 0 (
    echo.
    echo [SUCCESS] Compilation complete!
    echo.
    echo ========================================
    echo Starting game...
    echo ========================================
    echo.
    %OUTPUT_NAME%.exe
) else (
    echo.
    echo [ERROR] Compilation failed!
    echo Error code: %errorlevel%
    pause
    exit /b %errorlevel%
)

pause