/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 *
 * Shared function forward definitions
 */

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#include "tank.h"

extern struct Menu currentMenu;
extern enum GameState state;


void menuTick() {
    if (state != fsMenu || state != olMenu)
        return;
}

void menuRender () {
    if (state != fsMenu || state != olMenu)
        return;
}
