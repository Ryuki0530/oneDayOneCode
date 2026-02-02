from __future__ import annotations

from dataclasses import dataclass
from typing import List, Optional, Tuple

import pygame

from .constants import (
    BG,
    BOARD,
    BOARD_H,
    BOARD_MARGIN_X,
    BOARD_MARGIN_Y,
    BOARD_W,
    FPS,
    HEIGHT,
    SCREW_HIT_R,
    WIDTH,
)

from .entities import Plate, Screw
from .level_data import build_level_1
from .ui import UI
from .undo import PlateState, ScrewState, Snapshot, UndoStack


@dataclass
class GameState:
    screws: List[Screw]
    plates: List[Plate]
    removed_count: int = 0
    cleared: bool = False


class Game:
    def __init__(self) -> None:
        pygame.init()
        pygame.display.set_caption("Screw Out Puzzle (PyGame)")
        self.screen = pygame.display.set_mode((WIDTH, HEIGHT))
        self.clock = pygame.time.Clock()
        self.ui = UI()
        self.undo = UndoStack(capacity=300)

        self.initial_screws, self.initial_plates = build_level_1()
        self.state = self._make_fresh_state()

    def _make_fresh_state(self) -> GameState:
        # Deep-ish copy for safety
        screws = [Screw(s.id, s.x, s.y, s.installed) for s in self.initial_screws]
        plates = []
        for p in self.initial_plates:
            plates.append(
                Plate(
                    id=p.id,
                    rect=p.rect.copy(),
                    holes_local=list(p.holes_local),
                    layer=p.layer,
                    color=p.color,
                    vy=0.0,
                    falling=False,
                    removed=False,
                )
            )
        return GameState(screws=screws, plates=plates, removed_count=0, cleared=False)

    # -----------------------------
    # Undo snapshot / restore
    # -----------------------------
    def _snapshot(self) -> Snapshot:
        screw_states = {s.id: ScrewState(installed=s.installed) for s in self.state.screws}
        plate_states = {
            p.id: PlateState(x=p.rect.x, y=p.rect.y, vy=p.vy, falling=p.falling, removed=p.removed)
            for p in self.state.plates
        }
        return Snapshot(screw_states=screw_states, plate_states=plate_states, removed_count=self.state.removed_count)

    def _restore(self, snap: Snapshot) -> None:
        # Restore screws
        screw_by_id = {s.id: s for s in self.state.screws}
        for sid, ss in snap.screw_states.items():
            if sid in screw_by_id:
                screw_by_id[sid].installed = ss.installed

        # Restore plates
        plate_by_id = {p.id: p for p in self.state.plates}
        for pid, ps in snap.plate_states.items():
            if pid in plate_by_id:
                p = plate_by_id[pid]
                p.rect.x = ps.x
                p.rect.y = ps.y
                p.vy = ps.vy
                p.falling = ps.falling
                p.removed = ps.removed

        self.state.removed_count = snap.removed_count
        self.state.cleared = False  # re-evaluate below
        self._evaluate_clear()

    # -----------------------------
    # Core rule helpers
    # -----------------------------
    def _plates_sorted_for_draw(self) -> List[Plate]:
        return sorted(self.state.plates, key=lambda p: p.layer)

    def _plates_above_layer(self, layer: int) -> List[Plate]:
        return [p for p in self.state.plates if (p.layer > layer and not p.removed)]

    def _is_screw_blocked(self, screw: Screw) -> bool:
        # A screw is blocked if any *upper layer* plate covers the screw position.
        # Here we don't know which plate the screw belongs to; so we use:
        # - Determine the topmost plate at that screw position.
        # - If that topmost plate exists and it's above at least one other plate,
        #   we still treat the screw as blocked for interaction until uncovered.
        #
        # Implementation approach:
        # - If any plate (that is not removed) with layer >= some threshold covers it,
        #   it might block. But we want: "covered by upper plates".
        # - So we say: if there exists a plate that covers the screw point AND
        #   there exists another plate under it? That's messy.
        #
        # Simpler + stable:
        # - We define "blocker plates" as ANY plate with layer >= 3 in this level,
        #   but that becomes level-specific (not good).
        #
        # Better:
        # - Determine the maximum layer among plates covering that point.
        # - If that maximum layer is not the minimum among covering plates,
        #   screw is covered by something above something else.
        #
        # We actually need "clicking a screw is blocked if a plate above it exists".
        # That means: if there exists any plate covering the point that is not the
        # *lowest* one among covering plates, it's blocked.
        #
        # Equivalent:
        # - find plates covering the screw point (alive)
        # - if there are 2+ plates covering it, the topmost plate blocks access.
        # - if only 1 plate covers it, it's accessible (it's on top surface).
        #
        x, y = screw.pos()
        covering = [p for p in self.state.plates if (not p.removed and p.rect.collidepoint(x, y))]
        if len(covering) <= 1:
            return False
        # Something is stacked above -> blocked
        return True

    def _pick_screw(self, mx: int, my: int) -> Optional[Screw]:
        # Pick nearest installed screw within hit radius.
        best = None
        best_d2 = 10**18
        for s in self.state.screws:
            if not s.installed:
                continue
            if not s.hit_test(mx, my, SCREW_HIT_R):
                continue
            dx = mx - s.x
            dy = my - s.y
            d2 = dx * dx + dy * dy
            if d2 < best_d2:
                best_d2 = d2
                best = s
        return best

    def _recompute_support_and_fall(self) -> None:
        # Determine which plates should start falling
        for p in self.state.plates:
            if p.removed:
                continue
            supported = p.is_supported(self.state.screws)
            if supported:
                # If you want, you can stop fall mid-air if re-supported (rare in this puzzle).
                # We'll keep it simple: once falling, it stays falling.
                pass
            else:
                p.start_fall()

    def _update_physics(self) -> None:
        for p in self.state.plates:
            p.update_physics()

        # Update removed count (plates that just became removed)
        # We'll compute removed_count as removed screws count (spec says Removed: screws removed)
        # so this is not needed. But we keep clear evaluation.
        self._evaluate_clear()

    def _evaluate_clear(self) -> None:
        self.state.cleared = all(p.removed for p in self.state.plates)

    # -----------------------------
    # Actions
    # -----------------------------
    def _try_remove_screw(self, screw: Screw) -> bool:
        if self.state.cleared:
            return False

        if self._is_screw_blocked(screw):
            return False

        # Save undo snapshot BEFORE changing state
        self.undo.push(self._snapshot())

        screw.installed = False
        self.state.removed_count += 1

        # after removal, plates may lose support and start falling
        self._recompute_support_and_fall()
        return True

    def reset(self) -> None:
        self.state = self._make_fresh_state()
        self.undo.clear()

    def undo_once(self) -> None:
        snap = self.undo.pop()
        if snap is None:
            return
        self._restore(snap)

    # -----------------------------
    # Draw
    # -----------------------------
    def _draw_board(self) -> None:
        # Background
        self.screen.fill(BG)

        # Board area
        board_rect = pygame.Rect(BOARD_MARGIN_X, BOARD_MARGIN_Y, BOARD_W, BOARD_H)
        pygame.draw.rect(self.screen, BOARD, board_rect, border_radius=18)
        pygame.draw.rect(self.screen, (70, 75, 86), board_rect, 3, border_radius=18)

    def _draw_entities(self) -> None:
        # Plates by layer
        for p in self._plates_sorted_for_draw():
            p.draw(self.screen)

        # Screws above plates visually
        for s in self.state.screws:
            blocked = self._is_screw_blocked(s) if s.installed else False
            s.draw(self.screen, blocked=blocked)

    def _draw_ui(self) -> None:
        self.ui.draw_topbar(self.screen, removed=self.state.removed_count, can_undo=(len(self.undo) > 0))
        if self.state.cleared:
            self.ui.draw_clear(self.screen)

    # -----------------------------
    # Main loop
    # -----------------------------
    def run(self) -> None:
        running = True

        # Initial support computation (in case some plates have 0 support from start)
        self._recompute_support_and_fall()

        while running:
            dt = self.clock.tick(FPS)

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False

                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_r:
                        self.reset()
                        self._recompute_support_and_fall()
                    elif event.key == pygame.K_u:
                        self.undo_once()

                elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                    mx, my = event.pos
                    s = self._pick_screw(mx, my)
                    if s is not None:
                        self._try_remove_screw(s)

            # Update physics each frame
            self._update_physics()

            # Draw
            self._draw_board()
            self._draw_entities()
            self._draw_ui()
            pygame.display.flip()

        pygame.quit()
