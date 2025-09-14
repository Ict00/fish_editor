#include "render.h"
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 23

void out_line(int num) {
	printf("\x1b[1m%-3d \x1b[0m", num);
}

void out_bar(int mode, char *file_out) {
	printf("\x1b[47;30mMODE: %s | FILE: %-61s\x1b[0m\n", mode == 0 ? "NONE" : "TEXT", file_out);
}

int out_buffer(char *buffer, render_settings settings) {
	int cursor_rendered = 0;
	int x = 4;
	int z = 0;
	int i = 0;

	int line_counter = 1;
	int first_line_rendered = 0;

	for (; buffer[i] != 0; i++) {
		if (settings.page_offset != 0) {
			if (buffer[i] == '\n') {
				settings.page_offset--;
				line_counter++;
			}
			continue;
		}

		if (!first_line_rendered) {
			out_line(line_counter);
			first_line_rendered = 1;
		}
		

		if (settings.render_cursor) {
			if (i == settings.cursor_pos && !cursor_rendered) {
				if (buffer[i] == '\n') {
					z++;
					x=4;
					line_counter++;
					printf("\x1b[47;30m \n\x1b[0m");
					out_line(line_counter);
					cursor_rendered = 1;
					continue;
				}
				x++;
				printf("\x1b[47;30m%c\x1b[0m", buffer[i]);
				cursor_rendered = 1;
				continue;
			}
		}
		
		

		printf("%c", buffer[i]);

		if (buffer[i] == '\n') {
			line_counter++;
			x = 4;
			z++;
			
			out_line(line_counter);
		}

		x++;
		if (x >= WIDTH) {
			x = 0;
			printf("\n    ");	
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
