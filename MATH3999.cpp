#include "algorithm.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " algorithm_number inputFilePath outputFilePath" << std::endl;
        return 1;
    }

    std::string algorithmNumber = argv[1];
    std::string inputFilePath = argv[2];
    std::string outputFilePath = argv[3];

    if (algorithmNumber == "1") {
        runAlgorithm1(inputFilePath, outputFilePath);
    }
    else if (algorithmNumber == "2") {
        runAlgorithm2(inputFilePath, outputFilePath);
    }
    else if (algorithmNumber == "3") {
        runAlgorithm3(inputFilePath, outputFilePath);
    }
    else if (algorithmNumber == "4") {
        runAlgorithm4(inputFilePath, outputFilePath);
    }
    else if (algorithmNumber == "5") {
        runAlgorithm5(inputFilePath, outputFilePath);
    }
    else if (algorithmNumber == "6") {
        runAlgorithm6(inputFilePath, outputFilePath);
    }
    else if (algorithmNumber == "7") {
        runAlgorithm7(inputFilePath, outputFilePath);
    }

    return 0;
}