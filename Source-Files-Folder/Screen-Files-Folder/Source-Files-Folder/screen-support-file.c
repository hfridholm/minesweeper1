
#include "../Header-Files-Folder/screen-include-file.h"

bool setup_screen_struct(Screen* screen, char title[], uint16_t width, uint16_t height)
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

		return false;
	}

	Mix_Init(0);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	screen->width = width;
	screen->height = height;

	if(!make_screen_window(&screen->window, title, screen->width, screen->height))
	{
		SDL_Quit();

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

bool make_screen_window(Window** window, char title[], uint16_t width, uint16_t height)
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

bool render_mine_field(Screen screen, Field field, Board board)
{
	SDL_RenderClear(screen.render);

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

  return extract_file_image(image, filename);
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

  return extract_file_image(image, filename);
}

bool extract_file_font(Font** font, char filePath[], int size)
{
	*font = TTF_OpenFont(filePath, size);

	return (*font != NULL);
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

bool render_board_options(Screen screen)
{
	SDL_RenderClear(screen.render);

	uint16_t relativeBoardWidth = 100;
	uint16_t relativeBoardHeight = 200;

	float boardMargin = 0.1;

	uint16_t relativeTotalWidth = ((relativeBoardWidth * 3) + (boardMargin * relativeBoardWidth * 2));
	uint16_t relativeTotalHeight = relativeBoardHeight;

	float widthRel = ((float) screen.width / (float) relativeTotalWidth);
	float heightRel = ((float) screen.height / (float) relativeTotalHeight);

	float sizeFactor = (widthRel < heightRel) ? widthRel : heightRel;

	uint16_t totalHeight = relativeTotalHeight * sizeFactor;
	uint16_t totalWidth = relativeTotalWidth * sizeFactor;

	uint16_t height = (screen.height - totalHeight) / 2;
	uint16_t width = (screen.width - totalWidth) / 2;

	uint16_t boardHeight = (relativeBoardHeight * sizeFactor);
	uint16_t boardWidth = (relativeBoardWidth * sizeFactor);

	Rect stupidPosition = {width + 0, height, boardWidth, boardHeight};

	Rect normalPosition = {width + boardWidth + (boardMargin * boardWidth), height, boardWidth, boardHeight};

	Rect expertPosition = {width + (2 * boardWidth) + (2 * boardMargin * boardWidth), height, boardWidth, boardHeight};

	//printf("Render stupid\n");
	render_file_image(screen, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/stupid-board.png", stupidPosition);

	//printf("Render normal\n");
	render_file_image(screen, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/normal-board.jpeg", normalPosition);

	//printf("Render expert\n");
	render_file_image(screen, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/expert-board.png", expertPosition);

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
