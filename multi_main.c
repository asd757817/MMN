#include "multi_mm1.c"

int main(){
	srand((unsigned)time(NULL));

	// Define parameters

	double *avg_service_time = malloc(4 * sizeof(double));
	double *avg_waiting_time = malloc(4 * sizeof(double));
	double *avg_departure_time = malloc(4 * sizeof(double));
	double *avg_system_time = malloc(4 * sizeof(double));

	int repeat = 25;
	samples = 1000;
	for(int i=0;i<repeat;i++){
		_system **s = malloc(4 * sizeof(_system *));
		s[0] = system_new(1, 2, 1.5);
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

				if(q_insert(q_system, node_new(rand_exp(s[0]->lambda), rand_exp(s[0]->mu))))
					continue;
				else
					printf("Inset node failed !\n");
			}
			else if(!q_system->tail || q_system->tail->at <= 240){
				if(q_insert(q_system, node_new(rand_exp(0.5), rand_exp(s[0]->mu))))
					continue;
				else
					printf("Inset node failed !\n");
			}

			else if( !q_system->tail || q_system->tail->at <= 360){
				if(q_insert(q_system, node_new(rand_exp(2.5), rand_exp(s[0]->mu))))
					continue;
				else
					printf("Inset node failed !\n");
			}

			else if( !q_system->tail || q_system->tail->at <= 480){
				if(q_insert(q_system, node_new(rand_exp(3.5), rand_exp(s[0]->mu))))
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
		
		FILE *fp = fopen("multi.txt", "w");

		while(tmp){
			fprintf(fp,"%f %f %f\n", tmp->it, tmp->st, tmp->at);
			tmp = tmp->next;
		}

		tmp = q_system->head;
		while(tmp){
			scheduling(s[0], tmp, q_system_2);
			tmp = tmp->next;
		}

		tmp = q_system_2->head;

		fprintf(fp,"=====System_2=====\n");
		while(tmp){
			tmp->it = rand_exp(s[1]->lambda);
			tmp->st = rand_exp(s[1]->mu);
			scheduling(s[1], tmp, q_system_3);
			fprintf(fp,"%f %f %f\n", tmp->it, tmp->st, tmp->at);
			tmp = tmp->next;
		}


		fprintf(fp,"=====System_3=====\n");
		tmp = q_system_3->head;
		while(tmp){
			tmp->it = rand_exp(s[2]->lambda);
			tmp->st = rand_exp(s[2]->mu);
			scheduling(s[2], tmp, q_system_4);
			fprintf(fp,"%f %f %f\n", tmp->it, tmp->st, tmp->at);
			tmp = tmp->next;
		}

		fprintf(fp,"=====System_4=====\n");
		tmp = q_system_4->head;
		while(tmp){
			tmp->it = rand_exp(s[3]->lambda);
			tmp->st = rand_exp(s[3]->mu);
			scheduling(s[3], tmp, NULL);
			fprintf(fp,"%f %f %f\n", tmp->it, tmp->st, tmp->at);
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


