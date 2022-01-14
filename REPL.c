#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECGNIZED_STATEMENT } PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;

InputBuffer *new_input_buffer();
void print_prompt();
void read_input(InputBuffer *input_buffer);
void delete_input_buffer(InputBuffer *input_buffer);
MetaCommandResult do_meta_command(InputBuffer *input_buffer);
PrepareResult Prepare_statement(InputBuffer *input_buffer,
                                Statement *statement);
void execute_statement(Statement *statement);

int main(int argc, char *argv[]) {
  InputBuffer *input_buffer = new_input_buffer();
  // 声明 InputBuffer

  // 无限循环
  while (true) {
    // 打印提示
    print_prompt();
    // 读取输入
    read_input(input_buffer);

    // 处理元命令
    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
      case (META_COMMAND_SUCCESS):
        continue;
      case (META_COMMAND_UNRECGNIZED_COMMAND):
        printf("Unrecgnized command '%s' \n", input_buffer->buffer);
        continue;
      }
    }

    // 处理语句
    Statement statement;
    switch (Prepare_statement(input_buffer, &statement)) {
    case PREPARE_SUCCESS:
      break;
    case PREPARE_UNRECGNIZED_STATEMENT:
      printf("Unrecgnized keyword at start of '%s'. \n", input_buffer->buffer);
      continue;
    }

    // 执行语句
    execute_statement(&statement);
    printf("Executed.\n");
  }
  return 0;
}

InputBuffer *new_input_buffer() {
  InputBuffer *input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buffer_length = 0;
  input_buffer->input_length = 0;
  return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer *input_buffer) {
  ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input! \n");
    exit(EXIT_FAILURE);
  }

  // 忽略换行
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

void delete_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer *input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    delete_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECGNIZED_COMMAND;
  }
}

// 简略的 SQL 解析器
PrepareResult Prepare_statement(InputBuffer *input_buffer,
                                Statement *statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECGNIZED_STATEMENT;
}

void execute_statement(Statement *statement) {
  switch (statement->type) {
  case (STATEMENT_INSERT):
    printf("This is where we would do an insert.\n");
    break;
  case (STATEMENT_SELECT):
    printf("This is where we would do a select.\n");
    break;
  }
}