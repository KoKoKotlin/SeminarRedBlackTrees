from enum import Enum, auto
import os
import subprocess
import time

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
    return ','.join(res.stdout.decode('ascii').strip().split(' '))

def main():
    output = []
    start = 100_000
    stop  = 200_000
    step  = 10_000
    
    compile_version(VERSION.NON_REC)
    output.extend([f"REC,{i}," + execute(i) for i in range(start, stop + 1, step)])

    compile_version(VERSION.REC)
    output.extend([f"NON_REC,{i}," + execute(i) for i in range(start, stop + 1, step)])

    with open(f"benchmark/benchmark{time.time()}.csv", "w") as f:
        f.write("mode,number_of_keys,insert_rb,search_rb,delete_rb,height_rb,insert_bin,search_bin,delete_bin,height_bin\n")
        f.write("\n".join(output))


if __name__ == "__main__":
    main()