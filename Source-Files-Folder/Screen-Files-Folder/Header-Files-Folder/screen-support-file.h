
#ifndef SCREEN_SUPPORT_FILE_H
#define SCREEN_SUPPORT_FILE_H

bool make_screen_window(Window**, int, int, char[]);

bool make_window_surface(Surface**, Window*);

bool make_surface_render(Render**, Surface*);

bool make_surface_texture(Texture**, Render*, Surface*);

bool setup_screen_struct(Screen*, char[]);

void free_screen_struct(Screen);

bool color_point_square(Screen, Board, Point, Color);

bool render_field_square(Screen, Board, Point, Square);

bool render_field_symbol(Screen, Board, Point, Square);

bool render_mine_field(Screen, Field, Board);

bool render_surface_texture(Render*, Surface*, Rect);

bool extract_square_file(char*, Square);

bool extract_square_image(Surface**, Square);

bool screen_field_point(Rect*, Screen, Board, Point);

bool extract_symbol_image(Surface**, Square);

bool extract_symbol_file(char*, Square);

#endif
