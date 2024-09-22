
#include "../minesweeper.h"

bool mine_field_cleared(Field field, Board board)
{
  for(int hIndex = 0; hIndex < board.height; hIndex += 1)
  {
    for(int wIndex = 0; wIndex < board.width; wIndex += 1)
    {
      Square square = field[hIndex][wIndex];

      if(!square.mine && !square.visable) return false;
    }
  }
  return true;
}

void flag_field_mines(Field field, Board board)
{
  for(int hIndex = 0; hIndex < board.height; hIndex += 1)
  {
    for(int wIndex = 0; wIndex < board.width; wIndex += 1)
    {
      Square square = field[hIndex][wIndex];

      if(!square.mine || square.visable) continue;

      field[hIndex][wIndex].flagged = true;
    }
  }
}

void unlock_field_mines(Field field, Board board)
{
  for(int hIndex = 0; hIndex < board.height; hIndex += 1)
  {
    for(int wIndex = 0; wIndex < board.width; wIndex += 1)
    {
      Square square = field[hIndex][wIndex];

      if(!square.mine || square.flagged) continue;

      field[hIndex][wIndex].visable = true;
    }
  }
}

bool mine_field_exposed(Field field, Board board)
{
  for(int hIndex = 0; hIndex < board.height; hIndex += 1)
  {
    for(int wIndex = 0; wIndex < board.width; wIndex += 1)
    {
      Square square = field[hIndex][wIndex];

      if(square.mine && square.visable) return true;
    }
  }
  return false;
}

bool flag_field_square(Field field, Board board, Point point)
{
  if(!point_inside_board(point, board)) return false;

  Square square = field[point.height][point.width];

  if(square.visable) return false;

  field[point.height][point.width].flagged = !square.flagged;

  return true;
}

bool unlock_field_square(Field field, Board board, Point point)
{
  if(!point_inside_board(point, board)) return false;

  Square square = field[point.height][point.width];

  if(square.flagged || square.visable) return false;


	field[point.height][point.width].visable = true;

	if(square.adjacent != 0 || square.mine) return true;


  for(int hIndex = (point.height - 1); hIndex <= (point.height + 1); hIndex += 1)
	{
		for(int wIndex = (point.width - 1); wIndex <= (point.width + 1); wIndex += 1)
		{
      Point currPoint = {hIndex, wIndex};

			if(!point_inside_board(currPoint, board)) continue;

      Square currSquare = field[hIndex][wIndex];

			if(currSquare.flagged || currSquare.visable) continue;

      // You could use an if-statement here:
			unlock_field_square(field, board, currPoint);
		}
	}

	return true;
}

Field create_field_matrix(int height, int width)
{
  Field field = malloc(sizeof(Square*) * height);

  for(int hIndex = 0; hIndex < height; hIndex += 1)
  {
    field[hIndex] = malloc(sizeof(Square) * width);

    for(int wIndex = 0; wIndex < width; wIndex += 1)
    {
      field[hIndex][wIndex] = (Square) {false, false, false, 0};
    }
  }
  return field;
}

bool generate_mine_field(Field field, Board board)
{
  if(!activate_field_mines(field, board)) return false;

  if(!mark_adjacent_mines(field, board)) return false;

  return true;
}

bool activate_field_mines(Field field, Board board)
{
  int total = (board.height * board.width);

	if(board.mines >= total) return false;

	Point* points = every_field_point(board.height, board.width);

	for(int index = 0; index < board.mines; index = index + 1)
	{
		int random = (rand() % (total - index));

		Point point = points[random];

		field[point.height][point.width].mine = true;

    // You can use an if-statement here:
		delete_array_point(points, (total - index), random);
	}

	free(points);

  return true;
}

bool delete_array_point(Point* points, int amount, int start)
{
  if(start < 0 || start >= amount) return false;

	for(int index = start; index < (amount - 1); index = index + 1)
	{
		points[index] = points[index + 1];
	}
  return true;
}

Point* every_field_point(int height, int width)
{
	int total = (height * width);

	Point* points = malloc(sizeof(Point) * total);

	for(int hIndex = 0; hIndex < height; hIndex = hIndex + 1)
	{
		for(int wIndex = 0; wIndex < width; wIndex = wIndex + 1)
		{
			int index = (hIndex * width) + wIndex;

			points[index] = (Point) {hIndex, wIndex};
		}
	}
	return points;
}

bool mark_adjacent_mines(Field field, Board board)
{
  for(int hIndex = 0; hIndex < board.height; hIndex = hIndex + 1)
	{
		for(int wIndex = 0; wIndex < board.width; wIndex = wIndex + 1)
		{
			if(field[hIndex][wIndex].mine) continue;

			Point point = {hIndex, wIndex};
			int adjacent = adjacent_field_mines(field, board, point);

			field[hIndex][wIndex].adjacent = adjacent;
		}
	}
  return true;
}

int adjacent_field_mines(Field field, Board board, Point point)
{
	int adjacent = 0;

	for(int hIndex = (point.height - 1); hIndex <= (point.height + 1); hIndex += 1)
	{
		for(int wIndex = (point.width - 1); wIndex <= (point.width + 1); wIndex += 1)
		{
      Point point = {hIndex, wIndex};

			if(!point_inside_board(point, board)) continue;

			if(field[hIndex][wIndex].mine) adjacent += 1;
		}
	}
	return adjacent;
}

bool point_inside_board(Point point, Board board)
{
  bool hInside = (point.height >= 0 && point.height < board.height);
  bool wInside = (point.width >= 0 && point.width < board.width);

	return (hInside && wInside);
}

void free_mine_field(Field field, Board board)
{
  for(int hIndex = 0; hIndex < board.height; hIndex += 1)
  {
    free(field[hIndex]);
  }
  free(field);
}
