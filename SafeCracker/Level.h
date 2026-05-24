#ifndef LEVEL_H
#define LEVEL_H

struct LevelTarget {
  float fillAmount;
};

struct Level {
  const char* name;
  const LevelTarget* targets;
  int targetCount;
  unsigned long durationMs;
};

const LevelTarget FIRST_LEVEL_TARGETS[] = {
  {0.65},
  {-0.45},
  {0.85},
  {-0.7},
  {0.25}
};

const LevelTarget SECOND_LEVEL_TARGETS[] = {
  {-0.25},
  {0.45},
  {-0.85},
  {0.75},
  {-0.55},
  {0.95}
};

const Level FIRST_LEVEL = {
  "Level 1",
  FIRST_LEVEL_TARGETS,
  sizeof(FIRST_LEVEL_TARGETS) / sizeof(FIRST_LEVEL_TARGETS[0]),
  120000
};

const Level SECOND_LEVEL = {
  "Level 2",
  SECOND_LEVEL_TARGETS,
  sizeof(SECOND_LEVEL_TARGETS) / sizeof(SECOND_LEVEL_TARGETS[0]),
  90000
};

const Level LEVELS[] = {
  FIRST_LEVEL,
  SECOND_LEVEL
};

const int LEVEL_COUNT = sizeof(LEVELS) / sizeof(LEVELS[0]);

#endif
