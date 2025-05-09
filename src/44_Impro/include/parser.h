#define INPUT_BUFFER_SIZE 256

extern char input_buffer[];
extern int input_index;

int strcmp(const char* s1, const char* s2);
void process_command(const char* input);
