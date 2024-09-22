/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-22
 */

#include "../screen.h"

#include "screen-intern.h"

bool setup_screen_struct(Screen* screen, char title[], int width, int height)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		return false;
	}

	if(IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Quit();

		return false;
	}

	if(TTF_Init() == -1)
	{
		SDL_Quit();
		IMG_Quit();

		return false;
	}

	if(Mix_Init(0) != 0)
	{
		SDL_Quit();
		IMG_Quit();
		TTF_Quit();

		return false;
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	screen->width = width;
	screen->height = height;

	if(!make_screen_window(&screen->window, title, screen->width, screen->height))
	{
		SDL_Quit();
		IMG_Quit();
		TTF_Quit();
		Mix_Quit();

		return false;
	}

	screen->render = SDL_CreateRenderer(screen->window, -1, 0);

	return true;
}

bool make_screen_window(SDL_Window** window, char title[], int width, int height)
{
  *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	return (*window != NULL);
}

void free_screen_struct(Screen screen)
{
	SDL_DestroyRenderer(screen.render);

	SDL_DestroyWindow(screen.window);

	SDL_Quit();
}


void free_sounds_struct(Sounds sounds)
{
	Mix_FreeChunk(sounds.unlockEffect);

	Mix_FreeChunk(sounds.flagEffect);

	Mix_FreeChunk(sounds.winEffect);

	Mix_FreeChunk(sounds.loseEffect);

	Mix_Quit();
}

void free_images_struct(Images images)
{
	for(int index = 0; index < 8; index += 1)
	{
		SDL_FreeSurface(images.nextSymbols[index]);
	}

	SDL_FreeSurface(images.mineSymbol);

	SDL_FreeSurface(images.flagSymbol);

	SDL_FreeSurface(images.intactSquare);

	SDL_FreeSurface(images.blastSquare);

	SDL_FreeSurface(images.sweptSquare);

	IMG_Quit();
}

void free_fonts_struct(Fonts fonts)
{
	TTF_CloseFont(fonts.timeFont);

	TTF_Quit();
}

void free_gui_struct(GUI gui)
{
  info_print("Freeing gui");

	free_screen_struct(gui.screen);

	free_images_struct(gui.images);

	free_sounds_struct(gui.sounds);

	free_fonts_struct(gui.fonts);

  info_print("Freed gui");
}

bool setup_sounds_struct(Sounds* sounds)
{
	sounds->unlockEffect = Mix_LoadWAV("../assets/sounds/square-click-sound.wav");
  sounds->flagEffect = Mix_LoadWAV("../assets/sounds/square-flag-sound.wav");
  sounds->winEffect = Mix_LoadWAV("../assets/sounds/win-result-sound.wav");
  sounds->loseEffect = Mix_LoadWAV("../assets/sounds/lose-result-sound.wav");

	return true;
}

bool setup_fonts_struct(Fonts* fonts)
{
	if(!extract_file_font(&fonts->timeFont, "../assets/fonts/8Bit-font.ttf", 24))
	{
		printf("Could not extract font\n");
	}

	return true;
}

bool setup_images_struct(Images* images)
{
	char* nextStrings[] = {"one", "two", "three", "four", "five", "six", "seven", "eight"};

  for(int index = 0; index < 8; index += 1)
  {
    char filePath[200];

    sprintf(filePath, "%s/%s-symbol.png", SCREEN_IMAGE_FOLDER, nextStrings[index]);

    extract_file_image(&images->nextSymbols[index], filePath);
  }

  extract_file_image(&images->mineSymbol, "../assets/images/mine-symbol.png");
  extract_file_image(&images->flagSymbol, "../assets/images/flag-symbol.png");
  extract_file_image(&images->intactSquare, "../assets/images/intact-square.png");
  extract_file_image(&images->blastSquare, "../assets/images/blast-square.png");
  extract_file_image(&images->sweptSquare, "../assets/images/swept-square.png");
	extract_file_image(&images->fieldGround, "../assets/images/field-ground.png");
	extract_file_image(&images->boardGround, "../assets/images/board-ground.png");

	return true;
}

bool setup_gui_struct(GUI* gui, char title[], int width, int height)
{
	if(!setup_screen_struct(&gui->screen, title, width, height))
  {
    printf("Could not setup_display_screen!\n");

    return false;
  }
	if(!setup_sounds_struct(&gui->sounds))
  {
    printf("Could not setup_sounds_struct\n");

		free_screen_struct(gui->screen);

		return false;
  }

  if(!setup_images_struct(&gui->images))
  {
		printf("Could not setup_images_struct\n");

		free_screen_struct(gui->screen);

		free_sounds_struct(gui->sounds);

		return false;
  }

	if(!setup_fonts_struct(&gui->fonts))
	{
		printf("Could not setup_fonts_struct\n");

		free_screen_struct(gui->screen);

		free_sounds_struct(gui->sounds);

		free_images_struct(gui->images);

		return false;
	}

	return true;
}
