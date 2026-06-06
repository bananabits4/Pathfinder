#pragma once
#include "graph.h"
#include <vector>
#include <string>

// ─────────────────────────────────────────────
//  SearchResult – common return type
// ─────────────────────────────────────────────

struct SearchResult {
    std::vector<std::string> path;          // node IDs from start → goal
    double                   totalCost;     // sum of edge weights along path
    std::vector<std::string> visitOrder;    // order nodes were first expanded
    bool                     found;

    SearchResult() : totalCost(0.0), found(false) {}
};

// ─────────────────────────────────────────────
//  Algorithm declarations
// ─────────────────────────────────────────────

// Breadth-First Search (treats all edges as cost 1 for path, but records real cost)
SearchResult bfs(const Graph& g, const std::string& start, const std::string& goal);

// A* Search (uses Euclidean heuristic from node coordinates)
SearchResult astar(const Graph& g, const std::string& start, const std::string& goal);
