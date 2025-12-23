# camera_preview.py
# -----------------------------------------------------------------------------
# OpenCV でカメラを開いてプレビュー表示（最小）
# ここに「台形（紙）検出＆透視変換プレビュー」を追記した版
# 使い方:
#   python camera_preview.py 0
#   画面: 入力プレビュー と 透視変換後プレビュー（検出時のみ）
#   キー: q/ESC で終了, （今回は保存は未実装。次段でSPACE撮影を足せます）
# 依存: opencv-python, numpy
# -----------------------------------------------------------------------------

import cv2
import argparse
import numpy as np

def parse_args():
    parser = argparse.ArgumentParser(description="Camera Preview + Document Quad Detection")
    parser.add_argument("cam_id", type=int, nargs="?", default=0, help="Camera ID to use (default: 0)")
    return parser.parse_args()

# --- ここから追加: 台形検出ユーティリティ -------------------------------------

def _order_corners(pts: np.ndarray) -> np.ndarray:
    """
    4点を (tl, tr, br, bl) 順に並べ替える。
    pts: shape (4,2)
    """
    pts = np.asarray(pts, dtype=np.float32)
    s = pts.sum(axis=1)      # x+y
    d = np.diff(pts, axis=1) # x-y
    tl = pts[np.argmin(s)]
    br = pts[np.argmax(s)]
    tr = pts[np.argmin(d)]
    bl = pts[np.argmax(d)]
    return np.array([tl, tr, br, bl], dtype=np.float32)

def _is_rect_like(ordered4: np.ndarray, cos_thresh: float = 0.3) -> bool:
    """
    長方形らしさの簡易チェック: 隣接2辺がほぼ直交（cos≈0）。
    cos_thresh を小さくすると厳しくなる。
    """
    tl, tr, br, bl = ordered4
    v1 = tr - tl  # 水平
    v2 = bl - tl  # 垂直
    cos = abs(np.dot(v1, v2) / (np.linalg.norm(v1) * np.linalg.norm(v2) + 1e-6))
    return cos < cos_thresh

def find_document_quad(frame: np.ndarray, min_area_ratio: float = 0.10) -> np.ndarray | None:
    """
    入力フレームから紙（四角形）候補を1つ返す。
    返り値: shape(4,2) float32 (tl,tr,br,bl) or None
    """
    h, w = frame.shape[:2]
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray = cv2.GaussianBlur(gray, (5, 5), 0)

    edges = cv2.Canny(gray, 60, 160)
    contours, _ = cv2.findContours(edges, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    if not contours:
        return None

    min_area = (h * w) * min_area_ratio
    # 面積上位から10個程度をチェック
    for cnt in sorted(contours, key=cv2.contourArea, reverse=True)[:10]:
        area = cv2.contourArea(cnt)
        if area < min_area:
            continue

        peri = cv2.arcLength(cnt, True)
        approx = cv2.approxPolyDP(cnt, 0.02 * peri, True)  # 2% 近似
        if len(approx) != 4:
            continue
        if not cv2.isContourConvex(approx):
            continue

        ordered = _order_corners(approx.reshape(4, 2))
        if _is_rect_like(ordered):
            return ordered

    return None

def warp_perspective(frame: np.ndarray, corners: np.ndarray, min_size: int = 200) -> np.ndarray:
    """
    4隅 (tl,tr,br,bl) から透視変換。出力サイズは4辺長から概算。
    min_size は小さすぎる出力を避けるための下限。
    """
    tl, tr, br, bl = corners

    widthA = np.linalg.norm(br - bl)
    widthB = np.linalg.norm(tr - tl)
    heightA = np.linalg.norm(tr - br)
    heightB = np.linalg.norm(tl - bl)
    dstW = int(max(widthA, widthB))
    dstH = int(max(heightA, heightB))
    dstW = max(dstW, min_size)
    dstH = max(dstH, min_size)

    src = np.array([tl, tr, br, bl], dtype=np.float32)
    dst = np.array([[0, 0],
                    [dstW - 1, 0],
                    [dstW - 1, dstH - 1],
                    [0, dstH - 1]], dtype=np.float32)

    M = cv2.getPerspectiveTransform(src, dst)
    return cv2.warpPerspective(frame, M, (dstW, dstH))

# --- 追加ここまで -------------------------------------------------------------

def main():
    args = parse_args()

    cam = cv2.VideoCapture(args.cam_id)  # 解像度は変更しない（既存方針）
    if not cam.isOpened():
        print("Cannot open camera")
        return

    # 可変ウィンドウで見やすく
    cv2.namedWindow('Camera Preview', cv2.WINDOW_NORMAL)
    cv2.namedWindow('Scanned (preview)', cv2.WINDOW_NORMAL)

    last_corners = None  # 検出が切れても1フレーム限定で補完するための簡易バッファ

    try:
        while True:
            ret, frame = cam.read()
            if not ret:
                print("Can't receive frame (stream end?). Exiting ...")
                break

            # --- ここから追加: 台形検出 & 透視変換プレビュー -------------------
            corners = find_document_quad(frame, min_area_ratio=0.10)
            has_doc = corners is not None

            if has_doc:
                last_corners = corners
            elif last_corners is not None:
                # 直前の結果で 1 回だけプレビュー継続（チラつき軽減）
                corners = last_corners
                has_doc = True
                last_corners = None

            if has_doc:
                # 入力側に輪郭オーバレイ
                ipts = corners.astype(int)
                cv2.polylines(frame, [ipts], True, (0, 255, 0), 2)
                for i, p in enumerate(ipts):
                    cv2.circle(frame, p, 5, (0, 200, 255), -1)
                    cv2.putText(frame, str(i), p + 5, cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 200, 255), 2)

                # 透視変換して別ウィンドウで表示
                scanned = warp_perspective(frame, corners)
                cv2.imshow('Scanned (preview)', scanned)
            else:
                # 未検出時は空表示
                blank = np.zeros((360, 480, 3), dtype=np.uint8)
                cv2.putText(blank, "No document", (20, 60), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (180, 180, 180), 2)
                cv2.imshow('Scanned (preview)', blank)
            # --- 追加ここまで ---------------------------------------------------

            cv2.imshow('Camera Preview', frame)

            k = cv2.waitKey(1) & 0xFF
            if k in (27, ord('q')):  # ESC or q
                break

    finally:
        cam.release()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
