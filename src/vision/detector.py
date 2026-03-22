from ultralytics import YOLO

class DogDetector:
    def __init__(self):
        self.model = YOLO("yolov8n.pt")
    
    def detect(self, frame):
        results = self.model(frame)
        detections = []

        for result in results:
            for box in result.boxes:
                cls = int(box.cls[0])
                label = self.model.names[cls]

                if label == "dog":
                    x1, y1, x2, y2 = map(int, box.xyxy[0])
                    area = (x2 - x1) * (y2 - y1)

                    detections.append({"label" : label, "box" : (x1, y1, x2, y2), "area" : area})
        return detections