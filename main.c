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
InputBuffer* new_input_buffer(void) {
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
void print_prompt(void) {
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

typedef enum {
	META_COMMAND_SUCCESS, //输入的命令是正确的元数据
	META_COMMAND_UNRECOGNIZED_COMMAND //输入的命令是未识别的元数据
} MetaCommandResult;


/**
 * @brief 执行元数据命令 
 */
MetaCommandResult do_meta_command(InputBuffer* input_buffer){
	if(strcmp(input_buffer->buffer, ".exit") == 0){
		puts("good bye~");
		close_input_buffer(input_buffer);
		exit(EXIT_SUCCESS);
	}else{
		return META_COMMAND_UNRECOGNIZED_COMMAND;
	}
}

typedef enum {
	STATEMENT_INSERT, //insert语句类型
	STATEMENT_SELECT //select语句类型
} StatementType;

typedef enum {
	PREPARE_SUCCESS, //输入的sql是正确的sql
	PREPARE_UNRECOGNIZED_STATEMENT//输入的sql不正确
} PrepareResult;

typedef struct {
	StatementType type;
} Statement; //表示执行语句

/**
 * @brief 处理sql语句 
 */
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
	if(strncmp(input_buffer->buffer, "insert", 6) == 0){
		statement->type = STATEMENT_INSERT;
		return PREPARE_SUCCESS;
	}
	if(strncmp(input_buffer->buffer, "select", 6) == 0){
		statement->type = STATEMENT_SELECT;
		return PREPARE_SUCCESS;
	}
	return PREPARE_UNRECOGNIZED_STATEMENT;
}

/**
 * @brief 执行statement 
 */
void execute_statement(Statement* statement){
	switch(statement->type){
		case(STATEMENT_INSERT):
			//执行insert操作
			puts("This is where we would do an insert.");
			break;
		case(STATEMENT_SELECT):
			//执行update操作
			puts("This is where we would do an update.");
			break;
	}
}

int main(void) {
	InputBuffer* input_buffer = new_input_buffer();
	while (true) {
		print_prompt();
		read_input(input_buffer);

		
		//执行元数据操作
		if (input_buffer->buffer[0] == '.') {
			//以.为开头的语句表示执行元数据操作
			switch(do_meta_command(input_buffer)){
				case(META_COMMAND_SUCCESS):
					continue;
				case(META_COMMAND_UNRECOGNIZED_COMMAND):
					printf("Unrecognized command '%s' \n", input_buffer->buffer);
					continue;
			}
		}
		
		//执行sql操作
		Statement statement;
		switch(prepare_statement(input_buffer, &statement)){
			case(PREPARE_SUCCESS):
				break;
			case(PREPARE_UNRECOGNIZED_STATEMENT):
				printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
				continue;
		}
		execute_statement(&statement);
		puts("Executed.");
		
	}
	return 0;
}
