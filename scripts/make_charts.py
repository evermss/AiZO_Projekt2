import csv
import matplotlib.pyplot as plt
import os

os.makedirs("results/charts", exist_ok=True)

data = []

with open("results/benchmark.csv", "r") as file:
    reader = csv.DictReader(file)

    for row in reader:
        data.append(row)

algorithms = ["Prim", "Kruskal", "Dijkstra", "BellmanFord", "FordFulkerson"]

for algorithm in algorithms:
    x = []
    y_matrix = []
    y_list = []

    for row in data:
        if row["test"] == "A" and row["algorithm"] == algorithm:
            if row["structure"] == "matrix":
                x.append(int(row["vertices"]))
                y_matrix.append(int(row["avg_us"]))

            if row["structure"] == "list":
                y_list.append(int(row["avg_us"]))

    plt.figure()
    plt.plot(x, y_matrix, marker="o", label="matrix")
    plt.plot(x, y_list, marker="o", label="list")
    plt.xlabel("Vertices")
    plt.ylabel("Average time [us]")
    plt.title(algorithm + " - test A")
    plt.legend()
    plt.grid(True)
    plt.savefig("results/charts/" + algorithm + "_test_A.png")
    plt.close()

print("Charts saved in results/charts")