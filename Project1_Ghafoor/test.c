#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

typedef struct Queue0Node_temp
{
	struct Queue0Node_temp * next;
	double waitTime;
	double serviceTime;
} Queue0;

typedef struct Queue1Node_temp
{
	struct Queue1Node_temp * next;
	double waitTime;
	double serviceTime;
} Queue1;

Queue0 * Queue0_createNode(double waitTime, double serviceTime)
{
	Queue0 * newNode = malloc(sizeof(Queue0));
	newNode -> next = NULL;
	newNode -> waitTime = waitTime;
	newNode -> serviceTime = serviceTime;
	return (newNode);
}

Queue1 * Queue1_createNode(double waitTime, double serviceTime)
{
	Queue1 * newNode = malloc(sizeof(Queue1));
	newNode -> next = NULL;
	newNode -> waitTime = waitTime;
	newNode -> serviceTime = serviceTime;
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

Queue0 * removeHead0(Queue0 * head)
{
	Queue0 * temp = head -> next;
	head = temp;
	return (head);
}

Queue1 * removeHead1(Queue1 * head)
{
	Queue1 * temp = head -> next;
	head = temp;
	return (head);
}

int Queue0_length(Queue0 * queue)
{
	Queue0 * temp;
	int length = 0;

	if (queue == NULL)
	{
		return (0);
	}

	temp = queue;
	
	while (temp != NULL)
	{
		length ++;
		temp = temp -> next;
	}

	return (length);
}

int Queue1_length(Queue1 * queue)
{
	Queue1 * temp;
	int length = 0;

	if (queue == NULL)
	{
		return (0);
	}

	temp = queue;
	
	while (temp != NULL)
	{
		length ++;
		temp = temp -> next;
	}

	return (length);
}

void updateTime0(Queue0 * head)
{
	while (head != NULL)
	{
		head -> waitTime += .1;
		head = head -> next;
	}
}

void updateTime1(Queue1 * head)
{
	while (head != NULL)
	{
		head -> waitTime += .1;
		head = head -> next;
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
	double totTime = 0.0;
	double remain1 = 0.0;
	double remain2 = 0.0;
	int first0 = 0;
	int first1 = 0;
	Queue0 * head0;
	Queue1 * head1;
	Queue0 * node0;
	Queue1 * node1;
	double totServTime = 0.0;
	int sumLengths0 = 0;
	int sumLengths1 = 0;
	int a;
	double x;
	double r;

	a = rand();
	x = a / RAND_MAX;
	r = (-1/mu)*(log(1-x));
	double arrTime0 = lambda0*(pow(M_E, ((-1)*lambda0*r)));
	double arrTime1 = lambda1*(pow(M_E, ((-1)*lambda1*r)));

	while ((serviced0 + serviced1) < (2*total))
	{
		if (added0 < total)
		{
			remain1 = ((totTime/arrTime0) - floor(totTime/arrTime0));
			remain2 = totTime/arrTime0;
			if ((remain1 >= -0.000001 && remain1 <= 0.000001) || (remain2 >= .99999 && remain2 <= 1.00001))
			{
				if (first0 == 0)
 				{
 					head0 = Queue0_createNode(0, 0);
 					first0 = 1;
 				}
 				else
 				{
 					node0 = Queue0_createNode(0, 0);
					addToQueue0(head0, node0);
				}
				added0 ++;
				sumLengths0 += Queue0_length(head0);
			}
		}

		if (added1 < total)
		{
			remain1 = ((totTime/arrTime1) - floor(totTime/arrTime1));
			remain2 = totTime/arrTime1;
			if ((remain1 >= -0.000001 && remain1 <= 0.000001) || (remain2 >= .99999 && remain2 <= 1.00001))
			{
				if (first1 == 0)
				{
					head1 = Queue1_createNode(0, 0);
					first1 = 1;
				}
				else
				{
					node1 = Queue1_createNode(0, 0);
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
				totWait0 = totWait0 + head0 -> waitTime;
				servicing0 = 1;
				a = rand();
				x = a / RAND_MAX;
				r = (-1/mu)*(log(1-x));
				serviceTime = mu*pow(M_E, ((-1)*mu*r));
				totServTime = totServTime + serviceTime;
				serviceTime = serviceTime + totTime;
			}
			else if (head1 != NULL)
			{
				totWait1 = totWait1 + head1 -> waitTime;
				servicing1 = 1;
				a = rand();
				x = a / RAND_MAX;
				r = (-1/mu)*(log(1-x));
				serviceTime = mu*pow(M_E, ((-1)*mu*r));
				totServTime = totServTime + serviceTime;
				serviceTime = serviceTime + totTime;
			}
		}

		if (totTime >= serviceTime -.00000001 && totTime <= serviceTime + .00000001)
		{
			if (servicing0 == 1)
			{
				head0 = removeHead0(head0);
				if (head0 == NULL)
					first0 = 0;
				serviced0 ++;
				servicing0 = 0;
			}
			else if (servicing1 == 1)
			{
				head1 = removeHead1(head1);
				if (head1 == NULL)
					first1 = 0;
				serviced1 ++;
				servicing1 = 0;
			}
			serviceTime = 0;
		}

		//printf("total totWait0 %f\n", totWait0);
		//printf("total totWait1 %f\n", totWait1);
		//printf("length queue0 %d\n", Queue0_length(head0));
		//printf("length queue1 %d\n", Queue1_length(head1));
		//printf("total 0's %d\n", serviced0);
		//printf("total 1's %d\n", serviced1);
		updateTime0(head0);
		updateTime1(head1);
		totTime = totTime + .1;
	}

	FILE * fp;
	fp = fopen("proj1-a_output.txt", "w+");
	fprintf(fp, "Average wait time for 0 priority: %f seconds\n", totWait0/total);
	fprintf(fp, "Average wait time for 1 priority: %f seconds\n", totWait1/total);
	fprintf(fp, "Average queue length: %f tasks\n", (double)(sumLengths0 + sumLengths1)/(2*total));
	fprintf(fp, "Average CPU utilization: %f %%", 100*totServTime/totTime);
	fclose(fp);

}

void mode2(char * argv[])
{
	double serviceTime = 0.0;
	//double totWait0 = 0.0;
	//double totWait1 = 0.0;
	int serviced0 = 0;
	int serviced1 = 0;
	int servicing0 = 0;
	int servicing1 = 0;
	int added0 = 0;
	int added1 = 0;
	double totTime = 0.0;
	Queue0 * head0 = NULL;
	Queue1 * head1 = NULL;
	Queue0 * node0 = NULL;
	Queue1 * node1 = NULL;
	//double totServTime = 0.0;
	//int sumLengths0 = 0;
	//int sumLengths1 = 0;
	int next = 1;
	int arrival;
	int priority;
	int service;

	char * filename = argv[1];
 	FILE * fp = fopen(filename, "r");
 	char task[1000];
 	char * vals;

 	while (!feof(fp))
 	{
 		if (next == 1)
 		{
 			fgets(task, 1000, fp);
 			printf("hi\n");
 			vals = strtok(task, " ");
 			arrival = atoi(vals);
 			vals = strtok(NULL, " ");
 			priority = atoi(vals);
 			vals = strtok(NULL, " ");
 			service = atoi(vals);
 			next = 0;
 		}

 		if (priority == 0)
 		{
 			if (totTime >= arrival - 0.000001 && totTime <= arrival + 0.000001)
			{
 				if (head0 == NULL)
 				{
 					head0 = Queue0_createNode(0, service, arrival);
 					added0 ++;
 					next = 1;
 				}
 				else
 				{
 					node0 = Queue0_createNode(0, service, arrival);
 					addToQueue1(head1, node1);
 					added0 ++;
 					next = 1;
 				}
 			}
 		}
 		else if (priority == 1)
 		{
 			if (totTime >= arrival - 0.000001 && totTime <= arrival + 0.000001)
 			{
 				if (head1 == NULL)
 				{
 					head1 = Queue1_createNode(0, service, arrival);
 					added1 ++;
 					next = 1;
 				}
 				else
 				{
 					node1 = Queue1_createNode(0, service, arrival);
 					addToQueue1(head1, node1);
 					added1 ++;
 					next = 1;
 				}
 			}
 		}

 		if (servicing0 != 1 && servicing1 != 1)
 		{
 			if (head0 != NULL)
 			{
 				servicing0 = 1;
 				serviceTime = service + totTime;
 			}
 			else if (head1 != NULL)
 			{
 				servicing1 = 1;
 				serviceTime = service + totTime;
 			}
 		}

 		if (serviceTime >= (totTime - 0.000001) && serviceTime <= (totTime + 0.000001))
 		{
 			if (servicing0 == 1)
 			{
				head0 = removeHead0(head0);
				serviced0 ++;
				servicing0 = 0;
 			}
 			else if (servicing1 == 1)
 			{
 				head1 = removeHead1(head1);
				serviced1 ++;
				servicing1 = 0;
 			}
 			serviceTime = 0;
 		}
 		
 		totTime += .1; 		

 	}
}

int mode(int args)
{
	if (args == 5)
	{
		return 1;
	} else if (args == 2)
	{
		return 2;
	} else
	{
		return 0;
	}
}

int main(int argc, char * argv[])
{
	int type = mode(argc);

	if (type == 1)
	{
		mode1(argv);
	} else if (type == 2)
	{
		mode2(argv);
	} else
	{
		; //error
	}
	return 0;
}