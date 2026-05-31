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

void showHelp() {
    std::cout << "AiZO Projekt 2 - grafy\n";
    std::cout << "Tryby programu:\n";
    std::cout << " - help       -> pomoc\n";
    std::cout << " - singleFile -> pojedynczy test\n";
    std::cout << " - benchmark  -> badania\n";
}

void runSingleTest() {
    MatrixGraph matrixGraph(4);
    ListGraph listGraph(4);

    FileLoader::loadMatrixGraph("data/graph.txt", matrixGraph);
    FileLoader::loadListGraph("data/graph.txt", listGraph);

    std::cout << "\nMacierz grafu:\n";
    matrixGraph.print();

    std::cout << "\nLista grafu:\n";
    listGraph.print();

    Timer timer;

    std::cout << "\nPrim - macierz:\n";
    timer.start();
    Prim::runMatrix(matrixGraph);
    std::cout << "Czas: " << timer.stop() << " us\n";

    std::cout << "\nPrim - lista:\n";
    timer.start();
    Prim::runList(listGraph);
    std::cout << "Czas: " << timer.stop() << " us\n";

    std::cout << "\nKruskal - macierz:\n";
    Kruskal::runMatrix(matrixGraph);

    std::cout << "\nKruskal - lista:\n";
    Kruskal::runList(listGraph);

    std::cout << "\nDijkstra - macierz:\n";
    Dijkstra::runMatrix(matrixGraph, 0);

    std::cout << "\nDijkstra - lista:\n";
    Dijkstra::runList(listGraph, 0);

    std::cout << "\nBellman-Ford - macierz:\n";
    BellmanFord::runMatrix(matrixGraph, 0);

    std::cout << "\nBellman-Ford - lista:\n";
    BellmanFord::runList(listGraph, 0);

    std::cout << "\nFord-Fulkerson - macierz:\n";
    FordFulkerson::runMatrix(matrixGraph, 0, 3);

    std::cout << "\nFord-Fulkerson - lista:\n";
    FordFulkerson::runList(listGraph, 0, 3);
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

    const int TESTS = 3;

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

int main(int argc, char* argv[]) {
    if (argc == 1) {
        showHelp();
        runSingleTest();
        return 0;
    }

    std::string mode = argv[1];

    if (mode == "help" || mode == "--help") {
        showHelp();
        return 0;
    }

    if (mode == "singleFile" || mode == "--singleFile") {
        runSingleTest();
        return 0;
    }

    if (mode == "benchmark" || mode == "--benchmark") {
        runBenchmark();
        return 0;
    }

    std::cout << "Nieznany tryb programu.\n";
    showHelp();

    return 0;
}