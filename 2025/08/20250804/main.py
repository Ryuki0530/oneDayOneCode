import cv2
import argparse



class face_mosaicer:
    def __init__(self, pixelation_level,debug=False):
        self.debug = debug
        self.pixelation_level = pixelation_level
        self.face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')


    def mosaic(self, image):
        
        h, w = image.shape[:2]
        temp = cv2.resize(image, (int(w * self.pixelation_level), int(h * self.pixelation_level)), interpolation=cv2.INTER_LINEAR)
        return cv2.resize(temp, (w, h), interpolation=cv2.INTER_NEAREST)

    def face_search(self, image):
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        faces = self.face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
        return faces

    def face_mosaic(self, image):
        faces = self.face_search(image)
        for (x, y, w, h) in faces:
            if self.debug:
                print(f"Face detected at x:{x}, y:{y}, w:{w}, h:{h}")
                cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), 2)
            face = image[y:y+h, x:x+w]
            if face.size > 0:
                image[y:y+h, x:x+w] = self.mosaic(face)
        return image

        
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--debug", action="store_true", help="Enable debug mode")
    parser.add_argument("--pixelation", type=float, default=0.1, help="Pixelation level")
    args = parser.parse_args()

    if args.debug:
        print("Debug mode is enabled")
        print(f"Pixelation level set to: {args.pixelation}")
    
    #open camera
    cap = cv2.VideoCapture(1)
    if not cap.isOpened():
        print("Error: Could not open camera.")
        return

    face_mosaic = face_mosaicer(args.pixelation, args.debug)

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error coulud not open camera.")
            break

        frame = face_mosaic.face_mosaic(frame)

        cv2.imshow("Camera Feed", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

if __name__ == "__main__":
    main()
