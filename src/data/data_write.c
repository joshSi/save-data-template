#include "data_write.h"
#include <stdio.h>

void writeSaveData(SaveData* saveData, const char* filename){
	SDFile* file = pd->file->open( filename, kFileWrite );
	pd->file->write(file, "{\"name\":\"", 9u);
	pd->file->write(file, saveData->name, strlen(saveData->name));
	pd->file->write(file, "\",\"score\":", 10u);
	char score[10];
	snprintf(score, sizeof(score), "%0.2f", saveData->score);
	pd->file->write(file, score, strlen(score));
	pd->file->write(file, ",\"item_count\":", 14u);
	char itemCount[10];
	snprintf(itemCount, sizeof(itemCount), "%d", saveData->itemCount);
	pd->file->write(file, itemCount, strlen(itemCount));

	pd->file->write(file, ",\"items\":[", 10u);
	for (int i = 0; i < saveData->itemCount - 1; i++) {
		pd->file->write(file, "\"", 1u);
		pd->file->write(file, saveData->items[i], strlen(saveData->items[i]));
		pd->file->write(file, "\",", 2u);
	}
	if (saveData->itemCount > 0) {
		pd->file->write(file, "\"", 1u);
		pd->file->write(file, saveData->items[saveData->itemCount - 1], strlen(saveData->items[saveData->itemCount - 1]));
		pd->file->write(file, "\"", 1u);
	}

	pd->file->write(file, "]", 1u);
	pd->file->write(file, "}", 1u);
	pd->file->close(file);
}
