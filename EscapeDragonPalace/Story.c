#include "init.h"
#include "Rabbit.h"
#include "Turtle.h"


bool IsStartStory = false;
bool IsBossStory = false;
bool IsEndStory = false;


bool GetStartStory()
{
	return IsStartStory;
}

void SetStartStory(bool src)
{
	IsStartStory = src;
}

bool GetBossStory()
{
	return IsBossStory;
}

void SetBossStory(bool src)
{
	IsBossStory = src;
}

bool GetEndStory()
{
	return IsEndStory;
}

void SetEndStory(bool src)
{
	IsEndStory = src;
}


char g_RabbitColor = E_White;
char g_TurtleColor = E_White;

//���丮 ȭ�� Ʋ
char StoryFrame[SCREEN_HEIGHT][SCREEN_WIDTH] = 
{
"��============================================================================��",
"��                                                                 skip(q) >> ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��____________________________________________________________________________��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��                                                                            ��",
"��============================================================================��"
};

//���丮 �䳢
char StoryRabbit[RabbitY][8] =
{
	" (\\(\\",
	" ('-')",
	"o(   )"
};

//���丮 �ڶ�
char StoryTurtle[TURTLE_HEIGHT][20] =
{
	"       ______       ",
	" ___ _/ \__/ \_   /|",
	"(_o / \ /  \ / \_/ |",
	" \__ -----------__/ ",
	"     \_|_|_|_|_/    ",
	"    /_|_|  /_|_|    ",
};		


typedef enum StoryStatus
{
	E_Story = 0,  // �ؼ�
	E_Rabbit, // �䳢
	E_Turtle, // �ڶ�
} StoryStatus;

typedef struct Story
{
	StoryStatus status; //enum (�䳢���� �ڶ����� �ؼ�����)
	char ment[STR_MAX];
}Story;


Story StartStory[] = {
	{E_Story, "[�ƾƾ��� ���� ȣ���̰� ����Ǵ� ����...]"},
	{E_Story, "[��տ��� ���ɹް� �ʹ� �ڶ�� �䳢�� ���� �ǰ��� ���ٴ� �ҹ��� �����.]"},
	{E_Story, "[����� ������ �ޱ� ���� �䳢�� ������ �����ϴµ�...]"},
	{E_Turtle ,"�ȳ�! �츮 ��ÿ��� ��� 1+1 �̺�Ʈ �������ε� �ʵ� �÷�?"},
	{E_Rabbit, "������? ���� ����!" },
	{E_Turtle, "���� ����� ���� �ȳ����ٰ�!"},
	{E_Story, "[�׸��� ���]"},
	{E_Rabbit, "����? ����� ����? ����� ��� �־�?"},
	{E_Turtle, "...��������!!! ���� �䳢�� ���� ��� �Ǿ�����!!"},
	{E_Rabbit, "?!"},
	{E_Turtle, "���� ������Ű�°� ���� �����ϱ�.. ������ ���ֳ��°� ������?"},
	{E_Rabbit, "Ǯ����!!!"},
	{E_Turtle, "���� �Ŀ� ���ڰ�. ũ����!"}
};

Story BossStory[] = {
	{E_Turtle, "?! ����!! �װ� �� �ű⼭ ����!!"},
	{E_Rabbit, "�ʿ��� �� ���� ����!!" },
	{E_Turtle, "�׷��� �����ðڴ�? ���� �ºξ�!" },
	{E_Turtle, "�װ� �̱�� Ư���� ���� ��������." },
	{E_Rabbit, "�������� �� ���̾���! ����!" }
};

Story EndStory[] = {
	{E_Turtle, "����! �ȵ� �� ��!!"},
	{E_Rabbit, "���� �� ���̾�! �� �־�� ��û�� �ڶ�!"},
	{E_Story, "[�׷��� �䳢�� ���� ������ �ʰ� �ູ�ϰ� ��Ҵ�ϴ�~!]"}
};


void CharacterDraw()
{
	// �䳢 �׸���
	for (int y = 0; y < RabbitY; y++)
	{
		_DrawTextColor(6, 14 + y, StoryRabbit[y], g_RabbitColor);
	}

	// �ڶ� �׸���
	for (int y = 0; y < TURTLE_HEIGHT; y++)
	{
		_DrawTextColor(55, 11 + y, StoryTurtle[y], g_TurtleColor);
	}
}

void PlayStartStory()
{
	int StartStoryCount = sizeof(StartStory) / sizeof(StartStory[0]);

	for (int i = 0; i < StartStoryCount; i++)
	{
		switch (StartStory[i].status) {
		case E_Story:
			// ��Ʈ ���
			_DrawTextColor(4, 19, StartStory[i].ment, E_White);

			// ���丮 Ʋ �׸���
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				_DrawTextColor(0, y, StoryFrame[y], E_White);
			}
			break;
		case E_Rabbit:
			// ĳ���� �̸� ���
			_DrawTextColor(4, 19, "�䳢", E_White);

			// ��Ʈ ���
			_DrawTextColor(4, 21, StartStory[i].ment, E_White);

			// �䳢, �ڶ� ���� ����
			g_RabbitColor = E_White;
			g_TurtleColor = E_Gray;

			CharacterDraw();

			// ���丮 Ʋ �׸���
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				_DrawTextColor(0, y, StoryFrame[y], E_White);
			}
			break;
		case E_Turtle:
			// ĳ���� �̸� ���
			_DrawTextColor(4, 19, "�ڶ�", E_White);

			// ��Ʈ ���
			_DrawTextColor(4, 21, StartStory[i].ment, E_White);

			// �䳢, �ڶ� ���� ����
			g_RabbitColor = E_Gray;
			g_TurtleColor = E_White;

			CharacterDraw();

			// ���丮 Ʋ �׸���
			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				_DrawTextColor(0, y, StoryFrame[y], E_White);
			}
			break;
		}

		while (1)
		{
			if (g_Key == 'q' || g_Key == 'Q') 
			{
				IsStartStory = true; // ���� ���� ���丮 �Ϸ�
				return;
			}
			if (g_Key == ' ') break;

			//_Invalidate();
		}
	}

	IsStartStory = true; // ���� ���� ���丮 �Ϸ�
}

void PlayBossStory()
{
	int BossStoryCount = sizeof(BossStory) / sizeof(BossStory[0]);

	for (int i = 0; i < BossStoryCount; i++)
	{

		while (1)
		{
			if (g_Key == 'q' || g_Key == 'Q')
			{
				IsBossStory = true; // ���� ���� ���丮 �Ϸ�
				return;
			}
			if (g_Key == ' ') break;
		}
	}
}

void PlayEndStory()
{
	int EndStoryCount = sizeof(EndStory) / sizeof(EndStory[0]);

	for (int i = 0; i < EndStoryCount; i++)
	{

		while (1)
		{
			if (g_Key == 'q' || g_Key == 'Q')
			{
				IsEndStory = true; // ���� ���� ���丮 �Ϸ�
				return;
			}
			if (g_Key == ' ') break;
		}
	}
}