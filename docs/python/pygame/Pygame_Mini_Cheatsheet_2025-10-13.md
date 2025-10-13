# Pygame 使い方ミニチートシート（2025-10-13）

このページは上記課題「Dodge Rain」を30分で作るための最小限のリファレンスです。

---

## 初期化とウィンドウ生成
```python
import pygame
pygame.init()
screen = pygame.display.set_mode((480, 640))
pygame.display.set_caption("Dodge Rain")
clock = pygame.time.Clock()
```

## メインループの基本形
```python
running = True
while running:
    # 1) 入力・イベント
    for e in pygame.event.get():
        if e.type == pygame.QUIT:
            running = False

    keys = pygame.key.get_pressed()  # 同時押しも拾える
    # 2) 更新: 位置・速度・スポーンなどを更新
    # 3) 描画
    screen.fill((0, 0, 0))
    # pygame.draw.rect(screen, (255,255,255), rect)
    pygame.display.flip()  # 画面反映
    clock.tick(60)         # FPS固定
```

## 移動と境界制限
```python
player = pygame.Rect(210, 600, 60, 12)  # x, y, w, h
speed = 6
if keys[pygame.K_LEFT] or keys[pygame.K_a]:
    player.x -= speed
if keys[pygame.K_RIGHT] or keys[pygame.K_d]:
    player.x += speed
player.clamp_ip(screen.get_rect())  # 画面外に出ない
```

## スポーン（ランダム）と落下
```python
import random
enemies = []
spawn_timer = 0
spawn_interval = 700  # ミリ秒

now = pygame.time.get_ticks()
if now - spawn_timer > spawn_interval:
    spawn_timer = now
    x = random.randint(0, 480-30)
    enemies.append(pygame.Rect(x, -18, 30, 18))

# 更新（落下）
fall_speed = 4
for rect in enemies:
    rect.y += fall_speed
# 画面外の敵を回収
enemies = [r for r in enemies if r.top < 640]
```

## 衝突判定
```python
for r in enemies:
    if player.colliderect(r):
        # ゲームオーバーへ
        pass
```

## テキスト描画（スコア）
```python
font = pygame.font.SysFont(None, 28)
score_surf = font.render(f"Score: {score}", True, (255,255,255))
screen.blit(score_surf, (8, 8))
```

## 状態管理（簡易）
```python
PLAYING, GAME_OVER = 0, 1
state = PLAYING

if state == GAME_OVER:
    over = font.render("GAME OVER  R:Restart  ESC:Exit", True, (255,80,80))
    screen.blit(over, (40, 300))
```

## サウンド（任意）
```python
# pygame.mixer.init() は pygame.init() で多くの場合自動
hit_se = pygame.mixer.Sound(file="beep.wav")  # ファイルがある場合
hit_se.play()
```

## 終了処理と標準出力
```python
print(f"FINAL_SCORE:{score}")
pygame.quit()
```

---

## 最小スターター（写経用・完成品ではありません）
```python
import pygame, random

W, H = 480, 640
WHITE, BLACK = (255,255,255), (0,0,0)
PLAYING, GAME_OVER = 0, 1

def main():
    pygame.init()
    screen = pygame.display.set_mode((W, H))
    pygame.display.set_caption("Dodge Rain")
    clock = pygame.time.Clock()
    font = pygame.font.SysFont(None, 28)

    player = pygame.Rect(W//2-30, H-40, 60, 12)
    enemies = []
    score, hiscore = 0, 0

    state = PLAYING
    spawn_timer = 0
    spawn_interval = 700
    fall_speed = 4

    running = True
    while running:
        for e in pygame.event.get():
            if e.type == pygame.QUIT:
                running = False
            elif e.type == pygame.KEYDOWN and state == GAME_OVER:
                if e.key == pygame.K_r:  # Restart
                    # リスタート
                    player.update(W//2-30, H-40, 60, 12)
                    enemies.clear()
                    score = 0
                    state = PLAYING
                elif e.key == pygame.K_ESCAPE:
                    running = False

        keys = pygame.key.get_pressed()
        if state == PLAYING:
            if keys[pygame.K_LEFT] or keys[pygame.K_a]:
                player.x -= 6
            if keys[pygame.K_RIGHT] or keys[pygame.K_d]:
                player.x += 6
            player.clamp_ip(screen.get_rect())

            now = pygame.time.get_ticks()
            if now - spawn_timer > spawn_interval:
                spawn_timer = now
                x = random.randint(0, W-30)
                enemies.append(pygame.Rect(x, -18, 30, 18))
                # 難化
                if spawn_interval > 300:
                    spawn_interval -= 5
                if fall_speed < 12:
                    fall_speed += 0.02

            # 落下＆衝突
            i = 0
            while i < len(enemies):
                r = enemies[i]
                r.y += int(fall_speed)
                if r.top >= H:
                    score += 1
                    enemies.pop(i)
                elif player.colliderect(r):
                    state = GAME_OVER
                    hiscore = max(hiscore, score)
                    i += 1
                else:
                    i += 1

        # 描画
        screen.fill(BLACK)
        pygame.draw.rect(screen, WHITE, player)
        for r in enemies:
            pygame.draw.rect(screen, WHITE, r)
        screen.blit(font.render(f"Score: {score}", True, WHITE), (8, 8))
        screen.blit(font.render(f"Hi: {hiscore}", True, WHITE), (W-100, 8))

        if state == GAME_OVER:
            txt = font.render("GAME OVER  R:Restart  ESC:Exit", True, (255, 80, 80))
            screen.blit(txt, (30, H//2))

        pygame.display.flip()
        clock.tick(60)

    print(f"FINAL_SCORE:{score}")
    pygame.quit()

if __name__ == "__main__":
    main()
```

---

## デバッグの小技
- 矩形の境界を表示したいときは `pygame.draw.rect` で枠線にして描く（第5引数に線幅）
- スポーンが多すぎるときは `spawn_interval` を大きく、少なければ小さく調整
- 挙動が重い場合は `clock.get_fps()` を表示して目安にする