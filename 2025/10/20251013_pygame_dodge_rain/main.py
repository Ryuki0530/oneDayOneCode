import random
import sys
import pygame

WIDTH, HEIGHT = 480, 640
FPS = 60
PLAYER_SIZE = (60, 12)
ENEMY_SIZE = (20, 38)
START_SPEED = 6
MAX_SPEED = 12
START_SPAWN_DELAY = 900
MIN_SPAWN_DELAY = 240
DIFFICULTY_STEP = 0.02
FONT_NAME = "consolas"


class Player:
    def __init__(self):
        self.speed = 6
        self.rect = pygame.Rect(
            (WIDTH - PLAYER_SIZE[0]) // 2,
            HEIGHT - PLAYER_SIZE[1] - 20,
            *PLAYER_SIZE,
        )

    def update(self, dt, keys):
        move = (keys[pygame.K_RIGHT] or keys[pygame.K_d]) - (
            keys[pygame.K_LEFT] or keys[pygame.K_a]
        )
        self.rect.x += move * self.speed * dt
        self.rect.x = max(0, min(self.rect.x, WIDTH - self.rect.width))

    def shoot(self):
        return BulletFromPlayer(self.rect.centerx - 2, self.rect.top)

    def draw(self, surface):
        pygame.draw.rect(surface, pygame.Color("dodgerblue"), self.rect)


class BulletFromPlayer:
    def __init__(self, x, y):
        self.rect = pygame.Rect(x, y, 4, 10)
        self.speed = -10

    def update(self, dt):
        self.rect.y += self.speed * dt

    def draw(self, surface):
        pygame.draw.rect(surface, pygame.Color("yellow"), self.rect)

    def off_screen(self):
        return self.rect.bottom < 0

class Enemy:
    def __init__(self, speed):
        x = random.randint(0, WIDTH - ENEMY_SIZE[0])
        self.rect = pygame.Rect(x, -ENEMY_SIZE[1], *ENEMY_SIZE)
        self.speed = speed

    def update(self, dt):
        self.rect.y += self.speed * dt

    def draw(self, surface):
        pygame.draw.rect(surface, pygame.Color("orangered"), self.rect)

    def off_screen(self):
        return self.rect.top > HEIGHT


def spawn_enemy(enemies, speed):
    enemies.append(Enemy(speed))


def draw_text(surface, text, size, color, pos, anchor="topleft"):
    font = pygame.font.SysFont(FONT_NAME, size)
    rendered = font.render(text, True, color)
    rect = rendered.get_rect(**{anchor: pos})
    surface.blit(rendered, rect)


def game_loop(screen, clock):
    player = Player()
    enemies = []
    bullets = []
    score = 0
    spawn_delay = START_SPAWN_DELAY
    last_spawn = pygame.time.get_ticks()
    enemy_speed = START_SPEED
    state = "PLAYING"

    while True:
        dt = clock.tick(FPS) / 16.6667
        keys = pygame.key.get_pressed()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return score, False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    return score, False
                if state == "GAME_OVER" and event.key == pygame.K_r:
                    return score, True
                if event.key == pygame.K_SPACE and state == "PLAYING":
                    bullets.append(player.shoot())

        screen.fill(pygame.Color("black"))

        if state == "PLAYING":
            player.update(dt, keys)

            # bullets update / draw / collisions
            for bullet in bullets[:]:
                bullet.update(dt)
                bullet.draw(screen)
                if bullet.off_screen():
                    try:
                        bullets.remove(bullet)
                    except ValueError:
                        pass
                    continue
                for enemy in enemies[:]:
                    if bullet.rect.colliderect(enemy.rect):
                        try:
                            enemies.remove(enemy)
                        except ValueError:
                            pass
                        try:
                            bullets.remove(bullet)
                        except ValueError:
                            pass
                        score += 1
                        break

            now = pygame.time.get_ticks()
            if now - last_spawn >= spawn_delay:
                spawn_enemy(enemies, enemy_speed)
                last_spawn = now
                spawn_delay = max(MIN_SPAWN_DELAY, spawn_delay - 5)
                enemy_speed = min(MAX_SPEED, enemy_speed + DIFFICULTY_STEP)

            for enemy in enemies[:]:
                enemy.update(dt)
                enemy.draw(screen)
                if enemy.rect.colliderect(player.rect):
                    state = "GAME_OVER"
                if enemy.off_screen():
                    enemies.remove(enemy)
                    score += 1

            player.draw(screen)
        else:
            for enemy in enemies:
                enemy.draw(screen)
            player.draw(screen)
            draw_text(screen, "GAME OVER", 54, pygame.Color("white"), (WIDTH // 2, HEIGHT // 2 - 40), "center")
            draw_text(screen, "R: Restart / ESC: Exit", 24, pygame.Color("lightgray"), (WIDTH // 2, HEIGHT // 2 + 20), "center")

        draw_text(screen, f"SCORE: {score}", 24, pygame.Color("white"), (12, 12))
        pygame.display.flip()


def main():
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Dodge Rain")
    clock = pygame.time.Clock()
    high_score = 0
    final_score = 0
    running = True

    while running:
        score, restart = game_loop(screen, clock)
        final_score = score
        high_score = max(high_score, score)
        if restart:
            draw_text(screen, f"HIGHSCORE: {high_score}", 24, pygame.Color("yellow"), (WIDTH - 12, 12), "topright")
            pygame.display.flip()
            pygame.time.wait(600)
        else:
            running = False

    pygame.quit()
    print(f"FINAL_SCORE:{final_score}")
    sys.exit()


if __name__ == "__main__":
    main()