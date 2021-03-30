/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 *
 * Main menu UI definitions
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL_pixels.h>

#include "../tank.h"
#include "ui.h"

extern void startGame();
extern struct Menu *currentMenu;
extern char *name;

static const struct SDL_Color titleColors[2] = {{196, 28, 4}, {0, 0, 0, 0}};

static const struct SDL_Color buttonColors[2] = {{204, 212, 195, 255},
												 {255, 255, 255, 255}};

static const int tankMaxSize[2] = {380, 350};

static const int tankBgMaxSize[2] = {300, 250};

static bool playingIntro = true;

static struct Menu mm;

static struct Label title;

static struct Image logo;
static struct Image logoBackdrop;

static struct Button startButton;
static struct Button helpButton;
static struct Button quitButton;

void startCallback() {
	menuDestroy(&mm);
	startGame();
}

void quitCallback() {
	quitSDL();
	exit(0);
}

void helpCallback() {
	puts("E: Not implemented");
}

void runIntroAnimationCircle(struct Image *target) {
	if (!playingIntro)
		return;

	if (target->location.w >= tankBgMaxSize[0] ||
		target->location.h >= tankBgMaxSize[1]) {
		playingIntro = false;
		return;
	}

	target->location.w += 15;
	target->location.h += 15;
}

void runIntroAnimationImage(struct Image *target) {
	if (!playingIntro)
		return;

	if (target->location.w >= tankMaxSize[0] ||
		target->location.h >= tankMaxSize[1]) {
		playingIntro = false;
		return;
	}

	target->location.w += 15;
	target->location.h += 15;
}

void createMainMenu() {
	menuInit(&mm);

	labelInit(&title, name, titleColors[0], titleColors[1], 480, 50, 300, 75);
	menuAddLabel(&mm, &title);

	imageInit(&logoBackdrop, "res/ui/mm/circle.png", 480, 180, 0, 0, 0);
	logoBackdrop.onTick = &runIntroAnimationCircle;
	menuAddImage(&mm, &logoBackdrop);

	imageInit(&logo, "res/tank.png", 520, 220, 0, 0, 0);
	logo.onTick = &runIntroAnimationImage;
	menuAddImage(&mm, &logo);

	buttonInit(&startButton, "Play", buttonColors[0], buttonColors[1], 980, 600,
			   200, 75);
	startButton.onClick = &startGame;
	menuAddButton(&mm, &startButton);

	buttonInit(&helpButton, "Help", buttonColors[0], buttonColors[1], 540, 600,
			   200, 75);
	helpButton.onClick = &helpCallback;
	menuAddButton(&mm, &helpButton);

	buttonInit(&quitButton, "Quit", buttonColors[0], buttonColors[1], 100, 600,
			   200, 75);
	quitButton.onClick = &quitCallback;
	menuAddButton(&mm, &quitButton);

	currentMenu = &mm;
}
