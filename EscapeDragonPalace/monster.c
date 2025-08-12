#include "init.h"
#include "crab.h"
#include "fish_big.h"
#include "fish_small.h"

// ���� �ʱ�ȭ
void InitMonster()
{
	InitBigFish();		// ū ����� �ʱ�ȭ
	InitSmallFish();	// ���� ����� �ʱ�ȭ
	InitCrab();			// �ɰ� �ʱ�ȭ
}

// ���� alive false ó��
void ResetMonster()
{
	ResetBigFish();		// ū ����� falseó��
	ResetSmallFish();	// ���� ����� falseó��
	ResetCrab();		// �ɰ� falseó��
}

// ���� ������Ʈ
void UpdateMonster()
{
	UpdateBigFish();	// ū ����� ������Ʈ
	UpdateSmallFish();	// ���� ����� ������Ʈ
	UpdateCrab();		// �ɰ� ������Ʈ
}

// ���� �׸��� �Լ�
void DrawMonster()
{
	DrawBigFish();		// ū ����� �׸���
	DrawSmallFish();	// ���� ����� �׸���
	DrawCrab();			// �ɰ� �׸���
}

// ���� �ǰ� ó�� �Լ�
void MonsterHitPlayer()
{
	CrabHitPlayer();
	SmallFishHitPlayer();
	BigFishHitPlayer();
}



void PlayerHitMonster() {
	PlayerHitBigFish();
	PlayerHitSmallFish();
	PlayerHitCrab();
}