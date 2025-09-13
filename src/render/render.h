#pragma once

struct {
	int render_cursor;
	int cursor_pos;
} typedef render_settings;

void out_buffer(char* buffer, render_settings settings);
