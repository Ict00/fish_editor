#pragma once

struct {
	int render_cursor;
	int cursor_pos;
	int page_offset;
} typedef render_settings;

extern int WIDTH;
extern int HEIGHT;

void out_bar(int mode, char* file_out);
int out_buffer(char* buffer, render_settings settings);
