#include "init.h"
#include "item.h"
#include "Rabbit.h"
#include "map.h"
#include "weapon.h"
#include "monster.h"
#include "screens.h"
#include "turtle.h"
#include "fish_big.h"
#include "fish_small.h"

// ===========================================================

void Draw() // ȭ�� �׸���
{
	// ���� ���� ��
	if (!GetGameStart())
	{
		_SetColor(E_White); // ���� �� ����
		GameStartScreen();  // ���ӽ��� ȭ�� ���
		// ���� ����Ʈ ȿ�� (���, ȸ������ �����Ÿ�)
		if (GetTextE())
			_SetColor(E_White); // ���� �� ����
		else
			_SetColor(E_Gray); // ���� �� ����
		_DrawText(26, 22, "�ƹ� Ű�� ���� ���� �����ϱ�");
	}
	else if (!GetControlScreen() && GetGameStart())
	{
		_SetColor(E_White);

		for (int y = 0; y < 21; y++)
		{
			_DrawText(12, 2 + y, Controls[y]);
		}

		if (GetTextE())
			_SetColor(E_White); // ���� �� ����
		else
			_SetColor(E_Gray); // ���� �� ����
		_DrawText(28, 22, "�ƹ� Ű�� ���� �Ѿ��");
	}
	// ���� ���� ��
	else {
		// ���ӿ������� ��
		if (GetIsGameOver())
		{
			_SetColor(E_White); // ���� �� ����
			GameOverScreen();   // ���ӿ��� ȭ�� ���
			// ���� ����Ʈ ȿ�� (���, ȸ������ �����Ÿ�)
			if (GetTextE())
				_SetColor(E_White); // ���� �� ����
			else
				_SetColor(E_Gray); // ���� �� ����
			_DrawText(12, 21, "ESC�� ���� ���� Ȥ�� �ƹ� Ű�� ���� ����ȭ������ ���ư���");
		}
		// �������� Ŭ�������� ��
		else if (StageClear())
		{
			RabbitSCAnim();     // �������� Ŭ���� ȭ�� ���
			_Delay(45);         // �������� Ŭ���� �ִϸ��̼� ������
		}
		// ���� Ŭ�������� �� (�ڶ� ������ ü���� 0 ������ ��)
		else if (GetTurtleHp() <= 0)
		{
			_SetColor(E_White); // ���� �� ����
			GameClearSceen();   // ���� Ŭ���� ȭ�� ���
			// ���� ����Ʈ ȿ�� (���, ȸ������ �����Ÿ�)
			if (GetTextE())
				_SetColor(E_White); // ���� �� ����
			else
				_SetColor(E_Gray); // ���� �� ����
			_DrawText(12, 21, "ESC�� ���� ���� Ȥ�� �ƹ� Ű�� ���� ����ȭ������ ���ư���");


		}
		// �÷��� ���� ��
		else {
			// ���� ���� �� ���� ��
			if (!GetWeaponChosen())
			{
				for (int i = 0; i < NUMWEAPON; i++)
				{
					int isSelected = (i == GetSelectedIndex()); // ���� �������� ����
					DrawWeapon(&weaponList[i], i, isSelected);  // ���� ���� ���, �������� ����� �����
				}
			}
			// ���� ���� �Ϸ� �� ���� �����
			else
			{
				DrawMapBG(); // �� ��� �׸���

				// �������� ������ ������ ������ �� �Ǿ����� ��
				if (!GetMapSetting()) {
					ItemSetting();  // ������ ����
				}

				DrawItem();         // ������ �׸���
				_SetColor(E_White); // ������ �� ���� �ʱ�ȭ

				// �������� ��� �ڶ� �׸���
				if (GetMapStatus() == E_Ground) DrawTurtle();
				// �������� �ƴ� ���(�Ϲ� ��) ���� �׸���
				else DrawMonster();

				_SetColor(E_White); // ���� �� ���� �ʱ�ȭ

				// �÷��̾� �ֺ��� �������� ���� �� �˸����� ���
				if (IsNearItem())
					_DrawText(player.Pos.x, player.Pos.y - 3.f, "e�� ���� ������ �Ա�");

				_DrawText(3, 3, player.HeldWeapon->sprite); // ���� ��ܿ� ���� ������� ���� ��������Ʈ �׸���
				DrawHealth();       // ü�¹� �׸���
				DrawBuffNDebuff();  // �ӵ� ���� �� ����� ���� �ð� ǥ��

				// ���� �� ���� �׸���
				char mapTxt[STR_MAX] = "";	// ���� �� ���� ���ڿ�
				switch (GetMapStatus())
				{
				case E_Jail:
					strcpy(mapTxt, "����");
					break;
				case E_DragonPalace:
					strcpy(mapTxt, "���");
					break;
				case E_Sea1:
					strcpy(mapTxt, "�ٴ�1");
					break;
				case E_Sea2:
					strcpy(mapTxt, "�ٴ�2");
					break;
				case E_Ground:
					strcpy(mapTxt, "����(����)");
					break;
				}
				_DrawText(3, 5, mapTxt);
			}

			// �� �Ʒ��δ� ���� ���� �� ���� ��, ���� ���� �Ϸ� �� ���� ����� ��� �����

			DrawPlayer();   // �÷��̾� �׸���
			_SetColor(E_White); // �÷��̾� �� ���� �ʱ�ȭ

			// ���� ���� �Ϸ� �� �������� ��� �ڶ� ü�� �׸���
			if (GetWeaponChosen() && GetMapStatus() == E_Ground)
				DrawTurtleHP();

			// �� Ʋ �׸���
			DrawMap();
		}
	}
}

// ===========================================================

void Update() // ������Ʈ ����
{
	UpdateMapPos();         // �� ��ġ ������Ʈ
	UpdatePlayer();         // �÷��̾� ������Ʈ
	SetIsNearItem(false);   // �÷��̾ ������ ��ó�� �ִ��� ���� �ʱ�ȭ
	CheckItemPickup();      // ������ �Ծ����� üũ
	UpdateBuffs();          // �ӵ� ���� ���ӽð� üũ �� ���� ó��

	// �������� ��� �ڶ� ������Ʈ
	unsigned long now = _GetTickCount();
	if (GetMapStatus() == E_Ground)
	{
		UpdateTurtle(now);
		PlayerHitTurtle();      // �÷��̾ �ڶ� �����ߴ��� üũ
	}
	// �������� �ƴ� ���(�Ϲ� ��) ���� ������Ʈ
	else {
		UpdateMonster();        // ���� ������Ʈ
		MonsterHitPlayer();     // ���Ͱ� �÷��̾ �����ߴ��� üũ
		PlayerHitMonster();     // �÷��̾ ���͸� �����ߴ��� üũ 
	}
}

// ===========================================================

void Input() // Ű �Է�
{
	g_Key = _GetKey();
}

// ===========================================================

void main()
{
	// �ʱ�ȭ
	_BeginWindow();
	

	while (true)
	{
		InitWeapon(weaponList); // ���� �ʱ�ȭ
		InitPlayer();   // �÷��̾� �ʱ�ȭ
		InitMonster();  // ���� �ʱ�ȭ
		ResetItem();    // ������ alive false ó��
		InitItem();     // ������ �ʱ�ȭ

		unsigned long startTime = _GetTickCount();
		InitTurtle(startTime);  // �ڶ�(����) �ʱ�ȭ

		SetConsoleTitle("���Ż��");

		DrawStartScreen();  // ����ȭ�� �۵� �Լ�
		DrawControlsScreen();
		SelectWeapon();     // ���� ����
		player.HeldWeapon = &weaponList[GetSelectedIndex()];    // �÷��̾� ���� ����

		// ���� ����
		while (true)
		{
			if (!GetGameStart())
			{
				_getch(); // Ű���� ���� �����
			}

			GetInput();
			ItemFrameDelay();   // ������ ��� ȿ��
			Input();            // Ű �Է�
			Update();           // ������Ʈ
			_Invalidate();      // ȭ�� �׸��� (Draw() �Լ� �ڵ� ����)
			_Delay(30);

			// ���ӿ������� ��
			if (GetIsGameOver())
			{
				system("cls");          // ȭ�� �����
				ReturnStartScreen();    // ���ӿ��� ȭ�� �۵� �Լ�
				break;  // ���� ���� ����
			}
			// ���� Ŭ�������� �� (�ڶ� ������ ü���� 0 ������ ��)
			if (GetTurtleHp() <= 0)
			{
				system("cls");          // ȭ�� �����
				DrawGameClearScreen();  // ���� Ŭ���� ȭ�� �۵� �Լ�
				break;  // ���� ���� ����
			}
		}

		_EndWindow();
	}
}