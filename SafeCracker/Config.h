#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file Config.h
 * @brief Centralne hardverske pin definicije i konstante igre.
 *
 * Vrijednosti u ovoj datoteci koriste svi moduli firmwarea: prikaz,
 * logika levela, mreza, ulazni uredjaji i upravljanje potrosnjom.
 */

// --------------------
// Hardware pins
// --------------------
/** GPIO pin pasivne zujalice. */
#define BUZZER_PIN 13
/** GPIO pin vibracijskog motora. */
#define VIBRATION_PIN 26
/** ADC pin potenciometra koji simulira okretanje brave. */
#define POT_PIN 34
/** Joystick Y mora koristiti ADC1 jer ADC2 nije pouzdan dok je WiFi aktivan. */
#define JOYSTICK_Y_PIN 33
/** GPIO pin tipke, konfiguriran kao INPUT_PULLUP. */
#define BUTTON_PIN 12
/** I2C SDA pin OLED ekrana. */
#define OLED_SDA_PIN 21
/** I2C SCL pin OLED ekrana. */
#define OLED_SCL_PIN 22

// --------------------
// Display setup
// --------------------
/** Sirina OLED ekrana u pikselima. */
#define SCREEN_WIDTH 128
/** Visina OLED ekrana u pikselima. */
#define SCREEN_HEIGHT 64

// --------------------
// Game settings
// --------------------
/** X koordinata sredista nacrtane brave. */
const int VAULT_CENTER_X = 58;
/** Y koordinata sredista nacrtane brave. */
const int VAULT_CENTER_Y = 42;
/** Vanjski radijus brave na OLED-u. */
const int VAULT_OUTER_RADIUS = 20;
/** Unutarnji radijus sredista brave. */
const int VAULT_INNER_RADIUS = 5;
/** Duljina jedne rucke/kazaljke brave. */
const int VAULT_HANDLE_LENGTH = 15;
/** Radijus kruzica na kraju rucke brave. */
const int VAULT_HANDLE_RADIUS = 3;
/** Broj rucki koje se crtaju na bravi. */
const int VAULT_HANDLE_COUNT = 3;
/** Jedan puni krug u radijanima. */
const float FULL_ROTATION = 6.2831853;
/** X koordinata lijevog ruba horizontalne trake okretanja. */
const int BAR_X = 8;
/** Y koordinata gornjeg ruba horizontalne trake okretanja. */
const int BAR_Y = 2;
/** Sirina horizontalne trake okretanja. */
const int BAR_WIDTH = 112;
/** Visina horizontalne trake okretanja. */
const int BAR_HEIGHT = 12;
/** X koordinata sredine trake okretanja. */
const int BAR_CENTER_X = BAR_X + BAR_WIDTH / 2;
/** Osnovna Y koordinata strelice trenutnog cilja. */
const int TARGET_ARROW_Y = BAR_Y + BAR_HEIGHT + 6;
/** Velicina trokuta strelice trenutnog cilja. */
const int TARGET_ARROW_SIZE = 4;
/** Y koordinata strelice dok animacijski dodiruje traku. */
const int TARGET_ARROW_TOUCH_Y = BAR_Y + BAR_HEIGHT + TARGET_ARROW_SIZE;
/** Trajanje jednog ciklusa animacije strelice u milisekundama. */
const int TARGET_ARROW_CYCLE_MS = 1800;
/** Trajanje faze u kojoj je strelica spustena do trake. */
const int TARGET_ARROW_HOLD_MS = 120;
/** X koordinata vertikalne trake napretka kroz level. */
const int PROGRESS_BAR_X = 0;
/** Y koordinata vertikalne trake napretka kroz level. */
const int PROGRESS_BAR_Y = 18;
/** Sirina vertikalne trake napretka kroz level. */
const int PROGRESS_BAR_WIDTH = 5;
/** Visina vertikalne trake napretka kroz level. */
const int PROGRESS_BAR_HEIGHT = 44;
/** X koordinata teksta timera. */
const int TIMER_X = 9;
/** X koordinata poruke disanja. */
const int BREATH_TEXT_X = 91;
/** X koordinata okvira poruke disanja. */
const int BREATH_TEXT_BOX_X = 89;
/** Y koordinata okvira poruke disanja. */
const int BREATH_TEXT_BOX_Y = VAULT_CENTER_Y - 7;
/** Sirina okvira poruke disanja. */
const int BREATH_TEXT_BOX_WIDTH = 39;
/** Visina okvira poruke disanja. */
const int BREATH_TEXT_BOX_HEIGHT = 13;
/** Y koordinata kratke poruke za trenutni cilj. */
const int TARGET_MESSAGE_Y = BREATH_TEXT_BOX_Y + BREATH_TEXT_BOX_HEIGHT + 2;
/** Najmanja ADC vrijednost potenciometra. */
const int POT_MIN_VALUE = 0;
/** Najveca ADC vrijednost potenciometra. */
const int POT_MAX_VALUE = 4095;
/** Minimalna promjena ADC vrijednosti koja se smatra stvarnim okretanjem. */
const int POT_DEADZONE = 50;
/** Trajanje kratkog klik feedbacka u milisekundama. */
const int CLICK_VIBRATION_MS = 15;
/** Minimalno trajanje vibracijskog impulsa u milisekundama. */
const int MIN_VIBRATION_PULSE_MS = 60;
/** Najmanja ADC vrijednost joystick Y osi. */
const int JOYSTICK_MIN_VALUE = 0;
/** Najveca ADC vrijednost joystick Y osi. */
const int JOYSTICK_MAX_VALUE = 4095;
/** Zadana neutralna vrijednost joysticka prije kalibracije. */
const int JOYSTICK_CENTER_VALUE = 2048;
/** Broj uzoraka za kalibraciju neutralne vrijednosti joysticka. */
const int JOYSTICK_CENTER_SAMPLE_COUNT = 16;
/** Udaljenost od centra koju joystick mora prijeci za pomak menija. */
const int JOYSTICK_DEADZONE = 1000;
/** Minimalni razmak izmedju ponovljenih pomaka menija joystickom. */
const unsigned long JOYSTICK_MENU_REPEAT_MS = 250;
/** Faktor kojim se ADC promjena pretvara u kut rotacije brave. */
const float ROTATION_SPEED = 0.01;
/** Tolerancija unutar koje se cilj smatra pogodjenim. */
const float TARGET_HIT_TOLERANCE = 0.08;
/** Dopusteno prebacivanje preko cilja prije kazne za stabilnost. */
const float TARGET_OVERSHOOT_ALLOWANCE = 0.05;
/** Vrijeme koje igrac mora zadrzati cilj unutar tolerancije. */
const unsigned long TARGET_HIT_HOLD_MS = 250;
/** Trajanje ciklusa animacije poruke disanja. */
const int BREATH_BOB_CYCLE_MS = 2400;
/** Trajanje pomaknute faze animacije poruke disanja. */
const int BREATH_BOB_STEP_MS = 120;
/** Zadani webhook na koji se salju scoreovi. */
const char* const SCORE_WEBHOOK_URL = "https://ruc-esp.nerizz.com/api/scores";
/** Lozinka lokalnog WiFi konfiguracijskog portala. */
const char* const WIFI_PORTAL_PASSWORD = "safe1234";
/** Maksimalno cekanje WiFi spajanja prije povratka u setup portal. */
const unsigned long WIFI_CONNECT_TIMEOUT_MS = 12000;
/** Timeout HTTP POST zahtjeva za score u milisekundama. */
const unsigned long SCORE_POST_TIMEOUT_MS = 4000;
/** Najveci broj ciljeva koji se sprema u score statistiku. */
const int MAX_LEVEL_TARGETS = 12;
/** Najveci broj replay dogadjaja koji se cuva u memoriji. */
const int MAX_REPLAY_EVENTS = 180;
/** Minimalni vremenski razmak automatskih replay uzoraka. */
const unsigned long REPLAY_SAMPLE_INTERVAL_MS = 120;
/** Minimalna promjena fill vrijednosti za automatski replay uzorak. */
const float REPLAY_MIN_FILL_DELTA = 0.03;
/** Replay dogadjaj koji oznacava pocetak levela. */
const int REPLAY_EVENT_START = 0;
/** Replay dogadjaj koji oznacava pomak trake. */
const int REPLAY_EVENT_MOVE = 1;
/** Replay dogadjaj koji oznacava pogodjen cilj. */
const int REPLAY_EVENT_TARGET_HIT = 2;
/** Replay dogadjaj koji oznacava kraj levela. */
const int REPLAY_EVENT_FINISH = 3;

// --------------------
// Screen states
// --------------------
/** Pocetni meni za izbor levela i mreznih opcija. */
const int GAME_SCREEN_LEVEL_SELECT = 0;
/** Aktivna igra. */
const int GAME_SCREEN_PLAYING = 1;
/** Ekran s rezultatom nakon zavrsetka levela. */
const int GAME_SCREEN_FINISHED = 2;
/** Ekran koji prikazuje WiFi AP podatke za konfiguraciju. */
const int GAME_SCREEN_WIFI_CONFIG = 3;
/** Ekran s trenutnim mreznim statusom i setup akcijama. */
const int GAME_SCREEN_NETWORK_INFO = 4;

// --------------------
// Device settings
// --------------------
/** Vrijeme neaktivnosti nakon kojeg ESP32 ulazi u deep sleep. */
const unsigned long SLEEP_TIMEOUT_MS = 30000;

#endif
