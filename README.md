# Pathfinder — A* vs BFS

A C++ implementation of **A\* Search** and **Breadth-First Search** on a weighted graph, with an interactive browser visualizer.

![Graph with BFS and A* paths highlighted](https://via.placeholder.com/800x400/0e1117/e2e8f0?text=visualizer+screenshot)

## Features

- Clean multi-file C++ project (graph, search algorithms, JSON export)
- Both algorithms run on the same graph and results are compared side by side
- A\* uses a Euclidean heuristic derived from 2D node coordinates
- Outputs `result.json` which the visualizer reads — no hardcoded data in the HTML
- Step-by-step animation showing exploration order for each algorithm

## File Structure

```
pathfinder/
├── graph.h / graph.cpp         # Weighted undirected graph, NodeMeta with (x, y) coords
├── search.h / search.cpp       # BFS and A* implementations
├── json_export.h / json_export.cpp  # Serializes results to result.json
├── main.cpp                    # Entry point: builds graph, runs algos, writes JSON
├── visualizer.html             # Browser visualizer (reads result.json via fetch)
└── CMakeLists.txt              # CMake build file
```

## Build & Run

**Requirements:** C++17 compiler, CMake 3.14+ (or just g++ directly)

### With CMake
```bash
cmake -B build && cmake --build build
./build/pathfinder
```

### With g++ directly
```bash
g++ -std=c++17 -O2 -o pathfinder main.cpp graph.cpp search.cpp json_export.cpp
./pathfinder
```

This prints results to stdout and writes `result.json`.

## Visualizer

Because the visualizer uses `fetch`, you need to serve the files over HTTP — browsers block fetch on `file://` URLs.

```bash
python3 -m http.server 8080
# open http://localhost:8080/visualizer.html
```

The sidebar shows cost, nodes visited, and the path found for each algorithm. Hit **Replay** to watch the exploration animate step by step.

## How It Works

### Graph
Nodes are stored with `(x, y)` coordinates. These serve two purposes: A\*'s heuristic uses them to compute straight-line distance to the goal, and the visualizer uses them to place nodes on the canvas.

### BFS
Explores nodes level by level (shortest hops first). Finds a valid path but ignores edge weights when choosing which neighbor to visit next, so it won't always find the cheapest route.

### A\*
Uses a priority queue ordered by `g(n) + h(n)` — actual cost so far plus estimated cost to goal. The heuristic `h(n)` is the Euclidean distance between node coordinates. This guides the search toward the goal efficiently and always finds the optimal path (given an admissible heuristic).

### Sample Graph Results (A → K)

| Algorithm | Path | Cost | Nodes visited |
|-----------|------|------|---------------|
| BFS       | A→B→F→I→K | 24 | 12 |
| A\*       | A→C→G→J→K | 21 | 5  |

A\* visits fewer nodes and finds a cheaper path by using spatial information BFS ignores.

## Customizing the Graph

Edit `main.cpp` — add nodes with `g.addNode("X", x, y)` and edges with `g.addEdge("X", "Y", weight)`, then recompile and rerun. The visualizer picks up the new `result.json` automatically on refresh.

## License

MIT
