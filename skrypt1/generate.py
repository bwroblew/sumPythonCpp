import time
import random

LENGTHS = [10000, 500000]
CASES = 20
INT_RANGE = (-1000, 1000)

def generate_tests(length, case_id):
  file_name = "test" + str(length) + "_id" + str(case_id)
  f = open(file_name, "w")

  for _ in range(0, CASES):
    for _ in range(0, length):
      num = random.randint(INT_RANGE[0], INT_RANGE[1])
      f.write(str(num) + " ")
    f.write("\n")
  f.close()

if __name__ == "__main__":
  case_id = int(time.time()) % 1000
  for length in LENGTHS:
    generate_tests(length, case_id)


