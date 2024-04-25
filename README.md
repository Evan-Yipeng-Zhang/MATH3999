# MATH3999

# Capstone Mathematics Project - Computational Optimization

## Description
This project is a comprehensive exploration of computational optimization techniques, specifically addressing the Travelling Salesman Problem (TSP) and the Shortest Superstring Problem (SSP). The project includes implementations of several algorithms, such as Christofides' Algorithm for TSP and a Factor-4 Approximation for SSP, showcasing both theoretical analysis and practical application scenarios.

## Algorithms Overview
This project includes several algorithms, each designed for specific computational problems related to graph theory and string processing. Below is a brief description of each algorithm's function:

### Algorithm 1
- Implements a basic approach for solving the Travelling Salesman Problem (TSP) by generating all permutations of vertices to find the shortest possible route that visits each city and returns to the origin city.

### Algorithm 2
- Utilizes a greedy algorithm to approximate the solution to the TSP. This algorithm selects the shortest edges available while ensuring no cycles are formed, except for the cycle that closes the tour.

### Algorithm 3
- Employs dynamic programming to tackle the TSP, storing solutions to subproblems to avoid recalculating them and thus, significantly reducing the computational complexity.

### Algorithm 4
- Adopts a branch and bound approach for TSP, which systematically explores branches of the solution space by estimating the lower bounds of partial solutions and pruning branches that cannot yield a better solution than the best known.

### Algorithm 5
- Implements Christofides' algorithm, which provides a solution for metric TSP with a guarantee of no more than 1.5 times the optimal path. This method combines minimum spanning trees, minimum weight perfect matching, and shortest path algorithms.

### Algorithm 6
- Utilizes Professor William Cook's Blossom algorithm to generate minimum weighted perfect matching, crucial for constructing optimal solutions in Christofides' algorithm and other applications that require perfect matching in weighted graphs.

### Algorithm 7
- Solves the Shortest Superstring Problem, aiming to find the shortest superstring that contains all given strings as substrings. This algorithm is useful in fields such as bioinformatics for DNA sequencing, where concatenating multiple sequences efficiently is crucial.

## Features
- Implementation of Christofides' Algorithm for efficiently solving the metric TSP.
- Factor-4 Approximation algorithm for the Shortest Superstring Problem.
- Utilization of Professor William Cook's Blossom algorithm for generating minimum weighted perfect matchings as part of Algorithm 6.

## Getting Started

### Prerequisites
- Visual Studio 2022 or compatible versions.
- Access to the internet to download external tools required by Algorithm 6.

### Installation
1. Clone or download the repository to your local machine.
2. Open the solution file (`ProjectName.sln`) in Visual Studio.
3. Build the project to ensure all configurations are correctly set up.

### External Tools
- This project requires Professor William Cook's implementation of the Blossom algorithm, which is an addition to the Concorde package. Download and install from:
  - Blossom Algorithm: [math.uwaterloo.ca](https://math.uwaterloo.ca/~bico/software.html)
  - Concorde Package: [math.princeton.edu](http://www.math.princeton.edu/tsp/concorde.html)
- After installation, create a file named `Blossom4Path` in the project directory to specify the path to the executable of Cook's program. An example file is provided within the repository.

### Usage
The software can process different types of input formats depending on the algorithm being used:

#### For Algorithms 1-6 (Graph-Based Problems)
1. **Edge Format (Type 1)**
   - The first line contains the number of vertices (`n`) and edges (`m`).
   - Each subsequent line describes an edge, consisting of two vertices connected by the edge and the weight of that edge, formatted as `vertex1 vertex2 weight`.
   - Example:
     ```
     3 3
     1 2 10.0
     2 0 15.0
     1 0 20.0
     ```

2. **2-Dimensional Coordinates Format (Type 2)**
   - The first line contains the number of vertices (`n`).
   - Each subsequent line contains the coordinates of a vertex, formatted as `x-coordinate y-coordinate`.
   - Example:
     ```
     3
     0.0 0.0
     1.0 1.0
     2.0 2.0
     ```

  The output format mirrors the Type 1 input format, providing a list of edges with their associated weights, representing the solution to the applied algorithm.

#### For Algorithm 7 (Shortest Superstring Problem)
- Each line in the input file represents a string.
- The output is a single line representing the shortest superstring that contains all input strings as substrings.
- Example Input:
  ```
  abc
  bcd
  cde
  ```
- Example Output:
  ```
  abcde
  ```

To run the program, use the command format:

MATH3999 AlgorithmNumber inputFilePath outputFilePath

Where:
- `AlgorithmNumber` is the number representing one of the implemented algorithms (1-6).
- `inputFilePath` is the path to the input file.
- `outputFilePath` is the path where the output will be saved.

### Additional Details
Ensure you have created the Blossom4Path file to specify the location of Professor William Cook's program if using Algorithm 6.

## Built With
- Visual Studio 2022
- C++ Standard (C++17)

## Authors
- Zhang Yipeng
