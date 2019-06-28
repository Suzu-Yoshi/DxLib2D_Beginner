/*--+----1----+----2----+----3----+----4----+----5--//-+----6----+----7----+----8----+----9----+---*/
/* 01.cpp     																					   */
/* �͂��߂Ă�DxLib    																			   */

//���������� �v���O�����ǉ��������� ����������������������������������������

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

//########## �v���O�����ōŏ��Ɏ��s�����֐� ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(GAME_WINDOW_MODECHANGE);					//�E�B���h�E���[�h�ɐݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);			//�w��̐��l�ŉ�ʂ�\������
	SetWindowStyleMode(SET_WINDOW_ST_MODE_DEFAULT);				//�^�C�g���o�[�Ȃ�
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));					//�^�C�g���̕���

	if (DxLib_Init() == -1) { return -1; }						//�c�w���C�u��������������

	int DrawX = 0;	//�\���ʒuX
	int DrawY = 0;	//�\���ʒuY

	DrawString(DrawX, DrawY, "Hello World", GetColor(255, 255, 255));	//������`��

	WaitKey();			//�L�[���͑҂�

	DxLib_End();		//�c�w���C�u�����g�p�̏I������

	return 0;
}

//���������� �v���O�����ǉ������܂� ����������������������������������������