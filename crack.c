/* Program to encrypt data and crack a
 * password by Philip R. Simonson.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAXBUFF	64 // Maximum input buffer.
#define MAXPASS	24 // Maximum length of guess.

// Define the lowercase characters used for cracking.
const char alpha_lower[] = {
	"abcdefghijklmnopqrstuvwxyz"
};
// Define the uppercase characters used for cracking.
const char alpha_upper[] = {
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
};
// Define the numeric characters used for cracking.
const char alpha_digit[] = {
	"0123456789"
};

/* Generate a password for cracking.
 */
void generate(unsigned int index, unsigned int iteration,
	unsigned int length, char *guess, const char *password, bool *done)
{
	if(length == 0) {
		memset(guess + index, 0, 1);
#ifdef DEBUG
		printf("iteration %u, guess: %s\n", iteration, guess);
#endif
		if(!strncmp(guess, password, strlen(password))) { // Got the password.
			printf("*** Password Found ***\nSecret password is %s\n", guess);
			*done = true;
			return;
		} else if(index >= MAXPASS) { // Check for max password length.
			printf("*** Maximum Length Reached ***\nMaximum length is %i\n",
			MAXPASS);
			*done = true;
			return;
		}
		return;
	}

	for(unsigned int i = 0; i < 26; i++) {
		sprintf(guess + index, "%c", alpha_lower[i]);
		generate(index + 1, iteration, length - 1, guess, password, done);
	}
	for(unsigned int i = 0; i < 26; i++) {
		sprintf(guess + index, "%c", alpha_upper[i]);
		generate(index + 1, iteration, length - 1, guess, password, done);
	}
	for(unsigned int i = 0; i < 10; i++) {
		sprintf(guess + index, "%c", alpha_digit[i]);
		generate(index + 1, iteration, length - 1, guess, password, done);
	}
}
/* Crack password from this program.
 */
int crack(const char *password)
{
	clock_t start = clock();
	bool done = false;

	// Check if given password is greater than allowed.
	if(strlen(password) >= MAXPASS) {
		printf("*** Maximum Password Length Reached ***\n"
			"This program cannot exceed: %i\n", MAXPASS);
		return 1;
	}

	while(!done) {
		static unsigned int iteration = 0;
		static unsigned int length = 1;
		static char temp_buf[MAXPASS+1];
		++iteration;
		memset(temp_buf, 0, sizeof(temp_buf));
		generate(0, iteration, length, temp_buf, password, &done);
		length++;
	}
	printf("*** Total time taken is %li (in seconds) ***\n",
		(clock() - start) / CLOCKS_PER_SEC);
	return 0;
}
/* Get string from keyboard input.
 */
int get_string(char *s, int size)
{
	int c, i;
	for(i = 0; i < size && (c = getchar()) != EOF && c != '\n'; i++)
		s[i] = c;
	s[i] = '\0';
	return i;
}
/* Entry point for program.
 */
int main(void)
{
	char buf[MAXBUFF];

	printf("Enter password to crack: ");
	(void)get_string(buf, sizeof(buf));
	return crack(buf);
}
