/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

Point<3> convertToLAB(HSLAPixel pixel) {
    Point<3> result(pixel.h/360, pixel.s, pixel.l);
    return result;
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    int rows = theSource.getRows();
    int cols = theSource.getColumns();

    vector<Point<3>> points;
    points.reserve(theTiles.size());
    for (int i = 0; i < (int)theTiles.size(); i++) {
        Point<3> p = convertToLAB(theTiles[i].getAverageColor());
        points.push_back(p);
    }

    for (size_t i = 0; i < theTiles.size(); ++i) {
        TileImage empty;
        theTiles[i] = empty;
    }

    KDTree<3> tree(points);
    MosaicCanvas* canvas = new MosaicCanvas(rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            HSLAPixel regionColor = theSource.getRegionColor(r, c);
            Point<3> target = convertToLAB(regionColor);
            int idx = -1;
            tree.findNearestNeighbor(target, idx);
            canvas->setTile(r, c, &theTiles[idx]);
        }
    }

    return canvas;
}

TileImage* get_match_at_idx(const KDTree<3>& tree,
                                  map<Point<3>, int> tile_avg_map,
                                  vector<TileImage>& theTiles,
                                  const SourceImage& theSource, int row,
                                  int col)
{
    HSLAPixel avg = theSource.getRegionColor(row, col);
    Point<3> avgPoint = convertToLAB(avg);

    int index = -1;
    tree.findNearestNeighbor(avgPoint, index);

    return &theTiles[index];
}

