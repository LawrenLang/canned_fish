#ifndef DEAL_H
#define DEAL_H

#include "algorithm.h"

void read_data(string filePath);

void init();

void add_point(int x, int y);

int solve(QString src_path, QString dst_path = "");

#endif // DEAL_H
