/* Program to crack a password with very good brute force (maybe inefficient)
 * by Philip R. Simonson.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAXPWD 11	// Max number of characters for password.

/* Brute force a plain text password.
 */
bool brute_force(char *pass, int size, const char *password)
{
	char arr[95];		// Array to hold printable characters.
	char test[MAXPWD];	// Array to hold testing password.

	if(size >= MAXPWD) return false;

	// Fill array with printable characters.
	for(int i = 0; i < 95; i++) arr[i] = (i + 32);

	// Start clock and zero pass memory.
	clock_t start = clock();
	memset(pass, 0, size);

	// Loop through all array elements of both arr and pass.
	for(int c = 0; c < 95; c++)
		for(int d = 0; d < 95; d++)
			for(int e = 0; e < 95; e++)
				for(int f = 0; f < 95; f++)
					for(int g = 0; g < 95; g++)
						for(int h = 0; h < 95; h++)
							for(int i = 0; i < 95; i++)
								for(int j = 0; j < 95; j++)
									for(int k = 0; k < 95; k++)
										for(int l = 0; l < 95; l++)
											for(int m = 0; m < 95; m++) {
												test[0] = arr[m];
												test[1] = arr[l];
												test[2] = arr[k];
												test[3] = arr[j];
												test[4] = arr[i];
												test[5] = arr[h];
												test[6] = arr[g];
												test[7] = arr[f];
												test[8] = arr[e];
												test[9] = arr[d];
												test[10] = arr[c];

												for(int q = 0, r = MAXPWD-1; q < MAXPWD; q++, r--) {
													for(int s = 0; s < q; s++) {
														strncpy(pass, test+s, r);
														pass[r] = 0;

														if(!strcmp(pass, password)) {
															printf("Password is %s\n", pass);
															printf("Total time (in seconds): %lu\n",
																(clock()-start)/CLOCKS_PER_SEC);
															return true;
														} else {
															static clock_t clk = 0;
															if((clock()-clk)/CLOCKS_PER_SEC > 29) {
															clk = clock();
																printf("Time passed %lu - Current guess: %s\n",
																	(clock()-start)/CLOCKS_PER_SEC, pass);
															}
														}
													}
												}
											}

	printf("Password not found...\n");
	printf("Total time (in seconds): %lu\n", (clock()-start)/CLOCKS_PER_SEC);
	return false;
}
/* Gets a string from user input.
 */
int get_string(char *buf, int size)
{
	int c, i;

	// Get string; Backspace decrements counter.
	for(i = 0; i < size && (c = getchar()) != EOF && c != '\n'; )
		if(c == '\b' && i > 0) --i;
		else buf[i++] = c;

	// Terminate string with null character and return string length.
	buf[i] = '\0';
	return i;
}
#define GET_STRING_PROMPT(M,V,P,N) printf(M); int V = get_string(P, N);
/* Entry point for main program.
 */
int main(void)
{
	char buf[MAXPWD], found[MAXPWD];

	GET_STRING_PROMPT("Enter a password to crack: ", len, buf, sizeof(buf));
	if(len > 0) {
		if(!brute_force(found, sizeof(found)-1, buf))
			return 1;
	} else {
		printf("No input given.\n");
	}

	return 0;
}
