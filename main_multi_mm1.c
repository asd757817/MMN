#include "multi_mm1.c"

int main(){
	srand((unsigned)time(NULL));

	// Define parameters

	double *avg_service_time = malloc(4 * sizeof(double));
	double *avg_waiting_time = malloc(4 * sizeof(double));
	double *avg_departure_time = malloc(4 * sizeof(double));
	double *avg_system_time = malloc(4 * sizeof(double));

	int repeat = 10;
	samples = 1000;
	double x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	double std_2 = 0.2, mean_2 = 1;
	double std_3 = 0.1, mean_3 = 0.6;
	double std_4 = 0.3, mean_4 = 1.5;

	FILE *fp = fopen("multi.txt", "w");
	for(int i=0;i<repeat;i++){
		_system **s = malloc(4 * sizeof(_system *));
		s[0] = system_new(1, x1, 1.5);
		s[1] = system_new(2, 0, 1.2);
		s[2] = system_new(1, 0, 1.5);
		s[3] = system_new(2, 0, 1);

		q_system = q_new();
		queue *q_system_2 = q_new();
		queue *q_system_3 = q_new();
		queue *q_system_4 = q_new();

		// Create events and add to queue
		for(int i=0;i<samples;i++){
			if( !q_system->tail || q_system->tail->at <=120){

				if(q_insert(q_system, node_new(rand_exp(s[0]->lambda), rand_normal(0.1,0.6))))
					continue;
				else
					printf("Inset node failed !\n");
			}
			else if(!q_system->tail || q_system->tail->at <= 240){
				if(q_insert(q_system, node_new(rand_exp(x2), rand_normal(0.1,0.6))))
					continue;
				else
					printf("Inset node failed !\n");
			}

			else if( !q_system->tail || q_system->tail->at <= 360){
				if(q_insert(q_system, node_new(rand_exp(x3), rand_normal(0.1,0.6))))
					continue;
				else
					printf("Inset node failed !\n");
			}

			else if( !q_system->tail || q_system->tail->at <= 480){
				if(q_insert(q_system, node_new(rand_exp(x4), rand_normal(0.1,0.6))))
					continue;
				else
					printf("Inset node failed !\n");
			}

			else {
				count = i;
				break;
			}
		}
		if( count == 0 )
			count = samples;
		node *tmp = q_system->head;

		//printf("\nThe last person entered the system at: %f \n", q_system->tail->at);
		//printf("Total votes: %d \n", count);


		fprintf(fp,"=====System_1=====\n");
		tmp = q_system->head;
		while(tmp){
			scheduling(s[0], tmp, q_system_2, fp);
			tmp = tmp->next;
		}

		tmp = q_system_2->head;

		fprintf(fp,"=====System_2=====\n");
		while(tmp){
			tmp->it = rand_exp(s[1]->lambda);
			tmp->st = rand_normal(std_2, mean_2);
			scheduling(s[1], tmp, q_system_3, fp);
			tmp = tmp->next;
		}


		fprintf(fp,"=====System_3=====\n");
		tmp = q_system_3->head;
		while(tmp){
			tmp->it = rand_exp(s[2]->lambda);
			tmp->st = rand_normal(std_3, mean_3);
			scheduling(s[2], tmp, q_system_4, fp);
			tmp = tmp->next;
		}

		fprintf(fp,"=====System_4=====\n");
		tmp = q_system_4->head;
		while(tmp){
			tmp->it = rand_exp(s[3]->lambda);
			tmp->st = tmp->st = rand_normal(std_4, mean_4);
			scheduling(s[3], tmp, NULL, fp);
			tmp = tmp->next;
		}

		for(int i=0;i<4;i++){
			avg_service_time[i] += s[i]->total_service_time/count;
			avg_waiting_time[i] += s[i]->waiting_time/count;
			avg_departure_time[i] += get_dt(s[i]);
			avg_system_time[i] += s[i]->system_time/count;
		}
	}
	for(int i=0;i<4;i++){
		int hours = (int) avg_departure_time[i]/repeat/60;
		int minutes = (int)avg_departure_time[i]/repeat%60;
		printf("Service_time:%f, Waiting_time:%f, System_time:%f, 最後一人離開投票所時間：%d:%02d\n"",", avg_service_time[i]/repeat, avg_waiting_time[i]/repeat, avg_system_time[i]/repeat, hours+8, minutes);
	}
	/*
	 * show_system(s[0]);
	 * show_system(s[1]);
	 * show_system(s[2]);
	 * show_system(s[3]);
	 */

	return 0;
}


