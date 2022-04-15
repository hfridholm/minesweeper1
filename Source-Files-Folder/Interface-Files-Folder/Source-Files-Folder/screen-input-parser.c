
#include "../Header-Files-Folder/interface-files-includer.h"

Input input_screen_point(Point* point, Screen screen, Field mineField, const Board board)
{
  Point inputPoint = {-1, -1};

  Input inputEvent = INPUT_NONE;

  Event event;

  while(!point_inside_board(inputPoint, board))
  {
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
        inputPoint = parse_mouse_point(event, screen, board);

        inputEvent = INPUT_UNLOCK;
      }
      else if(event.button.button == SDL_BUTTON_RIGHT)
      {
        inputPoint = parse_mouse_point(event, screen, board);

        inputEvent = INPUT_FLAG;

        break;
      }
    }
  }

  *point = inputPoint;

  return inputEvent;
}

// Instead of Event, use only Event.motion
Point parse_mouse_point(const Event event, Screen screen, const Board board)
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
