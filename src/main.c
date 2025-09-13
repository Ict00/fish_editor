#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "render/render.h"
#include "utils.h"

#define TEXT 1
#define NONE 0

FILE* opened;
char* buffer;
int program_running = 1;
int cursor_pos = 0;
int mode = TEXT;

int main(int argc, char** argv) {
	typing(1);

	buffer = malloc(sizeof(char));
	buffer[0] = 1;
	argc -= 1;
	
	if (argc == 1) {
		
	}

	printf("\x1b[2J\x1b[H%s", buffer);

	while (program_running) {
		char a = getchar();

		switch (mode) {
			case TEXT:
				if (a == 27) {
					switch (get_escape_seq()) {
						case ESCAPE:
							mode = NONE;
							goto render;
						case ARROW_LEFT:
							if (cursor_pos - 1 >= 0) {
								cursor_pos--;
							}
							goto render;
						case ARROW_RIGHT:
							if (cursor_pos + 1 <= strlen(buffer)) {
								cursor_pos++;
							}
							goto render;
						default: goto render;
					}
				}
				if (a == 127) {
					if (cursor_pos - 1 >= 0) {
						cursor_pos--;
						erase_at_pos(&buffer, cursor_pos);
					}
					goto render;
				}
				if (cursor_pos > strlen(buffer))
					cursor_pos = strlen(buffer);
				
				if (strlen(buffer) == 0) {
					appendc_to_str(&buffer, a);
					cursor_pos = 1;
					goto render;
				}

				if (cursor_pos == strlen(buffer)) {
					appendc_to_str(&buffer, a);
					cursor_pos++;
					goto render;
				}
			
				insert_at_pos(&buffer, cursor_pos+1, a);
				cursor_pos += 2;
				break;
			case NONE:
				if (toupper(a) == 'I') {
					mode = TEXT;
					break;
				}
				if (a == 27) {
					if (get_escape_seq() == ESCAPE)
						program_running = 0;
				}
				break;
		}
render:

		out_buffer(buffer, (render_settings){.cursor_pos=cursor_pos, .render_cursor=mode});
	}

	
	typing(0);
	return 0;
}
