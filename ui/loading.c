/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 *
 * Loading menu definitions
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL_pixels.h>

#include "../tank.h"
#include "ui.h"

extern struct Menu *currentMenu;

static struct Menu loader;

static const struct SDL_Color cols[2] = {{255, 0, 0, 255}, {0, 0, 0, 0}};
static struct Label text;

void createLoaderMenu() {
	menuInit(&loader);

	labelInit(&text, "Loading...", cols[0], cols[1], 450, 70, 500, 150);
	menuAddLabel(&loader, &text);

	currentMenu = &loader;
}
