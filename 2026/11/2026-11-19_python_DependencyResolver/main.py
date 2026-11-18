import heapq


class DependencyResolver:
    def __init__(self) -> None:
        self._dependencies: dict[str, set[str]] = {}
        self._dependents: dict[str, set[str]] = {}

    def add_task(self, name: str) -> None:
        if name not in self._dependencies:
            self._dependencies[name] = set()
            self._dependents[name] = set()

    def add_dependency(self, task: str, depends_on: str) -> None:
        self.add_task(task)
        self.add_task(depends_on)

        if depends_on not in self._dependencies[task]:
            self._dependencies[task].add(depends_on)
            self._dependents[depends_on].add(task)

    def resolve(self) -> list[str]:
        if not self._dependencies:
            return []

        indegree = {name: len(dependencies)
                    for name, dependencies in self._dependencies.items()}
        ready = [name for name, degree in indegree.items() if degree == 0]
        heapq.heapify(ready)

        result: list[str] = []
        while ready:
            name = heapq.heappop(ready)
            result.append(name)

            for dependent in self._dependents[name]:
                indegree[dependent] -= 1
                if indegree[dependent] == 0:
                    heapq.heappush(ready, dependent)

        if len(result) != len(self._dependencies):
            raise ValueError("Circular dependency detected")

        return result

    def has_cycle(self) -> bool:
        indegree = {name: len(dependencies)
                    for name, dependencies in self._dependencies.items()}
        ready = [name for name, degree in indegree.items() if degree == 0]
        heapq.heapify(ready)

        visited = 0
        while ready:
            name = heapq.heappop(ready)
            visited += 1

            for dependent in self._dependents[name]:
                indegree[dependent] -= 1
                if indegree[dependent] == 0:
                    heapq.heappush(ready, dependent)

        return visited != len(self._dependencies)

    def remove_task(self, name: str) -> None:
        if name not in self._dependencies:
            return

        for depends_on in self._dependencies[name]:
            self._dependents[depends_on].discard(name)

        for dependent in self._dependents[name]:
            self._dependencies[dependent].discard(name)

        del self._dependencies[name]
        del self._dependents[name]

    def get_dependencies(self, name: str) -> list[str]:
        if name not in self._dependencies:
            return []
        return sorted(self._dependencies[name])


def main() -> None:
    resolver = DependencyResolver()

    resolver.add_dependency("Build", "Compile")
    resolver.add_dependency("Compile", "Generate")
    resolver.add_dependency("Test", "Build")
    resolver.add_task("Lint")

    print(resolver.resolve())
    # ['Generate', 'Compile', 'Build', 'Lint', 'Test']

    print(resolver.has_cycle())
    # False

    resolver.add_dependency("Generate", "Test")

    print(resolver.has_cycle())
    # True

    try:
        print(resolver.resolve())
    except ValueError:
        print("Circular dependency detected")


if __name__ == "__main__":
    main()