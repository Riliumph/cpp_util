#ifndef CSV_H
#define CSV_H
#include "data.h"

int
read_csv(const char* filename, struct DATA** data, int* data_num);

#endif // !CSV_H
