#ifndef INPUT_H
#define INPUT_H

/**
 * @file Input.h
 * @brief Obrada tipke i joysticka za navigaciju kroz ekrane.
 */

#include <Arduino.h>

#include "Config.h"
#include "GameLogic.h"
#include "Level.h"
#include "SafeNetwork.h"
#include "Power.h"

/** @brief Obradjuje joystick i tipku za trenutno aktivni ekran. */
void handleButtonActivity();
/** @brief Uzorkuje joystick i sprema njegovu neutralnu vrijednost. */
void calibrateJoystickCenter();
/** @brief Pomice selekciju u glavnom meniju kada je joystick izvan deadzonea. */
void handleJoystickLevelSelect();
/** @brief Prebacuje akciju na ekranu s mreznim informacijama. */
void handleJoystickNetworkInfo();
/** @return Ukupan broj stavki u glavnom meniju. */
int getMenuItemCount();

int getMenuItemCount() {
  return LEVEL_COUNT + 2;
}

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

void handleJoystickNetworkInfo() {
  if (gameScreen != GAME_SCREEN_NETWORK_INFO) {
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

  selectedNetworkInfoAction = selectedNetworkInfoAction == 0 ? 1 : 0;
  lastJoystickMoveTime = millis();
  joystickMenuReady = false;
  clickRight();
  resetSleepTimer();
}

void handleButtonActivity() {
  handleJoystickLevelSelect();
  handleJoystickNetworkInfo();

  if (digitalRead(BUTTON_PIN) == LOW) {
    resetSleepTimer();

    if (gameScreen == GAME_SCREEN_LEVEL_SELECT) {
      if (selectedLevelIndex < LEVEL_COUNT) {
        startSelectedLevel();
      } else if (selectedLevelIndex == LEVEL_COUNT) {
        selectedNetworkInfoAction = 0;
        gameScreen = GAME_SCREEN_NETWORK_INFO;
      } else {
        setWifiOfflineMode(!wifiOfflineMode);
      }

      delay(250);
      return;
    }

    if (gameScreen == GAME_SCREEN_WIFI_CONFIG) {
      setWifiOfflineMode(true);
      gameScreen = GAME_SCREEN_LEVEL_SELECT;
      delay(250);
      return;
    }

    if (gameScreen == GAME_SCREEN_NETWORK_INFO) {
      if (selectedNetworkInfoAction == 0) {
        gameScreen = GAME_SCREEN_LEVEL_SELECT;
      } else {
        startWifiConfigPortal(false);
      }

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
