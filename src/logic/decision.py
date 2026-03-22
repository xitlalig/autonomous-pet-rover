import time

class DogDecision:
    def __init__(self, cooldown = 10):
        self.dog_counter = 0
        self.last_trigger_time = 0
        self.cooldown = cooldown
    
    def update(self, detections):
        dog_detected = len(detections) > 0
        is_close = False

        for det in detections:
            if det["area"] > 50000:
                is_close = True
        
        if dog_detected:
            self.dog_counter += 1
        else:
            self.dog_counter = 0
        
        current_time = time.time()
        action = None

        if dog_detected and self.dog_counter > 5:
            if is_close and (current_time - self.last_trigger_time > self.cooldown):
                action = "DISPENSE_TREAT"
                self.last_trigger_time = current_time
        
        return action, dog_detected