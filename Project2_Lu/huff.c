//referenced Professor Lu's ECE264 textbook which provided many compression/decompression functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list
{
  int frequency;
  char ascii_val;
  struct list * next;
  struct list * left;
  struct list * right;
} Node;

/*void destroyTree(Node * node)
{
  if(node == NULL)
    return;
  destroyTree(node -> left);
  destroyTree(node -> right);
  free(node);
}*/

int countFreqs(char * file, int * array)
{
  int ch;
  int totalNum = 0;
  FILE * fp = fopen(file, "r");
  for (;;)
  {
    ch = fgetc(fp);
    if (ch == EOF)
    {
      break;
    }
    array[ch]++;
    totalNum++;
  }
  fclose(fp);
  return(totalNum);
}

Node * createNode(char ascii, int frequency)
{
  Node * newNode = malloc(sizeof(Node));
  newNode -> next = NULL;
  newNode -> left = NULL;
  newNode -> right = NULL;
  newNode -> frequency = frequency;
  newNode -> ascii_val = ascii;
  return newNode;
}

Node * insertInOrder(Node * head, Node * newNode)
{
  if (head == NULL)
  {
    head = newNode;
    return head;
  }
  if (head -> frequency < newNode -> frequency)
  {
    head -> next = insertInOrder(head -> next, newNode);
  }
  else
  {
    newNode -> next = head;
    return newNode;
  }
  return head;
}

Node * buildList(int * array)
{
  int counter = 0;
  Node * head = NULL;
  for (counter= 0; counter < 256; counter++)
  {
    if (array[counter] > 0)
    {
      Node * newNode = createNode(counter, array[counter]);
      head = insertInOrder(head, newNode);
    }
  }
  Node * newNode2 = createNode('\0', 1);
  head = insertInOrder(head, newNode2);
  return head;
}

Node * buildTree(Node * head)
{
  while(head -> next != NULL)
  {
    Node * newNode = createNode(0, ((head -> frequency) + (head -> next -> frequency)));
    newNode -> left = head;
    newNode -> right = head -> next;
    head = head -> next -> next;
    head = insertInOrder(head, newNode);
  }
  return head;
}

void numLeavesHelper(Node * root, int * num)
{
  if (root == NULL)
  {
    return;
  }
  Node * left = root -> left;
  Node * right = root -> right;
  if (left == NULL && right == NULL) //if left and right null, then at leaf
  {
    (*num)++; //add 1 to total leaves
    return;
  }
  numLeavesHelper(left, num); //traverse left side
  numLeavesHelper(right, num); //traverse right side
}

int numLeaves(Node * root)
{
  int num = 0;
  numLeavesHelper(root, &num);
  return num;
}

int treeHeightHelper(Node * root, int height)
{
  if (root == NULL) //end of that sub tree
  {
    return height; 
  }
  int left_height = treeHeightHelper(root -> left, height + 1); //go left one and add 1 to height
  int right_height = treeHeightHelper(root -> right, height + 1); //go right one and add 1 to height

  if (left_height < right_height) //pick the highest height
  {
    return right_height;
  }
  else
  {
    return left_height;
  }
}

int treeHeight(Node * root)
{
  return treeHeightHelper(root, 0);
}

void buildCodeBookHelper(Node * root, int ** codebook, int * row, int col)
{
  if (root == NULL)
  {
    return;
  }
  Node * left = root -> left;
  Node * right = root -> right;
  if (left == NULL && right == NULL) //at leaf node, so set first column place to this node's ascii
  {
    codebook[*row][0] = root -> ascii_val;
    (*row)++; //done writing the sequence for this character
    return;
  }
  if(left != NULL)
  {
    int numRow = numLeaves(left); //number of characters in the left side of the tree
    int ind;
    for(ind = * row; ind < (*row) + numRow; ind++) //start from last row filled and put a 0 all the way down for the column you're in
    {
     codebook[ind][col] = 0;
    }
    buildCodeBookHelper(left, codebook, row, col + 1); //traverse left side and use next column over
  }
  if(right != NULL)
  {
    int numRow = numLeaves(right); //number of characters in the left side of the tree
    int ind;
    for(ind = *row; ind < (*row) + numRow; ind++) //start from last row filled and put a 1 all the way down for the column you're in
    {
      codebook[ind][col] = 1;
    }
    buildCodeBookHelper(right, codebook, row, col + 1); //traverse right side and use next column over
  }
}

void buildCodeBook(Node * root, int ** codebook)
{
  int row = 0;
  buildCodeBookHelper(root, codebook, &row, 1); //use 1 for col since col 0 is the characters
}

int writeABit(FILE * fptr, unsigned char bit, unsigned char * bitOver, unsigned char * currentByte)
{
  if (*bitOver == 0)
  {
    *currentByte = 0; //reset currentByte if bitOver got to farthest right place
  }
  unsigned char temp = bit << (7 - (*bitOver)); //move incoming bit to farthest available left spot
  *currentByte |= temp; //keep original bits in currentByte and add whatever was set in the new spots   
  int value = 0;
  if ((*bitOver) == 7) //ready to print packed byte
  {
    int ret;
    ret = fwrite(currentByte, sizeof(unsigned char), 1, fptr); //print the byte to file
    if(ret == 1)
    {
     value = 1;
    }
    else
    {
      value = -1;
    }
  }
  *bitOver = (*bitOver + 1) % 8; //reset to 0 once it gets to 7 aka farthest right spot
  return value;
}

void char2bits(FILE * outfptr, int ch, unsigned char * bitOver, unsigned char * currentByte)
{
  unsigned char mask = 0x80; //1000 0000  
  while(mask > 0)
  {
    writeABit(outfptr, (ch & mask) == mask, bitOver, currentByte); //if 1 in same place as mask, print 1. Else, print 0
    mask >>= 1; //move the 1 in mask to the right
  }
}

void headerHelper(Node * root, FILE * outfptr, unsigned char * bitOver, unsigned char * currentByte)
{
  if (root == NULL)
  {
    return;
  }
  Node * left = root -> left;
  Node * right = root -> right;
  if (left == NULL && right == NULL) //leaf node
  {
    writeABit(outfptr, 1, bitOver, currentByte); //1 specifies a leaf node
    char2bits(outfptr, root -> ascii_val, bitOver, currentByte); //now write the bits for the character at that leaf
    return;
  }
  headerHelper(left, outfptr, bitOver, currentByte); //repeat with left subtree
  headerHelper(right, outfptr, bitOver, currentByte);
  writeABit(outfptr, 0, bitOver, currentByte); //0 specifies non leaf node
}

int padding(FILE * fptr, unsigned char * bitOver, unsigned char * currentByte)
{
  int rtv = 0;
  while((*bitOver) != 0)
  {
    rtv = writeABit(fptr, 0, bitOver, currentByte);
    if(rtv == -1)
    {
      return -1;
    }
  }
 return rtv;
}

void header(Node * root, unsigned int numChar, char * Filename)
{
  FILE * outfptr = fopen(Filename, "w");
  if(outfptr == NULL)
  {
    return;
  }
  unsigned char bitOver = 0;
  unsigned char currentByte = 0;
  headerHelper(root, outfptr, &bitOver, &currentByte);
  writeABit(outfptr, 0, &bitOver, &currentByte);
  padding(outfptr, &bitOver, &currentByte);
  fwrite(&numChar, sizeof(unsigned int), 1, outfptr);
  unsigned char newline = '\n';
  fwrite(&newline, sizeof(unsigned char), 1, outfptr);
  fclose(outfptr);
}

int compress(char * infile, char * outfile, int ** codebook, int * rowKey)
{
  FILE * infptr = fopen(infile, "r");
  FILE * outfptr = fopen(outfile, "a");
  unsigned char bitOver = 0;
  unsigned char currentByte = 0;
  while (!feof(infptr)) //while not at end of input file
  {
    int onechar = fgetc(infptr); //get a character
    if(onechar != EOF)
    {
      int ind = rowKey[onechar]; //get row number for that char in the codebook
      int ind2 = 1;
      while (codebook[ind][ind2] != -1) //keep getting the bit sequence from that row
      {
        writeABit(outfptr, (codebook[ind][ind2] == 1), &bitOver, &currentByte); //write what is in that row/column
        ind2++;
      }
    }
  }
  padding(outfptr, &bitOver, &currentByte);
  fclose(infptr);
  fclose(outfptr);
  printf("Compressed file with no errors!\n");
  return(0);
}

int main(int Argc, char **Argv)
{
  Node * root = NULL;
  char file[100];
  strcpy(file, Argv[1]);
  strcat(file, ".huff");
  int ascii_array[256] = {0}; 

  int numChar;
  numChar = countFreqs(Argv[1], ascii_array);
  Node * list = buildList(ascii_array);
  /*Node * listTest = list;
  while (listTest != NULL)
  {
    printf("Character: %c, Frequency: %d\n", listTest -> ascii_val, listTest -> frequency);
    listTest = listTest -> next;
  }*/
  root = buildTree(list);
  int numRow = numLeaves(root);
  int numCol = treeHeight(root);
  numCol++;
  int ** codebook = malloc(sizeof(int*) * numRow); //make multi-dimensional array for characters and their encodings
  int row;
  for (row = 0; row < numRow; row++)
  {
    codebook[row] = malloc(sizeof(int) * numCol);
    int col;
    for(col = 0; col < numCol; col++) //preset all spots to -1 as place holders
    {
      codebook[row][col] = -1;
    }
  }

  buildCodeBook(root, codebook); //fill in characters in first column and encodings in following columns
  int rowKey[256];
  int ind;
  for(ind = 0; ind < 256; ind++)
  {
    rowKey[ind] = -1;
    int ind2;
    for(ind2 = 0; ind2 < numRow; ind2 ++)
    {
      if(codebook[ind2][0] == ind)
      {
        rowKey[ind] = ind2; //tells which row in codebook you can find a specific ascii character
      }
    }
  }

  /*int row1;
  int col1;

  for (row1 = 0; row1 < numRow; row1++)
  {
    for(col1 = 0; col1 < numCol; col1++)
    {
      if (col1 == 0)
      {
        printf("%c  ", codebook[row1][0]);
      }
      else
      {
        printf("%d  ", codebook[row1][col1]);
      }
    }
    printf("\n");
  }*/

  header(root, numChar, file); 
  compress(Argv[1], file, codebook, rowKey); 

  //destroyTree(root);

  return 0;
}