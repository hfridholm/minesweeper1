/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-22
 */

#include "../screen.h"

#include "screen-intern.h"

/*
 * Get the position and size of difficulty option banner
 */
bool board_options_position(Rect* positions, int amount, Screen screen)
{
	int relativeBoardWidth = 73;
	int relativeBoardHeight = 86;

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

bool extract_file_image(Surface** image, char filePath[])
{
	*image = IMG_Load(filePath);

  return (*image != NULL);
}

/*
 * Get which symbol that should be displayed at the specified minefield square
 */
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

/*
 * Get which background image that should be displayed at the specified minefield square
 */
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


bool render_file_image(Screen screen, char filePath[], Rect position)
{
	Surface* image;

	if(!extract_file_image(&image, filePath))
	{
		return false;
	}

	return render_screen_image(screen, image, position);
}

bool make_surface_texture(Texture** texture, Render* render, Surface* surface)
{
  *texture = SDL_CreateTextureFromSurface(render, surface);

	return (texture != NULL);
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

/*
 * Render the different difficulty option banners
 */
bool render_board_options(GUI gui)
{
	SDL_RenderClear(gui.screen.render);

	render_board_ground(gui);


	Rect positions[3];

	board_options_position(positions, 3, gui.screen);


	render_file_image(gui.screen, "../assets/images/stupid-board.png", positions[0]);

	render_file_image(gui.screen, "../assets/images/normal-board.png", positions[1]);

	render_file_image(gui.screen, "../assets/images/expert-board.png", positions[2]);

	return true;
}

/*
 * Render text on the screen
 */
bool render_screen_text(Screen screen, char text[], Font* font, Color color, Rect position)
{
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, color);

	SDL_Texture* message = SDL_CreateTextureFromSurface(screen.render, surfaceMessage);

	SDL_RenderCopy(screen.render, message, NULL, &position);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);

	return true;
}

/*
 * Render the minefield
 */
bool render_mine_field(GUI gui, Field field, Board board, Stats stats)
{
	SDL_RenderClear(gui.screen.render);

	render_field_ground(gui);

	char timeText[200];
	Color color = {128, 128, 128};

	sprintf(timeText, "Time: %ld", (stats.stopClock - stats.startClock));

	int width, height;

	text_true_dimensions(&width, &height, timeText, gui.fonts.timeFont);

	Rect position = {10, 10, width * 2, height * 2};

	render_screen_text(gui.screen, timeText, gui.fonts.timeFont, color, position);

  for(int hIndex = 0; hIndex < board.height; hIndex += 1)
  {
    for(int wIndex = 0; wIndex < board.width; wIndex += 1)
    {
      Square square = field[hIndex][wIndex];
      Point point = {hIndex, wIndex};

      if(!render_field_square(gui, board, point, square))
      {
        printf("Could not render_field_square!\n");

        return false;
      }
    }
  }
  return true;
}

/*
 * Render a square in the minefield
 */
bool render_field_square(GUI gui, Board board, Point point, Square square)
{
	Image* squareImage = NULL;
	Image* symbolImage = NULL;

	if(!extract_square_image(&squareImage, square, gui.images))
  {
    printf("Could not extract_square_image!\n");

    return false;
  }

	if(!extract_symbol_image(&symbolImage, square, gui.images))
	{
		printf("Could not extract_symbol_image\n");

		return false;
	}

  Rect position;

  if(!screen_field_point(&position, gui.screen, board, point))
  {
    return false;
  }

  if(!render_screen_image(gui.screen, squareImage, position))
  {
    return false;
  }

	if(symbolImage != NULL)
	{
		if(!render_screen_image(gui.screen, symbolImage, position))
		{
			return false;
		}
	}
	return true;
}

/*
 * Get the position and size of a square in the minefield
 */
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

bool render_field_ground(GUI gui)
{
	Rect position = {0, 0, gui.screen.width, gui.screen.height};

	return render_screen_image(gui.screen, gui.images.fieldGround, position);
}

bool render_board_ground(GUI gui)
{
	Rect position = {0, 0, gui.screen.width, gui.screen.height};

	return render_screen_image(gui.screen, gui.images.boardGround, position);
}

/*
 * Render the minefield and the result message
 */
bool render_result_screen(GUI gui, Field field, Board board, Stats stats, Result result)
{
	if(!render_mine_field(gui, field, board, stats))
	{
		printf("Could not render_mine_field!\n");

		return false;
	}

	if(!render_result_message(gui, result))
	{
		printf("Could not render_result_message\n");
		return false;
	}

	SDL_RenderPresent(gui.screen.render);

	return true;
}

bool text_true_dimensions(int* width, int* height, char text[], Font* font)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, (Color) {255, 255, 255});

	*width = surface->w;
	*height = surface->h;

	SDL_FreeSurface(surface);

	return true;
}

bool result_message_position(Rect* position, char message[], GUI gui)
{
	int trueWidth, trueHeight;

	text_true_dimensions(&trueWidth, &trueHeight, message, gui.fonts.timeFont);

	float widthRel = ((float) gui.screen.width / (float) trueWidth);
	float heightRel = ((float) gui.screen.height / (float) trueHeight);
	float sizeFactor = (widthRel < heightRel) ? widthRel : heightRel;

	int width = (gui.screen.width - (sizeFactor * trueWidth)) / 2;
	int height = (gui.screen.height - (sizeFactor * trueHeight)) / 2;

	*position = (Rect) {width, height, sizeFactor * trueWidth, sizeFactor * trueHeight};

	return true;
}

/*
 * Render the result message on the screen
 */
bool render_result_message(GUI gui, Result result)
{
	Rect position;

	char text[200];

	if(result == RESULT_WIN)
	{
		Color color = {0, 200, 0};

		sprintf(text, "You Won!");

		result_message_position(&position, text, gui);

		if(!render_screen_text(gui.screen, text, gui.fonts.timeFont, color, position))
		{
			printf("Could not render text result!\n");
		}

		Mix_PlayChannel(-1, gui.sounds.winEffect, 0);
	}
	else if(result == RESULT_LOSE)
	{
		Color color = {128, 8, 0};

		sprintf(text, "You Lost!");

		result_message_position(&position, text, gui);

		if(!render_screen_text(gui.screen, text, gui.fonts.timeFont, color, position))
		{
			printf("Could not render text result!\n");
		}

		Mix_PlayChannel(-1, gui.sounds.loseEffect, 0);
	}
	return true;
}
