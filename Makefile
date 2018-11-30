all: run

run: mmn.c mmn.h mmn_main.c multi_mm1.c multi_mm1.h multi_main.c
	gcc -o mmn mmn_main.c -lm && ./mmn
	gcc -o multi_mm1 multi_main.c -lm && ./multi_mm1
	

clean:
	rm -f *.o

