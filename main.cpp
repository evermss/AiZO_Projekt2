#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Parameters.h"
#include "MatrixGraph.h"
#include "ListGraph.h"
#include "FileLoader.h"
#include "Prim.h"
#include "Kruskal.h"
#include "Dijkstra.h"
#include "BellmanFord.h"
#include "FordFulkerson.h"
#include "Timer.h"
#include "GraphGenerator.h"
#include <chrono>

void showHelp() {
    std::cout << "AiZO Projekt 2 - grafy\n";
    std::cout << "Tryby programu:\n";
    std::cout << " - help       -> pomoc\n";
    std::cout << " - singleFile -> pojedynczy test\n";
    std::cout << " - benchmark  -> badania\n";
    std::cout << " - snap       -> porownanie z datasetem SNAP\n";
    std::cout << " - snapAlgorithms -> czasy algorytmow na datasecie SNAP\n";
}

void runSingleTest() {
    std::string inputFile = Parameters::inputFile.empty()
        ? "data/graph.txt"
        : Parameters::inputFile;

    std::ifstream file(inputFile);
    if (!file.is_open()) {
        std::cout << "Blad: nie mozna otworzyc pliku wejściowego: " << inputFile << std::endl;
        return;
    }

    int vertices = 0;
    int edges = 0;
    file >> vertices >> edges;

    int maxVertexId = -1;
    int from = 0;
    int to = 0;
    int weight = 0;

    while (file >> from >> to >> weight) {
        if (from > maxVertexId) {
            maxVertexId = from;
        }
        if (to > maxVertexId) {
            maxVertexId = to;
        }
    }

    file.close();

    if (maxVertexId + 1 > vertices) {
        vertices = maxVertexId + 1;
    }

    MatrixGraph matrixGraph(vertices);
    ListGraph listGraph(vertices);

    FileLoader::loadMatrixGraph(inputFile, matrixGraph);
    FileLoader::loadListGraph(inputFile, listGraph);

    std::ofstream outputFile;
    bool saveToFile = !Parameters::outputFile.empty();

    if (saveToFile) {
        outputFile.open(Parameters::outputFile);

        if (!outputFile.is_open()) {
            std::cout << "Blad: nie mozna otworzyc pliku wyjsciowego: "
                      << Parameters::outputFile << std::endl;
            return;
        }
    }

    auto writeLine = [&](const std::string& text) {
        std::cout << text << "\n";

        if (saveToFile) {
            outputFile << text << "\n";
        }
    };

    auto runAndSaveOutput = [&](const std::string& title, auto algorithmCall) {
        writeLine("");
        writeLine("========================================");
        writeLine(title);
        writeLine("========================================");

        std::streambuf* originalBuffer = std::cout.rdbuf();
        std::ostringstream capturedOutput;
        std::cout.rdbuf(capturedOutput.rdbuf());

        algorithmCall();

        std::cout.rdbuf(originalBuffer);

        std::string result = capturedOutput.str();

        std::cout << result;

        if (saveToFile) {
            outputFile << result;
        }
    };

    writeLine("Tryb pojedynczego testu");
    writeLine("Plik wejsciowy: " + inputFile);
    writeLine("Liczba wierzcholkow: " + std::to_string(vertices));
    writeLine("Liczba krawedzi z pliku: " + std::to_string(edges));

    if (Parameters::inputFile.empty()) {
        runAndSaveOutput("Reprezentacja: macierz incydencji", [&]() {
            matrixGraph.print();
        });

        runAndSaveOutput("Reprezentacja: lista sasiedztwa", [&]() {
            listGraph.print();
        });
    }

    int selectedProblem = static_cast<int>(Parameters::problem);
    int selectedAlgorithm = static_cast<int>(Parameters::algorithm);
    int selectedStructure = static_cast<int>(Parameters::structure);

    int startVertex = Parameters::vertexStart;
    int endVertex = Parameters::vertexEnd;

    if (startVertex < 0 || startVertex >= vertices) {
        startVertex = 0;
    }

    if (endVertex < 0 || endVertex >= vertices) {
        endVertex = vertices - 1;
    }

    int algorithms[5];
    int algorithmsCount = 0;

    if (selectedAlgorithm == 0) { // allAlgorithms
        if (selectedProblem == 0) { // MST
            algorithms[algorithmsCount++] = 1; // Prim
            algorithms[algorithmsCount++] = 2; // Kruskal
        } else if (selectedProblem == 1) { // SP
            algorithms[algorithmsCount++] = 3; // Dijkstra
            algorithms[algorithmsCount++] = 4; // Bellman-Ford
        } else if (selectedProblem == 2) { // MF
            algorithms[algorithmsCount++] = 5; // Ford-Fulkerson
        } else {
            algorithms[algorithmsCount++] = 1;
            algorithms[algorithmsCount++] = 2;
            algorithms[algorithmsCount++] = 3;
            algorithms[algorithmsCount++] = 4;
            algorithms[algorithmsCount++] = 5;
        }
    } else {
        algorithms[algorithmsCount++] = selectedAlgorithm;
    }

    int structures[2];
    int structuresCount = 0;

    if (selectedStructure == 0) { // allStructures
        structures[structuresCount++] = 1; // incidence matrix
        structures[structuresCount++] = 2; // adjacency list
    } else {
        structures[structuresCount++] = selectedStructure;
    }

    for (int j = 0; j < structuresCount; j++) {
        if (structures[j] == 1) {
            runAndSaveOutput("Badana reprezentacja grafu: macierz incydencji", [&]() {
                matrixGraph.print();
            });
        } else if (structures[j] == 2) {
            runAndSaveOutput("Badana reprezentacja grafu: lista sasiedztwa", [&]() {
                listGraph.print();
            });
        }
    }

    for (int i = 0; i < algorithmsCount; i++) {
        for (int j = 0; j < structuresCount; j++) {
            int algorithm = algorithms[i];
            int structure = structures[j];

            if (algorithm == 1) { // Prim
                if (structure == 1) {
                    runAndSaveOutput("Algorytm: Prim | Reprezentacja: macierz incydencji", [&]() {
                        Prim::runMatrix(matrixGraph);
                    });
                } else if (structure == 2) {
                    runAndSaveOutput("Algorytm: Prim | Reprezentacja: lista sasiedztwa", [&]() {
                        Prim::runList(listGraph);
                    });
                }
            } else if (algorithm == 2) { // Kruskal
                if (structure == 1) {
                    runAndSaveOutput("Algorytm: Kruskal | Reprezentacja: macierz incydencji", [&]() {
                        Kruskal::runMatrix(matrixGraph);
                    });
                } else if (structure == 2) {
                    runAndSaveOutput("Algorytm: Kruskal | Reprezentacja: lista sasiedztwa", [&]() {
                        Kruskal::runList(listGraph);
                    });
                }
            } else if (algorithm == 3) { // Dijkstra
                if (structure == 1) {
                    runAndSaveOutput("Algorytm: Dijkstra | Reprezentacja: macierz incydencji", [&]() {
                        Dijkstra::runMatrix(matrixGraph, startVertex);
                    });
                } else if (structure == 2) {
                    runAndSaveOutput("Algorytm: Dijkstra | Reprezentacja: lista sasiedztwa", [&]() {
                        Dijkstra::runList(listGraph, startVertex);
                    });
                }
            } else if (algorithm == 4) { // Bellman-Ford
                if (structure == 1) {
                    runAndSaveOutput("Algorytm: Bellman-Ford | Reprezentacja: macierz incydencji", [&]() {
                        BellmanFord::runMatrix(matrixGraph, startVertex);
                    });
                } else if (structure == 2) {
                    runAndSaveOutput("Algorytm: Bellman-Ford | Reprezentacja: lista sasiedztwa", [&]() {
                        BellmanFord::runList(listGraph, startVertex);
                    });
                }
            } else if (algorithm == 5) { // Ford-Fulkerson
                if (structure == 1) {
                    runAndSaveOutput("Algorytm: Ford-Fulkerson | Reprezentacja: macierz incydencji", [&]() {
                        FordFulkerson::runMatrix(matrixGraph, startVertex, endVertex);
                    });
                } else if (structure == 2) {
                    runAndSaveOutput("Algorytm: Ford-Fulkerson | Reprezentacja: lista sasiedztwa", [&]() {
                        FordFulkerson::runList(listGraph, startVertex, endVertex);
                    });
                }
            }
        }
    }

    if (saveToFile) {
        outputFile.close();
        std::cout << "Wynik zapisano do pliku: " << Parameters::outputFile << "\n";
    }
}

void saveResult(
    std::ofstream& file,
    const std::string& testName,
    const std::string& algorithm,
    const std::string& structure,
    int vertices,
    int density,
    long long minTime,
    long long maxTime,
    long long avgTime
) {
    file << testName << ","
         << algorithm << ","
         << structure << ","
         << vertices << ","
         << density << ","
         << minTime << ","
         << maxTime << ","
         << avgTime
         << "\n";
}

void runBenchmarkForGraph(
    std::ofstream& file,
    const std::string& testName,
    int vertices,
    int density
) {
    MatrixGraph matrixGraph(vertices);
    ListGraph listGraph(vertices);

    GraphGenerator::generateMatrixGraph(matrixGraph, vertices, density);
    GraphGenerator::generateListGraph(listGraph, vertices, density);

    const int TESTS = 50;

    Timer timer;

    std::streambuf* originalBuffer = std::cout.rdbuf();
    std::ostringstream hiddenOutput;
    std::cout.rdbuf(hiddenOutput.rdbuf());

    long long sum;
    long long minTime;
    long long maxTime;
    long long time;

    sum = 0;
    minTime = 999999999;
    maxTime = 0;

    for (int i = 0; i < TESTS; i++) {
        timer.start();
        Prim::runMatrix(matrixGraph);
        time = timer.stop();

        sum += time;

        if (time < minTime) {
            minTime = time;
        }

        if (time > maxTime) {
            maxTime = time;
        }
    }

    saveResult(file, testName, "Prim", "matrix", vertices, density, minTime, maxTime, sum / TESTS);

    sum = 0;
    minTime = 999999999;
    maxTime = 0;

    for (int i = 0; i < TESTS; i++) {
        timer.start();
        Prim::runList(listGraph);
        time = timer.stop();

        sum += time;

        if (time < minTime) {
            minTime = time;
        }

        if (time > maxTime) {
            maxTime = time;
        }
    }

    saveResult(file, testName, "Prim", "list", vertices, density, minTime, maxTime, sum / TESTS);

    sum = 0;
    minTime = 999999999;
    maxTime = 0;

    for (int i = 0; i < TESTS; i++) {
        timer.start();
        Kruskal::runMatrix(matrixGraph);
        time = timer.stop();

        sum += time;

        if (time < minTime) {
            minTime = time;
        }

        if (time > maxTime) {
            maxTime = time;
        }
    }

    saveResult(file, testName, "Kruskal", "matrix", vertices, density, minTime, maxTime, sum / TESTS);

    sum = 0;
    minTime = 999999999;
    maxTime = 0;

    for (int i = 0; i < TESTS; i++) {
        timer.start();
        Kruskal::runList(listGraph);
        time = timer.stop();

        sum += time;

        if (time < minTime) {
            minTime = time;
        }

        if (time > maxTime) {
            maxTime = time;
        }
    }

    saveResult(file, testName, "Kruskal", "list", vertices, density, minTime, maxTime, sum / TESTS);

    sum = 0;
    minTime = 999999999;
    maxTime = 0;

    for (int i = 0; i < TESTS; i++) {
        timer.start();
        Dijkstra::runMatrix(matrixGraph, 0);
        time = timer.stop();

        sum += time;

        if (time < minTime) {
            minTime = time;
        }

        if (time > maxTime) {
            maxTime = time;
        }
    }

    saveResult(file, testName, "Dijkstra", "matrix", vertices, density, minTime, maxTime, sum / TESTS);

    sum = 0;
    minTime = 999999999;
    maxTime = 0;

    for (int i = 0; i < TESTS; i++) {
        timer.start();
        Dijkstra::runList(listGraph, 0);
        time = timer.stop();

        sum += time;

        if (time < minTime) {
            minTime = time;
        }

        if (time > maxTime) {
            maxTime = time;
        }
    }

    saveResult(file, testName, "Dijkstra", "list", vertices, density, minTime, maxTime, sum / TESTS);

    sum = 0;
    minTime = 999999999;
    maxTime = 0;

    for (int i = 0; i < TESTS; i++) {
        timer.start();
        BellmanFord::runMatrix(matrixGraph, 0);
        time = timer.stop();

        sum += time;

        if (time < minTime) {
            minTime = time;
        }

        if (time > maxTime) {
            maxTime = time;
        }
    }

    saveResult(file, testName, "BellmanFord", "matrix", vertices, density, minTime, maxTime, sum / TESTS);

    sum = 0;
    minTime = 999999999;
    maxTime = 0;

    for (int i = 0; i < TESTS; i++) {
        timer.start();
        BellmanFord::runList(listGraph, 0);
        time = timer.stop();

        sum += time;

        if (time < minTime) {
            minTime = time;
        }

        if (time > maxTime) {
            maxTime = time;
        }
    }

    saveResult(file, testName, "BellmanFord", "list", vertices, density, minTime, maxTime, sum / TESTS);

    sum = 0;
    minTime = 999999999;
    maxTime = 0;

    for (int i = 0; i < TESTS; i++) {
        timer.start();
        FordFulkerson::runMatrix(matrixGraph, 0, vertices - 1);
        time = timer.stop();

        sum += time;

        if (time < minTime) {
            minTime = time;
        }

        if (time > maxTime) {
            maxTime = time;
        }
    }

    saveResult(file, testName, "FordFulkerson", "matrix", vertices, density, minTime, maxTime, sum / TESTS);

    sum = 0;
    minTime = 999999999;
    maxTime = 0;

    for (int i = 0; i < TESTS; i++) {
        timer.start();
        FordFulkerson::runList(listGraph, 0, vertices - 1);
        time = timer.stop();

        sum += time;

        if (time < minTime) {
            minTime = time;
        }

        if (time > maxTime) {
            maxTime = time;
        }
    }

    saveResult(file, testName, "FordFulkerson", "list", vertices, density, minTime, maxTime, sum / TESTS);

    std::cout.rdbuf(originalBuffer);
}

void runBenchmark() {
    std::ofstream file("results/benchmark.csv");

    file << "test,algorithm,structure,vertices,density,min_us,max_us,avg_us\n";

    int sizes[] = {20, 50, 100, 150, 200};
    int densities[] = {25, 50, 75, 99};

    for (int i = 0; i < 5; i++) {
        int vertices = sizes[i];

        std::cout << "Badanie A: rozmiar "
                  << vertices
                  << ", gestosc 50%\n";

        runBenchmarkForGraph(file, "A", vertices, 50);
    }

    for (int i = 0; i < 4; i++) {
        int density = densities[i];

        std::cout << "Badanie B: rozmiar 100, gestosc "
                  << density
                  << "%\n";

        runBenchmarkForGraph(file, "B", 100, density);
    }

    file.close();

    std::cout << "Wyniki zapisano do results/benchmark.csv\n";
}

void runSnapComparison() {
    const std::string filename = "datasets/CA-GrQc-converted.txt";

    const int vertices = 5242;
    const int edges = 28980;

    ListGraph graph(vertices);

    FileLoader::loadListGraph(filename, graph);

    std::cout << "SNAP CA-GrQc comparison\n";
    std::cout << "Vertices: " << vertices << "\n";
    std::cout << "Edges: " << edges << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    int cppMstWeight = Kruskal::getMstWeightList(graph);

    auto end = std::chrono::high_resolution_clock::now();

    long long timeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "C++ Kruskal MST weight: " << cppMstWeight << "\n";
    std::cout << "C++ Kruskal time [us]: " << timeUs << "\n";
    std::cout << "NetworkX MST weight: 151373\n";

    if (cppMstWeight == 151373) {
        std::cout << "Result: OK - wyniki sa zgodne\n";
    } else {
        std::cout << "Result: UWAGA - wyniki sa rozne\n";
    }
}

void runSnapAlgorithmsBenchmark() {
    const std::string filename = "datasets/CA-GrQc_connected_directed_weighted.txt";
    const int vertices = 4158;

    ListGraph graph(vertices);

    FileLoader::loadListGraph(filename, graph);

    std::ofstream file("results/snap_algorithms.csv");

    file << "algorithm,time_us\n";
    file.flush();

    Timer timer;
    long long time;

    std::cout << "Start Prim...\n";
    {
        std::streambuf* originalBuffer = std::cout.rdbuf();
        std::ostringstream hiddenOutput;
        std::cout.rdbuf(hiddenOutput.rdbuf());

        timer.start();
        Prim::runList(graph);
        time = timer.stop();

        std::cout.rdbuf(originalBuffer);
    }
    file << "Prim," << time << "\n";
    file.flush();
    std::cout << "Prim OK\n";

    std::cout << "Start Kruskal...\n";
    timer.start();
    Kruskal::getMstWeightList(graph);
    time = timer.stop();
    file << "Kruskal," << time << "\n";
    file.flush();
    std::cout << "Kruskal OK\n";

    std::cout << "Start Dijkstra...\n";
    {
        std::streambuf* originalBuffer = std::cout.rdbuf();
        std::ostringstream hiddenOutput;
        std::cout.rdbuf(hiddenOutput.rdbuf());

        timer.start();
        Dijkstra::runList(graph, 0);
        time = timer.stop();

        std::cout.rdbuf(originalBuffer);
    }
    file << "Dijkstra," << time << "\n";
    file.flush();
    std::cout << "Dijkstra OK\n";

    std::cout << "Start Bellman-Ford...\n";
    {
        std::streambuf* originalBuffer = std::cout.rdbuf();
        std::ostringstream hiddenOutput;
        std::cout.rdbuf(hiddenOutput.rdbuf());

        timer.start();
        BellmanFord::runList(graph, 0);
        time = timer.stop();

        std::cout.rdbuf(originalBuffer);
    }
    file << "Bellman-Ford," << time << "\n";
    file.flush();
    std::cout << "Bellman-Ford OK\n";

    file.close();

    std::cout << "Wyniki zapisano do results/snap_algorithms.csv\n";
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        showHelp();
        runSingleTest();
        return 0;
    }

    std::string mode = argv[1];

    // Dodatkowe tryby projektu, których nie ma w bibliotece parametrów
    if (mode == "snap" || mode == "--snap") {
        runSnapComparison();
        return 0;
    }

    if (mode == "snapAlgorithms" || mode == "--snapAlgorithms") {
        runSnapAlgorithmsBenchmark();
        return 0;
    }

    // Obsługa standardowych parametrów przez bibliotekę prowadzącego
    int parametersResult = Parameters::readParameters(argc - 1, argv + 1);

    if (parametersResult != 0) {
        std::cout << "Blad odczytu parametrow.\n";
        showHelp();
        return parametersResult;
    }

    if (Parameters::runMode == Parameters::RunModes::help) {
        Parameters::help();
        showHelp();
        return 0;
    }

    if (Parameters::runMode == Parameters::RunModes::singleFile) {
        runSingleTest();
        return 0;
    }

    if (Parameters::runMode == Parameters::RunModes::benchmark) {
        runBenchmark();
        return 0;
    }

    std::cout << "Nieznany tryb programu.\n";
    showHelp();

    return 0;
}