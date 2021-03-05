/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "tank.h"

struct SDL_Surface* loadTexture(const char* texPath) {
	SDL_Surface *image = IMG_Load(texPath);

	if (!image) {
		printf("E: One or more textures are missing. Please check your game installation\n");
		exit(-1);
	}

	return image;
}
