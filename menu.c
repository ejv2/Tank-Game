/*
 * Ethan Marshall's Tank Game
 * Authored in Winter 2021 instead of a boring computing project
 * Copyright 2021 - Ethan Marshall
 *
 * Menu and UI control routines
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>

#include "tank.h"

extern enum GameState state;
extern struct SDL_Renderer *renderer;
extern TTF_Font *programFont;

static const char *buttonBackgroundTexture = "res/ui/btn.png";
static const char *buttonFocusBackgroundTexture = "res/ui/fbtn.png";

void menuInit(struct Menu *menu) {
	menu->labelCount = 0;
	menu->buttonCount = 0;
	menu->imageCount = 0;
}

void menuDestroy(struct Menu *menu) {
	for (int i = 0; i < menu->labelCount; i++) {
		labelDestroy(menu->labels[i]);
	}

	for (int j = 0; j < menu->buttonCount; j++) {
		buttonDestroy(menu->buttons[j]);
	}

	for (int k = 0; k < menu->imageCount; k++) {
		imageDestroy(menu->images[k]);
	}
}

void menuTick(struct Menu *menu) {
}

void menuRender(struct Menu *menu) {
	for (int i = 0; i < menu->labelCount; i++) {
		labelRender(menu->labels[i]);
	}

	for (int j = 0; j < menu->buttonCount; j++) {
		buttonRender(menu->buttons[j]);
	}

	for (int k = 0; k < menu->imageCount; k++) {
		imageDestroy(menu->images[k]);
	}
}

void menuAddLabel(struct Menu *menu, struct Label *label) {
	menu->labelCount++;
	if (menu->labelCount > UI_MAX_HUD_ELEMS) {
		puts("E: Maximum UI elements exceeded; could not register label");
		exit(1);
	}

	menu->labels[menu->labelCount - 1] = label;
}

void menuAddButton(struct Menu *menu, struct Button *button) {
	menu->buttonCount++;
	if (menu->buttonCount > UI_MAX_HUD_ELEMS) {
		puts("E: Maximum UI elements exceeded; could not register button");
		exit(1);
	}

	menu->buttons[menu->buttonCount - 1] = button;
}

void menuAddImage(struct Menu *menu, struct Image *image) {
	menu->imageCount++;
	if (menu->imageCount > UI_MAX_HUD_ELEMS) {
		puts("E: Maximum UI elements exceeded; could not register image");
		exit(1);
	}

	menu->images[menu->imageCount - 1] = image;
}

void labelInit(struct Label *label, char *text, struct SDL_Color fg,
			   struct SDL_Color bg, int x, int y, int w, int h) {
	SDL_Surface *tSurf = TTF_RenderText_Shaded(programFont, text, fg, bg);
	label->texture = SDL_CreateTextureFromSurface(renderer, tSurf);
	SDL_FreeSurface(tSurf);

	label->location.x = x;
	label->location.y = y;
	label->location.w = w;
	label->location.h = h;

	label->text = text;
	label->color[0] = fg;
	label->color[1] = bg;
}

void labelDestroy(struct Label *label) {
	SDL_DestroyTexture(label->texture);
}

void labelRender(struct Label *label) {
	SDL_RenderCopy(renderer, label->texture, NULL, &label->location);
}

void labelTick(struct Label *label) {
}

void buttonInit(struct Button *button, char *text,
				struct SDL_Color focusTextCol, struct SDL_Color unfocusTextCol,
				int x, int y, int w, int h) {
	SDL_Surface *tuSurf =
		TTF_RenderText_Solid(programFont, text, unfocusTextCol);
	SDL_Surface *tfSurf = TTF_RenderText_Solid(programFont, text, focusTextCol);

	SDL_Surface *ubgSurf = loadTexture(buttonBackgroundTexture);
	SDL_Surface *fbgSurf = loadTexture(buttonFocusBackgroundTexture);

	button->focusBackTex = SDL_CreateTextureFromSurface(renderer, fbgSurf);
	button->focusTextTex = SDL_CreateTextureFromSurface(renderer, tfSurf);
	button->unfocusBackTex = SDL_CreateTextureFromSurface(renderer, ubgSurf);
	button->unfocusTextTex = SDL_CreateTextureFromSurface(renderer, tuSurf);

	SDL_FreeSurface(tuSurf);
	SDL_FreeSurface(tfSurf);
	SDL_FreeSurface(ubgSurf);
	SDL_FreeSurface(fbgSurf);

	button->place.x = x;
	button->place.y = y;
	button->place.w = w;
	button->place.h = h;

	button->focused = false;

	button->wasFocused = false;
	button->wasClicked = false;

	button->onClick = NULL;
	button->onFocus = NULL;
}

void buttonDestroy(struct Button *button) {
	SDL_DestroyTexture(button->focusBackTex);
	SDL_DestroyTexture(button->focusTextTex);

	SDL_DestroyTexture(button->unfocusBackTex);
	SDL_DestroyTexture(button->unfocusTextTex);
}

void buttonRender(struct Button *button) {
	SDL_Point mouseP;
	uint32_t but = SDL_GetMouseState(&mouseP.x, &mouseP.y);

	button->focused = SDL_PointInRect(&mouseP, &button->place);

	SDL_Texture *backTexture;
	SDL_Texture *textTexture;
	if (button->focused) {
		backTexture = button->focusBackTex;
		textTexture = button->focusTextTex;

		/* Call frame-perfect hooks */
		if (!button->wasFocused && button->onFocus) {
			button->wasFocused = true;
			button->onFocus();
		}

		if (but & SDL_BUTTON(SDL_BUTTON_LEFT) && button->onClick) {
			if (!button->wasClicked)
				button->onClick();
			button->wasClicked = true;
		} else {
			button->wasClicked = false;
		}
	} else {
		button->wasFocused = false;

		backTexture = button->unfocusBackTex;
		textTexture = button->unfocusTextTex;
	}

	SDL_RenderCopy(renderer, backTexture, NULL, &button->place);
	SDL_RenderCopy(renderer, textTexture, NULL, &button->place);
}

void buttonTick(struct Button *button) {
}

void imageInit(struct Image *image, char *texturePath) {
}

void imageDestroy(struct Image *image) {
}

void imageRender(struct Image *image) {
}

void imageTick(struct Image *image) {
}
