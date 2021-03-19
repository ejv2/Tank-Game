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

		keysRegistered = keyi;
		memset(keys, 0x0, keysRegistered);

		keys[keyi] = down;
	} else if (keyi > keysRegistered) {
		keys = realloc(keys, sizeof(bool) * keyi);
		keysRegistered = keyi;

		/* ALERT: Hacky code warning!
		 *
		 * TODO: Find out a better way to do this (that doesn't crash the
		 * program)
		 *
		 * We are zeroing the entire keymap dictionary here!
		 * This will release all keys that the user is currently pressing,
		 * causing a possible visual delay/desync.
		 *
		 * The way I was doing this was to zero out the newly allocated keys,
		 * but it appears that was causing segfaults and random aborts from
		 * GLib.
		 *
		 * For now, I am going to stick to just re-initialising the whole
		 * dictionary.
		 * */
		memset(keys, 0x0, keysRegistered);

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
