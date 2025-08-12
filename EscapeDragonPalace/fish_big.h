#pragma once
#include "monster.h"

#define BIGFISH_WIDTH 15	// ū ����� �ʺ�
#define BIGFISH_HEIGHT 3	// ū ����� ����
#define BIGFISH_HP 4		// ū ����� ü��
#define BIGFISH_ATTACK 2	// ū ����� ���ݷ�
#define BIGFISH_CNT 17	// ū ����� �迭 ��

// ���� ����
static char g_BigFishGraphic[2][BIGFISH_HEIGHT][BIGFISH_WIDTH] = {
	{
		"    _______  ",
		" (\\/��((��o\\",
		" (/\\_______/ "
	},// ������ �׸� 0
		{
		"  _______   ",
		" /o��))��\\/)",
		" \\_______/\\)"
	}, // ���� �׸� 1
};

// ū �����
typedef struct BigFish
{
	Monster mon;		// ���� ����ü ��ø
	MyPoint pos;		// ��ġ
	float startPosX;	// ���� x ��ġ
	int attack;			// ���ݷ�
	int moveNum;		// �̵� ����
	Direction dir;		// ���� ����
} BigFish;

// �Լ�
void InitBigFish();		// �ʱ�ȭ
void DrawBigFish();		// �׸���
void UpdateBigFish();	// ������Ʈ
void BigFishHitPlayer();
void PlayerHitBigFish();
void ResetBigFish();