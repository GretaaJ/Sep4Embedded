#pragma once

#include <stdint.h>

typedef struct Sound Sound;
typedef struct Sound* Sound_t;

Sound_t createSound();
void measure(Sound_t self);
uint16_t getData(Sound_t self);
void sound_task(void* pvParameters);