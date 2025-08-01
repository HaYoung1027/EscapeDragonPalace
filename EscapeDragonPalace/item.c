#include "init.h"
#include "item.h"

char seaweedSprite[2][ITEM_SPRITE_ROWS][ITEM_SPRITE_COLS] = {
	{
		" )) (( ((",
		"((   )) ))",
		" )) (( (("
	},
	{
		"((  )) ))",
		" ))(( ((",
		"((  )) ))"
	}
};
char bubblesSprite[2][ITEM_SPRITE_ROWS][ITEM_SPRITE_COLS] = {
	{
		"ㅇ.o",
		".ㅇo"
	},
	{
		".ㅇo",
		"ㅇ.o"
	}
};


Item itemList[MAX_ITEM_COUNT];
int numItem = 0;
int frame = 0;


bool SettingItem = false;

bool GetSettingItem()
{
	return SettingItem;
}

void SetSettingItem(bool src)
{
	SettingItem = src;
}

// frame 값 가져오기
int GetFrame()
{
	return frame;
}


clock_t lastFrameTime = 0;
const int frameDelay = 400;  // 200ms마다 프레임 전환

void ItemFrameDelay() {
	clock_t now = clock();
	int elapsed = (now - lastFrameTime) * 1000 / CLOCKS_PER_SEC;

	if (elapsed >= frameDelay) {
		frame = (frame + 1) % 2;
		lastFrameTime = now;
	}

	_Invalidate();  // 매 프레임 그리기는 계속!
}


void DrawItem(Item* item, int frame) {

	SpriteType* sprite = &seaweedSprite;

	switch (item->type) {
	case ITEM_LIFE:
		sprite = &seaweedSprite;
		break;
	case ITEM_SPEED:
		sprite = &bubblesSprite;
		break;
	}

	int tempX = item->x - GetPlusX();


	for (int row = 0; row < ITEM_SPRITE_ROWS; row++) {
		if ((*sprite)[frame][row] == '\0') break;  // 공백 줄이면 중단

		for (int col = 0; col < ITEM_SPRITE_COLS; col++)
		{
			// 아이템 위치가 화면 내에 있을 때만 출력
			if(0 <= tempX + col && SCREEN_WIDTH >= tempX + col)
			{
				if (tempX + 7 > 0 && tempX < SCREEN_WIDTH) {
					_DrawText(tempX + col, item->y + row, (char[]) { (*sprite)[frame][row][col], 0 });
				}
			}
		}
	}
}



void InitItem() {

	// 첫 번째 아이템 - 해초
	itemList[numItem++] = (Item){
		.type = ITEM_LIFE,
		.x = 30, .y = 21,
		.isHeld = false,
		.value = 2,
		.width = 11,
		.height = 3,
		.mapStatus = 2
	};

	// 두 번째 아이템 - 공기방울
	itemList[numItem++] = (Item){
		.type = ITEM_SPEED,
		.x = 60, .y = 22,
		.isHeld = true,
		.value = 1,
		.width = 5,
		.height = 2,
		.mapStatus = 2
	};
}
