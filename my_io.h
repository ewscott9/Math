#ifndef MY_IO_H_ 
#define MY_IO_H_ 

void clear_stdin();
void get_string(char* out_string, int out_string_size);
int get_int();
int prompt_int(const char* text);
int prompt_int_ranged(const char* text, int min, int max);
void prompt_string(const char* prompt, char* out_string, int out_string_size);

#endif
