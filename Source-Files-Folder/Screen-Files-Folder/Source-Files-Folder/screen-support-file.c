
#include "../Header-Files-Folder/screen-include-file.h"

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

bool make_surface_texture(Texture** texture, Render* render, Surface* surface)
{
  *texture = SDL_CreateTextureFromSurface(render, surface);

	return (texture != NULL);
}

bool make_screen_window(Window** window, char title[], int width, int height)
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
	IMG_Quit();
  TTF_Quit();
  Mix_Quit();
}

bool render_mine_field(Screen screen, Field field, Board board, Images images)
{
	SDL_RenderClear(screen.render);

	render_field_ground(screen, images);

  for(int hIndex = 0; hIndex < board.height; hIndex += 1)
  {
    for(int wIndex = 0; wIndex < board.width; wIndex += 1)
    {
      Square square = field[hIndex][wIndex];
      Point point = {hIndex, wIndex};

      if(!render_field_square(screen, board, point, square, images))
      {
        printf("Could not render_field_square!\n");

        return false;
      }
    }
  }
  return true;
}

bool render_field_square(Screen screen, Board board, Point point, Square square, Images images)
{
	Image* squareImage = NULL;
	Image* symbolImage = NULL;

	if(!extract_square_image(&squareImage, square, images))
  {
    printf("Could not extract_square_image!\n");

    return false;
  }

	if(!extract_symbol_image(&symbolImage, square, images))
	{
		printf("Could not extract_symbol_image\n");

		return false;
	}

  Rect position;

  if(!screen_field_point(&position, screen, board, point))
  {
    return false;
  }

  if(!render_screen_image(screen, squareImage, position))
  {
    return false;
  }

	if(symbolImage != NULL)
	{
		if(!render_screen_image(screen, symbolImage, position))
		{
			return false;
		}
	}
	return true;
}

bool screen_field_point(Rect* position, Screen screen, Board board, Point point)
{
  if(!point_inside_board(point, board)) return false;

	int squareHeight = (screen.height / board.height);
	int squareWidth = (screen.width / board.width);

	int squareLength = (squareHeight > squareWidth) ? squareWidth : squareHeight;

	int fieldHeight = (squareLength * board.height);
	int fieldWidth = (squareLength * board.width);

	int heightBlank = (screen.height - fieldHeight) / 2;
	int widthBlank = (screen.width - fieldWidth) / 2;

	int relativeHeight = (point.height * squareLength);
	int relativeWidth = (point.width * squareLength);

	int height = (heightBlank + relativeHeight);
  int width = (widthBlank + relativeWidth);

  *position = (Rect) {width, height, squareLength, squareLength};

  return true;
}

bool extract_symbol_image(Image** image, Square square, Images images)
{
	if(square.visable && square.mine)
  {
    *image = images.mineSymbol;
  }
	else if(square.visable && square.adjacent != 0)
  {
    *image = images.nextSymbols[square.adjacent - 1];
  }
  else if(!square.visable && square.flagged)
  {
    *image = images.flagSymbol;
  }
	return true;
}

bool extract_square_image(Image** image, Square square, Images images)
{
	if(square.visable && square.mine)
  {
    *image = images.blastSquare;
  }
  else if(square.visable && !square.mine)
  {
    *image = images.sweptSquare;
  }
  else if(!square.visable)
  {
    *image = images.intactSquare;
  }
	else return false;

	return true;
}

bool extract_file_font(Font** font, char filePath[], int size)
{
	*font = TTF_OpenFont(filePath, size);

	return (*font != NULL);
}

#define FONT_FILE "../Source-Files-Folder/Screen-Files-Folder/Screen-Fonts-Folder/8Bit-font.ttf"
#define FONT_SIZE 24

bool render_screen_text(Screen screen, char text[], Color color, int width, int height, float size)
{
	Font* textFont = NULL;

	if(!extract_file_font(&textFont, (char*) FONT_FILE, FONT_SIZE))
	{
		printf("Could not extract font!\n");
		return false;
	}

	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(textFont, text, color);

	SDL_Texture* message = SDL_CreateTextureFromSurface(screen.render, surfaceMessage);

	float widthRel = ((float) screen.width / (float) surfaceMessage->w);
	float heightRel = ((float) screen.height / (float) surfaceMessage->h);
	float sizeFactor = (widthRel < heightRel) ? widthRel : heightRel;

	int textHeight = surfaceMessage->h * sizeFactor;
	int textWidth = surfaceMessage->w * sizeFactor;

	Rect position = {width - textWidth / 2, height - textHeight / 2, textWidth, textHeight};

	SDL_RenderCopy(screen.render, message, NULL, &position);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);

	TTF_CloseFont(textFont);

	return true;
}

bool board_options_position(Rect* positions, int amount, Screen screen)
{
	int relativeBoardWidth = 100;
	int relativeBoardHeight = 200;

	float boardMargin = 0.1;

	int relativeTotalWidth = ((relativeBoardWidth * 3) + (boardMargin * relativeBoardWidth * 2));
	int relativeTotalHeight = relativeBoardHeight;

	float widthRel = ((float) screen.width / (float) relativeTotalWidth);
	float heightRel = ((float) screen.height / (float) relativeTotalHeight);

	float sizeFactor = (widthRel < heightRel) ? widthRel : heightRel;

	int totalHeight = relativeTotalHeight * sizeFactor;
	int totalWidth = relativeTotalWidth * sizeFactor;

	uint16_t height = (screen.height - totalHeight) / 2;
	uint16_t width = (screen.width - totalWidth) / 2;

	int boardHeight = (relativeBoardHeight * sizeFactor);
	int boardWidth = (relativeBoardWidth * sizeFactor);

	for(int index = 0; index < amount; index += 1)
	{
		positions[index] = (Rect) {width + (index * boardWidth) + (index * boardMargin * boardWidth), height, boardWidth, boardHeight};
	}

	return true;
}

bool render_board_options(Screen screen, Images images)
{
	SDL_RenderClear(screen.render);

	render_board_ground(screen, images);


	Rect positions[3];

	board_options_position(positions, 3, screen);


	render_file_image(screen, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/stupid-board.png", positions[0]);

	render_file_image(screen, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/normal-board.jpeg", positions[1]);

	render_file_image(screen, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/expert-board.png", positions[2]);

	return true;
}

bool extract_file_image(Surface** image, char filePath[])
{
	*image = IMG_Load(filePath);

  return (*image != NULL);
}

bool render_file_image(Screen screen, char filePath[], Rect position)
{
	Surface* image;

	if(!extract_file_image(&image, filePath))
	{
		return false;
	}

	return render_screen_image(screen, image, position);
}

bool render_screen_image(Screen screen, Surface* image, Rect position)
{
	Texture* texture = NULL;

	if(!make_surface_texture(&texture, screen.render, image))
	{
		return false;
	}

	SDL_RenderCopy(screen.render, texture, NULL, &position);

	SDL_DestroyTexture(texture);

	return true;
}

void free_sounds_struct(Sounds sounds)
{
	Mix_FreeChunk(sounds.unlockEffect);

	Mix_FreeChunk(sounds.flagEffect);

	Mix_FreeChunk(sounds.winEffect);

	Mix_FreeChunk(sounds.loseEffect);
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
}

void free_screen_structs(Screen screen, Images images, Sounds sounds)
{
	free_screen_struct(screen);

	free_images_struct(images);

	free_sounds_struct(sounds);
}

bool render_result_screen(Screen screen, Images images, Sounds sounds, Field field, Board board, Result result)
{
	if(!render_mine_field(screen, field, board, images))
	{
		printf("Could not render_mine_field!\n");

		return false;
	}

	if(!render_result_message(screen, sounds, result))
	{
		printf("Could not render_result_message\n");
		return false;
	}

	SDL_RenderPresent(screen.render);

	return true;
}

bool render_result_message(Screen screen, Sounds sounds, Result result)
{
	if(result == RESULT_WIN)
	{
		Color color = {0, 200, 0};

		if(!render_screen_text(screen, "You Won!", color, screen.width / 2, screen.height / 2, 5))
		{
			printf("Could not render text result!\n");
		}

		Mix_PlayChannel(-1, sounds.winEffect, 0);
	}
	else if(result == RESULT_LOSE)
	{
		Color color = {128, 8, 0};

		if(!render_screen_text(screen, "You Lost!", color, screen.width / 2, screen.height / 2, 5))
		{
			printf("Could not render text result!\n");
		}

		Mix_PlayChannel(-1, sounds.loseEffect, 0);
	}
	return true;
}

bool game_result_handler(Screen screen, Images images, Sounds sounds, Field field, Board board, Result result)
{
	render_result_screen(screen, images, sounds, field, board, result);

	Event event;

	while(event.type != SDL_QUIT)
	{
		SDL_WaitEvent(&event);
	}

	return true;
}

bool game_action_handler(Field field, Board board, Screen* screen, Images images, Sounds sounds)
{
	if(!render_mine_field(*screen, field, board, images))
	{
		printf("Could not render_mine_field!\n");

		return false;
	}

	SDL_RenderPresent(screen->render);

	Point point = {-1, -1};

	Input inputEvent = input_screen_point(&point, screen, field, board, images);

	if(!point_input_handler(field, board, inputEvent, point, sounds))
	{
		return false;
	}

	return true;
}

bool point_input_handler(Field field, Board board, Input inputEvent, Point point, Sounds sounds)
{
	if(inputEvent == INPUT_QUIT)
	{
		return false;
	}
	else if(inputEvent == INPUT_UNLOCK)
	{
		if(unlock_field_square(field, board, point))
		{
			Mix_PlayChannel(-1, sounds.unlockEffect, 0);
		}
		else printf("Could not unlock_field_square!\n");
	}
	else if(inputEvent == INPUT_FLAG)
	{
		if(flag_field_square(field, board, point))
		{
			Mix_PlayChannel(-1, sounds.flagEffect, 0);
		}
		else printf("Could not flag_field_square!\n");
	}
	return true;
}

bool setup_sounds_struct(Sounds* sounds)
{
	sounds->unlockEffect = Mix_LoadWAV("../Source-Files-Folder/Screen-Files-Folder/Screen-Sounds-Folder/square-click-sound.wav");
  sounds->flagEffect = Mix_LoadWAV("../Source-Files-Folder/Screen-Files-Folder/Screen-Sounds-Folder/square-flag-sound.wav");
  sounds->winEffect = Mix_LoadWAV("../Source-Files-Folder/Screen-Files-Folder/Screen-Sounds-Folder/win-result-sound.wav");
  sounds->loseEffect = Mix_LoadWAV("../Source-Files-Folder/Screen-Files-Folder/Screen-Sounds-Folder/lose-result-sound.wav");

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

  extract_file_image(&images->mineSymbol, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/mine-symbol.png");
  extract_file_image(&images->flagSymbol, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/flag-symbol.png");
  extract_file_image(&images->intactSquare, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/intact-square.png");
  extract_file_image(&images->blastSquare, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/blast-square.png");
  extract_file_image(&images->sweptSquare, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/swept-square.png");
	extract_file_image(&images->fieldGround, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/field-ground.png");
	extract_file_image(&images->boardGround, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/board-ground.png");

	return true;
}

bool render_field_ground(Screen screen, Images images)
{
	Rect position = {0, 0, screen.width, screen.height};

	return render_screen_image(screen, images.fieldGround, position);
}

bool render_board_ground(Screen screen, Images images)
{
	Rect position = {0, 0, screen.width, screen.height};

	return render_screen_image(screen, images.boardGround, position);
}

bool setup_screen_structs(Screen* screen, char title[], int width, int height, Images* images, Sounds* sounds)
{
	if(!setup_screen_struct(screen, title, width, height))
  {
    printf("Could not setup_display_screen!\n");

    return false;
  }
	if(!setup_sounds_struct(sounds))
  {
    printf("Could not setup_sounds_struct\n");

		free_screen_struct(*screen);

		return false;
  }

  if(!setup_images_struct(images))
  {
		printf("Could not setup_images_struct\n");

		free_screen_struct(*screen);

		free_sounds_struct(*sounds);

		return false;
  }
	return true;
}
