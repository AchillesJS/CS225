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
    map<Point<3>, int> avgMap;
    for (int i = 0; i < (int)theTiles.size(); i++) {
        Point<3> p = convertToLAB(theTiles[i].getAverageColor());
        points.push_back(p);
        avgMap[p] = i;
    }

    KDTree<3> tree(points);
    MosaicCanvas* canvas = new MosaicCanvas(rows, cols);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            HSLAPixel regionColor = theSource.getRegionColor(r, c);
            Point<3> target = convertToLAB(regionColor);
            Point<3> nearest = tree.findNearestNeighbor(target);
            int idx = avgMap[nearest];
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
    // Create a tile which accurately represents the source region we'll be
    // using
    HSLAPixel avg = theSource.getRegionColor(row, col);
    Point<3> avgPoint = convertToLAB(avg);
    Point<3> nearestPoint = tree.findNearestNeighbor(avgPoint);

    // Check to ensure the point exists in the map
    map< Point<3>, int >::iterator it = tile_avg_map.find(nearestPoint);
    if (it == tile_avg_map.end())
        cerr << "Didn't find " << avgPoint << " / " << nearestPoint << endl;

    // Find the index
    int index = tile_avg_map[nearestPoint];
    return &theTiles[index];

}
