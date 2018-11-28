# MMN_model
M/M/N model with C

## Usage in main.c

1. Create system with N servers. lambda and mu are parameter for exp random variable generator.
```
	_system *s1 = system_new(1, 0.5, 0.6); //N, lambda, mu
	_system *s2 = system_new(2, 0.4, 0.4);
```

2. Create nodes(customers) and store into a queue.
```
  q_system = q_new();
  queue *q_system_2 = q_new();	  
  samples = 500000; // number of nodes
  for(int i=0;i<samples;i++){
      if(q_insert(q_system, node_new(rand_exp(s1->lambda), rand_exp(s1->mu))))
          continue;
      else
          printf("Inset node failed !\n");
  }
```
3. Put nodes into system one by one
```
	node *tmp = q_system->head;
	while(tmp){
		scheduling(s1, tmp, q_system_2);
		tmp = tmp->next;
	}
```

Put c into s. And collect the node which leave the system in q. NULL to drop the node.
```
scheduling(_system *s, node * c, queue *q);
```

4. Connect to next system and update interarrival time and service time.
```
	tmp = q_system_2->head;
	while(tmp){
		tmp->it = rand_exp(s2->lambda);
		tmp->st = rand_exp(s2->mu);
		scheduling(s2, tmp, q_system_3);
		tmp = tmp->next;
	}
```	
 
	
