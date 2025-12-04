from loguru import logger
import sys

logger.remove()
logger.add(
    sys.stdout,
    colorize=True,
    format="<green>{time:HH:mm:ss}</green> | <level>{level: <8}</level> | <level>{message}</level>",
)
logger.add(
    "app.log",
    rotation="200 KB",
    encoding="utf-8",
    format="{time:YYYY-MM-DD HH:mm:ss} | {level} | {message}",
)

logger.opt(colors=True).info("<cyan>Program Started</cyan>")

while True:
    user_input = input("Enter a number (0 to exit): ").strip()
    try:
        number = int(user_input)
    except ValueError:
        logger.warning(f"{user_input!r} is not a valid integer")
        continue

    if number == 0:
        logger.error("0 received, program terminated")
        break
    if number % 2 == 0:
        logger.info(f"{number} is even")
    else:
        logger.warning(f"{number} is odd")