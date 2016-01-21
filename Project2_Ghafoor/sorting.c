#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void generateScript()
{
	int count;
	int randomNum;
	srand(time(NULL));
	FILE * output = fopen("input.txt", "w+");

	for (count = 0; count < 999999; ++count)
	{
		randomNum = rand();
		/*if (randomNum % 3 == 0)
		{
			randomNum = -randomNum;
		}*/

		fprintf(output, "%d\n", randomNum);
	}

	fprintf(output, "%d", 0);

	fclose(output);
	return;
}

int * makeArray(char * argv[])
{
	int count;
	int * list = malloc(sizeof(int) * 1000000);
	char temp[1000];
	char * filename = argv[1];
 	FILE * input = fopen(filename, "r");

 	for (count = 0; fgets(temp, 100, input) != NULL; ++count)
 	{
 		list[count] = atoi(temp);
 	}

 	return (list);
}

int compare(const void *x, const void *y){   
   return ( *(int*)x >= *(int*)y );
}

void sort(int list[], int n) //comb sort
{
	int i; 
	int j; 
	int gap;
	int swapped = 1;
	int temp;
	gap = n;

	while (gap > 1 || swapped == 1)
	{
		gap = gap * 10 / 13;
		if (gap == 9 || gap == 10)
		{
			gap = 11;
		}
		if (gap < 1)
		{
			gap = 1;
		}
		swapped = 0; //reset swap flag to 0
		for (i = 0, j = gap; j < n; i++, j++)
		{
			if (list[i] > list[j])
			{
				temp = list[i];
				list[i] = list[j];
				list[j] = temp;
				swapped = 1; //set swap flag to 1
			}
		}
		//if swap flag is 0, stop sorting (already sorted)
	}
}

void sort2(int * list, int * pTemp, int count)
{
	int mIndex[4][256] = {{0}}; // index matrix
	int *pDst, *pSrc, *pTmp;
	int i,j,m,n;
	int u;

    for (i = 0; i < count; i++) // generate histograms
    {         
        u = list[i];
        for(j = 0; j < 4; j++)
        {
            mIndex[j][(int)(u & 0xff)]++;
            u >>= 8;
        }       
    }
    for (j = 0; j < 4; j++) // convert to indices
    {            
        n = 0;
        for (i = 0; i < 256; i++)
        {
            m = mIndex[j][i];
            mIndex[j][i] = n;
            n += m;
        }       
    }
    pDst = pTemp; // radix sort
    pSrc = list;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < count; i++)
        {
        	//printf("hi %d\n", count); 
            u = pSrc[i];
            m = (int)(u >> (j<<3)) & 0xff;
            pDst[mIndex[j][m]++] = u;
        }
        pTmp = pSrc;
        pSrc = pDst;
        pDst = pTmp;
    }
    list = pSrc;
}

void printArray(int * Array, int n)
{
	int counter;
	FILE * output = fopen("prj2_output.txt", "w+");

	for (counter = 0; counter < n; counter ++)
	{
		fprintf(output, "%d\n", Array[counter]);
	}

	fclose(output);
}

int main(int argc, char * argv[])
{
	int sorted = 0; 

	generateScript();

	double diff = 0;

	clock_t start_t, end_t;

	int * list = makeArray(argv);

	int * list2 = list;

	start_t = clock();

	sort2(list, list2, 1000000);

	//qsort(list, 1000000, sizeof(int), compare);

	end_t = clock();

	diff = end_t - start_t;

	printf("Simulation took %f seconds to execute \n", diff/CLOCKS_PER_SEC);

	printArray(list, 1000000);

	int counter;
	for (counter = 0; counter < 999998; counter ++)
	{
		if (list[counter] <= list[counter+1])
			sorted = 1;
		else
		{
			sorted = 0;
			break;
		}
	}
	if (sorted == 1)
		printf("sorted correctly\n");
	else
		printf("did not sort correctly\n");

	return (0);
}