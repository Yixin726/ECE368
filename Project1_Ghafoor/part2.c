#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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
	struct QueueNode_temp * prev;
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
	newNode -> prev = NULL;
	return (newNode);
}

void addToQueue(Queue* head, Queue* newNode)

{
	if (head->next != NULL)
	{
    	Queue* prevTail = head->prev;
        head->prev = newNode; //point head->prev to new tail element
        prevTail->next = newNode; //point previous tail->next to new tail
        newNode->prev = prevTail;
    }

    else //only one element...
    {
        head->prev = newNode;
        head->next = newNode;
        newNode->prev = head;
    }
    //std::cout << "addToQueue " << queueName << "\n"; //debug

}

void delete(Queue * head)
{
    if (head->prev != NULL && head->next != NULL) // if I'm not the only one...
    {
        Queue* qp = head->prev;
        if (qp->next != NULL)
        {
            qp->next = head->next;
        }
    }
    //std::cout << "\nDeleteQ: " << queueName << "\n";
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
				serviceTime = ceilf((mu*pow(M_E, ((-1)*mu*r)))*10)/10;
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
			delete(head);
		}
		else
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
	double arrTime0 = ceilf((lambda0*(pow(M_E, ((-1)*lambda0*r))))*10)/10;

	srand(time(NULL));
	a = rand();
	x = a / RAND_MAX;
	r = (-1/mu)*(log(1-x));
	double arrTime1 = ceilf((lambda1*(pow(M_E, ((-1)*lambda1*r))))*10)/10;

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
	fp = fopen("proj1-b_output.txt", "w+");
	//fprintf(fp, "Final time: %f\n", totTime);
	fprintf(fp, "Average wait 0: %f\n", totWait0/total);
	fprintf(fp, "Average wait 1: %f\n", totWait1/total);
	fprintf(fp, "Average load balancing factor: %f\n", totBalFac/(2*total));
	fprintf(fp, "Average queue length: %f\n", totLength/totTime);
	fprintf(fp, "Average utilization: %f\n", (totServeTime/totTime/64));
	printf("\nSUCCESS, SEE proj1-a_output.txt\n\n");
	return;
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
	clock_t t;
	t = clock();
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

	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
 
    printf("Simulation took %f seconds to execute \n", time_taken);
	return 0;
}