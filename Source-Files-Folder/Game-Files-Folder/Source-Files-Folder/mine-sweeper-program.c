
#include "../Header-Files-Folder/game-files-includer.h"

#define BEGINNER_BOARD (Board) {9, 9, 10};
#define INTERMEDIATE_BOARD (Board) {16, 16, 40};
#define EXPERT_BOARD (Board) {16, 30, 99};

int main(int argAmount, char* arguments[])
{
  srand(time(NULL));

  const Board board = INTERMEDIATE_BOARD;

  Field field = create_field_matrix(board.height, board.width);

  if(!generate_mine_field(field, board))
  {
    printf("Could not generate field!\n");

    free_mine_field(field, board.height);

    return false;
  }

  Screen screen = {NULL, NULL, NULL, 1280, 960};

  if(!setup_display_screen(&screen, "Mine Sweeper\0"))
  {
    printf("Could not setup_display_screen!\n");

    return false;
  }

  bool result = false;

  if(mine_sweeper_game(&result, screen, field, board))
  {
    if(!render_mine_field(screen, field, board))
    {
      printf("Could not render_mine_field!\n");

      return false;
    }
    SDL_UpdateWindowSurface(screen.window);

    if(result)
    {
      printf("You have won the game!\n");
    }
    else
    {
      printf("You have lost the game!\n");
    }

    Event event;

    while(event.type != SDL_QUIT)
    {
      SDL_PollEvent(&event);
    }
  }
  else
  {
    printf("The game was quitted!\n");
  }

  free_mine_field(field, board.height);

  free_display_screen(screen);

  return false;
}

bool game_still_running(bool* result, Field field, const Board board)
{
  if(mine_field_cleared(field, board))
  {
    *result = true;

    flag_field_mines(field, board);

    return false;
  }

  else if(mine_field_exposed(field, board))
  {
    *result = false;

    unlock_field_mines(field, board);

    return false;
  }

  return true;
}

bool mine_sweeper_game(bool* result, Screen screen, Field field, const Board board)
{
  while(game_still_running(result, field, board))
  {
    if(!render_mine_field(screen, field, board))
    {
      printf("Could not render_mine_field!\n");

      return false;
    }
    SDL_UpdateWindowSurface(screen.window);


    Point point = {-1, -1};

    Input inputEvent = input_screen_point(&point, screen, field, board);

    if(inputEvent == INPUT_QUIT)
    {
      printf("Exiting the window manually!\n");

      return false;
    }
    else if(inputEvent == INPUT_UNLOCK)
    {
      if(!unlock_field_square(field, board, point))
      {
        printf("Could not unlock_field_square!\n");
      }
    }
    else if(inputEvent == INPUT_FLAG)
    {
      if(!flag_field_square(field, board, point))
      {
        printf("Could not flag_field_square!\n");
      }
    }
  }
  printf("The game was played normaly!\n");

  return true;
}
