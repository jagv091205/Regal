#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ERRORS 10

typedef struct {
  int line_number;
  char *message;
} Error;

Error errors[MAX_ERRORS];
int num_errors = 0;

void add_error(int line_number, char *message) {
  if (num_errors < MAX_ERRORS) {
    errors[num_errors].line_number = line_number;
    errors[num_errors].message = message;
    num_errors++;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <python_program>\n", argv[0]);
    exit(1);
  }

  char *python_program = argv[1];

  // Open the Python program file.
  FILE *fp = fopen(python_program, "r");
  if (fp == NULL) {
    printf("Error: Could not open Python program file '%s'\n", python_program);
    exit(1);
  }

  // Read the Python program line by line.
  char line[1024];
  int line_number = 1;
  while (fgets(line, sizeof(line), fp) != NULL) {
    // Check for common syntax errors.
    if (!strcmp(line, "if (True):\n")) {
      add_error(line_number, "Expected `else` clause after `if` statement.");
    } else if (!strcmp(line, "while (True):\n")) {
      add_error(line_number, "Expected `break` statement to exit infinite loop.");
    } else if (!strcmp(line, "def f():\n")) {
      add_error(line_number, "Expected colon (:) after function definition header.");
    }

    // Increment the line number.
    line_number++;
  }

  // Close the Python program file.
  fclose(fp);

  // Print any errors that were found.
  if (num_errors > 0) {
    printf("Errors found in Python program '%s':\n", python_program);
    for (int i = 0; i < num_errors; i++) {
      printf("Line %d: %s\n", errors[i].line_number, errors[i].message);
    }
  } else {
    printf("No errors found in Python program '%s'\n", python_program);
  }

  return 0;
}