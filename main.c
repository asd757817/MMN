#include "mmn.c"

int main(){
    srand((unsigned)time(NULL));

    // Define parameters

	_system *s1 = system_new(10, 1000, 1000);
	_system *s2 = system_new(20, 2000, 2000);
	_system *s3 = system_new(10, 1000, 1000);
	_system *s4 = system_new(30, 2000, 2000);

	q_system = q_new();
	queue *q_system_2 = q_new();
	queue *q_system_3 = q_new();
	queue *q_system_4 = q_new();
    
    samples = 500000;
   
    // Create events and add to queue
    for(int i=0;i<samples;i++){
        if(q_insert(q_system, node_new(rand_exp(s1->lambda), rand_exp(s1->mu))))
            continue;
        else
            printf("Inset node failed !\n");
    }
	

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
	while(tmp){
		tmp->it = rand_exp(s2->lambda);
		tmp->st = rand_exp(s2->mu);
		scheduling(s2, tmp, q_system_3);
		tmp = tmp->next;
	}
	

	tmp = q_system_3->head;
	while(tmp){
		tmp->it = rand_exp(s3->lambda);
		tmp->st = rand_exp(s3->mu);
		scheduling(s3, tmp, q_system_4);
		tmp = tmp->next;
	}
	
	tmp = q_system_4->head;
	while(tmp){
		tmp->it = rand_exp(s4->lambda);
		tmp->st = rand_exp(s4->mu);
		scheduling(s4, tmp, NULL);
		tmp = tmp->next;
	}
	
	printf("\n");
    show_system(s1);
	show_system(s2);
	show_system(s3);
	show_system(s4);
	return 0;
}


