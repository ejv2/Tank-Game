/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 */

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "tank.h"

extern struct SDL_Renderer *renderer;
static bool levelFileParse(FILE *fp, struct Level* level);


static char* ent_textures[] = {
	"res/ent/wall.png",
};
static int ent_sizes[][2] = {
	{64, 64},
};


void levelInit(struct Level* level, uint32_t levelID) {
	level->levelIndex = levelID;

	level->entityCount = 0;

	char filename[50];
	snprintf(filename, 50, "levels/level%i.txt", levelID);
	level->levelFile = filename;

	FILE *lef = fopen(filename, "r");

	bool valid = levelFileParse(lef, level);
	if (!valid) {
		puts("E: Invalid level file detected");
		exit(-1);
	}
	
	fclose(lef);
}

void levelDestroy(struct Level* level) {
	for (int i = 0; i < level->entityCount; i++) {
		free(level->ents[i]);
	}
}

int addEntity(struct Level* level, enum EntityType type, uint8_t initialHealth, bool canDamage, int x, int y, uint8_t orientation) {
	level->entityCount++;
	if (level->entityCount > LVL_MAX_ENTITY_COUNT) {
		return -1;
	}

	struct Entity *ent = malloc(sizeof(struct Entity));
	ent->type = type;
	ent->health = initialHealth;
	ent->canDamage = canDamage;
	ent->x = x;
	ent->y = y;
	ent->orientation = orientation;

	ent->texSurface = loadTexture(ent_textures[type]);
	ent->texture = SDL_CreateTextureFromSurface(renderer, ent->texSurface);

	level->ents[level->entityCount - 1] = ent;

	return level->entityCount - 1;
}

void removeEntity(struct Level* level, unsigned int id) {
	level->ents[id]->isRemoved = true;
	level->entityCount--;
}

void levelRender(struct Level *level) {
	for (int i = 0; i < level->entityCount; i++) {
		struct Entity ent = *level->ents[i];
		
		struct SDL_Rect place = {
			ent.x, ent.y,
			ent_sizes[ent.type][0], ent_sizes[ent.type][1],
		};

		SDL_RenderCopyEx(renderer,
				ent.texture,
				NULL,
				&place,
				(ent.orientation * 90.0),
				NULL,
				SDL_FLIP_NONE);
	}
}

void levelTick(long milisTime) {

}

static bool levelFileParse(FILE *fp, struct Level* level) {
	char curLine[PARSE_MAX_LINE_LENGTH];

	while (fgets(curLine, PARSE_MAX_LINE_LENGTH, fp) != NULL) {
		char cmd = curLine[0];
		char* data = curLine + 2;

		switch (cmd) {
			case 's': /* Start coordinate */
			{
				level->startPoint[0] = atoi(strtok(data, ","));
				level->startPoint[1] = atoi(strtok(NULL, ","));
				break;
			}
			case 'w': /* Wall declaration */
			{
				int x = atoi(strtok(data, ","));
				int y = atoi(strtok(NULL, ","));
				int orientation = atoi(strtok(NULL, ","));
				
				if (addEntity(level, wall, 100, false, x, y, (uint8_t)orientation) < 0) {
					puts("E: Maximum entity count exceeded; could not declare wall!");
					return false;
				}
				break;
			}
			case '#': /* Comment */
			{
				break;
			}
			case '\n':
			{
				break;
			}
			default:
			{
				return false;
			}
		}
	}

	return true;
}
