#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define FIRST 65 // first char in ascii table
#define LAST 90 // last char in ascii table
#define MIN 0.06 // min max values for the IC
#define MAX 0.075
#define ALPHABET 26 // the total number of characters in the alphabet
#define LOOPTIME 7 // total number of times looping to find m

int m = 1; // key length of the cipher. It starts with 1, and loop until get to size of the cipher text or when we feel it's enough
int n = 0; // length of the text
int size = 0; // size for each substring. Needs to be global var to be used in two different functions
int *keyArray;

int letterMatrix[ALPHABET][2]; // store the occurrence of each letter (Eng alphabet currently)

void initMatrix() {
  int initLetter = FIRST; // ascii number for 'a'
  for (int i = 0; i < ALPHABET; i++) {
    for (int j = 0; j < 2; j++) {
      if (j == 0) { // if first column then it is character
        letterMatrix[i][j] = initLetter;
        initLetter++;
      }
      else { // else it is count holder
        letterMatrix[i][j] = 0;
      }
    }
  }
}

// search the letter to return the index
int searchLetter(int letter) {
  return letter - FIRST; // return the true index of the letter, since it starts from 97 corresponding to index 0.
}

int letterOccurrence(int letter, char* str, int n) {
  int letterIndex = searchLetter(letter);
  for (int i = 0; i < n; i++) {
    //printf("%c", str[i]);
    if (letter == str[i]) {
      letterMatrix[letterIndex][1]++; // increase the occurrence of the letter
    }
  }
  //printf("\n");
  int occurrence = letterMatrix[letterIndex][1];
  letterMatrix[letterIndex][1] = 0; // reset to use in next loop
  //printf("occurrence of letter %c: %d\n", letter, occurrence);
  return occurrence; // return the total occurrence of one letter in a text.
}

double icFormula(int f, int n) { // occurrence of a letter, size of the text
  double tu = f * (f - 1);
  //printf("Tu so: %f\n", tu);
  double mau = n * (n - 1);
  //printf("Mau so: %f\n", mau);
  double result = tu / mau;
  //printf("IC for this character: %f\n", result);
  return result;
}

// check if the total IC values are in range or not
int checkICValue(double avg) {
  printf("average value: %f\n", avg);
  if (avg > MIN && avg < MAX) {
    return 1;
  }
  return 0;
}

double indexOfCoincidence(char* str, int length) {
  if (m == 1) {
    length = n;
  }
  double result = 0;
  for (int i = FIRST; i <= LAST; i++) { // calculate the IC in case m = 0;
    int occurrence = letterOccurrence(i, str, length);
    //printf("Occurrence of letter %d: %d\n", i, occurrence);
    result = result + icFormula(occurrence, length);
  }
  printf("IC result: %f\n", result);
  return result; //return a result to store in the algo.
}

// this helps generating a subsequence string for cryptanalysis algorithm
char* generateSubString(char* str, int length, int count) {
  if (m == 1) { // m = 1 means the entire string already
    return str;
  }
  char *substr = (char*)malloc(sizeof(char));
  while (count < n) {
    substr[size] = str[count];
    //printf("original character: %c\n", str[count]);
    size++;
    count = count + length;
    //printf("count variable: %d\n", count);
  }
  //printf("Substring: %s\n", substr);
  return substr;
}

int temp = 0; // size of the possible key length array

void vigenereAlgorithm(char *str) {
  for(m = 1; m < LOOPTIME; m++) { // m can only reach the length of the original cipher text. LOOPTIME here is a value for testing
    double avg = 0;
    int k = 0; // index of final result
    printf("value of m noWWWWWWWWWWWWWW: %d\n", m);
    for (int i = 0; i < m; i++) {
      char substr[10000];
      memset(substr, '\0', sizeof(substr));
      strcpy(substr, generateSubString(str, m, i)); // put i here because we need to increase index to get new substring
      //printf("Substring: %s\n", substr);
      avg = avg + indexOfCoincidence(substr, size);
      size = 0; // reset for other substrings
    }
    avg = avg / m; // increment to get the total value, and then divide by the number of substrings to get average
    if (checkICValue(avg) == 1) {
      keyArray[temp] = m;
      temp++;
    }
    //printf("Average IC is: %f\n", avg);
  }
  if (m == n - 1) {
    printf("The text is meaningless");
  }
}

int main() {
  initMatrix(); // init matrix to store mapping of alphabet characters and their occurrences in the cipher text.
  char strTest2[1000] = "CHREEVOAHMAERATBIAXXWTNXBEEOPHBSBQMQEQERBWRVXUOAKXAOSXXWEAHBWGJMMQMNKGRFVGXWTRZXWIAKLXFPSKAUTEMNDCMGTSXMXBTUIADNGMGPSRELXNJELXVRVPRTULHDNQWTWDTYGBPHXTFALJHASVBFXNGLLCHRZBWELEKMSJIKNBHWRJGNMGJSGLXFEYPHAGNRBIEQJTAMRVLCRREMNDGLXRRIMGNSNRWCHRQHAEYEVTAQEBBIPEEWEVKAKOEWADREMXMTBHHCHRTKDNVRZCHRCLQOHPWQAIIWXNRMGWOIIFKEE";
  char str[1000] = "NLAZEIIBLJJI";
  char strTest[1000] = "RAZWHQACTRCHNXBQAIITUMBWMOIAMLQEQXANCVTANRNPZXRVXAVBLQRBEYJ";
  n = strlen(strTest);
  printf("string len: %d\n", n);
  keyArray = (int*) malloc(sizeof(int));
  vigenereAlgorithm(strTest);
  printf("Possible key length: ");
  for (int i = 0; i < temp; i++) {
    printf("%d ", keyArray[i]);
  }
  printf("\n");
  return 0;
}