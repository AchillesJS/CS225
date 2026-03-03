/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num)
{
    elems.insert(elems.end(), num, -1);
}

int DisjointSets::find(int elem)
{
    if (elems[elem] < 0) return elem;
    return elems[elem] = find(elems[elem]);   
}

void DisjointSets::setunion(int a, int b)
{
    a = find(a);
    b = find(b);
    if (a == b) return;

    if (elems[a] > elems[b]) std::swap(a, b);

    elems[a] += elems[b];   
    elems[b]  = a;          
}

int DisjointSets::size(int elem)
{
    int root = find(elem);
    return -elems[root];
}

