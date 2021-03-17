/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 *
 * Menu definitions used for control testing
 */

#include <stdbool.h>
#include <stdint.h>

#include "../tank.h"
#include "ui.h"

extern struct Menu *currentMenu;

static struct Menu test;

static struct Label label;
static struct Button button;
static struct Image image;

static const SDL_Color fg = {0, 0, 255};
static const SDL_Color bg = {255, 0, 0};
static const char *tex = "res/tank.png";

static bool tankReverse = false;

/*
** Used to test onFrame and onTick hooks
*/
static void imageAnimUpdate(struct Image *target) {
	target->rotation += 10;

	if (tankReverse) {
		if (target->location.x <= 100)
			tankReverse = false;

		target->location.x--;
	} else {
		if (target->location.x >= 1250)
			tankReverse = true;

		target->location.x++;
	}
}

/*
** Responsible for creating and returning a menu, along
** with updating the pointer which will allow us to render it
**
** The main translation unit is responsible for freeing/destroying
** the previous menu which was being rendered. It can choose to or
** not to do this, depending on what behaviour it wishes
*/
void createTestMenu() {
	menuInit(&test);

	labelInit(&label, "Test label", fg, bg, 100, 100, 300, 75);
	menuAddLabel(&test, &label);

	buttonInit(&button, "Test button", bg, fg, 100, 200, 300, 100);
	menuAddButton(&test, &button);

	imageInit(&image, (char *)tex, 100, 300, 200, 200, 0);
	image.onTick = &imageAnimUpdate;
	menuAddImage(&test, &image);

	currentMenu = &test;
}
