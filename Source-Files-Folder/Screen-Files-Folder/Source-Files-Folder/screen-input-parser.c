
#include "../Header-Files-Folder/screen-include-file.h"

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

bool parse_board_input(Board* board, Event event, Screen screen)
{
  uint16_t relativeBoardWidth = 100;
	uint16_t relativeBoardHeight = 200;

	float boardMargin = 0.1;

	uint16_t relativeTotalWidth = ((relativeBoardWidth * 3) + (boardMargin * relativeBoardWidth * 2));
	uint16_t relativeTotalHeight = relativeBoardHeight;

	float widthRel = ((float) screen.width / (float) relativeTotalWidth);
	float heightRel = ((float) screen.height / (float) relativeTotalHeight);

	float sizeFactor = (widthRel < heightRel) ? widthRel : heightRel;

	uint16_t totalWidth = relativeTotalWidth * sizeFactor;

	uint16_t boardWidth = (relativeBoardWidth * sizeFactor);

	uint16_t width = (screen.width - totalWidth) / 2;

  uint16_t relativeInputWidth = (event.motion.x - width);

  uint8_t inputBoard = ceil((relativeInputWidth - (2 * boardMargin * boardWidth)) / boardWidth);

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

// Instead of Event, use only Event.motion
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
