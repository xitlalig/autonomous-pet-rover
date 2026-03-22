import cv2
import time
from ultralytics import YOLO

cap = cv2.VideoCapture(0)
model = YOLO("yolov8n.pt")
dog_counter = 0
last_trigger_time = 0
cooldown = 10

while True:
    ret, frame = cap.read()
    results = model(frame)
    dog_detected = False

    for result in results:
        for box in result.boxes:
            cls = int(box.cls[0])
            label = model.names[cls]

            if label == "dog":
                dog_detected = True
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                box_area = (x2 - x1) * (y2 - y1)
                if box_area > 50000:
                    print("DOG IS CLOSE!!!")
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                cv2.putText(frame, "DOG", (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)
    
    if dog_detected:
        dog_counter += 1
    else:
        dog_counter = 0
    
    current_time = time.time()
    if dog_detected and dog_counter > 5:
        cv2.putText(frame, "DOG CONFIRMED", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 3)

        if current_time - last_trigger_time > cooldown:
            print("Dispense Treat!!!")
            last_trigger_time = current_time

    cv2.imshow("DOG DETECT", frame)
    if cv2.waitKey(1) == ord('q'):
        break
    
cap.release()
cv2.destroyAllWindows()