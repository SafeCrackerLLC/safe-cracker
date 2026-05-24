#ifndef RENDERING_H
#define RENDERING_H

#include <Arduino.h>

#include "Config.h"
#include "GameLogic.h"
#include "State.h"

void drawProgressBar();
void drawTimer();
void drawBreathMessage();
void drawTargetMessage();
void drawTurnBar();
void drawTargetArrow();
void drawVaultWheel();
void drawLevelSelectScreen();
void drawFinishedScreen();
void drawGame();

void drawProgressBar() {
  display.drawRect(PROGRESS_BAR_X, PROGRESS_BAR_Y, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT, WHITE);

  int fillHeight = getLevelProgress() * (PROGRESS_BAR_HEIGHT - 2);
  int fillY = PROGRESS_BAR_Y + PROGRESS_BAR_HEIGHT - 1 - fillHeight;

  display.fillRect(
    PROGRESS_BAR_X + 1,
    fillY,
    PROGRESS_BAR_WIDTH - 2,
    fillHeight,
    WHITE
  );
}

void drawTimer() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(TIMER_X, VAULT_CENTER_Y - 4);
  display.print(getRemainingSeconds());
  display.print("s");
}

void drawBreathMessage() {
  int moveOffset = getBreathTextMoveOffset();

  display.drawRect(
    BREATH_TEXT_BOX_X,
    BREATH_TEXT_BOX_Y,
    BREATH_TEXT_BOX_WIDTH,
    BREATH_TEXT_BOX_HEIGHT,
    WHITE
  );

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(BREATH_TEXT_X + moveOffset, VAULT_CENTER_Y - 4 + moveOffset);
  display.print(getBreathMessage());
}

void drawTargetMessage() {
  const char* targetMessage = getTargetMessage();

  if (targetMessage[0] == '\0') {
    return;
  }

  display.setTextSize(1);
  display.setTextColor(WHITE);

  if (targetMessage[0] == 'C') {
    display.setCursor(92, TARGET_MESSAGE_Y);
  } else {
    display.setCursor(94, TARGET_MESSAGE_Y);
  }

  display.print(targetMessage);
}

void drawTurnBar() {
  display.drawRect(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT, WHITE);
  display.drawLine(BAR_CENTER_X, BAR_Y, BAR_CENTER_X, BAR_Y + BAR_HEIGHT - 1, WHITE);
  display.drawLine(getTargetX(), BAR_Y, getTargetX(), BAR_Y + BAR_HEIGHT - 1, WHITE);

  int halfBarWidth = (BAR_WIDTH / 2) - 2;
  int fillWidth = abs(barFill) * halfBarWidth;

  if (barFill > 0) {
    display.fillRect(BAR_CENTER_X + 1, BAR_Y + 2, fillWidth, BAR_HEIGHT - 4, WHITE);
  } else if (barFill < 0) {
    display.fillRect(BAR_CENTER_X - fillWidth, BAR_Y + 2, fillWidth, BAR_HEIGHT - 4, WHITE);
  }
}

void drawTargetArrow() {
  int arrowY = getTargetArrowY();

  display.fillTriangle(
    getTargetX(),
    arrowY - TARGET_ARROW_SIZE,
    getTargetX() - TARGET_ARROW_SIZE,
    arrowY,
    getTargetX() + TARGET_ARROW_SIZE,
    arrowY,
    WHITE
  );
  display.drawLine(getTargetX(), arrowY, getTargetX(), arrowY + 2, WHITE);
}

void drawVaultWheel() {
  display.drawCircle(VAULT_CENTER_X, VAULT_CENTER_Y, VAULT_OUTER_RADIUS, WHITE);
  display.drawCircle(VAULT_CENTER_X, VAULT_CENTER_Y, VAULT_OUTER_RADIUS - 2, WHITE);
  display.fillCircle(VAULT_CENTER_X, VAULT_CENTER_Y, VAULT_INNER_RADIUS, WHITE);

  for (int i = 0; i < VAULT_HANDLE_COUNT; i++) {
    float handleAngle = vaultAngle + (FULL_ROTATION / VAULT_HANDLE_COUNT) * i;
    int handleX = VAULT_CENTER_X + cos(handleAngle) * VAULT_HANDLE_LENGTH;
    int handleY = VAULT_CENTER_Y + sin(handleAngle) * VAULT_HANDLE_LENGTH;

    display.drawLine(VAULT_CENTER_X, VAULT_CENTER_Y, handleX, handleY, WHITE);
    display.fillCircle(handleX, handleY, VAULT_HANDLE_RADIUS, WHITE);
  }
}

void drawLevelSelectScreen() {
  display.setTextColor(WHITE);

  display.setTextSize(1);
  display.setCursor(31, 2);
  display.print("SafeCracker");

  display.setCursor(27, 16);
  display.print("Choose level");

  for (int i = 0; i < LEVEL_COUNT; i++) {
    int rowY = 31 + i * 12;

    if (i == selectedLevelIndex) {
      display.fillRect(16, rowY - 2, 96, 11, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(24, rowY);
      display.print("> ");
      display.print(LEVELS[i].name);
      display.setTextColor(WHITE);
    } else {
      display.setCursor(24, rowY);
      display.print("  ");
      display.print(LEVELS[i].name);
    }
  }

  display.setCursor(22, 56);
  display.print("Btn: start");
}

void drawFinishedScreen() {
  display.setTextColor(WHITE);

  display.setTextSize(1);
  display.setCursor(37, 4);
  display.print("Finished!");

  display.setTextSize(1);

  display.setCursor(12, 26);
  display.print("Targets hit: ");
  display.print(getTargetsHit());
  display.print("/");
  display.print(getTotalTargets());

  display.setCursor(12, 38);
  display.print("Time left: ");
  display.print(getRemainingSeconds());
  display.print("s");

  display.drawRect(39, 51, 50, 12, WHITE);
  display.setCursor(46, 54);
  display.print("Again!");
}

void drawGame() {
  display.clearDisplay();

  if (gameScreen == GAME_SCREEN_LEVEL_SELECT) {
    drawLevelSelectScreen();
    display.display();
    return;
  }

  if (gameScreen == GAME_SCREEN_FINISHED) {
    drawFinishedScreen();
    display.display();
    return;
  }

  drawProgressBar();
  drawTurnBar();
  drawTargetArrow();
  drawVaultWheel();
  drawTimer();
  drawBreathMessage();
  drawTargetMessage();

  display.display();
}

#endif
