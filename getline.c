/*
 * getline.c - Gets lines from given file.
 *
 * Author: Philip R. Simonson
 * Date  : 05/21/2020
 *
 ***********************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Get a line from the file using fread.
 */
long get_line(char **lineptr, FILE *fp)
{
	long size = 0, nbytes = 0;
	char *p, *ptr, buf[256];

	if(lineptr == NULL) {
		return -1;
	}

	if(ferror(fp)) {
		return -1;
	}

	if(feof(fp)) {
		return -1;
	}

	while((nbytes = fread(p = buf, 1, sizeof(buf), fp)) > 0) {
		if(memchr(buf, '\n', sizeof(buf)) != 0) {
			ptr = realloc(*lineptr, size+nbytes+2);
			if(ptr == NULL) return -1;
			*lineptr = ptr;
			memcpy(&ptr[size], buf, nbytes);
			size += nbytes;
		} else {
			break;
		}
	}
	(*lineptr)[size++] = '\n';
	(*lineptr)[size] = '\0';
	return size;
}
/* Entry point for program.
 */
int main(void)
{
	char *line = NULL;
	FILE *fp = fopen("bitfiddle.c", "rt");
	if(fp == NULL) return 1;
	while(get_line(&line, fp) != EOF) {
		printf("%s", line);
	}
	fclose(fp);
	free(line);
	return 0;
}
