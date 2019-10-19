import sys
import os
import numpy
FIRST = 65 # first char in ascii table
LAST = 90 # last char in ascii table
MIN = 0.06 # min max values for the IC
MAX = 0.075
ALPHABET = 26 # the total number of characters in the alphabet
LOOPTIME = 7 # total number of times looping to find m

""" REFERENCE OF THE ALGORITHM: 
  Algorithm:  practicalcryptography.com/cryptanalysis/stochastic-searching/cryptanalysis-vigenere-cipher/
  Index of Coincidence (IC) formula: https://pages.mtu.edu/~shene/NSF-4/Tutorial/VIG/Vig-IOC.html 
"""

m = 1; # key length of the cipher. It starts with 1, and loop until get to size of the cipher text or when we feel it's enough
n = 0; # length of the text
keyArray = []
# store the occurrence of each letter (Eng alphabet currently)
letterMatrix = numpy.zeros([ALPHABET, 2], dtype = int)


def initMatrix():
    initLetter = FIRST # ascii number for 'a'
    for i in range(0, ALPHABET):
        for j in range(0, 2):
            if j == 0: # if first column then it is character
                letterMatrix[i][j] = initLetter
                initLetter = initLetter + 1
            else:
                letterMatrix[i][j] = 0

def searchLetter(letter):
    return letter - FIRST

def letterOccurrence(letter, str):
    letterIndex = searchLetter(letter)
    for i in range(0, len(str)):
        if letter == ord(str[i]): # ord() function in python converts character into integer. chr() converts int to char
            # increase the occurrence of the letter
            letterMatrix[letterIndex][1] += 1
    occurrence = letterMatrix[letterIndex][1]
    letterMatrix[letterIndex][1] = 0 # reset to use in next loop
    return occurrence; # return the total occurrence of one letter in a text.


def ICFormula(f, length):  # occurrence of a letter, size of the text
    tu = float(f * (f - 1))
    mau = float(length * (length - 1))
    result = float (tu / mau)
    return result

# check if the total IC values are in range or not
def checkICValue(avg):
    print ("average value: ", avg)
    if avg > MIN and avg < MAX:
        return 1
    return 0

# calculate IC in a specific m value
def indexOfCoincidence(str):
    result = 0
    for i in range (FIRST, LAST):
        occurrence = letterOccurrence(i, str)
        result = result + ICFormula(occurrence, len(str))
    print ("IC result: ", result)
    return result  # return a result to store in the algo.

# this helps generating a subsequence string for cryptanalysis algorithm
def generateSubstring(str, length, count):
    if length == 1:  # m = 1 means the entire string already
        return str
    substr = []
    while count < n: # since we have a cipher text size n, so always generate a substring to n max
        substr.append(str[count])
        count += length
    return substr



def vigenereAlgorithm(str, m):
    # m can only reach the length of the original cipher text. LOOPTIME here is a value for testing
    for m in range(1, LOOPTIME):
        avg = 0
        print ("Value of m currently: ", m)
        for i in range (0, m):
            # put i here because we need to increase index to get new substring
            substr = generateSubstring(str, m, i)
            avg += indexOfCoincidence(substr)
        avg = avg / m  # increment to get the total value, and then divide by the number of substrings to get average
        if checkICValue(avg) == 1:
            keyArray.append(m)

    if m == n - 1:
        print ("Meaningless text")

def main():
    initMatrix()
    print ("length of the str: ", n)
    vigenereAlgorithm(strTest, 1)
    for i in range (0, len(keyArray)):
        print (keyArray[i])

if __name__ == "__main__":
    strTest = "CHREEVOAHMAERATBIAXXWTNXBEEOPHBSBQMQEQERBWRVXUOAKXAOSXXWEAHBWGJMMQMNKGRFVGXWTRZXWIAKLXFPSKAUTEMNDCMGTSXMXBTUIADNGMGPSRELXNJELXVRVPRTULHDNQWTWDTYGBPHXTFALJHASVBFXNGLLCHRZBWELEKMSJIKNBHWRJGNMGJSGLXFEYPHAGNRBIEQJTAMRVLCRREMNDGLXRRIMGNSNRWCHRQHAEYEVTAQEBBIPEEWEVKAKOEWADREMXMTBHHCHRTKDNVRZCHRCLQOHPWQAIIWXNRMGWOIIFKEE"
    n = len(strTest)
    

main()
