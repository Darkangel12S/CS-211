#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) 
{
  FILE* fp = fopen(argv[1], "r");
  if (!fp) {
    perror("fopen failed");
    return EXIT_FAILURE;
  }
  // declare variable for the array of numbers, number of elements
  int numberOfElements = 0, targetNumbers = 0;

  // read first line and set that int to targetNumbers
  char buf[256];
  char* firstLine = fgets(buf, 256, fp);
  numberOfElements = atoi(firstLine);

  // read the second line and set that to numberOfElements
  char* secondLine = fgets(buf, 256, fp);
  targetNumbers = atoi(secondLine);

  // declare an array to store the total pool of numbers; 
  int numberList[numberOfElements];

  // read the third line and put every number in numberList[] 
  int count = 0, x = 0; 
  while (fscanf(fp, "%s", buf) == 1) // while there is still stuff in the text
  {
    x = atoi(buf); 
    numberList[count] = x; 
    count++; 
  }

  // sort arrayOfInts -- insertion sort
  int temp = 0, j = 0;
  for (int i = 0; i < numberOfElements; i++)
  {
    temp = numberList[i];
    j = i - 1;
    while (j >= 0 && numberList[j] > temp)
    {
      numberList[j+1] = numberList[j];
      j = j - 1;
    }
    numberList[j+1] = temp; 
  }

  // print out target numbers in decending order from the end of the array
  for (int i = numberOfElements-1; i >= numberOfElements-targetNumbers; i--)
  {
    printf("%d ", numberList[i]); 
  }
  fclose(fp);
}
