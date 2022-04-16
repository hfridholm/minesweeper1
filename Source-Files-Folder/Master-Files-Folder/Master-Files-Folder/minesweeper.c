
#include "../Header-Files-Folder/master-include-file.h"

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

bool mine_sweeper_game(bool* result, Screen* screen, Field field, Board board, Sounds sounds, Images images)
{
  while(game_still_running(result, field, board))
  {
    if(!render_mine_field(*screen, field, board, images))
    {
      printf("Could not render_mine_field!\n");

      return false;
    }

    SDL_RenderPresent(screen->render);

    Point point = {-1, -1};

    Input inputEvent = input_screen_point(&point, screen, field, board, images);

    if(inputEvent == INPUT_QUIT)
    {
      printf("Exiting the window manually!\n");

      return false;
    }
    else if(inputEvent == INPUT_UNLOCK)
    {


      Mix_PlayChannel(-1, sounds.unlockEffect, 0);


      if(!unlock_field_square(field, board, point))
      {
        printf("Could not unlock_field_square!\n");
      }
    }
    else if(inputEvent == INPUT_FLAG)
    {

      Mix_PlayChannel(-1, sounds.flagEffect, 0);

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

  Screen screen;

  if(!setup_screen_struct(&screen, "minesweeper", 1280, 720))
  {
    printf("Could not setup_display_screen!\n");

    return false;
  }

  Sounds sounds;

  sounds.unlockEffect = Mix_LoadWAV("../Source-Files-Folder/Screen-Files-Folder/Screen-Sounds-Folder/square-click-sound.wav");
  sounds.flagEffect = Mix_LoadWAV("../Source-Files-Folder/Screen-Files-Folder/Screen-Sounds-Folder/square-flag-sound.wav");
  sounds.winEffect = Mix_LoadWAV("../Source-Files-Folder/Screen-Files-Folder/Screen-Sounds-Folder/win-result-sound.wav");
  sounds.loseEffect = Mix_LoadWAV("../Source-Files-Folder/Screen-Files-Folder/Screen-Sounds-Folder/lose-result-sound.wav");


  Images images;

  char* nextStrings[] = {"one", "two", "three", "four", "five", "six", "seven", "eight"};

  for(uint8_t index = 0; index < 8; index += 1)
  {
    char filePath[200];

    sprintf(filePath, "%s/%s-symbol.png", SCREEN_IMAGE_FOLDER, nextStrings[index]);

    extract_file_image(&images.nextSymbols[index], filePath);
  }

  extract_file_image(&images.mineSymbol, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/mine-symbol.png");
  extract_file_image(&images.flagSymbol, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/flag-symbol.png");
  extract_file_image(&images.intactSquare, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/intact-square.png");
  extract_file_image(&images.blastSquare, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/blast-square.png");
  extract_file_image(&images.sweptSquare, "../Source-Files-Folder/Screen-Files-Folder/Screen-Images-Folder/swept-square.png");


  Board board;


  if(!input_screen_board(&board, &screen))
  {
    printf("could not input board!\n");

    free_screen_struct(screen);

    free_sounds_struct(sounds);

    free_images_struct(images);

    return false;
  }


  Field field = create_field_matrix(board.height, board.width);

  if(!generate_mine_field(field, board))
  {
    printf("Could not generate field!\n");

    free_mine_field(field, board);

    free_sounds_struct(sounds);

    free_images_struct(images);

    free_screen_struct(screen);

    return false;
  }


  bool result = false;

  if(mine_sweeper_game(&result, &screen, field, board, sounds, images))
  {
    if(!render_mine_field(screen, field, board, images))
    {
      printf("Could not render_mine_field!\n");

      free_mine_field(field, board);

      free_sounds_struct(sounds);

      free_screen_struct(screen);

      free_images_struct(images);

      return false;
    }

    if(result)
    {
      Mix_PlayChannel(-1, sounds.winEffect, 0);


      Color color = {0, 200, 0};

      if(!render_screen_text(screen, "You Won!", color, screen.width / 2, screen.height / 2, 5))
      {
        printf("Could not render text result!\n");
      }

      printf("You have won the game!\n");
    }
    else
    {
      Mix_PlayChannel(-1, sounds.loseEffect, 0);


      Color color = {128, 8, 0};

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

  printf("free_images_struct(images);\n");
  free_images_struct(images);

  printf("free_sounds_struct\n");
  free_sounds_struct(sounds);

  printf("free_mine_field(field, board);\n");
  free_mine_field(field, board);

  printf("free_screen_struct(screen);\n");
  free_screen_struct(screen);

  return false;
}
