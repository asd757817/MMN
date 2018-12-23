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
		tmp->at = -1;			// initialize
		tmp->which_server = -1;	// initialize
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
		tmp->dt = n->dt;
		tmp->wt = n->wt;
		
		/* First time entering the system */
		if( n->at == -1 ){
			q->t += n->it;	
			tmp->at = q->t;
		}
		else{
			tmp->at = n->at;
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

double rand_normal(double std, double mean){
	double u = rand() / (double)RAND_MAX;
	double v = rand() / (double)RAND_MAX;
	double w = sqrt(-2 * log(u)) * cos(2 * M_PI * v);
	double x ;
	/* Constrain the value interval */
	if( w < -3 )
		x = mean - 3*std;
	else if (w > 3)
		x = mean + 3*std;
	else
		x = mean + w*std;
	return x;
}

void scheduling(_system *s, node *n, queue *q_next_system, FILE *fp){
	if( n ){
		bool if_insert = false;
		while( !if_insert ){
			/* Update system time and update states of all queues */
			s->time = n->at;
			for(int i=0;i<s->n;i++){
				if( s->servers[i]->size > 0 && s->servers[i]->dt <= s->time ){
					s->servers[i]->dt = s->servers[i]->head->at;
					if(q_next_system){
						node *tmp = q_pop(s->servers[i]);
						tmp->at = tmp->dt, tmp->dt =0, tmp->wt=0;
						q_insert(q_next_system, tmp);
					}
					else
						q_pop(s->servers[i]);
				}
			}

			/* Check status of all queues and find the min_dt in all busy queues */
			queue *min_q = q_new();
			min_q->dt = DBL_MAX;
			bool need_wait = true;
			for(int i=0;i<s->n;i++){
				/* Server isn't busy, assign node to it */
				if(s->servers[i]->size == 0){
					s->servers[i]->dt = n->at + n->st;
					s->system_time += n->st;
					s->total_service_time += n->st;
					n->dt = n->at + n->st;
					n->at = n->at - n->wt;	// shift back
					/* Assign node to queue */
					n->which_server = i;
					if_insert = q_insert(s->servers[i], n);
					need_wait = false;
					break;
				}
				else if( min_q->dt > s->servers[i]->dt ){
					min_q = s->servers[i];
				}
			}
			/* If all queue are busy, find the min_dt to calculate the waiting time */
			if(need_wait){
				s->waiting_time += min_q->dt - n->at;
				n->wt = min_q->dt - n->at;
				s->system_time += min_q->dt - n->at;
				n->at = min_q->dt;	// shift real arrival time to min_q->dt
			}
		}
		if(!(n->next)){
			s->time = get_dt(s);
			for(int i=0;i<s->n;i++){
				if( s->servers[i]->size > 0 ){
					if( s->servers[i]->dt <= s->time ){
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
		fprintf(fp, "%f %f %f %f %f %d\n", n->it, n->st, n->at, n->dt, n->wt, n->which_server);
	}
}


void show_system(_system *s){
	double max_dt = s->servers[0]->dt;
	for(int i=0;i<s->n;i++){
		if( max_dt <= s->servers[i]->dt )
			max_dt = s->servers[i]->dt;
	}
	printf("Service:%f,wt:%f, dt:%f, System:%f\n",s->total_service_time/count, s->waiting_time/count, max_dt, s->system_time/count);
}


double get_dt(_system *s){
	double max_dt = s->servers[0]->dt;
	for(int i=0;i<s->n;i++){
		if( max_dt <= s->servers[i]->dt )
			max_dt = s->servers[i]->dt;
	}
	//printf("Departure time:%f\n", max_dt);
	return max_dt;
}

