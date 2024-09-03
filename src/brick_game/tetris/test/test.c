#include <check.h>
#include <stdlib.h>
#include "../tetris.h"

START_TEST(test_1) {
  TetBlock figures_template[] = {
    {0}, {0}, {0}, {0}, {0}, {1}, {1}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0},
    {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {1}, {1}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {1}, {0}, {0}, {0}, {1}, {0}, {0}, {1}, {1}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {1}, {1}, {1}, {0}, {0}, {1}, {0}, {0}, {0}, {0}, {0}, {0}
  };

    TetGame* tetg = createTetGame(10, 20, 4, 7, figures_template);
    TetPlayer player;
    player.action = TET_PLAYER_NOP;
    tetg->player = &player;

    dropNewFigure(tetg);
    tetg->nextf = rand() % tetg->figurest->count;

    calculateTet(tetg);
    player.action = TET_PLAYER_UP;
    moveFigureDown(tetg);
    calculateTet(tetg);
    player.action = TET_PLAYER_DOWN;
    calculateTet(tetg);
    player.action = TET_PLAYER_NOP;
    calculateTet(tetg);
    player.action = TET_PLAYER_LEFT;
    calculateTet(tetg);
    player.action = TET_PLAYER_RIGHT;
    calculateTet(tetg);
    moveFigureDown(tetg);

    tetg->ticks_forced = 0;
    calculateTet(tetg);

    plantFigure(tetg);
    resultLineTet(tetg);


    tetg->playing = TET_GAMEOVER;
    freeTatGame(tetg);
}
END_TEST

Suite *back() {
  Suite* s;
  TCase* tc_core;

  s = suite_create("back func");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_1);

  suite_add_tcase(s, tc_core);

  return s;
}

int main() {
  int number_failed = 0;
  SRunner* sr;

  sr = srunner_create(NULL);

  srunner_add_suite(sr, back());

  srunner_run_all(sr, CK_NORMAL);

  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

