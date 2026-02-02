from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Tuple

import pygame


@dataclass(frozen=True)
class ScrewState:
    installed: bool


@dataclass(frozen=True)
class PlateState:
    x: int
    y: int
    vy: float
    falling: bool
    removed: bool


@dataclass(frozen=True)
class Snapshot:
    screw_states: Dict[int, ScrewState]   # key: screw.id
    plate_states: Dict[int, PlateState]   # key: plate.id
    removed_count: int


class UndoStack:
    def __init__(self, capacity: int = 200) -> None:
        self._stack: List[Snapshot] = []
        self._capacity = capacity

    def clear(self) -> None:
        self._stack.clear()

    def push(self, snap: Snapshot) -> None:
        self._stack.append(snap)
        if len(self._stack) > self._capacity:
            # drop oldest
            self._stack.pop(0)

    def pop(self) -> Snapshot | None:
        if not self._stack:
            return None
        return self._stack.pop()

    def __len__(self) -> int:
        return len(self._stack)
