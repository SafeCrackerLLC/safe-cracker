#ifndef LEVEL_H
#define LEVEL_H

/**
 * @file Level.h
 * @brief Definicije levela i ciljnih polozaja trake.
 */

/** @brief Jedan cilj koji igrac mora pogoditi na traci okretanja. */
struct LevelTarget {
  /** Popunjenost trake od -1.0 do 1.0; negativno je lijevo, pozitivno desno. */
  float fillAmount;
};

/** @brief Konfiguracija jednog levela igre. */
struct Level {
  /** Naziv levela prikazan u meniju i poslan na leaderboard. */
  const char* name;
  /** Niz ciljeva koje igrac mora pogoditi redom. */
  const LevelTarget* targets;
  /** Broj ciljeva u nizu. */
  int targetCount;
  /** Maksimalno trajanje levela u milisekundama. */
  unsigned long durationMs;
};

/** Ciljevi prvog levela, redom kako ih igrac treba pogoditi. */
const LevelTarget FIRST_LEVEL_TARGETS[] = {
  {0.65},
  {-0.45},
  {0.85},
  {-0.7},
  {0.25}
};

/** Ciljevi drugog levela, redom kako ih igrac treba pogoditi. */
const LevelTarget SECOND_LEVEL_TARGETS[] = {
  {-0.25},
  {0.45},
  {-0.85},
  {0.75},
  {-0.55},
  {0.95}
};

/** Konfiguracija prvog levela. */
const Level FIRST_LEVEL = {
  "Level 1",
  FIRST_LEVEL_TARGETS,
  sizeof(FIRST_LEVEL_TARGETS) / sizeof(FIRST_LEVEL_TARGETS[0]),
  120000
};

/** Konfiguracija drugog levela. */
const Level SECOND_LEVEL = {
  "Level 2",
  SECOND_LEVEL_TARGETS,
  sizeof(SECOND_LEVEL_TARGETS) / sizeof(SECOND_LEVEL_TARGETS[0]),
  90000
};

/** Popis svih levela prikazanih u glavnom meniju. */
const Level LEVELS[] = {
  FIRST_LEVEL,
  SECOND_LEVEL
};

/** Ukupan broj dostupnih levela. */
const int LEVEL_COUNT = sizeof(LEVELS) / sizeof(LEVELS[0]);

#endif
