#include "tetris.h"

#ifndef TETRIC_C
#define TETRIC_C

TetFigurest* createTetFigurest(int count, int figure_size,
                               TetBlock* figures_template) {
  TetFigurest* tetft = (TetFigurest*)malloc(sizeof(TetFigurest));
  tetft->count = count;
  tetft->size = figure_size;
  tetft->blocks = figures_template;
  return tetft;
}

void freeTetFigurest(TetFigurest* tetft) {
  if (tetft) {
    free(tetft);
  }
}

TetField* createTetField(int wigth, int height) {
  TetField* tetf = (TetField*)malloc(sizeof(TetField));
  tetf->width = wigth;
  tetf->height = height;
  tetf->blocks = (TetBlock*)malloc(sizeof(TetBlock) * wigth * height);

  for (int i = 0; i < wigth * height; i++) {
    tetf->blocks[i].b = 0;
  }

  return tetf;
}

void freeTetField(TetField* tetf) {
  if (tetf) {
    if (tetf->blocks) {
      free(tetf->blocks);
    }
    free(tetf);
  }
}

TetGame* createTetGame(int field_wigth, int field_height, int figure_size,
                       int count, TetBlock* figures_template) {
  
  FILE* file;

  TetGame* tetg = (TetGame*)malloc(sizeof(TetGame));
  tetg->field = createTetField(field_wigth, field_height);
  tetg->figurest = createTetFigurest(count, figure_size, figures_template);

  tetg->ticks = TET_TICKET_GO;
  tetg->ticks_forced = TET_TICKET_GO;
  tetg->score = 0;
  tetg->playing = TET_PLAYING;
  if ((file = fopen("record.txt", "r"))) {
    fscanf(file, "%d", &tetg->record);
  } else
    tetg->record = 0;
  tetg->level = 0;
  tetg->level_s = 0;
  tetg->nextf = rand() % tetg->figurest->count;

  return tetg;
}

void freeTatGame(TetGame* tetg) {
  if (tetg) {
    freeTetFigurest(tetg->figurest);
    freeTetField(tetg->field);
    free(tetg);
  }
}

void moveFigureDown(TetGame* tetg) { tetg->figure->y++; }

void moveFigureUp(TetGame* tetg) { tetg->figure->y--; }

void moveFigureRight(TetGame* tetg) { tetg->figure->x++; }

void moveFigureLeft(TetGame* tetg) { tetg->figure->x--; }

int collisionTet(TetGame* tetg) {
  TetFigure* t = tetg->figure;
  TetField* tf = tetg->field;
  int res = 0;

  // Предполагаем фигуру в виде квадрата со стороной size
  for (int y = 0; y < t->size; y++) {
    for (int x = 0; x < t->size; x++) {
      if (t->blocks[y * t->size + x].b != 0) {
        int fx = t->x + x;
        int fy = t->y + y;
        if (tf->blocks[fy * tf->width + fx].b != 0) res = 1;
        if (fx < 0 || fx >= tf->width || fy < 0 || fy >= tf->height) res = 1;
      }
    }
  }
  return res;
}

void plantFigure(TetGame* tetg) {
  TetFigure* t = tetg->figure;
  int score = 0;
  for (int y = 0; y < t->size; y++) {
    for (int x = 0; x < t->size; x++) {
      if (t->blocks[y * t->size + x].b != 0) {
        score++;
        int fy = t->y + y;
        int fx = t->x + x;
        tetg->field->blocks[fy * tetg->field->width + fx].b =
            t->blocks[y * t->size + x].b;
      }
    }
  }
  tetg->score += score;
  if (tetg->score > tetg->record) tetg->record = tetg->score;
}

// Создаем функцию подсчитывающую очки и удаляющую собранную линию
int resultLineTet(TetGame* tetg) {
  TetField* tf = tetg->field;
  int count;
  int line_activity = 0;
  int res = 0;
  for (int y = 0; y < tf->height; y++) {
    count = 0;
    for (int x = 0; x < tf->width; x++) {
      if (tf->blocks[y * tf->width + x].b != 0) count++;
      if (count == tf->width) {
        for (int y_new = y; y_new > 0; y_new--) {
          for (int x_new = 0; x_new < tf->width; x_new++) {
            tf->blocks[y_new * tf->width + x_new].b =
                tf->blocks[(y_new - 1) * tf->width + x_new].b;
          }
        }
        for (int i = 0; i < tf->width; i++) tf->blocks[i].b = 0;
        line_activity += 1;
      }
    }
  }

  if (line_activity == 1)
    res = LINE_1;
  else if (line_activity == 2)
    res = LINE_2;
  else if (line_activity == 3)
    res = LINE_3;
  else if (line_activity >= 4)
    res = LINE_4;

  return res;
}

TetFigure* createTetFigure(TetGame* tetg) {
  TetFigure* t = (TetFigure*)malloc(sizeof(TetFigure));
  t->x = 0;
  t->y = 0;
  t->size = tetg->figurest->size;
  t->blocks = (TetBlock*)malloc(sizeof(TetBlock) * t->size * t->size);
  return t;
}

void freeTetFigure(TetFigure* tetfig) {
  if (tetfig) {
    if (tetfig->blocks) {
      free(tetfig->blocks);
    }
    free(tetfig);
  }
}

void dropNewFigure(TetGame* tetg) {
  TetFigure* t = createTetFigure(tetg);
  t->x = tetg->field->width / 2 - t->size / 2;
  t->y = 0;
  int fnum = tetg->nextf;
  for (int y = 0; y < t->size; y++) {
    for (int x = 0; x < t->size; x++) {
      t->blocks[y * t->size + x].b =
          tetg->figurest->blocks[fnum * t->size * t->size + y * t->size + x].b;
    }
  }
  freeTetFigure(tetg->figure);
  tetg->figure = t;
  tetg->nextf = rand() % tetg->figurest->count;
}

TetFigure* rotFigure(TetGame* tetg) {
  TetFigure* t = createTetFigure(tetg);
  TetFigure* told = tetg->figure;

  t->x = told->x;
  t->y = told->y;

  for (int y = 0; y < t->size; y++) {
    for (int x = 0; x < t->size; x++) {
      t->blocks[y * t->size + x].b =
          told->blocks[x * t->size + t->size - 1 - y].b;
    }
  }
  return t;
}

void calculateTet(TetGame* tetg) {
  // Так как эта функция будет вызываться примерно 30 раз в секунду,
  // а скорость перемещения фигуры куда медленее, будем вести отчет тактов
  if ((tetg->score - tetg->level_s) > 600) {
    tetg->level_s += 600;
    tetg->level++;
  }

  if (tetg->ticks_forced <= 0) {
    tetg->ticks_forced = tetg->ticks - tetg->level * 2;
    moveFigureDown(tetg);
    if (collisionTet(tetg)) {
      moveFigureUp(tetg);
      plantFigure(tetg);
      tetg->score += resultLineTet(tetg);
      if (tetg->score > tetg->record) tetg->record = tetg->score;
      dropNewFigure(tetg);
      if (collisionTet(tetg)) {
        tetg->playing = TET_GAMEOVER;
      }
    }
  }

  switch (tetg->player->action) {
    case TET_PLAYER_RIGHT:
      moveFigureRight(tetg);
      if (collisionTet(tetg)) moveFigureLeft(tetg);
      break;
    case TET_PLAYER_LEFT:
      moveFigureLeft(tetg);
      if (collisionTet(tetg)) moveFigureRight(tetg);
      break;
    case TET_PLAYER_DOWN:
      moveFigureDown(tetg);
      if (collisionTet(tetg)) moveFigureUp(tetg);
      break;
    case TET_PLAYER_UP: {
      TetFigure* t = rotFigure(tetg);
      TetFigure* told = tetg->figure;
      tetg->figure = t;
      if (collisionTet(tetg)) {
        tetg->figure = told;
        freeTetFigure(t);
      } else {
        freeTetFigure(told);
      }
      break;
    }
    case TET_PLAYER_NOP:
    default:
      break;
  }

  tetg->ticks_forced--;
}

#endif