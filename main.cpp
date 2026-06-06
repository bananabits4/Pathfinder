#include "graph.h"
#include "search.h"
#include "json_export.h"

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

// ─────────────────────────────────────────────
//  Pretty-print a SearchResult to stdout
// ─────────────────────────────────────────────
static void printResult(const string& algoName, const SearchResult& r) {
    cout << "\n═══════════════════════════════════════\n";
    cout << "  " << algoName << "\n";
    cout << "═══════════════════════════════════════\n";

    if (!r.found) {
        cout << "  ✗ No path found.\n";
        return;
    }

    cout << "  Path  : ";
    for (size_t i = 0; i < r.path.size(); ++i) {
        if (i) cout << " → ";
        cout << r.path[i];
    }
    cout << "\n";
    cout << "  Cost  : " << fixed << setprecision(2)
         << r.totalCost << "\n";
    cout << "  Visited (" << r.visitOrder.size() << " nodes): ";
    for (size_t i = 0; i < r.visitOrder.size(); ++i) {
        if (i) cout << ", ";
        cout << r.visitOrder[i];
    }
    cout << "\n";
}

// ─────────────────────────────────────────────
//  Build the example graph
// ─────────────────────────────────────────────
static Graph buildSampleGraph() {
    Graph g;

    // West
g.addNode("Mumbai",      100, 300);
g.addNode("Pune",        180, 320);
g.addNode("Ahmedabad",   120, 180);
g.addNode("Surat",       130, 240);
g.addNode("Rajkot",       60, 180);

// North
g.addNode("Jaipur",      250, 150);
g.addNode("Delhi",       350, 120);
g.addNode("Chandigarh",  330,  50);
g.addNode("Amritsar",    260,  20);
g.addNode("Dehradun",    420,  80);
g.addNode("Lucknow",     500, 140);
g.addNode("Kanpur",      520, 170);
g.addNode("Varanasi",    620, 180);

// Central
g.addNode("Bhopal",      320, 250);
g.addNode("Indore",      250, 270);
g.addNode("Nagpur",      400, 320);
g.addNode("Raipur",      550, 300);

// East
g.addNode("Patna",       700, 150);
g.addNode("Ranchi",      700, 250);
g.addNode("Kolkata",     850, 220);
g.addNode("Bhubaneswar", 760, 350);

// South
g.addNode("Hyderabad",   420, 420);
g.addNode("Vijayawada",  560, 450);
g.addNode("Visakhapatnam",650, 400);
g.addNode("Bengaluru",   350, 550);
g.addNode("Mysuru",      300, 620);
g.addNode("Chennai",     520, 620);
g.addNode("Coimbatore",  380, 700);
g.addNode("Madurai",     450, 760);
g.addNode("Kochi",       300, 760);
g.addNode("Thiruvananthapuram", 320, 850);

// Major connections
g.addEdge("Mumbai","Pune",150);
g.addEdge("Mumbai","Surat",280);
g.addEdge("Surat","Ahmedabad",270);
g.addEdge("Ahmedabad","Rajkot",220);

g.addEdge("Ahmedabad","Jaipur",660);
g.addEdge("Jaipur","Delhi",280);
g.addEdge("Delhi","Chandigarh",250);
g.addEdge("Chandigarh","Amritsar",230);
g.addEdge("Delhi","Dehradun",250);

g.addEdge("Delhi","Lucknow",550);
g.addEdge("Lucknow","Kanpur",90);
g.addEdge("Kanpur","Varanasi",330);
g.addEdge("Varanasi","Patna",250);

g.addEdge("Indore","Bhopal",190);
g.addEdge("Jaipur","Indore",600);
g.addEdge("Bhopal","Nagpur",350);

g.addEdge("Lucknow","Nagpur",680);
g.addEdge("Nagpur","Raipur",290);
g.addEdge("Raipur","Ranchi",450);
g.addEdge("Ranchi","Kolkata",400);
g.addEdge("Patna","Ranchi",330);
g.addEdge("Patna","Kolkata",580);

g.addEdge("Raipur","Bhubaneswar",550);
g.addEdge("Bhubaneswar","Kolkata",440);

g.addEdge("Nagpur","Hyderabad",500);
g.addEdge("Hyderabad","Vijayawada",280);
g.addEdge("Vijayawada","Visakhapatnam",350);
g.addEdge("Visakhapatnam","Kolkata",870);

g.addEdge("Hyderabad","Bengaluru",570);
g.addEdge("Bengaluru","Mysuru",150);
g.addEdge("Bengaluru","Chennai",350);

g.addEdge("Bengaluru","Coimbatore",360);
g.addEdge("Coimbatore","Kochi",190);
g.addEdge("Coimbatore","Madurai",220);

g.addEdge("Madurai","Thiruvananthapuram",300);
g.addEdge("Kochi","Thiruvananthapuram",210);

g.addEdge("Chennai","Vijayawada",450);
g.addEdge("Chennai","Madurai",460);

    return g;
}

// ─────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────
int main() {
    const string START = "Dehradun";
    const string GOAL  = "Thiruvananthapuram";

    cout << "Pathfinder — A* vs BFS\n";
    cout << "Graph: 12 nodes, 20 edges\n";
    cout << "Start: " << START << "   Goal: " << GOAL << "\n";

    Graph g = buildSampleGraph();
    SearchResult bfsR   = bfs(g, START, GOAL);
    SearchResult astarR = astar(g, START, GOAL);

    printResult("Breadth-First Search (BFS)", bfsR);
    printResult("A* Search", astarR);

    string json = JsonExporter::buildJson(g, bfsR, astarR, START, GOAL);
    ofstream out("result.json");
    out << json;
    out.close();

    cout << "\n✓ result.json written — open visualizer.html to explore.\n";

    return 0;
}