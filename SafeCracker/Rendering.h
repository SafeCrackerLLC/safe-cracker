#ifndef RENDERING_H
#define RENDERING_H

/**
 * @file Rendering.h
 * @brief OLED renderiranje svih ekrana igre.
 */

#include <Arduino.h>

#include "Config.h"
#include "GameLogic.h"
#include "SafeNetwork.h"
#include "State.h"

/** @brief Crta vertikalni napredak kroz ciljeve. */
void drawProgressBar();
/** @brief Crta preostalo vrijeme trenutnog levela. */
void drawTimer();
/** @brief Crta poruku INHALE/EXHALE/DONE/TIME. */
void drawBreathMessage();
/** @brief Crta pomocnu poruku za trenutni cilj. */
void drawTargetMessage();
/** @brief Crta horizontalnu traku okretanja i marker cilja. */
void drawTurnBar();
/** @brief Crta animiranu strelicu iznad cilja. */
void drawTargetArrow();
/** @brief Crta animirani kotac brave. */
void drawVaultWheel();
/** @brief Crta glavni meni s levelima i WiFi opcijama. */
void drawLevelSelectScreen();
/** @brief Crta ekran rezultata nakon zavrsetka levela. */
void drawFinishedScreen();
/** @brief Crta upute za spajanje na konfiguracijski WiFi portal. */
void drawWifiConfigScreen();
/** @brief Crta SSID, webhook i akcije za ponovno podesavanje mreze. */
void drawNetworkInfoScreen();
/** @brief Crta ekran prema trenutnom gameScreen stanju. */
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

  display.setCursor(0, 14);
  display.print(getWifiStatusLabel());

  int menuItemCount = LEVEL_COUNT + 2;

  for (int i = 0; i < menuItemCount; i++) {
    int rowY = 26 + i * 10;
    String label;

    if (i < LEVEL_COUNT) {
      label = LEVELS[i].name;
    } else if (i == LEVEL_COUNT) {
      label = "Network info";
    } else {
      label = wifiOfflineMode ? "Go online" : "Go offline";
    }

    if (i == selectedLevelIndex) {
      display.fillRect(8, rowY - 2, 112, 9, WHITE);
      display.setTextColor(BLACK);
      display.setCursor(12, rowY);
      display.print("> ");
      display.print(label);
      display.setTextColor(WHITE);
    } else {
      display.setCursor(12, rowY);
      display.print("  ");
      display.print(label);
    }
  }

}

void drawFinishedScreen() {
  display.setTextColor(WHITE);

  display.setTextSize(1);
  display.setCursor(37, 4);
  display.print(levelWasWon ? "Unlocked!" : "Finished");

  display.setTextSize(1);

  display.setCursor(12, 18);
  display.print("Targets hit: ");
  display.print(getTargetsHit());
  display.print("/");
  display.print(getTotalTargets());

  display.setCursor(12, 30);
  display.print("Time: ");
  display.print(levelSolvedTimeMs / 1000.0, 1);
  display.print("s");

  display.setCursor(12, 42);
  display.print("Stability: ");
  display.print(levelStabilityScore);
  display.print("%");

  display.setCursor(34, 56);
  display.print("Btn: menu");
}

void drawWifiConfigScreen() {
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(13, 2);
  display.print("WiFi setup");

  display.setCursor(0, 16);
  display.print("SSID:");
  display.setCursor(0, 26);
  display.print(wifiPortalSsid);

  display.setCursor(0, 38);
  display.print("Pass:");
  display.print(WIFI_PORTAL_PASSWORD);

  display.setCursor(0, 50);
  display.print("Open 192.168.4.1");
  display.setCursor(0, 58);
  display.print("Btn: offline");
}

void drawNetworkInfoScreen() {
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(22, 2);
  display.print("Network info");

  display.setCursor(0, 14);
  display.print(getWifiStatusLabel());

  display.setCursor(0, 24);
  display.print("SSID:");
  if (configuredSsid.length() > 0) {
    display.print(configuredSsid.substring(0, 15));
  } else {
    display.print("-");
  }

  display.setCursor(0, 34);
  display.print("URL:");
  String webhookUrl = getConfiguredWebhookUrl();
  display.print(webhookUrl.substring(0, 17));
  display.setCursor(0, 44);
  display.print(webhookUrl.substring(17, 38));

  for (int i = 0; i < 2; i++) {
    int x = i == 0 ? 8 : 64;
    int y = 55;

    if (selectedNetworkInfoAction == i) {
      display.fillRect(x - 2, y - 2, 54, 10, WHITE);
      display.setTextColor(BLACK);
    } else {
      display.drawRect(x - 2, y - 2, 54, 10, WHITE);
      display.setTextColor(WHITE);
    }

    display.setCursor(x, y);
    display.print(i == 0 ? "Back" : "New setup");
  }

  display.setTextColor(WHITE);
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

  if (gameScreen == GAME_SCREEN_WIFI_CONFIG) {
    drawWifiConfigScreen();
    display.display();
    return;
  }

  if (gameScreen == GAME_SCREEN_NETWORK_INFO) {
    drawNetworkInfoScreen();
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
