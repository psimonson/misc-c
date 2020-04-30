#include <stdio.h>
#include <string.h>

/* Gets a list of tokens from input.
 */
char *crypt(char *s, int len, const char *key)
{
	int i, j, key_len = strlen(key);

	for(i = 0, j = 0; i < len; i++, j++) {
		if(j >= key_len) j = 0;
		s[i] = s[i] ^ ((((key[j] - 10) / 2) + j) % 127);
	}
	s[i] = '\0';
	return s;
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
	char buf[128];

	printf("Enter string to encrypt:\n");
	int len = get_string(buf, sizeof(buf));

	printf("Original : %s\n", buf);
	printf("Encrypted: %s\n", crypt(buf, len, "ENSOIEJ"));
	printf("Decrypted: %s\n", crypt(buf, len, "ENSOIEJ"));

	return 0;
}
