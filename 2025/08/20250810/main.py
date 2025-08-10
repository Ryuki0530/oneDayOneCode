import cv2
import argparse
import mediapipe as mp
from PIL import Image, ImageDraw, ImageFont
import os
from putTextJPforOpenCV import putTextJP

class MediaPipeHandDetector:
    def __init__(self):
        self.hands = mp.solutions.hands.Hands(
            static_image_mode=False,
            max_num_hands=2,
            min_detection_confidence=0.5,
            min_tracking_confidence=0.5
        )
        pass

    def hand_detection(self, frame):
        results = self.hands.process(frame)
        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                mp.solutions.drawing_utils.draw_landmarks(frame, hand_landmarks, mp.solutions.hands.HAND_CONNECTIONS)
        return frame
    
    def get_hand_gesture(self, frame):
        results = self.hands.process(frame)
        
        # 手の位置を描画
        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                mp.solutions.drawing_utils.draw_landmarks(frame, hand_landmarks, mp.solutions.hands.HAND_CONNECTIONS)
        
        if not results.multi_hand_landmarks:
            return None, frame

        gestures = []
        for i, hand_landmarks in enumerate(results.multi_hand_landmarks):
            # 指先と付け根のランドマークインデックス
            tips_ids = [4, 8, 12, 16, 20]
            fingers = []

            # 親指
            if hand_landmarks.landmark[tips_ids[0]].x < hand_landmarks.landmark[tips_ids[0] - 1].x:
                fingers.append(1)
            else:
                fingers.append(0)

            # 他の指
            for j in range(1, 5):
                if hand_landmarks.landmark[tips_ids[j]].y < hand_landmarks.landmark[tips_ids[j] - 2].y:
                    fingers.append(1)
                else:
                    fingers.append(0)

            total_fingers = sum(fingers)

            if total_fingers == 0:
                gesture = "グー"
            elif total_fingers == 2 and fingers[1] and fingers[2] and not fingers[3] and not fingers[4]:
                gesture = "チョキ"
            elif total_fingers == 5:
                gesture = "パー"
            else:
                gesture = "不明"

            gestures.append(gesture)
            
            # 手首の位置を取得してテキストを描画
            h, w, c = frame.shape
            wrist = hand_landmarks.landmark[0]  # 手首のランドマーク
            x = int(wrist.x * w)
            y = int(wrist.y * h)
            
            # 背景となる四角形を描画（テキストの可読性向上）
            text_size = cv2.getTextSize(gesture, cv2.FONT_HERSHEY_SIMPLEX, 1, 2)[0]
            cv2.rectangle(frame, (x - 10, y - text_size[1] - 20), 
                         (x + text_size[0] + 10, y - 5), (0, 0, 0), -1)
            
            # ジェスチャーテキストを描画
            frame = putTextJP(
                frame,
                gesture,
                (x, y - 10),        # orgは左下基準（OpenCV互換）
                1.0,                # font_scale
                (0, 255, 0),        # color (BGR)
                2,                  # thickness
                cv2.LINE_AA,        # lineType(未使用だが互換のため受ける)
                font_path="C:\\Windows\\Fonts\\meiryo.ttc",
                bg_color=(0, 0, 0), # 任意：背景を付けたい場合
                padding=6
            )

        
        return gestures, frame

class MainClass:
    def __init__(self):
        self.hand_detector = MediaPipeHandDetector()
        self.debug = False

    def root(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("--debug", action="store_true", help="Enable debug mode")
        parser.add_argument("--cam", type=int, default=0, help="Camera index (default: 0)")
        args = parser.parse_args()

        if args.debug:
            self.debug = args.debug
            print("Debug mode is enabled")
        
        # カメラを開く（引数名を修正）
        cap = cv2.VideoCapture(args.cam)
        if not cap.isOpened():
            print("Error: Could not open camera.")
            return

        # メインループを呼び出し
        self.main_loop(cap)

    def main_loop(self, cap):
        while True:
            ret, frame = cap.read()
            if not ret:
                print("Error: Could not read frame.")
                break

            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            # frame = self.hand_detector.hand_detection(frame)
            gestures, frame = self.hand_detector.get_hand_gesture(frame)
            frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)


            if self.debug:
                print("Frame processed with hand detection.")
            
            # フレームを表示
            cv2.imshow("Camera Feed", frame)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        cap.release()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    main = MainClass()
    main.root()


