
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data/data_read.h"
#include "data/data_write.h"
#include "keyboard/keyboard.h"

SaveData* saveData = NULL;

typedef struct {
  PDKeyboard* keyboard;
  int keyboardIsAnimating;
  LCDFont* font;
  char* text;
  unsigned int length;
  float cameraX;
} UserData;

const int textLeft = 190;

int update(void* ud)
{
	UserData* data = ud;

	const int keyboardIsVisible = keyboardApi.isVisible(data->keyboard);

	pd->graphics->clear(kColorWhite);
	pd->graphics->setFont(data->font);
	pd->graphics->setDrawMode(kDrawModeCopy);
	pd->graphics->drawRect(textLeft - 4 - data->cameraX, 8, 180, 24, kColorBlack);
	pd->graphics->drawText("Name: ", 7, kASCIIEncoding, 10 - data->cameraX, 10);
	// pd->graphics->drawText(saveData->name, 65, kASCIIEncoding, 100, 10);

	if (data->length == 0 && !keyboardIsVisible) {
			pd->graphics->drawText("Press A to edit", 99, kASCIIEncoding, textLeft - data->cameraX, 10);
	} else {
			if (keyboardIsVisible) {
					pd->graphics->fillRect(textLeft - 4 - data->cameraX, 8, 180, 24, kColorBlack);
					pd->graphics->setDrawMode(kDrawModeInverted);
			}
			pd->graphics->drawText(data->text, data->length, kASCIIEncoding, textLeft - data->cameraX, 10);
	}

	if (!keyboardApi.isVisible(data->keyboard)) {
			PDButtons pressed;
			pd->system->getButtonState(NULL, &pressed, NULL);
			if (pressed & kButtonA) {
					keyboardApi.show(data->keyboard, data->text, data->length);
			}
			snprintf(saveData->name, sizeof(saveData->name), "%s", data->text);
	} else if (!data->keyboardIsAnimating && (pd->system->getCurrentTimeMilliseconds() % 1000 < 500)) {
			const int textWidth = pd->graphics->getTextWidth(data->font, data->text, data->length, kASCIIEncoding, 0);
			const int carretLeft = textLeft - data->cameraX + textWidth + 4;
			pd->graphics->fillRect(carretLeft, 10, 2, 20, kColorWhite);
	}

	data->keyboardIsAnimating = 0;

	pd->graphics->drawText("Score: ", 7, kASCIIEncoding, 10 - data->cameraX, 40);
	char buf[10];
	snprintf(buf, sizeof(buf), "%0.2f", saveData->score);
	pd->graphics->drawText(buf, 10, kASCIIEncoding, 100 - data->cameraX, 40);

	pd->graphics->drawText("Items: ", 7, kASCIIEncoding, 10 - data->cameraX, 70);
	for (int i = 0; i < saveData->itemCount; i++) {
		pd->graphics->drawText(saveData->items[i], 65, kASCIIEncoding, 100 - data->cameraX, 70 + i * 25);
	}

	return 1;
}

static void keyboardAnimating(void * _Nonnull userdata) {
	UserData *data = userdata;
	data->cameraX = LCD_COLUMNS - keyboardApi.getLeft(data->keyboard);
	data->keyboardIsAnimating = 1;
}

static void textChanged(void * _Nonnull userdata) {
	UserData *data = userdata;
	keyboardApi.getText(data->keyboard, &data->text, &data->length);
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg;
	switch(event) {
		case kEventInit:
		{
			initBase(pd);
	
			UserData* ud = pd->system->realloc(NULL, sizeof(UserData));
			ud->keyboard = keyboardApi.newKeyboard();
			const char* error = NULL;
			ud->font = pd->graphics->loadFont(FONTPATH, &error);
			if (error) {
				pd->system->error("Unable to load font: %s", error);
			}
	
			keyboardApi.setPlaydateUpdateCallback(ud->keyboard, update, ud);
			keyboardApi.setRefreshRate(ud->keyboard, kDefaultRefreshRate);
			keyboardApi.setKeyboardAnimatingCallback(ud->keyboard, keyboardAnimating, ud);
			keyboardApi.setTextChangedCallback(ud->keyboard, textChanged, ud);
	
			saveData= malloc(sizeof(SaveData));
			loadSaveData(saveData, "save.json");
			ud->text = pd->system->realloc(NULL, sizeof(char) * sizeof(saveData->name));
	
			snprintf(ud->text, sizeof(saveData->name), "%s", saveData->name);
			ud->length = strlen(ud->text);
			pd->system->setUpdateCallback(update, ud);
		}
			break;
		case kEventTerminate:
		case kEventLowPower:
		{
			writeSaveData(saveData, "save.json");
			free(saveData);
		}
			break;
		default:
			break;
	}
	
	return 0;
}
