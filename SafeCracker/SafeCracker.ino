#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Config.h"
#include "Feedback.h"
#include "GameLogic.h"
#include "Input.h"
#include "Level.h"
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
int gameScreen = GAME_SCREEN_LEVEL_SELECT;
bool levelWasWon = false;
int joystickCenterValue = JOYSTICK_CENTER_VALUE;
bool joystickMenuReady = true;
unsigned long gameStartTime = 0;
unsigned long levelFinishedTime = 0;
unsigned long targetHitStartTime = 0;
unsigned long lastJoystickMoveTime = 0;

// --------------------
// Device / power state
// --------------------
unsigned long lastActivityTime = 0;
volatile bool sleepRequested = false;
esp_timer_handle_t sleepTimer = NULL;

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

  lastValue = analogRead(POT_PIN);
  calibrateJoystickCenter();
  lastActivityTime = millis();
  initializeSleepTimer();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true);
  }

  display.clearDisplay();
  display.display();
}

// --------------------
// Arduino game loop
// --------------------
void loop() {
  handleButtonActivity();

  if (gameScreen == GAME_SCREEN_PLAYING) {
    updateLevelTimer();
    updateVaultRotationFromPot();
  }

  drawGame();
  sleepIfInactive();

  delay(10);
}
