/*
 * minesweeper
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-22
 */

#include "minesweeper.h"

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


bool game_result_handler(Field field, Board board, Stats stats, Result result, GUI* gui)
{
	render_result_screen(*gui, field, board, stats, result);

	SDL_Event event;

	while(event.type != SDL_QUIT)
	{
		SDL_WaitEvent(&event);
	}

	return true;
}

bool game_action_handler(Field field, Board board, Stats* stats, GUI* gui)
{
	if(!render_mine_field(*gui, field, board, *stats))
	{
		printf("Could not render_mine_field!\n");

		return false;
	}

	SDL_RenderPresent(gui->screen.render);

	Point point = {-1, -1};

	Input inputEvent = input_screen_point(&point, field, board, stats, gui);

	if(!point_input_handler(field, board, inputEvent, point, *gui))
	{
		return false;
	}

	return true;
}

bool mine_sweeper_game(Result* result, Field field, Board board, Stats* stats, GUI* gui)
{
  while(game_still_running(result, field, board))
  {
    if(!game_action_handler(field, board, stats, gui))
    {
      return false;
    }
  }
  return true;
}

int main(int argAmount, char* arguments[])
{
  info_print("Start of main");

  srand(time(NULL));

  GUI gui;

  if(!setup_gui_struct(&gui, "minesweeper", 1280, 720))
  {
    printf("Could not setup_screen_structs\n");

    return false;
  }

  Board board;

  if(!input_screen_board(&board, &gui))
  {
    printf("could not input board!\n");

    free_gui_struct(gui);

    return false;
  }

  Field field = create_field_matrix(board.height, board.width);

  if(!generate_mine_field(field, board))
  {
    printf("Could not generate field!\n");

    free_mine_field(field, board);
    free_gui_struct(gui);

    return false;
  }

  Stats stats = {0, time(NULL), time(NULL)};

  Result result;

  if(mine_sweeper_game(&result, field, board, &stats, &gui))
  {
    game_result_handler(field, board, stats, result, &gui);
  }

  free_mine_field(field, board);

  free_gui_struct(gui);

  info_print("End of main");

  return false;
}
