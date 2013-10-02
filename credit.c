/*
 * @(#) credit.c
 *
 * Takes a credit card number from stdin and checks it for 
 * syntactic validity against Hans Peter Luhn's algorithm.
 * If valid, also prints card type (Visa, Mastercard, Discover, or AmEx)
 *
 * Credit card account numbers for testing:
 * http://www.paypalobjects.com/en_US/vhelp/paypalmanager_help/credit_card_numbers.htm
 *
 * @author chindesaurus
 * @version 1.00
 */

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* minimum and maximum credit card number lengths */
#define MIN_LENGTH 13
#define MAX_LENGTH 16

/* function prototypes */
bool isNumeric(char *);
bool luhn(char *);
int  addDigits(int);
void getInput(char *);
void checkPrefix(char *);


int
main(int argc, char **argv) {

    char *input = malloc((MAX_LENGTH + 1) * sizeof(*input));
    assert(input != NULL);

    do {
        printf("Enter credit card number: ");
        getInput(input);
    } while (isNumeric(input) == false);

    if (luhn(input) == false)
        printf("invalid\n"); 
    else {
        printf("valid ");
        checkPrefix(input); 
    }
    
    free(input);
    return 0;     
} 


/*
 * Takes a numeric string s and returns true if every character of s
 * is numeric, else returns false.
 */
bool
isNumeric(char *s) {
    if (s == NULL)
        return false;
    
    while (*s != '\0') {
        if (! isdigit(*s))
            return false;
        s++;
    }
    return true;
}


/* 
 * Implements Luhn's algorithm: takes a numeric string s and returns
 * true if s represents a syntactically valid credit card number,
 * else returns false.
 */
bool
luhn(char *s) {
    int sum = 0;
    int digit, i;
    int length = strlen(s);
    
    if (! (length >= MIN_LENGTH && length <= MAX_LENGTH)) 
        return false;    
    
    // even length
    if (length % 2 == 0) {
        for (i = 0; i < length; i++) {
            digit = s[i] - '0';
            if (i % 2 == 0)
                sum += addDigits(2 * digit);
            else
                sum += digit;
        } 
    }
    // odd length
    else {
        for (i = 0; i < length; i++) {
            digit = s[i] - '0';
            if (i % 2 == 0)
                sum += digit;
            else
                sum += addDigits(2 * digit);
        } 
    }

    // sum is divisible by 10 for valid cards
    return (sum % 10 == 0);
}


/* 
 * Returns the sum of the digits of n. 
 */
int 
addDigits(int n) {
    return (n / 10) + (n % 10);
}


/* 
 * Reads a string from stdin into buffer until newline or EOF is reached
 * (including leading and trailing whitespace), then null-terminates buffer. 
 */ 
void
getInput(char *buffer) {
   
    int length = 0;                 // number of characters in buffer 
    int capacity = MAX_LENGTH + 1;  // capacity of buffer 
    int c;                          // character read
    
    while ( (c = fgetc(stdin)) != '\n' && c != EOF) {

        // if input is too long, try to double the buffer capacity 
        if (length + 1 > capacity) {
            capacity *= 2;
            buffer = realloc(buffer, capacity * sizeof(*buffer));
            assert(buffer != NULL);
        }    

        // put current character in the buffer
        buffer[length++] = c;
    }
    
    // null-terminate buffer
    buffer[length] = '\0';
}


/* 
 * Checks length of the card number and its first few digits (prefix)
 * to determine the card type. Prints the card type to stdout.
 */
void
checkPrefix(char *s) {
    
    int length = strlen(s);

    // Visa: 13 or 16 digits starting with 4
    if ((length == 13 || length == 16) && s[0] == '4')
        printf("Visa\n");

    // Mastercard: 16 digits starting with 5
    else if (length == 16 && s[0] == '5')
        printf("Mastercard\n");

    // Discover: 16 digits starting with 6011
    else if (length == 16 && s[0] == '6' && s[1] == '0' && s[2] == '1' && s[3] == '1')
        printf("Discover\n");

    // American Express: 15 digits starting with 34 or 37
    else if (length == 15 && s[0] == '3' && (s[1] == '4' || s[1] == '7'))
        printf("American Express\n");
    else 
        printf("\nThis number is syntactically valid, but it is not a Visa, Mastercard, Discover, or American Express.\n");
}
