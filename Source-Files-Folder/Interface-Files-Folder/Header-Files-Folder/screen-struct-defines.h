
#ifndef SCREEN_STRUCT_DEFINES_H
#define SCREEN_STRUCT_DEFINES_H

typedef SDL_Window Window;

typedef SDL_Renderer Renderer;

typedef SDL_Surface	Surface;

typedef SDL_Texture Texture;

typedef SDL_Rect Rect;

typedef SDL_Color Color;

typedef SDL_Event Event;

typedef enum Input
{
  INPUT_NONE, INPUT_QUIT, INPUT_UNLOCK, INPUT_FLAG
} Input;

typedef struct Screen
{
  Window* window;
  Surface* surface;
  Renderer* renderer;
  int width;
  int height;
} Screen;

#endif
