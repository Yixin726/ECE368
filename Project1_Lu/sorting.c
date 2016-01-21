#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"
#include <math.h>


long *Load_File (char *Filename, int *Size)
{
	FILE * file;
	file = fopen(Filename, "r");
	long holder;
	fscanf(file, "%ld", &holder);
	*Size = holder;
	long * array = malloc((*Size) * sizeof(long));

	int i = 0;
	while(fscanf(file, "%ld", &holder) == 1)
	{
		array[i] = holder;
		i ++;
	}

	fclose(file);
	return array;
}

int Save_File (char *Filename, long *Array, int Size)
{
	FILE * file;
	file = fopen(Filename, "w");
	int num_print = 0;

	int counter = 0;
	while (counter <= Size)
	{
		if (counter == 0)
		{
			fprintf(file, "%d\n", Size);
		}
		else
		{
			fprintf(file, "%ld\n", Array[counter - 1]);
			num_print ++;
		}
		counter ++;
	}

	fclose(file);
	return num_print;
}

int cmpfunc (const void * a, const void * b)
{
  return ( *(int*)b - *(int*)a );
}

int cmpfunc2 (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int check_sorted(long *Array, int n)
{
	int counter;
	int sorted = 1;
	for (counter = 0; counter < n-1; counter ++)
	{
		if (Array[counter] > Array[counter + 1])
		{
			sorted = 0;
			return sorted;
		}
	}
	return sorted;
}

void Shell_Insertion_Sort (long *Array, int Size, double *NComp, double *NMove)
{
	int max = Size;
	int counter_q = 0;
	int counter_p = 0;
	int num_gaps = 0;
	long temp = 0;

	//loop through all possibilities of 3^p * 2*q less than size 
	while (pow(3,counter_p) < max)
	{
		while(pow(2,counter_q) < max)
		{
			temp = pow(2,counter_q) * pow(3, counter_p);
			if (temp < max)
			{
				num_gaps ++; //find needed size of array
			}
			counter_q ++;
		}
		counter_q = 0;
		counter_p ++;
	}

	long * gap_array = malloc(num_gaps * sizeof(long));
	counter_p = 0;
	counter_q = 0;
	int place = 0;

	//do same as above, but now put elements into allocated array
	while (pow(3,counter_p) < max)
	{
		while(pow(2,counter_q) < max)
		{
			temp = pow(2,counter_q) * pow(3, counter_p);
			if (temp < max)
			{
				gap_array[place] = temp;
				place ++;
			}
			counter_q ++;
		}
		counter_q = 0;
		counter_p ++;
	}

	qsort(gap_array, num_gaps, sizeof(long), cmpfunc);

	int x; 
	int i;
	int j;
	int gap;

	//base shell sort seen on wikipedia
  	for(x = 0; x < num_gaps; x++)
    {
  		gap = (int) gap_array[x];
        for(i = gap; i < Size; i++)
		{
	    	temp = Array[i];
	    	*NMove += 1;
	    	*NComp += 1;
			for(j = i; j >= gap && temp < Array[j - gap]; j -= gap )
			{
				Array[j] = Array[j - gap];
	    		*NMove +=1;
    		}
	    	Array[j] = temp;
	    	*NMove = *NMove += 1;
		}
    }

 	free(gap_array);

 	//check if sorted
	/*int sorted = check_sorted(Array, Size);
 	if (sorted == 1)
 		printf("congrats!\n");
 	else
 		printf("not correct...\n");*/
}

void Improved_Bubble_Sort(long *Array, int Size, double *NComp, double *NMove)
{
	int gap = Size;
	int num_gaps = 0;
	while (gap > 1)
	{
		gap = floor(gap/(1.3));
		if (gap == 9 || gap == 10)
		{
			gap = 11;
		}
		if (gap < 1)
		{
			gap = 1;
		}

		num_gaps ++;
	}

	gap = Size;
	int counter = 0;
	int * gap_array = malloc(num_gaps * sizeof(int));
	while (counter < num_gaps)
	{
		gap = floor(gap/(1.3));
		if (gap == 9 || gap == 10)
		{
			gap = 11;
		}
		if (gap < 1)
		{
			gap = 1;
		}
		gap_array[counter] = gap;
		counter ++;
	}

	qsort(gap_array, num_gaps, sizeof(int), cmpfunc);

	int swapped = 1;
	int i;
	int j;
	int gap_counter = 0;
	gap = gap_array[gap_counter];
	int temp;

	//loop through all gaps, bubble sort one pass for each gap, finish with gap of 1. end if gap is at 1, but no swaps were made
	while (gap > 1 || swapped == 1)
	{
		if (gap_counter >= num_gaps-1) //make sure you keep using gap of 1 until fully sorted
			gap = 1;
		else
			gap = gap_array[gap_counter];
		swapped = 0;
		for(i = 0, j = gap; j < Size; i++, j++)
		{
			*NComp += 1;
	  		if(Array[i] > Array[j])
	    	{
	    		*NMove += 3; //3 moves between temp value and 2 array values
	        	temp = Array[i];
	        	Array[i] = Array[j];
	        	Array[j] = temp;
	        	swapped = 1;
	    	}
		}
		gap_counter ++;
	}

	free(gap_array);

	//check if sorted
	/*int sorted = check_sorted(Array, Size);
 	if (sorted == 1)
 		printf("congrats!\n");
 	else
 		printf("not correct...\n");*/
}

void Save_Seq1 (char *Filename, int N)
{
	int max = N;
	int counter_q = 0;
	int counter_p = 0;
	int num_gaps = 0;
	long temp = 0;

	while (pow(3,counter_p) < max)
	{
		while(pow(2,counter_q) < max)
		{
			temp = pow(2,counter_q) * pow(3, counter_p);
			if (temp < max)
			{
				num_gaps ++;
			}
			counter_q ++;
		}
		counter_q = 0;
		counter_p ++;
	}

	long * gap_array = malloc(num_gaps * sizeof(long));
	counter_p = 0;
	counter_q = 0;
	int i = 0;

	while (pow(3,counter_p) < max)
	{
		while(pow(2,counter_q) < max)
		{
			temp = pow(2,counter_q) * pow(3, counter_p);
			if (temp < max)
			{
				gap_array[i] = temp;
				i ++;
			}
			counter_q ++;
		}
		counter_q = 0;
		counter_p ++;
	}

	qsort(gap_array, num_gaps, sizeof(long), cmpfunc2);

	FILE * file;
	file = fopen(Filename, "w");

	int counter = 0;
	while (counter < num_gaps)
	{
		fprintf(file, "%ld\n", gap_array[counter]);
		counter ++;
	}

	fclose(file);
}

void Save_Seq2 (char *Filename, int N)
{
	int gap = N;
	int num_gaps = 0;
	while (gap > 1)
	{
		gap = floor(gap/(1.3));
		if (gap == 9 || gap == 10)
		{
			gap = 11;
		}
		if (gap < 1)
		{
			gap = 1;
		}
		num_gaps ++;
	}

	gap = N;
	int counter = 0;
	long * gap_array = malloc(num_gaps * sizeof(long));
	while (counter < num_gaps)
	{
		gap = floor(gap/(1.3));
		if (gap == 9 || gap == 10)
		{
			gap = 11;
		}
		if (gap < 1)
		{
			gap = 1;
		}
		gap_array[counter] = gap;
		counter ++;
	}

	qsort(gap_array, num_gaps, sizeof(long), cmpfunc2);

	FILE * file;
	int last = 0;
	file = fopen(Filename, "w");

	int counter2 = 0;
	while (counter2 < num_gaps)
	{
		if (gap_array[counter2] != last)
			fprintf(file, "%ld\n", gap_array[counter2]);
		last = gap_array[counter2];
		counter2 ++;
	}

	fclose(file);
}