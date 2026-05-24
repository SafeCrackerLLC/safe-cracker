#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <Arduino.h>

#include "Config.h"

void beepWithVibration(int frequency, int durationMs);
void clickRight();
void clickLeft();
void playVictorySound();
void playLoseSound();

void beepWithVibration(int frequency, int durationMs) {
  digitalWrite(VIBRATION_PIN, HIGH);
  tone(BUZZER_PIN, frequency, durationMs);
  delay(durationMs);
  noTone(BUZZER_PIN);

  if (durationMs < MIN_VIBRATION_PULSE_MS) {
    delay(MIN_VIBRATION_PULSE_MS - durationMs);
  }

  digitalWrite(VIBRATION_PIN, LOW);
}

void clickRight() {
  beepWithVibration(600, CLICK_VIBRATION_MS);
}

void clickLeft() {
  beepWithVibration(300, CLICK_VIBRATION_MS);
}

void playVictorySound() {
  beepWithVibration(660, 70);
  delay(30);
  beepWithVibration(880, 80);
  delay(30);
  beepWithVibration(1040, 110);
}

void playLoseSound() {
  beepWithVibration(330, 90);
  delay(40);
  beepWithVibration(220, 140);
}

#endif
