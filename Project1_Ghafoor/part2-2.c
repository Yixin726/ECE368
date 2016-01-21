//Dan Suciu & Mitch Bouma
//needs to compile with gcc -Wall -Wshadow -pg -g -lm PRJ1.c -o a.out
//mode 1 takes a bit longer to run

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int maxservice = 0;
int minservice = 0;
int totservice = 0;

int serviced0 = 0;
int serviced1 = 0;
int numAvailable = 64;
double totWait0 = 0.0;
double totWait1 = 0.0;
double maxMu = 100;
double minMu = 0.0;
double totBalFac = 0.0;
int length0 = 0;
int length1 = 0;
int totLength = 0;
double totServeTime = 0.0;

typedef struct Queue0Node_temp
{
	int gone;
	int finished;
	int subtasks;
	int waittime;
	int service;
	int duration[2][1000];

	struct Queue0Node_temp * next;
} Queue0;

typedef struct Queue1Node_temp
{
	int gone;
	int finished;
	int subtasks;
	int waittime;
	int service;
	int duration[2][1000];

	struct Queue1Node_temp * next;
} Queue1;

//-----------------------------------------------------------------------------//

typedef struct QueueNode_temp
{
	double * serviceTimes;
	double waitTime;
	double timeIn;
	double timeOut;
	int numProcesses;
	int serviced;
	int numServiced;
	int servicing;
	int type;
	struct QueueNode_temp * next;
} Queue;

Queue * createNode(double totTime, int type)
{
	Queue * newNode = malloc(sizeof(Queue));
	newNode -> serviceTimes = malloc(32*sizeof(double));
	newNode -> servicing = 0;
	newNode -> type = type;
	newNode -> timeIn = totTime;
	newNode -> timeOut = 0.0;
	newNode -> waitTime = 0.0;
	newNode -> serviced = 0;
	newNode -> numServiced = 0;
	newNode -> numProcesses = (rand() % 32) + 1;
	newNode -> next = NULL;
	return (newNode);
}

void addToQueue(Queue * head, Queue * newNode)
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

Queue * delete(Queue * * head)
{
	if (*head == NULL)
		return NULL;

	Queue * tempNextP;
	tempNextP = (*head)->next;

	return tempNextP;
}

void setServicing(Queue * head, double totTime, double mu)
{
	int counter;
	double a;
	double r;
	double x;
	double serviceTime = 0.0;
	srand(time(NULL));

	while (numAvailable != 0 && head != NULL)
	{
		if (head -> servicing != 1 && head -> numProcesses <= numAvailable)
		{
			for (counter = 0; counter < head -> numProcesses; counter ++)
			{
				a = rand();
				x = a / RAND_MAX;
				r = (-1/mu)*(log(1-x));
				serviceTime = ceilf(((1/mu)*pow(M_E, ((-1)*mu*r)))*10)/10;
				if (serviceTime > minMu)
					minMu = serviceTime;
				if (serviceTime < maxMu)
					maxMu = serviceTime;
				totServeTime += serviceTime;
				serviceTime = serviceTime + totTime;
				head -> serviceTimes[counter] = serviceTime;
			}
			totBalFac += (minMu - maxMu)/(1/mu);
			minMu = 0.0;
			maxMu = 100;
			if (head -> type == 0)
			{
				totWait0 += (totTime - head -> timeIn);
				length0 --;
			}
			else
			{
				totWait1 += (totTime - head -> timeIn);
				length1 --;
			}
			head -> timeOut = totTime;
			head -> servicing = 1;
			numAvailable -= head -> numProcesses;
		}
		head = head -> next;
	}
}

void checkServiceTime(Queue * head, double totTime, int total, int * serviced)
{
	int counter;
	while (head != NULL && *serviced < total)
	{
		if (head -> servicing == 1 && head -> serviced != 1)
		{
			for (counter = 0; counter < head -> numProcesses; counter ++)
			{
				if ((totTime >= (head -> serviceTimes[counter] - .001)) && (totTime <= (head -> serviceTimes[counter] + .001)))
				{
					head -> numServiced ++;
					numAvailable ++;
				}
			}
		}
		if (head -> numServiced == head -> numProcesses && head -> serviced != 1)
		{
			if (head -> type == 0)
			{
				totWait0 += head -> waitTime;
				serviced0 ++;
			}
			else
			{
				totWait1 += head -> waitTime;
				serviced1 ++;
			}
			head -> serviced = 1;
			//head = delete(&head);
		}
		//else
			head = head -> next;
	}
	return;
}

void mode1(char * argv[])
{
	double lambda0 = atof(argv[1]);
	double lambda1 = atof(argv[2]);
	double mu = atof(argv[3]);
	int total = atof(argv[4]);
	double totTime = 0.0;
	int first0 = 0;
	int first1 = 0;
	double remain = 0.0;
	int added0 = 0;
	int added1 = 0;
	Queue * head0;
	Queue * head1;
	Queue * node0;
	Queue * node1;
	double a;
	double x;
	double r;

	srand(time(NULL));
	a = rand();
	x = a / RAND_MAX;
	r = (-1/mu)*(log(1-x));
	double arrTime0 = ceilf(((1/lambda0)*(pow(M_E, ((-1)*lambda0*r))))*10)/10;

	srand(time(NULL));
	a = rand();
	x = a / RAND_MAX;
	r = (-1/mu)*(log(1-x));
	double arrTime1 = ceilf(((1/lambda1)*(pow(M_E, ((-1)*lambda1*r))))*10)/10;

	while((serviced0 + serviced1) < (2*total))
	{
		if (head0 == NULL)
			first0 = 0;
		if (head1 == NULL)
			first1 = 0;

		if (added0 < total)
		{
			remain = ((totTime/arrTime0) - floor(totTime/arrTime0));
			if ((remain >= -0.001 && remain <= 0.001) || (remain >= .999 && remain <= 1.001))
			{
				if (first0 == 0)
 				{
 					head0 = createNode(totTime, 0);
 					first0 = 1;
 				}
 				else
 				{
 					node0 = createNode(totTime, 0);
					addToQueue(head0, node0);
				}
				length0 ++;
				totLength += length0;
				added0 ++;

				a = rand();
				x = a / RAND_MAX;
				r = (-1/mu)*(log(1-x));
				arrTime0 = ceilf(((1/lambda0)*(pow(M_E, ((-1)*lambda0*r))))*10)/10;
			}
		}
		if (added1 < total)
		{
			remain = ((totTime/arrTime1) - floor(totTime/arrTime1));
			if ((remain >= -0.001 && remain <= 0.001) || (remain >= .999 && remain <= 1.001))
			{
				if (first1 == 0)
 				{
 					head1 = createNode(totTime, 1);
 					first1 = 1;
 				}
 				else
 				{
 					node1 = createNode(totTime, 1);
					addToQueue(head1, node1);
				}
				length1 ++;
				totLength += length1;
				added1 ++;

				a = rand();
				x = a / RAND_MAX;
				r = (-1/mu)*(log(1-x));
				arrTime1 = ceilf(((1/lambda1)*(pow(M_E, ((-1)*lambda1*r))))*10)/10;
			}
		}

		if (head0 != NULL && numAvailable != 0)
		{
			setServicing(head0, totTime, mu);
		}
		if (head1 != NULL && numAvailable != 0)
		{
			setServicing(head1, totTime, mu);	
		}

		if (head0 != NULL && serviced0 != total)
		{
			checkServiceTime(head0, totTime, total, &serviced0);
		}
		if (head1 != NULL && serviced1 != total)
		{
			checkServiceTime(head1, totTime, total, &serviced1);
		}

		/*if (head0 != NULL)
		{
			canDelete0(head0);
		}
		if (head1 != NULL)
		{
			canDelete1(head1);
		}*/

		//printf("time: %f\n", totTime);
		//printf("0's serviced: %d\n", serviced0);
		//printf("1's serviced: %d\n", serviced1);
		//printf("available: %d\n", numAvailable);
		//printf("Available: %d\n\n", numAvailable);
		//sleep(5);
		
		totTime += 0.1;
	}

	FILE * fp;
	fp = fopen("proj1_output.txt", "w+");
	//fprintf(fp, "Final time: %f\n", totTime);
	fprintf(fp, "Average wait 0: %f\n", totWait0/total);
	fprintf(fp, "Average wait 1: %f\n", totWait1/total);
	fprintf(fp, "Average queue length: %f\n", totLength/totTime);
	fprintf(fp, "Average utilization: %f\n", (totServeTime/totTime/64));
	fprintf(fp, "Average load balancing factor: %f\n", totBalFac/(2*total));
	printf("\nSUCCESS, SEE proj1_output.txt\n\n");

	fclose(fp);
	return;
}

//-----------------------------------------------------------------------------//
Queue0 * Queue0_createNode(int subtasks, int list[])
{
	int count;
	Queue0 * newNode = malloc(sizeof(Queue0));
	newNode -> gone = 0;
	newNode -> finished = 0;
	newNode -> subtasks = subtasks;
	newNode -> waittime = 0;
	newNode -> service = 0;
	memset(newNode -> duration, 0, sizeof(newNode -> duration[0][0]) * 2 * 1000);
	
	for(count = 0; count < subtasks; ++count)
	{
		newNode -> duration[0][count] = list[count];
	}

	newNode -> next = NULL;
	return (newNode);
}

Queue1 * Queue1_createNode(int subtasks, int list[])
{
	int count;
	Queue1 * newNode = malloc(sizeof(Queue1));
	newNode -> gone = 0;
	newNode -> finished = 0;
	newNode -> subtasks = subtasks;
	newNode -> waittime = 0;
	newNode -> service = 0;
	memset(newNode -> duration, 0, sizeof(newNode -> duration[0][0]) * 2 * 1000);

	for(count = 0; count < subtasks; ++count)
	{
		newNode -> duration[0][count] = list[count];
	}

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

//-----------------------------------------------------------------------------//

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

void mode2(char * argArray[])
{
	int processors = 64;
	int runningprocessors = 0;
	int duration[1000];
	int arrival;
	int priority;
	int subtasks;
	int timing = 0;
	int count = 0;
	int currentLength = 0;
	int sumLength = 0;
	int have0 = 0;
	int have1 = 0;
	int entered = 0;
	int serviced = 0;
	int served0 = 0;
	int served1 = 0;
	int wait0 = 0;
	int wait1 = 0;
	int active = 0;

	Queue0 * head0 = NULL;
	Queue1 * head1 = NULL;
	Queue0 * node0 = NULL;
	Queue1 * node1 = NULL;
	Queue0 * temp0 = NULL;
	Queue1 * temp1 = NULL;

	char * filename = argArray[1];
 	FILE * fp = fopen(filename, "r");

 	char task[1000];
 	char * vals;

 	fgets(task, 1000, fp);
 	vals = strtok(task, " ");
 	arrival = atoi(vals);
 	vals = strtok(NULL, " ");
 	priority = atoi(vals);
 	vals = strtok(NULL, " ");
	subtasks = atoi(vals);

	for (count = 0; count < subtasks; ++count)
	{
		vals = strtok(NULL, " ");
		duration[count] = atoi(vals);
	}

	if (priority == 0)
	{
		head0 = Queue0_createNode(subtasks, duration);
		have0 = 1;
	} else
	{
		head1 = Queue1_createNode(subtasks, duration);
		have1 = 1;
	}
	++entered;
	++currentLength;
	sumLength += currentLength;

	fgets(task, 1000, fp);
 	vals = strtok(task, " ");
 	arrival = atoi(vals);
 	vals = strtok(NULL, " ");
 	priority = atoi(vals);
 	vals = strtok(NULL, " ");
	subtasks = atoi(vals);

	for (count = 0; count < subtasks; ++count)
	{
		vals = strtok(NULL, " ");
		duration[count] = atoi(vals);
	}

	if (priority == 0)
	{
		if (!have0)
		{
			head0 = Queue0_createNode(subtasks, duration);
			have0 = 1;	
		} else
		{
			node0 = Queue0_createNode(subtasks, duration);
			addToQueue0(head0, node0);
		}
	} else
	{
		if (!have1)
		{
			head1 = Queue1_createNode(subtasks, duration);
			have1 = 1;
		} else
		{
			node1 = Queue1_createNode(subtasks, duration);
			addToQueue1(head1, node1);
		}
	}
	++entered;
	++currentLength;
	sumLength += currentLength;

	fgets(task, 1000, fp);
 	vals = strtok(task, " ");
 	arrival = atoi(vals);
 	vals = strtok(NULL, " ");
 	priority = atoi(vals);
 	vals = strtok(NULL, " ");
	subtasks = atoi(vals);

	while ((!feof(fp) || (serviced != entered)) && timing < 100000)
	{
		while (timing == arrival)
		{
			for (count = 0; count < subtasks; ++count)
			{
				vals = strtok(NULL, " ");
				duration[count] = atoi(vals);
			}

			if (priority == 0)
			{
				if (!have0) //checking for a head
				{
					head0 = Queue0_createNode(subtasks, duration);
					have0 = 1;	
				} else
				{
					node0 = Queue0_createNode(subtasks, duration);
					addToQueue0(head0, node0);
				}
				++served0;
			} else
			{
				if (!have1) //checking for a head
				{
					head1 = Queue1_createNode(subtasks, duration);
					have1 = 1;
				} else
				{
					node1 = Queue1_createNode(subtasks, duration);
					addToQueue1(head1, node1);
				}
				++served1;
			}
			++entered;
			++currentLength;

			if (feof(fp))
			{
				arrival = 0; //to stop the while loop above at the end of the file
			}

			if (!feof(fp)) //to avoid seg fault
			{
				fgets(task, 1000, fp);
 				vals = strtok(task, " ");
 				arrival = atoi(vals);
 				vals = strtok(NULL, " ");
 				priority = atoi(vals);
 				vals = strtok(NULL, " ");
				subtasks = atoi(vals);
			}
		}
		sumLength += currentLength;

		temp0 = head0; //throwaway pointers (shallow copy)
		temp1 = head1; //throwaway pointers (shallow copy)

		//scanning and allocating processors for 0's
		if (processors != 0)
		{
			while (temp0 != NULL)
			{
				if (temp0 -> gone == 0) //skip gone nodes
				{
					if ((temp0 -> finished == 0) && (temp0 -> duration[1][0] == 0) && (processors >= temp0 -> subtasks))
					{ //if one stated is 0 ^ then they all will be 0, whole task must start at once
						for (count = 0; count < (temp0 -> subtasks); ++count)
						{
							temp0 -> duration[1][count] = 1; //set the started value
							--processors; //take a processor
						}
					}
					temp0 = temp0 -> next;
				} else 
				{
					temp0 = temp0 -> next;
				}
			}

			//scanning and allocating processors for 1's
			while (temp1 != NULL)
			{
				if (temp1 -> gone == 0)
				{
					if ((temp1 -> finished == 0) && (temp1 -> duration[1][0] == 0) && (processors >= temp1 -> subtasks))
					{
						for (count = 0; count < (temp1 -> subtasks); ++count)
						{
							temp1 -> duration[1][count] = 1;
							--processors;
						}
					}
					temp1 = temp1 -> next;
				} else 
				{
					temp1 = temp1 -> next;
				}
			}
		}

		++timing; //time increment

		temp0 = head0; //throwaway pointers (shallow copy)
		temp1 = head1; //throwaway pointers (shallow copy)

		while (temp0 != NULL)
		{
			for (count = 0, active = 0; !(temp0 -> gone) && count < temp0 -> subtasks; ++count)
			{
				if (temp0 -> duration[1][count] == 1)
				{
					++(temp0 -> service);
					break;
				}
			}
			temp0 = temp0 -> next;
		}

		while (temp1 != NULL)
		{
			for (count = 0; !(temp1 -> gone) && count < temp1 -> subtasks; ++count)
			{
				if (temp1 -> duration[1][count] == 1)
				{
					++(temp1 -> service);
					break;
				}
			}
			temp1 = temp1 -> next;
		}

		temp0 = head0; //throwaway pointers (shallow copy)
		temp1 = head1; //throwaway pointers (shallow copy)

		runningprocessors += 64 - processors;

		//parsing list and decrementing durations
		while (temp0 != NULL)
		{
			if (temp0 -> gone == 0) //if its gone, skip it
			{ 
				for(count = 0; count < (temp0 -> subtasks); ++count)
				{
					if (temp0 -> duration[1][count] == 1) //if it has started...
					{
						--(temp0 -> duration[0][count]); //decrement duration
					} 
					if ((temp0 -> duration[0][count] == 0) && (temp0 -> duration[1][count] == 1))
					{ //if it is decremented to 0 && if its started variable is 1
						temp0 -> finished += 1;
						++processors;
						temp0 -> duration[1][count] = 0;
					} 
					if ((temp0 -> finished == temp0 -> subtasks) && !(temp0 -> gone)) //if youve finished all subtasks
					{
						--currentLength;
						++serviced;
						wait0 += temp0 -> waittime;
						totservice += temp0 -> service;

						if (temp0 -> service > maxservice)
						{
							maxservice = temp0 -> service;
						} else if (temp0 -> service < minservice)
						{
							minservice = temp0 -> service;
						}

						temp0 -> gone = 1;
					}
				} 
				temp0 -> waittime += 1;
				temp0 = temp0 -> next;
			} else 
			{
				temp0 = temp0 -> next;
			}
		}

		//same as above
		while (temp1 != NULL)
		{
			if (temp1 -> gone == 0) //if its gone, skip it
			{ 
				for(count = 0; count < (temp1 -> subtasks); ++count)
				{
					if (temp1 -> duration[1][count] == 1) //if it has started...
					{
						--(temp1 -> duration[0][count]); //decrement duration
					} 
					if ((temp1 -> duration[0][count] == 0) && (temp1 -> duration[1][count] == 1))
					{ //if it is decremented to 0 && if its started variable is 1
						temp1 -> finished += 1;
						++processors;
						temp1 -> duration[1][count] = 0;
					} 
					if ((temp1 -> finished == temp1 -> subtasks) && !(temp1 -> gone)) //if youve finished all subtasks
					{
						--currentLength;
						++serviced;
						wait1 += temp1 -> waittime;
						totservice += temp1 -> service;

						if (temp1 -> service > maxservice)
						{
							maxservice = temp1 -> service;
						} else if (temp1 -> service < minservice)
						{
							minservice = temp1 -> service;
						}

						temp1 -> gone = 1;
					}
				} 
				temp1 -> waittime += 1;
				temp1 = temp1 -> next;
			} else 
			{
				temp1 = temp1 -> next;
			}
		}
	
	}

	//calculations

	double avgwait0 = (double) wait0/served0;
	double avgwait1 = (double) wait1/served1;
	double avgsize =  (double) sumLength/timing;
	double avgCPUutil = (double) runningprocessors/timing;
	double avgservice = (double) totservice/serviced;
	double avgLoadBal = (double) (maxservice - minservice)/avgservice;

	
	FILE * output = fopen("proj1_output.txt", "w+");

	fprintf(output, "Avg wait time for 0: %f\n", avgwait0);
	fprintf(output, "Avg wait time for 1: %f\n", avgwait1);
	fprintf(output, "Avg Queue Length is: %f\n", avgsize);
	fprintf(output, "Avg CPU Utilization is: %f\n", avgCPUutil);
	fprintf(output, "Avg Load Balancing Factor is: %f", avgLoadBal);

	printf("\nSUCCESS, SEE proj1_output.txt\n\n");

	fclose(output);
	fclose(fp);

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
		mode2(argv);
	} else
	{
		; //error
	}
	return 0;
}