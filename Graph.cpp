#include "Graph.h"
#include <fstream>
#include <stack>
#include <iostream>
#include <sstream>

Graph::Graph(int V, int E) : V(V), E(E), adj(V), adjMatrix(V, std::vector<float>(V, 0)) {
    edges.reserve(E);
}

void Graph::addEdge(int u, int v, float w) {
    adj[u].push_back({ v, w });
    adj[v].push_back({ u, w });
    edges.emplace_back(w, u, v);
    adjMatrix[u][v] = w;
    adjMatrix[v][u] = w;
}

void Graph::removeEdge(int u, int v, float w) {
    for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
        if (it->first == v && it->second == w) {
            adj[u].erase(it);
            break;
        }
    }
    for (auto it = adj[v].begin(); it != adj[v].end(); ++it) {
        if (it->first == u && it->second == w) {
            adj[v].erase(it);
            break;
        }
    }
}

void Graph::printEulerTour(const std::string& outputFilePath) {
    std::ofstream outFile(outputFilePath);
    std::stack<int> stack;
    int u = 0;
    for (int i = 0; i < V; i++) {
        if (!adj[i].empty()) {
            u = i;
            break;
        }
    }

    stack.push(u);

    // Write the number of vertices and edges at the top
    outFile << V << " " << E << "\n";

    while (!stack.empty()) {
        int u = stack.top();
        stack.pop();
        std::list<std::pair<int, float>> temp(adj[u].begin(), adj[u].end());

        if (!temp.empty()) {
            for (auto it = temp.begin(); it != temp.end(); ++it) {
                int v = it->first;
                float w = it->second;
                bool isLast = std::next(it) == temp.end(); // Check if this is the last element in the list

                if (isValidNextEdge(u, v, w) || isLast) {
                    outFile << u << " " << v << " " << w << "\n";
                    outFile.flush();
                    removeEdge(u, v, w);
                    stack.push(v);  // Move to the next vertex
                    break;  // Break after pushing to ensure we follow the correct path
                }
            }
        }
    }

    outFile.close();
}

bool Graph::isValidNextEdge(int u, int v, float w) {
    std::vector<char> visited(V, 0);
    int count1 = DFSCount(u, visited);
    removeEdge(u, v, w);
    std::vector<char> visited2(V, 0);
    int count2 = DFSCount(u, visited2);
    addEdge(u, v, w); // Revert changes

    return count1 == count2;
}

int Graph::DFSCount(int v, std::vector<char>& visited) {
    visited[v] = 1;
    int count = 1;
    for (std::pair<int, int> adjacent : adj[v]) {
        if (!visited[adjacent.first]) {
            count += DFSCount(adjacent.first, visited);
        }
    }
    return count;
}

int Graph::find(std::vector<int>& parent, int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

void Graph::unionSet(std::vector<int>& parent, std::vector<int>& rank, int u, int v) {
    int uroot = find(parent, u);
    int vroot = find(parent, v);

    if (rank[uroot] < rank[vroot]) {
        parent[uroot] = vroot;
    }
    else if (rank[uroot] > rank[vroot]) {
        parent[vroot] = uroot;
    }
    else {
        parent[vroot] = uroot;
        rank[uroot]++;
    }
}

void Graph::kruskalMST(const std::string& outputPath) {
    std::vector<int> parent(V);
    std::vector<int> rank(V, 0);
    std::vector<std::tuple<float, int, int>> mst;

    // Initialize parent and rank
    for (int v = 0; v < V; ++v) parent[v] = v;

    // Sort edges based on their weight
    sort(edges.begin(), edges.end());

    // Iterate through all sorted edges
    for (const auto& edge : edges) {
        float weight = std::get<0>(edge);
        int u = std::get<1>(edge);
        int v = std::get<2>(edge);

        int uroot = find(parent, u);
        int vroot = find(parent, v);

        // If including this edge doesn't cause cycle, include it in the result and union the two sets.
        if (uroot != vroot) {
            mst.push_back(std::make_tuple(weight, u, v));
            unionSet(parent, rank, uroot, vroot);
        }
    }

    // Write to output file
    std::ofstream outFile(outputPath);
    if (!outFile) {
        std::cerr << "Cannot open output file." << std::endl;
        return;
    }

    // Write the number of vertices and the number of edges in the MST
    outFile << V << " " << mst.size() << std::endl;

    for (const auto& edge : mst) {
        float weight = std::get<0>(edge);
        int u = std::get<1>(edge);
        int v = std::get<2>(edge);
        outFile << u << " " << v << " " << weight << std::endl;
    }
    outFile.close();
}

int Graph::minKey(std::vector<float>& key, std::vector<bool>& mstSet) {
    float min = FLT_MAX, min_index = -1;

    for (int v = 0; v < V; v++)
        if (!mstSet[v] && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}

void Graph::primMST(const std::string& outputPath) {
    std::vector<int> parent(V); // Array to store constructed MST
    std::vector<float> key(V, FLT_MAX); // Key values used to pick minimum weight edge in cut
    std::vector<bool> mstSet(V, false); // To represent set of vertices not yet included in MST

    key[0] = 0; // Make key 0 so that this vertex is picked as first vertex
    parent[0] = -1; // First node is always root of MST

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet);
        mstSet[u] = true;

        for (int v = 0; v < V; v++)
            if (adjMatrix[u][v] && !mstSet[v] && adjMatrix[u][v] < key[v])
                parent[v] = u, key[v] = adjMatrix[u][v];
    }

    // Write the constructed MST to the output file
    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open " << outputPath << " for writing." << std::endl;
        return;
    }

    // Writing number of vertices and number of edges in MST, which is V-1
    outFile << V << " " << V - 1 << std::endl;

    for (int i = 1; i < V; i++)
        outFile << parent[i] << " " << i << " " << adjMatrix[i][parent[i]] << "\n";
    outFile.close();
}

void Graph::constructHamiltonianCycle(const std::string& outputPath) {
    std::vector<int> cycle = { 0 }; // Starting with vertex 0
    std::vector<bool> inCycle(V, false);
    inCycle[0] = true;

    for (int i = 1; i < V; i++) {
        int closest = -1, minDist = FLT_MAX;
        for (int j = 0; j < V; j++) {
            if (!inCycle[j] && adjMatrix[cycle.back()][j] < minDist) {
                closest = j;
                minDist = adjMatrix[cycle.back()][j];
            }
        }
        cycle.push_back(closest);
        inCycle[closest] = true;
    }
    cycle.push_back(cycle.front()); // Make it a cycle by connecting back to the start

    std::ofstream outFile(outputPath);
    if (!outFile) {
        std::cerr << "Could not open file for writing." << std::endl;
        exit(1);
    }

    outFile << V << " " << V << std::endl;

    for (size_t i = 0; i < cycle.size() - 1; i++) {
        int u = cycle[i];
        int v = cycle[i + 1];
        float weight = adjMatrix[u][v]; // Accessing the weight of the edge
        outFile << u << " " << v << " " << weight << std::endl;
    }
    outFile.close();
}

void Graph::duplicateEdgesInMST(const std::string& mstOutputFile, const std::string& duplicatedEdgesFile) {
    std::ifstream inFile(mstOutputFile);
    std::ofstream outFile(duplicatedEdgesFile);

    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }

    int V, E;
    inFile >> V >> E; // Read number of vertices and edges

    // Write the number of vertices and doubled number of edges to the new file
    outFile << V << " " << 2 * E << std::endl;

    // Duplicate each edge by reading and then writing each edge twice
    int u, v;
    float w;
    std::vector<std::tuple<float, int, int>> edges;
    while (inFile >> u >> v >> w) {
        edges.push_back(std::make_tuple(w, u, v));
    }

    for (const auto& edge : edges) {
        outFile << std::get<1>(edge) << " " << std::get<2>(edge) << " " << std::get<0>(edge) << std::endl;
        outFile << std::get<1>(edge) << " " << std::get<2>(edge) << " " << std::get<0>(edge) << std::endl; // Duplicate
    }
}

void Graph::convertToHamiltonianCycle(Graph g, const std::string& outputPath) {
    std::unordered_set<int> visited;
    std::vector<int> hamiltonianCycle;
    std::vector<float> cycleWeight;
    std::ofstream outFile(outputPath);

    // Write the number of vertices and number of edges (equal to the number of vertices in Hamiltonian cycle) to the new file
    outFile << V << " " << V << std::endl;

    int count = 0;
    for (const auto& edge : edges) {
        if (count == 0) {
            visited.insert(std::get<1>(edge));
            hamiltonianCycle.push_back(std::get<1>(edge));
            visited.insert(std::get<2>(edge));
            hamiltonianCycle.push_back(std::get<2>(edge));
            cycleWeight.push_back(g.adjMatrix[hamiltonianCycle[hamiltonianCycle.size() - 2]][hamiltonianCycle[hamiltonianCycle.size() - 1]]);
        }
        else if (visited.insert(std::get<2>(edge)).second) { // Successfully inserted means not visited before
            hamiltonianCycle.push_back(std::get<2>(edge));
            cycleWeight.push_back(g.adjMatrix[hamiltonianCycle[hamiltonianCycle.size() - 2]][hamiltonianCycle[hamiltonianCycle.size() - 1]]);
        }
        count++;
    }
    hamiltonianCycle.push_back(hamiltonianCycle.front()); // Closing the cycle
    cycleWeight.push_back(g.adjMatrix[hamiltonianCycle[hamiltonianCycle.size() - 2]][hamiltonianCycle[hamiltonianCycle.size() - 1]]);

    for (size_t i = 0; i < hamiltonianCycle.size() - 1; ++i) {
        outFile << hamiltonianCycle[i] << " " << hamiltonianCycle[i + 1] << " " << cycleWeight[i] << '\n';
    }
}

std::vector<int> Graph::findOddDegreeVertices() {
    std::unordered_map<int, int> degreeCount;
    for (int i = 0; i < V; ++i) {
        degreeCount[i] = adj[i].size();
    }

    std::vector<int> oddVertices;
    for (const auto& vertex : degreeCount) {
        if (vertex.second % 2 != 0) { // If degree is odd
            oddVertices.push_back(vertex.first);
        }
    }
    return oddVertices;
}

void Graph::prepareMWPMInput(const std::vector<int>& oddVertices, const std::string& mwpmInputPath, std::unordered_map<int, int>& vertexMap) {
    std::ofstream mwpmInputFile(mwpmInputPath);
    std::unordered_set<int> oddVerticesSet(oddVertices.begin(), oddVertices.end());
    std::vector<std::tuple<float, int, int>> filteredEdges;

    for (const auto& edge : edges) {
        if (oddVerticesSet.count(std::get<1>(edge)) && oddVerticesSet.count(std::get<2>(edge))) {
            filteredEdges.emplace_back(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
        }
    }

    mwpmInputFile << oddVertices.size() << " " << filteredEdges.size() << "\n";
    for (const auto& edge : filteredEdges) {
        mwpmInputFile << vertexMap[std::get<1>(edge)] << " " << vertexMap[std::get<2>(edge)] << " " << std::get<0>(edge) << "\n";
    }
}

void Graph::saveGraphToFile(const std::string& filePath) {
    std::ofstream outFile(filePath);
    outFile << V << " " << E << "\n";
    for (const auto& edge : edges) {
        outFile << std::get<1>(edge) << " " << std::get<2>(edge) << " " << std::get<0>(edge) << "\n";
    }
}

Graph create_graph(const std::string& inputFile) {
    std::ifstream inFile(inputFile);
    std::string line;
    if (!inFile) {
        std::cerr << "Cannot open graph file." << std::endl;
        exit(101);
    }

    // Read the first line to determine the format
    std::getline(inFile, line);
    std::istringstream iss(line);
    int V, E;

    if (iss >> V >> E) {  // Reading the format with specified vertices and edges
        Graph g(V, E);
        int u, v;
        float w;
        for (int i = 0; i < E; ++i) {
            if (!std::getline(inFile, line)) { // Ensures each edge data is on a new line
                std::cerr << "Error reading edge data, line " << i + 1 << "." << std::endl;
                exit(102);
            }
            std::istringstream edgeStream(line);
            if (!(edgeStream >> u >> v >> w)) {
                std::cerr << "Error processing edge data on line " << i + 1 << "." << std::endl;
                exit(102);
            }
            g.addEdge(u, v, w);
        }
        return g;
    }
    else {  // Reading the format with only vertices and their coordinates
        Graph g(V, V * (V - 1) / 2);  // Assuming a complete graph
        std::vector<std::pair<float, float>> vertices(V);
        float x, y;
        for (int i = 0; i < V; ++i) {
            if (!std::getline(inFile, line)) {
                std::cerr << "Error reading coordinate data on line " << i + 1 << "." << std::endl;
                exit(102);
            }
            std::istringstream coordStream(line);
            if (!(coordStream >> x >> y)) {
                std::cerr << "Error processing coordinates on line " << i + 1 << "." << std::endl;
                exit(102);
            }
            vertices[i] = { x, y };
        }

        // Calculate distances and add edges
        for (int i = 0; i < V; ++i) {
            for (int j = i + 1; j < V; ++j) {
                float dist = std::sqrt(std::pow(vertices[j].first - vertices[i].first, 2) + std::pow(vertices[j].second - vertices[i].second, 2));
                g.addEdge(i, j, dist);
            }
        }
        return g;
    }
}