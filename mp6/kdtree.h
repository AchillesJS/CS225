/**
 * @file kdtree.h
 * KDTree implementation using Points in k-dimensional space.
 *
 *
 */

#ifndef _KDTREE_H_
#define _KDTREE_H_

#include <vector>
#include "util/coloredout.h"
#include "point.h"

using std::vector;
using std::string;
using std::ostream;
using std::cout;
using std::endl;

template <int Dim>
class KDTree
{
  private:
    /** Internal node of KDTree */
    struct KDTreeNode
    {
      Point<Dim> point;
      KDTreeNode *left, *right;
      int idx;                              

      KDTreeNode() : point(), left(NULL), right(NULL), idx(-1) { }
      KDTreeNode(const Point<Dim>& p, int i)
          : point(p), left(NULL), right(NULL), idx(i) { }
    };

  public:
    /* --------------––---–----------------------------------------​ */
    bool smallerDimVal(const Point<Dim>& first,
                       const Point<Dim>& second, int curDim) const;

    bool shouldReplace(const Point<Dim>& target,
                       const Point<Dim>& currentBest,
                       const Point<Dim>& potential) const;

    KDTree(const vector<Point<Dim>>& newPoints);
    KDTree(const KDTree& other);
    KDTree const & operator=(const KDTree& rhs);
    ~KDTree();

    
    Point<Dim> findNearestNeighbor(const Point<Dim>& query) const;

    Point<Dim> findNearestNeighbor(const Point<Dim>& query,
                                   int& outIndex) const;

    /* --------------––---–----------------------------------------​ */
    void printTree(ostream& out = cout,
                   colored_out::enable_t enable_bold = colored_out::COUT,
                   int modWidth = -1) const;

  private:
    KDTreeNode *root;
    size_t size;

    int  getPrintData(KDTreeNode * subroot) const;
    void printTree  (KDTreeNode * subroot, std::vector<std::string>& output,
                     int left, int top, int width, int currd) const;

    
};

#include "kdtree.cpp"
#include "kdtree_extras.cpp"
#endif

