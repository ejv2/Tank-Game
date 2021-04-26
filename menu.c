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
	for (int i = 0; i < menu->labelCount; i++) {
		labelTick(menu->labels[i]);
	}

	for (int j = 0; j < menu->buttonCount; j++) {
		buttonTick(menu->buttons[j]);
	}

	for (int k = 0; k < menu->imageCount; k++) {
		imageTick(menu->images[k]);
	}
}

void menuRender(struct Menu *menu) {
	for (int i = 0; i < menu->labelCount; i++) {
		labelRender(menu->labels[i]);
	}

	for (int j = 0; j < menu->buttonCount; j++) {
		buttonRender(menu->buttons[j]);
	}

	for (int k = 0; k < menu->imageCount; k++) {
		imageRender(menu->images[k]);
	}

	for (int l = 0; l < menu->partialImageCount; l++) {
		partialImageRender(menu->partialImages[l]);
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

void menuAddPartialImage(struct Menu *menu, struct PartialImage *image) {
	menu->partialImageCount++;
	if (menu->imageCount > UI_MAX_HUD_ELEMS) {
		puts("E: Maximum UI elements exceeded; could not register p_image");
		exit(1);
	}

	menu->partialImages[menu->partialImageCount - 1] = image;
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

	label->onFrame = NULL;
	label->onTick = NULL;
}

void labelDestroy(struct Label *label) {
	SDL_DestroyTexture(label->texture);
}

void labelRender(struct Label *label) {
	if (label->onFrame)
		label->onFrame(label);

	SDL_RenderCopy(renderer, label->texture, NULL, &label->location);
}

void labelTick(struct Label *label) {
	if (label->onTick)
		label->onTick(label);
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

	button->onFrame = NULL;
	button->onTick = NULL;
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

	if (button->onFrame)
		button->onFrame(button);

	SDL_RenderCopy(renderer, backTexture, NULL, &button->place);
	SDL_RenderCopy(renderer, textTexture, NULL, &button->place);
}

void buttonTick(struct Button *button) {
	if (button->onTick)
		button->onTick(button);
}

void imageInit(struct Image *image, char *texturePath, int x, int y, int w,
			   int h, float rot) {
	SDL_Surface *texSurf = loadTexture(texturePath);
	image->imageTexture = SDL_CreateTextureFromSurface(renderer, texSurf);
	SDL_FreeSurface(texSurf);

	image->location.x = x;
	image->location.y = y;
	image->location.w = w;
	image->location.h = h;
	image->rotation = rot;

	image->onFrame = NULL;
	image->onTick = NULL;
}

void imageDestroy(struct Image *image) {
	SDL_DestroyTexture(image->imageTexture);
}

void imageRender(struct Image *image) {
	if (image->onFrame)
		image->onFrame(image);

	SDL_RenderCopyEx(renderer, image->imageTexture, NULL, &image->location,
					 image->rotation, NULL, SDL_FLIP_NONE);
}

void imageTick(struct Image *image) {
	if (image->onTick)
		image->onTick(image);
}

void partialImageInit(struct PartialImage *image, char *texturePath, int x,
					  int y, int w, int h, int imageX, int imageY, int imageW,
					  int imageH, float rot) {
	SDL_Surface *texSurf = loadTexture(texturePath);
	image->imageTexture = SDL_CreateTextureFromSurface(renderer, texSurf);
	SDL_FreeSurface(texSurf);

	image->location.x = x;
	image->location.y = y;
	image->location.w = w;
	image->location.h = h;
	image->rotation = rot;

	image->imagePortion.x = imageX;
	image->imagePortion.y = imageY;
	image->imagePortion.w = imageW;
	image->imagePortion.h = imageH;

	image->onFrame = NULL;
	image->onTick = NULL;
}

void partialImageDestroy(struct PartialImage *image) {
	SDL_DestroyTexture(image->imageTexture);
}

void partialImageRender(struct PartialImage *image) {
	if (image->onFrame)
		image->onFrame(image);

	SDL_RenderCopyEx(renderer, image->imageTexture, &image->imagePortion,
					 &image->location, image->rotation, NULL, SDL_FLIP_NONE);
}

void partialImageTick(struct PartialImage *image) {
	if (image->onTick)
		image->onTick(image);
}
