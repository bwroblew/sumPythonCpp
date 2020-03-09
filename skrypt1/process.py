import sys
import time
import numpy as np

ITERS = 1000

def load_cases(file_path):
  file = open(file_path, "r")
  cases = []
  for line in file:
    line = line.strip().split(" ")
    line = [int(num) for num in line]
    cases.append(line)
  return cases

def process_lib(case):
  start = time.time()
  for _ in range(0, ITERS):
    s = sum(case)
  end = time.time()
  avg_time = (end - start) / ITERS
  return avg_time, s

def process_nop(case):
  x = int(input())
  start = time.time()
  for i in range(0, ITERS):
    for j in range(20):
      if (x==0):
        x = x * 10 - i
  end = time.time()
  avg_time = (end - start)
  return avg_time, ITERS
  

def process_own(case):
  start = time.time()
  for _ in range(0, ITERS):
    s = 0
    for elem in case:
      s += elem
  end = time.time()
  avg_time = (end - start) / ITERS
  return avg_time, s

def process_np(cases):
  np_cases = np.array(cases)
  start = time.time()
  for _ in range(0, ITERS):
    s = np.add.reduce(np_cases)
  end = time.time()
  avg_time = (end - start) / ITERS
  return avg_time, s

def process(cases, use_lib_impl, use_np=False):
  times = []
  results = []
  for case in cases:
    avg_case_time, result = 0.0, 0
    if use_lib_impl:
      avg_case_time, result = process_lib(case)
      if use_np:
        avg_case_time, result = process_np(case)
    else:
      avg_case_time, result = process_own(case)
    times.append(avg_case_time)
    results.append(result)
    avg_time = sum(times) / len(times)
  return avg_time, results

def save(name, time, results):
  f = open(name, "w")
  f.write(str(time) + "\n")
  for result in results:
    f.write(str(result) + "\n")
  f.close()


if __name__ == "__main__":
  if len(sys.argv) < 2:
    print("Pass case file path as command line argument")
  else:
    cases = load_cases(sys.argv[1])
    print(len(cases))
    if len(sys.argv) < 3 or sys.argv[2] == "lib":
      time_lib, results_lib = process(cases, True)
      save("res_lib", time_lib, results_lib)
      print(time_lib)
    if len(sys.argv) < 3 or sys.argv[2] == "own":
      time_own, results_own = process(cases, False)
      save("res_own", time_own, results_own)
      print(time_own)
    if len(sys.argv) < 3 or sys.argv[2] == "np":
      time_np, results_np = process(cases, True, True)
      save("res_np", time_np, results_np)
      print(time_np)
    if len(sys.argv) < 3 or sys.argv[2] == "nop":
      time_nop, ignore = process_nop(cases)
      print(time_nop)
