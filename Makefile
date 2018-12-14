all: run

run: mmn.c mmn.h main_mmn.c multi_mm1.c multi_mm1.h main_multi_mm1.c
	gcc -o mmn main_mmn.c -lm && ./mmn
	gcc -o multi_mm1 main_multi_mm1.c -lm && ./multi_mm1
	

clean:
	rm -f *.o

