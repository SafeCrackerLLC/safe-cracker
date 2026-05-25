#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <esp_timer.h>

extern Adafruit_SSD1306 display;
extern float vaultAngle;
extern float barFill;
extern int lastValue;
extern int currentTargetIndex;
extern int selectedLevelIndex;
extern int selectedNetworkInfoAction;
extern int gameScreen;
extern bool levelWasWon;
extern int joystickCenterValue;
extern bool joystickMenuReady;
extern unsigned long gameStartTime;
extern unsigned long levelFinishedTime;
extern unsigned long targetHitStartTime;
extern unsigned long lastJoystickMoveTime;
extern unsigned long levelSolvedTimeMs;
extern int levelStabilityScore;
extern int stabilitySampleCount;
extern float stabilityErrorSum;
extern int stabilityOvershootCount;
extern int stabilityDirectionChanges;
extern int lastPotDirection;
extern int currentRunTargetCount;
extern float currentRunTargets[MAX_LEVEL_TARGETS];
extern int targetStabilityScore[MAX_LEVEL_TARGETS];
extern int targetStabilitySampleCount[MAX_LEVEL_TARGETS];
extern float targetStabilityErrorSum[MAX_LEVEL_TARGETS];
extern int targetStabilityOvershootCount[MAX_LEVEL_TARGETS];
extern int targetStabilityDirectionChanges[MAX_LEVEL_TARGETS];
extern int targetLastPotDirection[MAX_LEVEL_TARGETS];
extern int replayEventCount;
extern unsigned long replayEventTimeMs[MAX_REPLAY_EVENTS];
extern float replayEventFill[MAX_REPLAY_EVENTS];
extern int replayEventTargetIndex[MAX_REPLAY_EVENTS];
extern int replayEventType[MAX_REPLAY_EVENTS];
extern unsigned long lastReplayEventTime;
extern float lastReplayEventFill;
extern unsigned long lastActivityTime;
extern volatile bool sleepRequested;
extern esp_timer_handle_t sleepTimer;
extern bool wifiConfigPortalActive;
extern bool wifiConnectInProgress;
extern bool wifiHasCredentials;
extern bool wifiOfflineMode;
extern unsigned long wifiConnectStartedAt;
extern String configuredSsid;
extern String configuredPassword;
extern String configuredWebhookUrl;
extern String wifiPortalSsid;
extern String networkStatusMessage;

#endif
