/*--+----1----+----2----+----3----+----4----+----5-----+----6----+----7----+----8----+----9----+---*/
/* 03.cpp     																					   */
/* キー入力の追加																				   */

//########## ヘッダーファイル読み込み ##########
#include "DxLib.h"

//########## マクロ定義 ##########
#define GAME_WIDTH	800		//画面の横の大きさ
#define GAME_HEIGHT	600		//画面の縦の大きさ
#define GAME_COLOR	32		//画面のカラービット

#define GAME_WINDOW_NAME	"GAME TITLE"		//ウィンドウのタイトル
#define GAME_WINDOW_MODECHANGE	TRUE			//TRUE：ウィンドウモード / FALSE：フルスクリーン

#define SET_WINDOW_ST_MODE_DEFAULT			0	//デフォルト
#define SET_WINDOW_ST_MODE_TITLE_NONE		1	//タイトルバーなし
#define SET_WINDOW_ST_MODE_TITLE_FLAME_NONE	2	//タイトルバーとフレームなし
#define SET_WINDOW_ST_MODE_FLAME_NONE		3	//フレームなし

#define GAME_FPS_SPEED					   60	//FPSの設定

//########## グローバル変数 ##########
//FPS関連
int StartTimeFps;						//測定開始時刻
int CountFps;							//カウンタ
float CalcFps;							//計算結果
int SampleNumFps = GAME_FPS_SPEED;		//平均を取るサンプル数

//文字の位置
int DrawX = 0;	//表示位置X
int DrawY = 0;	//表示位置Y

//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

char DrawMojiRetsu[64] = { '\0' };	//描画する文字列
int DrawWidth = 0;					//文字列の幅を取得
int DrawHeight = 0;					//フォントのサイズ(文字列の高さ)を取得

//キー状態を取得
char AllKeyState[256];			//すべてのキーの状態が入る

//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

//########## プロトタイプ宣言 ##########
VOID MY_FPS_UPDATE(VOID);			//FPS値を計測、更新する関数
VOID MY_FPS_DRAW(VOID);				//FPS値を描画する関数
VOID MY_FPS_WAIT(VOID);				//FPS値を計測し、待つ関数
VOID MY_PLAY_DRAW(VOID);			//ゲーム画面を描画


//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//キーの入力状態を更新する関数
VOID MY_PLAY_DRAW_INFO(VOID);		//ゲーム画面の情報を描画
VOID MY_PLAY_OPERATION(VOID);		//ゲーム画面での操作

//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

//########## プログラムで最初に実行される関数 ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(GAME_WINDOW_MODECHANGE);					//ウィンドウモードに設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);			//指定の数値で画面を表示する
	SetWindowStyleMode(SET_WINDOW_ST_MODE_TITLE_NONE);			//タイトルバーなし
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));					//タイトルの文字

	if (DxLib_Init() == -1) { return -1; }						//ＤＸライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);								//[DxLib_Init()の後]裏画面に描画(ダブルバッファリング)

	//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

	sprintf(DrawMojiRetsu, "Hello World");									//文字列を整形(コピー)
	DrawWidth = GetDrawStringWidth(DrawMojiRetsu, strlen(DrawMojiRetsu));	//文字列の幅を取得
	DrawHeight = GetFontSize();												//フォントのサイズ(文字列の高さ)を取得
	DrawX = GAME_WIDTH / 2 - DrawWidth / 2;									//画面の中央（X位置）
	DrawY = GAME_HEIGHT / 2 - DrawHeight / 2;								//画面の中央（Y位置）

	//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

	//無限ループ
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージ処理の結果がエラーのとき、強制終了
		if (ClearDrawScreen() != 0) { break; }	//画面を消去できなかったとき、強制終了

		//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

		MY_ALL_KEYDOWN_UPDATE();				//押しているキー状態を取得

		//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

		MY_FPS_UPDATE();	//FPSの処理[更新]

		//■■■■■ プログラム削除ここから ■■■■■■■■■■■■■■■■■■■■
		//DrawX++;	//位置を移動
		//DrawY++;	//位置を移動
		//■■■■■ プログラム削除ここまで ■■■■■■■■■■■■■■■■■■■■

		//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

		MY_PLAY_OPERATION();	//ゲーム画面での操作

		//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

		MY_PLAY_DRAW();			//ゲーム画面を描画

		//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

		MY_PLAY_DRAW_INFO();	//ゲーム画面の情報を描画	

		//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

		MY_FPS_DRAW();			//FPSの処理[描画]

		ScreenFlip();			//モニタのリフレッシュレートの速さで裏画面を再描画

		MY_FPS_WAIT();			//FPSの処理[待つ]
	}

	DxLib_End();		//ＤＸライブラリ使用の終了処理

	return 0;
}

//########## FPS値を計測、更新する関数 ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1フレーム目なら時刻を記憶
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60フレーム目なら平均を計算
	{
		int now = GetNowCount();
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}


//########## FPS値を描画する関数 ##########
VOID MY_FPS_DRAW(VOID)
{
	//文字列を描画
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

//########## FPS値を計測し、待つ関数 ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//かかった時間
	int waitTime = CountFps * 1000 / GAME_FPS_SPEED - resultTime;	//待つべき時間

	if (waitTime > 0)		//指定のFPS値よりも早い場合
	{
		Sleep(waitTime);	//待機
	}
	return;
}

//########## ゲーム画面を描画する関数 ##########
VOID MY_PLAY_DRAW(VOID)
{
	DrawString(DrawX, DrawY, "Hello World", GetColor(255, 255, 255));	//文字を描画

	//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

	DrawBox(DrawX, DrawY, DrawX + DrawWidth, DrawY + DrawHeight, GetColor(255, 0, 0), FALSE);	//四角を描画(TRUEは塗りつぶし)
	
	//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

	return;
}

//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

//########## ゲーム画面の情報を描画する関数 ##########
VOID MY_PLAY_DRAW_INFO(VOID)
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "X:%04d / Y:%04d ", DrawX, DrawY);		//文字を描画
	return;
}

//########## キーの入力状態を更新する関数 ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];			//一時的に、現在のキーの入力状態を格納する

	GetHitKeyStateAll(TempKey); // 全てのキーの入力状態を得る

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)	//押されているキーのキーコードを押しているとき
		{
			AllKeyState[i]++;	//押されている
		}
		else
		{
			AllKeyState[i] = 0;	//押されていない
		}
	}
	return;
}

//########## ゲーム画面での操作を行う関数 ##########
VOID MY_PLAY_OPERATION(VOID)
{
	if (AllKeyState[KEY_INPUT_UP] != 0)				//上矢印キーが押されているとき
	{
		DrawY--;	//上に移動
	}

	if (AllKeyState[KEY_INPUT_DOWN] != 0)			//下矢印キーが押されているとき
	{
		DrawY++;	//下に移動
	}

	if (AllKeyState[KEY_INPUT_LEFT] != 0)				//左矢印キーが押されているとき
	{
		DrawX--;	//左に移動
	}

	if (AllKeyState[KEY_INPUT_RIGHT] != 0)			//右矢印キーが押されているとき
	{
		DrawX++;	//右に移動
	}

	return;
}

//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲