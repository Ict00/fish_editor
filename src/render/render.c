#include "render.h"
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 24

void out_buffer(char *buffer, render_settings settings) {
	printf("\x1b[2J\x1b[H");
	
	int cursor_rendered = 0;
	
	for (int i = 0; buffer[i] != 0; i++) {
		if (settings.render_cursor) {
			if (i == settings.cursor_pos && !cursor_rendered) {
				if (buffer[i] == '\n') {
					printf("\x1b[47;30m \n\x1b[0m");
					cursor_rendered = 1;
					continue;
				}
				printf("\x1b[47;30m%c\x1b[0m", buffer[i]);
				cursor_rendered = 1;
				continue;
			}
		}
		printf("%c", buffer[i]);
	}
	
	if (settings.render_cursor && !cursor_rendered) {
		printf("\x1b[47;30m \x1b[0m");
	}

	fflush(stdout);
}
