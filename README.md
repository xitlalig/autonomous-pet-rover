# autonomous-pet-rover
An AI-powered autonomous rover that navigates a home environment, detects household pets, and dispenses treats based on recognition.

## Features
- Autonomous navigation with obstacle avoidance
- Real-time pet detection using computer vision
- Treat dispensing mechanism

## System Architecture
- Raspberry Pi: vision + decision making
- Microcontroller(Arduino/ESP32): motor control
- Sensors: ultrasonic / ToF for obstacle avoidance

## Tech Stack
- Python, OpenCV
- Embedded C/MicroPython
- Raspberry Pi + Arduino/ESP32
- 3D Printing(CAD)

## Demo
Coming soon ...

## Hardware
- 4WD chassis kit
- Motor Driver(L298N)
- Ultrasonic Sensors
- Servo Motor (treat dispenser)
- Raspberry Pi + Camera

## Progress
(3/21/26) Using YOLO + filtering to only detect dogs
<img width="500" alt="Screenshot (16)" src="https://github.com/user-attachments/assets/93f6b546-f29e-4ac1-a13a-c9e5c6967f08" />

(3/26/26) Current chassis setup. Motor drives with button toggle!
<img width="500" alt="IMG_4772" src="https://github.com/user-attachments/assets/8651cf10-c078-48ba-ac59-904558e10914" />

(3/30/26) Setup update! Added 3 ultrasonic sensors, p.s. I ran into some issues with my sensors. Read more about it in [Challenges & Learning](#challenges--learning)

<img width="500" alt="IMG_4787" src="https://github.com/user-attachments/assets/dfa682d2-125b-49f9-b730-678866f90f13" />


(4/05/26) Currently working on integrating my raspberry pi into the project, as well as collecting data to start training my own ai model! Heres my first pic with my ArduCam and Raspberry Pi 4:
<img width="500" alt="test" src="https://github.com/user-attachments/assets/4c976bd2-0a4c-47fd-ba90-8d071d4338c2" />


## Challenges & Learning
**(3/30/36) Ultrasonic Sensor Problem**
- My initial issue came from all 3 sensors returning 999, which meant that none of my sensors were receiving an echo signal back.
- Originally I used a voltage divider with 2 10kΩ resistors, which should output 2.5V when hooked up to the VIN pin on the ESP32.
- 2.5V should work in theory, but Amazon reviews of the product suggested 3V minimum in order to get a reading.
- So I got rid of the voltage divider and hooked my ECHO pin directly to the 3V3 pin on the ESP32, and everything was resolved!
- **Solution:** Power the HC-SR04 VCC from the ESP32 3V3 pin instead of VIN. Since the sensor runs on 3.3V, the ECHO pin also outputs 3.3V which is safe for the ESP32 GPIO directly, no voltage divider needed. Final pins used: TRIG_F=12, ECHO_F=4, TRIG_FL=22, ECHO_FL=23, TRIG_FR=18, ECHO_FR=19.
