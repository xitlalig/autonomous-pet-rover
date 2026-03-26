//left
int IN1 = 27;
int IN2 = 26;
int ENA = 14;
//right
int IN3 = 25;
int IN4 = 33;
int ENB = 32;
//button
int BUTTON = 13;
int lastState = HIGH;
int currentState;

// State machine
enum CarState { IDLE, FORWARD, BACKWARD, STOPPED };
CarState carState = IDLE;
unsigned long stateStartTime = 0;
bool running = false;

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  currentState = digitalRead(BUTTON);

  // Detect button press (LOW to HIGH transition)
  if (lastState == LOW && currentState == HIGH) {
    if (!running) {
      // Start the sequence
      Serial.println("Button pressed, car moving!");
      running = true;
      carState = FORWARD;
      stateStartTime = millis();
      digitalWrite(ENA, HIGH);
      digitalWrite(ENB, HIGH);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else {
      // Stop immediately on second press
      Serial.println("Button pressed, stopping!");
      running = false;
      carState = IDLE;
      stopMotors();
    }
  }
  lastState = currentState;

  // Handle timed state transitions (only when running)
  if (running) {
    unsigned long elapsed = millis() - stateStartTime;

    if (carState == FORWARD && elapsed >= 3000) {
      // Switch to backward
      carState = BACKWARD;
      stateStartTime = millis();
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

    } else if (carState == BACKWARD && elapsed >= 3000) {
      // Switch to stopped pause
      carState = STOPPED;
      stateStartTime = millis();
      stopMotors();

    } else if (carState == STOPPED && elapsed >= 5000) {
      // Sequence done, go idle
      running = false;
      carState = IDLE;
      Serial.println("Sequence complete.");
    }
  }
}
