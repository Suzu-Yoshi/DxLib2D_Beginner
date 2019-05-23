/*--+----1----+----2----+----3----+----4----+----5--//-+----6----+----7----+----8----+----9----+---*/
/* 03.cpp     																					   */

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

#define GAME_FPS_SPEED					   60

//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

//########## 列挙型 ##########
enum GAME_SCENE {
	GAME_SCENE_TITLE,	//タイトル画面
	GAME_SCENE_PLAY,	//プレイ画面
	GAME_SCENE_END		//エンド画面
};

//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

//########## グローバル変数 ##########
//FPS関連
int StartTimeFps;						//測定開始時刻
int CountFps;							//カウンタ
float CalcFps;							//計算結果
int SampleNumFps = GAME_FPS_SPEED;		//平均を取るサンプル数

//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

//キーボード関連
char AllKeyState[256];			//すべてのキーの状態が入る

//シーン関連
int GameSceneNow = (int)GAME_SCENE_TITLE;	//最初のゲーム画面をタイトルに設定

//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

//########## プロトタイプ宣言 ##########
VOID MY_FPS_UPDATE(VOID);			//FPS値を計測、更新する関数
VOID MY_FPS_DRAW(VOID);				//FPS値を描画する関数
VOID MY_FPS_WAIT(VOID);				//FPS値を計測し、待つ関数

//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//キーの入力状態を更新する関数

VOID MY_GAME_TITLE(VOID);			//タイトル画面の関数
VOID MY_GAME_PLAY(VOID);			//プレイ画面の関数
VOID MY_GAME_END(VOID);				//エンド画面の関数

//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

//########## プログラムで最初に実行される関数 ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(GAME_WINDOW_MODECHANGE);					//ウィンドウモードに設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);			//指定の数値で画面を表示する
	SetWindowStyleMode(SET_WINDOW_ST_MODE_DEFAULT);				//タイトルバーなし
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));					//タイトルの文字

	if (DxLib_Init() == -1) { return -1; }						//ＤＸライブラリ初期化処理

	//■■■■■ プログラム削除ここから ■■■■■■■■■■■■■■■■■■■■
	//int X = 0;
	//int Y = 0;
	//■■■■■ プログラム削除ここまで ■■■■■■■■■■■■■■■■■■■■

	SetDrawScreen(DX_SCREEN_BACK);								//Draw系関数は裏画面に描画

	//無限ループ
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージ処理の結果がエラーのとき、強制終了

		if (ClearDrawScreen() != 0) { break; }	//画面を消去できなかったとき、強制終了

		//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

		MY_ALL_KEYDOWN_UPDATE();				//キーの状態を取得

		//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

		//シーン選択
		switch (GameSceneNow)
		{
		case (int)GAME_SCENE_TITLE:	//タイトル画面の処理ここから

			MY_GAME_TITLE();	//タイトル画面の処理

			break;	//タイトル画面の処理ここまで

		case (int)GAME_SCENE_PLAY:	//プレイ画面の処理ここから

			MY_GAME_PLAY();		//プレイ画面の処理

			break;	//プレイ画面の処理ここまで

		case (int)GAME_SCENE_END:	//エンド画面の処理ここから

			MY_GAME_END();		//エンド画面の処理

			break;	//エンド画面の処理ここまで

		default:

			break;

		}

		MY_FPS_UPDATE();						//FPSの処理[更新]

		MY_FPS_DRAW();		//FPSの処理[描画]

		//■■■■■ プログラム削除ここから ■■■■■■■■■■■■■■■■■■■■
		//DrawString(X, Y, "Hello World", GetColor(255, 255, 255));	//文字を描画

		//X++;	//横に移動
		//Y++;	//縦に移動
		//■■■■■ プログラム削除ここまで ■■■■■■■■■■■■■■■■■■■■

		ScreenFlip();		//モニタのリフレッシュレートの速さで裏画面を再描画

		MY_FPS_WAIT();		//FPSの処理[待つ]
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
		Sleep(waitTime);	//待つ
	}
	return;
}

//▼▼▼▼▼ プログラム追加ここから ▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

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

//########## タイトル画面の関数 ##########
VOID MY_GAME_TITLE(VOID)
{
	if (AllKeyState[KEY_INPUT_RETURN] != 0)	//エンターキーが押されていた時
	{
		GameSceneNow = (int)GAME_SCENE_PLAY;	//シーンをプレイ画面にする
	}

	DrawString(0, 0, "タイトル画面(エンターキーを押してください)", GetColor(255, 255, 255));

	return;
}

//########## プレイ画面の関数 ##########
VOID MY_GAME_PLAY(VOID)
{
	if (AllKeyState[KEY_INPUT_SPACE] != 0)	//スペースーキーが押されていた時
	{
		GameSceneNow = (int)GAME_SCENE_END;	//シーンをエンド画面にする
	}

	DrawString(0, 0, "プレイ画面(スペースキーを押してください)", GetColor(255, 255, 255));

	return;
}

//########## エンド画面の関数 ##########
VOID MY_GAME_END(VOID)
{
	if (AllKeyState[KEY_INPUT_BACK] != 0)	//バックスペースーキーが押されていた時
	{
		GameSceneNow = (int)GAME_SCENE_TITLE;	//シーンをタイトル画面にする
	}

	DrawString(0, 0, "エンド画面(バックスペースキーを押してください)", GetColor(255, 255, 255));

	return;
}

//▲▲▲▲▲ プログラム追加ここまで ▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲