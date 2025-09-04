import cv2
import dlib
import numpy as np
import time

PREDICTOR_PATH = "shape_predictor_68_face_landmarks.dat"  # 別途ダウンロード必要

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(PREDICTOR_PATH)

def calc_ear(eye):
    A = np.linalg.norm(eye[1] - eye[5])
    B = np.linalg.norm(eye[2] - eye[4])
    C = np.linalg.norm(eye[0] - eye[3])
    return (A + B) / (2.0 * C)

LEFT_EYE_IDX = [36, 37, 38, 39, 40, 41]
RIGHT_EYE_IDX = [42, 43, 44, 45, 46, 47]

class SleepDetector:
    def __init__(self, args):
        self.args = args
        self.cap = cv2.VideoCapture(args.device)
        if not self.cap.isOpened():
            raise RuntimeError("カメラが開けません")
        self.ear_thresh = 0.20  # EAR閾値
        self.closed_sec_thresh = 2.0  # 連続閉眼秒数
        self.frame_rate = 10  # 1秒あたりのフレーム数
        self.last_frames = []  # (timestamp, EAR)の履歴

    def detect(self):
        # 直近2秒間のフレームを取得し、EARを計算
        frames = []
        start = time.time()
        while time.time() - start < self.closed_sec_thresh:
            ret, frame = self.cap.read()
            if not ret:
                return False, "カメラ取得失敗"
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            faces = detector(gray)
            ear = 1.0  # デフォルトは目が開いているとみなす
            if faces:
                shape = predictor(gray, faces[0])
                coords = np.array([[p.x, p.y] for p in shape.parts()])
                left_eye = coords[LEFT_EYE_IDX]
                right_eye = coords[RIGHT_EYE_IDX]
                left_ear = calc_ear(left_eye)
                right_ear = calc_ear(right_eye)
                ear = (left_ear + right_ear) / 2.0
                frames.append(ear)
                if self.args.view:
                    # デバッグ表示
                    for idx in LEFT_EYE_IDX + RIGHT_EYE_IDX:
                        pt = (coords[idx][0], coords[idx][1])
                        cv2.circle(frame, pt, 2, (0,255,0), -1)
                    cv2.putText(frame, f'EAR: {ear:.2f}', (10,30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,0,255), 2)
                    cv2.imshow("Debug", frame)
                    if cv2.waitKey(1) & 0xFF == 27:
                        break
            else:
                frames.append(1.0)  # 顔未検出時は目が開いている扱い
            time.sleep(1.0 / self.frame_rate)
        # EARが閾値未満の割合
        closed = [ear < self.ear_thresh for ear in frames]
        closed_ratio = sum(closed) / len(closed) if frames else 0
        closed_sec = closed_ratio * self.closed_sec_thresh
        if closed_sec >= self.closed_sec_thresh * 0.9:  # ほぼ全フレーム閉眼
            info = f"EAR={np.mean(frames):.2f} / 連続{closed_sec:.1f}s"
            return True, info
        else:
            return False, ""

    def release(self):
        if hasattr(self, "cap") and self.cap:
            self.cap.release()
        if self.args.view:
            cv2.destroyAllWindows()