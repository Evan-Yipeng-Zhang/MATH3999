#include "Algorithm.h"
#include <fstream>
#include <iostream>
#include <sstream>

int verticesCount;
std::unordered_map<std::string, int> stringToIndex;
std::unordered_map<int, std::string> indexToString;

int calculateOverlap(const std::string& a, const std::string& b) {
    int maxOverlap = 0;
    for (int i = 1; i <= std::min(a.size(), b.size()); ++i) {
        if (a.substr(a.size() - i) == b.substr(0, i)) {
            maxOverlap = i;
        }
    }
    return maxOverlap;
}

std::vector<std::pair<std::string, std::string>> parseMatchingOutput(const std::string& matchingOutputPath) {
    std::ifstream inFile(matchingOutputPath);
    std::vector<std::pair<std::string, std::string>> matchedPairs;
    std::string line;

    if (!inFile.is_open()) {
        std::cerr << "Failed to open file: " << matchingOutputPath << std::endl;
        return matchedPairs;
    }

    // Skip the first line containing metadata about the number of vertices and edges
    std::getline(inFile, line);

    // Parse the matched pairs from the file
    while (std::getline(inFile, line)) {
        std::istringstream lineStream(line);
        int u, v, w;
        if (lineStream >> u >> v >> w) {
            // Convert integer vertices back to their original string format
            std::string uStr = indexToString.at(u);
            std::string vStr = indexToString.at(v - verticesCount);
            matchedPairs.emplace_back(uStr, vStr);
        }
    }
    return matchedPairs;
}

std::string concatenateFromMatchedPairs(const std::vector<std::pair<std::string, std::string>>& matchedPairs) {
    if (matchedPairs.empty()) return "";

    // Build the sequence map from matched pairs and track unvisited vertices
    std::unordered_map<std::string, std::string> nextInSequence;
    std::unordered_set<std::string> unvisitedVertices;
    for (const auto& pair : matchedPairs) {
        nextInSequence[pair.first] = pair.second;
        unvisitedVertices.insert(pair.first);
    }

    std::string result = "";

    // Iterate until there are no unvisited vertices left
    while (!unvisitedVertices.empty()) {
        // Start with an arbitrary unvisited vertex
        std::string current = *unvisitedVertices.begin();
        std::string cycleStart = current;
        std::string cycle = current; // Initialize cycle with the starting string

        // Follow the cycle
        while (true) {
            unvisitedVertices.erase(current); // Mark current vertex as visited
            const std::string& next = nextInSequence[current];

            if (next == cycleStart) { //  || unvisitedVertices.find(next) == unvisitedVertices.end()
                // Calculate overlap with the start of the cycle to close the cycle properly
                int overlap = calculateOverlap(current, next);
                result += current;
                break; // Break if the cycle is complete or next vertex is already visited
            }

            // Calculate overlap and append the non-overlapping part of the next string
            int overlap = calculateOverlap(current, next);
            result += current.substr(0, current.size() - overlap);

            current = next; // Move to the next vertex in the sequence
        }
    }

    return result;
}

Graph constructDistanceGraph(const std::vector<std::string>& strings) {
    int index = 1; // Start indexing from 1 to reserve 0 for the empty string
    stringToIndex[""] = 0; // Map the empty string to 0
    indexToString[0] = "";

    for (const auto& s : strings) {
        if (stringToIndex.find(s) == stringToIndex.end()) {
            stringToIndex[s] = index;
            indexToString[index] = s;
            index++;
        }
    }

    // Construct the graph using integer vertices
    Graph g(strings.size() + 1, strings.size() * (strings.size() + 1));
    for (const auto& s1 : strings) {
        int from = stringToIndex[s1];
        for (const auto& s2 : strings) {
            if (s1 != s2) {
                int to = stringToIndex[s2];
                float weight = s1.size() - calculateOverlap(s1, s2);
                g.addEdge(from, to, weight);
            }
        }
        // Handle edges to and from the empty string (vertex 0)
        g.addEdge(0, from, 0); // From empty string to s1
        g.addEdge(from, 0, s1.length()); // From s1 to empty string
    }
    return g;
}

std::vector<std::string> readStringsFromFile(const std::string& filePath) {
    std::ifstream inFile(filePath);
    std::vector<std::string> strings;
    std::string line;
    while (std::getline(inFile, line)) {
        // Use a stringstream to trim whitespace and tabs from the line
        std::istringstream iss(line);
        std::string trimmedString;
        iss >> trimmedString; // This reads the first token, ignoring leading whitespace
        if (!trimmedString.empty()) {
            strings.push_back(trimmedString);
        }
    }
    return strings;
}

void saveBipartiteGraphToFile(const Graph& g, const std::string& filePath) {
    std::ofstream outFile(filePath);

    // Output the vertices count (doubled) and edges count to the file
    outFile << g.V * 2 << " " << g.E << std::endl;

    for (const auto& edge : g.edges) {
        // For a bipartite graph constructed from an integer-based graph,
        // we can distinguish the two sets by adding an offset to one set
        int from = std::get<1>(edge); // Offset for set X
        int to = std::get<2>(edge) + g.V; // Offset for set Y, ensuring it's disjoint from X

        outFile << from << " " << to << " " << std::get<0>(edge) << std::endl;
    }
}

void runAlgorithm7(const std::string& inputFile, const std::string& outputFile) {
    std::string basePath = getExecutablePath();

    // Read strings and construct the distance graph
    std::vector<std::string> strings = readStringsFromFile(inputFile);
    Graph g = constructDistanceGraph(strings);
    verticesCount = g.V;
    g.saveGraphToFile(basePath + "\\distance_graph");

    // Convert the graph to a bipartite graph and find the MWPM
    saveBipartiteGraphToFile(g, basePath + "\\bipartite_graph");
    std::string blossomPath = readToolPath();
    std::string cmdMWPM = blossomPath + "/blossom4 -e " + basePath + "\\bipartite_graph -w " + basePath + "\\matching_output";
    system(cmdMWPM.c_str());

    // Parse the MWPM output and concatenate strings
    std::vector<std::pair<std::string, std::string>> matchedPairs = parseMatchingOutput(basePath + "\\matching_output");
    std::string concatenatedString = concatenateFromMatchedPairs(matchedPairs);

    // Write the concatenated (super)string to the output file
    std::ofstream outFile(outputFile);
    outFile << concatenatedString;
    std::cout << "Superstring generated by algorthm 7 successfully." << std::endl;
}