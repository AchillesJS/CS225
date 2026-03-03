/**
 * @file graph_tools.cpp
 * This is where you will implement several functions that operate on graphs.
 * Be sure to thoroughly read the comments above each function, as they give
 *  hints and instructions on how to solve the problems.
 */

#include "graph_tools.h"

/**
 * Finds the minimum edge weight in the Graph graph.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to search
 * @return the minimum weighted edge
 *
 * @todo Label the minimum edge as "MIN". It will appear blue when
 *  graph.savePNG() is called in minweight_test.
 *
 * @note You must do a traversal.
 * @note You may use the STL stack and queue.
 * @note You may assume the graph is connected.
 *
 * @hint Initially label vertices and edges as unvisited.
 */
int GraphTools::findMinWeight(Graph& graph)
{
    for (Vertex v : graph.getVertices())
        graph.setVertexLabel(v, "UNEXPLORED");

    std::vector<Edge> edges = graph.getEdges();
    for (const Edge& e : edges)
        graph.setEdgeLabel(e.source, e.dest, "UNEXPLORED");

    Vertex start = graph.getVertices().front();
    std::queue<Vertex> q;
    q.push(start);
    graph.setVertexLabel(start, "VISITED");

    int    minWeight = edges[0].weight;
    Vertex minU      = edges[0].source;
    Vertex minV      = edges[0].dest;

    while (!q.empty()) {
        Vertex cur = q.front(); q.pop();
        for (Vertex neigh : graph.getAdjacent(cur)) {
            int w = graph.getEdgeWeight(cur, neigh);
            if (w < minWeight) {
                minWeight = w;
                minU = cur;  minV = neigh;
            }
            if (graph.getVertexLabel(neigh) == "UNEXPLORED") {
                graph.setVertexLabel(neigh, "VISITED");
                q.push(neigh);
            }
        }
    }
    graph.setEdgeLabel(minU, minV, "MIN");
    return minWeight;
}

/**
 * Returns the shortest distance (in edges) between the Vertices
 *  start and end.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to search
 * @param start - the vertex to start the search from
 * @param end - the vertex to find a path to
 * @return the minimum number of edges between start and end
 *
 * @todo Label each edge "MINPATH" if it is part of the minimum path
 *
 * @note Remember this is the shortest path in terms of edges,
 *  not edge weights.
 * @note Again, you may use the STL stack and queue.
 * @note You may also use the STL's unordered_map, but it is possible
 *  to solve this problem without it.
 *
 * @hint In order to draw (and correctly count) the edges between two
 *  vertices, you'll have to remember each vertex's parent somehow.
 */
int GraphTools::findShortestPath(Graph& graph, Vertex start, Vertex end)
{
    for (Vertex v : graph.getVertices())
        graph.setVertexLabel(v, "UNEXPLORED");
    for (const Edge& e : graph.getEdges())
        graph.setEdgeLabel(e.source, e.dest, "UNEXPLORED");

    std::queue<Vertex> q;
    std::unordered_map<Vertex, Vertex> parent;

    q.push(start);
    graph.setVertexLabel(start, "VISITED");
    parent[start] = start;

    while (!q.empty() && graph.getVertexLabel(end) != "VISITED") {
        Vertex cur = q.front(); q.pop();
        for (Vertex neigh : graph.getAdjacent(cur)) {
            if (graph.getVertexLabel(neigh) == "UNEXPLORED") {
                graph.setVertexLabel(neigh, "VISITED");
                parent[neigh] = cur;
                q.push(neigh);
            }
        }
    }

    int length = 0;
    for (Vertex v = end; v != start; v = parent[v]) {
        Vertex p = parent[v];
        graph.setEdgeLabel(p, v, "MINPATH");
        ++length;
    }
    return length;
}

/**
 * Finds a minimal spanning tree on a graph.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to find the MST of
 *
 * @todo Label the edges of a minimal spanning tree as "MST"
 *  in the graph. They will appear blue when graph.savePNG() is called.
 *
 * @note Use your disjoint sets class from MP 7.1 to help you with
 *  Kruskal's algorithm. Copy the files into the libdsets folder.
 * @note You may call std::sort instead of creating a priority queue.
 */
void GraphTools::findMST(Graph& graph)
{
    for (const Edge& e : graph.getEdges())
        graph.setEdgeLabel(e.source, e.dest, "UNEXPLORED");

    const std::vector<Vertex> verts = graph.getVertices();
    std::unordered_map<Vertex, int> index;
    for (unsigned i = 0; i < verts.size(); ++i) index[verts[i]] = i;

    DisjointSets dsets;
    dsets.addelements(verts.size());

    std::vector<Edge> edges = graph.getEdges();
    std::sort(edges.begin(), edges.end(),
              [](const Edge& a, const Edge& b) { return a.weight < b.weight; });

    int used = 0;
    for (const Edge& e : edges) {
        int setU = dsets.find(index[e.source]);
        int setV = dsets.find(index[e.dest]);

        if (setU != setV) {
            dsets.setunion(setU, setV);
            graph.setEdgeLabel(e.source, e.dest, "MST");
            if (++used == static_cast<int>(verts.size()) - 1) break;
        }
    }
}

