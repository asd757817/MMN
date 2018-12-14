#include "multi_mm1.c"

int main(){
	srand((unsigned)time(NULL));

	// Define parameters

	double *avg_service_time = malloc(4 * sizeof(double));
	double *avg_waiting_time = malloc(4 * sizeof(double));
	double *avg_departure_time = malloc(4 * sizeof(double));
	double *avg_system_time = malloc(4 * sizeof(double));

	int repeat = 1;
	samples = 50;
	double std_2 = 0.3, mean_2 = 1;
	double std_3 = 0.1, mean_3 = 0.5;
	double std_4 = 0.3, mean_4 = 1;

	FILE *fp = fopen("output_multi_mm1.txt", "w");
	for(int i=0;i<repeat;i++){
		_system **s = malloc(4 * sizeof(_system *));
		s[0] = system_new(1, 0.25, 1.5);
		s[1] = system_new(1, 0, 1.2);
		s[2] = system_new(1, 0, 1.5);
		s[3] = system_new(1, 0, 1);

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
				if(q_insert(q_system, node_new(rand_exp(0.5), rand_normal(0.1,0.6))))
					continue;
				else
					printf("Inset node failed !\n");
			}

			else if( !q_system->tail || q_system->tail->at <= 360){
				if(q_insert(q_system, node_new(rand_exp(2.5), rand_normal(0.1,0.6))))
					continue;
				else
					printf("Inset node failed !\n");
			}

			else if( !q_system->tail || q_system->tail->at <= 480){
				if(q_insert(q_system, node_new(rand_exp(3.5), rand_normal(0.1,0.6))))
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

	printf("\n");
	for(int i=0;i<4;i++){
		printf("Ser_t:%f, Wai_t:%f, Dep_t:%f, Sys_t:%f\n", avg_service_time[i]/repeat, avg_waiting_time[i]/repeat, avg_departure_time[i]/repeat, avg_system_time[i]/repeat);
	}
	/*
	 * show_system(s[0]);
	 * show_system(s[1]);
	 * show_system(s[2]);
	 * show_system(s[3]);
	 */

	return 0;
}


