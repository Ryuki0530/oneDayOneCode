import time
from numba import njit

def normal_sum_squares(n: int) -> int:
    s = 0
    for i in range(n):
        s += i * i
    return s


@njit
def numba_sum_squares(n: int) -> int:
    s = 0
    for i in range(n):
        s += i * i
    return s


def benchmark(n: int) -> None:
    start = time.perf_counter()
    normal_sum_squares(n)
    normal_elapsed = time.perf_counter() - start

    start = time.perf_counter()
    numba_sum_squares(n)
    numba_first_elapsed = time.perf_counter() - start

    start = time.perf_counter()
    numba_sum_squares(n)
    numba_second_elapsed = time.perf_counter() - start

    print(f"Normal loop:         {normal_elapsed:8.3f} sec")
    print(f"Numba (first call):  {numba_first_elapsed:8.3f} sec")
    print(f"Numba (second call): {numba_second_elapsed:8.3f} sec")


if __name__ == "__main__":
    benchmark(10_000_000)

"""
python .\main.py
Normal loop:            1.611 sec
Numba (first call):     6.909 sec
Numba (second call):    0.000 sec
二回目以降早すぎてウケる。
"""