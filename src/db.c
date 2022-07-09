#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    
    return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer* input_buffer) {
    // If the buffer is set to NULL, it is mallocatted by getline and should be freed later.
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // Remove trailing newline character \n
    input_buffer->input_length = bytes_read - 1; // 1 character takes 1 byte.
    input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMEND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit")==0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMEND_UNRECOGNIZED_COMMAND;
    }
}

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
} Statement;


PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    /*
        insert using srncmp as the "insert" keyword will be followed by data, i.e.
            insert 1 cstack foo@bar.com
    */
    
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement) {
    switch (statement->type) {
        case(STATEMENT_INSERT):
            printf("This is where we will do an insert.");
            break;
        case(STATEMENT_SELECT):
            printf("This is where we will do a select.");
            break;
    }
}

int main() {
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (strncmp(input_buffer->buffer, ".", 1) == 0) {
            switch (do_meta_command(input_buffer)){
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMEND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }
        Statement statement;
        switch (prepare_statement(input_buffer, &statement)){
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognied keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
        }

        execute_statement(&statement);
        printf("Exectued.\n");
    }
    return(0);
}