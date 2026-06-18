input_file = "datasets/CA-GrQc.txt"
output_file = "datasets/CA-GrQc-converted.txt"

edges = []
nodes = set()

with open(input_file, "r") as file:
    for line in file:
        if line.startswith("#"):
            continue

        parts = line.split()

        if len(parts) < 2:
            continue

        start = int(parts[0])
        end = int(parts[1])

        nodes.add(start)
        nodes.add(end)

        edges.append((start, end))

node_list = sorted(nodes)
node_id = {}

for i, node in enumerate(node_list):
    node_id[node] = i

with open(output_file, "w") as file:
    file.write(str(len(node_list)) + " " + str(len(edges)) + "\n")

    for start, end in edges:
        new_start = node_id[start]
        new_end = node_id[end]

        weight = (new_start + new_end) % 100 + 1

        file.write(str(new_start) + " " + str(new_end) + " " + str(weight) + "\n")

print("Converted SNAP dataset saved to", output_file)
print("Vertices:", len(node_list))
print("Edges:", len(edges))