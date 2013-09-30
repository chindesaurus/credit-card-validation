credit: credit.c
	gcc -ggdb -std=c99 -Wall -Werror -o credit credit.c -lm

clean:
	rm -f *.o a.out core credit
