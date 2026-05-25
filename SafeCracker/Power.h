#ifndef POWER_H
#define POWER_H

#include <Arduino.h>
#include <esp_timer.h>
#include <esp_sleep.h>

#include "Config.h"
#include "State.h"

void handleSleepTimerExpired(void* arg);
void initializeSleepTimer();
void resetSleepTimer();
void goToSleep();
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
