// maze.cpp
#include "maze.h"
#include "dsets.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <vector>
#include <random>
#include <queue>
#include <algorithm>

using namespace cs225;

SquareMaze::SquareMaze() : width_(0), height_(0), bestX_(0) { }

void SquareMaze::makeMaze(int width, int height) {
    width_ = width;
    height_ = height;

    DisjointSets dsets;
    dsets.addelements(width_ * height_);

    rightWalls.assign(width_ * height_, true);
    downWalls.assign(width_ * height_, true);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distDir(0, 1);
    std::uniform_int_distribution<> distCell(0, width_ * height_ - 1);

    int remaining = width_ * height_ - 1;
    while (remaining > 0) {
        int cell = distCell(gen);
        int dir = distDir(gen);
        int x = cell % width_;
        int y = cell / width_;

        if (dir == 0) {
            if (x == width_ - 1) continue;
            int neighbor = y * width_ + (x + 1);
            int r1 = dsets.find(cell);
            int r2 = dsets.find(neighbor);
            if (r1 != r2) {
                dsets.setunion(r1, r2);
                rightWalls[cell] = false;
                --remaining;
            }
        } else {
            if (y == height_ - 1) continue;
            int neighbor = (y + 1) * width_ + x;
            int r1 = dsets.find(cell);
            int r2 = dsets.find(neighbor);
            if (r1 != r2) {
                dsets.setunion(r1, r2);
                downWalls[cell] = false;
                --remaining;
            }
        }
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    if (dir == 0 && x + 1 < width_) {
        return !rightWalls[y * width_ + x];
    }
    if (dir == 1 && y + 1 < height_) {
        return !downWalls[y * width_ + x];
    }
    if (dir == 2 && x - 1 >= 0) {
        return !rightWalls[y * width_ + (x - 1)];
    }
    if (dir == 3 && y - 1 >= 0) {
        return !downWalls[(y - 1) * width_ + x];
    }
    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if (dir == 0) {
        rightWalls[y * width_ + x] = exists;
    } else if (dir == 1) {
        downWalls[y * width_ + x] = exists;
    }
}

std::vector<int> SquareMaze::solveMaze() {
    std::vector<bool> visited(width_ * height_, false);
    std::vector<int> dist(width_ * height_, 0);
    std::vector<int> parent(width_ * height_, -1);
    std::queue<int> q;

    visited[0] = true;
    q.push(0);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        int x = curr % width_;
        int y = curr / width_;

        for (int dir = 0; dir < 4; dir++) {
            if (canTravel(x, y, dir)) {
                int nx = x + (dir == 0) - (dir == 2);
                int ny = y + (dir == 1) - (dir == 3);
                int next = ny * width_ + nx;
                if (!visited[next]) {
                    visited[next] = true;
                    dist[next] = dist[curr] + 1;
                    parent[next] = curr * 4 + dir;
                    q.push(next);
                }
            }
        }
    }

    int bestDist = -1;
    bestX_ = 0;
    for (int x = 0; x < width_; x++) {
        int cell = (height_ - 1) * width_ + x;
        if (visited[cell] &&
            (dist[cell] > bestDist ||
             (dist[cell] == bestDist && x < bestX_)))
        {
            bestDist = dist[cell];
            bestX_ = x;
        }
    }

    std::vector<int> path;
    int curr = (height_ - 1) * width_ + bestX_;
    while (curr != 0) {
        int p = parent[curr];
        int dir = p % 4;
        path.push_back(dir);
        curr = p / 4;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

PNG* SquareMaze::drawMaze() const {
    PNG* image = new PNG(width_ * 10 + 1, height_ * 10 + 1);

    for (int x = 10; x < width_ * 10 + 1; x++) {
        HSLAPixel* p = image->getPixel(x, 0);
        *p = HSLAPixel(0, 0, 0);
    }

    for (int y = 0; y < height_ * 10 + 1; y++) {
        HSLAPixel* p = image->getPixel(0, y);
        *p = HSLAPixel(0, 0, 0);
    }

    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            if (downWalls[y * width_ + x]) {
                for (int i = 0; i <= 10; i++) {
                    HSLAPixel* p = image->getPixel(x * 10 + i, y * 10 + 10);
                    *p = HSLAPixel(0, 0, 0);
                }
            }
            if (rightWalls[y * width_ + x]) {
                for (int i = 0; i <= 10; i++) {
                    HSLAPixel* p = image->getPixel(x * 10 + 10, y * 10 + i);
                    *p = HSLAPixel(0, 0, 0);
                }
            }
        }
    }

    return image;
}

PNG* SquareMaze::drawMazeWithSolution() {
    PNG* image = drawMaze();
    std::vector<int> solution = solveMaze();

    int x = 5;
    int y = 5;
    for (int dir : solution) {
        int dx = (dir == 0) - (dir == 2);
        int dy = (dir == 1) - (dir == 3);
        for (int k = 0; k <= 10; k++) {
            HSLAPixel* p = image->getPixel(x + dx * k, y + dy * k);
            *p = HSLAPixel(0, 1, 0.5);
        }
        x += dx * 10;
        y += dy * 10;
    }

    for (int i = 1; i < 10; i++) {
        HSLAPixel* p = image->getPixel((bestX_ * 10) + i, height_ * 10);
        *p = HSLAPixel(0, 0, 1);
    }

    return image;
}

