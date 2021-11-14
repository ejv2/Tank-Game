/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "tank.h"
#include "ui/ui.h"

static const uint32_t sdl_systems =
	SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER;
static const uint32_t sdl_winflags = 0;
static const uint32_t sdl_rendflags = SDL_RENDERER_ACCELERATED;

const char *name = "Tank Game";
static const int x = SDL_WINDOWPOS_CENTERED;
static const int y = SDL_WINDOWPOS_CENTERED;
static const int w = 1280;
static const int h = 720;

static double maxtps = 60.0;

bool running = false;
bool focused = true;

uint32_t ticks = 0;
uint32_t frames = 0;

uint64_t tickCount = 0;
enum GameState state = fsMenu;
struct Menu *currentMenu;

int currentLevel = 1;
static struct Player player;
static struct Level level;

struct SDL_Window *window;
struct SDL_Renderer *renderer;

static const char *programFontPath = "res/joystix.ttf";
TTF_Font *programFont;

void printBanner() {
	puts("Tank Game - A Really Simply SDL Game");
	puts("By Ethan Marshall - 2021");
}

void printHelp() {
	printBanner();
	puts("\nRun with no arguments to run the stock game");
	puts("Run with arguments to set options for the game:\n");
}

void initSDL(uint32_t systems) {
	if (SDL_Init(systems)) {
		printf("E: Failed to setup SDL!\nError message: %s\n", SDL_GetError());
		exit(1);
	}

	if (TTF_Init() == -1) {
		printf(
			"E: Failed to initialise font loading engine!\nError message: %s\n",
			TTF_GetError());
		exit(1);
	}

	programFont = TTF_OpenFont(programFontPath, 72);

	if (!programFont) {
		printf("E: Failed to load required game fonts! Check your game "
			   "install\nError message: %s\n",
			   TTF_GetError());
		exit(1);
	}

	window = SDL_CreateWindow(name, x, y, w, h, sdl_winflags);
	if (!window) {
		printf("E: Failed to set up display!\nError message: %s\n",
			   SDL_GetError());
		quitSDL();
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, sdl_rendflags);
	if (!renderer) {
		printf("E: Failed to set up renderer!\nError message: %s\n",
			   SDL_GetError());
		quitSDL();
		exit(1);
	}
}

void quitSDL() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_CloseFont(programFont);
	TTF_Quit();

	SDL_Quit();

	switch (state) {
	case game:
		levelDestroy(&level);
		break;
	case fsMenu: /* FALLTHROUGH */
	case olMenu:
		menuDestroy(currentMenu);
		break;
	default: /* This is fine */
		break;
	}
}

void startGame() {
	menuDestroy(currentMenu);

	state = game;
	currentMenu = NULL;

	tankInit(&player);
	levelInit(&level, &player, currentLevel);
}

void init() {
	initRandom();

	running = true;

	state = fsMenu;
	createMainMenu();
}

void render() {
	SDL_RenderClear(renderer);

	switch (state) {
	case fsMenu:
		menuRender(currentMenu);
		break;
	case olMenu:
		menuRender(currentMenu);

		tankRender(&player);
		levelRender(&level);
		break;
	case game:
		tankRender(&player);
		levelRender(&level);
		break;
	case failure:
		break;
	case success:
		break;
	default:
		puts("BUG: Unknown game state!");
		exit(1);
	}

	SDL_RenderPresent(renderer);
}

void tick() {
	tickCount++;

	long now = SDL_GetTicks();

	switch (state) {
	case fsMenu:
		menuTick(currentMenu);
		break;
	case olMenu:
		menuTick(currentMenu);
		levelTick(&level, now);
		tankTick(&player, now);
		break;
	case game:
		levelTick(&level, now);
		tankTick(&player, now);
	case failure:
		break;
	case success:
		break;
	default:
		puts("BUG: Unknown game state!");
		exit(1);
	}
}

void handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e) > 0) {
		switch (e.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			updateKeys(e.key.keysym.sym, true);
			break;
		case SDL_KEYUP:
			updateKeys(e.key.keysym.sym, false);
			break;
		case SDL_MOUSEBUTTONDOWN:
			updateMice(e.button.button, true);
			break;
		case SDL_MOUSEBUTTONUP:
			updateMice(e.button.button, false);
			break;
		case SDL_WINDOWEVENT:
			switch (e.window.event) {
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				focused = true;
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				focused = false;
				break;
			default:
				break;
			}
			break;
		}
	}
}

int main(int argc, char **argv) {
	printBanner();
	initSDL(sdl_systems);

	long milisTime = SDL_GetTicks();
	long lastTime = SDL_GetPerformanceCounter();
	double unprocessed = 0;
	double tickInterval = (double)SDL_GetPerformanceFrequency() / maxtps;

	init();

	while (running) {
		long now = SDL_GetPerformanceCounter();
		unprocessed += (double)(now - lastTime) / tickInterval;
		lastTime = now;

		while (unprocessed >= 1) {
			ticks++;
			tick();
			unprocessed -= 1;
		}

		SDL_Delay(2);

		frames++;
		render();
		handleEvents();

		if (SDL_GetTicks() - milisTime > 1000) {
			milisTime += 1000;
			printf("DEBUG: %i ticks, approx %i fps\n", ticks, frames);

			frames = 0;
			ticks = 0;
		}
	}

	quitSDL();
}
