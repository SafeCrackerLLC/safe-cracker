#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

#include "Config.h"
#include "GameLogic.h"
#include "Level.h"
#include "Power.h"

void handleButtonActivity();
void calibrateJoystickCenter();
void handleJoystickLevelSelect();

void calibrateJoystickCenter() {
  long total = 0;

  for (int i = 0; i < JOYSTICK_CENTER_SAMPLE_COUNT; i++) {
    total += analogRead(JOYSTICK_Y_PIN);
    delay(2);
  }

  joystickCenterValue = total / JOYSTICK_CENTER_SAMPLE_COUNT;
  joystickMenuReady = true;
}

void handleJoystickLevelSelect() {
  if (gameScreen != GAME_SCREEN_LEVEL_SELECT) {
    return;
  }

  int joystickY = analogRead(JOYSTICK_Y_PIN);
  int distanceFromCenter = joystickY - joystickCenterValue;

  if (abs(distanceFromCenter) <= JOYSTICK_DEADZONE) {
    joystickMenuReady = true;
    return;
  }

  if (!joystickMenuReady) {
    return;
  }

  if (millis() - lastJoystickMoveTime < JOYSTICK_MENU_REPEAT_MS) {
    return;
  }

  if (distanceFromCenter > 0) {
    moveSelectedLevel(1);
  } else {
    moveSelectedLevel(-1);
  }

  lastJoystickMoveTime = millis();
  joystickMenuReady = false;
  resetSleepTimer();
}

void handleButtonActivity() {
  handleJoystickLevelSelect();

  if (digitalRead(BUTTON_PIN) == LOW) {
    resetSleepTimer();

    if (gameScreen == GAME_SCREEN_LEVEL_SELECT) {
      startSelectedLevel();
      delay(250);
      return;
    }

    if (gameScreen == GAME_SCREEN_FINISHED) {
      showLevelSelect();
      delay(250);
      return;
    }

    delay(50);
  }
}

#endif
