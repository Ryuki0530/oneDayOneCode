import cv2
import argparse
import numpy as np

def find_document_quad(frame :cv2.Mat, min_area_ratio: float = 0.1):
    h,w = frame.shape[:2]
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    cv2.imshow('Gray', gray_frame)
    gaussian = cv2.GaussianBlur(gray_frame, (5,5), 0)
    cv2.imshow('Blurred', gaussian)

    edges = cv2.Canny(gaussian, 60, 160)
    cv2.imshow('Edges', edges) 
    
    edge_overlay = frame.copy()
    edge_mask = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)
    edge_overlay = cv2.addWeighted(edge_overlay, 0.7, edge_mask, 0.3, 0)
    cv2.imshow('Edge Overlay', edge_overlay)

    contours, _ = cv2.findContours(edges, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    if not contours:
        return None
    
    min_area = (h * w) * min_area_ratio
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

def _is_rect_like(ordered4: np.ndarray, cos_thresh: float = 0.3) -> bool:
    tl, tr, br, bl = ordered4
    v1 = tr - tl
    v2 = bl - tl
    cos = abs(np.dot(v1, v2) / (np.linalg.norm(v1) * np.linalg.norm(v2) + 1e-6))
    return cos < cos_thresh


def _order_corners(pts: np.ndarray) -> np.ndarray:

    pts = np.asarray(pts, dtype=np.float32)
    s = pts.sum(axis=1)
    d = np.diff(pts, axis=1)
    tl = pts[np.argmin(s)]
    br = pts[np.argmax(s)]
    tr = pts[np.argmin(d)]
    bl = pts[np.argmax(d)]
    return np.array([tl, tr, br, bl], dtype=np.float32)

def parse_args():
    parser = argparse.ArgumentParser(description="Camera Preview")
    parser.add_argument("cam_id", type=int, nargs="?", default=0, help="Camera ID to use (default: 0)")
    return parser.parse_args()

def main():
    args = parse_args()

    cam = cv2.VideoCapture(args.cam_id)

    if not cam.isOpened():
        print("Cannot open camera")
        return
    last_corners = None
    try:
        while True:
            ret, frame = cam.read()
            if not ret:
                print("Can't receive frame (stream end?). Exiting ...")
                break
            cv2.imshow('Camera Preview', frame)

            corners = find_document_quad(frame)
            has_doc = corners is not None
            
            if has_doc:
                last_corners = corners
            elif last_corners is not None:
                corners = last_corners
                has_doc = True
                last_corners = None

           

            k = cv2.waitKey(1) & 0xFF
            if k in (27, ord('q')):
                break
    
    finally:
        cam.release()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    main()