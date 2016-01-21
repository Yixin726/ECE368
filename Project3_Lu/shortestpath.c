#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

int minDistance(int distances[], int visited[], int numVerts)
{
  int counter;
  int min = INT_MAX;
  int minIndex = -1;

  for (counter = 0; counter < numVerts; counter ++)
  {
    if (visited[counter] != 1 && distances[counter] < min)
    {
      min = distances[counter];
      minIndex = counter;
    }
  }

  return minIndex;
}

void dijkstra(int** connections, int begin, int numVerts, int end)
{
  int distances[numVerts];
  int visited[numVerts];
  int previous[numVerts];
  previous[begin] = -1;
  int i;
  int counter2;
  int min;
  int j = 0;
  int tempdist;

  for (i = 0; i < numVerts; i++) //set all distances to infinity and all nodes to unvisited
  {
    distances[i] = INT_MAX;
    visited[i] = 0;
  }

  distances[begin] = 0;

  while(visited[end] != 1)
  {
    min = minDistance(distances, visited, numVerts);
    visited[min] = 1;
    //printf("visited vert: %d\n", min);

    for (counter2 = 0; counter2 < numVerts; counter2 ++)
    {
      if (visited[counter2] == 0 && connections[min][counter2] != 0 
        && (distances[min] + connections[min][counter2] < distances[counter2]))
      {
        if (connections[min][counter2] == -1)
        {
          tempdist = 0;
        }
        else
        {
          tempdist = connections[min][counter2];
        }
        distances[counter2] = distances[min] + tempdist;
        previous[counter2] = min;
      }
    }
  }

  /*int counter = 0;
  for (counter = 0; counter < 50; counter ++)
  {
    printf("%d previous: %d\n", counter, previous[counter]);
  }*/

  printf("%d\n", distances[end]);
  int u = end;
  int array[numVerts];

  for (i = 0; i < numVerts; i++)
  {
    array[i] = 0;
  }

  i = 0;
  array[0] = end;

  while (previous[u] != -1)
  {
    i ++;
    array[i] = previous[u];
    u = previous[u];
  }

  for(j = i; j >= 0; j --)
  {
    printf("%d ",array[j]);
  }
  printf("\n");

  return;
}

int distance(int x1, int y1, int x2, int y2)
{
  int value = (int)sqrt(pow(x2-x1,2) + pow(y2-y1,2));
  return value;
}


int main(int Argc, char **Argv)
{
  FILE* fp = fopen(Argv[1], "r");
  FILE* fp2 = fopen(Argv[2], "r");
  int numQuery;
  int numVerts;
  int numEdges;
  int tempNode;
  int tempX;
  int tempY;
  int tempnode1;
  int tempnode2;
  int tempdist;
  int i;

  fscanf(fp2, "%d\n", &numQuery);
  fscanf(fp, "%d %d\n", &numVerts, &numEdges);

  int** locations = (int **)malloc(sizeof(int*) * numVerts);
  int** connections = (int **)malloc(sizeof(int*) * numVerts);

  for (i = 0; i < numVerts; i++) 
  {
    locations[i] = (int*)calloc(2, sizeof(int)); //initialize matrix numVerts down, and 2 across
    connections[i] =(int*)calloc(numVerts, sizeof(int)); //initialize matrix numVerts long, and numVerts down for all adjacent vertices
  }

  for(i = 0; i < numVerts; i++) //add coordinates to each point
  {
    fscanf(fp, "%d %d %d\n", &tempNode, &tempX, &tempY);
    locations[i][0] = tempX;
    locations[i][1] = tempY;
  }

  for(i = 0; i < numEdges; i++) //put neighbor in first available column and corresponding distance in second available column
  {
    fscanf(fp, "%d %d\n", &tempnode1, &tempnode2);
    tempdist = distance(locations[tempnode1][0], locations[tempnode1][1], locations[tempnode2][0], locations[tempnode2][1]);
    if (tempdist == 0)
    {
      tempdist = -1;
    }
    connections[tempnode1][tempnode2] = tempdist;
    connections[tempnode2][tempnode1] = tempdist;
  }

  /*for(i = 0; i < 1000; i++)
  {
    printf("%d: ", i);
    for(j = 0; j < numVerts; j++)
    {
      printf("%d ", j;
    }
    printf("\n");
  }*/

  for(i = 0; i < numQuery; i++) //run dijkstra for each query
  {
    fscanf(fp2, " %d %d\n", &tempnode1, &tempnode2);
    dijkstra(connections, tempnode1, numVerts, tempnode2);
  }

  for (i = 0; i < numVerts; i++) 
  {
    free(locations[i]);
    free(connections[i]);
  }

  free(locations);
  free(connections);
  fclose(fp);
  fclose(fp2);

  return(0);
}
