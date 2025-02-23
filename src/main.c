
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data/data_read.h"

SaveData* saveData = NULL;

int update(void* ud)
{
	pd->graphics->drawText("Name: ", 7, kASCIIEncoding, 10, 10);
	pd->graphics->drawText(saveData->name, 65, kASCIIEncoding, 100, 10);

	pd->graphics->drawText("Score: ", 7, kASCIIEncoding, 10, 40);
	char buf[10];
	snprintf(buf, sizeof(buf), "%d", saveData->score);
	pd->graphics->drawText(buf, 10, kASCIIEncoding, 100, 40);

	pd->graphics->drawText("Items: ", 7, kASCIIEncoding, 10, 70);
	for (int i = 0; i < saveData->itemCount; i++) {
		pd->graphics->drawText(saveData->items[i], 65, kASCIIEncoding, 100, 70 + i * 20);
	}
	return 1;
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg;

	if ( event == kEventInit )
	{
		initBase(playdate);
		pd->system->setUpdateCallback(update, NULL);

		saveData= malloc(sizeof(SaveData));
		loadSaveData(saveData, "save.json");
	}

	if ( event == kEventTerminate ) {
		free(saveData);
	}
	
	return 0;
}
