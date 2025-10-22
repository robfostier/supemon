#pragma once

void enable_colors(void);

void npc_dialog(char *message, int time_in_ms);

void clear_terminal(void);

int get_input(int min, int max);
int get_input_counted(int min, int max);

void print_colored(const char* color, const char* text);

const char* type_to_color(const char* type);