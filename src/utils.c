#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>

int get_escape_seq() {
	// First was already taken :(
	char second = getchar();

	if (second != '[') {
		ungetc(second, stdin);
		return ESCAPE;
	}

	char third = getchar();
	
	switch (third) {
		case 'A': return ARROW_UP;
		case 'B': return ARROW_DOWN;
		case 'C': return ARROW_RIGHT;
		case 'D': return ARROW_LEFT;
		default:
			// We haven't learned how to utilize other escape sequences so we drop them
			ungetc(third, stdin);
			break;
	}

	return -1;
}

void typing(int enable) {
	static struct termios oldt, newt;
    
	if (enable) {
		tcgetattr(STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		printf("\x1b[?25l");
	} else {
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		printf("\x1b[?25h");
	}
}

void erase_at_pos(char** dest, int pos) {
	char* new_str = malloc(sizeof(char) * strlen(*dest));
	int b = 0;
	
	for (int i = 0; (*dest)[i] != 0; i++) {
		if (i != pos) {
			new_str[b] = (*dest)[i];
			b++;
		}
	}

	new_str[b] = 0;
	free(*dest);
	*dest = new_str;
}

void insert_at_pos(char **dest, int pos, char ch) {
	int nl = strlen(*dest) + 2;
	char* new_dest = malloc(sizeof(char)*nl);
	int b = 0;

	for (int i = 0; (*dest)[i] != 0; i++) {
		new_dest[b] = (*dest)[i];
		if (b == pos) {
			b++;
			new_dest[b] = ch;
		}
		b++;
	}
	new_dest[b] = 0;
	free(*dest);
	*dest = new_dest;
}

void append_to_str(char **dest, char *str) {
	int new_length = strlen(*dest) + strlen(str) + 1;
	char* new_dest = malloc(sizeof(char)*new_length);
	strcpy(new_dest, *dest);
	strcat(new_dest, str);
	free(*dest);
	*dest = new_dest;
}

void appendc_to_str(char **dest, char ch) {
	char t[2];
	t[0] = ch;
	t[1] = 0;
	append_to_str(dest, t);
}
