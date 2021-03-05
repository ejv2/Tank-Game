/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "tank.h"

extern struct SDL_Renderer *renderer;


static const char* tankTexture = "res/tank.png";
static const int tankSize = 60;


void tankInit(struct Player *player) {
	player->health = 100;

	player->x = 100;
	player->y = 100;
	player->heading = 0.0;

	player->texSurface = loadTexture(tankTexture);
	player->texture = SDL_CreateTextureFromSurface(renderer, player->texSurface);
}

void tankDestroy(struct Player *player) {
	SDL_DestroyTexture(player->texture);
	SDL_FreeSurface(player->texSurface);
}

void tankRender(struct Player *player) {
	struct SDL_Rect place = {
		player->x, player->y,
		tankSize, tankSize,
	};

	SDL_RenderCopyEx(renderer,
			player->texture,
			NULL, 
			&place,
			player->heading,
			NULL,
			SDL_FLIP_NONE);
}

void tankTick(struct Player *player, long milisTime) {
	if (isKeyDown((uint8_t)SDLK_UP)) {
		player->y--;
	}

	if (isKeyDown((uint8_t)SDLK_DOWN)) {
		player->y++;
	}

	if (isKeyDown((uint8_t)SDLK_RIGHT)) {
		player->x++;
	}

	if (isKeyDown((uint8_t)SDLK_LEFT)) {
		player->x--;
	}
}
