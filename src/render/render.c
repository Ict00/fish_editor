#include "render.h"
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 24

int out_buffer(char *buffer, render_settings settings) {
	printf("\x1b[2J\x1b[H");
	
	int cursor_rendered = 0;
	int x = 0;
	int z = 0;
	int i = 0;
	for (; buffer[i] != 0; i++) {
		if (settings.page_offset != 0) {
			if (buffer[i] == '\n')
				settings.page_offset--;
			continue;
		}

		if (settings.render_cursor) {
			if (i == settings.cursor_pos && !cursor_rendered) {
				if (buffer[i] == '\n') {
					z++;
					x=0;
					printf("\x1b[47;30m \n\x1b[0m");
					cursor_rendered = 1;
					continue;
				}
				x++;
				printf("\x1b[47;30m%c\x1b[0m", buffer[i]);
				cursor_rendered = 1;
				continue;
			}
		}
		
		if (buffer[i] == '\n') {
			x = 0;
			z++;
		}

		printf("%c", buffer[i]);
		x++;
		if (x >= WIDTH) {
			x = 0;
			printf("\n");
			z++;
		}
		if (z >= HEIGHT) {
			break;
		}
	}
	
	if (settings.render_cursor && !cursor_rendered) {
		printf("\x1b[47;30m \x1b[0m"); fflush(stdout);
		return i;
	}

	fflush(stdout);

	return settings.cursor_pos;
}
