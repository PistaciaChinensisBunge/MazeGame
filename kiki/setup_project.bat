@echo off
chcp 65001 > nul
title Maze Game Project Setup

echo ========================================
echo       20x20 Maze Game Project Setup
echo ========================================

echo Creating directory structure...
mkdir src 2>nul
mkdir include 2>nul
mkdir resources 2>nul
mkdir .vscode 2>nul

echo Creating 20x20 maze file...
echo 20 20 > resources\maze_20x20.txt
echo 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 >> resources\maze_20x20.txt
echo 1 -1 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 1 >> resources\maze_20x20.txt
echo 1 0 1 1 1 0 1 0 1 1 1 1 0 1 0 1 1 1 0 1 >> resources\maze_20x20.txt
echo 1 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 1 >> resources\maze_20x20.txt
echo 1 0 1 0 1 1 1 1 1 0 1 0 1 1 1 1 0 1 0 1 >> resources\maze_20x20.txt
echo 1 0 0 0 0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 1 >> resources\maze_20x20.txt
echo 1 1 1 1 1 0 1 0 1 1 1 1 1 0 1 1 1 1 1 1 >> resources\maze_20x20.txt
echo 1 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 >> resources\maze_20x20.txt
echo 1 0 1 1 1 1 1 1 1 1 1 1 1 0 1 0 1 1 0 1 >> resources\maze_20x20.txt
echo 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 1 0 0 1 >> resources\maze_20x20.txt
echo 1 1 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 0 1 1 >> resources\maze_20x20.txt
echo 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 >> resources\maze_20x20.txt
echo 1 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 1 >> resources\maze_20x20.txt
echo 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 >> resources\maze_20x20.txt
echo 1 1 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 1 0 1 >> resources\maze_20x20.txt
echo 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 1 >> resources\maze_20x20.txt
echo 1 0 1 1 1 1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 >> resources\maze_20x20.txt
echo 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 1 >> resources\maze_20x20.txt
echo 1 0 1 1 1 1 1 0 1 0 1 1 1 1 1 1 1 1 0 1 >> resources\maze_20x20.txt
echo 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 -2 1 >> resources\maze_20x20.txt
echo 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 >> resources\maze_20x20.txt

echo Creating .vscode\c_cpp_properties.json...
(
echo {
echo     "configurations": [
echo         {
echo             "name": "Win32",
echo             "includePath": [
echo                 "${workspaceFolder}/**",
echo                 "${workspaceFolder}/include",
echo                 "C:/raylib/include"
echo             ],
echo             "defines": [],
echo             "compilerPath": "C:/mingw64/bin/g++.exe",
echo             "cStandard": "c11",
echo             "cppStandard": "c++11",
echo             "intelliSenseMode": "windows-gcc-x64"
echo         }
echo     ],
echo     "version": 4
echo }
) > .vscode\c_cpp_properties.json

echo.
echo ========================================
echo Project setup complete!
echo ========================================
echo.
echo Next steps:
echo 1. Create the code files as described above
echo 2. Place your image resources in the resources\ folder:
echo    - character.png (player)
echo    - start.png (start tile)
echo    - end.png (end tile)
echo    - floor.png (floor tile)
echo    - wall.png (wall tile)
echo    - grass.png (grass tile)
echo    - lava.png (lava tile)
echo 3. Run build.bat to compile and run the game
echo.
echo Notes:
echo - Maze file is already created (maze_20x20.txt)
echo - Window size is 1000x850 to accommodate 20x20 maze
echo - All text is in English to avoid encoding issues
echo - Game will center the 20x20 maze on screen
echo.
echo Press any key to continue...
pause