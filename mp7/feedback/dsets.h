#ifndef DSETS_H
#define DSETS_H

#include <vector>

class DisjointSets {
public:
    // Add N singleton sets
    void addelements(int N);
    // Find the representative (root) of x
    int find(int x);
    // Union the sets containing i and j
    void setunion(int i, int j);
    // Return the size of the set containing x
    int size(int x) const;

private:
    std::vector<int> parent;
};

#endif

