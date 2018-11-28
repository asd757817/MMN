all: run

run: mmn.c mmn.h main.c
	gcc -o mmn main.c -lm && ./mmn
	

clean:
	rm -f *.o

