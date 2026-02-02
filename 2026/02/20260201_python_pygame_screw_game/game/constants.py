from __future__ import annotations

# Window
WIDTH = 600
HEIGHT = 700
FPS = 60

# Colors
BG = (22, 24, 28)
BOARD = (40, 42, 48)
TEXT = (240, 240, 240)
MUTED = (170, 170, 170)

# Screws
SCREW_R = 12
SCREW_HIT_R = 16

# Plate physics
GRAVITY = 0.55
SUPPORT_RADIUS = 12  # hole center <= this distance to an installed screw => supported

# Rendering
PLATE_CORNER_RADIUS = 10

# Board area (visual)
BOARD_MARGIN_X = 70
BOARD_MARGIN_Y = 80
BOARD_W = WIDTH - BOARD_MARGIN_X * 2
BOARD_H = HEIGHT - BOARD_MARGIN_Y * 2

# Clear condition threshold
OFFSCREEN_Y = HEIGHT + 80
