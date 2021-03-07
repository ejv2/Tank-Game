/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 *
 * Shared function forward definitions
 */

#ifndef TANK_H_INCLUDED
#define TANK_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#define PARSE_MAX_LINE_LENGTH 50
#define LVL_MAX_ENTITY_COUNT 1000
#define UI_MAX_HUD_ELEMS 75

struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;
struct FILE;

// General
void printBanner();
void printHelp();

void initSDL(uint32_t systems);
void quitSDL();

void init();
void render();
void tick();

// Game state
enum GameState {
	fsMenu = 1, /* Full screen menu */
	olMenu = 2, /* Overlay menu */
	game = 3,  /* Playing the main game */
	failure = 4,/* You failed */
	success = 5,/* You won */
};

// Menus
enum ControlType {
	label = 1,
	button = 2,
	picture = 3,
};

struct Menu {
	bool fullScreen;
	enum ControlType controls[UI_MAX_HUD_ELEMS];
};

// Util
struct SDL_Surface *loadTexture(const char *texPath);

// Level manager
enum EntityType { wall = 0, enemy = 1, goal = 2 };

struct Entity {
	enum EntityType type;

	uint8_t health;
	bool canDamage;

	int x, y;
	uint8_t orientation;

	bool isRemoved;

	struct SDL_Surface *texSurface;
	struct SDL_Texture *texture;
};

struct Level {
	int levelIndex;
	char *levelFile;

	int startPoint[2];

	uint32_t entityCount;
	struct Entity *ents[LVL_MAX_ENTITY_COUNT];
};

void levelInit(struct Level *level, uint32_t levelID);
void levelDestroy(struct Level *level);
int addEntity(struct Level *level, enum EntityType type, uint8_t initialHealth,
			  bool canDamage, int x, int y, uint8_t oriantation);
void removeEntity();

void levelRender(struct Level *level);
void levelTick(long milisTime);

// Tank/player manager
struct Player {
	uint8_t health;

	int x, y;
	double heading;

	struct SDL_Surface *texSurface;
	struct SDL_Texture *texture;
};

void tankInit(struct Player *player);
void tankDestroy(struct Player *player);

void tankRender(struct Player *player);
void tankTick(struct Player *player, long milisTime);

// Input handler
void updateKeys(char key, bool down);
void updateMice(uint8_t button, bool pressed);
bool isKeyDown(char key);
bool isMousePressed(uint8_t button);

#endif
