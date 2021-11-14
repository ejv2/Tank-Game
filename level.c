/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 *
 * Level and entity management routines
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tank.h"

extern struct SDL_Renderer *renderer;
static bool levelFileParse(FILE *fp, struct Level *level);

static char *ent_textures[] = {
	"res/ent/wall.png",
};
static int ent_sizes[][2] = {
	{64, 64},
};

static bool nodeDebounce = false;
static int node_textureCount = 1;
static char *node_textures[] = {"res/lvl/move.png"};
static struct SDL_Texture **node_loadedTextures;

static char *placeholderNode_path = "res/lvl/prompt.png";
static struct SDL_Texture *placeholderNode;

void levelInit(struct Level *level, struct Player *player, uint32_t levelID) {
	level->levelIndex = levelID;

	level->entityCount = 0;
	level->nodesUsed = 0;

	node_loadedTextures = malloc(sizeof(SDL_Texture *) * node_textureCount);
	for (int i = 0; i < node_textureCount; i++) {
		SDL_Surface *surf = loadTexture(node_textures[i]);
		node_loadedTextures[i] = SDL_CreateTextureFromSurface(renderer, surf);

		SDL_FreeSurface(surf);
	}

	SDL_Surface *placeholderSurf = loadTexture(placeholderNode_path);
	placeholderNode = SDL_CreateTextureFromSurface(renderer, placeholderSurf);

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

	player->x = level->startPoint[0];
	player->y = level->startPoint[1];
}

void levelDestroy(struct Level *level) {
	for (int i = 0; i < level->entityCount; i++) {
		free(level->ents[i]);
	}

	for (int j = 0; j < node_textureCount; j++) {
		SDL_DestroyTexture(node_loadedTextures[j]);
	}

	free(node_loadedTextures);
	free(level->nodes);
}

int addEntity(struct Level *level, enum EntityType type, uint8_t initialHealth,
			  bool canDamage, int x, int y, uint8_t orientation) {
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

void removeEntity(struct Level *level, unsigned int id) {
	level->ents[id]->isRemoved = true;
	level->entityCount--;
}

void levelRender(struct Level *level) {
	int x, y;
	struct SDL_Rect mouserect;

	SDL_GetMouseState(&x, &y);
	mouserect.w = 32;
	mouserect.h = 32;
	mouserect.x = x; /* - (mouserect.w / 2); */
	mouserect.y = y; /* - (mouserect.h / 2); */


	SDL_RenderCopyEx(renderer, placeholderNode, NULL, &mouserect, 0, NULL, SDL_FLIP_NONE);


	if (isMousePressed(1)) {
		if (!nodeDebounce) {
			if (level->nodesUsed < level->maxNodes) {
				nodeDebounce = true;

				level->nodes[level->nodesUsed].type = move;
				level->nodes[level->nodesUsed].x = x;
				level->nodes[level->nodesUsed].y = y;
				level->nodes[level->nodesUsed].orientation = 0;

				level->nodesUsed++;
			}
		}
	}

	if (!isMousePressed(1)) {
		nodeDebounce = false;
	}

	for (int i = 0; i < level->entityCount; i++) {
		struct Entity ent = *level->ents[i];

		struct SDL_Rect place = {
			ent.x,
			ent.y,
			ent_sizes[ent.type][0],
			ent_sizes[ent.type][1],
		};

		SDL_RenderCopyEx(renderer, ent.texture, NULL, &place,
						 (ent.orientation * 90.0), NULL, SDL_FLIP_NONE);
	}

	for (int j = 0; j < level->nodesUsed; j++) {
		struct TankNode node = level->nodes[j];
		struct SDL_Rect place = {level->nodes[j].x, level->nodes[j].y, 36, 36};

		SDL_RenderCopyEx(renderer, node_loadedTextures[node.type], NULL, &place,
						 node.orientation, NULL, SDL_FLIP_NONE);
	}
}

void levelTick(struct Level *level, long milisTime) {
}

static bool levelFileParse(FILE *fp, struct Level *level) {
	char curLine[PARSE_MAX_LINE_LENGTH];

	while (fgets(curLine, PARSE_MAX_LINE_LENGTH, fp) != NULL) {
		char cmd = curLine[0];
		char *data = curLine + 2;

		switch (cmd) {
		case 's': /* Start coordinate */
		{
			level->startPoint[0] = strtoimax(strtok(data, ","), NULL, 10);
			level->startPoint[1] = strtoimax(strtok(NULL, ","), NULL, 10);
			break;
		}
		case 'm': /* Node max */
		{
			level->maxNodes = strtoimax(data, NULL, 10);
			level->nodes = malloc(sizeof(struct TankNode) * level->maxNodes);
			break;
		}
		case 'w': /* Wall declaration */
		{
			int x = strtoimax(strtok(data, ","), NULL, 10);
			int y = strtoimax(strtok(NULL, ","), NULL, 10);
			int orientation = strtoimax(strtok(NULL, ","), NULL, 10);

			if (addEntity(level, wall, 100, false, x, y, (uint8_t)orientation) <
				0) {
				puts("E: Maximum entity count exceeded; could not declare "
					 "wall!");
				return false;
			}
			break;
		}
		case '#': /* Comment */
		{
			break;
		}
		case '\n': {
			break;
		}
		default: {
			return false;
		}
		}
	}

	return true;
}
