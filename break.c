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

/* Cryptography with XOR scheme.
 */
char *crypt(const char *data, int len, const char *hash)
{
	static char temp[MAXBUFF];
	int key_len = strlen(hash);
	int i, j, k;

	memset(temp, 0, sizeof(temp));
	for(i = 0, j = 0, k = 0; j < len; i++, j++, k++) {
		if(k >= key_len) k = 0;
		temp[i] = data[j] ^ ((((hash[k] - 10) / 2) + k) % 127);
	}
	temp[i] = 0;
	return temp;
}
/* Generate a password for cracking.
 */
void generate(const char *cipher, int cipher_len, unsigned int index,
	unsigned int iteration, unsigned int length, char *guess,
	const char *plain, bool *done)
{
	if(length == 0) {
		memset(guess + index, 0, 1);
#ifdef DEBUG
		printf("iteration %u, guess: %s\n", iteration, guess);
#endif
		const char *decrypted = crypt(cipher, strlen(cipher), guess);
		if(!strcmp(decrypted, plain)) { // Got the password.
			printf("Decrypted: %s\n"
				"*** Password Found ***\n"
				"Secret password is %s\n", decrypted, guess);
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
		generate(cipher, cipher_len, index + 1, iteration, length - 1, guess, plain, done);
	}
	for(unsigned int i = 0; i < 26; i++) {
		sprintf(guess + index, "%c", alpha_upper[i]);
		generate(cipher, cipher_len, index + 1, iteration, length - 1, guess, plain, done);
	}
	for(unsigned int i = 0; i < 10; i++) {
		sprintf(guess + index, "%c", alpha_digit[i]);
		generate(cipher, cipher_len, index + 1, iteration, length - 1, guess, plain, done);
	}
}
/* Crack password from this program.
 */
int crack(const char *cipher, int cipher_len, const char *plain)
{
	static unsigned int iteration = 0;
	clock_t start = clock();
	bool done = false;

	// Check if given password is greater than allowed.
	if(cipher_len >= MAXBUFF || strlen(plain) >= MAXBUFF) {
		printf("*** Maximum Cipher/Plain Text Length Reached ***\n"
			"This program cannot exceed: %i\n", MAXBUFF);
		return 1;
	}

	while(!done) {
		static unsigned int length = 1;
		static char temp_buf[MAXPASS+1];
		++iteration;
		memset(temp_buf, 0, sizeof(temp_buf));
		generate(cipher, cipher_len, 0, iteration, length, temp_buf, plain, &done);
		length++;
	}
	printf(
		"*** Total iterations taken %u ***\n"
		"*** Total time taken is %li (in seconds) ***\n",
		iteration, (clock() - start) / CLOCKS_PER_SEC);
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
	char buf[MAXBUFF], buf2[MAXBUFF];

	// Get plain text from user to break an XOR encryption hash.
	printf("Enter plain text: ");
	int len = get_string(buf, sizeof(buf));
	const char *cipher = crypt(buf, len, "AB");
	strncpy(buf2, cipher, len);
	printf("Encrypted: %s\n", buf2);
	return crack(buf2, len, buf);
}
