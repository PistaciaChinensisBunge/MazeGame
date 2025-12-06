#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Maze.h"

class Player {
public:
    Player();
    ~Player();
    
    void SetPosition(int x, int y);
    void Move(int dx, int dy, const Maze& maze);
    void Draw(const Maze& maze) const;
    void Reset();
    
    Point GetPosition() const { return position; }
    bool HasReachedEnd() const { return reachedEnd; }
    
    void LoadTexture(const char* filename);
    
private:
    Point position;
    Texture2D texture;
    int size;
    bool reachedEnd;
};

#endif // PLAYER_H