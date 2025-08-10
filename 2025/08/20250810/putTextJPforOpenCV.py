# -*- coding: utf-8 -*-
import os
import cv2
import numpy as np
from PIL import Image, ImageDraw, ImageFont

def putTextJP(
    frame_bgr,
    text: str,
    org: tuple,                 # (x, y) ・・・OpenCV互換（デフォは左下基準）
    font_scale: float = 1.0,    # OpenCVのfontScaleに対応
    color: tuple = (0, 255, 0), # BGR
    thickness: int = 2,         # 太さ（Pillowではstrokeや太字風で近似）
    lineType: int = cv2.LINE_AA,# 未使用（互換のため残す）
    *,
    font_path: str = None,      # 例: r"C:\Windows\Fonts\meiryo.ttc"
    bg_color: tuple | None = None,  # 例: (0, 0, 0) を指定すると背景矩形を描画（BGR）
    padding: int = 6,               # 背景の余白
    bottomLeftOrigin: bool = False  # Trueにするとorgを左下ではなく左上基準に
):
    """
    OpenCVのcv2.putText風に日本語を描画（Pillow利用）。
    - fontFaceは受け取らない（OpenCV互換の呼び方で差し替えやすい）
    - 戻り値: 描画後のBGRフレーム
    """

    # 1) フォント決定
    if font_path is None:
        # Windows想定の日本語フォント候補を探索（見つからなければPillow既定）
        candidates = [
            "C:\\Windows\\Fonts\\meiryo.ttc",
            "C:\\Windows\\Fonts\\YuGothM.ttc",
            "C:\\Windows\\Fonts\\msgothic.ttc",
        ]
        for p in candidates:
            if os.path.exists(p):
                font_path = p
                break

    base_px = 32  # 基準となるpx
    font_size = max(12, int(round(base_px * float(font_scale))))
    try:
        font = ImageFont.truetype(font_path, font_size) if font_path else ImageFont.load_default()
    except Exception:
        font = ImageFont.load_default()

    # 2) OpenCV(BGR) -> PIL(RGB)
    img_rgb = cv2.cvtColor(frame_bgr, cv2.COLOR_BGR2RGB)
    pil_img = Image.fromarray(img_rgb)
    draw = ImageDraw.Draw(pil_img)

    # 3) OpenCVのorgは通常「左下」が基準。Pillowは「左上」基準。
    x, y_baseline = org
    if bottomLeftOrigin:
        # すでに左上基準として扱う（互換オプション）
        text_xy = (x, y_baseline)
    else:
        # フォントのアセントを使って上端yを求める
        try:
            ascent, descent = font.getmetrics()
        except Exception:
            ascent = font_size  # 取得できない場合のフォールバック
        y_top = y_baseline - ascent
        text_xy = (x, y_top)

    # 4) テキストサイズ（bbox）を取得して背景を描く（任意）
    #   PillowはRGB想定なのでBGR -> RGBに並び替え
    stroke_w = max(0, thickness - 1)
    bbox = draw.textbbox(text_xy, text, font=font, stroke_width=stroke_w)
    # bbox = (left, top, right, bottom)
    if bg_color is not None:
        l, t, r, b = bbox
        l -= padding; t -= padding; r += padding; b += padding
        draw.rectangle([l, t, r, b],
                       fill=(bg_color[2], bg_color[1], bg_color[0]))

    # 5) 本文描画
    draw.text(
        text_xy,
        text,
        font=font,
        fill=(color[2], color[1], color[0]),
        stroke_width=stroke_w,
        stroke_fill=(color[2], color[1], color[0])  # 同色で太字風
    )

    # 6) PIL(RGB) -> OpenCV(BGR)
    out_bgr = cv2.cvtColor(np.array(pil_img), cv2.COLOR_RGB2BGR)
    return out_bgr
