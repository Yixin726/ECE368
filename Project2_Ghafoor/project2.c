#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*void generateScript() //make random input file - testing purposes
{
	int count;
	int randomNum;
	srand(time(NULL));
	FILE * output = fopen("input.txt", "w+");

	for (count = 0; count < 999999; ++count)
	{
		randomNum = rand();
		if (randomNum % 3 == 0)
		{
			randomNum = -randomNum;
		}

		fprintf(output, "%d\n", randomNum);
	}

	fprintf(output, "%d", 0);

	fclose(output);
	return;
}*/

/*int * makeArray(char * argv[]) //make array from input file - testing purposes
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
}*/

/*int compare(const void *x, const void *y) //compare for qsort - attempt 2
{   
   return ( *(int*)x >= *(int*)y );
}*/

/*void bubble_comb(int list[], int n) //comb sort - attempt 1
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
}*/

void counting_new(int * list, int * Temp, int n) //3rd attempt
{
	int mIndex[4][256] = {{0}};
	int *Dst;
	int *Src;
	int *Tmp;
	int i;
	int j;
	int m;
	int q;
	int u;

    for (i = 0; i < n; i++)
    {         
        u = list[i];
        for(j = 0; j < 4; j++)
        {
            mIndex[j][(int)(u & 0xff)]++; //masking - show only significant bits
            u >>= 8; //bit shift right - divide by 2^8
        }       
    }
    for (j = 0; j < 4; j++)
    {            
        q = 0;
        for (i = 0; i < 256; i++)
        {
            m = mIndex[j][i];
            mIndex[j][i] = q;
            q += m;
        }       
    }
    Dst = Temp;
    Src = list;
    for (j = 0; j < 4; j++)
    {
        for (i = 0; i < n; i++)
        {
        	//printf("got here %d\q", n); 
            u = Src[i];
            m = (int)(u >> (j<<3)) & 0xff;
            Dst[mIndex[j][m]++] = u;
        }
        Tmp = Src;
        Src = Dst;
        Dst = Tmp;
    }
    list = Src;
    return;
}

void sort(int list[], int n) //set up and call counting sort (called by tester)
{
	int counter;
	for (counter = 0; counter < n; counter++)
		list[counter] ^= 0x80000000; //complement sign bits

	int * list2 = malloc(n*sizeof(int));

	counting_new(list, list2, n);

	for (counter = 0; counter < n; counter++)
		list[counter] ^= 0x80000000; //complement sign bits

	free(list2);
	return;
}

/*void printArray(int * Array, int n) //print array to output file - testing purposes
{
	int counter;
	FILE * output = fopen("prj2_output.txt", "w+");


	for (counter = 0; counter < n; counter ++)
	{
		fprintf(output, "%d\n", Array[counter]);
	}

	fclose(output);
}*/

/*int main(int argc, char * argv[]) //make input, make array, start clock, sort, check if sorted - testing purposes
{
	int sorted = 0; 

	generateScript();

	double diff = 0;

	clock_t start_t, end_t;

	int * list = makeArray(argv);

	int * list2 = malloc(n*sizeof(int));

	int counter2;
	for (counter2 = 0; counter2 < 1000000; counter2++)
		list[counter2] ^= 0x80000000;

	start_t = clock();

	sort2(list, list2, 1000000);

	//qsort(list, 1000000, sizeof(int), compare);

	for (counter2 = 0; counter2 < 1000000; counter2++)
		list[counter2] ^= 0x80000000;

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
}*/