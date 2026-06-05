#ifndef POWER_H
#define POWER_H

/**
 * @file Power.h
 * @brief Upravljanje neaktivnoscu i ESP32 deep sleep nacinom rada.
 */

#include <Arduino.h>
#include <esp_timer.h>
#include <esp_sleep.h>

#include "Config.h"
#include "State.h"

/**
 * @brief Callback ESP timera koji oznacava da je sleep zatrazen.
 * @param arg Ne koristi se.
 */
void handleSleepTimerExpired(void* arg);
/** @brief Kreira i pokrece timer za neaktivnost. */
void initializeSleepTimer();
/** @brief Resetira vrijeme zadnje aktivnosti i ponovno armira sleep timer. */
void resetSleepTimer();
/** @brief Gasi OLED i prebacuje ESP32 u deep sleep do pritiska tipke. */
void goToSleep();
/** @brief Provjerava treba li uredjaj uspavati. */
void sleepIfInactive();

void handleSleepTimerExpired(void* arg) {
  sleepRequested = true;
}

void initializeSleepTimer() {
  esp_timer_create_args_t sleepTimerArgs = {};
  sleepTimerArgs.callback = &handleSleepTimerExpired;
  sleepTimerArgs.arg = NULL;
  sleepTimerArgs.dispatch_method = ESP_TIMER_TASK;
  sleepTimerArgs.name = "sleep_timer";

  esp_timer_create(&sleepTimerArgs, &sleepTimer);
  resetSleepTimer();
}

void resetSleepTimer() {
  lastActivityTime = millis();
  sleepRequested = false;

  if (sleepTimer == NULL) {
    return;
  }

  esp_timer_stop(sleepTimer);
  esp_timer_start_once(sleepTimer, SLEEP_TIMEOUT_MS * 1000ULL);
}

void goToSleep() {
  Serial.println("Going to sleep...");
  display.clearDisplay();
  display.display();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, LOW);
  esp_deep_sleep_start();
}

void sleepIfInactive() {
  if (wifiConfigPortalActive) {
    resetSleepTimer();
    return;
  }

  if (sleepRequested) {
    goToSleep();
  }
}

#endif
