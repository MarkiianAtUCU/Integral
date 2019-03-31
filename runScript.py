import subprocess
import os
import sys

EXECUTABLE = "./build/Integral"+".exe" if os.name == 'nt' else ""
EPSILON = 1000
CONFIG_NAME = "conf.txt"
SAMPLE_CONFIG = \
"""
err_abs: 400
err_rel: 0.0005
num_threads: *
x_start: -100
x_end: 100
y_start: -100
y_end: 100
"""

if len(sys.argv) != 2:
    raise Exception("Invalid call, valid call:\npython runScript.py <num of iteration> ")

iter_num = int(sys.argv[1])



def check_all(lst):
    res = []
    for i in range(1, len(lst)):
        res.append(abs(lst[0]-lst[i]) <= EPSILON)
    return all(res)


def test_method(n, n_threads):
    res = []
    print(f"testing on {n_threads} thread")

    with open(CONFIG_NAME,"w") as f:
        f.write(SAMPLE_CONFIG.replace("*", str(n_threads)))

    for i in range(n):
        print(f"{EXECUTABLE} {CONFIG_NAME} <-- {i+1} time")
        result = subprocess.run([EXECUTABLE, CONFIG_NAME], stdout=subprocess.PIPE)
        tmp_res = [i.strip().split(":") for i in result.stdout.decode('utf-8').split("\n")]
        res.append(int(tmp_res[3][1]))
        print(" :".join(tmp_res[1])+"  "+" :".join(tmp_res[2])+"\n")

    return min(res), float(tmp_res[0][1])


methods = [1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12, 16, 32]


def main():
    comp_results = ""

    if not os.path.exists(EXECUTABLE):
        raise Exception("No executable found, consider running compilation with Cmake file")

    print(f"Running C++ {len(methods)} methods {iter_num} times each:")
    # print("        ALG            TIME")
    results = []
    for i in methods:
        method_time, result = test_method(iter_num, i)
        print(f"{method_time}us for {i} threads\n")
        comp_results+=f"{i}, {method_time}\n"
        # print(f" * {i + ' ' * (max(len(i[0]), 16)-len(i[0]))}   {method_time} us")
        results.append(result)
    print(comp_results)
    print("All results are the same" if check_all(results) else "Error, results are not the same")


if __name__ == '__main__':
    main()
