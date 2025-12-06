#include "Player.h"
#include <iostream>

Player::Player() : size(24), reachedEnd(false) {
    position = {0, 0};
    texture = {0};
}

Player::~Player() {
    if (texture.id > 0) {
        UnloadTexture(texture);
    }
}

void Player::SetPosition(int x, int y) {
    position = {x, y};
    reachedEnd = false;
}

void Player::Move(int dx, int dy, const Maze& maze) {
    if (reachedEnd) return;
    
    int newX = position.x + dx;
    int newY = position.y + dy;
    
    if (maze.IsWalkable(newX, newY)) {
        position.x = newX;
        position.y = newY;
        
        if (maze.GetCell(newX, newY) == Maze::TERRAIN_END) {
            reachedEnd = true;
        }
    }
}

void Player::Draw(const Maze& maze) const {
    int cellSize = maze.GetCellSize();
    int offsetX = maze.GetOffsetX();
    int offsetY = maze.GetOffsetY();
    
    int screenX = offsetX + position.x * cellSize + (cellSize - size) / 2;
    int screenY = offsetY + position.y * cellSize + (cellSize - size) / 2;
    
    if (texture.id > 0) {
        DrawTexture(texture, screenX, screenY, WHITE);
    } else {
        DrawCircle(screenX + size/2, screenY + size/2, size/2, BLUE);
        DrawCircleLines(screenX + size/2, screenY + size/2, size/2, DARKBLUE);
    }
}

void Player::Reset() {
    reachedEnd = false;
}

void Player::LoadTexture(const char* filename) {
    if (!FileExists(filename)) {
        std::cout << "Warning: Player texture not found: " << filename << std::endl;
        return;
    }
    
    Image image = LoadImage(filename);
    if (image.data == NULL) {
        std::cout << "Warning: Cannot load player image: " << filename << std::endl;
        return;
    }
    
    Texture2D loadedTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    
    if (loadedTexture.id > 0) {
        texture = loadedTexture;
        std::cout << "Loaded player texture: " << filename << std::endl;
    } else {
        std::cout << "Warning: Cannot create player texture: " << filename << std::endl;
    }
}