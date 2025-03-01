#ifndef DATA_BASE_H
#define DATA_BASE_H

typedef struct 
{
  char name[65];
  float score;
  int itemCount;
  char** items;
} SaveData;

#endif
