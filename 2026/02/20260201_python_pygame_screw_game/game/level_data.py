from __future__ import annotations

from typing import List, Tuple

import pygame

from .entities import Plate, Screw


def build_level_1() -> tuple[list[Screw], list[Plate]]:
    # ---- Screws (at fixed world coords) ----
    screws = [
        Screw(1, 210, 170),
        Screw(2, 300, 170),
        Screw(3, 390, 170),
        Screw(4, 210, 290),
        Screw(5, 300, 290),
        Screw(6, 390, 290),
        Screw(7, 255, 430),
        Screw(8, 345, 430),
    ]

    # ---- Plates ----
    # Design idea:
    # - Lower plates are more "base", upper plates cover some screws -> blocked removal.
    # - Each plate has 2 holes (support points) or more.
    plates: list[Plate] = []

    # Plate 1: bottom base (layer 1)
    plates.append(
        Plate(
            id=101,
            rect=pygame.Rect(165, 405, 270, 36),
            holes_local=[(90, 18), (180, 18)],  # roughly at screws 7,8
            layer=1,
            color=(80, 190, 120),
        )
    )

    # # Plate 2: mid left diagonal (layer 2)
    # plates.append(
    #     Plate(
    #         id=102,
    #         rect=pygame.Rect(160, 260, 210, 40),
    #         holes_local=[(50, 20), (140, 20)],  # screws 4,5
    #         layer=2,
    #         color=(240, 90, 90),
    #     )
    # )

    # Plate 3: mid right diagonal (layer 2)
    plates.append(
        Plate(
            id=103,
            rect=pygame.Rect(230, 260, 210, 40),
            holes_local=[(70, 20), (160, 20)],  # screws 5,6
            layer=2,
            color=(90, 170, 240),
        )
    )

    # Plate 4: top bar (layer 3) - covers screws 1..3 (blocking)
    plates.append(
        Plate(
            id=104,
            rect=pygame.Rect(170, 140, 260, 44),
            holes_local=[(40, 22), (130, 22), (220, 22)],  # near screws 1,2,3
            layer=3,
            color=(250, 210, 90),
        )
    )

    # Plate 5: center small plate (layer 4) - covers screw 5 to make a nasty block
    plates.append(
        Plate(
            id=105,
            rect=pygame.Rect(255, 235, 90, 55),
            holes_local=[(20, 28), (70, 28)],  # near screw 5, plus extra
            layer=4,
            color=(170, 110, 210),
        )
    )

    return screws, plates
