/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-22
 */

#ifndef SCREEN_STRUCT_H
#define SCREEN_STRUCT_H

typedef enum Input
{
  INPUT_NONE, INPUT_QUIT, INPUT_UNLOCK, INPUT_FLAG
} Input;

typedef struct Screen
{
  SDL_Window* window;
  SDL_Renderer* render;
  uint16_t width;
  uint16_t height;
} Screen;

typedef struct Images
{
  SDL_Surface* nextSymbols[8];
  SDL_Surface* mineSymbol;
  SDL_Surface* flagSymbol;
  SDL_Surface* blastSquare;
  SDL_Surface* sweptSquare;
  SDL_Surface* intactSquare;
  SDL_Surface* boardGround;
  SDL_Surface* fieldGround;
} Images;

typedef struct Sounds
{
  Mix_Chunk* unlockEffect;
  Mix_Chunk* flagEffect;
  Mix_Chunk* winEffect;
  Mix_Chunk* loseEffect;
} Sounds;

typedef struct Fonts
{
  TTF_Font* timeFont;
} Fonts;

typedef struct GUI
{
  Screen screen;
  Images images;
  Sounds sounds;
  Fonts fonts;
} GUI;

#define SCREEN_IMAGE_FOLDER "../assets/images"

#define INTACT_SQUARE_FILE "intact-square.png"
#define SWEPT_SQUARE_FILE "swept-square.png"
#define BLAST_SQUARE_FILE "blast-square.png"

#define MINE_SYMBOL_FILE "mine-symbol.png"
#define FLAG_SYMBOL_FILE "flag-symbol.png"

#define FONT_FILE "../assets/fonts/8Bit-font.ttf"
#define FONT_SIZE 24

#endif // SCREEN_STRUCT_H
