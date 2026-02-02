from __future__ import annotations

from dataclasses import dataclass
from typing import List, Tuple

import pygame

from .constants import (
    GRAVITY,
    OFFSCREEN_Y,
    PLATE_CORNER_RADIUS,
    SCREW_R,
    SUPPORT_RADIUS,
)


Vec2 = Tuple[int, int]


@dataclass
class Screw:
    id: int
    x: int
    y: int
    installed: bool = True

    def pos(self) -> Vec2:
        return (self.x, self.y)

    def draw(self, screen: pygame.Surface, blocked: bool = False) -> None:
        # Body
        if not self.installed:
            # removed screws: draw faint outline (optional) or nothing
            pygame.draw.circle(screen, (90, 90, 90), self.pos(), SCREW_R, 2)
            return

        color = (220, 220, 230) if not blocked else (140, 140, 150)
        rim = (40, 40, 50)

        pygame.draw.circle(screen, rim, self.pos(), SCREW_R + 2)
        pygame.draw.circle(screen, color, self.pos(), SCREW_R)

        # Cross (+)
        cx, cy = self.pos()
        l = 8
        pygame.draw.line(screen, (60, 60, 70), (cx - l, cy), (cx + l, cy), 3)
        pygame.draw.line(screen, (60, 60, 70), (cx, cy - l), (cx, cy + l), 3)

    def hit_test(self, mx: int, my: int, hit_r: int) -> bool:
        dx = mx - self.x
        dy = my - self.y
        return (dx * dx + dy * dy) <= (hit_r * hit_r)


@dataclass
class Plate:
    id: int
    rect: pygame.Rect
    holes_local: List[Vec2]  # local coords relative to rect.topleft
    layer: int
    color: Tuple[int, int, int]

    vy: float = 0.0
    falling: bool = False
    removed: bool = False  # offscreen -> True

    def holes_world(self) -> List[Vec2]:
        ox, oy = self.rect.topleft
        return [(ox + hx, oy + hy) for (hx, hy) in self.holes_local]

    def is_supported(self, screws: List[Screw]) -> bool:
        if self.removed:
            return False
        # At least one hole is supported by any installed screw within SUPPORT_RADIUS
        for (hx, hy) in self.holes_world():
            for s in screws:
                if not s.installed:
                    continue
                dx = s.x - hx
                dy = s.y - hy
                if (dx * dx + dy * dy) <= (SUPPORT_RADIUS * SUPPORT_RADIUS):
                    return True
        return False

    def update_physics(self) -> None:
        if self.removed:
            return
        if not self.falling:
            return

        self.vy += GRAVITY
        self.rect.y += int(self.vy)

        if self.rect.top > OFFSCREEN_Y:
            self.removed = True

    def start_fall(self) -> None:
        if self.removed:
            return
        if not self.falling:
            self.falling = True
            self.vy = 0.0

    def stop_fall(self) -> None:
        self.falling = False
        self.vy = 0.0

    def draw(self, screen: pygame.Surface) -> None:
        if self.removed:
            return

        # Plate body
        pygame.draw.rect(screen, self.color, self.rect, border_radius=PLATE_CORNER_RADIUS)

        # Hole visuals
        for (hx, hy) in self.holes_world():
            pygame.draw.circle(screen, (20, 20, 20), (hx, hy), 7)
            pygame.draw.circle(screen, (80, 80, 80), (hx, hy), 7, 2)
