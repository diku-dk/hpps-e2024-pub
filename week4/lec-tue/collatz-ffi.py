import sys
import ctypes

c_lib = ctypes.CDLL('./libcollatz.so')

if __name__ == '__main__':
    k = int(sys.argv[1])
    sum = 0
    for i in range(1, k):
        sum += c_lib.collatz(i)
    print(sum)
