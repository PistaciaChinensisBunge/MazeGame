#include "Maze.h"
#include <fstream>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <random>
#include <chrono>

const std::vector<Point> Maze::directions = {
    {0, -1},  // 上
    {1, 0},   // 右
    {0, 1},   // 下
    {-1, 0}   // 左
};

Maze::Maze() : width(0), height(0), cellSize(32), currentDisplayPath(0), offsetX(0), offsetY(0) {}

Maze::~Maze() {
    UnloadTextures();
}

bool Maze::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open maze file: " << filename << std::endl;
        return false;
    }
    
    grid.clear();
    
    file >> height >> width;
    
    grid.resize(height, std::vector<int>(width));
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            file >> grid[y][x];
            
            if (grid[y][x] == TERRAIN_START) {
                startPoint = {x, y};
            } else if (grid[y][x] == TERRAIN_END) {
                endPoint = {x, y};
            }
        }
    }
    
    file.close();
    
    std::cout << "Maze loaded successfully: " << filename << std::endl;
    std::cout << "Size: " << width << "x" << height << std::endl;
    std::cout << "Start: (" << startPoint.x << ", " << startPoint.y << ")" << std::endl;
    std::cout << "End: (" << endPoint.x << ", " << endPoint.y << ")" << std::endl;
    
    CalculateAllPaths();
    
    return true;
}

void Maze::GenerateRandom(int rows, int cols) {
    width = cols;
    height = rows;
    
    grid.clear();
    grid.resize(height, std::vector<int>(width, TERRAIN_FLOOR));
    
    for (int x = 0; x < width; x++) {
        grid[0][x] = TERRAIN_WALL;
        grid[height - 1][x] = TERRAIN_WALL;
    }
    for (int y = 0; y < height; y++) {
        grid[y][0] = TERRAIN_WALL;
        grid[y][width - 1] = TERRAIN_WALL;
    }
    
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(1, width - 2);
    
    int wallCount = (width * height) / 4;
    for (int i = 0; i < wallCount; i++) {
        int x = dist(rng);
        int y = dist(rng);
        
        if (!(x == 1 && y == 1) && !(x == width - 2 && y == height - 2)) {
            grid[y][x] = TERRAIN_WALL;
        }
    }
    
    startPoint = {1, 1};
    endPoint = {width - 2, height - 2};
    grid[startPoint.y][startPoint.x] = TERRAIN_START;
    grid[endPoint.y][endPoint.x] = TERRAIN_END;
    
    CalculateAllPaths();
}

void Maze::Clear() {
    grid.clear();
    width = 0;
    height = 0;
}

void Maze::CalculateAllPaths() {
    if (width == 0 || height == 0) return;
    
    std::cout << "Calculating paths..." << std::endl;
    paths.dfsPath = FindPathDFS(startPoint, endPoint);
    paths.bfsPath = FindPathBFS(startPoint, endPoint);
    paths.dijkstraPath = FindPathDijkstra(startPoint, endPoint);
    paths.aStarPath = FindPathAStar(startPoint, endPoint);
    
    std::cout << "DFS path length: " << paths.dfsPath.size() << std::endl;
    std::cout << "BFS path length: " << paths.bfsPath.size() << std::endl;
    std::cout << "Dijkstra path length: " << paths.dijkstraPath.size() << std::endl;
    std::cout << "A* path length: " << paths.aStarPath.size() << std::endl;
}

void Maze::SetDisplayPath(int pathType) {
    currentDisplayPath = pathType;
}

void Maze::Draw(int screenWidth, int screenHeight) {
    offsetX = (screenWidth - width * cellSize) / 2;
    offsetY = (screenHeight - height * cellSize) / 2 - 50;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            DrawCell(x, y);
        }
    }
    
    DrawPaths();
    DrawGrid();
}

void Maze::DrawCell(int x, int y) const {
    Rectangle rect = {
        static_cast<float>(offsetX + x * cellSize),
        static_cast<float>(offsetY + y * cellSize),
        static_cast<float>(cellSize),
        static_cast<float>(cellSize)
    };
    
    int terrain = grid[y][x];
    
    auto it = textures.find(terrain);
    if (it != textures.end() && it->second.id > 0) {
        DrawTexture(it->second, offsetX + x * cellSize, offsetY + y * cellSize, WHITE);
    } else {
        Color color;
        switch (terrain) {
            case TERRAIN_START: 
                color = GREEN;
                DrawRectangleRec(rect, color);
                DrawText("S", offsetX + x * cellSize + 10, offsetY + y * cellSize + 5, 20, WHITE);
                break;
            case TERRAIN_END: 
                color = RED;
                DrawRectangleRec(rect, color);
                DrawText("E", offsetX + x * cellSize + 10, offsetY + y * cellSize + 5, 20, WHITE);
                break;
            case TERRAIN_FLOOR: color = LIGHTGRAY; break;
            case TERRAIN_WALL: color = DARKGRAY; break;
            case TERRAIN_GRASS: color = GREEN; break;
            case TERRAIN_LAVA: color = ORANGE; break;
            default: color = GRAY; break;
        }
        if (terrain != TERRAIN_START && terrain != TERRAIN_END) {
            DrawRectangleRec(rect, color);
        }
    }
    
    DrawRectangleLinesEx(rect, 1, GRAY);
}

void Maze::DrawPaths() {
    switch (currentDisplayPath) {
        case 1: // DFS
            DrawPath(paths.dfsPath, BLUE);
            break;
        case 2: // BFS
            DrawPath(paths.bfsPath, YELLOW);
            break;
        case 3: // Dijkstra
            DrawPath(paths.dijkstraPath, PURPLE);
            break;
        case 4: // A*
            DrawPath(paths.aStarPath, ORANGE);
            break;
        default:
            break;
    }
}

void Maze::DrawPath(const std::vector<Point>& path, Color color) {
    if (path.size() < 2) return;
    
    for (size_t i = 0; i < path.size() - 1; i++) {
        Point p1 = path[i];
        Point p2 = path[i + 1];
        
        int x1 = offsetX + p1.x * cellSize + cellSize / 2;
        int y1 = offsetY + p1.y * cellSize + cellSize / 2;
        int x2 = offsetX + p2.x * cellSize + cellSize / 2;
        int y2 = offsetY + p2.y * cellSize + cellSize / 2;
        
        DrawLine(x1, y1, x2, y2, color);
        DrawCircle(x1, y1, 3, color);
    }
    
    if (!path.empty()) {
        Point last = path.back();
        int x = offsetX + last.x * cellSize + cellSize / 2;
        int y = offsetY + last.y * cellSize + cellSize / 2;
        DrawCircle(x, y, 3, color);
    }
}

void Maze::DrawGrid() const {
    for (int x = 0; x <= width; x++) {
        DrawLine(offsetX + x * cellSize, offsetY, 
                 offsetX + x * cellSize, offsetY + height * cellSize, GRAY);
    }
    for (int y = 0; y <= height; y++) {
        DrawLine(offsetX, offsetY + y * cellSize, 
                 offsetX + width * cellSize, offsetY + y * cellSize, GRAY);
    }
}

Maze::TerrainType Maze::GetCell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return static_cast<TerrainType>(grid[y][x]);
    }
    return TERRAIN_WALL;
}

bool Maze::IsWalkable(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return grid[y][x] != TERRAIN_WALL;
}

bool Maze::IsValid(Point p) const {
    return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height && grid[p.y][p.x] != TERRAIN_WALL;
}

std::vector<Point> Maze::FindPathDFS(Point start, Point end) {
    if (!IsValid(start) || !IsValid(end)) return {};
    
    std::vector<Point> path;
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::map<Point, Point> parent;
    std::stack<Point> stack;
    
    stack.push(start);
    visited[start.y][start.x] = true;
    
    while (!stack.empty()) {
        Point current = stack.top();
        stack.pop();
        
        if (current.x == end.x && current.y == end.y) {
            return ReconstructPath(parent, current);
        }
        
        for (const auto& dir : directions) {
            Point next = {current.x + dir.x, current.y + dir.y};
            
            if (IsValid(next) && !visited[next.y][next.x]) {
                visited[next.y][next.x] = true;
                parent[next] = current;
                stack.push(next);
            }
        }
    }
    
    return {};
}

std::vector<Point> Maze::FindPathBFS(Point start, Point end) {
    if (!IsValid(start) || !IsValid(end)) return {};
    
    std::vector<Point> path;
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::map<Point, Point> parent;
    std::queue<Point> queue;
    
    queue.push(start);
    visited[start.y][start.x] = true;
    
    while (!queue.empty()) {
        Point current = queue.front();
        queue.pop();
        
        if (current.x == end.x && current.y == end.y) {
            return ReconstructPath(parent, current);
        }
        
        for (const auto& dir : directions) {
            Point next = {current.x + dir.x, current.y + dir.y};
            
            if (IsValid(next) && !visited[next.y][next.x]) {
                visited[next.y][next.x] = true;
                parent[next] = current;
                queue.push(next);
            }
        }
    }
    
    return {};
}

std::vector<Point> Maze::FindPathDijkstra(Point start, Point end) {
    if (!IsValid(start) || !IsValid(end)) return {};
    
    auto cmp = [](const std::pair<int, Point>& a, const std::pair<int, Point>& b) {
        return a.first > b.first;
    };
    
    std::priority_queue<std::pair<int, Point>, 
                        std::vector<std::pair<int, Point>>, 
                        decltype(cmp)> pq(cmp);
    
    std::vector<std::vector<int>> distance(height, std::vector<int>(width, INT_MAX));
    std::map<Point, Point> parent;
    
    distance[start.y][start.x] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto currentPair = pq.top();
        pq.pop();
        int dist = currentPair.first;
        Point current = currentPair.second;
        
        if (dist > distance[current.y][current.x]) continue;
        
        if (current.x == end.x && current.y == end.y) {
            return ReconstructPath(parent, current);
        }
        
        for (const auto& dir : directions) {
            Point next = {current.x + dir.x, current.y + dir.y};
            
            if (IsValid(next)) {
                int newDist = dist + 1;
                
                if (newDist < distance[next.y][next.x]) {
                    distance[next.y][next.x] = newDist;
                    parent[next] = current;
                    pq.push({newDist, next});
                }
            }
        }
    }
    
    return {};
}

std::vector<Point> Maze::FindPathAStar(Point start, Point end) {
    if (!IsValid(start) || !IsValid(end)) return {};
    
    struct ComparePoint {
        std::vector<std::vector<int>>* fScore;
        
        bool operator()(const Point& a, const Point& b) const {
            return (*fScore)[a.y][a.x] > (*fScore)[b.y][b.x];
        }
    };
    
    std::vector<std::vector<int>> gScore(height, std::vector<int>(width, INT_MAX));
    std::vector<std::vector<int>> fScore(height, std::vector<int>(width, INT_MAX));
    std::map<Point, Point> cameFrom;
    
    gScore[start.y][start.x] = 0;
    fScore[start.y][start.x] = Heuristic(start, end);
    
    ComparePoint cmp;
    cmp.fScore = &fScore;
    
    std::priority_queue<Point, std::vector<Point>, ComparePoint> openSet(cmp);
    std::vector<std::vector<bool>> inOpenSet(height, std::vector<bool>(width, false));
    
    openSet.push(start);
    inOpenSet[start.y][start.x] = true;
    
    while (!openSet.empty()) {
        Point current = openSet.top();
        openSet.pop();
        inOpenSet[current.y][current.x] = false;
        
        if (current.x == end.x && current.y == end.y) {
            return ReconstructPath(cameFrom, current);
        }
        
        for (const auto& dir : directions) {
            Point neighbor = {current.x + dir.x, current.y + dir.y};
            
            if (!IsValid(neighbor)) continue;
            
            int tentativeGScore = gScore[current.y][current.x] + 1;
            
            if (tentativeGScore < gScore[neighbor.y][neighbor.x]) {
                cameFrom[neighbor] = current;
                gScore[neighbor.y][neighbor.x] = tentativeGScore;
                fScore[neighbor.y][neighbor.x] = tentativeGScore + Heuristic(neighbor, end);
                
                if (!inOpenSet[neighbor.y][neighbor.x]) {
                    openSet.push(neighbor);
                    inOpenSet[neighbor.y][neighbor.x] = true;
                }
            }
        }
    }
    
    return {};
}

std::vector<Point> Maze::ReconstructPath(const std::map<Point, Point>& cameFrom, Point current) {
    std::vector<Point> path;
    while (cameFrom.find(current) != cameFrom.end()) {
        path.push_back(current);
        current = cameFrom.at(current);
    }
    path.push_back(current);
    std::reverse(path.begin(), path.end());
    return path;
}

int Maze::Heuristic(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void Maze::LoadTextures() {
    std::map<int, std::string> textureFiles = {
        {TERRAIN_START, "resources/start.png"},
        {TERRAIN_END, "resources/end.png"},
        {TERRAIN_FLOOR, "resources/floor.png"},
        {TERRAIN_WALL, "resources/wall.png"},
        {TERRAIN_GRASS, "resources/grass.png"},
        {TERRAIN_LAVA, "resources/lava.png"}
    };
    
    for (const auto& pair : textureFiles) {
        const std::string& filename = pair.second;
        
        if (!FileExists(filename.c_str())) {
            std::cout << "Warning: Texture file not found: " << filename << std::endl;
            continue;
        }
        
        Image image = LoadImage(filename.c_str());
        if (image.data == NULL) {
            std::cout << "Warning: Cannot load image: " << filename << std::endl;
            continue;
        }
        
        Texture2D loadedTexture = LoadTextureFromImage(image);
        UnloadImage(image);
        
        if (loadedTexture.id > 0) {
            textures[pair.first] = loadedTexture;
            std::cout << "Loaded texture: " << filename << std::endl;
        } else {
            std::cout << "Warning: Cannot create texture: " << filename << std::endl;
        }
    }
}

void Maze::UnloadTextures() {
    for (auto& pair : textures) {
        if (pair.second.id > 0) {
            UnloadTexture(pair.second);
        }
    }
    textures.clear();
}