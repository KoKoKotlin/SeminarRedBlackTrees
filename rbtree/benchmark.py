from collections import defaultdict
from enum import Enum, auto
import os
import subprocess
import time

from typing import List, Dict
from functools import reduce
from pprint import pprint

class VERSION(Enum):
    NON_REC = auto()
    REC = auto() 

def compile_version(version):
    if version == VERSION.REC:
        os.system("cd src && make clean && make all REC=1")        
    elif version == VERSION.NON_REC:
        os.system("cd src && make clean && make all")

def execute(number_of_keys):
    print(f"Running benchmark for {number_of_keys} keys!")
    res = subprocess.run(["./src/rbtree.out", str(number_of_keys)], capture_output=True)
    return list(map(int, res.stdout.decode('ascii').strip().split(' ')))

def write_to_file(data: Dict[int, List[float]], mode: str):
    with open(f"benchmark/benchmark_{mode}_{time.time()}.csv", "w") as f:
        f.write("no keys,insert_{rb},search_{rb},delete_{rb},height_{rb},insert_{bin},search_{bin},delete_{bin},height_{bin}\n")
        for key, val in data.items():
            f.write(",".join(map(str, [key] + val)) + "\n")

class Batch:
    def __init__(self, id):
        self.measurements = dict()
        self.id = id

    def add_mesurement(self, no_nodes, data):
        self.measurements[no_nodes] = data

    def __repr__(self) -> str:
        output = f"Batch {self.id}:\n"
        for key, val in self.measurements.items():
            output += f"{key}: {val}\n"

        return output

def add_lists(l1, l2):
    assert len(l1) == len(l2), "Error in List Dimensions!"
    return [l1[i] + l2[i] for i in range(len(l1))]

def avg_data(batches: List[Batch]) -> Dict[int, List[float]]:

    # TODO: make list lenght a constamt
    avg = defaultdict(lambda: [0] * 8)

    # add all measurements up, that have the same number of nodes
    for batch in batches:
        for key, val in batch.measurements.items():
            avg[key] = add_lists(avg[key], list(map(lambda x: x / len(batches), val)))

    return avg

def main():
    start = 100_000
    stop  = 200_000
    step  = 10_000
    batch_size = 10

    compile_version(VERSION.NON_REC)
    batches = []
    for i in range(batch_size):
        print(f"batch {i}:")
        current_batch = Batch(i)
        for nodes in range(start, stop + 1, step):
            current_batch.add_mesurement(nodes, execute(nodes))

        batches.append(current_batch)

    print("Results: ")
    print(batches)

    avg = avg_data(batches)
    pprint(avg)
    write_to_file(avg, "NON_REC")

    compile_version(VERSION.REC)
    batches = []
    for i in range(batch_size):
        print(f"batch {i}:")
        current_batch = Batch(i)
        for nodes in range(start, stop + 1, step):
            current_batch.add_mesurement(nodes, execute(nodes))

        batches.append(current_batch)

    print("Results: ")
    print(batches)

    avg = avg_data(batches)
    pprint(avg)
    write_to_file(avg, "REC")

if __name__ == "__main__":
    main()