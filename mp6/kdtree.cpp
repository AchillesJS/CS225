/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

namespace {
    template <int Dim>
    inline double sd(const Point<Dim>& a, const Point<Dim>& b) {
        double d = 0;
        for (int i = 0; i < Dim; ++i) {
            double diff = a[i] - b[i];
            d += diff * diff;
        }
        return d;
    }
}

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    if (first[curDim] < second[curDim]) return true;
    if (second[curDim] < first[curDim]) return false;
    return first < second;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    double dc = sd<Dim>(target, currentBest);
    double dp = sd<Dim>(target, potential);
    if (dp < dc) return true;
    if (dc < dp) return false;
    return potential < currentBest;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& ptsIn)
{
    if (ptsIn.empty()) { root = nullptr; size = 0; return; }

    vector<pair<Point<Dim>, int>> pts;
    pts.reserve(ptsIn.size());
    for (size_t i = 0; i < ptsIn.size(); ++i)
        pts.emplace_back(ptsIn[i], static_cast<int>(i));
    size = pts.size();

    struct Builder {
        static KDTreeNode* build(vector<pair<Point<Dim>, int>>& p,
                                 int l, int r, int depth) {
            if (l >= r) return nullptr;
            int ax = depth % Dim;
            int m  = l + (r - l - 1) / 2;
            nth_element(p.begin() + l, p.begin() + m, p.begin() + r,
                        [ax](const std::pair<Point<Dim>, int>& a,
                             const std::pair<Point<Dim>, int>& b) {
                            if (a.first[ax] != b.first[ax])
                                return a.first[ax] < b.first[ax];
                            return a.first < b.first;
                        });
            KDTreeNode* n = new KDTreeNode(p[m].first, p[m].second);
            n->left  = build(p, l,     m, depth + 1);
            n->right = build(p, m + 1, r, depth + 1);
            return n;
        }
    };

    root = Builder::build(pts, 0, static_cast<int>(pts.size()), 0);
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree& other) : size(other.size)
{
    struct Clone {
        static KDTreeNode* go(KDTreeNode* n) {
            if (!n) return nullptr;
            KDTreeNode* c = new KDTreeNode(n->point, n->idx);
            c->left  = go(n->left);
            c->right = go(n->right);
            return c;
        }
    };
    root = Clone::go(other.root);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree& rhs)
{
    if (this != &rhs) {
        struct Destroy { static void go(KDTreeNode* n){ if(!n) return; go(n->left); go(n->right); delete n; } };
        Destroy::go(root);
        size = rhs.size;
        struct Clone { static KDTreeNode* go(KDTreeNode* n){ if(!n) return nullptr; KDTreeNode* c=new KDTreeNode(n->point,n->idx); c->left=go(n->left); c->right=go(n->right); return c; } };
        root = Clone::go(rhs.root);
    }
    return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree()
{
    struct Destroy { static void go(KDTreeNode* n){ if(!n) return; go(n->left); go(n->right); delete n; } };
    Destroy::go(root);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& q) const
{
    int dummy = -1;
    return findNearestNeighbor(q, dummy);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& q, int& idx) const
{
    Point<Dim> best; double bd = 0.0; bool first = true;
    struct Search {
        static void go(const KDTreeNode* n, const Point<Dim>& q,
                       Point<Dim>& best, int& bestIdx,
                       double& bd, bool& first, int depth) {
            if (!n) return;
            int ax = depth % Dim;
            const KDTreeNode* near = (q[ax] < n->point[ax]) ? n->left : n->right;
            const KDTreeNode* far  = (near == n->left) ? n->right : n->left;
            go(near, q, best, bestIdx, bd, first, depth + 1);
            double dist = sd<Dim>(q, n->point);
            if (first || dist < bd || (dist == bd && n->point < best)) {
                best = n->point; bestIdx = n->idx; bd = dist; first = false;
            }
            double diff = q[ax] - n->point[ax];
            if (first || diff * diff <= bd)
                go(far, q, best, bestIdx, bd, first, depth + 1);
        }
    };
    idx = -1;
    Search::go(root, q, best, idx, bd, first, 0);
    return best;
}

