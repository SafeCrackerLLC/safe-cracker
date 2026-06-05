#ifndef STATE_H
#define STATE_H

/**
 * @file State.h
 * @brief Globalno stanje firmwarea dijeljeno izmedju modula.
 *
 * Definicije varijabli nalaze se u SafeCracker.ino, a ostali headeri ovdje
 * dobivaju pristup stanju bez dupliciranja memorije.
 */

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <esp_timer.h>

/** OLED driver instanca. */
extern Adafruit_SSD1306 display;
/** Trenutni kut animirane brave u radijanima. */
extern float vaultAngle;
/** Trenutna popunjenost trake od -1.0 do 1.0. */
extern float barFill;
/** Zadnja ocitana ADC vrijednost potenciometra. */
extern int lastValue;
/** Indeks cilja koji igrac trenutno pogadja. */
extern int currentTargetIndex;
/** Indeks trenutno odabrane stavke u glavnom meniju. */
extern int selectedLevelIndex;
/** Odabrana akcija na ekranu mreznih informacija. */
extern int selectedNetworkInfoAction;
/** Trenutni ekran aplikacije, jedna od GAME_SCREEN_* konstanti. */
extern int gameScreen;
/** Oznacava je li zadnji zavrseni level uspjesno rijesen. */
extern bool levelWasWon;
/** Kalibrirana neutralna ADC vrijednost joystick Y osi. */
extern int joystickCenterValue;
/** Oznacava smije li joystick ponovno pomaknuti meni. */
extern bool joystickMenuReady;
/** Vrijeme pocetka trenutnog levela iz millis(). */
extern unsigned long gameStartTime;
/** Vrijeme zavrsetka levela iz millis(); 0 dok level jos traje. */
extern unsigned long levelFinishedTime;
/** Vrijeme od kada je trenutni cilj unutar tolerancije. */
extern unsigned long targetHitStartTime;
/** Vrijeme zadnjeg pomaka joysticka kroz meni. */
extern unsigned long lastJoystickMoveTime;
/** Ukupno vrijeme rjesavanja zadnjeg levela u milisekundama. */
extern unsigned long levelSolvedTimeMs;
/** Stabilnost zadnjeg pokusaja od 0 do 100. */
extern int levelStabilityScore;
/** Broj uzoraka koristenih za stabilnost cijelog levela. */
extern int stabilitySampleCount;
/** Zbroj normaliziranih odstupanja od cilja za stabilnost levela. */
extern float stabilityErrorSum;
/** Broj prelazaka preko dozvoljenog cilja tijekom levela. */
extern int stabilityOvershootCount;
/** Broj promjena smjera potenciometra tijekom levela. */
extern int stabilityDirectionChanges;
/** Zadnji smjer pomaka potenciometra: -1, 0 ili 1. */
extern int lastPotDirection;
/** Broj ciljeva kopiranih za trenutni replay/score payload. */
extern int currentRunTargetCount;
/** Ciljne fill vrijednosti trenutnog pokusaja. */
extern float currentRunTargets[MAX_LEVEL_TARGETS];
/** Izracunata stabilnost za svaki cilj trenutnog pokusaja. */
extern int targetStabilityScore[MAX_LEVEL_TARGETS];
/** Broj uzoraka stabilnosti po cilju. */
extern int targetStabilitySampleCount[MAX_LEVEL_TARGETS];
/** Zbroj normaliziranih odstupanja po cilju. */
extern float targetStabilityErrorSum[MAX_LEVEL_TARGETS];
/** Broj overshoot dogadjaja po cilju. */
extern int targetStabilityOvershootCount[MAX_LEVEL_TARGETS];
/** Broj promjena smjera potenciometra po cilju. */
extern int targetStabilityDirectionChanges[MAX_LEVEL_TARGETS];
/** Zadnji smjer potenciometra zapamcen zasebno za svaki cilj. */
extern int targetLastPotDirection[MAX_LEVEL_TARGETS];
/** Broj spremljenih replay dogadjaja. */
extern int replayEventCount;
/** Vrijeme svakog replay dogadjaja relativno na pocetak levela. */
extern unsigned long replayEventTimeMs[MAX_REPLAY_EVENTS];
/** Fill vrijednost trake u svakom replay dogadjaju. */
extern float replayEventFill[MAX_REPLAY_EVENTS];
/** Indeks aktivnog cilja u svakom replay dogadjaju. */
extern int replayEventTargetIndex[MAX_REPLAY_EVENTS];
/** Tip svakog replay dogadjaja, jedna od REPLAY_EVENT_* konstanti. */
extern int replayEventType[MAX_REPLAY_EVENTS];
/** Apsolutno millis() vrijeme zadnjeg spremljenog replay dogadjaja. */
extern unsigned long lastReplayEventTime;
/** Fill vrijednost zadnjeg spremljenog replay dogadjaja. */
extern float lastReplayEventFill;
/** Zadnje vrijeme korisnicke aktivnosti iz millis(). */
extern unsigned long lastActivityTime;
/** Flag koji timer postavlja kada uredjaj treba u deep sleep. */
extern volatile bool sleepRequested;
/** ESP timer koji prati neaktivnost uredjaja. */
extern esp_timer_handle_t sleepTimer;
/** Oznacava radi li konfiguracijski WiFi portal. */
extern bool wifiConfigPortalActive;
/** Oznacava da je u tijeku pokusaj spajanja na WiFi. */
extern bool wifiConnectInProgress;
/** Oznacava postoje li spremljeni WiFi credentiali. */
extern bool wifiHasCredentials;
/** Oznacava da uredjaj namjerno radi bez WiFi slanja scoreova. */
extern bool wifiOfflineMode;
/** Vrijeme pocetka trenutnog WiFi spajanja iz millis(). */
extern unsigned long wifiConnectStartedAt;
/** Spremljeni ili trenutno uneseni WiFi SSID. */
extern String configuredSsid;
/** Spremljena ili trenutno unesena WiFi lozinka. */
extern String configuredPassword;
/** Spremljeni webhook URL za slanje scoreova. */
extern String configuredWebhookUrl;
/** SSID lokalnog konfiguracijskog AP-a. */
extern String wifiPortalSsid;
/** Detaljnija tekstualna poruka mreznog statusa. */
extern String networkStatusMessage;

#endif
