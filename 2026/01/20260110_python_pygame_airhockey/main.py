import pygame
import sys
import math

# 初期化
pygame.init()

# 定数
WIDTH, HEIGHT = 800, 600
FPS = 60
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
CYAN = (0, 255, 255)

# パドル
PADDLE_WIDTH = 120
PADDLE_HEIGHT = 15
PADDLE_SPEED = 6

# ボール
BALL_RADIUS = 10
BALL_MAX_SPEED = 8
BALL_INITIAL_SPEED = 4

class Ball:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.vx = BALL_INITIAL_SPEED
        self.vy = -BALL_INITIAL_SPEED
        self.radius = BALL_RADIUS
    
    def update(self):
        self.x += self.vx
        self.y += self.vy
        
        # 壁反射
        if self.x - self.radius < 0 or self.x + self.radius > WIDTH:
            self.vx = -self.vx
            self.x = max(self.radius, min(WIDTH - self.radius, self.x))
        
        if self.y - self.radius < 0:
            self.vy = -self.vy
            self.y = max(self.radius, self.y)
    
    def reflect_paddle(self, paddle):
        # パドルとの当たり判定（矩形近似）
        if (self.x > paddle.x - PADDLE_WIDTH//2 and 
            self.x < paddle.x + PADDLE_WIDTH//2 and
            self.y + self.radius > paddle.y and
            self.y < paddle.y + PADDLE_HEIGHT):
            
            # パドル中心からの相対位置を正規化 [-1, 1]
            t = (self.x - paddle.x) / (PADDLE_WIDTH / 2)
            t = max(-1, min(1, t))
            
            # 反射角を決定
            self.vx = BALL_MAX_SPEED * t
            self.vy = -abs(self.vy) - 0.5  # 上方向へ
            
            # 速度上限
            speed = math.sqrt(self.vx**2 + self.vy**2)
            if speed > BALL_MAX_SPEED:
                self.vx = self.vx * BALL_MAX_SPEED / speed
                self.vy = self.vy * BALL_MAX_SPEED / speed
            
            # ボール位置を調整
            self.y = paddle.y - self.radius
            return True
        return False
    
    def draw(self, screen):
        pygame.draw.circle(screen, CYAN, (int(self.x), int(self.y)), self.radius)

class Paddle:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def update(self, keys):
        if keys[pygame.K_LEFT] or keys[pygame.K_a]:
            self.x -= PADDLE_SPEED
        if keys[pygame.K_RIGHT] or keys[pygame.K_d]:
            self.x += PADDLE_SPEED
        
        # 画面外に出ない
        self.x = max(PADDLE_WIDTH//2, min(WIDTH - PADDLE_WIDTH//2, self.x))
    
    def draw(self, screen):
        rect = pygame.Rect(self.x - PADDLE_WIDTH//2, self.y, PADDLE_WIDTH, PADDLE_HEIGHT)
        pygame.draw.rect(screen, WHITE, rect)

# メイン
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Air Hockey")
clock = pygame.time.Clock()
font = pygame.font.Font(None, 36)

paddle = Paddle(WIDTH // 2, HEIGHT - 30)
ball = Ball(WIDTH // 2, HEIGHT // 2)
score = 0
game_running = True

while game_running:
    clock.tick(FPS)
    
    # イベント処理
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            game_running = False
    
    keys = pygame.key.get_pressed()
    
    # 更新
    paddle.update(keys)
    ball.update()
    
    if ball.reflect_paddle(paddle):
        score += 1
    
    # ミス判定
    if ball.y > HEIGHT:
        ball = Ball(WIDTH // 2, HEIGHT // 2)
        score = 0
    
    # 描画
    screen.fill(BLACK)
    ball.draw(screen)
    paddle.draw(screen)
    
    score_text = font.render(f"Score: {score}", True, WHITE)
    screen.blit(score_text, (10, 10))
    
    pygame.display.flip()

pygame.quit()
sys.exit()