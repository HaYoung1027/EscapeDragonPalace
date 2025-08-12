#pragma once
#include "monster.h"

// ��ũ��
#define CRAB_WIDTH 10   // �ɰ� �ʺ�
#define CRAB_HEIGHT 3   // �ɰ� ����
#define CRAB_HP 5		// �ɰ� ü��
#define CRAB_ATTACK 1   // �ɰ� ���ݷ�
#define CRAB_CNT 17     // �ɰ� �迭 ��
#define TILE_SIZE 1     //�ٴ� ũ��
#define CRAB_EYE 15     //�ɰ� �ν� ����

#define AGGRO_X 20      // �ɰ� ��׷� ���� X
#define AGGRO_Y 6       // �ɰ� ��׷� ���� Y (�䳢 ���� + ���� ����)
#define AGGRO_OFF_X 38  // �ɰ� ��׷� Ǯ���� ����

#define CRAB_SKILL_COOLTIME 4000  // �ɰ� ��ų ��Ÿ��

// ���� ����
const static char g_CrabGraphic[2][CRAB_HEIGHT][CRAB_WIDTH] = {
    {
        "(\\/) (\\/)",
         "  \\o_o/ ",
         " =(___)= "
    }, // �⺻

    {
        "(|)   (|)",
         "  \\o_o/ ",
         " =(___)= "
    } // ����
};

// �ɰ� ���� ����
typedef enum CrabStatus
{
    E_NONE,     // �⺻
    E_ATTACK,   // ����
} CrabStatus;

// �ɰ� ����ü
typedef struct Crab
{
    Monster mon;		// ���� ����ü ��ø
    Skill skill;		// ��ų ����ü ��ø
    MyPoint pos;		// ��ġ
    float startPosX;	// ���� x ��ġ
    CrabStatus attackStatus; // ���� ���� ���� (�׷���)
    Direction dir;      // �̵� ����
    int moveNum;		// �̵� ����
	bool isChase;       // �ɰ� �߰� ����
} Crab;


// �Լ�
void InitCrab();    // �ʱ�ȭ
void DrawCrab();    // �׸���
void UpdateCrab();  // ������Ʈ
void PlayerHitCrab(); // �÷��̾� ���� ó��
void CrabHitPlayer(); // �÷��̾� ���� ó��
void ResetCrab();   // �ɰ� ���� �ʱ�ȭ