# M/M/N_model and parallel multiple M/M/1 model
* mmn_* for M/M/N
* multi_mm1 for parallel multiple M/M/1

## Usage
```shell
$ make
### output ###
gcc -o mmn mmn_main.c -lm && ./mmn

Ser_t:0.677105, Wai_t:83.569473, Dep_t:708.182249, Sys_t:84.246578
Ser_t:0.848938, Wai_t:0.506480, Dep_t:710.399607, Sys_t:1.355418
Ser_t:0.668915, Wai_t:8.994543, Dep_t:725.735465, Sys_t:9.663459
Ser_t:1.007326, Wai_t:0.921483, Dep_t:727.956412, Sys_t:1.928809
gcc -o multi_mm1 multi_main.c -lm && ./multi_mm1

Ser_t:0.677105, Wai_t:83.569473, Dep_t:708.182249, Sys_t:84.246578
Ser_t:0.848938, Wai_t:0.740978, Dep_t:710.882497, Sys_t:1.589916
Ser_t:0.668915, Wai_t:9.766296, Dep_t:726.711382, Sys_t:10.435211
Ser_t:1.007326, Wai_t:1.264990, Dep_t:729.719037, Sys_t:2.272316
```

Ser_t: Average service time
Wai_t: Average waiting time
Sys_t: Average system time
Dep_t: The average timing when the last node left the system

Four lines for four systems.
Dep_t in first line means the last one leaves system_1 at 708.182249, and the second Dep_t means the last one leaves system_2 at 710.399607.

## Mdify mmn_main.c and multi_main.c to generate your model

1. Create system with N servers. lambda and mu are parameter for exp random variable generator.
```
	_system *s1 = system_new(1, 0.5, 0.6); //N, lambda, mu
	_system *s2 = system_new(2, 0.4, 0.4);
```

2. Create nodes(customers or events) and store into a queue.
```
/* Create a queue to store nodes */
  q_system = q_new();	
  queue *q_system_2 = q_new();
  
/* Number of nodes */
  samples = 500000;
  
/* Generate interarrival time and service time and put nodes into queue*/
  for(int i=0;i<samples;i++){
      if(q_insert(q_system, node_new(rand_exp(s1->lambda), rand_exp(s1->mu))))
          continue;
      else
          printf("Inset node failed !\n");
  }
```

3. Put nodes into system
```
	node *tmp = q_system->head;
	while(tmp){
		scheduling(s1, tmp, q_system_2);
		/*  
		*  Put tmp into s1
		*  q_system_2 means nodes will enter system_2 right after system_1  
		*  If there's not the next system, replace it with NULL, scheduling(s1, tmp, NULL)
		*/
		tmp = tmp->next;
	}
```

4. One system connects to the next system and update interarrival time and service time.
```
	tmp = q_system_2->head;
	while(tmp){
		tmp->it = rand_exp(s2->lambda); // Not be used in my program
		tmp->st = rand_exp(s2->mu);
		scheduling(s2, tmp, q_system_3);
		tmp = tmp->next;
	}
```

I assume a node enter the next system right after leaving the previous system, so its arrival time in new system equals to departure time in last system. 
The interarrival time is used only in "First system".
If you need to change this, modify mmn.c & multi_mm1.c q_insert() function

```
bool q_insert(queue *q, node *n){
    if(!q | !n){
        return false;
    }
    else{
        node *tmp = malloc(sizeof(node));
        tmp->it = n->it;
        tmp->st = n->st;
	
	/*  Mofify ↓↓↓↓↓  */
        /*  
	 *  Calculate arrival time 
         *  First time entering the system whose arrival time equals to 0  
	 */
        if( n->at == 0 ){
            q->t += n->it;
            tmp->at = q->t;
        }
        /*  Arrive the next system. Use departure time that left the last system as arrival time in this system */
        else{
            tmp->at = n->at ; // node n's arrival updated with its depature time while leaving last system
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
```
Update arraval time with departure time in scheduling() function
```
/* No need to wait */
	if(s->servers[i]->size == 0){
		s->servers[i]->dt = n->at + n->st;
		s->system_time += n->st;
		s->total_service_time += n->st;
		n->at += n->st;	// Update arrival time 
		if_insert = q_insert(s->servers[i], n);
		s->servers[i]->dt = n->at;
		break;
	}
```


	
