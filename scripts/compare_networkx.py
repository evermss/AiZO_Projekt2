import networkx as nx

input_file = "datasets/CA-GrQc-converted.txt"

graph = nx.Graph()

with open(input_file, "r") as file:
    first_line = file.readline().split()

    vertices = int(first_line[0])
    edges_count = int(first_line[1])

    for line in file:
        parts = line.split()

        if len(parts) < 3:
            continue

        start = int(parts[0])
        end = int(parts[1])
        weight = int(parts[2])

        graph.add_edge(start, end, weight=weight)

mst = nx.minimum_spanning_tree(graph)
mst_weight = mst.size(weight="weight")

result_text = (
        "NetworkX comparison\n"
        + "Vertices: " + str(vertices) + "\n"
        + "Edges: " + str(edges_count) + "\n"
        + "MST weight: " + str(int(mst_weight)) + "\n"
)

print(result_text)

with open("results/networkx_comparison.txt", "w") as result_file:
    result_file.write(result_text)