#include "data_read.h"

char* pd_strdup(const char* str)
{
	size_t len = strlen(str);
	char* s = malloc(len+1);
	memcpy(s, str, len);
	s[len] = '\0';
	return s;
}

void loadSaveData(SaveData* saveData, const char* filename) {
	json_decoder decoder =
	{
		.decodeError = decodeError,
		.willDecodeSublist = willDecodeSublist,
		.userdata = saveData
	};
	SDFile* file = pd->file->open(filename, kFileRead|kFileReadData);

	json_value val;
	pd->json->decode(&decoder, (json_reader){ .read = (json_readFunc*)pd->file->read, .userdata = file }, &val);
	pd->file->close(file);
}

void decodeError(json_decoder* decoder, const char* error, int linenum)
{
	pd->system->logToConsole("decode error line %i: %s", linenum, error);
}

void willDecodeSublist(json_decoder* decoder, const char* name, json_value_type type)
{
	if ( strcmp(name, "_root") == 0 )
	{
		decoder->didDecodeTableValue = didDecodeSaveDataTableValue;
		decoder->didDecodeArrayValue = didDecodeSaveDataArrayValue;
	}
}

void didDecodeSaveDataArrayValue(json_decoder* decoder, int pos, json_value value)
{
	SaveData* saveData = decoder->userdata;
	if ( strcmp(decoder->path, "items") == 0 )
	{
		saveData->items[pos-1] = pd_strdup(json_stringValue(value));
	}
	decoder->userdata = saveData;
}

void didDecodeSaveDataTableValue(json_decoder* decoder, const char* key, json_value value)
{
	SaveData* saveData = decoder->userdata;
	if ( strcmp(key, "name") == 0 && value.type == kJSONString ) {
		strncpy(saveData->name, json_stringValue(value), sizeof(saveData->name) - 1);
	}
	else if ( strcmp(key, "score") == 0 && value.type == kJSONInteger ) {
		saveData->score = json_intValue(value);
	}
	else if ( strcmp(key, "item_count") == 0 && value.type == kJSONInteger ) {
		saveData->itemCount = json_intValue(value);
		saveData->items = malloc(sizeof(char*) * saveData->itemCount);
	}
	decoder->userdata = saveData;
}
