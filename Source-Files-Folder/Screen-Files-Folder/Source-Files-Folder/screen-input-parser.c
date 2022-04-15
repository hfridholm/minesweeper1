
#include "../Header-Files-Folder/screen-include-file.h"

Input input_screen_point(Point* point, Screen* screen, Field mineField, Board board)
{
  Point inputPoint = {-1, -1};

  Input inputEvent = INPUT_NONE;

  Event event;

  while(!point_inside_board(inputPoint, board))
  {
    if(!render_mine_field(*screen, mineField, board)) return false;

    SDL_RenderPresent(screen->render);

    if(!SDL_WaitEvent(&event)) continue;

    if(event.type == SDL_QUIT)
    {
      inputEvent = INPUT_QUIT;

      return inputEvent;
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN)
    {
      if(event.button.button == SDL_BUTTON_LEFT)
      {
        inputPoint = parse_mouse_point(event, *screen, board);

        inputEvent = INPUT_UNLOCK;
      }
      else if(event.button.button == SDL_BUTTON_RIGHT)
      {
        inputPoint = parse_mouse_point(event, *screen, board);

        inputEvent = INPUT_FLAG;

        break;
      }
    }
    else if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
      printf("Resizing! width: %d height: %d\n", event.window.data1, event.window.data2);

      SDL_SetWindowSize(screen->window, event.window.data1, event.window.data2);

      screen->width = event.window.data1;
      screen->height = event.window.data2;
    }
  }

  *point = inputPoint;

  return inputEvent;
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
