#ifndef BASE_H
#define BASE_H
#include "pd_api.h"

extern const PlaydateAPI *pd;

static const float kDefaultRefreshRate = 30.0f;
static const char* FONTPATH = "Asheville-Sans-14-Bold";

void initBase(PlaydateAPI* playdate);

#endif
