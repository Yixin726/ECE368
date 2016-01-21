#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct Queue0Node_temp
{
	double service;
	double waittime;
	int processes;
	struct Queue0Node_temp * next;
} Queue0;

typedef struct Queue1Node_temp
{
	double service;
	double waittime;
	int processes;
	struct Queue1Node_temp * next;
} Queue1;

Queue0 * Queue0_createNode(double servicetime)
{
	Queue0 * newNode = malloc(sizeof(Queue0));
	newNode -> service = servicetime;
	newNode -> waittime = 0.0;
	newNode -> processes = 0;
	newNode -> next = NULL;
	return (newNode);
}

Queue1 * Queue1_createNode(double servicetime)
{
	Queue1 * newNode = malloc(sizeof(Queue1));
	newNode -> service = servicetime;
	newNode -> waittime = 0.0;
	newNode -> processes = 0;
	newNode -> next = NULL;
	return (newNode);
}

void addToQueue0(Queue0 * head, Queue0 * newNode)
{
	if (head == NULL)
	{
		head = newNode;
	}

	while (head -> next != NULL)
	{
		head = head -> next;
	}

	head -> next = newNode;
}

void addToQueue1(Queue1 * head, Queue1 * newNode)
{
	if (head == NULL)
	{
		head = newNode;
	}

	while (head -> next != NULL)
	{
		head = head -> next;
	}

	head -> next = newNode;
}

int Queue0_length(Queue0 * head)
{
	Queue0 * temp;
	int length;

	if (head == NULL)
	{
		return (0);
	}

	temp = head;
	
	for (length = 0; temp != NULL; length++)
	{
		temp = temp -> next;
	}

	return (length);
}

int Queue1_length(Queue1 * head)
{
	Queue1 * temp;
	int length;

	if (head == NULL)
	{
		return (0);
	}

	temp = head;
	
	for (length = 0; temp != NULL; length++)
	{
		temp = temp -> next;
	}

	return (length);
}

Queue0 * removeHead0(Queue0 * head)
{
	if (head != NULL)
	{
		Queue0 * temp = head -> next;
		head = temp;
		return (head);
	}
	return (head);
}

Queue1 * removeHead1(Queue1 * head)
{
	if (head != NULL)
	{
		Queue1 * temp = head -> next;
		head = temp;
		return (head);
	}
	return (head);
}

void updateTime0(Queue0 * head)
{
	while (head != NULL)
	{
		head -> waittime += .1;
		head = head -> next;
	}
}

void updateTime1(Queue1 * head)
{
	while (head != NULL)
	{
		head -> waittime += .1;
		head = head -> next;
	}
}

int mode(int args)
{
	if (args == 5)
	{
		return 1;
	} 
	else if (args == 2)
	{
		return 2;
	} 
	else 
	{
		return 0;
	}
}

void mode1(char * argv[])
{
	double lambda0 = atof(argv[1]);
	double lambda1 = atof(argv[2]);
	double mu = atof(argv[3]);
	int total = atof(argv[4]);
	double serviceTime = 0.0;
	double totWait0 = 0.0;
	double totWait1 = 0.0;
	int serviced0 = 0;
	int serviced1 = 0;
	int servicing0 = 0;
	int servicing1 = 0;
	int added0 = 0;
	int added1 = 0;
	int servers = 64;
	double totTime = 0.0;
	double remain1 = 0.0;
	int first0 = 0;
	int first1 = 0;
	Queue0 * head0;
	Queue1 * head1;
	Queue0 * node0;
	Queue1 * node1;
	double totServTime = 0.0;
	int sumLengths0 = 0;
	int sumLengths1 = 0;
	double a;
	double x;
	double r;

	srand(time(NULL));
	a = rand();
	x = a / RAND_MAX;
	r = (-1/mu)*(log(1-x));
	double arrTime0 = ceilf((lambda0*(pow(M_E, ((-1)*lambda0*r))))*10)/10;

	srand(time(NULL));
	a = rand();
	x = a / RAND_MAX;
	r = (-1/mu)*(log(1-x));
	double arrTime1 = ceilf((lambda1*(pow(M_E, ((-1)*lambda1*r))))*10)/10;
	printf("arr time 0: %f\n", arrTime0);
	printf("arr time 1: %f\n", arrTime1);
	sleep(2);

	while ((serviced0 + serviced1) < (2*total))
	{
		if (added0 < total)
		{
			remain1 = ((totTime/arrTime0) - floor(totTime/arrTime0));
			if ((remain1 >= -0.001 && remain1 <= 0.001) || (remain1 >= .999 && remain1 <= 1.001))
			{
				if (first0 == 0)
 				{
 					head0 = Queue0_createNode(0);
 					first0 = 1;
 				}
 				else
 				{
 					node0 = Queue0_createNode(0);
					addToQueue0(head0, node0);
				}
				added0 ++;
				sumLengths0 += Queue0_length(head0);
			}
		}

		if (added1 < total)
		{
			remain1 = ((totTime/arrTime1) - floor(totTime/arrTime1));
			if ((remain1 >= -0.001 && remain1 <= 0.001) || (remain1 >= .999 && remain1 <= 1.001))
			{
				if (first1 == 0)
				{
					head1 = Queue1_createNode(0);
					first1 = 1;
				}
				else
				{
					node1 = Queue1_createNode(0);
					addToQueue1(head1, node1);
				}
				added1 ++;
				sumLengths1 += Queue1_length(head1);
			}
		}

		if (servicing0 != 1 && servicing1 != 1)
		{
			if (head0 != NULL)
			{
				totWait0 = totWait0 + head0 -> waittime;
				servicing0 = 1;
				srand(time(NULL));
				a = rand();
				x = a / RAND_MAX;
				r = (-1/mu)*(log(1-x));
				serviceTime = ceilf((mu*pow(M_E, ((-1)*mu*r)))*10)/10;
				totServTime = totServTime + serviceTime;
				serviceTime = serviceTime + totTime;
			}
			else if (head1 != NULL)
			{
				totWait1 = totWait1 + head1 -> waittime;
				servicing1 = 1;
				srand(time(NULL));
				a = rand();
				x = a / RAND_MAX;
				r = (-1/mu)*(log(1-x));
				serviceTime = roundf((mu*pow(M_E, ((-1)*mu*r)))*10)/10;
				totServTime = totServTime + serviceTime;
				serviceTime = serviceTime + totTime;
			}
		}

		if (totTime >= serviceTime - 0.001 && totTime <= serviceTime + 0.001)
		{
			if (servicing0 == 1)
			{
				head0 = removeHead0(head0);
				if (head0 == NULL)
					first0 = 0;
				serviced0 ++;
				printf("time 0: %f\n", totTime);
				servicing0 = 0;
			}
			else if (servicing1 == 1)
			{
				head1 = removeHead1(head1);
				if (head1 == NULL)
					first1 = 0;
				serviced1 ++;
				printf("time 1: %f\n", totTime);
				servicing1 = 0;
			}
			serviceTime = 0;
		}

		updateTime0(head0);
		updateTime1(head1);
		totTime += .1;
	}

	FILE * fp;
	fp = fopen("proj1-a_output.txt", "w+");
	fprintf(fp, "Average wait time for 0 priority: %f seconds\n", totWait0/total);
	fprintf(fp, "Average wait time for 1 priority: %f seconds\n", totWait1/total);
	fprintf(fp, "Average queue length: %f tasks\n", (double)(sumLengths0 + sumLengths1)/(2*total));
	fprintf(fp, "Average CPU utilization: %f %%", 100*totServTime/totTime);
	fclose(fp);

	printf("\nSUCCESS, SEE proj1-a_output.txt\n\n");
	return;
}

int main(int argc, char * argv[])
{
	int type = mode(argc);

	if (type == 1)
	{
		mode1(argv);
	} else if (type == 2)
	{
		//mode2(argv);
	} else
	{
		; //error
	}
	return 0;
}