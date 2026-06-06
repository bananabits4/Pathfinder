#pragma once
#include "graph.h"
#include "search.h"
#include <string>
#include <sstream>
#include <iomanip>

// ─────────────────────────────────────────────
//  Tiny JSON builder – no external deps
// ─────────────────────────────────────────────

class JsonExporter {
public:
    // Build a full JSON blob for the visualiser
    static std::string buildJson(
        const Graph&        g,
        const SearchResult& bfsResult,
        const SearchResult& astarResult,
        const std::string&  startNode,
        const std::string&  goalNode);

private:
    static std::string quoteList(const std::vector<std::string>& v);
    static std::string edgeList (const Graph& g);
    static std::string nodeList (const Graph& g);
};
