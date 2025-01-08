#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_LENGTH 255

typedef struct {
	char buffer[BUFFER_LENGTH];
	ssize_t input_lenght;
} InputBuffer;



/**
 * @brief 初始化一个InputBuffer
 */
InputBuffer* new_input_buffer() {
	InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
	input_buffer->input_lenght = 0;
	return input_buffer;
}

/**
 * @brief 释放input_buffer内存
 *
 * @param input_buffer
 */
void close_input_buffer(InputBuffer* input_buffer) {
	free(input_buffer->buffer);
	free(input_buffer);
}

ssize_t getline(char s[]) {
	ssize_t c, i;
	for (i = 0; i < BUFFER_LENGTH - 1 && (c = getchar()) != '\n' && c != EOF ; ++i) {
		s[i] = c;
	}
	if ('\n' == c) {
		s[i++] = '\n';
	}
	s[i] = '\0';
	return i;
}

/**
 * @brief 打印提示
 */
void print_prompt() {
	printf("mini-sql > ");
}

void read_input(InputBuffer* input_buffer) {
	ssize_t bytes_read = getline(input_buffer->buffer);
	if (bytes_read <= 0) {
		puts("Error reading input");
		exit(EXIT_FAILURE);
	}
	//忽略尾行换行符号
	input_buffer->input_lenght = bytes_read - 1;
	input_buffer->buffer[bytes_read -  1] = 0;
}


int main(int argc, char *argv[]) {
	InputBuffer* input_buffer = new_input_buffer();
	while (true) {
		print_prompt();
		read_input(input_buffer);

		if (strcmp(input_buffer->buffer, ".exit") == 0) {
			puts("good bye~");
			close_input_buffer(input_buffer);
			exit(EXIT_SUCCESS);
		} else {
			printf("Unrecognized command '%s'.\n", input_buffer->buffer);
		}
	}
	return 0;
}
