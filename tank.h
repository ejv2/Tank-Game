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

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

#define PARSE_MAX_LINE_LENGTH 50
#define LVL_MAX_ENTITY_COUNT 1000
#define UI_MAX_HUD_ELEMS 75

/* General */
void printBanner();
void printHelp();

void initSDL(uint32_t systems);
void quitSDL();

void init();
void startGame();

void render();
void tick();

/* Game state */
enum GameState {
	fsMenu = 1,	 /* Full screen menu */
	olMenu = 2,	 /* Overlay menu */
	game = 3,	 /* Playing the main game */
	failure = 4, /* You failed */
	success = 5, /* You won */
};

/* Menus */
struct Label {
	SDL_Rect location;

	char *text;
	struct SDL_Color color[2];

	struct SDL_Texture *texture;

	void (*onFrame)(struct Label *target);
	void (*onTick)(struct Label *target);
};

struct Button {
	bool focused;

	bool wasFocused;
	bool wasClicked;

	struct SDL_Rect place;

	char *text;

	struct SDL_Texture *focusTextTex;
	struct SDL_Texture *focusBackTex;

	struct SDL_Texture *unfocusTextTex;
	struct SDL_Texture *unfocusBackTex;

	void (*onFocus)();
	void (*onClick)();
	void (*onFrame)(struct Button *target);
	void (*onTick)(struct Button *target);
};

struct Image {
	SDL_Rect location;
	float rotation;

	struct SDL_Texture *imageTexture;

	void (*onFrame)(struct Image *target);
	void (*onTick)(struct Image *target);
};

struct PartialImage {
	SDL_Rect location;
	SDL_Rect imagePortion;

	float rotation;

	struct SDL_Texture *imageTexture;

	void (*onFrame)(struct PartialImage *target);
	void (*onTick)(struct PartialImage *target);
};

struct Menu {
	bool fullScreen;

	uint8_t labelCount;
	struct Label *labels[UI_MAX_HUD_ELEMS];

	uint8_t buttonCount;
	struct Button *buttons[UI_MAX_HUD_ELEMS];

	uint8_t imageCount;
	struct Image *images[UI_MAX_HUD_ELEMS];

	uint8_t partialImageCount;
	struct PartialImage *partialImages[UI_MAX_HUD_ELEMS];
};

void menuInit(struct Menu *menu);
void menuDestroy(struct Menu *menu);
void menuRender(struct Menu *menu);
void menuTick(struct Menu *menu);

void menuAddLabel(struct Menu *menu, struct Label *label);
void menuAddButton(struct Menu *menu, struct Button *button);
void menuAddImage(struct Menu *menu, struct Image *image);
void menuAddPartialImage(struct Menu *menu, struct PartialImage *image);

void labelInit(struct Label *label, char *text, struct SDL_Color fg,
			   struct SDL_Color bg, int x, int y, int w, int h);
void labelDestroy(struct Label *label);
void labelRender(struct Label *label);
void labelTick(struct Label *label);

void buttonInit(struct Button *button, char *text,
				struct SDL_Color focusTextCol, struct SDL_Color unfocusTextCol,
				int x, int y, int w, int h);
void buttonDestroy(struct Button *button);
void buttonRender(struct Button *button);
void buttonTick(struct Button *button);

void imageInit(struct Image *image, char *texturePath, int x, int y, int w,
			   int h, float rot);
void imageDestroy(struct Image *image);
void imageRender(struct Image *image);
void imageTick(struct Image *image);

void partialImageInit(struct PartialImage *image, char *texturePath, int x,
					  int y, int w, int h, int imageX, int imageY, int imageW,
					  int imageH, float rot);
void partialImageDestroy(struct PartialImage *image);
void partialImageRender(struct PartialImage *image);
void partialImageTick(struct PartialImage *image);

/* Util */
struct SDL_Surface *loadTexture(const char *texPath);
void initRandom();
int randint(int min, int max);

/* Tank/player manager */
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

/* Level manager */
enum EntityType { wall = 0, enemy = 1, goal = 2 };
enum NodeType { move = 0, turn = 1 };

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

struct TankNode {
	int x, y;
	double orientation;

	enum NodeType type;
};

struct Level {
	int levelIndex;
	char *levelFile;

	int startPoint[2];

	uint32_t entityCount;
	struct Entity *ents[LVL_MAX_ENTITY_COUNT];

	int maxNodes;
	int nodesUsed;
	struct TankNode *nodes;
};

void levelInit(struct Level *level, struct Player *player, uint32_t levelID);
void levelDestroy(struct Level *level);
int addEntity(struct Level *level, enum EntityType type, uint8_t initialHealth,
			  bool canDamage, int x, int y, uint8_t oriantation);
void removeEntity();

void levelRender(struct Level *level);
void levelTick(struct Level *level, long milisTime);

/* Input handler */
void updateKeys(char key, bool down);
void updateMice(uint8_t button, bool pressed);
bool isKeyDown(char key);
bool isMousePressed(uint8_t button);

#endif
