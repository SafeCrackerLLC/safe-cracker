#ifndef FEEDBACK_H
#define FEEDBACK_H

/**
 * @file Feedback.h
 * @brief Zajednicki zvucni i vibracijski feedback za igru.
 */

#include <Arduino.h>

#include "Config.h"

/**
 * @brief Pusti ton i istovremeno aktivira vibracijski motor.
 * @param frequency Frekvencija tona u Hz.
 * @param durationMs Trajanje tona u milisekundama.
 */
void beepWithVibration(int frequency, int durationMs);
/** @brief Kratki feedback za pozitivni/desni pomak. */
void clickRight();
/** @brief Kratki feedback za negativni/lijevi pomak. */
void clickLeft();
/** @brief Melodija koja oznacava uspjesno rijesen level. */
void playVictorySound();
/** @brief Melodija koja oznacava neuspjesan zavrsetak levela. */
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
