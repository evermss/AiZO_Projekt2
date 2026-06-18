import csv
import os
import matplotlib.pyplot as plt

os.makedirs("results/charts", exist_ok=True)

algorithms = []
times = []

with open("results/snap_algorithms.csv", "r") as file:
    reader = csv.DictReader(file)

    for row in reader:
        algorithms.append(row["algorithm"])
        times.append(int(row["time_us"]))

print("Loaded data:")
for algorithm, time in zip(algorithms, times):
    print(algorithm, time)

plt.figure(figsize=(10, 6))
plt.bar(algorithms, times)

plt.xlabel("Algorytm")
plt.ylabel("Czas wykonania [us]")
plt.title("Czas działania algorytmów na datasecie CA-GrQc")
plt.yscale("log")
plt.grid(axis="y")
plt.tight_layout()

plt.savefig("results/charts/snap_algorithms_time.png", dpi=200)
plt.close()

print("Chart saved in results/charts/snap_algorithms_time.png")