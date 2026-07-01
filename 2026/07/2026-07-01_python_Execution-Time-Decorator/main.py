import time
import functools


def measure_time(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        result = func(*args, **kwargs)
        end = time.perf_counter()
        ms = (end - start) * 1000
        print(f"{func.__name__} took {ms:.2f} ms")
        return result

    return wrapper


@measure_time
def slow_sum(n):
    total = 0
    for i in range(n):
        total += i
    return total


if __name__ == "__main__":
    res = slow_sum(1000000)
    print(f"Result = {res}")
