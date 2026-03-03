#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include "dsets.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"

using namespace cs225;

class SquareMaze {
  public:
    SquareMaze();

    void makeMaze(int width, int height);

    bool canTravel(int x, int y, int dir) const;

    void setWall(int x, int y, int dir, bool exists);

    std::vector<int> solveMaze();

    PNG* drawMaze() const;

    PNG* drawMazeWithSolution();

  private:
    int width_, height_;
    int bestX_;
    std::vector<bool> rightWalls;
    std::vector<bool> downWalls;

    inline int cellIndex(int x, int y) const {
        return y * width_ + x;
    }
};

#endif

