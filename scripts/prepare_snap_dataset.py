from collections import defaultdict, deque

input_file = "datasets/CA-GrQc.txt"

output_undirected = "datasets/CA-GrQc_undirected_weighted.txt"
output_directed = "datasets/CA-GrQc_directed_weighted.txt"
output_connected_undirected = "datasets/CA-GrQc_connected_undirected_weighted.txt"
output_connected_directed = "datasets/CA-GrQc_connected_directed_weighted.txt"

edges = set()
nodes = set()

# Wczytanie oryginalnego datasetu SNAP
with open(input_file, "r") as file:
    for line in file:
        if line.startswith("#"):
            continue

        parts = line.split()

        if len(parts) < 2:
            continue

        u = int(parts[0])
        v = int(parts[1])

        if u == v:
            continue

        a = min(u, v)
        b = max(u, v)

        edges.add((a, b))
        nodes.add(a)
        nodes.add(b)

# Numerowanie wierzcholkow od 0
node_list = sorted(nodes)
node_id = {}

for i, node in enumerate(node_list):
    node_id[node] = i

# Krawedzie po zmianie numeracji
renumbered_edges = []

for u, v in edges:
    new_u = node_id[u]
    new_v = node_id[v]
    renumbered_edges.append((new_u, new_v))

vertices_count = len(node_list)

# Funkcja deterministycznie przypisujaca wage
def get_weight(u, v):
    return ((u + 1) * 31 + (v + 1) * 17) % 100 + 1

# Zapis grafu nieskierowanego z wagami
with open(output_undirected, "w") as file:
    file.write(str(vertices_count) + " " + str(len(renumbered_edges)) + "\n")

    for u, v in renumbered_edges:
        weight = get_weight(u, v)
        file.write(str(u) + " " + str(v) + " " + str(weight) + "\n")

# Zapis grafu skierowanego z wagami
with open(output_directed, "w") as file:
    file.write(str(vertices_count) + " " + str(len(renumbered_edges) * 2) + "\n")

    for u, v in renumbered_edges:
        weight = get_weight(u, v)
        file.write(str(u) + " " + str(v) + " " + str(weight) + "\n")
        file.write(str(v) + " " + str(u) + " " + str(weight) + "\n")

# Szukanie najwiekszej spojnej skladowej
adjacency = defaultdict(list)

for u, v in renumbered_edges:
    adjacency[u].append(v)
    adjacency[v].append(u)

visited = set()
largest_component = []

for node in range(vertices_count):
    if node in visited:
        continue

    component = []
    queue = deque([node])
    visited.add(node)

    while queue:
        current = queue.popleft()
        component.append(current)

        for neighbor in adjacency[current]:
            if neighbor not in visited:
                visited.add(neighbor)
                queue.append(neighbor)

    if len(component) > len(largest_component):
        largest_component = component

largest_component_set = set(largest_component)

component_edges = []

for u, v in renumbered_edges:
    if u in largest_component_set and v in largest_component_set:
        component_edges.append((u, v))

# Nowe numerowanie tylko dla najwiekszej spojnej skladowej
component_nodes = sorted(largest_component_set)
component_id = {}

for i, node in enumerate(component_nodes):
    component_id[node] = i

connected_edges = []

for u, v in component_edges:
    new_u = component_id[u]
    new_v = component_id[v]
    connected_edges.append((new_u, new_v))

connected_vertices = len(component_nodes)

# Zapis spojnego grafu nieskierowanego z wagami
with open(output_connected_undirected, "w") as file:
    file.write(str(connected_vertices) + " " + str(len(connected_edges)) + "\n")

    for u, v in connected_edges:
        weight = get_weight(u, v)
        file.write(str(u) + " " + str(v) + " " + str(weight) + "\n")

# Zapis spojnego grafu skierowanego z wagami
with open(output_connected_directed, "w") as file:
    file.write(str(connected_vertices) + " " + str(len(connected_edges) * 2) + "\n")

    for u, v in connected_edges:
        weight = get_weight(u, v)
        file.write(str(u) + " " + str(v) + " " + str(weight) + "\n")
        file.write(str(v) + " " + str(u) + " " + str(weight) + "\n")

print("Prepared dataset files:")
print(output_undirected)
print(output_directed)
print(output_connected_undirected)
print(output_connected_directed)

print()
print("Full graph:")
print("Vertices:", vertices_count)
print("Undirected edges:", len(renumbered_edges))
print("Directed edges:", len(renumbered_edges) * 2)

print()
print("Largest connected component:")
print("Vertices:", connected_vertices)
print("Undirected edges:", len(connected_edges))
print("Directed edges:", len(connected_edges) * 2)