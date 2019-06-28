/*--+----1----+----2----+----3----+----4----+----5-----+----6----+----7----+----8----+----9----+---*/
/* 03.cpp     																					   */
/* �L�[���͂̒ǉ�																				   */

//########## �w�b�_�[�t�@�C���ǂݍ��� ##########
#include "DxLib.h"

//########## �}�N����` ##########
#define GAME_WIDTH	800		//��ʂ̉��̑傫��
#define GAME_HEIGHT	600		//��ʂ̏c�̑傫��
#define GAME_COLOR	32		//��ʂ̃J���[�r�b�g

#define GAME_WINDOW_NAME	"GAME TITLE"		//�E�B���h�E�̃^�C�g��
#define GAME_WINDOW_MODECHANGE	TRUE			//TRUE�F�E�B���h�E���[�h / FALSE�F�t���X�N���[��

#define SET_WINDOW_ST_MODE_DEFAULT			0	//�f�t�H���g
#define SET_WINDOW_ST_MODE_TITLE_NONE		1	//�^�C�g���o�[�Ȃ�
#define SET_WINDOW_ST_MODE_TITLE_FLAME_NONE	2	//�^�C�g���o�[�ƃt���[���Ȃ�
#define SET_WINDOW_ST_MODE_FLAME_NONE		3	//�t���[���Ȃ�

#define GAME_FPS_SPEED					   60	//FPS�̐ݒ�

//########## �O���[�o���ϐ� ##########
//FPS�֘A
int StartTimeFps;						//����J�n����
int CountFps;							//�J�E���^
float CalcFps;							//�v�Z����
int SampleNumFps = GAME_FPS_SPEED;		//���ς����T���v����

//�����̈ʒu
int DrawX = 0;	//�\���ʒuX
int DrawY = 0;	//�\���ʒuY

//���������� �v���O�����ǉ��������� ����������������������������������������

char DrawMojiRetsu[64] = { '\0' };	//�`�悷�镶����
int DrawWidth = 0;					//������̕����擾
int DrawHeight = 0;					//�t�H���g�̃T�C�Y(������̍���)���擾

//�L�[��Ԃ��擾
char AllKeyState[256];			//���ׂẴL�[�̏�Ԃ�����

//���������� �v���O�����ǉ������܂� ����������������������������������������

//########## �v���g�^�C�v�錾 ##########
VOID MY_FPS_UPDATE(VOID);			//FPS�l���v���A�X�V����֐�
VOID MY_FPS_DRAW(VOID);				//FPS�l��`�悷��֐�
VOID MY_FPS_WAIT(VOID);				//FPS�l���v�����A�҂֐�
VOID MY_PLAY_DRAW(VOID);			//�Q�[����ʂ�`��


//���������� �v���O�����ǉ��������� ����������������������������������������

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//�L�[�̓��͏�Ԃ��X�V����֐�
VOID MY_PLAY_DRAW_INFO(VOID);		//�Q�[����ʂ̏���`��
VOID MY_PLAY_OPERATION(VOID);		//�Q�[����ʂł̑���

//���������� �v���O�����ǉ������܂� ����������������������������������������

//########## �v���O�����ōŏ��Ɏ��s�����֐� ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(GAME_WINDOW_MODECHANGE);					//�E�B���h�E���[�h�ɐݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);			//�w��̐��l�ŉ�ʂ�\������
	SetWindowStyleMode(SET_WINDOW_ST_MODE_TITLE_NONE);			//�^�C�g���o�[�Ȃ�
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));					//�^�C�g���̕���

	if (DxLib_Init() == -1) { return -1; }						//�c�w���C�u��������������
	SetDrawScreen(DX_SCREEN_BACK);								//[DxLib_Init()�̌�]����ʂɕ`��(�_�u���o�b�t�@�����O)

	//���������� �v���O�����ǉ��������� ����������������������������������������

	sprintf(DrawMojiRetsu, "Hello World");									//������𐮌`(�R�s�[)
	DrawWidth = GetDrawStringWidth(DrawMojiRetsu, strlen(DrawMojiRetsu));	//������̕����擾
	DrawHeight = GetFontSize();												//�t�H���g�̃T�C�Y(������̍���)���擾
	DrawX = GAME_WIDTH / 2 - DrawWidth / 2;									//��ʂ̒����iX�ʒu�j
	DrawY = GAME_HEIGHT / 2 - DrawHeight / 2;								//��ʂ̒����iY�ʒu�j

	//���������� �v���O�����ǉ������܂� ����������������������������������������

	//�������[�v
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W�����̌��ʂ��G���[�̂Ƃ��A�����I��
		if (ClearDrawScreen() != 0) { break; }	//��ʂ������ł��Ȃ������Ƃ��A�����I��

		//���������� �v���O�����ǉ��������� ����������������������������������������

		MY_ALL_KEYDOWN_UPDATE();				//�����Ă���L�[��Ԃ��擾

		//���������� �v���O�����ǉ������܂� ����������������������������������������

		MY_FPS_UPDATE();	//FPS�̏���[�X�V]

		//���������� �v���O�����폜�������� ����������������������������������������
		//DrawX++;	//�ʒu���ړ�
		//DrawY++;	//�ʒu���ړ�
		//���������� �v���O�����폜�����܂� ����������������������������������������

		//���������� �v���O�����ǉ��������� ����������������������������������������

		MY_PLAY_OPERATION();	//�Q�[����ʂł̑���

		//���������� �v���O�����ǉ������܂� ����������������������������������������

		MY_PLAY_DRAW();			//�Q�[����ʂ�`��

		//���������� �v���O�����ǉ��������� ����������������������������������������

		MY_PLAY_DRAW_INFO();	//�Q�[����ʂ̏���`��	

		//���������� �v���O�����ǉ������܂� ����������������������������������������

		MY_FPS_DRAW();			//FPS�̏���[�`��]

		ScreenFlip();			//���j�^�̃��t���b�V�����[�g�̑����ŗ���ʂ��ĕ`��

		MY_FPS_WAIT();			//FPS�̏���[�҂�]
	}

	DxLib_End();		//�c�w���C�u�����g�p�̏I������

	return 0;
}

//########## FPS�l���v���A�X�V����֐� ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1�t���[���ڂȂ玞�����L��
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60�t���[���ڂȂ畽�ς��v�Z
	{
		int now = GetNowCount();
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}


//########## FPS�l��`�悷��֐� ##########
VOID MY_FPS_DRAW(VOID)
{
	//�������`��
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

//########## FPS�l���v�����A�҂֐� ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//������������
	int waitTime = CountFps * 1000 / GAME_FPS_SPEED - resultTime;	//�҂ׂ�����

	if (waitTime > 0)		//�w���FPS�l���������ꍇ
	{
		Sleep(waitTime);	//�ҋ@
	}
	return;
}

//########## �Q�[����ʂ�`�悷��֐� ##########
VOID MY_PLAY_DRAW(VOID)
{
	DrawString(DrawX, DrawY, "Hello World", GetColor(255, 255, 255));	//������`��

	//���������� �v���O�����ǉ��������� ����������������������������������������

	DrawBox(DrawX, DrawY, DrawX + DrawWidth, DrawY + DrawHeight, GetColor(255, 0, 0), FALSE);	//�l�p��`��(TRUE�͓h��Ԃ�)
	
	//���������� �v���O�����ǉ������܂� ����������������������������������������

	return;
}

//���������� �v���O�����ǉ��������� ����������������������������������������

//########## �Q�[����ʂ̏���`�悷��֐� ##########
VOID MY_PLAY_DRAW_INFO(VOID)
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "X:%04d / Y:%04d ", DrawX, DrawY);		//������`��
	return;
}

//########## �L�[�̓��͏�Ԃ��X�V����֐� ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];			//�ꎞ�I�ɁA���݂̃L�[�̓��͏�Ԃ��i�[����

	GetHitKeyStateAll(TempKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)	//������Ă���L�[�̃L�[�R�[�h�������Ă���Ƃ�
		{
			AllKeyState[i]++;	//������Ă���
		}
		else
		{
			AllKeyState[i] = 0;	//������Ă��Ȃ�
		}
	}
	return;
}

//########## �Q�[����ʂł̑�����s���֐� ##########
VOID MY_PLAY_OPERATION(VOID)
{
	if (AllKeyState[KEY_INPUT_UP] != 0)				//����L�[��������Ă���Ƃ�
	{
		DrawY--;	//��Ɉړ�
	}

	if (AllKeyState[KEY_INPUT_DOWN] != 0)			//�����L�[��������Ă���Ƃ�
	{
		DrawY++;	//���Ɉړ�
	}

	if (AllKeyState[KEY_INPUT_LEFT] != 0)				//�����L�[��������Ă���Ƃ�
	{
		DrawX--;	//���Ɉړ�
	}

	if (AllKeyState[KEY_INPUT_RIGHT] != 0)			//�E���L�[��������Ă���Ƃ�
	{
		DrawX++;	//�E�Ɉړ�
	}

	return;
}

//���������� �v���O�����ǉ������܂� ����������������������������������������