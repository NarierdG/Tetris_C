CC = gcc
FLAG = -Wall -Werror -Wextra

all: install

install: mkbuild
	$(CC) $(FLAG) -c brick_game/tetris/tetris.c
	$(CC) $(FLAG) -c gui/cli/front.c
	$(CC) $(FLAG) *.o -lncurses -o build/tetris
	rm -f *.o *.txt

mkbuild:
	rm -rf build/
	mkdir build

uninstall: clean
	rm -rf build/

add_gcov_flags:
	$(eval FLAG += --coverage -fprofile-arcs -ftest-coverage )

clean:
	rm -rf *.o *.txt *.dSYM *.gcda *.gcno dist.tar.gz *.info test brick_game/tetris/*.gcda brick_game/tetris/*.gcno report/ html/ latex/

dist: install
	rm -rf dist.tar.gz
	tar -cvzf dist.tar.gz build/tetris

back_lib:
	$(CC) $(FLAG) brick_game/tetris/tetris.c -c
	ar rsc build/back.a *.o
	ranlib build/back.a
	rm -f *.o

test: clean mkbuild back_lib
	$(CC) $(FLAG) brick_game/tetris/test/test.c -L. build/back.a -o test -lncurses -lcheck -lm -lsubunit -lpthread
	./test

gcov_report: add_gcov_flags test
	./test
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	rm -f *.gcda *.gcno *.info test

run:
	build/tetris