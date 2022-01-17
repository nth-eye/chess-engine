#ifndef LOG_H
#define LOG_H

#include <cstdio>

#define OUT                 printf                                              // Print output
// #define LOG(fmt, ...)       printf("<%s> " fmt "\n", __func__, ##__VA_ARGS__)   // Debug logging

#ifndef OUT
#define OUT(...)
#warning "To enable printing define OUT as output mechanisms";
#endif
#ifndef LOG
#define LOG(...)
#warning "To enable logging define LOG as output mechanisms";
#endif

#endif
