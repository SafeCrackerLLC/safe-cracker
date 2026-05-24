#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <Arduino.h>

#include "Config.h"
#include "Feedback.h"
#include "Level.h"
#include "Power.h"
#include "State.h"

void startLevel(const Level& level);
void startSelectedLevel();
void showLevelSelect();
void finishLevel(bool wasWon);
const Level& getCurrentLevel();
const Level& getSelectedLevel();
const LevelTarget& getCurrentTarget();
bool isLevelComplete();
bool isLevelOutOfTime();
int getTargetsHit();
int getTotalTargets();
int getRemainingSeconds();
const char* getBreathMessage();
float getGameProgress();
float getLevelProgress();
float getTargetFillAmount();
float getTargetProgress();
const char* getTargetMessage();
int getBreathTextMoveOffset();
int getTargetArrowY();
int getTargetX();
bool isCurrentTargetHit();
void moveSelectedLevel(int direction);
void updateLevelTimer();
void updateTargetProgress();
void updateTurnBarFromPot(int potValue);
void updateVaultRotationFromPot();

const Level* currentLevel = &LEVELS[0];

void startLevel(const Level& level) {
  currentLevel = &level;
  vaultAngle = 0;
  barFill = 0;
  lastValue = analogRead(POT_PIN);
  currentTargetIndex = 0;
  levelWasWon = false;
  levelFinishedTime = 0;
  targetHitStartTime = 0;
  gameStartTime = millis();
  gameScreen = GAME_SCREEN_PLAYING;
}

void startSelectedLevel() {
  startLevel(getSelectedLevel());
}

void showLevelSelect() {
  gameScreen = GAME_SCREEN_LEVEL_SELECT;
  levelWasWon = false;
  levelFinishedTime = 0;
  targetHitStartTime = 0;
}

void finishLevel(bool wasWon) {
  if (gameScreen != GAME_SCREEN_PLAYING) {
    return;
  }

  levelWasWon = wasWon;
  levelFinishedTime = millis();
  gameScreen = GAME_SCREEN_FINISHED;

  if (levelWasWon) {
    playVictorySound();
  } else {
    playLoseSound();
  }
}

const Level& getCurrentLevel() {
  return *currentLevel;
}

const Level& getSelectedLevel() {
  return LEVELS[selectedLevelIndex];
}

const LevelTarget& getCurrentTarget() {
  return getCurrentLevel().targets[currentTargetIndex];
}

bool isLevelComplete() {
  return currentTargetIndex >= getCurrentLevel().targetCount;
}

bool isLevelOutOfTime() {
  unsigned long elapsedTime = millis() - gameStartTime;
  return elapsedTime >= getCurrentLevel().durationMs;
}

int getTargetsHit() {
  if (currentTargetIndex > getCurrentLevel().targetCount) {
    return getCurrentLevel().targetCount;
  }

  return currentTargetIndex;
}

int getTotalTargets() {
  return getCurrentLevel().targetCount;
}

int getRemainingSeconds() {
  unsigned long timeNow = levelFinishedTime > 0 ? levelFinishedTime : millis();
  unsigned long elapsedTime = timeNow - gameStartTime;
  unsigned long durationMs = getCurrentLevel().durationMs;

  if (elapsedTime >= durationMs) {
    return 0;
  }

  return (durationMs - elapsedTime + 999) / 1000;
}

const char* getBreathMessage() {
  if (isLevelComplete()) {
    return "DONE";
  }

  if (isLevelOutOfTime()) {
    return "TIME";
  }

  if (getTargetFillAmount() < 0) {
    return "INHALE";
  }

  return "EXHALE";
}

float getGameProgress() {
  unsigned long elapsedTime = millis() - gameStartTime;
  unsigned long durationMs = getCurrentLevel().durationMs;

  if (elapsedTime >= durationMs) {
    return 1.0;
  }

  return (float)elapsedTime / durationMs;
}

float getLevelProgress() {
  int targetCount = getCurrentLevel().targetCount;

  if (targetCount <= 0) {
    return 1.0;
  }

  return constrain((float)currentTargetIndex / targetCount, 0.0, 1.0);
}

float getTargetFillAmount() {
  if (isLevelComplete()) {
    return 0;
  }

  return constrain(getCurrentTarget().fillAmount, -1.0, 1.0);
}

float getTargetProgress() {
  float targetFill = getTargetFillAmount();

  if (targetFill == 0) {
    return 1.0;
  }

  if ((targetFill > 0 && barFill < 0) || (targetFill < 0 && barFill > 0)) {
    return 0;
  }

  return constrain(abs(barFill) / abs(targetFill), 0.0, 1.0);
}

const char* getTargetMessage() {
  if (isLevelComplete()) {
    return "Done!";
  }

  if (isLevelOutOfTime()) {
    return "Time!";
  }

  float targetProgress = getTargetProgress();

  if (isCurrentTargetHit()) {
    return "Hold!";
  }

  if (targetProgress >= 0.75) {
    return "Close!";
  }

  if (targetProgress >= 0.5) {
    return "More!";
  }

  return "";
}

int getBreathTextMoveOffset() {
  int phaseTime = millis() % BREATH_BOB_CYCLE_MS;

  if (phaseTime < BREATH_BOB_STEP_MS) {
    return 1;
  }

  return 0;
}

int getTargetArrowY() {
  int phaseTime = millis() % TARGET_ARROW_CYCLE_MS;

  if (phaseTime < TARGET_ARROW_HOLD_MS) {
    return TARGET_ARROW_TOUCH_Y;
  }

  return TARGET_ARROW_Y;
}

int getTargetX() {
  float halfBarWidth = (BAR_WIDTH / 2.0) - 2;
  return BAR_CENTER_X + getTargetFillAmount() * halfBarWidth;
}

bool isCurrentTargetHit() {
  if (isLevelComplete() || isLevelOutOfTime()) {
    return false;
  }

  float targetFill = getTargetFillAmount();

  if (targetFill > 0) {
    return barFill >= targetFill - TARGET_HIT_TOLERANCE &&
      barFill <= targetFill + TARGET_OVERSHOOT_ALLOWANCE;
  }

  if (targetFill < 0) {
    return barFill <= targetFill + TARGET_HIT_TOLERANCE &&
      barFill >= targetFill - TARGET_OVERSHOOT_ALLOWANCE;
  }

  return abs(barFill) <= TARGET_HIT_TOLERANCE;
}

void updateTargetProgress() {
  if (isLevelComplete() || isLevelOutOfTime()) {
    return;
  }

  if (!isCurrentTargetHit()) {
    targetHitStartTime = 0;
    return;
  }

  if (targetHitStartTime == 0) {
    targetHitStartTime = millis();
    return;
  }

  if (millis() - targetHitStartTime < TARGET_HIT_HOLD_MS) {
    return;
  }

  currentTargetIndex++;
  targetHitStartTime = 0;

  if (isLevelComplete()) {
    finishLevel(true);
  } else {
    clickRight();
  }
}

void updateLevelTimer() {
  if (gameScreen != GAME_SCREEN_PLAYING) {
    return;
  }

  if (isLevelOutOfTime()) {
    finishLevel(false);
  }
}

void moveSelectedLevel(int direction) {
  if (LEVEL_COUNT <= 0) {
    return;
  }

  selectedLevelIndex += direction;

  if (selectedLevelIndex < 0) {
    selectedLevelIndex = LEVEL_COUNT - 1;
  } else if (selectedLevelIndex >= LEVEL_COUNT) {
    selectedLevelIndex = 0;
  }

  clickRight();
}

void updateTurnBarFromPot(int potValue) {
  barFill = map(potValue, POT_MIN_VALUE, POT_MAX_VALUE, -100, 100) / 100.0;
  barFill = constrain(barFill, -1.0, 1.0);
}

void updateVaultRotationFromPot() {
  int currentValue = analogRead(POT_PIN);
  int diff = currentValue - lastValue;

  updateTurnBarFromPot(currentValue);
  updateTargetProgress();

  if (gameScreen != GAME_SCREEN_PLAYING) {
    return;
  }

  if (abs(diff) <= POT_DEADZONE) {
    return;
  }

  vaultAngle += diff * ROTATION_SPEED;

  while (vaultAngle >= FULL_ROTATION) {
    vaultAngle -= FULL_ROTATION;
  }

  while (vaultAngle < 0) {
    vaultAngle += FULL_ROTATION;
  }

  if (diff > 0) {
    clickRight();
  } else {
    clickLeft();
  }

  lastValue = currentValue;
  resetSleepTimer();
}

#endif
