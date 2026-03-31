// Left motors
int IN1 = 27;
int IN2 = 26;
int ENA = 14;

// Right motors
int IN3 = 25;
int IN4 = 33;
int ENB = 32;

// Button
int BUTTON = 13;

// Ultrasonic sensors
int TRIG_F  = 5;  int ECHO_F  = 4;   
int TRIG_FL = 22;  int ECHO_FL = 23;  
int TRIG_FR = 18;  int ECHO_FR = 19;  

int lastState = HIGH;
int currentState;
bool running = false;

// Avoidance state
enum DriveState { DRIVING, TURNING_LEFT, TURNING_RIGHT, BACKING_UP };
DriveState driveState = DRIVING;
unsigned long maneuverStart = 0;

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  digitalWrite(ENA, LOW); digitalWrite(ENB, LOW);
}

void driveForward() {
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);  
}

void driveBackward() {
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);   
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);   
}

void turnLeft() {
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);   
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);  
}

void turnRight() {
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);   
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 60000); 
  if (duration == 0) return 999; 
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(TRIG_F,  OUTPUT); pinMode(ECHO_F,  INPUT);
  pinMode(TRIG_FL, OUTPUT); pinMode(ECHO_FL, INPUT);
  pinMode(TRIG_FR, OUTPUT); pinMode(ECHO_FR, INPUT);
}

void loop() {
  currentState = digitalRead(BUTTON);

  if (lastState == LOW && currentState == HIGH) {
    if (!running) {
      Serial.println("Starting!");
      running = true;
      driveState = DRIVING;
      driveForward();
    } else {
      Serial.println("Stopping!");
      running = false;
      driveState = DRIVING;
      stopMotors();
    }
  }
  lastState = currentState;

  if (!running) return;

  long distF  = getDistance(TRIG_F,  ECHO_F);
  long distFL = getDistance(TRIG_FL, ECHO_FL);
  long distFR = getDistance(TRIG_FR, ECHO_FR);
  unsigned long elapsed = millis() - maneuverStart;

  Serial.print("F:"); Serial.print(distF);
  Serial.print(" FL:"); Serial.print(distFL);
  Serial.print(" FR:"); Serial.println(distFR);

  if (driveState == DRIVING) {
    if (distF < 25 || distFL < 25 || distFR < 25) {
      maneuverStart = millis();
      driveState = BACKING_UP;
      driveBackward();
      Serial.println("Obstacle! Backing up...");
    }

  } else if (driveState == BACKING_UP && elapsed >= 600) {
    maneuverStart = millis();
    if (distFR > distFL) {
      driveState = TURNING_RIGHT;
      turnRight();
      Serial.println("Turning right");
    } else {
      driveState = TURNING_LEFT;
      turnLeft();
      Serial.println("Turning left");
    }

  } else if ((driveState == TURNING_LEFT || driveState == TURNING_RIGHT) && elapsed >= 500) {
    if (distF > 25) {
      driveState = DRIVING;
      driveForward();
      Serial.println("Path clear, moving forward");
    } else {
      maneuverStart = millis();
    }
  }
}
