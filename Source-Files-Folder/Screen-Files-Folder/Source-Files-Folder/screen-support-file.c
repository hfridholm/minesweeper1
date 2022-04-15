
#include "../Header-Files-Folder/screen-include-file.h"

bool setup_screen_struct(Screen* screen, char title[])
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

	if(!make_screen_window(&screen->window, screen->height, screen->width, title))
	{
		SDL_Quit();

		return false;
	}

	if(!make_window_surface(&screen->surface, screen->window))
	{
		SDL_DestroyWindow(screen->window);

		SDL_Quit();

		return false;
	}

	if(!make_surface_render(&screen->render, screen->surface))
	{
		SDL_FreeSurface(screen->surface);

		SDL_DestroyWindow(screen->window);

		SDL_Quit();

		return false;
	}

	return true;
}

bool make_surface_texture(Texture** texture, Render* render, Surface* surface)
{
  *texture = SDL_CreateTextureFromSurface(render, surface);

	return (texture != NULL);
}

bool make_surface_render(Render** render, Surface* surface)
{
  *render = SDL_CreateSoftwareRenderer(surface);

	return (*render != NULL);
}

bool make_window_surface(Surface** surface, Window* window)
{
  *surface = SDL_GetWindowSurface(window);

	return (*surface != NULL);
}

bool make_screen_window(Window** window, int height, int width, char title[])
{
  *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	return (*window != NULL);
}

void free_screen_struct(Screen screen)
{
	SDL_DestroyRenderer(screen.render);

	SDL_FreeSurface(screen.surface);

	SDL_DestroyWindow(screen.window);

	SDL_Quit();
}

bool render_mine_field(Screen screen, Field field, Board board)
{
  for(int hIndex = 0; hIndex < board.height; hIndex += 1)
  {
    for(int wIndex = 0; wIndex < board.width; wIndex += 1)
    {
      const Square square = field[hIndex][wIndex];
      const Point point = {hIndex, wIndex};

      if(!render_field_square(screen, board, point, square))
      {
        printf("Could not render_field_square!\n");

        return false;
      }

      if(!render_field_symbol(screen, board, point, square))
      {
        printf("Could not render_field_symbol!\n");

        return false;
      }
    }
  }

  SDL_RenderPresent(screen.render);

  return true;
}

bool render_field_square(Screen screen, Board board, Point point, Square square)
{
  Surface* image = NULL;

  if(!extract_square_image(&image, square))
  {
    printf("Could not extract_square_image!\n");

    return false;
  }

  Rect position;

  if(!screen_field_point(&position, screen, board, point))
  {
    SDL_FreeSurface(image);

    return false;
  }

  if(!render_surface_texture(screen.render, image, position))
  {
    SDL_FreeSurface(image);

    return false;
  }

  SDL_FreeSurface(image);

  return true;
}

bool screen_field_point(Rect* position, Screen screen, Board board, Point point)
{
  if(!point_inside_board(point, board)) return false;

	const int squareHeight = (screen.height / board.height);
  const int squareWidth = (screen.width / board.width);

	const int squareLength = (squareHeight > squareWidth) ? squareWidth : squareHeight;

	const int fieldHeight = (squareLength * board.height);
	const int fieldWidth = (squareLength * board.width);

	const int heightBlank = (screen.height - fieldHeight) / 2;
	const int widthBlank = (screen.width - fieldWidth) / 2;

	const int relativeHeight = (point.height * squareLength);
	const int relativeWidth = (point.width * squareLength);

	const int height = (heightBlank + relativeHeight);
  const int width = (widthBlank + relativeWidth);

  *position = (Rect) {width, height, squareLength, squareLength};

  return true;
}

bool render_field_symbol(Screen screen, Board board, Point point, Square square)
{
  Surface* image = NULL;

  if(!extract_symbol_image(&image, square))
  {
    printf("Could not extract_symbol_image!\n");

    return false;
  }

  // If the image is NULL, but the function returned true anyways.
  if(image == NULL)
  {
    // This means that the function returned an empty image (blank)
    return true;
  }

  Rect position;

  if(!screen_field_point(&position, screen, board, point))
  {
    SDL_FreeSurface(image);

    return false;
  }

  if(!render_surface_texture(screen.render, image, position))
  {
    SDL_FreeSurface(image);

    return false;
  }

  SDL_FreeSurface(image);

  return true;
}

bool extract_symbol_image(Surface** image, Square square)
{
  char filename[200]; memset(filename, 0, 200);

  if(!extract_symbol_file(filename, square))
  {
    return false;
  }

  // If the function returned true but no filename
  if(strlen(filename) == 0)
  {
    // Returning true without extracting image (no image)
    return true;
  }

  *image = IMG_Load(filename);

  return (*image != NULL);
}

char* adjacentFiles[] = {"one-symbol.png", "two-symbol.png", "three-symbol.png", "four-symbol.png", "five-symbol.png", "six-symbol.png", "seven-symbol.png", "eight-symbol.png"};

bool extract_symbol_file(char* filename, Square square)
{
	memset(filename, 0, 200);

	if(square.visable && square.mine)
  {
    sprintf(filename, "%s/%s", SCREEN_IMAGE_FOLDER, MINE_SYMBOL_FILE);
  }
	else if(square.visable && square.adjacent != 0)
  {
    char* adjacentFile = adjacentFiles[square.adjacent - 1];

    sprintf(filename, "%s/%s", SCREEN_IMAGE_FOLDER, adjacentFile);
  }
  else if(!square.visable && square.flagged)
  {
    sprintf(filename, "%s/%s", SCREEN_IMAGE_FOLDER, FLAG_SYMBOL_FILE);
  }
  return true;
}

bool extract_square_image(Surface** image, Square square)
{
  char filename[200]; memset(filename, 0, 200);

  if(!extract_square_file(filename, square))
  {
    return false;
  }

  *image = IMG_Load(filename);

  return (*image != NULL);
}

bool extract_square_file(char* filename, Square square)
{
	memset(filename, 0, 200);

	if(square.visable && square.mine)
  {
    sprintf(filename, "%s/%s", SCREEN_IMAGE_FOLDER, BLAST_SQUARE_FILE);
  }
  else if(square.visable && !square.mine)
  {
    sprintf(filename, "%s/%s", SCREEN_IMAGE_FOLDER, SWEPT_SQUARE_FILE);
  }
  else if(!square.visable)
  {
    sprintf(filename, "%s/%s", SCREEN_IMAGE_FOLDER, INTACT_SQUARE_FILE);
  }
  return true;
}

bool render_surface_texture(Render* render, Surface* surface, Rect position)
{
	Texture* texture = NULL;

	if(!make_surface_texture(&texture, render, surface))
	{
		return false;
	}

	SDL_RenderCopy(render, texture, NULL, &position);

	SDL_DestroyTexture(texture);

	return true;
}
