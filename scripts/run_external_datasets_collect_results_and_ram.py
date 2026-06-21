#!/usr/bin/env python3
"""
Run all converted external datasets and collect two kinds of results:

1) Application benchmark CSV, produced by the C++ program itself through:
       --resultsFile results/external_dataset_program_results/dataset<Name>ProgramResults.csv

   This is the file that contains the program's own finalStatistics rows.

2) Global Python/system CSV:
       results/external_dataset_global_measurements.csv

   This records dataset name, dataset size, algorithm, structure, Python-measured
   wall time, process peak RSS from /proc, exit status, logs, and whether the C++
   program actually appended a result row.

This version intentionally uses LONG parameter names:
    --inputFile, --outputFile, --resultsFile, --problem, --algorithm, --structure,
    --vertexStart, --vertexEnd

That avoids possible issues with short aliases.
"""

from __future__ import annotations

import csv
import os
import re
import subprocess
import threading
import time
from dataclasses import dataclass
from pathlib import Path
from typing import TextIO


PROJECT_ROOT = Path(__file__).resolve().parents[1]

EXECUTABLE_CANDIDATES = [
    PROJECT_ROOT / "build" / "aizo_project_2",
    PROJECT_ROOT / "cmake-build-debug" / "AiZO_Projekt2",
    PROJECT_ROOT / "aizo_project_2",
    ]

DATASET_DIR = PROJECT_ROOT / "datasets"

RESULTS_DIR = PROJECT_ROOT / "results"
PROGRAM_RESULTS_DIR = RESULTS_DIR / "external_dataset_program_results"
PROGRAM_OUTPUT_DIR = RESULTS_DIR / "external_dataset_program_outputs"
LOG_DIR = RESULTS_DIR / "external_dataset_logs"

GLOBAL_CSV = RESULTS_DIR / "external_dataset_global_measurements.csv"

# No timeout: if the program crashes/freezes, that is part of the experiment.
# The script writes RUNNING before each test, so after reboot you know which test died.
TIMEOUT_SECONDS: int | None = None

SOURCE_VERTEX = 0
TARGET_VERTEX_CANDIDATE = 100

# Values from src/include/Parameters.h:
# enum class Problems { mst = 0, sp = 1, mf = 2 }
PROBLEMS = {
    "MST": 0,
    "SP": 1,
    "MF": 2,
}

# enum class Algorithms { allAlgorithms = 0, prim = 1, kruskal = 2, ... }
ALGORITHMS = {
    "PRIM": 1,
    "KRUSKAL": 2,
    "DIJKSTRA": 3,
    "BELLMAN_FORD": 4,
    "FORD_FULKERSON": 5,
}

# enum class Structures { allStructures = 0, incidenceMatrix = 1, adjacencyList = 2 }
STRUCTURES = {
    "ADJACENCY_MATRIX": 1,
    "ADJACENCY_LIST": 2,
}

TEST_PLAN = [
    # Directed weighted datasets.
    ("directed", "SP", "DIJKSTRA", "ADJACENCY_MATRIX"),
    ("directed", "SP", "BELLMAN_FORD", "ADJACENCY_MATRIX"),
    ("directed", "MF", "FORD_FULKERSON", "ADJACENCY_MATRIX"),
    ("directed", "SP", "DIJKSTRA", "ADJACENCY_LIST"),
    ("directed", "SP", "BELLMAN_FORD", "ADJACENCY_LIST"),
    ("directed", "MF", "FORD_FULKERSON", "ADJACENCY_LIST"),

    # MST datasets.
    ("directed", "MST", "PRIM", "ADJACENCY_MATRIX"),
    ("directed", "MST", "KRUSKAL", "ADJACENCY_MATRIX"),
    ("directed", "MST", "PRIM", "ADJACENCY_LIST"),
    ("directed", "MST", "KRUSKAL", "ADJACENCY_LIST"),
]


@dataclass(frozen=True)
class DatasetPair:
    name: str
    directed_file: Path
    undirected_file: Path
    directed_vertices: int
    directed_edges: int
    undirected_vertices: int
    undirected_edges: int


@dataclass(frozen=True)
class TestCase:
    dataset: DatasetPair
    input_kind: str
    problem: str
    algorithm: str
    structure: str

    @property
    def input_file(self) -> Path:
        return self.dataset.directed_file if self.input_kind == "directed" else self.dataset.undirected_file

    @property
    def vertices(self) -> int:
        return self.dataset.directed_vertices if self.input_kind == "directed" else self.dataset.undirected_vertices

    @property
    def edges(self) -> int:
        return self.dataset.directed_edges if self.input_kind == "directed" else self.dataset.undirected_edges


def output_safe_name(name: str) -> str:
    safe = "".join(ch if ch.isalnum() else "_" for ch in name).strip("_")
    while "__" in safe:
        safe = safe.replace("__", "_")
    return safe or "dataset"


def snake_to_title(name: str) -> str:
    parts = re.split(r"[_\-\s]+", name)
    return "".join(part[:1].upper() + part[1:] for part in parts if part)


def dataset_name_from_weighted_file(path: Path) -> tuple[str, str] | None:
    name = path.name

    if name.endswith("_directed_weighted.txt"):
        return name[:-len("_directed_weighted.txt")], "directed"

    if name.endswith("_undirected_weighted.txt"):
        return name[:-len("_undirected_weighted.txt")], "undirected"

    return None


def read_header(path: Path) -> tuple[int, int]:
    with path.open("r", encoding="utf-8", errors="replace") as file:
        header = file.readline().strip().split()

    if len(header) != 2:
        raise ValueError(f"{path}: expected '<vertices> <edges>' header")

    vertices = int(header[0])
    edges = int(header[1])

    if vertices < 0 or edges < 0:
        raise ValueError(f"{path}: negative vertices/edges in header")

    return vertices, edges


def discover_dataset_pairs() -> list[DatasetPair]:
    raw: dict[str, dict[str, Path]] = {}

    for path in sorted(DATASET_DIR.glob("*_weighted.txt")):
        parsed = dataset_name_from_weighted_file(path)
        if parsed is None:
            continue

        dataset_name, kind = parsed
        raw.setdefault(dataset_name, {})[kind] = path

    pairs: list[DatasetPair] = []

    for name, files in sorted(raw.items()):
        directed = files.get("directed")
        undirected = files.get("undirected")

        if directed is None or undirected is None:
            print(f"[WARN] Skipping {name}: missing directed/undirected pair")
            continue

        directed_vertices, directed_edges = read_header(directed)
        undirected_vertices, undirected_edges = read_header(undirected)

        pairs.append(
            DatasetPair(
                name=name,
                directed_file=directed,
                undirected_file=undirected,
                directed_vertices=directed_vertices,
                directed_edges=directed_edges,
                undirected_vertices=undirected_vertices,
                undirected_edges=undirected_edges,
            )
        )

    return pairs


def find_executable() -> Path:
    for path in EXECUTABLE_CANDIDATES:
        if path.exists() and os.access(path, os.X_OK):
            return path

    tried = "\n".join(f"  - {path}" for path in EXECUTABLE_CANDIDATES)
    raise FileNotFoundError(f"Could not find executable. Tried:\n{tried}")


def program_results_file_for_dataset(dataset_name: str) -> Path:
    return PROGRAM_RESULTS_DIR / f"dataset{snake_to_title(dataset_name)}ProgramResults.csv"


def program_output_file_for_case(case: TestCase, run_index: int) -> Path:
    safe_dataset = output_safe_name(case.dataset.name)
    return (
            PROGRAM_OUTPUT_DIR
            / f"{run_index:04d}_{safe_dataset}_{case.problem}_{case.algorithm}_{case.structure}_{case.input_kind}.txt"
    )


def build_command(executable: Path, case: TestCase, results_file: Path, output_file: Path) -> list[str]:
    source = min(SOURCE_VERTEX, max(0, case.vertices - 1))
    target = min(TARGET_VERTEX_CANDIDATE, max(0, case.vertices - 1))

    command = [
        str(executable),
        "--singleFile",
        "--inputFile", str(case.input_file),
        "--outputFile", str(output_file),
        #"--resultsFile", str(results_file),
        "--problem", str(PROBLEMS[case.problem]),
        "--algorithm", str(ALGORITHMS[case.algorithm]),
        "--structure", str(STRUCTURES[case.structure]),
    ]

    if case.problem in {"SP", "MF"}:
        command += [
            "--vertexStart", str(source),
            "--vertexEnd", str(target),
        ]

    return command


def read_proc_status_kib(pid: int) -> tuple[int | None, int | None]:
    """
    Return (VmRSS, VmHWM) in KiB if available.
    VmRSS = current resident memory.
    VmHWM = high-water mark / peak resident memory reported by Linux.
    """
    status_path = Path("/proc") / str(pid) / "status"

    try:
        text = status_path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return None, None

    rss = None
    hwm = None

    for line in text.splitlines():
        if line.startswith("VmRSS:"):
            parts = line.split()
            if len(parts) >= 2:
                rss = int(parts[1])
        elif line.startswith("VmHWM:"):
            parts = line.split()
            if len(parts) >= 2:
                hwm = int(parts[1])

    return rss, hwm


def stream_reader(pipe, output_file: TextIO) -> None:
    try:
        for chunk in iter(lambda: pipe.readline(), ""):
            if not chunk:
                break
            output_file.write(chunk)
            output_file.flush()
    finally:
        try:
            pipe.close()
        except Exception:
            pass


def csv_fieldnames() -> list[str]:
    return [
        "timestamp",
        "dataset",
        "datasetKind",
        "inputFile",
        "inputFileSizeBytes",
        "vertices",
        "edges",
        "problem",
        "algorithm",
        "structure",
        "source",
        "target",
        "status",
        "exitCode",

        # Time measured by this Python runner around the full process.
        "pythonWallSeconds",

        # RAM measured from Linux /proc while the process is alive.
        "maxVmRssKiB",
        "maxVmRssMiB",
        "maxVmHwmKiB",
        "maxVmHwmMiB",

        # Program-side benchmark file written through --resultsFile.
        "programResultsFile",
        "programResultsSizeBeforeBytes",
        "programResultsSizeAfterBytes",
        "programResultsBytesAdded",

        # Output/log files.
        "programOutputFile",
        "programOutputSizeBytes",
        "stdoutLog",
        "stderrLog",

        "command",
        "note",
    ]


def append_row(path: Path, row: dict[str, object]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    exists = path.exists()

    with path.open("a", encoding="utf-8", newline="") as file:
        writer = csv.DictWriter(file, fieldnames=csv_fieldnames(), extrasaction="ignore")

        if not exists:
            writer.writeheader()

        writer.writerow(row)
        file.flush()
        os.fsync(file.fileno())


def replace_last_row(path: Path, row: dict[str, object]) -> None:
    if not path.exists():
        append_row(path, row)
        return

    with path.open("r", encoding="utf-8", newline="") as file:
        rows = list(csv.DictReader(file))

    if not rows:
        append_row(path, row)
        return

    rows[-1].update({key: str(value) for key, value in row.items()})

    with path.open("w", encoding="utf-8", newline="") as file:
        writer = csv.DictWriter(file, fieldnames=csv_fieldnames(), extrasaction="ignore")
        writer.writeheader()
        writer.writerows(rows)
        file.flush()
        os.fsync(file.fileno())


def run_one(executable: Path, case: TestCase, run_index: int, total_runs: int) -> None:
    RESULTS_DIR.mkdir(parents=True, exist_ok=True)
    PROGRAM_RESULTS_DIR.mkdir(parents=True, exist_ok=True)
    PROGRAM_OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    LOG_DIR.mkdir(parents=True, exist_ok=True)

    results_file = program_results_file_for_dataset(case.dataset.name)
    output_file = program_output_file_for_case(case, run_index)

    results_before = results_file.stat().st_size if results_file.exists() else 0

    source = min(SOURCE_VERTEX, max(0, case.vertices - 1))
    target = min(TARGET_VERTEX_CANDIDATE, max(0, case.vertices - 1))

    command = build_command(executable, case, results_file, output_file)

    safe_dataset = output_safe_name(case.dataset.name)
    log_prefix = f"{run_index:04d}_{safe_dataset}_{case.problem}_{case.algorithm}_{case.structure}_{case.input_kind}"

    stdout_log = LOG_DIR / f"{log_prefix}.stdout.log"
    stderr_log = LOG_DIR / f"{log_prefix}.stderr.log"

    base_row = {
        "timestamp": time.strftime("%Y-%m-%d %H:%M:%S"),
        "dataset": case.dataset.name,
        "datasetKind": case.input_kind,
        "inputFile": str(case.input_file),
        "inputFileSizeBytes": case.input_file.stat().st_size,
        "vertices": case.vertices,
        "edges": case.edges,
        "problem": case.problem,
        "algorithm": case.algorithm,
        "structure": case.structure,
        "source": source if case.problem in {"SP", "MF"} else "",
        "target": target if case.problem in {"SP", "MF"} else "",
        "status": "RUNNING",
        "exitCode": "",
        "pythonWallSeconds": "",
        "maxVmRssKiB": "",
        "maxVmRssMiB": "",
        "maxVmHwmKiB": "",
        "maxVmHwmMiB": "",
        "programResultsFile": str(results_file),
        "programResultsSizeBeforeBytes": results_before,
        "programResultsSizeAfterBytes": "",
        "programResultsBytesAdded": "",
        "programOutputFile": str(output_file),
        "programOutputSizeBytes": "",
        "stdoutLog": str(stdout_log),
        "stderrLog": str(stderr_log),
        "command": " ".join(command),
        "note": "RUNNING row written before launch. If the machine freezes/OOMs, edit this row to CRASHED after reboot.",
    }

    append_row(GLOBAL_CSV, base_row)

    print(
        f"[RUN {run_index:03d}/{total_runs:03d}] "
        f"{case.dataset.name} {case.problem} {case.algorithm} {case.structure} {case.input_kind}",
        flush=True,
    )

    started = time.perf_counter()

    max_rss_kib = 0
    max_hwm_kib = 0

    process = None

    try:
        with stdout_log.open("w", encoding="utf-8", errors="replace") as stdout_file, \
                stderr_log.open("w", encoding="utf-8", errors="replace") as stderr_file:

            process = subprocess.Popen(
                command,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1,
            )

            stdout_thread = threading.Thread(target=stream_reader, args=(process.stdout, stdout_file), daemon=True)
            stderr_thread = threading.Thread(target=stream_reader, args=(process.stderr, stderr_file), daemon=True)
            stdout_thread.start()
            stderr_thread.start()

            while process.poll() is None:
                rss, hwm = read_proc_status_kib(process.pid)

                if rss is not None:
                    max_rss_kib = max(max_rss_kib, rss)
                if hwm is not None:
                    max_hwm_kib = max(max_hwm_kib, hwm)

                if TIMEOUT_SECONDS is not None and (time.perf_counter() - started) > TIMEOUT_SECONDS:
                    process.kill()
                    process.wait()
                    break

                time.sleep(0.05)

            return_code = process.wait()

            # One final read after exit may still work shortly.
            rss, hwm = read_proc_status_kib(process.pid)
            if rss is not None:
                max_rss_kib = max(max_rss_kib, rss)
            if hwm is not None:
                max_hwm_kib = max(max_hwm_kib, hwm)

            stdout_thread.join(timeout=5)
            stderr_thread.join(timeout=5)

        wall = time.perf_counter() - started

        results_after = results_file.stat().st_size if results_file.exists() else 0
        added = results_after - results_before

        output_size = output_file.stat().st_size if output_file.exists() else 0

        if TIMEOUT_SECONDS is not None and wall > TIMEOUT_SECONDS and return_code != 0:
            status = "TIMEOUT"
        elif return_code != 0:
            status = "FAILED"
        else:
            status = "OK"

        note = "Application appended a benchmark row through --resultsFile."
        if status == "NO_RESULT_ROW":
            note = (
                "Application exited with code 0 but did not append to --resultsFile. "
                "This usually means the executable was not rebuilt from the single-run-results-enabled source, "
                "or Parameters::resultsFile is empty inside runSingle(). Check stdout log for printStatisticsToFile."
            )
        elif status == "FAILED":
            note = "Application returned a non-zero exit code. Check stderr log."
        elif status == "TIMEOUT":
            note = f"Timeout after {TIMEOUT_SECONDS} seconds."

        final_row = {
            **base_row,
            "status": status,
            "exitCode": return_code,
            "pythonWallSeconds": f"{wall:.6f}",
            "maxVmRssKiB": max_rss_kib if max_rss_kib else "",
            "maxVmRssMiB": f"{max_rss_kib / 1024:.3f}" if max_rss_kib else "",
            "maxVmHwmKiB": max_hwm_kib if max_hwm_kib else "",
            "maxVmHwmMiB": f"{max_hwm_kib / 1024:.3f}" if max_hwm_kib else "",
            "programResultsSizeAfterBytes": results_after,
            "programResultsBytesAdded": added,
            "programOutputSizeBytes": output_size,
            "note": note,
        }

        replace_last_row(GLOBAL_CSV, final_row)

        print(
            f"          -> {status} pythonWall={wall:.6f}s "
            f"rss={final_row['maxVmRssMiB']}MiB "
            f"hwm={final_row['maxVmHwmMiB']}MiB "
            f"results+={added}B",
            flush=True,
        )

    except Exception as exc:
        wall = time.perf_counter() - started

        if process is not None and process.poll() is None:
            try:
                process.kill()
            except Exception:
                pass

        results_after = results_file.stat().st_size if results_file.exists() else 0
        output_size = output_file.stat().st_size if output_file.exists() else 0

        final_row = {
            **base_row,
            "status": "RUNNER_ERROR",
            "exitCode": "",
            "pythonWallSeconds": f"{wall:.6f}",
            "maxVmRssKiB": max_rss_kib if max_rss_kib else "",
            "maxVmRssMiB": f"{max_rss_kib / 1024:.3f}" if max_rss_kib else "",
            "maxVmHwmKiB": max_hwm_kib if max_hwm_kib else "",
            "maxVmHwmMiB": f"{max_hwm_kib / 1024:.3f}" if max_hwm_kib else "",
            "programResultsSizeAfterBytes": results_after,
            "programResultsBytesAdded": results_after - results_before,
            "programOutputSizeBytes": output_size,
            "note": f"Runner error: {exc}",
        }

        replace_last_row(GLOBAL_CSV, final_row)
        print(f"          -> RUNNER_ERROR {exc}", flush=True)


def main() -> int:
    executable = find_executable()
    datasets = discover_dataset_pairs()
    datasets = [dataset for dataset in datasets if dataset.name.endswith("_connected")]

    if not datasets:
        print(f"No converted dataset pairs found in {DATASET_DIR}")
        return 1

    cases = [
        TestCase(dataset=dataset, input_kind=input_kind, problem=problem, algorithm=algorithm, structure=structure)
        for dataset in datasets
        for input_kind, problem, algorithm, structure in TEST_PLAN
    ]

    print("[INFO] Runner version:       long flags + /proc RAM + Python wall time")
    print(f"[INFO] Executable:           {executable}")
    print(f"[INFO] Dataset dir:          {DATASET_DIR}")
    print(f"[INFO] Program results dir:  {PROGRAM_RESULTS_DIR}")
    print(f"[INFO] Program output dir:   {PROGRAM_OUTPUT_DIR}")
    print(f"[INFO] Global CSV:           {GLOBAL_CSV}")
    print(f"[INFO] Logs dir:             {LOG_DIR}")
    print(f"[INFO] Datasets:             {len(datasets)}")
    print(f"[INFO] Runs:                 {len(cases)}")
    print(f"[INFO] Timeout:              {TIMEOUT_SECONDS}")
    print()

    for index, case in enumerate(cases, start=1):
        run_one(executable, case, index, len(cases))

    print()
    print("[DONE]")
    print(f"Program benchmark results: {PROGRAM_RESULTS_DIR}")
    print(f"Global measurements:       {GLOBAL_CSV}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

