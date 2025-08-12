#pragma once
#include "monster.h"

#define SMALLFISH_WIDTH 8	// ���� ����� �ʺ�
#define SMALLFISH_HP 1		// ���� ����� ü��
#define SMALLFISH_ATTACK 1	// ���� ����� ���ݷ� (�� ĭ)
#define SMALLFISH_CNT 8	// ���� ����� �迭 ��

// ���� ����
static char g_SmallFishGraphic[SMALLFISH_WIDTH] = "<')))><";

// ���� �����
typedef struct SmallFish
{
	Monster mon;		// ���� ����ü ��ø
	MyPoint pos;		// ��ġ
	int attack;		    // ���ݷ�
	bool isRush;
} SmallFish;

// �Լ�
void InitSmallFish();	// �ʱ�ȭ
void DrawSmallFish();	// �׸���
void UpdateSmallFish();	// ������Ʈ
void SmallFishHitPlayer();	// �÷��̾� ����
void PlayerHitSmallFish();	// ���� ����� �ǰ� ó��
void ResetSmallFish();
