#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

// ─────────────────────────────────────────────
//  Graph: weighted undirected adjacency list
// ─────────────────────────────────────────────

struct Edge {
    std::string to;
    double      weight;
};

struct NodeMeta {
    std::string id;
    double      x, y;       // 2-D position (used as heuristic coords)
};

class Graph {
public:
    // Add a node with optional 2-D coords
    void addNode(const std::string& id, double x = 0.0, double y = 0.0);

    // Add a weighted, undirected edge
    void addEdge(const std::string& from, const std::string& to, double weight);

    // Accessors
    const std::vector<Edge>&     neighbours(const std::string& id) const;
    const NodeMeta&              meta      (const std::string& id) const;
    std::vector<std::string>     nodes     ()                      const;
    bool                         hasNode   (const std::string& id) const;

    // Straight-line (Euclidean) distance between two nodes – used as A* heuristic
    double heuristic(const std::string& a, const std::string& b) const;

private:
    std::unordered_map<std::string, NodeMeta>        meta_;
    std::unordered_map<std::string, std::vector<Edge>> adj_;
};
