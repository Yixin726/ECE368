//referenced Professor Lu's ECE264 textbook which provided many compression/decompression functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list
{
  char ascii_val;
  struct list * next;
  struct list * left;
  struct list * right;
} Node;

void destroyTree(Node * node)
{
	if(node == NULL)
		return;
	destroyTree(node -> left);
	destroyTree(node -> right);
	free(node);
}

Node * createNode(char ascii)
{
  Node * newNode = malloc(sizeof(Node));
  newNode -> next = NULL;
  newNode -> left = NULL;
  newNode -> right = NULL;
  newNode -> ascii_val = ascii;
  return newNode;
}

Node * mergeNodes(Node * head)
{
	if (head -> next -> next == NULL)
	{
		Node * newParent = createNode(0);
		newParent -> left = head;
		newParent -> right = head -> next;
		newParent -> left -> next = NULL;
		return newParent;
	}
	head -> next = mergeNodes(head -> next);
	return head;
}

Node * insertNode(Node * head, Node * wn)
{
	if (head == NULL)
	{
		head = wn;
		return head;
	}
	head -> next = insertNode(head -> next, wn);
	return head;
}

int readABit(FILE * fptr, unsigned char * bit, unsigned char * bitOver, unsigned char * currentByte)
{
	int ret = 1;
	if((*bitOver) == 0)
	{
		ret = fread(currentByte, sizeof(unsigned char), 1, fptr); //take in a new byte from huffed file
	}
	unsigned char temp = (*currentByte) >> (7 - (*bitOver)); //shift bits as far right as they can go 
	temp = temp & 0X01; //temp = 1 if the farthest right bit is set
	*bitOver = (*bitOver + 1) % 8; 
	*bit = temp;
	return 0;
}

Node * readInHeader(FILE * infptr)
{
	int done = 0;
	unsigned char bitOver = 0;
	unsigned char currentByte = 0;
	unsigned char onebit = 0;
	Node * head = NULL;
	while (done == 0)
	{
		readABit(infptr, &onebit, &bitOver, &currentByte); //will always be 1 first time
		if (onebit == 1) //leaf node
		{
			int bitcount;
			unsigned char value = 0;
			for(bitcount = 0; bitcount < 8; bitcount++)
			{
	      		value <<= 1; //shift left one
	      		readABit(infptr, &onebit, &bitOver, &currentByte);
	      		value |= onebit;
	    	}
	    	//push a tree node into a list
	    	Node * tn = createNode(value);
	    	head = insertNode(head, tn);
		}
		else
		{
			if (head == NULL)
				printf("ERROR");
			if (head -> next == NULL) //dont building tree tree
			{
				done = 1;
			}
			else
			{
				head = mergeNodes(head);
			}
		}
	}
	return(head);
}

int decompress(char * infile, char * outfile)
{
	FILE * infptr = fopen(infile, "r");
	Node * root = readInHeader(infptr);
	unsigned int numChar = 0;
	fread(&numChar, sizeof(unsigned int), 1, infptr);
	unsigned char newline;

	fread(&newline, sizeof(unsigned char), 1, infptr);
	if(newline != '\n') //make sure newline character is correct
	{
		printf("ERROR! Newline\n");
		return(0);
	}
	unsigned char bitOver = 0;
	unsigned char onebit = 0;
	unsigned char currentByte = 0;
	//open the new file for writing too
	FILE * outfptr = fopen(outfile, "w");
	while(numChar != 0)
	{
		Node * tn = root;
		while (tn -> left != NULL) //not leaf node
		{
			readABit(infptr, &onebit, &bitOver, &currentByte);
			if(onebit == 0)
				tn = tn -> left;
			else
				tn = tn -> right;
		}
		fprintf(outfptr, "%c", tn -> ascii_val);
		numChar--;
	}
	destroyTree(root); //free all nodes of the tree
	fclose(infptr);
	fclose(outfptr);
	printf("File successfully unhuffed!\n");
	return(0);
}

int main(int Argc, char **Argv)
{
	char outfile[100];
	strcpy(outfile, Argv[1]);
	strcat(outfile, ".unhuff");
	decompress(Argv[1], outfile);
	return(0);
}