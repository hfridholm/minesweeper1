
#include "../Header-Files-Folder/master-include-file.h"

#define STUPID_BOARD (Board) {9, 9, 10};
#define NORMAL_BOARD (Board) {16, 16, 40};
#define EXPERT_BOARD (Board) {16, 30, 99};

bool game_still_running(bool* result, Field field, Board board)
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

bool mine_sweeper_game(bool* result, Screen* screen, Field field, Board board)
{
  while(game_still_running(result, field, board))
  {
    if(!render_mine_field(*screen, field, board))
    {
      printf("Could not render_mine_field!\n");

      return false;
    }

    SDL_RenderPresent(screen->render);

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

int main(int argAmount, char* arguments[])
{
  srand(time(NULL));

  const Board board = NORMAL_BOARD;

  Field field = create_field_matrix(board.height, board.width);

  if(!generate_mine_field(field, board))
  {
    printf("Could not generate field!\n");

    free_mine_field(field, board.height);

    return false;
  }

  Screen screen;

  if(!setup_screen_struct(&screen, "minesweeper", 1280, 720))
  {
    printf("Could not setup_display_screen!\n");

    return false;
  }

  bool result = false;

  if(mine_sweeper_game(&result, &screen, field, board))
  {
    if(!render_mine_field(screen, field, board))
    {
      printf("Could not render_mine_field!\n");

      return false;
    }

    if(result)
    {
      Color color = {0, 200, 0};

      if(!render_screen_text(screen, "You Won!", color, screen.width / 2, screen.height / 2, 5))
      {
        printf("Could not render text result!\n");
      }

      printf("You have won the game!\n");
    }
    else
    {
      Color color = {200, 0, 0};

      if(!render_screen_text(screen, "You Lost!", color, screen.width / 2, screen.height / 2, 5))
      {
        printf("Could not render text result!\n");
      }

      printf("You have lost the game!\n");
    }

    SDL_RenderPresent(screen.render);

    Event event;

    while(event.type != SDL_QUIT)
    {
      SDL_WaitEvent(&event);
    }
  }
  else
  {
    printf("The game was quitted!\n");
  }

  printf("free_mine_field(field, board.height);\n");
  free_mine_field(field, board.height);

  printf("free_screen_struct(screen);\n");
  free_screen_struct(screen);

  return false;
}
