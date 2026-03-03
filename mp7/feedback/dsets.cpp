#include "dsets.h"
#include <cassert>

void DisjointSets::addelements(int N) {
    for (int i = 0; i < N; i++) {
        parent.push_back(-1);
    }
}

int DisjointSets::find(int x) {
    assert(x >= 0 && x < (int)parent.size());
    if (parent[x] < 0) return x;
    parent[x] = find(parent[x]);
    return parent[x];
}

void DisjointSets::setunion(int a, int b) {
    int rootA = find(a);
    int rootB = find(b);
    if (rootA == rootB) return;
    // “union by size”: parent[root] 
    if (parent[rootA] < parent[rootB]) {
        parent[rootA] += parent[rootB];
        parent[rootB] = rootA;
    } else {
        parent[rootB] += parent[rootA];
        parent[rootA] = rootB;
    }
}

int DisjointSets::size(int x) const {
    int root = x;
    while (parent[root] >= 0) {
        root = parent[root];
    }
    return -parent[root];
}

