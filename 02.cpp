/*--+----1----+----2----+----3----+----4----+----5--//-+----6----+----7----+----8----+----9----+---*/
/* 02.cpp     																					   */
/* FPS(Flame Per Second�F�P�b���̃t���[����)�̐ݒ���s��										   */
/* FPS(Flame Per Second�F�̑�������肭��܂�Ȃ��ꍇ�́A�ȉ��̂R���m�F���悤�I				   */
/* ���j�^�̃��t���b�V�����[�g�̂ق����Ⴂ���d�����Ȃ����Ƃł��B���߂܂��傤�B					   */
/* �v���W�F�N�g�̃v���p�e�B�ŁA�f�o�b�O���@��Mtd / MT�ɂ��ĂȂ����}���`�X���b�h					   */
/* SetDrawScreen(DX_SCREEN_BACK);���L�q���Y��Ă��遨�_�u���o�b�t�@�����O���邱�ƂŁA�f�����`�悪��*/

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

//���������� �v���O�����ǉ��������� ����������������������������������������

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

//########## �v���g�^�C�v�錾 ##########
VOID MY_FPS_UPDATE(VOID);			//FPS�l���v���A�X�V����֐�
VOID MY_FPS_DRAW(VOID);				//FPS�l��`�悷��֐�
VOID MY_FPS_WAIT(VOID);				//FPS�l���v�����A�҂֐�
VOID MY_PLAY_DRAW(VOID);			//�Q�[����ʂ�`��

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

	//���������� �v���O�����폜�������� ����������������������������������������
	//int DrawX = 0;	//�\���ʒuX
	//int DrawY = 0;	//�\���ʒuY
	//DrawString(DrawX, DrawY, "Hello World", GetColor(255, 255, 255));	//������`��
	//���������� �v���O�����폜�����܂� ����������������������������������������

	//�������[�v
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W�����̌��ʂ��G���[�̂Ƃ��A�����I��
		if (ClearDrawScreen() != 0) { break; }	//��ʂ������ł��Ȃ������Ƃ��A�����I��
		
		MY_FPS_UPDATE();	//FPS�̏���[�X�V]

		DrawX++;	//�ʒu���ړ�
		DrawY++;	//�ʒu���ړ�

		MY_PLAY_DRAW();		//�Q�[����ʂ�`��

		MY_FPS_DRAW();		//FPS�̏���[�`��]

		ScreenFlip();		//���j�^�̃��t���b�V�����[�g�̑����ŗ���ʂ��ĕ`��
		
		MY_FPS_WAIT();		//FPS�̏���[�҂�]
	}

	//���������� �v���O�����ǉ������܂� ����������������������������������������

	DxLib_End();		//�c�w���C�u�����g�p�̏I������

	return 0;
}

//���������� �v���O�����ǉ��������� ����������������������������������������

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
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255),"FPS:%.1f", CalcFps);
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

//########## FPS�l��`�悷��֐� ##########
VOID MY_PLAY_DRAW(VOID)
{
	//�������`��
	DrawString(DrawX, DrawY, "Hello World", GetColor(255, 255, 255));	//������`��
	return;
}

//���������� �v���O�����ǉ������܂� ����������������������������������������