/*--+----1----+----2----+----3----+----4----+----5--//-+----6----+----7----+----8----+----9----+---*/
/* 02.cpp     																					   */
/* FPS(Flame Per Second：１秒毎のフレーム数)の設定を行う										   */
/* FPS(Flame Per Second：の速さが上手く定まらない場合は、以下の３つを確認しよう！				   */
/* モニタのリフレッシュレートのほうが低い→仕方がないことです。諦めましょう。					   */
/* プロジェクトのプロパティで、デバッグ方法をMtd / MTにしてない→マルチスレッド					   */
/* SetDrawScreen(DX_SCREEN_BACK);を記述し忘れている→ダブルバッファリングすることで、素早い描画が可*/

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

//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

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

//########## プロトタイプ宣言 ##########
VOID MY_FPS_UPDATE(VOID);			//FPS値を計測、更新する関数
VOID MY_FPS_DRAW(VOID);				//FPS値を描画する関数
VOID MY_FPS_WAIT(VOID);				//FPS値を計測し、待つ関数
VOID MY_PLAY_DRAW(VOID);			//ゲーム画面を描画

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

	//■■■■■ プログラム削除ここから ■■■■■■■■■■■■■■■■■■■■
	//int DrawX = 0;	//表示位置X
	//int DrawY = 0;	//表示位置Y
	//DrawString(DrawX, DrawY, "Hello World", GetColor(255, 255, 255));	//文字を描画
	//■■■■■ プログラム削除ここまで ■■■■■■■■■■■■■■■■■■■■

	//無限ループ
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージ処理の結果がエラーのとき、強制終了
		if (ClearDrawScreen() != 0) { break; }	//画面を消去できなかったとき、強制終了
		
		MY_FPS_UPDATE();	//FPSの処理[更新]

		DrawX++;	//位置を移動
		DrawY++;	//位置を移動

		MY_PLAY_DRAW();		//ゲーム画面を描画

		MY_FPS_DRAW();		//FPSの処理[描画]

		ScreenFlip();		//モニタのリフレッシュレートの速さで裏画面を再描画
		
		MY_FPS_WAIT();		//FPSの処理[待つ]
	}

	//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

	DxLib_End();		//ＤＸライブラリ使用の終了処理

	return 0;
}

//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

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
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255),"FPS:%.1f", CalcFps);
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

//########## FPS値を描画する関数 ##########
VOID MY_PLAY_DRAW(VOID)
{
	//文字列を描画
	DrawString(DrawX, DrawY, "Hello World", GetColor(255, 255, 255));	//文字を描画
	return;
}

//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲