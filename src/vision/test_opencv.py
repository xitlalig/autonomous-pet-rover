import cv2
from ultralytics import YOLO

cap = cv2.VideoCapture(0)
model = YOLO("yolov8n.pt")
dog_counter = 0


while True:
    ret, frame = cap.read()
    results = model(frame)
    dog_detected = False

    for result in results:
        #goes through every detected object in this frame
        for box in result.boxes:
            #the class ID
            cls = int(box.cls[0])
            #model name = dog
            label = model.names[cls]

            #filter only dogs
            if label == "dog":
                dog_detected = True

                #get bounding box
                x1, y1, x2, y2 = map(int, box.xyxy[0])

                #draw rectangle
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)

                #add label
                cv2.putText(frame, "DOG", (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)
    
    if dog_detected:
        dog_counter += 1
    else:
        dog_counter = 0
    
    if dog_counter > 5:
        cv2.putText(frame, "DOG CONFIRMED", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 3)
    
    #display
    cv2.imshow("DOG DETECT", frame)
    if cv2.waitKey(1) == ord('q'):
        break
    
cap.release()
cv2.destroyAllWindows()