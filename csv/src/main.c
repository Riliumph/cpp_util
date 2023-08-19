// standard
#include <stdio.h>
#include <stdlib.h>

#include "csv.h"

int
main(void)
{
  data_t* data_list = NULL;
  int data_num;
  read_csv("./data.csv", &data_list, &data_num);

  for (int i = 0; i < data_num; ++i) {
    printf("%d: %c, %s, %d, %d\n",
           i,
           data_list[i].type,
           data_list[i].cost,
           data_list[i].hint,
           data_list[i].prio);
  }
  free(data_list);
  return 0;
}
