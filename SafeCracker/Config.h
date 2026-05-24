#ifndef CONFIG_H
#define CONFIG_H

// --------------------
// Hardware pins
// --------------------
#define BUZZER_PIN 13
#define VIBRATION_PIN 26
#define POT_PIN 34
#define JOYSTICK_Y_PIN 27
#define BUTTON_PIN 12

// --------------------
// Display setup
// --------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// --------------------
// Game settings
// --------------------
const int VAULT_CENTER_X = 58;
const int VAULT_CENTER_Y = 42;
const int VAULT_OUTER_RADIUS = 20;
const int VAULT_INNER_RADIUS = 5;
const int VAULT_HANDLE_LENGTH = 15;
const int VAULT_HANDLE_RADIUS = 3;
const int VAULT_HANDLE_COUNT = 3;
const float FULL_ROTATION = 6.2831853;
const int BAR_X = 8;
const int BAR_Y = 2;
const int BAR_WIDTH = 112;
const int BAR_HEIGHT = 12;
const int BAR_CENTER_X = BAR_X + BAR_WIDTH / 2;
const int TARGET_ARROW_Y = BAR_Y + BAR_HEIGHT + 6;
const int TARGET_ARROW_SIZE = 4;
const int TARGET_ARROW_TOUCH_Y = BAR_Y + BAR_HEIGHT + TARGET_ARROW_SIZE;
const int TARGET_ARROW_CYCLE_MS = 1800;
const int TARGET_ARROW_HOLD_MS = 120;
const int PROGRESS_BAR_X = 0;
const int PROGRESS_BAR_Y = 18;
const int PROGRESS_BAR_WIDTH = 5;
const int PROGRESS_BAR_HEIGHT = 44;
const int TIMER_X = 9;
const int BREATH_TEXT_X = 91;
const int BREATH_TEXT_BOX_X = 89;
const int BREATH_TEXT_BOX_Y = VAULT_CENTER_Y - 7;
const int BREATH_TEXT_BOX_WIDTH = 39;
const int BREATH_TEXT_BOX_HEIGHT = 13;
const int TARGET_MESSAGE_Y = BREATH_TEXT_BOX_Y + BREATH_TEXT_BOX_HEIGHT + 2;
const int POT_MIN_VALUE = 0;
const int POT_MAX_VALUE = 4095;
const int POT_DEADZONE = 50;
const int CLICK_VIBRATION_MS = 15;
const int MIN_VIBRATION_PULSE_MS = 60;
const int JOYSTICK_MIN_VALUE = 0;
const int JOYSTICK_MAX_VALUE = 4095;
const int JOYSTICK_CENTER_VALUE = 2048;
const int JOYSTICK_CENTER_SAMPLE_COUNT = 16;
const int JOYSTICK_DEADZONE = 1000;
const unsigned long JOYSTICK_MENU_REPEAT_MS = 250;
const float ROTATION_SPEED = 0.01;
const float TARGET_HIT_TOLERANCE = 0.08;
const float TARGET_OVERSHOOT_ALLOWANCE = 0.05;
const unsigned long TARGET_HIT_HOLD_MS = 250;
const int BREATH_BOB_CYCLE_MS = 2400;
const int BREATH_BOB_STEP_MS = 120;

// --------------------
// Screen states
// --------------------
const int GAME_SCREEN_LEVEL_SELECT = 0;
const int GAME_SCREEN_PLAYING = 1;
const int GAME_SCREEN_FINISHED = 2;

// --------------------
// Device settings
// --------------------
const unsigned long SLEEP_TIMEOUT_MS = 30000;

#endif
