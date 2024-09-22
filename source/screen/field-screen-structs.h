
#ifndef FIELD_SCREEN_STRUCTS_H
#define FIELD_SCREEN_STRUCTS_H

typedef SDL_Window   Window;
typedef SDL_Renderer Render;
typedef SDL_Surface  Surface;
typedef SDL_Texture  Texture;
typedef SDL_Rect     Rect;
typedef SDL_Color    Color;
typedef SDL_Event    Event;
typedef SDL_Surface  Image;

typedef TTF_Font     Font;

typedef Mix_Chunk    Effect;
typedef Mix_Music    Music;

typedef enum Input
{
  INPUT_NONE, INPUT_QUIT, INPUT_UNLOCK, INPUT_FLAG
} Input;

typedef struct Screen
{
  Window* window;
  Render* render;
  uint16_t width;
  uint16_t height;
} Screen;

typedef struct Images
{
  Image* nextSymbols[8];
  Image* mineSymbol;
  Image* flagSymbol;
  Image* blastSquare;
  Image* sweptSquare;
  Image* intactSquare;
  Image* boardGround;
  Image* fieldGround;
} Images;

typedef struct Sounds
{
  Effect* unlockEffect;
  Effect* flagEffect;
  Effect* winEffect;
  Effect* loseEffect;
} Sounds;

typedef struct Fonts
{
  Font* timeFont;
} Fonts;

typedef struct GUI
{
  Screen screen;
  Images images;
  Sounds sounds;
  Fonts fonts;
} GUI;

#endif
