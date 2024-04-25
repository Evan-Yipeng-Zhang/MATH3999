#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <climits>
#include <cstdlib>
#include <algorithm>

class Graph {
public:
    int V;    // Number of vertices
    int E;    // Number of edges
    std::vector < std::list<std::pair<int, float>>> adj; // Adjacency list
    std::vector<std::tuple<float, int, int>> edges; // Edge list
    std::vector<std::vector<float>> adjMatrix; // Adjacency matrix

    Graph(int V, int E);  // Constructor
    void addEdge(int u, int v, float w); // Function to add an edge
    void removeEdge(int u, int v, float w); // Function to remove edge
    void saveGraphToFile(const std::string& filePath); // Save graph

    // Algorithm 1 
    void printEulerTour(const std::string& outputFilePath);
    int DFSCount(int v, std::vector<char>& visited);
    bool isValidNextEdge(int u, int v, float w);

    // Algorithm 2
    void kruskalMST(const std::string& outputPath);
    int find(std::vector<int>& parent, int i); // Find with path compression
    void unionSet(std::vector<int>& parent, std::vector<int>& rank, int u, int v); // Union by rank

    // Algorithm 3
    void primMST(const std::string& outputPath);
    int minKey(std::vector<float>& key, std::vector<bool>& mstSet);

    // Algorithm 4
    void constructHamiltonianCycle(const std::string& outputPath);

    // Algorithm 5
    void duplicateEdgesInMST(const std::string& mstOutputFile, const std::string& duplicatedEdgesFile);
    void convertToHamiltonianCycle(Graph g, const std::string& outputPath);

    // Algorithm 6
    std::vector<int> findOddDegreeVertices();
    void prepareMWPMInput(const std::vector<int>& oddVertices, const std::string& mwpmInputPath, std::unordered_map<int, int>& vertexMap);
};

Graph create_graph(const std::string& inputFile);

#endif // GRAPH_H