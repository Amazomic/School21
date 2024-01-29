ALL: clean s21_grep

s21_grep:
	gcc -Wall -Werror -Wextra s21_grep.c parcer.c  run_flags.c  -o s21_grep

clean:
	rm -rf s21_grep

test_func:ALL
	sh test_func_grep.sh

test_leak:ALL
	sh test_leak_grep.sh

test_func_linux:ALL
	bash test_func_grep.sh

style:
	clang-format -n *.c *.h --style=Google



