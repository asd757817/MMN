# M/M/N_model and parallel multiple M/M/1 model
* mmn_* for M/M/N
* multi_mm1 for parallel multiple M/M/1

## Usage, modify mmn_main.c and multi_main.c to generate your model

1. Create system with N servers. lambda and mu are parameter for exp random variable generator.
```
	_system *s1 = system_new(1, 0.5, 0.6); //N, lambda, mu
	_system *s2 = system_new(2, 0.4, 0.4);
```

2. Create nodes(customers or events) and store into a queue.
```
/* Create a queue to store nodes */
  q_system = q_new();	
  queue *q_system_2 = q_new();	  )
  
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

4. Connect to next system and update interarrival time and service time.
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
The interarrival 
 
	
