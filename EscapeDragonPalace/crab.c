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
		if (tempCrab[idx].mon.isDamaged && _GetTickCount() - tempCrab[idx].mon.lastHitTime >= MONSTER_INVINCIBLE_TIME)
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

		// �߷� ����
		Crab_ApplyGravity(&tempCrab[idx]);

		// �÷��̾ ������ �ִ��� Ȯ��
		// �ɰ� �� �� ���� 10�� �� �ν� (���밪 ���)
		// �ν� �� 20 �Ѿ�� ��׷� Ǯ��
		int playerMiddlePosX = player.Pos.x + RABBIT_WIDTH - 9;	// �÷��̾� X��ǥ �߰���
		int crabMiddlePosX = tempCrab[idx].pos.x - CRAB_WIDTH / 2;	// �ɰ� X��ǥ �߰���

		// �÷��̾�, �ɰ� �Ÿ� üũ (���밪)
		int distanceAbsX = abs(crabMiddlePosX - GetPlusX() - playerMiddlePosX);
		int distanceAbsY = abs(tempCrab[idx].pos.y - player.Pos.y);

		// TODO: ���߿� �ּ� Ǯ��
		// ��׷� ���� ���� ������
		//if (distanceAbsX <= AGGRO_X && distanceAbsY <= AGGRO_Y)
		//{
		//	tempCrab[idx].isChase = true;	// �÷��̾� �߰�
		//	tempCrab[idx].mon.speed = AGGRO_SPEED;	// �ӵ� �ø���
		//}

		// ��׷� ������ �����
		if (distanceAbsX > AGGRO_OFF_X || (distanceAbsY > AGGRO_Y && !player.IsJumping))
		{
			// �߰� ���� ��׷ΰ� Ǯ���� ���
			if (tempCrab[idx].isChase)
			{
				tempCrab[idx].dir = !tempCrab[idx].dir;	// ���� ���� �ݴ�� �̵�
				tempCrab[idx].mon.speed = CRAB_SPEED;	// ���� �ӵ���� ����
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
			else if (playerMiddlePosX - 5 > crabMiddlePosX - GetPlusX())
			{
				tempCrab[idx].dir = E_Right;
			}
			// -> ��5: ������ �ν��ϴ� ����
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

static bool Crab_IsOnGround(Crab* crab)// �ɰ԰� ���� ���� �ִ��� Ȯ��
{
	// �߾� X��ǥ
	int tileX = (crab->pos.x + (CRAB_WIDTH / 2)) / TILE_SIZE;
	// �߹� YŸ�� ��ǥ
	int tileY = (crab->pos.y + CRAB_HEIGHT) / TILE_SIZE;

	// �� ���� üũ
	if (tileY < 0 || tileY >= MAP_HEIGHT || tileX < 0 || tileX >= MAP_WIDTH)
		return false;

	// ���� �� �޾ƿ���
	int mapStatus = GetMapStatus();

	// �������� ���� Ȯ��
	if (mapStatus >= 0 && mapStatus < STAGE_CNT)
	{
		if (g_StagePlatform[mapStatus][tileY][tileX] == '=')
			return true;
	}

	// �⺻ �� ���� Ȯ��
	if (g_Map[tileY][tileX] == '=')
		return true;

	return false;
}

static void Crab_ApplyGravity(Crab* crab) //�߷� ����
{
	if (!Crab_IsOnGround(crab))// �ɰ԰� ���� ���� ���� ������ �߷� ����
	{
		// ���� ������ ����
		crab->pos.y += 1.0f;

		// ȭ�� �ϴ� ���� ����
		if (crab->pos.y > SCREEN_HEIGHT - CRAB_HEIGHT - 1) //-1 ���ϸ� �ȿ� ��
			crab->pos.y = SCREEN_HEIGHT - CRAB_HEIGHT - 1;
	}
	else
	{
		// ���� ���� ����
		int tileY = (crab->pos.y + CRAB_HEIGHT) / TILE_SIZE;
		crab->pos.y = (tileY * TILE_SIZE) - CRAB_HEIGHT;
	}
}

// �÷��̾� -> �ɰ� ���� ó��
void PlayerHitCrab()
{
	Crab* tempCrab = g_CrabList[GetMapStatus()];	// �ɰ� �ӽ� ���� ��������
	unsigned long now = _GetTickCount();	// ���� �ð� ��������
	Rect weaponPos = GetWeaponRect();	// ���� ��ġ ���� ��������

	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		// ���� ��ġ ����
		int posX = tempCrab[idx].pos.x - GetPlusX();
		int posY = tempCrab[idx].pos.y;
		Rect monsterPos = { posX, posY, CRAB_WIDTH, CRAB_HEIGHT };

		if (!player.IsAttacking // �÷��̾ ���� ���� �ƴ� ���
			|| !tempCrab[idx].mon.alive	// ���Ͱ� �׾��� ���
			|| tempCrab[idx].mon.isDamaged) continue;	// ���Ͱ� ���� ������ ��� �Ѿ��

		// �浹 �� �ɰ� ������ ó��
		if (IsOverlap(weaponPos, monsterPos))
		{
			// �ɰ� ���� �ð��� �� ������ ��� �Ѿ��
			if (now - tempCrab[idx].mon.lastHitTime < MONSTER_INVINCIBLE_TIME) continue;

			tempCrab[idx].mon.hp -= player.HeldWeapon->attack;	// �ɰ� ü�� ����
			tempCrab[idx].mon.isDamaged = true;		// ���� ���� ����
			tempCrab[idx].mon.lastHitTime = now;	// ������ �ǰݴ��� �ð� ����

		}

		// �ɰ� ü���� 0 ������ ��
		if (tempCrab[idx].mon.hp <= 0)
		{
			tempCrab[idx].mon.alive = false;	// ��� ó��
		}
	}
}

// �ɰ� -> �÷��̾� ���� ó��
void CrabHitPlayer()
{
	//if (player.isBleeding) return;	//���� ���¸� ������ X

	//unsigned int now = _GetTickCount();

	//for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	//{
	//	Crab* tempCrab = g_CrabList[GetMapStatus()];
	//	// �ɰ� ��ġ��
	//	int posX = tempCrab[idx].pos.x - GetPlusX();
	//	int posY = tempCrab[idx].pos.y;
	//	Rect PlayerPos = GetPlayerRect();
	//	Rect CrabPos = { posX + 3, posY, CRAB_WIDTH, CRAB_HEIGHT };

	//	// �ɰ԰� ������� ������ �Ѿ��
	//	if (!tempCrab[idx].mon.alive) continue;

	//	// �ɰԿ� �浹�Ǿ��� ��, �������°� �ƴ� ��
	//	// �� �ɰ� ��ų ��Ÿ���� ������ �� ��ų ���
	//	if (IsOverlap(CrabPos, PlayerPos)
	//		&& now - tempCrab[idx].skill.coolTime >= tempCrab[idx].skill.coolTime)
	//	{
	//		tempCrab[idx].skill.isAttack = true;
	//	}

	//	// �ɰ԰� ���� ���¸� �÷��̾� ���� ó��
	//	if (tempCrab[idx].skill.isAttack)
	//	{
	//		SetInvincibleTime(true);	// �÷��̾� ���� ����
	//		player.lastHitTime = _GetTickCount();	// �÷��̾� ������ �ǰ� �ð� ����

	//		// �÷��̾ ������ �ƴ� ��� �ǰݴ����� ��
	//		if (now - lastBleedTick >= INVINCIBLE_TIME)
	//		{
	//			--player.Health;		// �÷��̾� HP ���� (�� ĭ)
	//			++bleedCnt;				// ���� Ƚ�� ����
	//			lastBleedTick = now;	// ������ ���� �ð� ����

	//			// ���� Ƚ���� 3���� �Ѿ�� ���
	//			if (bleedCnt >= tempCrab[idx].skill.attackCnt)
	//			{
	//				SetInvincibleTime(false);	// �÷��̾� ���� ����
	//				tempCrab[idx].skill.isAttack = false;	// �ɰ� ���� ���� ����
	//				tempCrab[idx].skill.coolTime = CRAB_SKILL_COOLTIME;	// �ɰ� ��Ÿ�� �ʱ�ȭ
	//				bleedCnt = 0;	// ���� Ƚ�� �ʱ�ȭ
	//			}
	//		}
	//	}
	//}
}

// �ɰ� ���� �ʱ�ȭ 
void ResetCrab()
{
	Crab* tempCrab = g_CrabList[GetMapStatus()];
	for (int idx = 0; idx < g_CrabListIdx[GetMapStatus()]; idx++)
	{
		tempCrab[idx].mon.alive = false;
	}
}

// �ɰ� �ʱ�ȭ
void InitCrab()
{
	// �ɰ� ����� �Ӽ� ����
	g_CrabMon = (Monster)
	{
		.alive = true,			// ���� ����
		.hp = 5,				// ü��
		.isDamaged = false,		// �ǰ� ���� (���� ����)
		.lastHitTime = 0,		// ������ �ǰ� �ð�
		.speed = CRAB_SPEED,	// �̵� �ӵ�
	};
	g_CrabSkill = (Skill)
	{
		.isAttack = false,	// ��ų ��� ����
		.attack = 1,		// ���ݷ� (�� ĭ)
		.attackCnt = 3,		// ��ų Ƚ��
		.coolTime = CRAB_SKILL_COOLTIME,	// ��ų ��Ÿ��
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
		.startPosX = 83,
		.moveNum = 90,
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
		.moveNum = 56,
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
		.moveNum = 44,
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