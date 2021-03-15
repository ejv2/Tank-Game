/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 *
 * Input management routines
 */

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

static bool *keys = NULL;
static int keysRegistered = 0;

static bool mice[] = {false, false, false, false, false};

void updateKeys(char key, bool down) {
	uint8_t keyi = (int8_t)key;

	if (keys == NULL) {
		keys = malloc(sizeof(bool) * keyi);

		keysRegistered = (int)key;
		for (int i = 0; i < keysRegistered + 1; i++) {
			keys[i] = 0x0;
		}

		keys[keyi] = down;
	} else if (keyi > keysRegistered) {
		keys = realloc(keys, sizeof(bool) * keyi);

		int prevKeysRegistered = keysRegistered;
		keysRegistered = keyi;
		for (int i = prevKeysRegistered;
			 i < (keysRegistered - prevKeysRegistered) + 1; i++) {
			keys[i] = 0x0;
		}

		keys[keyi] = down;
	} else {
		keys[keyi] = down;
	}
}

void updateMice(uint8_t button, bool pressed) {
	mice[button] = pressed;
}

bool isKeyDown(char key) {
	int8_t keyi = (int8_t)key;
	if (keyi > keysRegistered) {
		return false;
	} else {
		return keys[keyi];
	}
}

bool isMousePressed(uint8_t button) {
	return mice[button];
}
