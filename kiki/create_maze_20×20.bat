@echo off
chcp 65001 > nul
echo Creating 20x20 maze file...

echo 20 20 > resources\maze_20x20.txt

REM Generate a 20x20 maze with various terrain types
(
echo 20 20
echo 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
echo 1 -1 0 0 1 0 0 0 1 0 0 0 0 1 0 0 0 0 0 1
echo 1 0 1 0 1 1 1 0 1 0 1 1 0 1 0 1 1 1 0 1
echo 1 0 1 0 0 0 1 0 0 0 1 0 0 1 0 0 0 1 0 1
echo 1 0 1 1 1 0 1 1 1 1 1 0 1 1 1 1 0 1 0 1
echo 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 1
echo 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 1 1 1 0 1
echo 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 1
echo 1 0 1 1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1
echo 1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 1
echo 1 1 1 1 1 0 1 1 1 1 1 1 1 1 1 1 1 1 0 1
echo 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1
echo 1 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 1 0 1
echo 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1
echo 1 1 1 1 1 1 1 1 1 1 1 0 1 1 1 1 1 1 0 1
echo 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 1
echo 1 0 1 1 1 1 1 1 1 0 1 1 1 1 1 1 1 1 1 1
echo 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 1
echo 1 0 1 1 1 1 1 0 1 0 1 1 1 1 1 1 1 1 0 1
echo 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 -2 1
echo 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
) > resources\maze_20x20.txt

echo 20x20 maze file created successfully!
echo.
echo Place your texture files in resources\ folder:
echo - start.png (start position)
echo - end.png (end position)
echo - floor.png (floor)
echo - wall.png (walls)
echo - grass.png (grass terrain)
echo - lava.png (lava terrain)
echo - slime.png (slime terrain)
echo - character.png (player character)
echo.
pause