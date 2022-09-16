#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include<math.h>

//Function declerations
bool is_prime(int n);

int main(int argc, char* argv[]) 
{
  int number = atoi(argv[1]);
  int prime1 = 0, prime2 = 0, prime3 = 0;
  //get an array of all the prime numbers less than number
  int numberOfPrimes = 0, primeIncr = 0;
  for (int i = 2; i < number; i++) // finds the # of prime #s less than number
  { 
    if (is_prime(i)) { numberOfPrimes++; }
  }
  int arrayOfPrimes[numberOfPrimes];
  for (int i = 2; i < number; i++) // puts all the primes less than number in an array
  {
    if (is_prime(i)) 
    {
      arrayOfPrimes[primeIncr] = i;
      primeIncr++;
    }
  }
  // triple for loop
  for (int i = 0; i < primeIncr; i++)
  {
    for (int j = 0; j < primeIncr; j++)
    {
      for (int l = 0; l < primeIncr; l++)
      {
        // if the sum of those prime numbers = target number
        int sum = arrayOfPrimes[i] + arrayOfPrimes[j] + arrayOfPrimes[l];
        if (sum == number) 
        {
          // initialize all those numbers as the three primes
          prime1 = arrayOfPrimes[i];
          prime2 = arrayOfPrimes[j]; 
          prime3 = arrayOfPrimes[l];
          goto end; // break out of loop
        }
        else { sum = 0; }
        // if the sum of those prime numbers = target number
      }
    }
  }
  end:
  // not in acending order
  //for (int i = 0; i < primeIncr; i++) { printf("%d - ", arrayOfPrimes[i]); }
  printf("%d = %d + %d + %d", number, prime1, prime2, prime3);

}

bool is_prime(int n)
{
  if (n < 2) { return false; }
  for (int i = 2; i <= sqrt(n); i++)
  {
    if ((n % i) == 0) { return false; }
  }
  return true; 
}
