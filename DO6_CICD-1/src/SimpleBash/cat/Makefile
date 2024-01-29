ALL: clean s21_cat

s21_cat:
	gcc -Wall -Werror -Wextra main.c parcer.c reader.c -o s21_cat

clean:
	rm -rf s21_cat

test_func:ALL
	sh test_func_cat.sh

test_leak:ALL
	sh test_leak_cat.sh

test_func_linux:ALL
	bash test_func_cat.sh

style:
	clang-format -n *.c *.h --style=Google
