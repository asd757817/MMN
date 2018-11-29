#include "mmn.c"

int main(){
	srand((unsigned)time(NULL));

	// Define parameters

	_system *s1 = system_new(2, 0.5, 2);
	_system *s2 = system_new(5, 0, 1);
	_system *s3 = system_new(2, 0, 2);
	_system *s4 = system_new(5, 0, 1.6);
	_system *s5 = system_new(5, 0, 1.6);

	q_system = q_new();
	queue *q_system_2 = q_new();
	queue *q_system_3 = q_new();
	queue *q_system_4 = q_new();
	queue *q_system_5 = q_new();
	
	samples = 1000;
	
	
	// Create events and add to queue
	for(int i=0;i<samples;i++){
		if( !q_system->tail || q_system->tail->at <=120){
		
			if(q_insert(q_system, node_new(rand_exp(s1->lambda), rand_exp(s1->mu))))
				continue;
			else
				printf("Inset node failed !\n");
		}
		else if(!q_system->tail || q_system->tail->at <= 240){
			if(q_insert(q_system, node_new(rand_exp(1.5), rand_exp(s1->mu))))
				continue;
			else
				printf("Inset node failed !\n");
		}

		else if( !q_system->tail || q_system->tail->at <= 360){
			if(q_insert(q_system, node_new(rand_exp(2.7), rand_exp(s1->mu))))
				continue;
			else
				printf("Inset node failed !\n");
		}
		
		else if( !q_system->tail || q_system->tail->at <= 480){
			if(q_insert(q_system, node_new(rand_exp(3.8), rand_exp(s1->mu))))
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

	FILE *fp = fopen("test.txt", "w");
	
	while(tmp){
		fprintf(fp,"%f %f %f\n", tmp->it, tmp->st, tmp->at);
		tmp = tmp->next;
	}

	tmp = q_system->head;
	while(tmp){
		scheduling(s1, tmp, q_system_2);
		tmp = tmp->next;
	}

	tmp = q_system_2->head;

	fprintf(fp,"=====System_2=====\n");
	while(tmp){
		tmp->it = rand_exp(s2->lambda);
		tmp->st = rand_exp(s2->mu);
		scheduling(s2, tmp, q_system_3);
		fprintf(fp,"%f %f %f\n", tmp->it, tmp->st, tmp->at);
		tmp = tmp->next;
	}


	fprintf(fp,"=====System_3=====\n");
	tmp = q_system_3->head;
	while(tmp){
		tmp->it = rand_exp(s3->lambda);
		tmp->st = rand_exp(s3->mu);
		scheduling(s3, tmp, q_system_4);
		fprintf(fp,"%f %f %f\n", tmp->it, tmp->st, tmp->at);
		tmp = tmp->next;
	}
	
	fprintf(fp,"=====System_4=====\n");
	tmp = q_system_4->head;
	while(tmp){
		tmp->it = rand_exp(s4->lambda);
		tmp->st = rand_exp(s4->mu);
		scheduling(s4, tmp, q_system_5);
		fprintf(fp,"%f %f %f\n", tmp->it, tmp->st, tmp->at);
		tmp = tmp->next;
	}

	fprintf(fp,"=====System_5=====\n");
	tmp = q_system_5->head;
	while(tmp){
		tmp->it = rand_exp(s5->lambda);
		tmp->st = rand_exp(s5->mu);
		scheduling(s5, tmp, NULL);
		fprintf(fp,"%f %f %f\n", tmp->it, tmp->st, tmp->at);
		tmp = tmp->next;
	}

	printf("\n");
	show_system(s1);
	show_system(s2);
	show_system(s3);
	show_system(s4);
	show_system(s5);
	return 0;
}


