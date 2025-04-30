#include "csv.h"
// standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int
count_line(const char* filename)
{
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Cannot read file.");
    exit(-1);
  }

  char line[256] = "";
  int line_count = 0;
  while (fgets(line, 256, fp) != NULL) {
    if (line[0] != '#') {
      ++line_count;
    }
  }
  fclose(fp);
  return line_count;
}

int
read_csv(const char* filename, data_t** data, int* data_num)
{
  *data_num = count_line(filename);

  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Cannot read file.");
    exit(-1);
  }

  *data = (data_t*)malloc(*data_num * sizeof(data_t));
  if (*data == NULL) {
    printf("Cannot allocate memory\n");
    exit(-1);
  }

  char line[256] = "";
  int idx = 0;
  while (fgets(line, 256, fp) != NULL) {
    if (line[0] == '#') {
      continue;
    }
    sscanf(line,
           "%[^,],%[^,],%d,%d",
           &(*data + idx)->type,
           (*data + idx)->cost,
           &(*data + idx)->hint,
           &(*data + idx)->prio);
    ++idx;
  }
  return 0;
}
