#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "20180284.h"

#define MAX_PROCESS_COUNT 5
#define BUFFER_SIZE 256
#define READ 0
#define WRITE 1

char read_event[BUFFER_SIZE] = "event:read";
char pass_event[BUFFER_SIZE] = "event:pass";
char exit_event[BUFFER_SIZE] = "event:exit";

int create_process(size_t size) {
  static int current = 0;

  if (size <= 0) {
    return current;
  }
  
  pid_t child = fork();
  if (child == 0) {
    current++;
    return create_process(size - 1); 
  }

  return current;
}

int get_next_index(int current, size_t size) {
  return current + 1 - ((current + 1)  / size * size);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: 20180284 <file>\n");
		return 0;
	}

  int pipes[MAX_PROCESS_COUNT][2] = {};
  for (int i = 0 ; i < MAX_PROCESS_COUNT; i++) {
    assert(pipe(pipes[i]) != -1);
  }

  int current = create_process(MAX_PROCESS_COUNT - 1);
  int next = get_next_index(current, MAX_PROCESS_COUNT);
  assert(current != next);

  pid_t pid = getpid();

  FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
    printf("Error Unable to open %s\n", argv[1]);
		return 0;
  }

  char buffer[BUFFER_SIZE] = {};

  if (current == 0) {
    write(pipes[next][WRITE], read_event, BUFFER_SIZE);
  }

  while (1) {
    memset(buffer, 0, BUFFER_SIZE);
    read(pipes[current][READ], buffer, BUFFER_SIZE);
    
    if (strcmp(buffer, read_event) == 0) {
      char *line = NULL;
      size_t length = 0;
    
      if (getline(&line, &length, fp) == -1) {
        break;
      }
        
      printf("%d %s", pid, line);
        
      free(line);
      

      int pass_proccess_size = MAX_PROCESS_COUNT - 1;
      write(pipes[next][WRITE], pass_event, BUFFER_SIZE);
      write(pipes[next][WRITE], &pass_proccess_size, sizeof(int));

      write(pipes[next][WRITE], read_event, BUFFER_SIZE);
    } else if (strcmp(buffer, pass_event) == 0) {
      char *line = NULL;
      size_t length = 0;

      if (getline(&line, &length, fp) == -1) {
        write(pipes[next][WRITE], exit_event, BUFFER_SIZE);
        break;
      }
      
      int pass_proccess_size;
      read(pipes[current][READ], &pass_proccess_size, sizeof(int));
      
      if (--pass_proccess_size > 0) {
        write(pipes[next][WRITE], pass_event, BUFFER_SIZE);
        write(pipes[next][WRITE], &pass_proccess_size, sizeof(int));
      }
    } else if (strcmp(buffer, exit_event) == 0) {
      write(pipes[next][WRITE], exit_event, BUFFER_SIZE);
      break;
    } else {
      write(pipes[next][WRITE], exit_event, BUFFER_SIZE);
      break;
    }
  }

  fclose(fp);
	return 0;
}

