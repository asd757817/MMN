#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

// Define node 
typedef struct node{
    struct node *next;
    double it;  // interarrival time
    double st;  // service time
	double at;  // arrival time
} node;

// Define queue
typedef struct queue{
    node *head;
	node *tail;
    int size;
	double t;
	double dt;
} queue;

// Define system
typedef struct _system{
	double time; 	// Time variance
	double waiting_time;		// Waiting time in system
	double system_time; // Total time in system 
	double total_service_time;
	queue **servers;
	int n; // N servers
	double lambda;
	double mu;
} _system;

node *node_new(double it, double st);	// Create a new node
queue *q_new(); 						// Create a new queue
_system *system_new(int n, double lambda, double mu);
bool q_insert(queue *q, node *n);		// Insert node into queue
node *q_pop(queue *q);					// Pop a node out 

double rand_exp(double lambda);			// Generate exponential RV with lambda rate
void run_service(_system s);

/* Global variables */
double lambda_1, lamda_2, mu;			// Parameters for generate exp rv
int samples;							// Number of events
int count;
queue *q_system;						// Queue which stores all events

void scheduling(_system *s, node *n, queue *q_next_system);
void show_system();
double get_dt(_system *s);