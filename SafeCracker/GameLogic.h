#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

/**
 * @file GameLogic.h
 * @brief Pravila levela, pogodak ciljeva, score, replay i napredak igre.
 */

#include <Arduino.h>

#include "Config.h"
#include "Feedback.h"
#include "Level.h"
#include "SafeNetwork.h"
#include "Power.h"
#include "State.h"

/** @brief Pokrece zadani level i resetira stanje trenutne partije. */
void startLevel(const Level& level);
/** @brief Pokrece trenutno odabrani level iz glavnog menija. */
void startSelectedLevel();
/** @brief Vraca igru na ekran za izbor levela. */
void showLevelSelect();
/**
 * @brief Zavrsava aktivni level, racuna score i po potrebi salje rezultat.
 * @param wasWon true ako su svi ciljevi pogodjeni prije isteka vremena.
 */
void finishLevel(bool wasWon);
/** @return Level koji se trenutno igra. */
const Level& getCurrentLevel();
/** @return Level koji je trenutno odabran u meniju. */
const Level& getSelectedLevel();
/** @return Cilj koji igrac trenutno treba pogoditi. */
const LevelTarget& getCurrentTarget();
/** @return true ako su pogodjeni svi ciljevi trenutnog levela. */
bool isLevelComplete();
/** @return true ako je isteklo vrijeme trenutnog levela. */
bool isLevelOutOfTime();
/** @return Broj pogodjenih ciljeva. */
int getTargetsHit();
/** @return Ukupan broj ciljeva u levelu. */
int getTotalTargets();
/** @return Preostale sekunde levela. */
int getRemainingSeconds();
/** @return Kratka poruka za smjer disanja/okretanja. */
const char* getBreathMessage();
/** @return Napredak vremena levela od 0.0 do 1.0. */
float getGameProgress();
/** @return Napredak kroz ciljeve levela od 0.0 do 1.0. */
float getLevelProgress();
/** @return Ciljana popunjenost trake za trenutni cilj. */
float getTargetFillAmount();
/** @return Relativni napredak prema trenutnom cilju. */
float getTargetProgress();
/** @return Poruka koja pomaze igracu pogoditi ili zadrzati cilj. */
const char* getTargetMessage();
/** @return Mali animacijski pomak teksta disanja. */
int getBreathTextMoveOffset();
/** @return Y koordinata animirane strelice cilja. */
int getTargetArrowY();
/** @return X koordinata trenutnog cilja na traci. */
int getTargetX();
/** @return true ako je trenutna popunjenost trake unutar tolerancije cilja. */
bool isCurrentTargetHit();
/**
 * @brief Pomice selekciju levela/menu stavke za zadani smjer.
 * @param direction Smjer pomaka, tipicno -1 za gore ili 1 za dolje.
 */
void moveSelectedLevel(int direction);
/** @brief Provjerava istek vremena levela. */
void updateLevelTimer();
/** @brief Obradjuje zadrzavanje na cilju i prelazak na iduci cilj. */
void updateTargetProgress();
/** @brief Resetira statistiku stabilnosti i replay podatke. */
void resetLevelStats();
/**
 * @brief Uzorkuje odstupanje od cilja za izracun stabilnosti.
 * @param potDiff Promjena ADC vrijednosti potenciometra od zadnjeg citanja.
 */
void recordLevelStability(int potDiff);
/**
 * @brief Sprema replay dogadjaj ako je dovoljno bitan ili forsiran.
 * @param eventType Tip dogadjaja, jedna od REPLAY_EVENT_* konstanti.
 * @param force true ako se dogadjaj sprema bez interval/delta filtriranja.
 */
void recordReplayEvent(int eventType, bool force);
/** @return Stabilnost cijelog levela od 0 do 100. */
int calculateLevelStability();
/**
 * @brief Racuna stabilnost jednog cilja.
 * @param targetIndex Indeks cilja za koji se racuna stabilnost.
 * @return Stabilnost jednog cilja od 0 do 100.
 */
int calculateTargetStability(int targetIndex);
/** @brief Racuna stabilnost za sve ciljeve trenutnog pokusaja. */
void calculateAllTargetStabilityScores();
/**
 * @brief Pretvara ADC vrijednost potenciometra u popunjenost trake.
 * @param potValue ADC vrijednost potenciometra.
 */
void updateTurnBarFromPot(int potValue);
/** @brief Cita potenciometar, rotira bravu, biljezi replay i azurira cilj. */
void updateVaultRotationFromPot();

/** Pokazivac na level koji se trenutno igra. */
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
  levelSolvedTimeMs = 0;
  levelStabilityScore = 0;
  resetLevelStats();
  gameStartTime = millis();
  recordReplayEvent(REPLAY_EVENT_START, true);
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
  levelSolvedTimeMs = levelFinishedTime - gameStartTime;
  levelStabilityScore = calculateLevelStability();
  calculateAllTargetStabilityScores();
  recordReplayEvent(REPLAY_EVENT_FINISH, true);
  gameScreen = GAME_SCREEN_FINISHED;

  if (levelWasWon) {
    playVictorySound();
    postScoreWebhook(
      selectedLevelIndex + 1,
      getCurrentLevel().name,
      levelSolvedTimeMs,
      levelStabilityScore,
      getTargetsHit(),
      getTotalTargets()
    );
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

  recordReplayEvent(REPLAY_EVENT_TARGET_HIT, true);
  currentTargetIndex++;
  targetHitStartTime = 0;

  if (isLevelComplete()) {
    finishLevel(true);
  } else {
    clickRight();
  }
}

void resetLevelStats() {
  stabilitySampleCount = 0;
  stabilityErrorSum = 0;
  stabilityOvershootCount = 0;
  stabilityDirectionChanges = 0;
  lastPotDirection = 0;
  currentRunTargetCount = min(getCurrentLevel().targetCount, MAX_LEVEL_TARGETS);
  replayEventCount = 0;
  lastReplayEventTime = 0;
  lastReplayEventFill = barFill;

  for (int i = 0; i < MAX_LEVEL_TARGETS; i++) {
    currentRunTargets[i] = 0;
    targetStabilityScore[i] = 100;
    targetStabilitySampleCount[i] = 0;
    targetStabilityErrorSum[i] = 0;
    targetStabilityOvershootCount[i] = 0;
    targetStabilityDirectionChanges[i] = 0;
    targetLastPotDirection[i] = 0;
  }

  for (int i = 0; i < currentRunTargetCount; i++) {
    currentRunTargets[i] = getCurrentLevel().targets[i].fillAmount;
  }
}

void recordLevelStability(int potDiff) {
  if (gameScreen != GAME_SCREEN_PLAYING || isLevelComplete() || isLevelOutOfTime()) {
    return;
  }

  float targetFill = getTargetFillAmount();
  int targetIndex = currentTargetIndex;
  stabilityErrorSum += constrain(abs(barFill - targetFill) / 2.0, 0.0, 1.0);
  stabilitySampleCount++;

  if (targetIndex >= 0 && targetIndex < MAX_LEVEL_TARGETS) {
    targetStabilityErrorSum[targetIndex] += constrain(abs(barFill - targetFill) / 2.0, 0.0, 1.0);
    targetStabilitySampleCount[targetIndex]++;
  }

  if (
    (targetFill > 0 && barFill > targetFill + TARGET_OVERSHOOT_ALLOWANCE) ||
    (targetFill < 0 && barFill < targetFill - TARGET_OVERSHOOT_ALLOWANCE)
  ) {
    stabilityOvershootCount++;

    if (targetIndex >= 0 && targetIndex < MAX_LEVEL_TARGETS) {
      targetStabilityOvershootCount[targetIndex]++;
    }
  }

  int direction = 0;
  if (potDiff > POT_DEADZONE) {
    direction = 1;
  } else if (potDiff < -POT_DEADZONE) {
    direction = -1;
  }

  if (direction != 0) {
    if (lastPotDirection != 0 && direction != lastPotDirection) {
      stabilityDirectionChanges++;
    }

    lastPotDirection = direction;

    if (targetIndex >= 0 && targetIndex < MAX_LEVEL_TARGETS) {
      if (targetLastPotDirection[targetIndex] != 0 && direction != targetLastPotDirection[targetIndex]) {
        targetStabilityDirectionChanges[targetIndex]++;
      }

      targetLastPotDirection[targetIndex] = direction;
    }
  }
}

void recordReplayEvent(int eventType, bool force) {
  if (replayEventCount >= MAX_REPLAY_EVENTS) {
    return;
  }

  unsigned long timeNow = millis();
  unsigned long timeMs = timeNow >= gameStartTime ? timeNow - gameStartTime : 0;

  if (!force) {
    if (abs(barFill - lastReplayEventFill) < REPLAY_MIN_FILL_DELTA) {
      return;
    }

    if (timeNow - lastReplayEventTime < REPLAY_SAMPLE_INTERVAL_MS) {
      return;
    }
  }

  replayEventTimeMs[replayEventCount] = timeMs;
  replayEventFill[replayEventCount] = barFill;
  replayEventTargetIndex[replayEventCount] = currentTargetIndex;
  replayEventType[replayEventCount] = eventType;
  replayEventCount++;
  lastReplayEventTime = timeNow;
  lastReplayEventFill = barFill;
}

int calculateLevelStability() {
  if (stabilitySampleCount <= 0) {
    return 100;
  }

  float averageError = stabilityErrorSum / stabilitySampleCount;
  float overshootRate = (float)stabilityOvershootCount / stabilitySampleCount;
  float directionRate = (float)stabilityDirectionChanges / stabilitySampleCount;
  float score = 100.0 - averageError * 55.0 - overshootRate * 30.0 - directionRate * 15.0;

  return constrain((int)(score + 0.5), 0, 100);
}

int calculateTargetStability(int targetIndex) {
  if (targetIndex < 0 || targetIndex >= MAX_LEVEL_TARGETS || targetStabilitySampleCount[targetIndex] <= 0) {
    return 100;
  }

  float averageError = targetStabilityErrorSum[targetIndex] / targetStabilitySampleCount[targetIndex];
  float overshootRate = (float)targetStabilityOvershootCount[targetIndex] / targetStabilitySampleCount[targetIndex];
  float directionRate = (float)targetStabilityDirectionChanges[targetIndex] / targetStabilitySampleCount[targetIndex];
  float score = 100.0 - averageError * 55.0 - overshootRate * 30.0 - directionRate * 15.0;

  return constrain((int)(score + 0.5), 0, 100);
}

void calculateAllTargetStabilityScores() {
  for (int i = 0; i < currentRunTargetCount; i++) {
    targetStabilityScore[i] = calculateTargetStability(i);
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
  int menuItemCount = LEVEL_COUNT + 2;

  if (menuItemCount <= 0) {
    return;
  }

  selectedLevelIndex += direction;

  if (selectedLevelIndex < 0) {
    selectedLevelIndex = menuItemCount - 1;
  } else if (selectedLevelIndex >= menuItemCount) {
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
  recordLevelStability(diff);
  recordReplayEvent(REPLAY_EVENT_MOVE, false);
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
