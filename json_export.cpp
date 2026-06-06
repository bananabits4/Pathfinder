#include "json_export.h"
#include <set>
#include <utility>

// ── helper: turn a string vector into a JSON array of strings ──────────────
std::string JsonExporter::quoteList(const std::vector<std::string>& v) {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) os << ",";
        os << "\"" << v[i] << "\"";
    }
    os << "]";
    return os.str();
}

// ── build array of node objects ────────────────────────────────────────────
std::string JsonExporter::nodeList(const Graph& g) {
    std::ostringstream os;
    os << "[";
    auto ids = g.nodes();
    for (size_t i = 0; i < ids.size(); ++i) {
        if (i) os << ",";
        const NodeMeta& m = g.meta(ids[i]);
        os << std::fixed << std::setprecision(2);
        os << "{\"id\":\"" << m.id << "\","
           << "\"x\":"     << m.x  << ","
           << "\"y\":"     << m.y  << "}";
    }
    os << "]";
    return os.str();
}

// ── build array of edge objects (deduplicated) ─────────────────────────────
std::string JsonExporter::edgeList(const Graph& g) {
    // use a set to avoid duplicates for undirected edges
    using EdgeKey = std::pair<std::string,std::string>;
    std::set<EdgeKey>        seen;
    std::ostringstream       os;
    std::vector<std::string> objs;

    for (const std::string& u : g.nodes()) {
        for (const Edge& e : g.neighbours(u)) {
            EdgeKey key = (u < e.to)
                        ? EdgeKey{u, e.to}
                        : EdgeKey{e.to, u};
            if (seen.count(key)) continue;
            seen.insert(key);

            std::ostringstream obj;
            obj << std::fixed << std::setprecision(2);
            obj << "{\"from\":\"" << u    << "\","
                << "\"to\":\""   << e.to << "\","
                << "\"weight\":" << e.weight << "}";
            objs.push_back(obj.str());
        }
    }

    os << "[";
    for (size_t i = 0; i < objs.size(); ++i) {
        if (i) os << ",";
        os << objs[i];
    }
    os << "]";
    return os.str();
}

// ── top-level JSON builder ─────────────────────────────────────────────────
std::string JsonExporter::buildJson(
    const Graph&        g,
    const SearchResult& bfs,
    const SearchResult& astar,
    const std::string&  startNode,
    const std::string&  goalNode)
{
    std::ostringstream os;
    os << std::fixed << std::setprecision(4);

    os << "{\n";
    os << "  \"start\": \""   << startNode << "\",\n";
    os << "  \"goal\": \""    << goalNode  << "\",\n";
    os << "  \"nodes\": "     << nodeList(g) << ",\n";
    os << "  \"edges\": "     << edgeList(g) << ",\n";

    // BFS
    os << "  \"bfs\": {\n";
    os << "    \"found\": "      << (bfs.found ? "true" : "false") << ",\n";
    os << "    \"totalCost\": "  << bfs.totalCost   << ",\n";
    os << "    \"path\": "       << quoteList(bfs.path)       << ",\n";
    os << "    \"visitOrder\": " << quoteList(bfs.visitOrder) << "\n";
    os << "  },\n";

    // A*
    os << "  \"astar\": {\n";
    os << "    \"found\": "      << (astar.found ? "true" : "false") << ",\n";
    os << "    \"totalCost\": "  << astar.totalCost   << ",\n";
    os << "    \"path\": "       << quoteList(astar.path)       << ",\n";
    os << "    \"visitOrder\": " << quoteList(astar.visitOrder) << "\n";
    os << "  }\n";
    os << "}\n";

    return os.str();
}
