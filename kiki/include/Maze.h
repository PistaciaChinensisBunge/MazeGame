#ifndef MAZE_H
#define MAZE_H

#include "raylib.h"
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <stack>
#include <climits>
#include <algorithm>
#include <iostream>

struct Point {
    int x, y;
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class Maze {
public:
    enum TerrainType {
        TERRAIN_START = -1,
        TERRAIN_END = -2,
        TERRAIN_FLOOR = 0,
        TERRAIN_WALL = 1,
        TERRAIN_GRASS = 2,
        TERRAIN_LAVA = 3
    };
    
    struct PathResult {
        std::vector<Point> dfsPath;
        std::vector<Point> bfsPath;
        std::vector<Point> dijkstraPath;
        std::vector<Point> aStarPath;
    };
    
    Maze();
    ~Maze();
    
    bool LoadFromFile(const std::string& filename);
    void GenerateRandom(int rows, int cols);
    void Clear();
    
    void CalculateAllPaths();
    void SetDisplayPath(int pathType);
    int GetDisplayPath() const { return currentDisplayPath; }
    
    void Draw(int screenWidth, int screenHeight);
    void DrawPaths();
    void DrawGrid() const;
    
    Point GetStart() const { return startPoint; }
    Point GetEnd() const { return endPoint; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetCellSize() const { return cellSize; }
    TerrainType GetCell(int x, int y) const;
    bool IsWalkable(int x, int y) const;
    
    void LoadTextures();
    void UnloadTextures();
    
    std::vector<Point> FindPathDFS(Point start, Point end);
    std::vector<Point> FindPathBFS(Point start, Point end);
    std::vector<Point> FindPathDijkstra(Point start, Point end);
    std::vector<Point> FindPathAStar(Point start, Point end);
    
    int GetOffsetX() const { return offsetX; }
    int GetOffsetY() const { return offsetY; }
    
private:
    std::vector<std::vector<int>> grid;
    std::map<int, Texture2D> textures;
    
    PathResult paths;
    Point startPoint;
    Point endPoint;
    int width, height;
    int cellSize;
    int currentDisplayPath;
    int offsetX, offsetY;
    
    std::vector<Point> ReconstructPath(const std::map<Point, Point>& cameFrom, Point current);
    int Heuristic(Point a, Point b);
    bool IsValid(Point p) const;
    
    void DrawCell(int x, int y) const;
    void DrawPath(const std::vector<Point>& path, Color color);
    
    static const std::vector<Point> directions;
};

#endif // MAZE_H