#include "raylib.h"
#include "Maze.h"
#include "Player.h"
#include <string>
#include <iostream>

enum GameState {
    MENU,
    PLAYING,
    VICTORY
};

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "Maze Game - Path Algorithm Visualization");
    SetTargetFPS(60);
    
    Maze maze;
    Player player;
    
    maze.LoadTextures();
    player.LoadTexture("resources/character.png");
    
    bool mazeLoaded = false;
    if (FileExists("resources/maze_20x20.txt")) {
        mazeLoaded = maze.LoadFromFile("resources/maze_20x20.txt");
    } else {
        std::cout << "Warning: Maze file not found, generating random 20x20 maze" << std::endl;
        maze.GenerateRandom(20, 20);
        mazeLoaded = true;
    }
    
    if (!mazeLoaded) {
        std::cout << "Error: Cannot load maze" << std::endl;
        CloseWindow();
        return -1;
    }
    
    Point start = maze.GetStart();
    player.SetPosition(start.x, start.y);
    
    GameState gameState = MENU;
    
    while (!WindowShouldClose()) {
        if (gameState == MENU) {
            if (IsKeyPressed(KEY_SPACE)) {
                gameState = PLAYING;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                break;
            }
        }
        else if (gameState == PLAYING) {
            if (IsKeyPressed(KEY_UP)) {
                player.Move(0, -1, maze);
            }
            if (IsKeyPressed(KEY_DOWN)) {
                player.Move(0, 1, maze);
            }
            if (IsKeyPressed(KEY_LEFT)) {
                player.Move(-1, 0, maze);
            }
            if (IsKeyPressed(KEY_RIGHT)) {
                player.Move(1, 0, maze);
            }
            
            if (IsKeyPressed(KEY_ONE)) {
                maze.SetDisplayPath(1);
            }
            if (IsKeyPressed(KEY_TWO)) {
                maze.SetDisplayPath(2);
            }
            if (IsKeyPressed(KEY_THREE)) {
                maze.SetDisplayPath(3);
            }
            if (IsKeyPressed(KEY_FOUR)) {
                maze.SetDisplayPath(4);
            }
            if (IsKeyPressed(KEY_ZERO)) {
                maze.SetDisplayPath(0);
            }
            
            if (player.HasReachedEnd()) {
                gameState = VICTORY;
            }
        }
        else if (gameState == VICTORY) {
            if (IsKeyPressed(KEY_SPACE)) {
                player.SetPosition(start.x, start.y);
                player.Reset();
                gameState = PLAYING;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                gameState = MENU;
            }
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        if (gameState == MENU) {
            DrawText("MAZE GAME", screenWidth/2 - MeasureText("MAZE GAME", 60)/2, 100, 60, DARKBLUE);
            DrawText("Path Algorithm Visualization", screenWidth/2 - MeasureText("Path Algorithm Visualization", 35)/2, 180, 35, BLUE);
            
            Rectangle startButton = {screenWidth/2 - 180, 300, 360, 70};
            DrawRectangleRec(startButton, BLUE);
            DrawRectangleLinesEx(startButton, 3, DARKBLUE);
            DrawText("Press SPACE to Start Game", screenWidth/2 - MeasureText("Press SPACE to Start Game", 30)/2, 320, 30, WHITE);
            
            DrawText("Instructions:", 50, 420, 28, DARKGRAY);
            DrawText("1. Use Arrow Keys to move the player", 70, 470, 22, DARKGRAY);
            DrawText("2. Number Keys 1-4 switch between different path algorithms", 70, 500, 22, DARKGRAY);
            DrawText("3. Move from Start (S) to End (E) to win", 70, 530, 22, DARKGRAY);
            DrawText("4. Press 0 to hide all paths", 70, 560, 22, DARKGRAY);
            DrawText("5. Press ESC to exit or return to menu", 70, 590, 22, DARKGRAY);
            
            DrawText("Path Algorithms:", 50, 650, 28, DARKGRAY);
            DrawText("1: DFS (Depth First Search) - Blue", 70, 690, 22, DARKGRAY);
            DrawText("2: BFS (Breadth First Search) - Yellow", 70, 720, 22, DARKGRAY);
            DrawText("3: Dijkstra - Purple", 70, 750, 22, DARKGRAY);
            DrawText("4: A* Algorithm - Orange", 70, 780, 22, DARKGRAY);
        }
        else if (gameState == PLAYING) {
            maze.Draw(screenWidth, screenHeight);
            player.Draw(maze);
            
            DrawText("Controls:", 10, screenHeight - 170, 22, DARKGRAY);
            DrawText("Arrow Keys: Move Player", 20, screenHeight - 140, 20, DARKGRAY);
            DrawText("1: DFS Path  2: BFS Path", 20, screenHeight - 115, 20, DARKGRAY);
            DrawText("3: Dijkstra Path  4: A* Path", 20, screenHeight - 90, 20, DARKGRAY);
            DrawText("0: Hide Paths  ESC: Menu", 20, screenHeight - 65, 20, DARKGRAY);
            
            std::string pathType;
            switch (maze.GetDisplayPath()) {
                case 1: pathType = "DFS (Blue)"; break;
                case 2: pathType = "BFS (Yellow)"; break;
                case 3: pathType = "Dijkstra (Purple)"; break;
                case 4: pathType = "A* (Orange)"; break;
                default: pathType = "None"; break;
            }
            DrawText(("Current Path: " + pathType).c_str(), screenWidth - 250, screenHeight - 40, 20, DARKGRAY);
            
            Point pos = player.GetPosition();
            std::string posText = "Position: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")";
            DrawText(posText.c_str(), 10, 10, 22, DARKGRAY);
            
            DrawText("Terrain Legend:", screenWidth - 250, 10, 22, DARKGRAY);
            DrawText("S: Start (Green)", screenWidth - 250, 40, 18, DARKGRAY);
            DrawText("E: End (Red)", screenWidth - 250, 65, 18, DARKGRAY);
            DrawText("Gray: Floor", screenWidth - 250, 90, 18, DARKGRAY);
            DrawText("Dark Gray: Wall", screenWidth - 250, 115, 18, DARKGRAY);
            DrawText("Green: Grass", screenWidth - 250, 140, 18, DARKGRAY);
            DrawText("Orange: Lava", screenWidth - 250, 165, 18, DARKGRAY);
        }
        else if (gameState == VICTORY) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 150, 0, 200});
            DrawText("CONGRATULATIONS!", screenWidth/2 - MeasureText("CONGRATULATIONS!", 60)/2, screenHeight/2 - 80, 60, GOLD);
            DrawText("You reached the end of the maze!", screenWidth/2 - MeasureText("You reached the end of the maze!", 35)/2, screenHeight/2 + 10, 35, WHITE);
            
            Rectangle restartButton = {screenWidth/2 - 180, screenHeight/2 + 80, 360, 60};
            DrawRectangleRec(restartButton, BLUE);
            DrawRectangleLinesEx(restartButton, 3, DARKBLUE);
            DrawText("Press SPACE to Play Again", screenWidth/2 - MeasureText("Press SPACE to Play Again", 28)/2, screenHeight/2 + 95, 28, WHITE);
            
            Rectangle menuButton = {screenWidth/2 - 180, screenHeight/2 + 160, 360, 60};
            DrawRectangleRec(menuButton, DARKGRAY);
            DrawRectangleLinesEx(menuButton, 3, GRAY);
            DrawText("Press ESC to Return to Menu", screenWidth/2 - MeasureText("Press ESC to Return to Menu", 28)/2, screenHeight/2 + 175, 28, WHITE);
        }
        
        DrawFPS(10, 10);
        EndDrawing();
    }
    
    maze.UnloadTextures();
    CloseWindow();
    
    return 0;
}