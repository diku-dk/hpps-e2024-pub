# collatz.py

import sys

def collatz(n):
    i = 0
    while n != 1:
        if n % 2 == 0:
            n = n / 2
        else:
            n = 3 * n + 1
        i = i + 1
    return i

if __name__ == '__main__':
    k = int(sys.argv[1])
    sum = 0
    for i in range(1, k):
        sum += collatz(i)
    print(sum)
