void dispense() {
  long dispenseEndTime = millis() + dispenseTimeout;
  if (!motorJammed) {
    while (digitalRead(switchPin) == LOW) { // initial movement to get past switch point
      advanceMotor(millis(), debounce);
      if (millis() > dispenseEndTime) {
        jam();
        break;
      };
    };
    resetMotor();
    if (!motorJammed) mqtt.publish("/pets/feed/cats", "fed");
  } else jam();
}

void advanceMotor(long startTime, int runTime) {
  while (startTime + runTime > millis()) digitalWrite(motorPin, HIGH);
  digitalWrite(motorPin, LOW);
}

void jam() {
  digitalWrite(motorPin, LOW);
  mqtt.publish("/pets/feed/cats", "jam");
  motorJammed = true;
}

void resetMotor() {
  long dispenseEndTime = millis() + dispenseTimeout;
  while (digitalRead(switchPin) == HIGH) {
    advanceMotor(millis(), debounce);
    if (millis() > dispenseEndTime) {
      jam();
      break;
    };
  };
}