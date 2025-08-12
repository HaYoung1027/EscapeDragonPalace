#include "crab.h"
#include "Rabbit.h"

// ���� ����
Monster g_CrabMon;	// �ɰ� ���� ����ü ���� ����
Skill g_CrabSkill;	// �ɰ� ��ų ����ü ���� ����
Crab g_CrabList[STAGE_CNT][CRAB_CNT];	// �ɰ� ����Ʈ �迭
int g_CrabListIdx[STAGE_CNT] = { 0, };	// �� �� �ɰ� ��

static int bleedCnt = 0;	// ���� Ƚ��
static unsigned long lastBleedTick = 0;	// ������ ���� �ð�

// �ɰ� �׸���
void DrawCrab()
{
	// ���� �� ������ �ӽ÷� �ҷ�����
	Crab* tempCrab = g_CrabList[GetMapStatus()];

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		if (!tempCrab[idx].mon.alive) continue;


		// �����ð� ������ �ǰ� ���� ����
		if (tempCrab[idx].mon.isDamaged && GetTickCount() - tempCrab[idx].mon.lastHitTime >= MONSTER_INVINCIBLE_TIME)
			tempCrab[idx].mon.isDamaged = false;

		// �ǰ� �� �����, ��� ������
		_SetColor(tempCrab[idx].mon.isDamaged ? E_Yellow : E_BrightRed);

		int posX = tempCrab[idx].pos.x - GetPlusX();

		for (int y = 0; y < CRAB_HEIGHT; y++)
		{
			for (int x = 0; x < CRAB_WIDTH; x++)
			{
				// ȭ�� ������ �Ѿ ��� �׸��� �ʱ�
				if (0 > posX + x || SCREEN_WIDTH <= posX + x) continue;

				_DrawText(posX + x, tempCrab[idx].pos.y + y, (char[]) { g_CrabGraphic[0][y][x], 0 });
			}
		}
	}
}

// �ɰ� ������Ʈ
void UpdateCrab()
{
	// ���� �� ������ �ӽ÷� �ҷ�����
	Crab* tempCrab = g_CrabList[GetMapStatus()];

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		// ���� ������� ������ �Ѿ��
		if (!tempCrab[idx].mon.alive) continue;

		// TODO: �߷� �ۿ� �ڵ� �ۼ�

		// �÷��̾ ������ �ִ��� Ȯ��
		// �ɰ� �� �� ���� 10�� �� �ν� (���밪 ���)
		// �ν� �� 20 �Ѿ�� ��׷� Ǯ��
		int playerMiddlePosX = player.Pos.x + RABBIT_WIDTH - 9;	// �÷��̾� X��ǥ �߰���
		int crabMiddlePosX = tempCrab[idx].pos.x - CRAB_WIDTH / 2;	// �ɰ� X��ǥ �߰���

		// �÷��̾�, �ɰ� �Ÿ� üũ (���밪)
		int distanceAbsX = abs(crabMiddlePosX - GetPlusX() - playerMiddlePosX);
		int distanceAbsY = abs(tempCrab[idx].pos.y - player.Pos.y);

		// ��׷� ���� ���� ������
		if (distanceAbsX <= AGGRO_X && distanceAbsY <= AGGRO_Y)
		{
			tempCrab[idx].isChase = true;	// �÷��̾� �߰�
		}
		
		// ��׷� ������ �����
		if (distanceAbsX > AGGRO_OFF_X || (distanceAbsY > AGGRO_Y && !player.IsJumping))
		{
			// �߰� ���� ��׷ΰ� Ǯ���� ���
			if(tempCrab[idx].isChase)
			{
				tempCrab[idx].dir = !tempCrab[idx].dir;	// ���� ���� �ݴ�� �̵�
			}
			tempCrab[idx].isChase = false;	// �÷��̾� �߰� ����
		}

		// �߰ݻ����� �� �̵�
		if (tempCrab[idx].isChase)
		{
			// �ɰ԰� �÷��̾�� �����ʿ� ���� ��
			if (playerMiddlePosX + 5 < crabMiddlePosX - GetPlusX())
			{
				tempCrab[idx].dir = E_Left;
			}
			// �÷��̾�� ���ʿ� ���� ��
			else if(playerMiddlePosX - 5 > crabMiddlePosX - GetPlusX())
			{
				tempCrab[idx].dir = E_Right;
			}
			// -> ��5: ������ �ν��ϴ� ���� �ø���
		}
		else
		{
			// �Ϲ� �̵�: ������ ���� �ȿ��� �̵�
			// TODO: ��׷� Ǯ���� �� Ǯ�� �� �ڸ����� �����̴� �� �����غ���
			if (tempCrab[idx].pos.x <= tempCrab[idx].startPosX)
			{
				tempCrab[idx].dir = E_Right;
			}
			else if (tempCrab[idx].pos.x + CRAB_WIDTH >= tempCrab[idx].startPosX + tempCrab[idx].moveNum)
			{
				tempCrab[idx].dir = E_Left;
			}
		}

		// �ɰ� �̵�
		tempCrab[idx].pos.x += (tempCrab[idx].dir == E_Right) 
			? tempCrab[idx].mon.speed 
			: -tempCrab[idx].mon.speed;
	}

}

// �÷��̾� -> �ɰ� ���� ó��
void PlayerHitCrab()
{

}

// �ɰ� -> �÷��̾� ���� ó��
void CrabHitPlayer()
{
	if (player.isBleeding) return;	//���� ���¸� ������ X

	unsigned int now = _GetTickCount();

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		Crab* tempCrab = &g_CrabList[GetMapStatus()][idx];
		int posX = tempCrab[idx].pos.x - GetPlusX();
		int posY = tempCrab[idx].pos.y;
		Rect PlayerPos = GetPlayerRect();
		Rect CrabPos = { posX + 3, posY, 1, CRAB_WIDTH };

		// �ɰ԰� ������� ������ �Ѿ��
		if (!tempCrab[idx].mon.alive) continue;

		// �ɰԿ� �浹�Ǿ��� ��, �������°� �ƴ� �� ��ų ��� �� ���� ���·� ��ȯ
		if (IsOverlap(CrabPos, PlayerPos))
		{
			tempCrab[idx].skill.isAttack = true;
		}

		// �ɰ԰� ���� ���¸� �÷��̾� ���� ó��
		if (tempCrab[idx].skill.isAttack)
		{
			SetInvincibleTime(true);	// �÷��̾� ���� ����
			player.lastHitTime = GetTickCount();	// �÷��̾� ������ �ǰ� �ð� ����

			// �÷��̾ ������ �ƴ� ��� �ǰݴ����� ��
			if (now - lastBleedTick >= INVINCIBLE_TIME)
			{
				--player.Health;		// �÷��̾� HP ���� (�� ĭ)
				++bleedCnt;				// ���� Ƚ�� ����
				lastBleedTick = now;	// ������ ���� �ð� ����

				// ���� Ƚ���� 3���� �Ѿ�� ���
				if (bleedCnt >= tempCrab[idx].skill.attackCnt)
				{
					SetInvincibleTime(false);	// �÷��̾� ���� ����
					tempCrab[idx].skill.isAttack = false;	// �ɰ� ���� ���� ����
					bleedCnt = 0;	// ���� Ƚ�� �ʱ�ȭ
				}
			}
		}
	}
}

// �÷��̾� ���� ó��
void BleedingPlayer() 
{
}

void ResetCrab() {}   // �ɰ� ���� �ʱ�ȭ

// �ɰ� �ʱ�ȭ
void InitCrab()
{
	// �ɰ� ����� �Ӽ� ����
	g_CrabMon = (Monster)
	{
		.alive = true,		// ���� ����
		.hp = 5,			// ü��
		.isDamaged = false,	// �ǰ� ���� (���� ����)
		.lastHitTime = 0,	// ������ �ǰ� �ð�
		.speed = 0.8,		// �̵� �ӵ�
	};
	g_CrabSkill = (Skill)
	{
		.isAttack = false,	// ��ų ��� ����
		.attack = 1,		// ���ݷ� (�� ĭ)
		.attackCnt = 3,	// ��ų Ƚ��
		.coolTime = 4000,	// ��Ÿ�� (4��)
	};

	// ����
	g_CrabList[E_Jail][g_CrabListIdx[E_Jail]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 95,		// X ��ǥ
		.pos.y = 21,		// Y ��ǥ
		.startPosX = 95,	// �ʱ� X ��ǥ
		.moveNum = 64,		// �̵� ����
		.dir = E_Right,		// �̵� ����
		.attackStatus = E_NONE,	// �ɰ� ���� ����
	};

	g_CrabList[E_Jail][g_CrabListIdx[E_Jail]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 170,
		.pos.y = 21,
		.startPosX = 170,
		.moveNum = 64,
		.dir = E_Right,
		.attackStatus = E_NONE,	// �ɰ� ���� ����
	};

	g_CrabList[E_Jail][g_CrabListIdx[E_Jail]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 277,
		.pos.y = 21,
		.startPosX = 277,
		.moveNum = 92,
		.dir = E_Right,
		.attackStatus = E_NONE,	// �ɰ� ���� ����
	};

	// ���
	g_CrabList[E_DragonPalace][g_CrabListIdx[E_DragonPalace]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 72,
		.pos.y = 21,
		.startPosX = 72,
		.moveNum = 60,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_DragonPalace][g_CrabListIdx[E_DragonPalace]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 170,
		.pos.y = 21,
		.startPosX = 170,
		.moveNum = 61,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_DragonPalace][g_CrabListIdx[E_DragonPalace]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 246,
		.pos.y = 21,
		.startPosX = 246,
		.moveNum = 86,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_DragonPalace][g_CrabListIdx[E_DragonPalace]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 440,
		.pos.y = 21,
		.startPosX = 440,
		.moveNum = 125,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	// �ٴ�1
	g_CrabList[E_Sea1][g_CrabListIdx[E_Sea1]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 80,
		.pos.y = 13,
		.startPosX = 80,
		.moveNum = 96,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea1][g_CrabListIdx[E_Sea1]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 187,
		.pos.y = 21,
		.startPosX = 187,
		.moveNum = 75,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea1][g_CrabListIdx[E_Sea1]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 373,
		.pos.y = 21,
		.startPosX = 373,
		.moveNum = 73,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea1][g_CrabListIdx[E_Sea1]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 574,
		.pos.y = 17,
		.startPosX = 574,
		.moveNum = 60,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea1][g_CrabListIdx[E_Sea1]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 580,
		.pos.y = 7,
		.startPosX = 580,
		.moveNum = 47,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	// �ٴ�2
	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 76,
		.pos.y = 10,
		.startPosX = 76,
		.moveNum = 76,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 150,
		.pos.y = 21,
		.startPosX = 150,
		.moveNum = 41,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 404,
		.pos.y = 21,
		.startPosX = 404,
		.moveNum = 50,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 546,
		.pos.y = 21,
		.startPosX = 491,
		.moveNum = 55,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 491,
		.pos.y = 16,
		.startPosX = 491,
		.moveNum = 55,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};

	g_CrabList[E_Sea2][g_CrabListIdx[E_Sea2]++] = (Crab)
	{
		.mon = g_CrabMon,
		.skill = g_CrabSkill,
		.pos.x = 610,
		.pos.y = 21,
		.startPosX = 610,
		.moveNum = 62,
		.dir = E_Right,
		.attackStatus = E_NONE,
	};
}