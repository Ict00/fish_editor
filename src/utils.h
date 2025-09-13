#pragma once

// KEYS (it's really needed yeah)
#define ARROW_UP 0
#define ARROW_DOWN 1
#define ARROW_LEFT 2
#define ARROW_RIGHT 3
#define ESCAPE 4

struct {
	int width;
	int height;
} typedef termsize_t;


termsize_t get_size();
int get_escape_seq();
void typing(int enable);
void insert_at_pos(char** dest, int pos, char ch);
void erase_at_pos(char** dest, int pos);
void append_to_str(char** dest, char* str);
void appendc_to_str(char** dest, char ch);
