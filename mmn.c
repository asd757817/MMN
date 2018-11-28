#include "mmn.h"

// Create a queue
queue *q_new(){
	queue *q = malloc(sizeof(queue));
	if(q){
		q->head = NULL;
		q->tail = NULL;
		q->size = 0;
		q->t = 0;
		q->dt = 0;
		return q;
	}
	else
		return NULL;
}

// Create a node
node *node_new(double it, double st){
	node *tmp = malloc(sizeof(node));
	if(tmp){
		tmp->it = it;
		tmp->st = st;
		tmp->at = 0;
		return tmp;
	}
	else
		return NULL;
}

// Create a system
_system *system_new(int n, double lambda, double mu){
	_system *tmp = malloc(sizeof(_system));
	if(tmp){
		tmp->time = 0;
		tmp->n = n;
		tmp->lambda = lambda;
		tmp->mu = mu;
		tmp->waiting_time = 0;
		tmp->system_time = 0;
		tmp->total_service_time =0;
		tmp->servers = malloc(n * sizeof(queue *));
		for(int i=0;i<n;i++){
			tmp->servers[i] = q_new();
		}
		return tmp;
	}
	else
		return NULL;
}

// Add node to queue tail
bool q_insert(queue *q, node *n){
	if(!q | !n){
		return false;
	}
	else{
		node *tmp = malloc(sizeof(node));
		tmp->it = n->it;
		tmp->st = n->st;
		if( n->at ==0 ){
			q->t += n->it;
			tmp->at = q->t;
		}
		else{
			tmp->at = n->at + n->it;
		}
		if(q->head && q->tail){
			q->tail->next = tmp;
			q->tail = tmp;
		}
		else{
			q->head = tmp;
			q->tail = tmp;
		}		
		q->size += 1;
		return true;
	}
}

node *q_pop(queue *q){
	node *tmp = q->head;
	if(q->size == 1){
		q->head = NULL;
		q->tail = NULL;
	}
	else{
		q->head = q->head->next;
	}
	q->size -= 1;
	return tmp;
}
double rand_exp(double lambda){
	double u;
	u = rand()/(RAND_MAX + 1.0);// generate random number between 0~1
	return -log(1-u)/lambda;
}

void scheduling(_system *s, node *n, queue *q_next_system){
	if( n ){
		queue *min_q = s->servers[0];
		for(int i=0;i<s->n;i++){
			if( min_q->size > s->servers[i]->size ){
				min_q = s->servers[i];
			}
		}
		s->time = n->at;
		q_insert(min_q, n);
	}

	if(!(n->next)){
		bool ifrun = true;
		while(ifrun){
			for(int i=0;i<s->n;i++){
				while((s->servers[i]->head) && (s->servers[i]->t <= s->time)){
					double q_dt = s->servers[i]->dt;
					double n_at = s->servers[i]->head->at;
					double n_st = s->servers[i]->head->st;
					/* Don't wait */
					if( q_dt <= n_at ){
						s->servers[i]->dt = n_at + n_st; // Depart time
						s->system_time += n_st;
						s->total_service_time += n_st;
						s->servers[i]->head->at = s->servers[i]->dt; // Update arrival time
						if(q_next_system){
							node *tmp = q_pop(s->servers[i]);
							q_insert(q_next_system, tmp);
						}
						else
							q_pop(s->servers[i]);
					}
					/* Need to wait */
					else{
						s->waiting_time += q_dt - n_at;
						s->system_time += q_dt - n_at + n_st;
						s->servers[i]->dt = q_dt + n_st; // Depart time
						s->total_service_time += n_st;
						s->servers[i]->head->at = s->servers[i]->dt; // Update arrival time
						if(q_next_system){
							node *tmp = q_pop(s->servers[i]);
							q_insert(q_next_system, tmp);
						}
						else
							q_pop(s->servers[i]);
					}

				}
				double max_dt = s->servers[0]->dt;
				for(int i=0;i<s->n;i++){
					if(max_dt <= s->servers[i]->dt)
						max_dt = s->servers[i]->dt;
				}
				s->time = max_dt;
			}
			bool keep_run = false;
			for(int i=0;i<s->n;i++){
				if(s->servers[i]->head)
					keep_run = true;
			}
			ifrun = keep_run;
		}
	}

	/* Update system state untill system time*/
	else{
		/* For loop for all queue */
		for(int i=0;i<s->n;i++){ 
			while((s->servers[i]->head) && (s->servers[i]->t <= s->time)){
				double q_dt = s->servers[i]->dt;
				double n_at = s->servers[i]->head->at;
				double n_st = s->servers[i]->head->st;

				//printf("%f %f %f\n", q_dt, n_st, n_at);

				/* Don't wait */
				if( q_dt <= n_at ){
					s->servers[i]->dt = n_at + n_st; // Depart time
					s->system_time += n_st;
					s->total_service_time += n_st;
					s->servers[i]->head->at = s->servers[i]->dt; // Update arrival time
					if(q_next_system){
						node *tmp = q_pop(s->servers[i]);
						q_insert(q_next_system, tmp);
					}
					else
						q_pop(s->servers[i]);
				}
				/* Need to wait */
				else{
					s->waiting_time += q_dt - n_at;
					s->system_time += q_dt - n_at + n_st;
					s->servers[i]->dt = q_dt + n_st; // Depart time
					s->total_service_time += n_st;
					s->servers[i]->head->at = s->servers[i]->dt; // Update arrival time
					if(q_next_system){
						node *tmp = q_pop(s->servers[i]);
						q_insert(q_next_system, tmp);
					}
					else
						q_pop(s->servers[i]);
				}
			}
		}
	}
}

void show_system(_system *s){
	double max_dt = s->servers[0]->dt;
	for(int i=0;i<s->n;i++){
		if( max_dt <= s->servers[i]->dt )
			max_dt = s->servers[i]->dt;
	}
	printf("Service:%f,wt:%f, dt:%f, System:%f\n",s->total_service_time/samples, s->waiting_time/samples, max_dt, s->system_time/samples);
}


