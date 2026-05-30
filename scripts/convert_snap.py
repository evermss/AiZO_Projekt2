input_file = "datasets/CA-GrQc.txt"
output_file = "datasets/CA-GrQc-converted.txt"

node_map = {}
edges = []

def get_new_id(old_id):
    if old_id not in node_map:
        node_map[old_id] = len(node_map)
    return node_map[old_id]

with open(input_file, "r") as file:
    for line in file:
        line = line.strip()

        if line == "" or line.startswith("#"):
            continue

        parts = line.split()

        old_start = int(parts[0])
        old_end = int(parts[1])

        start = get_new_id(old_start)
        end = get_new_id(old_end)

        weight = (len(edges) % 100) + 1

        edges.append((start, end, weight))

with open(output_file, "w") as file:
    file.write(str(len(edges)) + " " + str(len(node_map)) + "\n")

    for start, end, weight in edges:
        file.write(str(start) + " " + str(end) + " " + str(weight) + "\n")

print("Zapisano:", output_file)
print("Wierzcholki:", len(node_map))
print("Krawedzie:", len(edges))