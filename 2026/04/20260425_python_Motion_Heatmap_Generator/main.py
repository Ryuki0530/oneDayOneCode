import cv2
import numpy as np

CAMERA_INDEX = 1
THRESHOLD = 30


def main() -> None:
	cap = cv2.VideoCapture(CAMERA_INDEX)
	if not cap.isOpened():
		return

	prev_gray = None
	motion_accum = None
	last_frame = None

	while True:
		ok, frame = cap.read()
		if not ok:
			break
		last_frame = frame.copy()
		gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

		if prev_gray is None:
			prev_gray = gray
			motion_accum = np.zeros_like(gray, dtype=np.float32)
			continue

		diff = cv2.absdiff(prev_gray, gray)
		_, mask = cv2.threshold(diff, THRESHOLD, 255, cv2.THRESH_BINARY)
		motion_accum += mask.astype(np.float32)
		prev_gray = gray

		cv2.imshow("diff", mask)
		if cv2.waitKey(1) & 0xFF == ord("q"):
			break

	cap.release()
	cv2.destroyAllWindows()

	if motion_accum is None or last_frame is None:
		return

	heatmap_gray = cv2.normalize(motion_accum, None, 0, 255, cv2.NORM_MINMAX).astype(np.uint8)
	heatmap_color = cv2.applyColorMap(heatmap_gray, cv2.COLORMAP_JET)
	overlay = cv2.addWeighted(last_frame, 0.6, heatmap_color, 0.4, 0)

	cv2.imwrite("motion_heatmap.png", heatmap_color)
	cv2.imwrite("last_frame.png", last_frame)
	cv2.imwrite("overlay_heatmap.png", overlay)


if __name__ == "__main__":
	main()
