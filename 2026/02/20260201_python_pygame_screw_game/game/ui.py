from __future__ import annotations

import pygame

from .constants import TEXT, MUTED


class UI:
    def __init__(self) -> None:
        pygame.font.init()
        self.font = pygame.font.SysFont(None, 26)
        self.big = pygame.font.SysFont(None, 58)

    def draw_topbar(self, screen: pygame.Surface, removed: int, can_undo: bool) -> None:
        msg = f"Removed: {removed}   |   R: reset   U: undo{' (ok)' if can_undo else ''}"
        surf = self.font.render(msg, True, TEXT)
        screen.blit(surf, (14, 14))

        hint = "Click screws that are not covered by upper plates."
        hs = self.font.render(hint, True, MUTED)
        screen.blit(hs, (14, 40))

    def draw_clear(self, screen: pygame.Surface) -> None:
        s = self.big.render("CLEAR!", True, (255, 255, 255))
        rect = s.get_rect(center=(screen.get_width() // 2, 110))
        screen.blit(s, rect)
