#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <string>
#include <QCoreApplication>

long filesize(FILE* f);
std::string read_source(const char* f);

#endif //FILES_H
