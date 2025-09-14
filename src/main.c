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

FILE* opened = NULL;
char* buffer;

char* filename = "buffer";

int program_running = 1;
int cursor_pos = 1;
int page_offset = 0;
int mode = TEXT;

int main(int argc, char** argv) {
	typing(1);

	buffer = malloc(sizeof(char));
	buffer[0] = 1;
	argc -= 1;
	argv++;
	
	if (argc == 1) {
		if (is_file(argv[0])) {
			free(buffer);
			buffer = read_file(argv[0]);
			filename = argv[0];
		}
		
		opened = fopen(argv[0], "w");
	}

	printf("\x1b[2J\x1b[H");
	out_bar(mode, filename);
	cursor_pos = out_buffer(buffer, (render_settings){.cursor_pos=cursor_pos, .render_cursor=mode, .page_offset=page_offset});

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
						case ARROW_DOWN:
							page_offset++;
							goto render;
						case ARROW_UP:
							if (page_offset != 0)
								page_offset--;
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
				if (cursor_pos-1 >= 0) {
					insert_at_pos(&buffer, cursor_pos-1, a);
					cursor_pos++;
				}
				break;
			case NONE:
				if (toupper(a) == 'I') {
					mode = TEXT;
					break;
				}
				if (toupper(a) == 'S') {
					if (opened == NULL) {
						typing(0);
						printf("\x1b[2J\x1b[HName of file: ");
						char fname[512];
						scanf("%s", fname);
						filename = strdup(fname);
						opened = fopen(fname, "w");
						printf("\x1b[2J\x1b[H"); fflush(stdout);
						typing(1);
					}

					fprintf(opened, "%s", buffer);
					printf("\x1b[%d;%dHSaved!", 24/2, (80/2)-6); fflush(stdout);
					getchar();
				}
				if (a == 27) {
					if (get_escape_seq() == ESCAPE)
						program_running = 0;
				}
				break;
		}
render:
		printf("\x1b[2J\x1b[H");
		out_bar(mode, filename);
		cursor_pos = out_buffer(buffer, (render_settings){.cursor_pos=cursor_pos, .render_cursor=mode, .page_offset=page_offset});
	}

	
	typing(0);
	fclose(opened);
	return 0;
}
