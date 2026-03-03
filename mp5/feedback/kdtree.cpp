/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
 if (first[curDim] < second[curDim])
        return true;
    if (second[curDim] < first[curDim])
        return false;
    return first < second;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */

       double distCur = 0, distPot = 0;
    for (int i = 0; i < Dim; ++i) {
        double d1 = target[i] - currentBest[i];
        double d2 = target[i] - potential[i];
        distCur += d1*d1;
        distPot += d2*d2;
    }
    if (distPot < distCur) return true;
    if (distCur < distPot) return false;
    return potential < currentBest;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
     if (newPoints.empty()) {
        root = nullptr;
        size = 0;
        return;
    }
    
    vector<Point<Dim>> pts = newPoints;
    size = pts.size();

    struct Builder {
        static KDTreeNode* build(vector<Point<Dim>>& pts,
                                 int lo, int hi, int depth)
        {
            if (lo >= hi) return nullptr;
            int dim = depth % Dim;
            int mid = lo + (hi - lo - 1) / 2;
            nth_element(pts.begin()+lo, pts.begin()+mid, pts.begin()+hi,
                [dim](const Point<Dim>& a, const Point<Dim>& b) {
                    if (a[dim] != b[dim]) return a[dim] < b[dim];
                    return a < b;
                });
            auto* node = new KDTreeNode(pts[mid]);
            node->left  = build(pts, lo,      mid,      depth+1);
            node->right = build(pts, mid+1,   hi,       depth+1);
            return node;
        }
    };

    root = Builder::build(pts, 0, pts.size(), 0);
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree& other)
    : size(other.size)
{
    struct Cloner {
        static KDTreeNode* clone(KDTreeNode* n) {
            if (!n) return nullptr;
            auto* c = new KDTreeNode(n->point);
            c->left  = clone(n->left);
            c->right = clone(n->right);
            return c;
        }
    };
    root = Cloner::clone(other.root);
}
template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree& rhs) {
  /**
   * @todo Implement this function!
   */

    if (this != &rhs) {
        struct Destroyer {
            static void destroy(KDTreeNode* n) {
                if (!n) return;
                destroy(n->left);
                destroy(n->right);
                delete n;
            }
        };
        Destroyer::destroy(root);

        size = rhs.size;
        struct Cloner {
            static KDTreeNode* clone(KDTreeNode* n) {
                if (!n) return nullptr;
                auto* c = new KDTreeNode(n->point);
                c->left  = clone(n->left);
                c->right = clone(n->right);
                return c;
            }
        };
        root = Cloner::clone(rhs.root);
    }
    return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
   struct Destroyer {
        static void destroy(KDTreeNode* n) {
            if (!n) return;
            destroy(n->left);
            destroy(n->right);
            delete n;
        }
    };
    Destroyer::destroy(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
     Point<Dim> best;
    double bestDist = 0;
    bool first = true;

    struct NN {
        static void search(const KDTreeNode* node,
                           const Point<Dim>& query,
                           Point<Dim>& best,
                           double& bestDist,
                           bool& first,
                           int depth)
        {
            if (!node) return;
            int d = depth % Dim;
            const KDTreeNode* near  = (query[d] < node->point[d] ? node->left  : node->right);
            const KDTreeNode* far   = (near == node->left             ? node->right : node->left);
            search(near, query, best, bestDist, first, depth+1);

            double dist = 0;
            for (int i = 0; i < Dim; ++i) {
                double diff = query[i] - node->point[i];
                dist += diff*diff;
            }
            if (first || dist < bestDist || (dist == bestDist && node->point < best)) {
                best     = node->point;
                bestDist = dist;
                first    = false;
            }
            double diff = query[d] - node->point[d];
            if (first || diff*diff <= bestDist)
                search(far, query, best, bestDist, first, depth+1);
        }
    };

    NN::search(root, query, best, bestDist, first, 0);
    return best;
}

