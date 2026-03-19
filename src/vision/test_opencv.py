import cv2

# VideoCapture object
# 0 = default camera(laptop camera)
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Camera disconnected.")

else:
    while True:
        ret, img = cap.read()
        
        if not ret:
            break
        cv2.imshow('test', img)
        if cv2.waitKey(30) & 0xff == ord('q'):
            break
    
    cap.release()
    cv2.destroyAllWindows()

    
#release camera, and close all opencv windows
cap.release()
cv2.destroyAllWindows()