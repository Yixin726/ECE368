//Dan Suciu & Mitch Bouma
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Queue0Node_temp
{
	int gone;
	int finished;
	int subtasks;
	int duration[2][1000];

	struct Queue0Node_temp * next;
} Queue0;

typedef struct Queue1Node_temp
{
	int gone;
	int finished;
	int subtasks;
	int duration[2][1000];

	struct Queue1Node_temp * next;
} Queue1;

//-----------------------------------------------------------------------------//

Queue0 * Queue0_createNode(int subtasks, int list[])
{
	int count;
	Queue0 * newNode = malloc(sizeof(Queue0));
	newNode -> gone = 0;
	newNode -> finished = 0;
	newNode -> subtasks = subtasks;
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

void mode1(char * argArray[])
{
	return;
}

void mode2(char * argArray[])
{
	int processors = 64;
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
	int wait0 = 0;
	int wait1 = 0;

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
		printf("added a 0\n");
	} else
	{
		head1 = Queue1_createNode(subtasks, duration);
		have1 = 1;
		printf("added a 1\n");
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
		printf("added a 0\n");
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
		printf("added a 1\n");
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

	while (!feof(fp) || (serviced != entered))
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
				printf("added a 0\n");
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
				printf("added a 1\n");
			}
			++entered;
			++currentLength;
			sumLength += currentLength;

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

		temp0 = head0; //throwaway pointers (shallow copy)
		temp1 = head1; //throwaway pointers (shallow copy)

		//scanning and allocating processors for 0's
		while (temp0 != NULL)
		{
			if (temp0 -> gone == 0) //skip gone nodes
			{
				if ((temp0 -> duration[1][0] == 0) && (processors >= temp0 -> subtasks))
				{ //if one stated is 0 ^ then they all will be 0, whole task must start at once
					for (count = 0; count < temp0 -> subtasks; ++count)
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
				if ((temp1 -> duration[1][0] == 0) && (processors >= temp1 -> subtasks))
				{
					for (count = 0; count < temp1 -> subtasks; ++count)
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

		printf("time is %d\n", timing);
		printf("serviced is: %d\n", serviced);
		printf("entered is: %d\n", entered);

		++timing; //time increment

		temp0 = head0; //throwaway pointers (shallow copy)
		temp1 = head1; //throwaway pointers (shallow copy)

		//parsing list and decrementing durations
		for (count = 0; temp0 != NULL; ++count)
		{
			if (temp0 -> gone == 0) //if its gone, skip it
			{
				if ((temp0 -> duration[0][count] == 0) && (temp0 -> duration[1][count] == 1))
				{ //if it is decremented to 0 && if its started variable is 1
					++(temp0 -> finished);
					++processors;
					temp0 -> duration[1][count] = 0;

					if (temp0 -> finished == temp0 -> subtasks) //if youve finished all subtasks
					{
						--currentLength;
						++serviced;
						temp0 -> gone = 1;
						printf("finished a 0\n");
					}
				} else if (temp0 -> duration[1][count] == 1) //if the above was not true (non zero), and its started variable is 1
				{
					--(temp0 -> duration[0][count]); //decrement duration

				}
				if ((count + 1) == temp0 -> subtasks) //if weve looked at all the subtasks, move on to next node
				{
					count = -1; //-1 because after the increment it will be 0
					temp0 = temp0 -> next;
					++wait0;
				}
			} else 
			{
				count = -1; //-1 because after the increment it will be 0
				temp0 = temp0 -> next;
			}
		}

		//same as above
		for (count = 0; temp1 != NULL; ++count)
		{
			if (temp1 -> gone == 0)
			{
				if ((temp1 -> duration[0][count] == 0) && (temp1 -> duration[1][count] == 1))
				{
					++(temp1 -> finished);
					++processors;
					temp1 -> duration[1][count] = 0;

					if (temp1 -> finished == temp1 -> subtasks)
					{
						--currentLength;
						++serviced;
						temp1 -> gone = 1;
						printf("finished a 1\n");
					}
				} else if (temp1 -> duration[1][count] == 1)
				{
					--(temp1 -> duration[0][count]);

				}
				if ((count + 1) == temp1 -> subtasks)
				{
					count = -1;
					temp1 = temp1 -> next;
					++wait1;
				}
			} else 
			{
				count = -1;
				temp1 = temp1 -> next;
			}
		}
	}

	//calculations

	double avgwait0 = (double) wait0/timing;
	double avgwait1 = (double) wait1/timing;
	
	FILE * output = fopen("proj1_output.txt", "w+");

	fprintf(output, "Avg wait time for 0: %f", avgwait0);
	fprintf(output, "Avg wait time for 1: %f", avgwait1);

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