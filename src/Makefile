all : main

clean :
	rm -rf *.o a.out ../build/graph

cppstyle:
	cp -R ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extensions=c *.c
	rm -rf CPPLINT.cfg

format:
	clang-format -i *.c

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem ./

add:
	git add func.c func.h parse.c parse.h structs.c structs.h print.c print.h graph.c Makefile
	git commit -m "."
	git push origin dierdreh

structs.o: structs.c
	gcc -Wall -Werror -Wextra -c structs.c

parse.o: parse.c
	gcc -Wall -Werror -Wextra -c parse.c

func.o: func.c
	gcc -Wall -Werror -Wextra -c func.c

print.o: print.c
	gcc -Wall -Werror -Wextra -c print.c

graph.o: graph.c
	gcc -Wall -Werror -Wextra -c graph.c

main: structs.o parse.o func.o print.o graph.o
	gcc -Wall -Werror -Wextra func.o structs.o print.o parse.o graph.o -o ../build/graph

leaks:
	leaks -atExit -- ../build/graph