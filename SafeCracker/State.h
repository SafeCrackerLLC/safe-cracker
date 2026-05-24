#ifndef STATE_H
#define STATE_H

#include <Adafruit_SSD1306.h>
#include <esp_timer.h>

extern Adafruit_SSD1306 display;
extern float vaultAngle;
extern float barFill;
extern int lastValue;
extern int currentTargetIndex;
extern int selectedLevelIndex;
extern int gameScreen;
extern bool levelWasWon;
extern int joystickCenterValue;
extern bool joystickMenuReady;
extern unsigned long gameStartTime;
extern unsigned long levelFinishedTime;
extern unsigned long targetHitStartTime;
extern unsigned long lastJoystickMoveTime;
extern unsigned long lastActivityTime;
extern volatile bool sleepRequested;
extern esp_timer_handle_t sleepTimer;

#endif
