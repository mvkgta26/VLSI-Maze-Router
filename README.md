**MAIN: Router/router.cpp** <br>
**OUTPUT/SOLUTION: Router/solution.txt** <br>
**SOLUTION VISUAL: Router/SHORTEST-ROUTE-VISUAL.jpg**

# VLSI-Maze-Router
Finds the shortest path from Source-Cell to Destination-Cell through a maze on a grid of squares.

## Maze Router Algorithm:
### 2 Main Steps:

1)**EXPAND**: Breadth first search to find all paths from source cell to target cell. Expansion algorithm used is a variant of Dijkstra's Algorithm.

2)**BACKTRACE**: Trace shortest path back to source from the target cell



## Implementation Details:

### 2-Key Data Structures:

1) **Routing Grid**:
Routing grid is represented by a 2D Array of objects. Each cell holds the x and y coordinates of corresponding cell.

2) **Wavefront**:
A Min-Heap of grid-cell objects, sorted by the amount of distance from source.
