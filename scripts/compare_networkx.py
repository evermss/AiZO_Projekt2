import networkx as nx

input_file = "datasets/CA-GrQc-converted.txt"

graph = nx.Graph()

with open(input_file, "r") as file:
    first_line = file.readline().split()
    edges_count = int(first_line[0])
    vertices_count = int(first_line[1])

    for line in file:
        parts = line.split()

        if len(parts) != 3:
            continue

        start = int(parts[0])
        end = int(parts[1])
        weight = int(parts[2])

        graph.add_edge(start, end, weight=weight)

mst = nx.minimum_spanning_tree(graph, algorithm="kruskal")
mst_weight = mst.size(weight="weight")

print("NetworkX comparison")
print("Vertices:", vertices_count)
print("Edges:", edges_count)
print("MST weight:", int(mst_weight))