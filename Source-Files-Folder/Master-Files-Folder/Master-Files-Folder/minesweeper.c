
#include "../Header-Files-Folder/master-include-file.h"

bool game_still_running(Result* result, Field field, Board board)
{
  if(mine_field_cleared(field, board))
  {
    *result = RESULT_WIN;

    flag_field_mines(field, board);

    return false;
  }

  else if(mine_field_exposed(field, board))
  {
    *result = RESULT_LOSE;

    unlock_field_mines(field, board);

    return false;
  }

  return true;
}

bool mine_sweeper_game(Result* result, Field field, Board board, Screen* screen, Images images, Sounds sounds)
{
  while(game_still_running(result, field, board))
  {
    if(!game_action_handler(field, board, screen, images, sounds))
    {
      return false;
    }
  }
  return true;
}

int main(int argAmount, char* arguments[])
{
  srand(time(NULL));

  Screen screen;
  Images images;
  Sounds sounds;

  if(!setup_screen_structs(&screen, "minesweeper", 1280, 720, &images, &sounds))
  {
    printf("Could not setup_screen_structs\n");

    return false;
  }

  Board board;

  if(!input_screen_board(&board, &screen, images))
  {
    printf("could not input board!\n");

    free_screen_structs(screen, images, sounds);

    return false;
  }

  Field field = create_field_matrix(board.height, board.width);

  if(!generate_mine_field(field, board))
  {
    printf("Could not generate field!\n");

    free_mine_field(field, board);
    free_screen_structs(screen, images, sounds);

    return false;
  }

  Result result;

  if(mine_sweeper_game(&result, field, board, &screen, images, sounds))
  {
    game_result_handler(screen, images, sounds, field, board, result);
  }

  printf("free_screen_structs(screen, images, sounds);\n");
  printf("free_mine_field(field, board);\n");

  free_mine_field(field, board);
  free_screen_structs(screen, images, sounds);

  return false;
}
