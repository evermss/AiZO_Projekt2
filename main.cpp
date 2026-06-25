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
#include "GraphGenerator.h"
#include "Timer.h"
#include <chrono>
#include <ctime>
#include <iomanip>

class NullBuffer : public std::streambuf {
protected:
    int_type overflow(int_type c) override {
        return traits_type::not_eof(c);
    }
};

void showHelp() {
    std::cout << "AiZO Projekt 2 - grafy\n";
    std::cout << "Standardowe tryby programu z biblioteki parametrow:\n";
    std::cout << " - --help       -> pomoc\n";
    std::cout << " - --singleFile -> pojedynczy test\n";
    std::cout << " - --benchmark  -> badania\n";
    std::cout << "\n";
    std::cout << "Dodatkowe tryby demonstracyjne projektu:\n";
    std::cout << " - --snap            -> porownanie wyniku MST z NetworkX na datasecie SNAP\n";
    std::cout << " - --snapAlgorithms  -> pomiar czasow algorytmow na datasecie SNAP\n";
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

    unsigned int seed =
        123456789u
        + static_cast<unsigned int>(vertices) * 97u
        + static_cast<unsigned int>(density) * 31u
        + static_cast<unsigned int>(problem) * 13u;

    auto nextRandom = [&seed]() {
        seed = seed * 1664525u + 1013904223u;
        return seed;
    };

    auto randomWeight = [&nextRandom]() {
        return 1 + static_cast<int>(nextRandom() % 99u);
    };

    auto addEdgeToBoth = [&](
        int from,
        int to,
        int weight
    ) {
        if (from < 0 || from >= vertices || to < 0 || to >= vertices) {
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

        if (mstProblem) {
            if (from > to) {
                int temp = from;
                from = to;
                to = temp;
            }
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

    GraphGenerator::generateMatrixGraph(matrixGraph, vertices, density);
    GraphGenerator::generateListGraph(listGraph, vertices, density);

    const int TESTS = iterations;

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

    auto measureAndSave = [&](
    const std::string& algorithmName,
    const std::string& structureName,
    auto algorithmCall
) {
        anyTestExecuted = true;

        long long sum = 0;
        long long minTime = 0;
        long long maxTime = 0;

        NullBuffer nullBuffer;
        std::streambuf* originalBuffer = std::cout.rdbuf(&nullBuffer);

        for (int i = 0; i < TESTS; i++) {
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

        std::cout.rdbuf(originalBuffer);

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
            sum / TESTS
        );
    };

    // MST: Prim
    if (
        shouldRunProblem(0) &&
        shouldRunAlgorithm(1) &&
        shouldRunStructure(1)
    ) {
        measureAndSave("Prim", "matrix", [&]() {
            Prim::runMatrix(matrixGraph);
        });
    }

    if (
        shouldRunProblem(0) &&
        shouldRunAlgorithm(1) &&
        shouldRunStructure(2)
    ) {
        measureAndSave("Prim", "list", [&]() {
            Prim::runList(listGraph);
        });
    }

    // MST: Kruskal
    if (
        shouldRunProblem(0) &&
        shouldRunAlgorithm(2) &&
        shouldRunStructure(1)
    ) {
        measureAndSave("Kruskal", "matrix", [&]() {
            Kruskal::runMatrix(matrixGraph);
        });
    }

    if (
        shouldRunProblem(0) &&
        shouldRunAlgorithm(2) &&
        shouldRunStructure(2)
    ) {
        measureAndSave("Kruskal", "list", [&]() {
            Kruskal::runList(listGraph);
        });
    }

    // SP: Dijkstra
    if (
        shouldRunProblem(1) &&
        shouldRunAlgorithm(3) &&
        shouldRunStructure(1)
    ) {
        measureAndSave("Dijkstra", "matrix", [&]() {
            Dijkstra::runMatrix(matrixGraph, startVertex);
        });
    }

    if (
        shouldRunProblem(1) &&
        shouldRunAlgorithm(3) &&
        shouldRunStructure(2)
    ) {
        measureAndSave("Dijkstra", "list", [&]() {
            Dijkstra::runList(listGraph, startVertex);
        });
    }

    // SP: Bellman-Ford
    if (
        shouldRunProblem(1) &&
        shouldRunAlgorithm(4) &&
        shouldRunStructure(1)
    ) {
        measureAndSave("BellmanFord", "matrix", [&]() {
            BellmanFord::runMatrix(matrixGraph, startVertex);
        });
    }

    if (
        shouldRunProblem(1) &&
        shouldRunAlgorithm(4) &&
        shouldRunStructure(2)
    ) {
        measureAndSave("BellmanFord", "list", [&]() {
            BellmanFord::runList(listGraph, startVertex);
        });
    }

    // MF: Ford-Fulkerson
    if (
        shouldRunProblem(2) &&
        shouldRunAlgorithm(5) &&
        shouldRunStructure(1)
    ) {
        measureAndSave("FordFulkerson", "matrix", [&]() {
            FordFulkerson::runMatrix(matrixGraph, startVertex, endVertex);
        });
    }

    if (
        shouldRunProblem(2) &&
        shouldRunAlgorithm(5) &&
        shouldRunStructure(2)
    ) {
        measureAndSave("FordFulkerson", "list", [&]() {
            FordFulkerson::runList(listGraph, startVertex, endVertex);
        });
    }

    if (!anyTestExecuted) {
        std::cout
            << "Blad: wybrany algorytm nie pasuje do wybranego problemu.\n";
    }
}
void runBenchmark() {
    // Parametry odczytane przez bibliotekę prowadzącego.
    const std::string resultsFile = Parameters::resultsFile;
    const int vertices = Parameters::vertexCount;
    const int density = Parameters::density;
    const int iterations = Parameters::iterations;

    const int problem = static_cast<int>(Parameters::problem);
    const int algorithm = static_cast<int>(Parameters::algorithm);
    const int structure = static_cast<int>(Parameters::structure);

    // Sprawdzenie, czy użytkownik podał plik wynikowy.
    if (resultsFile.empty()) {
        std::cout
            << "Blad: nie podano pliku wynikowego --resultsFile.\n";
        return;
    }

    // Graf musi posiadać co najmniej dwa wierzchołki.
    if (vertices < 2) {
        std::cout
            << "Blad: liczba wierzcholkow musi byc co najmniej 2.\n";
        return;
    }

    // W badaniach używane są gęstości, np. 25, 50, 75 i 99%.
    if (density < 1 || density > 99) {
        std::cout
            << "Blad: gestosc musi nalezec do zakresu 1-99.\n";
        return;
    }

    // Każdy przypadek musi być wykonany co najmniej raz.
    if (iterations < 1) {
        std::cout
            << "Blad: liczba powtorzen musi byc dodatnia.\n";
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
    std::cout << "NetworkX MST weight: 151373 "
          << "(wartosc uzyskana w skrypcie scripts/compare_networkx.py)\n";

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