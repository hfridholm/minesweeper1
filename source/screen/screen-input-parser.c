/*
 * Written by Hampus Fridholm
 *
 * Last updated: 2024-09-22
 */

#include "../screen.h"

#include "screen-intern.h"

/*
 * Perform an action based on how the user clicked on what square
 */
bool point_input_handler(Field field, Board board, Input inputEvent, Point point, GUI gui)
{
	if(inputEvent == INPUT_QUIT)
	{
		return false;
	}
	else if(inputEvent == INPUT_UNLOCK)
	{
		if(unlock_field_square(field, board, point))
		{
			Mix_PlayChannel(-1, gui.sounds.unlockEffect, 0);
		}
		else printf("Could not unlock_field_square!\n");
	}
	else if(inputEvent == INPUT_FLAG)
	{
		if(flag_field_square(field, board, point))
		{
			Mix_PlayChannel(-1, gui.sounds.flagEffect, 0);
		}
		else printf("Could not flag_field_square!\n");
	}
	return true;
}

/*
 * Let the user choose which difficulty to play
 */
bool input_screen_board(Board* board, GUI* gui)
{
  Board inputBoard = {0, 0, 0};

  Event event;

  while(inputBoard.width <= 0 || inputBoard.height <= 0 || inputBoard.mines <= 0)
  {
    if(!render_board_options(*gui)) return false;

    SDL_RenderPresent(gui->screen.render);


    if(!SDL_WaitEvent(&event)) continue;

    if(event.type == SDL_QUIT)
    {
      return false;
    }

    else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
      if(!parse_board_input(&inputBoard, event, gui->screen))
      {
        continue;
      }
    }
    else if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
      printf("Resizing! width: %d height: %d\n", event.window.data1, event.window.data2);

      SDL_SetWindowSize(gui->screen.window, event.window.data1, event.window.data2);

      gui->screen.width = event.window.data1;
      gui->screen.height = event.window.data2;
    }
  }
  *board = inputBoard;

  return true;
}

/*
 * Register which minefield square the user clicks on
 */
Input input_screen_point(Point* point, Field mineField, Board board, Stats* stats, GUI* gui)
{
  Point inputPoint = {-1, -1};

  Input inputEvent = INPUT_NONE;

  Event event;

  while(!point_inside_board(inputPoint, board))
  {
    stats->stopClock = time(NULL);

    if(!render_mine_field(*gui, mineField, board, *stats)) return false;

    SDL_RenderPresent(gui->screen.render);


    if(!SDL_PollEvent(&event)) continue;

    if(event.type == SDL_QUIT)
    {
      inputEvent = INPUT_QUIT;

      return inputEvent;
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN)
    {
      if(event.button.button == SDL_BUTTON_LEFT)
      {
        inputPoint = parse_mouse_point(event, gui->screen, board);

        inputEvent = INPUT_UNLOCK;
      }
      else if(event.button.button == SDL_BUTTON_RIGHT)
      {
        inputPoint = parse_mouse_point(event, gui->screen, board);

        inputEvent = INPUT_FLAG;

        break;
      }
    }
    else if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
      printf("Resizing! width: %d height: %d\n", event.window.data1, event.window.data2);

      SDL_SetWindowSize(gui->screen.window, event.window.data1, event.window.data2);

      gui->screen.width = event.window.data1;
      gui->screen.height = event.window.data2;
    }
  }

  *point = inputPoint;

  return inputEvent;
}

#define NUMBER_IN_BOUNDS(NUMBER, MINIMUM, MAXIMUM) (NUMBER >= MINIMUM && NUMBER <= MAXIMUM)

/*
 * Check which difficulty the user wants to play
 */
bool parse_board_input(Board* board, Event event, Screen screen)
{
  Rect positions[3];

  board_options_position(positions, 3, screen);

  int inputBoard = 0;

  for(int index = 0; index < 3; index += 1)
  {
    if(NUMBER_IN_BOUNDS(event.motion.x, positions[index].x, positions[index].x + positions[index].w) &&
      NUMBER_IN_BOUNDS(event.motion.y, positions[index].y, positions[index].y + positions[index].h))
    {
      inputBoard = index + 1;
      break;
    }
  }



  printf("inputBoard: %d\n", inputBoard);

  if(inputBoard == 1)
  {
    *board = STUPID_BOARD;
  }
  else if(inputBoard == 2)
  {
    *board = NORMAL_BOARD;
  }
  else if(inputBoard == 3)
  {
    *board = EXPERT_BOARD;
  }
  else
  {
    return false;
  }

  return true;
}

/*
 * Parse which minefield square the user hovers over
 *
 * Instead of Event, use only Event.motion
 */
Point parse_mouse_point(Event event, Screen screen, Board board)
{
  const int squareHeight = (screen.height / board.height);
  const int squareWidth = (screen.width / board.width);

	const int squareLength = (squareHeight > squareWidth) ? squareWidth : squareHeight;

  const int fieldHeight = (squareLength * board.height);
	const int fieldWidth = (squareLength * board.width);

	const int heightBlank = (screen.height - fieldHeight) / 2;
	const int widthBlank = (screen.width - fieldWidth) / 2;

  const int relativeWidth = (event.motion.x - widthBlank);
  const int relativeHeight = (event.motion.y - heightBlank);

  int width = floor( (double) relativeWidth / (double) squareLength);
  int height = floor( (double) relativeHeight / (double) squareLength);

  return (Point) {height, width};
}
