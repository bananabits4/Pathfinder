#include "graph.h"
#include <cmath>
#include <stdexcept>

void Graph::addNode(const std::string& id, double x, double y) {
    meta_[id] = { id, x, y };
    if (adj_.find(id) == adj_.end())
        adj_[id] = {};
}

void Graph::addEdge(const std::string& from, const std::string& to, double weight) {
    if (!hasNode(from) || !hasNode(to))
        throw std::invalid_argument("Edge references unknown node");
    adj_[from].push_back({ to,   weight });
    adj_[to  ].push_back({ from, weight });
}

const std::vector<Edge>& Graph::neighbours(const std::string& id) const {
    auto it = adj_.find(id);
    if (it == adj_.end()) throw std::invalid_argument("Unknown node: " + id);
    return it->second;
}

const NodeMeta& Graph::meta(const std::string& id) const {
    auto it = meta_.find(id);
    if (it == meta_.end()) throw std::invalid_argument("Unknown node: " + id);
    return it->second;
}

std::vector<std::string> Graph::nodes() const {
    std::vector<std::string> ids;
    ids.reserve(meta_.size());
    for (auto& kv : meta_) ids.push_back(kv.first);
    return ids;
}

bool Graph::hasNode(const std::string& id) const {
    return meta_.find(id) != meta_.end();
}

double Graph::heuristic(const std::string& a, const std::string& b) const {
    const auto& ma = meta(a);
    const auto& mb = meta(b);
    double dx = ma.x - mb.x;
    double dy = ma.y - mb.y;
    return std::sqrt(dx*dx + dy*dy);
}
