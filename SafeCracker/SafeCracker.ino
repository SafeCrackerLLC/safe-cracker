#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Config.h"
#include "Feedback.h"
#include "GameLogic.h"
#include "Input.h"
#include "Level.h"
#include "SafeNetwork.h"
#include "Power.h"
#include "Rendering.h"
#include "State.h"

// --------------------
// Display setup
// --------------------
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --------------------
// Game state
// --------------------
float vaultAngle = 0;
float barFill = 0;
int lastValue = 0;
int currentTargetIndex = 0;
int selectedLevelIndex = 0;
int selectedNetworkInfoAction = 0;
int gameScreen = GAME_SCREEN_LEVEL_SELECT;
bool levelWasWon = false;
int joystickCenterValue = JOYSTICK_CENTER_VALUE;
bool joystickMenuReady = true;
unsigned long gameStartTime = 0;
unsigned long levelFinishedTime = 0;
unsigned long targetHitStartTime = 0;
unsigned long lastJoystickMoveTime = 0;
unsigned long levelSolvedTimeMs = 0;
int levelStabilityScore = 0;
int stabilitySampleCount = 0;
float stabilityErrorSum = 0;
int stabilityOvershootCount = 0;
int stabilityDirectionChanges = 0;
int lastPotDirection = 0;
int currentRunTargetCount = 0;
float currentRunTargets[MAX_LEVEL_TARGETS];
int targetStabilityScore[MAX_LEVEL_TARGETS];
int targetStabilitySampleCount[MAX_LEVEL_TARGETS];
float targetStabilityErrorSum[MAX_LEVEL_TARGETS];
int targetStabilityOvershootCount[MAX_LEVEL_TARGETS];
int targetStabilityDirectionChanges[MAX_LEVEL_TARGETS];
int targetLastPotDirection[MAX_LEVEL_TARGETS];
int replayEventCount = 0;
unsigned long replayEventTimeMs[MAX_REPLAY_EVENTS];
float replayEventFill[MAX_REPLAY_EVENTS];
int replayEventTargetIndex[MAX_REPLAY_EVENTS];
int replayEventType[MAX_REPLAY_EVENTS];
unsigned long lastReplayEventTime = 0;
float lastReplayEventFill = 0;
unsigned long lastSerialStatusTime = 0;

// --------------------
// Device / power state
// --------------------
unsigned long lastActivityTime = 0;
volatile bool sleepRequested = false;
esp_timer_handle_t sleepTimer = NULL;
bool wifiConfigPortalActive = false;
bool wifiConnectInProgress = false;
bool wifiHasCredentials = false;
bool wifiOfflineMode = false;
unsigned long wifiConnectStartedAt = 0;
String configuredSsid = "";
String configuredPassword = "";
String configuredWebhookUrl = "";
String wifiPortalSsid = "";
String networkStatusMessage = "";

// --------------------
// Arduino setup
// --------------------
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VIBRATION_PIN, OUTPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(VIBRATION_PIN, LOW);

  Serial.begin(115200);
  delay(1000);
  Serial.println("SafeCracker booting...");
  Serial.println("Starting I2C on SDA 21 / SCL 22...");
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  Serial.println("Starting OLED...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true);
  }
  Serial.println("OLED found.");

  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(255);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("SafeCracker");
  display.println("OLED test OK");
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();

  Serial.println("Reading potentiometer...");
  lastValue = analogRead(POT_PIN);
  Serial.println("Calibrating joystick...");
  calibrateJoystickCenter();
  Serial.println("Starting sleep timer...");
  lastActivityTime = millis();
  initializeSleepTimer();
  Serial.println("Starting network...");
  initializeNetwork();
  Serial.println("SafeCracker ready.");
}

// --------------------
// Arduino game loop
// --------------------
void loop() {
  updateNetwork();
  handleButtonActivity();

  if (millis() - lastSerialStatusTime >= 2000) {
    lastSerialStatusTime = millis();
    Serial.print("Running. Screen=");
    Serial.print(gameScreen);
    Serial.print(" WiFi=");
    Serial.println(getWifiStatusLabel());
  }

  if (gameScreen == GAME_SCREEN_PLAYING) {
    updateLevelTimer();
    updateVaultRotationFromPot();
  }

  drawGame();
  sleepIfInactive();

  delay(10);
}
