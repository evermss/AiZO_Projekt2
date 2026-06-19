import csv
import os
import matplotlib.pyplot as plt

os.makedirs("results/charts", exist_ok=True)

data = []

with open("results/benchmark.csv", "r") as file:
    reader = csv.DictReader(file)
    for row in reader:
        data.append(row)

algorithms = ["Prim", "Kruskal", "Dijkstra", "BellmanFord", "FordFulkerson"]

for algorithm in algorithms:
    x_matrix = []
    y_matrix = []
    x_list = []
    y_list = []

    for row in data:
        if row["test"] == "A" and row["algorithm"] == algorithm:
            if row["structure"] == "matrix":
                x_matrix.append(int(row["vertices"]))
                y_matrix.append(int(row["avg_us"]))
            elif row["structure"] == "list":
                x_list.append(int(row["vertices"]))
                y_list.append(int(row["avg_us"]))

    plt.figure(figsize=(9, 6))
    plt.plot(x_matrix, y_matrix, marker="o", label="Macierz", color="blue", linewidth=2)
    plt.plot(x_list, y_list, marker="o", label="Lista", color="pink", linewidth=2)
    plt.xlabel("Liczba wierzcholkow", fontsize=12)
    plt.ylabel("Sredni czas [us] - skala logarytmiczna", fontsize=12)
    plt.yscale("log")
    plt.ylim(bottom=1)
    plt.title(algorithm + " - badanie A", fontsize=14)
    plt.legend(fontsize=11)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("results/charts/" + algorithm + "_badanie_A.png", dpi=200)
    plt.close()

for algorithm in algorithms:
    x_matrix = []
    y_matrix = []
    x_list = []
    y_list = []

    for row in data:
        if row["test"] == "B" and row["algorithm"] == algorithm:
            if row["structure"] == "matrix":
                x_matrix.append(int(row["density"]))
                y_matrix.append(int(row["avg_us"]))
            elif row["structure"] == "list":
                x_list.append(int(row["density"]))
                y_list.append(int(row["avg_us"]))

    plt.figure(figsize=(9, 6))
    plt.plot(x_matrix, y_matrix, marker="o", label="Macierz", color="blue", linewidth=2)
    plt.plot(x_list, y_list, marker="o", label="Lista", color="pink", linewidth=2)
    plt.xlabel("Gestosc grafu [%]", fontsize=12)
    plt.ylabel("Sredni czas [us] - skala logarytmiczna", fontsize=12)
    plt.yscale("log")
    plt.ylim(bottom=10)

    plt.xticks([20, 30, 40, 50, 60, 70, 80, 90, 100])
    plt.xlim(15, 100)

    plt.title(algorithm + " - badanie B", fontsize=14)
    plt.legend(fontsize=11)
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("results/charts/" + algorithm + "_badanie_B.png", dpi=200)
    plt.close()

print("Charts saved in results/charts")