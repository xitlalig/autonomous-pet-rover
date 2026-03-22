import cv2
from vision.detector import DogDetector
from logic.decision import DogDecision

cap = cv2.VideoCapture(0)

detector = DogDetector()
decision = DogDecision()

while True:
    ret, frame = cap.read()
    if not ret:
        break

    detections = detector.detect(frame)
    action, dog_detected = decision.update(detections)

    for det in detections:
        x1, y1, x2, y2 = det["box"]
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
        cv2.putText(frame, "DOG", (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    
    if dog_detected:
        cv2.putText(frame, "DOG DETECTED", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    
    if action == "DISPENSE_TREAT":
        print("Dispense Treat!!!")
    
    cv2.imshow("DOG DETECT", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()