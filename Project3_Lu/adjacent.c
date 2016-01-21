#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int Argc, char **Argv)
{
	FILE * file;
	char * filename = Argv[1];
	file = fopen(filename, "r");
	int totVerts;
	int totEdges;
	int holder;
	int prev;
	int test;
	int first;
	int second;

	fscanf(file, "%d %d\n", &totVerts, &totEdges);
	fscanf(file, "%d", &prev);
	fscanf(file, "%d", &test);
	fscanf(file, "%d\n", &test);
	fscanf(file, "%d", &holder);

	int ** adjArray = malloc(sizeof(int*) * totVerts);
	int row;

	for (row = 0; row < totVerts; row++)
	{
		adjArray[row] = malloc(sizeof(int) * 1000);
	    int col;
	    for(col = 0; col < 1000; col++) //preset all spots to -1 as place holders
	    {
	      adjArray[row][col] = -1;
	    }
	}

	int adjCount[totVerts];
	int a;
	for (a = 0; a < totVerts; a ++)
	{
		adjCount[a] = 0;
	}

	while (prev < holder) //runs until you are at first adjacency entry
	{
		prev = holder;
		fscanf(file, "%d", &test);
		fscanf(file, "%d\n", &test);
		fscanf(file, "%d\n", &holder);
	}

	first = holder;
	fscanf(file, "%d\n", &second);
	int move = adjCount[first];
	adjArray[first][move] = second;
	adjCount[first] ++;
	move = adjCount[second];
	adjArray[second][move] = first;
	adjCount[second] ++;

	while ((fscanf(file, "%d", &first) == 1) && (fscanf(file, "%d\n", &second) == 1))
	{
		move = adjCount[first];
		adjArray[first][move] = second;
		adjCount[first] ++;
		move = adjCount[second];
		adjArray[second][move] = first;
		adjCount[second] ++;
	}

	int counter;
	int counter2 = 0;
	for (counter = 0; counter < totVerts; counter ++)
	{
		if (adjArray[counter][counter2] != -1)
		{
			printf("%d: ", counter);
			while (adjArray[counter][counter2] != -1)
			{
				printf("%d ", adjArray[counter][counter2]);
				counter2 ++;
			}
			printf("\n");
			counter2 = 0;
		}
	}

	fclose(file);

	return 0;
}