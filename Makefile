all: run

run: mmn.c mmn.h main_mmn.c multi_mm1.c multi_mm1.h main_multi_mm1.c
	gcc -o mmn main_mmn.c -lm && ./mmn
	gcc -o multi_mm1 main_multi_mm1.c -lm && ./multi_mm1

exe: mmn.h multi_mm1.h out_main_mmn.c out_main_multi_mm1.c
	gcc -o mmn_output out_main_mmn.c -lm && ./mmn_output 1200 2 4 2 4
	gcc -o multi_mm1_output out_main_multi_mm1.c -lm && ./multi_mm1_output 1200 2 4 2 4

output_multi.txt: $(exe)
	./multi_mm1_output 1200 1 2 1 2

output_mmn.txt:	$(exe)
	./mmn_output 1200 1 2 1 2

plot: output_multi.txt output_mmn.txt
	#gnuplot scripts/mm1_wait.gp && eog scripts/photo/mm1_wait_time.png 
	#gnuplot scripts/mmn_wait.gp && eog scripts/photo/mmn_wait_time.png 
	gnuplot scripts/mmn_service.gp && eog scripts/photo/mmn_service_time.png 
	gnuplot scripts/mm1_service.gp && eog scripts/photo/mm1_service_time.png 
	#gnuplot scripts/best_wt.gp && eog scripts/best_wt.png 
	#gnuplot scripts/best_dt.gp && eog scripts/best_dt.png 
clean:
	rm -f *.o

