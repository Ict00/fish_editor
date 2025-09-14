#include "utils.h"
#include "render/render.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

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
		case '3':
			  getchar(); // skip
			  return DELETE;
		default:
			// We haven't learned how to utilize other escape sequences so we drop them
			ungetc(third, stdin);
			break;
	}

	return -1;
}

void get_size() {
	struct winsize w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
		WIDTH = w.ws_col;
		HEIGHT = w.ws_row-2;
	} else {
		perror("Error getting terminal size");
	}
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

char* read_file(const char *file_name) {
	FILE* file = fopen(file_name, "r");

	if (!file) {
		fprintf(stderr, "Failed to open file '%s'\n", file_name);
		exit(-1);
		return "";
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* content = malloc(length + 1);

	if (!content) {
		fclose(file);
		return "";
	}
	
	fread(content, 1, length, file);
	content[length] = 0;
	
	fclose(file);

	return content;
}

int is_file(const char *path) {
	struct stat path_stat;
	
	if (stat(path, &path_stat) != 0) {
		return 0;
	}
	return S_ISREG(path_stat.st_mode);
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
