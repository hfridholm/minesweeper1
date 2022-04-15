
#ifndef FIELD_SCREEN_STRUCTS_H
#define FIELD_SCREEN_STRUCTS_H

typedef SDL_Window   Window;
typedef SDL_Renderer Render;
typedef SDL_Surface  Surface;
typedef SDL_Texture  Texture;
typedef SDL_Rect     Rect;
typedef SDL_Color    Color;
typedef SDL_Event    Event;

typedef TTF_Font     Font;

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

#endif
