// Так называемая защита от множественного включения
// Эта директива проверяет, определена ли макрос-константа TETRIS
// И если TETRIS не определена, выполняется код между #ifndef и #endif
#ifndef TETRIS_H
#define TETRIS_H

#include <curses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define TET_TICKET_GO 30
#define TET_LEVEL_GO 600

#define TET_WIDTH 10
#define TET_HEIGTH 20

#define TET_STAT_X 13
#define TET_STAT_RECORD 0
#define TET_STAT_SCORE 3
#define TET_STAT_LEVEL 6
#define TET_STAT_ACTIVITY 9
#define TET_STAT_NEXT 14

enum { LINE_1 = 100, LINE_2 = 300, LINE_3 = 700, LINE_4 = 1500 };

enum {
  TET_PLAYER_NOP = 0,
  TET_PLAYER_UP,
  TET_PLAYER_DOWN,
  TET_PLAYER_LEFT,
  TET_PLAYER_RIGHT
};

enum { TET_GAMEOVER = 0, TET_PLAYING, TET_PAUSE };

typedef struct TetBlock {
  int b;
} TetBlock;

typedef struct TetField {
  int width;
  int height;
  TetBlock* blocks;
} TetField;

typedef struct TetFigure {
  int x;
  int y;
  int size;
  TetBlock* blocks;
} TetFigure;

typedef struct TetFigurest {
  int count;
  int size;
  TetBlock* blocks;
} TetFigurest;

typedef struct TetPlayer {
  int action;
} TetPlayer;

typedef struct TetGame {
  TetField* field;
  TetFigure* figure;
  TetFigurest* figurest;
  TetPlayer* player;
  int ticks;
  int ticks_forced;
  int score;
  int playing;
  int record;
  int level;
  int level_s;
  int nextf;

} TetGame;

TetFigurest* createTetFigurest(int count, int figure_size,
                               TetBlock* figures_template);
void freeTetFigurest(TetFigurest* tetft);
TetField* createTetField(int wigth, int height);
void freeTetField(TetField* tetf);
TetGame* createTetGame(int field_wigth, int field_height, int figure_size,
                       int count, TetBlock* figures_template);
void freeTatGame(TetGame* tetg);
void moveFigureDown(TetGame* tetg);
void moveFigureUp(TetGame* tetg);
void moveFigureRight(TetGame* tetg);
void moveFigureLeft(TetGame* tetg);
int collisionTet(TetGame* tetg);
void plantFigure(TetGame* tetg);
int resultLineTet(TetGame* tetg);
TetFigure* createTetFigure(TetGame* tetg);
void freeTetFigure(TetFigure* tetfig);
void dropNewFigure(TetGame* tetg);
TetFigure* rotFigure(TetGame* tetg);
void calculateTet(TetGame* tetg);

#endif