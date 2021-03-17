/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 *
 * Main menu UI definitions
 */

#include "../tank.h"
#include "ui.h"

extern struct Menu *currentMenu;

static struct Menu mm;

void createMainMenu() {
	menuInit(&mm);

	currentMenu = &mm;
}
