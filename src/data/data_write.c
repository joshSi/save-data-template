#include "data_write.h"

void writefile(void* userdata, const char* str, int len) {
	pd->file->write((SDFile*)userdata, str, len);
}
