#include "../../brick_game/tetris/tetris.h"

void pauseTetGame(TetGame *tetg) {
  while (tetg->playing != TET_PLAYING) {
    int cl = getch();
    switch (cl) {
    case 'P':
    case 'p':
      tetg->playing = TET_PLAYING;
      break;
    default:
      break;
    }
  }
}

void printTetGame(TetGame *tetg) {
  TetField *tf = tetg->field;
  TetFigure *t = tetg->figure;

  int printing_bit;

  mvwprintw(stdscr, TET_STAT_RECORD, TET_STAT_X, "%s", "RECORD:");
  mvwprintw(stdscr, TET_STAT_RECORD + 1, TET_STAT_X, "%d", tetg->record);
  mvwprintw(stdscr, TET_STAT_SCORE, TET_STAT_X, "%s", "SCORE:");
  mvwprintw(stdscr, TET_STAT_SCORE + 1, TET_STAT_X, "%d", tetg->score);
  mvwprintw(stdscr, TET_STAT_LEVEL, TET_STAT_X, "%s", "LEVEL:");
  mvwprintw(stdscr, TET_STAT_LEVEL + 1, TET_STAT_X, "%d", tetg->level);
  mvwprintw(stdscr, TET_STAT_ACTIVITY, TET_STAT_X, "%s", "ACTIVITY:");
  switch (tetg->playing) {
  case (TET_PLAYING):
    mvwprintw(stdscr, TET_STAT_ACTIVITY + 1, TET_STAT_X, "%s", "Playing");
    break;
  case (TET_PAUSE):
    mvwprintw(stdscr, TET_STAT_ACTIVITY + 1, TET_STAT_X, "%s", "Pause  ");
    break;
  }
  mvwprintw(stdscr, TET_STAT_NEXT, TET_STAT_X, "%s", "NEXT:");
  for (int y = 0; y < t->size; y++) {
    for (int x = 0; x < t->size; x++) {
      if (tetg->figurest
              ->blocks[tetg->nextf * t->size * t->size + y * t->size + x]
              .b != 0) {
        mvaddch((y + TET_STAT_NEXT + 1), (x + TET_STAT_X), 'X');
      } else
        mvaddch((y + TET_STAT_NEXT + 1), (x + TET_STAT_X), ' ');
    }
  }

  for (int y = 0; y <= tf->height + 1; y++) {
    for (int x = 0; x <= tf->width + 1; x++) {
      if (y > 0 && x > 0 && x <= tf->width && y <= tf->height) {
        printing_bit = ' ';
        if (tf->blocks[(y - 1) * tf->width + (x - 1)].b != 0) {
          printing_bit = 'X';
        } else {
          int yf = y - 1 - t->y;
          int xf = x - 1 - t->x;
          if (xf >= 0 && xf < t->size && yf >= 0 && yf < t->size)
            if (t->blocks[yf * t->size + xf].b != 0) {
              printing_bit = 'X';
            }
        }
      } else if (y == 0 || y > tetg->field->height) {
        printing_bit = '*';
      } else if (x == 0 || x > tetg->field->width) {
        printing_bit = '*';
      }
      mvaddch(y, x, printing_bit);
    }
  }

  refresh();

  if (tetg->playing == TET_PAUSE) {
    pauseTetGame(tetg);
  }
}

void gameStatTet(TetGame *tetg) {
  clear();

  TetField *tf = tetg->field;
  int printing_bit;
  for (int y = 0; y <= tf->height + 1; y++) {
    for (int x = 0; x <= tf->width + 1; x++) {
      printing_bit = ' ';
      if (y == 0 || y > tetg->field->height) {
        printing_bit = '*';
      } else if (x == 0 || x > tetg->field->width) {
        printing_bit = '*';
      }
      mvaddch(y, x, printing_bit);
    }
  }

  if (tetg->playing == TET_PLAYING) {
    mvwprintw(stdscr, (tf->height / 2), 1, "%s", "START GAME");
    mvwprintw(stdscr, (tf->height / 2 + 1), 1, "%s", "PRESS \"S\"!");
    mvwprintw(stdscr, (tf->height / 2 + 3), 1, "%s", "PAUSE \"P\"!");
    mvwprintw(stdscr, (tf->height / 2 + 4), 1, "%s", "EXIT  \"Q\"!");
    int cl = 0;
    while (cl != 's' && cl != 'S') {
      cl = getch();
    }
  }
  if (tetg->playing == TET_GAMEOVER) {
    mvwprintw(stdscr, (tf->height / 2), 1, "%s", "GAME OVER!");
    refresh();
    sleep(3);
  }
  refresh();
}

// Теперь это надо засунуть в основную отрисовку игры в раздел print
// Типо такого:
// ***********  RECORD:
// *         *  123
// *         *
// *         *  SCORE:
// *         *  1234
// *         *
// *         *  LEVEL:
// *         *  2
// *         *
// *         *  ACTIVITY:
// *         *  Playing
// *         *
// *         *
// *         *
// *         *  NEXT:
// *         *   X
// *         *   X
// *         *   X
// *         *   X
// ***********

int main() {

TetBlock figures_template[] = {
    {0}, {0}, {0}, {0}, {0}, {1}, {1}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0},
    {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {1}, {1}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {1}, {1}, {1}, {0}, {0}, {1}, {0}, {0}, {0}, {0}, {0}, {0}
};


  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  keypad(stdscr, TRUE);

  TetGame *tetg = createTetGame(TET_WIDTH, TET_HEIGTH, 4, 7, figures_template);
  TetPlayer player;
  player.action = TET_PLAYER_NOP;
  tetg->player = &player;

  dropNewFigure(tetg);
  tetg->nextf = rand() % tetg->figurest->count;

  struct timespec start, end, ts1, ts2 = {0, 0};

  gameStatTet(tetg);

  while (tetg->playing != TET_GAMEOVER) {
    clock_gettime(CLOCK_MONOTONIC, &start);
    int cl = getch();

    switch (cl) {
    case KEY_UP:
      player.action = TET_PLAYER_UP;
      break;
    case KEY_DOWN:
      player.action = TET_PLAYER_DOWN;
      break;
    case KEY_LEFT:
      player.action = TET_PLAYER_LEFT;
      break;
    case KEY_RIGHT:
      player.action = TET_PLAYER_RIGHT;
      break;
    case 'p':
    case 'P':
      tetg->playing = TET_PAUSE;
      break;
    case 'q':
    case 'Q':
      tetg->playing = TET_GAMEOVER;
      break;
    default:
      player.action = TET_PLAYER_NOP;
      break;
    }

    calculateTet(tetg);
    printTetGame(tetg);
    clock_gettime(CLOCK_MONOTONIC, &end);

    if (end.tv_sec - start.tv_sec <= 0 &&
        (ts2.tv_nsec = 33000000 - (end.tv_nsec - start.tv_nsec)) > 0)
      nanosleep(&ts2, &ts1);
  }

  FILE *file;
  file = fopen("record.txt", "w");
  fprintf(file, "%d", tetg->record);

  gameStatTet(tetg);

  freeTatGame(tetg);
  endwin();
  return 0;
}