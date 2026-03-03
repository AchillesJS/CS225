/**
 * @file NimLearner.cpp
 * CS 225 - Fall 2017
 */

#include "NimLearner.h"
#include <string>
#include <cstdlib>
#include <ctime>

/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true) {
  for (int t = startingTokens; t >= 0; --t) {
    Vertex v1 = g_.insertVertex("p1-" + std::to_string(t));
    g_.insertVertex("p2-" + std::to_string(t));
    if (t == (int)startingTokens) startingVertex_ = v1;
  }
  for (int t = startingTokens; t > 0; --t) {
    Vertex p1cur = g_.getVertexByLabel("p1-" + std::to_string(t));
    Vertex p2cur = g_.getVertexByLabel("p2-" + std::to_string(t));
    for (int take = 1; take <= 2 && t - take >= 0; ++take) {
      Vertex p2next = g_.getVertexByLabel("p2-" + std::to_string(t - take));
      g_.insertEdge(p1cur, p2next);
      g_.setEdgeWeight(p1cur, p2next, 0);
      Vertex p1next = g_.getVertexByLabel("p1-" + std::to_string(t - take));
      g_.insertEdge(p2cur, p1next);
      g_.setEdgeWeight(p2cur, p1next, 0);
    }
  }
  std::srand(std::time(nullptr));
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const {
  std::vector<Edge> path;
  Vertex cur = startingVertex_;
  while (true) {
    std::string lbl = g_.getVertexLabel(cur);
    if (lbl.substr(lbl.find('-') + 1) == "0") break;
    std::vector<Vertex> adj = g_.getAdjacent(cur);
    Vertex nxt = adj[std::rand() % adj.size()];
    path.push_back(g_.getEdge(cur, nxt));
    cur = nxt;
  }
  return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) {
  if (path.empty()) return;
  std::string lastLabel = g_.getVertexLabel(path.back().dest);
  bool p1win = lastLabel.find("p2-0") == 0;
  for (const Edge & e : path) {
    std::string src = g_.getVertexLabel(e.source);
    bool srcP1 = src.find("p1") == 0;
    int delta = (p1win ? (srcP1 ? 1 : -1) : (srcP1 ? -1 : 1));
    int w = g_.getEdgeWeight(e.source, e.dest);
    g_.setEdgeWeight(e.source, e.dest, w + delta);
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}

