# maze-solver
# INTRODUCTION

This is a program to solve pixel mazes. It is written in C++ using openCV. Mazes are passed in as black and white bitmap files, where the black pixels are walls and white pixels are path squares. Mazes must have only one white pixel on the top and bottom rows representing the start and end of the maze respectively. I have included some example mazes, though I recommend using the Daedalus Maze Generation software to create more. 

# TODO
* Implement more algorithms (high priority)
  * Breadth First Search
  * Dijkstra's Algorithm
  * A*
* Better command line parsing (medium priority)
  * include file system operations
  * include an option to run all the algorithms
* Restructure file hierarchy for source code (low priority)
* Create a GUI using Qt (low priority)
* Restructure code with adjacency list (low priority)
