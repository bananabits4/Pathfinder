#include "search.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <functional>
#include <algorithm>

using namespace std;

// ─────────────────────────────────────────────
//  Helper: reconstruct path from cameFrom map
// ─────────────────────────────────────────────
static vector<string> reconstruct(
    const unordered_map<string,string>& cameFrom,
    const string& start,
    const string& goal)
{
    vector<string> path;
    string cur = goal;
    while (cur != start) {
        path.push_back(cur);
        cur = cameFrom.at(cur);
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

// ─────────────────────────────────────────────
//  BFS  (unweighted hop-count exploration)
//  Cost is computed a posteriori along the path.
// ─────────────────────────────────────────────
SearchResult bfs(const Graph& g, const string& start, const string& goal) {
    SearchResult res;

    queue<string> frontier;
    unordered_set<string> visited;
    unordered_map<string,string> cameFrom;
    unordered_map<string,double> costTo;

    frontier.push(start);
    visited.insert(start);
    costTo[start] = 0.0;

    while (!frontier.empty()) {
        string cur = frontier.front();
        frontier.pop();
        res.visitOrder.push_back(cur);

        if (cur == goal) {
            res.found = true;
            res.path  = reconstruct(cameFrom, start, goal);

            for (size_t i = 1; i < res.path.size(); ++i) {
                const string& u = res.path[i-1];
                const string& v = res.path[i];
                for (const Edge& e : g.neighbours(u)) {
                    if (e.to == v) {
                        res.totalCost += e.weight;
                        break;
                    }
                }
            }
            return res;
        }

        for (const Edge& edge : g.neighbours(cur)) {
            if (!visited.count(edge.to)) {
                visited.insert(edge.to);
                cameFrom[edge.to] = cur;
                frontier.push(edge.to);
            }
        }
    }
    return res;
}

// ─────────────────────────────────────────────
//  A*  (weighted edges + Euclidean heuristic)
// ─────────────────────────────────────────────
SearchResult astar(const Graph& g, const string& start, const string& goal) {
    SearchResult res;

    using Pair = pair<double, string>;
    priority_queue<Pair, vector<Pair>, greater<Pair>> open;

    unordered_map<string,double> gScore;
    unordered_map<string,string> cameFrom;
    unordered_set<string> closed;

    for (const string& n : g.nodes())
        gScore[n] = numeric_limits<double>::infinity();

    gScore[start] = 0.0;

    open.push({g.heuristic(start, goal), start});

    while (!open.empty()) {
        auto [f, cur] = open.top();
        open.pop();

        if (closed.count(cur))
            continue;

        closed.insert(cur);
        res.visitOrder.push_back(cur);

        if (cur == goal) {
            res.found     = true;
            res.totalCost = gScore[goal];
            res.path      = reconstruct(cameFrom, start, goal);
            return res;
        }

        for (const Edge& edge : g.neighbours(cur)) {
            if (closed.count(edge.to))
                continue;

            double tentative = gScore[cur] + edge.weight;

            if (tentative < gScore[edge.to]) {
                gScore[edge.to]   = tentative;
                cameFrom[edge.to] = cur;

                double h = g.heuristic(edge.to, goal);
                open.push({tentative + h, edge.to});
            }
        }
    }

    return res;
}