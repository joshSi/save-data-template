#include "base.h"

const struct PlaydateAPI *pd;

void initBase(PlaydateAPI* playdate) {
  pd = playdate;
  pd->display->setRefreshRate(kDefaultRefreshRate);
}
