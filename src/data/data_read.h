#ifndef DATA_LOAD_H
#define DATA_LOAD_H

#include "data_base.h"
#include "../base/base.h"

char* pd_strdup(const char* str);

void loadSaveData(SaveData* saveData, const char* filename);

void decodeError(json_decoder* decoder, const char* error, int linenum);

void willDecodeSublist(json_decoder* decoder, const char* name, json_value_type type);

void* didDecodeSaveDataSublist(json_decoder* decoder, const char* name, json_value_type type);

void didDecodeSaveDataTableValue(json_decoder* decoder, const char* key, json_value value);

void didDecodeSaveDataArrayValue(json_decoder* decoder, int index, json_value value);

#endif
