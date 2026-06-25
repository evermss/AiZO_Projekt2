#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <limits>

#include "Parameters.h"
#include "MatrixGraph.h"
#include "ListGraph.h"
#include "FileLoader.h"
#include "Edge.h"
#include "Prim.h"
#include "Kruskal.h"
#include "Dijkstra.h"
#include "BellmanFord.h"
#include "FordFulkerson.h"
#include "Timer.h"

void showHelp() {
    std::cout << "AiZO Projekt 2 - grafy\n";
    std::cout << "Standardowe tryby programu z biblioteki parametrow:\n";
    std::cout << " - --help       -> pomoc\n";
    std::cout << " - --singleFile -> pojedynczy test\n";
    std::cout << " - --benchmark  -> badania\n";
    std::cout << "\n";
    std::cout << "Dodatkowe tryby demonstracyjne projektu:\n";
    std::cout << " - --snap            -> porownanie MST na datasecie SNAP\n";
    std::cout << " - --snapAlgorithms  -> pomiar czasow algorytmow na datasecie SNAP\n";
}

bool isAlgorithmCompatible(int problem, int algorithm) {
    if (problem == 0) {
        return algorithm == 1 || algorithm == 2;
    }

    if (problem == 1) {
        return algorithm == 3 || algorithm == 4;
    }

    if (problem == 2) {
        return algorithm == 5;
    }

    return false;
}

std::string getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    std::ostringstream stream;
    stream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

    return stream.str();
}

std::string getProblemName(int problem) {
    if (problem == 0) {
        return "MST";
    }

    if (problem == 1) {
        return "SP";
    }

    if (problem == 2) {
        return "MF";
    }

    return "UNKNOWN";
}

int findSetLocal(int* parent, int vertex) {
    if (parent[vertex] != vertex) {
        parent[vertex] = findSetLocal(parent, parent[vertex]);
    }

    return parent[vertex];
}

void unionSetsLocal(int* parent, int* rank, int first, int second) {
    int firstRoot = findSetLocal(parent, first);
    int secondRoot = findSetLocal(parent, second);

    if (firstRoot == secondRoot) {
        return;
    }

    if (rank[firstRoot] < rank[secondRoot]) {
        parent[firstRoot] = secondRoot;
    } else if (rank[firstRoot] > rank[secondRoot]) {
        parent[secondRoot] = firstRoot;
    } else {
        parent[secondRoot] = firstRoot;
        rank[firstRoot]++;
    }
}

void sortEdgesByWeightLocal(Edge* edges, int edgesCount) {
    for (int gap = edgesCount / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < edgesCount; i++) {
            Edge temp = edges[i];
            int j = i;

            while (j >= gap &&
                   edges[j - gap].getWeight() > temp.getWeight()) {

                edges[j] = edges[j - gap];
                j -= gap;
            }

            edges[j] = temp;
        }
    }
}

template <typename GraphType>
long long calculateKruskalMstWeight(const GraphType& graph) {
    int vertices = graph.getVertices();

    if (vertices <= 0) {
        return 0;
    }

    int edgesCount = 0;

    graph.forEachEdge(
        [&](int start, int end, int weight) {
            (void) start;
            (void) end;
            (void) weight;

            edgesCount++;
        }
    );

    if (edgesCount == 0) {
        return 0;
    }

    Edge* edges = new Edge[edgesCount];
    int currentEdge = 0;

    graph.forEachEdge(
        [&](int start, int end, int weight) {
            edges[currentEdge] = Edge(start, end, weight);
            currentEdge++;
        }
    );

    sortEdgesByWeightLocal(edges, edgesCount);

    int* parent = new int[vertices];
    int* rank = new int[vertices];

    for (int i = 0; i < vertices; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    long long totalWeight = 0;
    int selectedEdges = 0;

    for (int i = 0; i < edgesCount && selectedEdges < vertices - 1; i++) {
        int start = edges[i].getStart();
        int end = edges[i].getEnd();
        int weight = edges[i].getWeight();

        if (start < 0 || start >= vertices ||
            end < 0 || end >= vertices) {

            continue;
        }

        int startRoot = findSetLocal(parent, start);
        int endRoot = findSetLocal(parent, end);

        if (startRoot != endRoot) {
            totalWeight += weight;
            selectedEdges++;

            unionSetsLocal(parent, rank, startRoot, endRoot);
        }
    }

    delete[] edges;
    delete[] parent;
    delete[] rank;

    return totalWeight;
}

void runSingleTest() {
    std::string inputFile = Parameters::inputFile.empty()
        ? "data/graph.txt"
        : Parameters::inputFile;

    std::ifstream file(inputFile);

    if (!file.is_open()) {
        std::cout << "Blad: nie mozna otworzyc pliku wejsciowego: "
                  << inputFile
                  << "\n";
        return;
    }

    int vertices = 0;
    int edges = 0;

    if (!(file >> vertices >> edges)) {
        std::cout << "Blad: niepoprawny format pliku wejsciowego.\n";
        return;
    }

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

    if (vertices <= 0) {
        std::cout << "Blad: graf nie ma wierzcholkow.\n";
        return;
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
                      << Parameters::outputFile
                      << "\n";
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

    int selectedProblem = static_cast<int>(Parameters::problem);
    int selectedAlgorithm = static_cast<int>(Parameters::algorithm);
    int selectedStructure = static_cast<int>(Parameters::structure);

    if (selectedProblem < 0 || selectedProblem > 2) {
        writeLine("Blad: niepoprawny problem. 0 = MST, 1 = SP, 2 = MF.");
        return;
    }

    if (selectedAlgorithm < 0 || selectedAlgorithm > 5) {
        writeLine("Blad: niepoprawny algorytm.");
        return;
    }

    if (selectedStructure < 0 || selectedStructure > 2) {
        writeLine("Blad: niepoprawna struktura.");
        return;
    }

    if (
        selectedAlgorithm != 0 &&
        !isAlgorithmCompatible(selectedProblem, selectedAlgorithm)
    ) {
        writeLine("Blad: wybrany algorytm nie pasuje do wybranego problemu.");
        return;
    }

    int startVertex = Parameters::vertexStart;
    int endVertex = Parameters::vertexEnd;

    if (startVertex < 0 || startVertex >= vertices) {
        startVertex = 0;
    }

    if (endVertex < 0 || endVertex >= vertices) {
        endVertex = vertices - 1;
    }

    if (startVertex == endVertex && vertices > 1) {
        if (endVertex == vertices - 1) {
            endVertex = 0;
        } else {
            endVertex = vertices - 1;
        }
    }

    int algorithms[5];
    int algorithmsCount = 0;

    if (selectedAlgorithm == 0) {
        if (selectedProblem == 0) {
            algorithms[algorithmsCount++] = 1;
            algorithms[algorithmsCount++] = 2;
        } else if (selectedProblem == 1) {
            algorithms[algorithmsCount++] = 3;
            algorithms[algorithmsCount++] = 4;
        } else if (selectedProblem == 2) {
            algorithms[algorithmsCount++] = 5;
        }
    } else {
        algorithms[algorithmsCount++] = selectedAlgorithm;
    }

    int structures[2];
    int structuresCount = 0;

    if (selectedStructure == 0) {
        structures[structuresCount++] = 1;
        structures[structuresCount++] = 2;
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

            if (algorithm == 1) {
                if (structure == 1) {
                    runAndSaveOutput("Algorytm: Prim | Reprezentacja: macierz incydencji", [&]() {
                        Prim::runMatrix(matrixGraph);
                    });
                } else if (structure == 2) {
                    runAndSaveOutput("Algorytm: Prim | Reprezentacja: lista sasiedztwa", [&]() {
                        Prim::runList(listGraph);
                    });
                }
            } else if (algorithm == 2) {
                if (structure == 1) {
                    runAndSaveOutput("Algorytm: Kruskal | Reprezentacja: macierz incydencji", [&]() {
                        Kruskal::runMatrix(matrixGraph);
                    });
                } else if (structure == 2) {
                    runAndSaveOutput("Algorytm: Kruskal | Reprezentacja: lista sasiedztwa", [&]() {
                        Kruskal::runList(listGraph);
                    });
                }
            } else if (algorithm == 3) {
                if (structure == 1) {
                    runAndSaveOutput("Algorytm: Dijkstra | Reprezentacja: macierz incydencji", [&]() {
                        Dijkstra::runMatrix(matrixGraph, startVertex, endVertex);
                    });
                } else if (structure == 2) {
                    runAndSaveOutput("Algorytm: Dijkstra | Reprezentacja: lista sasiedztwa", [&]() {
                        Dijkstra::runList(listGraph, startVertex, endVertex);
                    });
                }
            } else if (algorithm == 4) {
                if (structure == 1) {
                    runAndSaveOutput("Algorytm: Bellman-Ford | Reprezentacja: macierz incydencji", [&]() {
                        BellmanFord::runMatrix(matrixGraph, startVertex, endVertex);
                    });
                } else if (structure == 2) {
                    runAndSaveOutput("Algorytm: Bellman-Ford | Reprezentacja: lista sasiedztwa", [&]() {
                        BellmanFord::runList(listGraph, startVertex, endVertex);
                    });
                }
            } else if (algorithm == 5) {
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
        std::cout << "Wynik zapisano do pliku: "
                  << Parameters::outputFile
                  << "\n";
    }
}

void saveResult(
    std::ofstream& file,
    const std::string& testName,
    int problem,
    const std::string& algorithm,
    const std::string& structure,
    int vertices,
    int edges,
    int density,
    int iterations,
    int startVertex,
    int endVertex,
    long long minTime,
    long long maxTime,
    long long avgTime
) {
    file << getCurrentDateTime() << ","
         << testName << ","
         << getProblemName(problem) << ","
         << algorithm << ","
         << structure << ","
         << vertices << ","
         << edges << ","
         << density << ","
         << iterations << ",";

    if (problem == 0) {
        file << "-1,-1,";
    } else {
        file << startVertex << ","
             << endVertex << ",";
    }

    file << minTime << ","
         << maxTime << ","
         << avgTime
         << "\n";
}

int generateCommonBenchmarkGraph(
    MatrixGraph& matrixGraph,
    ListGraph& listGraph,
    int vertices,
    int density,
    int problem,
    int startVertex,
    int endVertex
) {
    bool** used = new bool*[vertices];

    for (int i = 0; i < vertices; i++) {
        used[i] = new bool[vertices];

        for (int j = 0; j < vertices; j++) {
            used[i][j] = false;
        }
    }

    bool mstProblem = problem == 0;

    long long maxEdges = 0;

    if (mstProblem) {
        maxEdges =
            static_cast<long long>(vertices) *
            static_cast<long long>(vertices - 1) /
            2;
    } else {
        maxEdges =
            static_cast<long long>(vertices) *
            static_cast<long long>(vertices - 1);
    }

    long long targetEdgesLong = maxEdges * density / 100;

    if (targetEdgesLong < vertices - 1) {
        targetEdgesLong = vertices - 1;
    }

    if (targetEdgesLong > maxEdges) {
        targetEdgesLong = maxEdges;
    }

    int targetEdges = static_cast<int>(targetEdgesLong);

    int maxWeight = targetEdges * 4 / 5;

    if (maxWeight < 1) {
        maxWeight = 1;
    }

    unsigned int seed =
        123456789u
        + static_cast<unsigned int>(vertices) * 97u
        + static_cast<unsigned int>(density) * 31u
        + static_cast<unsigned int>(problem) * 13u;

    auto nextRandom = [&seed]() {
        seed = seed * 1664525u + 1013904223u;
        return seed;
    };

    auto randomWeight = [&nextRandom, maxWeight]() {
        return 1 + static_cast<int>(
            nextRandom() % static_cast<unsigned int>(maxWeight)
        );
    };

    auto addEdgeToBoth = [&](
        int from,
        int to,
        int weight
    ) {
        if (from < 0 || from >= vertices ||
            to < 0 || to >= vertices) {

            return false;
        }

        if (from == to) {
            return false;
        }

        if (used[from][to]) {
            return false;
        }

        used[from][to] = true;

        matrixGraph.addEdge(from, to, weight);
        listGraph.addEdge(from, to, weight);

        return true;
    };

    int currentEdges = 0;

    if (mstProblem) {
        for (int i = 0; i < vertices - 1 && currentEdges < targetEdges; i++) {
            int from = i;
            int to = i + 1;

            if (from > to) {
                int temp = from;
                from = to;
                to = temp;
            }

            if (addEdgeToBoth(from, to, randomWeight())) {
                currentEdges++;
            }
        }
    } else {
        if (
            startVertex >= 0 &&
            startVertex < vertices &&
            endVertex >= 0 &&
            endVertex < vertices &&
            startVertex != endVertex &&
            currentEdges < targetEdges
        ) {
            if (addEdgeToBoth(startVertex, endVertex, randomWeight())) {
                currentEdges++;
            }
        }

        for (int i = 0; i < vertices - 1 && currentEdges < targetEdges; i++) {
            if (addEdgeToBoth(i, i + 1, randomWeight())) {
                currentEdges++;
            }
        }
    }

    while (currentEdges < targetEdges) {
        int from = static_cast<int>(
            nextRandom() % static_cast<unsigned int>(vertices)
        );

        int to = static_cast<int>(
            nextRandom() % static_cast<unsigned int>(vertices)
        );

        if (from == to) {
            continue;
        }

        if (mstProblem && from > to) {
            int temp = from;
            from = to;
            to = temp;
        }

        if (addEdgeToBoth(from, to, randomWeight())) {
            currentEdges++;
        }
    }

    for (int i = 0; i < vertices; i++) {
        delete[] used[i];
    }

    delete[] used;

    return currentEdges;
}

void runBenchmarkForGraph(
    std::ofstream& file,
    const std::string& testName,
    int vertices,
    int density,
    int iterations,
    int problem,
    int algorithm,
    int structure
) {
    MatrixGraph matrixGraph(vertices);
    ListGraph listGraph(vertices);

    int startVertex = Parameters::vertexStart;
    int endVertex = Parameters::vertexEnd;

    if (startVertex < 0 || startVertex >= vertices) {
        startVertex = 0;
    }

    if (endVertex < 0 || endVertex >= vertices) {
        endVertex = vertices - 1;
    }

    if (startVertex == endVertex && vertices > 1) {
        if (endVertex == vertices - 1) {
            endVertex = 0;
        } else {
            endVertex = vertices - 1;
        }
    }

    int edgeCount = generateCommonBenchmarkGraph(
        matrixGraph,
        listGraph,
        vertices,
        density,
        problem,
        startVertex,
        endVertex
    );

    auto shouldRunProblem = [problem](int expectedProblem) {
        return problem == expectedProblem;
    };

    auto shouldRunAlgorithm = [algorithm](int expectedAlgorithm) {
        return algorithm == 0 || algorithm == expectedAlgorithm;
    };

    auto shouldRunStructure = [structure](int expectedStructure) {
        return structure == 0 || structure == expectedStructure;
    };

    bool anyTestExecuted = false;

    auto measureAndSave = [&](
        const std::string& algorithmName,
        const std::string& structureName,
        auto algorithmCall
    ) {
        anyTestExecuted = true;

        long long sum = 0;
        long long minTime = 0;
        long long maxTime = 0;

        for (int i = 0; i < iterations; i++) {
            Timer timer;

            timer.start();
            algorithmCall();
            long long time = timer.stop();

            sum += time;

            if (i == 0 || time < minTime) {
                minTime = time;
            }

            if (i == 0 || time > maxTime) {
                maxTime = time;
            }
        }

        saveResult(
            file,
            testName,
            problem,
            algorithmName,
            structureName,
            vertices,
            edgeCount,
            density,
            iterations,
            startVertex,
            endVertex,
            minTime,
            maxTime,
            sum / iterations
        );
    };

    if (
        shouldRunProblem(0) &&
        shouldRunAlgorithm(1) &&
        shouldRunStructure(1)
    ) {
        measureAndSave("Prim", "matrix", [&]() {
            Prim::runMatrix(matrixGraph, false);
        });
    }

    if (
        shouldRunProblem(0) &&
        shouldRunAlgorithm(1) &&
        shouldRunStructure(2)
    ) {
        measureAndSave("Prim", "list", [&]() {
            Prim::runList(listGraph, false);
        });
    }

    if (
        shouldRunProblem(0) &&
        shouldRunAlgorithm(2) &&
        shouldRunStructure(1)
    ) {
        measureAndSave("Kruskal", "matrix", [&]() {
            Kruskal::runMatrix(matrixGraph, false);
        });
    }

    if (
        shouldRunProblem(0) &&
        shouldRunAlgorithm(2) &&
        shouldRunStructure(2)
    ) {
        measureAndSave("Kruskal", "list", [&]() {
            Kruskal::runList(listGraph, false);
        });
    }

    if (
        shouldRunProblem(1) &&
        shouldRunAlgorithm(3) &&
        shouldRunStructure(1)
    ) {
        measureAndSave("Dijkstra", "matrix", [&]() {
            Dijkstra::runMatrix(matrixGraph, startVertex, endVertex, false);
        });
    }

    if (
        shouldRunProblem(1) &&
        shouldRunAlgorithm(3) &&
        shouldRunStructure(2)
    ) {
        measureAndSave("Dijkstra", "list", [&]() {
            Dijkstra::runList(listGraph, startVertex, endVertex, false);
        });
    }

    if (
        shouldRunProblem(1) &&
        shouldRunAlgorithm(4) &&
        shouldRunStructure(1)
    ) {
        measureAndSave("BellmanFord", "matrix", [&]() {
            BellmanFord::runMatrix(matrixGraph, startVertex, endVertex, false);
        });
    }

    if (
        shouldRunProblem(1) &&
        shouldRunAlgorithm(4) &&
        shouldRunStructure(2)
    ) {
        measureAndSave("BellmanFord", "list", [&]() {
            BellmanFord::runList(listGraph, startVertex, endVertex, false);
        });
    }

    if (
        shouldRunProblem(2) &&
        shouldRunAlgorithm(5) &&
        shouldRunStructure(1)
    ) {
        measureAndSave("FordFulkerson", "matrix", [&]() {
            FordFulkerson::runMatrix(matrixGraph, startVertex, endVertex, false);
        });
    }

    if (
        shouldRunProblem(2) &&
        shouldRunAlgorithm(5) &&
        shouldRunStructure(2)
    ) {
        measureAndSave("FordFulkerson", "list", [&]() {
            FordFulkerson::runList(listGraph, startVertex, endVertex, false);
        });
    }

    if (!anyTestExecuted) {
        std::cout
            << "Blad: wybrany algorytm nie pasuje do wybranego problemu.\n";
    }
}

void runBenchmark() {
    const std::string resultsFile = Parameters::resultsFile;
    const int vertices = Parameters::vertexCount;
    const int density = Parameters::density;
    const int iterations = Parameters::iterations;

    const int problem = static_cast<int>(Parameters::problem);
    const int algorithm = static_cast<int>(Parameters::algorithm);
    const int structure = static_cast<int>(Parameters::structure);

    if (resultsFile.empty()) {
        std::cout << "Blad: nie podano pliku wynikowego --resultsFile.\n";
        return;
    }

    if (vertices < 2) {
        std::cout << "Blad: liczba wierzcholkow musi byc co najmniej 2.\n";
        return;
    }

    if (density < 1 || density > 99) {
        std::cout << "Blad: gestosc musi nalezec do zakresu 1-99.\n";
        return;
    }

    if (iterations < 1) {
        std::cout << "Blad: liczba powtorzen musi byc dodatnia.\n";
        return;
    }

    if (problem < 0 || problem > 2) {
        std::cout
            << "Blad: niepoprawny problem. "
            << "0 = MST, 1 = SP, 2 = MF.\n";
        return;
    }

    if (algorithm < 0 || algorithm > 5) {
        std::cout
            << "Blad: niepoprawny algorytm. "
            << "0 = wszystkie dla danego problemu, "
            << "1 = Prim, 2 = Kruskal, "
            << "3 = Dijkstra, 4 = Bellman-Ford, "
            << "5 = Ford-Fulkerson.\n";
        return;
    }

    if (structure < 0 || structure > 2) {
        std::cout
            << "Blad: niepoprawna struktura. "
            << "0 = obie, 1 = macierz, 2 = lista.\n";
        return;
    }

    if (algorithm != 0 && !isAlgorithmCompatible(problem, algorithm)) {
        std::cout << "Blad: wybrany algorytm nie pasuje do problemu.\n";
        return;
    }

    bool writeHeader = true;

    {
        std::ifstream existingFile(
            resultsFile,
            std::ios::binary | std::ios::ate
        );

        if (existingFile.is_open() && existingFile.tellg() > 0) {
            writeHeader = false;
        }
    }

    std::ofstream file(resultsFile, std::ios::app);

    if (!file.is_open()) {
        std::cout
            << "Blad: nie mozna otworzyc pliku wynikowego: "
            << resultsFile
            << "\n";
        return;
    }

    if (writeHeader) {
        file
            << "datetime,test,problem,algorithm,structure,"
            << "vertices,edges,density,iterations,"
            << "start_vertex,end_vertex,"
            << "min_us,max_us,avg_us\n";
    }

    std::cout
        << "Benchmark: wierzcholki=" << vertices
        << ", gestosc=" << density << "%"
        << ", powtorzenia=" << iterations
        << "\n";

    runBenchmarkForGraph(
        file,
        "PARAM",
        vertices,
        density,
        iterations,
        problem,
        algorithm,
        structure
    );

    file.close();

    std::cout
        << "Wyniki zapisano do: "
        << resultsFile
        << "\n";
}

void runSnapComparison() {
    const std::string filename =
        "datasets/CA-GrQc_connected_undirected_weighted.txt";

    const int vertices = 4158;
    const int edges = 13422;

    ListGraph graph(vertices);

    FileLoader::loadListGraph(filename, graph);

    std::cout << "SNAP CA-GrQc comparison\n";
    std::cout << "File: " << filename << "\n";
    std::cout << "Vertices: " << vertices << "\n";
    std::cout << "Edges: " << edges << "\n";

    Timer timer;

    timer.start();
    long long cppMstWeight = calculateKruskalMstWeight(graph);
    long long timeUs = timer.stop();

    std::cout << "C++ Kruskal MST weight: " << cppMstWeight << "\n";
    std::cout << "C++ Kruskal time [us]: " << timeUs << "\n";
    std::cout
        << "Wartosc porownaj z wynikiem skryptu "
        << "scripts/compare_networkx.py dla tego samego pliku.\n";
}

void runSnapAlgorithmsBenchmark() {
    const std::string mstFile =
        "datasets/CA-GrQc_connected_undirected_weighted.txt";

    const std::string directedFile =
        "datasets/CA-GrQc_connected_directed_weighted.txt";

    const int vertices = 4158;
    const int source = 0;
    const int sink = vertices - 1;

    ListGraph mstGraph(vertices);
    ListGraph directedGraph(vertices);

    FileLoader::loadListGraph(mstFile, mstGraph);
    FileLoader::loadListGraph(directedFile, directedGraph);

    std::ofstream file("results/snap_algorithms.csv");

    if (!file.is_open()) {
        std::cout << "Blad: nie mozna otworzyc results/snap_algorithms.csv\n";
        return;
    }

    file << "datetime,algorithm,structure,vertices,source,sink,time_us\n";

    auto measureAndSave = [&](const std::string& algorithmName, auto algorithmCall) {
        Timer timer;

        timer.start();
        algorithmCall();
        long long time = timer.stop();

        file << getCurrentDateTime() << ","
             << algorithmName << ","
             << "list,"
             << vertices << ","
             << source << ","
             << sink << ","
             << time
             << "\n";

        file.flush();

        std::cout << algorithmName
                  << " OK, time [us]: "
                  << time
                  << "\n";
    };

    std::cout << "SNAP algorithms benchmark\n";

    measureAndSave("Prim", [&]() {
        Prim::runList(mstGraph, false);
    });

    measureAndSave("Kruskal", [&]() {
        Kruskal::runList(mstGraph, false);
    });

    measureAndSave("Dijkstra", [&]() {
        Dijkstra::runList(directedGraph, source, sink, false);
    });

    measureAndSave("Bellman-Ford", [&]() {
        BellmanFord::runList(directedGraph, source, sink, false);
    });

    measureAndSave("Ford-Fulkerson", [&]() {
        FordFulkerson::runList(directedGraph, source, sink, false);
    });

    file.close();

    std::cout << "Wyniki zapisano do results/snap_algorithms.csv\n";
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        showHelp();
        return 0;
    }

    std::string mode = argv[1];

    if (mode == "snap" || mode == "--snap") {
        runSnapComparison();
        return 0;
    }

    if (mode == "snapAlgorithms" || mode == "--snapAlgorithms") {
        runSnapAlgorithmsBenchmark();
        return 0;
    }

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