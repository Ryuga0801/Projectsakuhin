#include "DxLib.h"
#include<math.h>
#include<stdlib.h>

#define GAME_WIDTH 800
#define GAME_HEIGHT 600
#define GAME_COLOR 32

#define GAME_WINDOW_NAME "Violence forest"
#define GAME_WINDOW_MODECHANGE TRUE

#define SET_WINDOW_ST_MODE_DEFAULT 0

#define GAME_FPS_SPEED 60

#define GAME_TITLE "gazou\\title.png"
#define GAME_CLEAR "gazou\\clear.png"
#define GAME_OVER  "gazou\\gameover.png"
#define GAME_SOUSA "gazou\\sousa.png"

#define GAME_SENTOU "gazou\\mori.jpg"

#define SENTOU_TEXT "gazou\\textbox.png"

#define GAME_PLAYER "chara\\chara3.png"

#define GAME_ENEMY_PANDA   "chara\\panda.png"
#define GAME_ENEMY_KIRIN   "chara\\kirin.png"
#define GAME_ENEMY_COW	   "chara\\cow.png"
#define GAME_ENEMY_HAMSTER "chara\\hamster.png"
#define GAME_ENEMY_HARI	   "chara\\hari.png"
#define GAME_ENEMY_LION	   "chara\\lion.png"

#define GAME_MAP_PNG			 "map\\mapdata2.png"
#define GAME_MAP_CSV_SAIKASO	 "map\\mapdata_saikaso.csv"
#define GAME_MAP_CSV_WOOD		 "map\\mapdata_ki.csv"
#define GAME_MAP_CSV_KABE		 "map\\mapdata_kabe.csv"
#define GAME_MAP_CSV_NEKKO		 "map\\mapdata_nekko.csv"

#define GAME_MUSIC_TITLE "music\\tsukito_ookami.mp3"
#define GAME_MUSIC_FIELD "music\\Pappa_Parappa.mp3"
#define GAME_MUSIC_BATTLE "music\\sakeDBD.mp3"

#define GAME_MAP_BUN_YOKO_CNT 86	//マップの分割数（横）
#define GAME_MAP_BUN_TATE_CNT 46    //マップの分割数（縦）

#define GAME_MAP_YOKO	100			//マップの数（横）	
#define GAME_MAP_TATE	80			//マップの数（縦）

#define GAME_MAP_YOKO_SIZE 8		//マップの大きさ（横）
#define GAME_MAP_TATE_SIZE 8		//マップの大きさ（縦)

#define GAME_CHARA_BUN_YOKO_CNT 12	//キャラの分割数（横）
#define GAME_CHARA_BUN_TATE_CNT 8	//キャラの分割数（縦）

#define GAME_CHARA_YOKO_SIZE 26		//キャラの大きさ（横）	//ツチノコの場合：48	サーナイトの場合：24
#define GAME_CHARA_TATE_SIZE 26		//キャラの大きさ（縦）	//ツチノコの場合：48	サーナイトの場合：32

#define GAME_CHARA_MOTION_NUM 12	//キャラの画像数

#define GAME_CHARA_KIND_YOKO 3		//キャラの横の種類
#define GAME_CHARA_KIND_TATE 4		//キャラの縦の種類

#define GAME_CHARA_SPEED 3			//キャラのスピード

#define FNT_TANU_NAME TEXT("たぬき油性マジック")

#define GAME_MAP_NOTDOWN_KIND	50		//下に行けないマップの種類
#define GAME_MAP_OKDOWN_KIND	16		//下に行けるマップの種類

#define GAME_MAP_NOTUP_KIND		60		//上に行けないマップの種類

#define GAME_MAP_NOTLEFT_KIND	30	//左に行けないマップの種類
#define GAME_MAP_NOTRIGHT_KIND	30	//右に行けないマップの種類

#define GAME_MAP_GOAL  20		//ゴールのマップの種類

#define GAME_MAP_IDOU_KIND		1	//マップ移動の種類

enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_SOUSA,
	GAME_SCENE_IDOU,
	GAME_SCENE_SENTOU,
	GAME_SCENE_OVER,
	GAME_SCENE_CLEAR
};

enum BATTLE_SCENE
{
	BATTLE_START,
	BATTLE_SENTAKU1,
	BATTLE_SENTAKU2,
	BATTLE_ENEMY_TURN,
	BATTLE_ATTACK_HAZURE,
	BATTLE_PLAYERDOWN,
	BATTLE_ENEMYDOWN,
	BATTLE_PLAYERESCAPE,
	BATTLE_PLAYERNORESCAPE
};

enum CHARA_IMAGE {
	CHARA_KIND_1 = 0,
	CHARA_KIND_2 = 3,
	CHARA_KIND_3 = 6,
	CHARA_KIND_4 = 9,
	CHARA_KIND_5 = 48,
	CHARA_KIND_6 = 51,
	CHARA_KIND_7 = 54,
	CHARA_KIND_8 = 57,
	CHARA_KIND_END = 95
};//キャラの種類の列挙型

char AllKeyState[256];

int GameSceneNow = (int)GAME_SCENE_TITLE;
int BattleSceneNow = (int)BATTLE_START;

int BattleScenebefore;

int ScrollCntYoko = 0;			//スクロールカウンタ（横）

int ScrollDistPlusYoko = 1;	//スクロールする距離（横）

struct STRUCT_GAZOU
{
	int Handle;
	char FilePath[128];
	int X;
	int Y;
	int Height;
	int Width;
	int C_Width;
	int C_Height;
	BOOL IsView;
};

struct STRUCT_MAP
{
	char FilePath[128];	//ファイルのパス
	int Handle[GAME_MAP_BUN_TATE_CNT * GAME_MAP_BUN_YOKO_CNT];	 //画像のハンドル
	int X[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];		 //横の位置
	int Y[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];		 //縦の位置
	int Width[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];	 //横のサイズ
	int Height[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];	 //縦のサイズ
	int C_Width[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];	 //横の中心位置
	int C_Height[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT]; //縦の中心位置

	BOOL charaStopFlag;	//キャラの移動制御フラグ

};//MAP構造体

struct STRUCT_CHARACTOR
{
	char FilePath[128];	//ファイルのパス
	int Handle[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];	 //画像のハンドル
	int X[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];		 //横の位置
	int Y[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];		 //縦の位置
	int Width[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];	 //横のサイズ
	int Height[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];	 //縦のサイズ
	int C_Width[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];	 //横の中心位置
	int C_Height[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT]; //縦の中心位置
};//CHARA構造体

struct STRUCT_PLAYER
{
	int Handle[GAME_CHARA_MOTION_NUM];	//画像のハンドル
	int X;								//横の位置
	int Y;								//縦の位置
	int Width;							//横のサイズ
	int Height;							//縦のサイズ
	int C_Width;						//横の中心位置
	int C_Height;						//縦の中心位置
	int NowHandleNum;					//現在の画像のハンドル
	int NowHandleCnt;					//現在のハンドルカウント
	int NowHandleCntMAX;				//現在のハンドルカウントMAX

	int Speed;							//速度

	int MoveDist;						//移動距離
	int MoveDist_Count;                 //どれだけ移動したか 

	BOOL CanMoveLeft;					//左に行けるか
	BOOL CanMoveRight;					//右に行けるか
	BOOL CanMoveUp;						//上に行けるか
	BOOL CanMoveDown;					//下に行けるか

	BOOL BOSSENGAGE;		//ボスと会敵したか

	int atariX;			//当たり判定のX位置
	int atariY;			//当たり判定のY位置
	int atariWidth;		//当たり判定の幅
	int atariHeight;	//当たり判定の高さ

	RECT atariRect;		//当たり判定の矩形領域

};//PLAYER構造体

struct STRUCT_MUSIC
{
	int Handle;				//音のハンドル 
	char FilePath[128];		//ファイルのパス
	int Playtype;			//音の再生方法
};

typedef STRUCT_GAZOU GAZOU;
typedef STRUCT_MAP MAP;

typedef STRUCT_CHARACTOR CHARA;
typedef STRUCT_PLAYER PLAYER;

typedef STRUCT_MUSIC MUSIC;

//ウィンドウ関係
WNDPROC WndProc;			//ウィンドウプロシージャのアドレス
BOOL IsWM_CREATE = FALSE;	//WM_CREATEが正常に動作したか判定する

GAZOU title;
GAZOU sousa;
GAZOU sentou;
GAZOU over;
GAZOU clear;
GAZOU map;

GAZOU text;

GAZOU panda;
GAZOU kirin;
GAZOU cow;
GAZOU hamster;
GAZOU hari;
GAZOU lion;

MAP MapImage;	//マップの画像

CHARA CharaImage;	//キャラの画像
PLAYER Myplayer;	//プレイヤー

MUSIC MUSIC_TITLE;
MUSIC MUSIC_FIELD;
MUSIC MUSIC_BATTLE;

// プレイヤーのイメージ画像の番号を設定する
int PlayerImageNum[GAME_CHARA_MOTION_NUM] = {		//ツチノコの場合		//サーナイトの場合
	39,40,41,	//上									//39,40,41,	//上		//84,85,86,	//上	
	15,16,17,	//左									//15,16,17,	//左		//60,61,62,	//左	
	3,4,5,	//下										//3,4,5,	//下		//48,49,50,	//下
	27,28,29	//右									//27,28,29	//右		//72,73,74,	//右	

};

int StartTimeFps;
int CountFps;
float CalcFps;
int SampleNumFps = GAME_FPS_SPEED;

int HFontTanu_32;

BOOL GAZOU_LOAD(GAZOU*, int, int, const char*);

BOOL FONT_CREATE(const char*, int*, int, int, int);

BOOL MY_MAP_READ_CSV_NUM_SAIKASO(FILE*, const char*);
BOOL MY_MAP_READ_CSV_NUM_NEKKO(FILE*, const char*);
BOOL MY_MAP_READ_CSV_NUM_KI(FILE*, const char*);
BOOL MY_MAP_READ_CSV_NUM_KABE(FILE*, const char*);

BOOL MY_MAP_LOAD_BUNKATSU(MAP*, int, int, int, int, int, const char*);	//MAPを分割して読み込む設定をする関数

BOOL MY_CHARA_LOAD_BUNKATSU(CHARA*, int, int, int, int, int, const char*);

BOOL MY_INIT_PLAYER(PLAYER*, CHARA, int*, int, int, int);					//プレイヤーを初期化する関数

BOOL MY_MUSIC_LOAD(MUSIC*, const char*);

VOID SENTOU_GAZOU_DRAW(VOID);

VOID GAME_START(VOID);//戦闘画面の始まり
VOID SENTAKU1(VOID);//プレイヤーの選択画面その１
VOID SENTAKU2(VOID);//プレイヤーの選択画面その２
VOID ATTACKMISS(VOID);//攻撃が外れた時の処理
VOID ENEMY_TURN(VOID);//敵の行動
VOID PLAYERDOWN(VOID);//プレイヤーが倒された時の処理
VOID ENEMYDOWN(VOID);//エネミーが倒された時の処理
VOID PLAYERESCAPE(VOID);//プレイヤーが逃げられた時の処理
VOID PLAYERNOESCAPE(VOID);//プレイヤーが逃げられなかった時の処理

VOID ALL_KEYDOWN_UPDATE(VOID);

BOOL IsActiveCheck = FALSE;	//画面のアクティブチェック有効化

int StopActiveStartTime = 0;	//非アクティブ化の開始時間
int StopActiveStopTime = 0;		//非アクティブ化の終了時間
int StopActiveTotalTime = 0;	//非アクティブ化の経過時間

VOID FPS_UPDATE(VOID);
VOID FPS_DRAW(VOID);
VOID FPS_WAIT(VOID);

VOID MY_PLAYER_INITPOSI(VOID);

VOID MY_PLAY_PLAYER_DRAW(VOID);		//プレイヤーを表示する関数

VOID MY_PLAY_PLAYER_OPERATION(VOID);	//プレイヤーを操作する関数

VOID MY_PLAY_MAP_DRAW(VOID);	//マップを表示する関数

VOID MY_PLAY_DRAW(VOID); //マップとプレイヤーを表示する関数

VOID MY_GAME_PLAY(VOID);		//プレイ画面の関数

int MapData_saikaso[GAME_MAP_TATE][GAME_MAP_YOKO];	//マップのデータ
int MapData_saikaso_Init[GAME_MAP_TATE][GAME_MAP_YOKO];//マップのデータ（初期化用）

int MapData_ki[GAME_MAP_TATE][GAME_MAP_YOKO];	//マップのデータ
int MapData_ki_Init[GAME_MAP_TATE][GAME_MAP_YOKO];//マップのデータ（初期化用）

int MapData_kabe[GAME_MAP_TATE][GAME_MAP_YOKO];	//マップのデータ
int MapData_kabe_Init[GAME_MAP_TATE][GAME_MAP_YOKO];//マップのデータ（初期化用）

int MapData_nekko[GAME_MAP_TATE][GAME_MAP_YOKO]; //マップのデータ
int MapData_nekko_Init[GAME_MAP_TATE][GAME_MAP_YOKO];//マップのデータ（初期化用）

int MapData_goal[GAME_MAP_TATE][GAME_MAP_YOKO];//マップのデータ
int MapData_goal_Init[GAME_MAP_TATE][GAME_MAP_YOKO];//マップのデータ（初期化用）

FILE* fp_map_csv;		//ファイルの管理番号（ポインタ≒ハンドル）

int MapNotDownKind[GAME_MAP_NOTDOWN_KIND] = {79,80,81,82,83,84,
											 165,166,167,168,169,170,
											 756,757,842,843,
											 2296,2297,2298,2299,
											 2645,2646,2647,2648,2649,2650};	//下に行けないマップの番号

int MapOKDownKind[GAME_MAP_OKDOWN_KIND] = {159,160,161,162,245,
										   246,247,248,331,332,
										   333,334,417,418,419,420}; //下に行けるマップの番号

int MapNotUpKind[GAME_MAP_NOTUP_KIND] = { 670,671,756,757,842,843,
										  932,933,934,935,936,937,
										 1018,1019,1020,1021,1022,1023,
										 1104,1105,1106,1107,1108,1109,
										 1190,1191,1192,1193,1194,1195,
										 1276,1277,1278,1279,1280,1281,
										 1362,1363,1364,1365,1366,1367,
										 2296,2297,2298,2299,
										 2645,2646,2647,2648,2649,2650 };	//上に行けないマップの番号

int MapNotLeftKind[GAME_MAP_NOTLEFT_KIND] = { 764,765,
											  850,851,
											  936,937,
											  2298,2299,
											  2649,2650 };		//左に行けないマップの番号

int MapNotRightKind[GAME_MAP_NOTRIGHT_KIND] = { 761,762,
											    846,847,
											    932,933,
												2296,2297,
												2645,2646 };	//右に行けないマップの番号

int MapGoalKind[GAME_MAP_GOAL] = { 664,665,
								  749,750,751,752,
								  875,876,877,878,
								  921,922,923,924,
								  1007,1008,1009,1010};//ゴールのマップの番号


RECT rectMap_DownNG[GAME_MAP_TATE][GAME_MAP_YOKO];
RECT rectMap_DownNG_First[GAME_MAP_TATE][GAME_MAP_YOKO];

RECT rectMap_DownOK[GAME_MAP_TATE][GAME_MAP_YOKO];
RECT rectMap_DownOK_First[GAME_MAP_TATE][GAME_MAP_YOKO];

RECT rectMap_LeftNG[GAME_MAP_TATE][GAME_MAP_YOKO];
RECT rectMap_LeftNG_First[GAME_MAP_TATE][GAME_MAP_YOKO];

RECT rectMap_RightNG[GAME_MAP_TATE][GAME_MAP_YOKO];
RECT rectMap_RightNG_First[GAME_MAP_TATE][GAME_MAP_YOKO];

RECT rectMap_UpNG[GAME_MAP_TATE][GAME_MAP_YOKO];
RECT rectMap_UpNG_First[GAME_MAP_TATE][GAME_MAP_YOKO];

RECT rectMap_goal[GAME_MAP_TATE][GAME_MAP_YOKO];
RECT rectMap_goal_First[GAME_MAP_TATE][GAME_MAP_YOKO];

VOID MY_SET_PLAYER_ATARI(PLAYER*);

BOOL MY_CHECK_RECT_ATARI(RECT, RECT);
BOOL MY_CHECK_RECT_ATARI_CHARA_MAP(RECT, RECT[GAME_MAP_TATE][GAME_MAP_YOKO]);//マップとの当たり判定をする関数

int ran=1;	//戦闘画面に入るときのランダム

int textX = 120;//戦闘画面のテキストのX座標
int textY = 460;//戦闘画面のテキストのY座標

int missrand;//攻撃が外れるランダム関数

int escaperand;//プレイヤーが逃げられるかのランダム関数

int PEC = GetColor(255, 0, 255);
int TC = GetColor(0, 0, 0);

int Tama = 10;
int Tama_Max = 10;

int battlerand = 0;

int hunt = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(GAME_WINDOW_MODECHANGE);
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	SetWindowStyleMode(SET_WINDOW_ST_MODE_DEFAULT);
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));

	if (DxLib_Init() == -1) { return -1; }

	SetDrawScreen(DX_SCREEN_BACK);

	if (GAZOU_LOAD(&panda, 0, 0, GAME_ENEMY_PANDA) == FALSE) { return -1; }
	if (GAZOU_LOAD(&kirin, 0, 0, GAME_ENEMY_KIRIN) == FALSE) { return -1; }
	if (GAZOU_LOAD(&cow, 0, 0, GAME_ENEMY_COW) == FALSE) { return -1; }
	if (GAZOU_LOAD(&hamster, 0, 0, GAME_ENEMY_HAMSTER) == FALSE) { return -1; }
	if (GAZOU_LOAD(&hari, 0, 0, GAME_ENEMY_HARI) == FALSE) { return -1; }

	if (GAZOU_LOAD(&title, 0, 0, GAME_TITLE) == FALSE) { return -1; }
	if (GAZOU_LOAD(&sousa, 0, 0, GAME_SOUSA) == FALSE) { return -1; }
	if (GAZOU_LOAD(&over, 0, 0, GAME_OVER) == FALSE) { return -1; }
	if (GAZOU_LOAD(&clear, 0, 0, GAME_CLEAR) == FALSE) { return -1; }

	if (GAZOU_LOAD(&sentou, 0, 0, GAME_SENTOU) == FALSE) { return -1; }
	
	if (GAZOU_LOAD(&text, 0, 0, SENTOU_TEXT) == FALSE) { return -1; }
	
	if (MY_MAP_LOAD_BUNKATSU(&MapImage, GAME_MAP_BUN_TATE_CNT * GAME_MAP_BUN_YOKO_CNT, GAME_MAP_BUN_YOKO_CNT, GAME_MAP_BUN_TATE_CNT, GAME_MAP_YOKO_SIZE, GAME_MAP_TATE_SIZE, GAME_MAP_PNG) == FALSE) { MessageBox(NULL, GAME_MAP_PNG, "NotFound", MB_OK); return -1; }	//MAPを読み込む

	if (MY_CHARA_LOAD_BUNKATSU(&CharaImage, GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT, GAME_CHARA_BUN_YOKO_CNT, GAME_CHARA_BUN_TATE_CNT, GAME_CHARA_YOKO_SIZE, GAME_CHARA_TATE_SIZE, GAME_PLAYER) == FALSE) { MessageBox(NULL, GAME_PLAYER, "NotFound", MB_OK); return -1; }	//CHARAを読み込む
	
	if (MY_INIT_PLAYER(&Myplayer,CharaImage,&PlayerImageNum[0],100,100,4) == FALSE) { MessageBox(NULL, GAME_PLAYER, "NotInitPlayer", MB_OK); return -1; }

	if (MY_MAP_READ_CSV_NUM_SAIKASO(fp_map_csv, GAME_MAP_CSV_SAIKASO) == FALSE) { MessageBox(NULL, GAME_MAP_CSV_SAIKASO, "NotFound", MB_OK); return -1; }	//CSVを読み込む
	if (MY_MAP_READ_CSV_NUM_KI(fp_map_csv, GAME_MAP_CSV_WOOD) == FALSE) { MessageBox(NULL, GAME_MAP_CSV_WOOD, "NotFound", MB_OK); return -1; }	//CSVを読み込む
	if (MY_MAP_READ_CSV_NUM_KABE(fp_map_csv, GAME_MAP_CSV_KABE) == FALSE) { MessageBox(NULL, GAME_MAP_CSV_KABE, "NotFound", MB_OK); return -1; }	//CSVを読み込む
	if (MY_MAP_READ_CSV_NUM_NEKKO(fp_map_csv, GAME_MAP_CSV_NEKKO) == FALSE) { MessageBox(NULL, GAME_MAP_CSV_NEKKO, "NotFound", MB_OK); return -1; }

	MY_MUSIC_LOAD(&MUSIC_TITLE, GAME_MUSIC_TITLE);
	MY_MUSIC_LOAD(&MUSIC_FIELD, GAME_MUSIC_FIELD);
	MY_MUSIC_LOAD(&MUSIC_BATTLE, GAME_MUSIC_BATTLE);

	if (FONT_CREATE(FNT_TANU_NAME, &HFontTanu_32, 32, 1, DX_FONTTYPE_ANTIALIASING_EDGE) == FALSE) { return -1; }

	//画面のアクティブチェック有効化
	IsActiveCheck = TRUE;

	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }

		ALL_KEYDOWN_UPDATE();

		FPS_UPDATE();

		switch (GameSceneNow)
		{
		case(int)GAME_SCENE_TITLE:
			
			if(CheckSoundMem(MUSIC_TITLE.Handle)==0)
			{
				if (CheckSoundMem(MUSIC_FIELD.Handle) == 1)
				{
					StopSoundMem(MUSIC_FIELD.Handle);
				}
				ChangeVolumeSoundMem(255 * 40 / 100, MUSIC_TITLE.Handle);
				PlaySoundMem(MUSIC_TITLE.Handle, DX_PLAYTYPE_LOOP);
			}
			
			DrawGraph(title.X, title.Y, title.Handle, TRUE);

			if (BattleSceneNow != (int)BATTLE_START)
			{
				BattleSceneNow = (int)BATTLE_START;
			}
			
			if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_SOUSA;
			}

			break;

		case(int)GAME_SCENE_SOUSA:
			
			DrawGraph(sousa.X, sousa.Y, sousa.Handle, TRUE);

			if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				MY_PLAYER_INITPOSI();
				GameSceneNow = (int)GAME_SCENE_IDOU;
				Tama = Tama_Max;
			}

			break;

		case(int)GAME_SCENE_IDOU:

			if (CheckSoundMem(MUSIC_FIELD.Handle) == 0)
			{
				if (CheckSoundMem(MUSIC_TITLE.Handle) == 1)
				{
					StopSoundMem(MUSIC_TITLE.Handle);
				}
				ChangeVolumeSoundMem(255 * 40 / 100, MUSIC_FIELD.Handle);
				PlaySoundMem(MUSIC_FIELD.Handle, DX_PLAYTYPE_LOOP,TRUE);
			}

			if (CheckSoundMem(MUSIC_BATTLE.Handle) == 1)
			{
				StopSoundMem(MUSIC_BATTLE.Handle);
			}

			/*if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_CLEAR;
			}

			if (AllKeyState[KEY_INPUT_BACK] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_OVER;
			}*/

			if (Myplayer.MoveDist_Count == 3)
			{
				battlerand = rand() % 90 + 0;
				if (battlerand >= 85)
				{
					for (int i = 0; i < 1; i++)
					{
						ran = rand() % 5 + 1;
					}

					GameSceneNow = (int)GAME_SCENE_SENTOU;
					BattleSceneNow = (int)BATTLE_START;
				}
				Myplayer.MoveDist_Count = 0;
			}

			//MY_PLAY_MAP_DRAW();			//マップを描画
			//MY_PLAY_PLAYER_DRAW();		//プレイヤーを描画
			MY_PLAY_DRAW();
			MY_PLAY_PLAYER_OPERATION();	//プレイヤーを操作

			break;

		case(int)GAME_SCENE_SENTOU:

			SENTOU_GAZOU_DRAW();

			if (CheckSoundMem(MUSIC_BATTLE.Handle) == 0)
			{
				if (CheckSoundMem(MUSIC_FIELD.Handle) == 1)
				{
					StopSoundMem(MUSIC_FIELD.Handle);
				}
				ChangeVolumeSoundMem(255 * 40 / 100, MUSIC_BATTLE.Handle);
				PlaySoundMem(MUSIC_BATTLE.Handle, DX_PLAYTYPE_LOOP, TRUE);
			}

			switch (BattleSceneNow)
			{
			case(int)BATTLE_START:

				GAME_START();

				break;

			case(int)BATTLE_SENTAKU1:

				SENTAKU1();

				break;

			case(int)BATTLE_SENTAKU2:

				SENTAKU2();

				break;

			case(int)BATTLE_ENEMY_TURN:

				ENEMY_TURN();

				break;

			case(int)BATTLE_ATTACK_HAZURE:

				ATTACKMISS();

				break;

			case(int)BATTLE_PLAYERDOWN:

				PLAYERDOWN();

				break;

			case(int)BATTLE_ENEMYDOWN:

				ENEMYDOWN();

				break;

			case(int)BATTLE_PLAYERESCAPE:

				PLAYERESCAPE();

				break;

			case(int)BATTLE_PLAYERNORESCAPE:

				PLAYERNOESCAPE();

				break;

			default:
				break;
			}

			break;

		case(int)GAME_SCENE_OVER:

			DrawGraph(over.X, over.Y, over.Handle, TRUE);

			if (AllKeyState[KEY_INPUT_BACK] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_TITLE;
			}

			if (CheckSoundMem(MUSIC_BATTLE.Handle) == 1)
			{
				StopSoundMem(MUSIC_BATTLE.Handle);
			}

			break;

		case(int)GAME_SCENE_CLEAR:

			DrawGraph(clear.X, clear.Y, clear.Handle, TRUE);

			if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_TITLE;
			}

			if (CheckSoundMem(MUSIC_BATTLE.Handle) == 1)
			{
				StopSoundMem(MUSIC_BATTLE.Handle);
			}

			break;

		default:
			break;
		}
		FPS_DRAW();

		ScreenFlip();

		FPS_WAIT();
	}

	for (int mapCnt = 0; mapCnt < GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT; mapCnt++)
	{
		DeleteGraph(MapImage.Handle[mapCnt]);	//マップのハンドルを削除
	}

	for (int charaCnt = 0; charaCnt < GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT; charaCnt++)
	{
		DeleteGraph(MapImage.Handle[charaCnt]);	//キャラのハンドルを削除
	}

	DeleteSoundMem(MUSIC_TITLE.Handle);
	DeleteSoundMem(MUSIC_FIELD.Handle);
	DeleteSoundMem(MUSIC_BATTLE.Handle);

	DeleteGraph(title.Handle);
	DeleteGraph(sousa.Handle);
	DeleteGraph(over.Handle);
	DeleteGraph(clear.Handle);

	DeleteGraph(sentou.Handle);

	DeleteGraph(text.Handle);

	DeleteGraph(panda.Handle);
	DeleteGraph(kirin.Handle);
	DeleteGraph(cow.Handle);
	DeleteGraph(hamster.Handle);
	DeleteGraph(hari.Handle);
	DeleteGraph(lion.Handle);

	DxLib_End();		//ＤＸライブラリ使用の終了処理

	return 0;
}

VOID SENTOU_GAZOU_DRAW(VOID)
{
	Myplayer.MoveDist_Count = 0;
	DrawGraph(0, 0, sentou.Handle, TRUE);
	DrawGraph(0, 400, text.Handle, TRUE);
	if (ran == 1)
	{
		DrawExtendGraph(280, 200, 465, 350, panda.Handle, TRUE);
	}
	else if (ran == 2)
	{
		DrawExtendGraph(280, 200, 465, 350, kirin.Handle, TRUE);
	}
	else if (ran == 3)
	{
		DrawExtendGraph(280, 200, 465, 350, cow.Handle, TRUE);
	}
	else if (ran == 4)
	{
		DrawExtendGraph(280, 200, 465, 350, hamster.Handle, TRUE);
	}
	else if(ran==5)
	{
		DrawExtendGraph(280, 200, 465, 350, hari.Handle, TRUE);
	}

}

VOID GAME_START(VOID)
{
	SetFontSize(64);
	if (ran == 1)
	{
		DrawString(textX, textY, "パンダが現れた！",PEC);
	}
	else if (ran == 2)
	{
		DrawString(textX, textY, "キリンが現れた！", PEC);
	}
	else if (ran == 3)
	{
		DrawString(textX, textY, "ウシが現れた！", PEC);
	}
	else if (ran == 4)
	{
		DrawString(textX, textY, "ハムスターが現れた！", PEC);
	}
	else if (ran == 5)
	{
		DrawString(textX, textY, "ハリネズミが現れた！", PEC);
	}

	SetFontSize(30);
	DrawString(textX + 450, textY + 100, "Push Enter!", TC);
	if (AllKeyState[KEY_INPUT_RETURN] == 1)
	{
		BattleSceneNow = (int)BATTLE_SENTAKU1;
		escaperand = rand() % 3 + 1;
	}
}

VOID SENTAKU1(VOID)
{
	SetFontSize(25);
	DrawString(0, 405, "君はどうする？", PEC);
	SetFontSize(58);
	DrawString(textX, textY, "1.戦う", PEC);
	DrawString(textX+240, textY, "2.逃げる", PEC);
	SetFontSize(30);
	DrawFormatString(0, textY + 100, TC, "残弾数:%d", Tama);

	if (AllKeyState[KEY_INPUT_1] == 1)
	{
		BattleSceneNow = (int)BATTLE_SENTAKU2;
		missrand = rand() % 10 + 1;
	}

	if (AllKeyState[KEY_INPUT_2] == 1)
	{
		if (escaperand == 3)
		{
			BattleSceneNow = (int)BATTLE_PLAYERNORESCAPE;
		}
		else if(escaperand < 3)
		{
			BattleSceneNow = (int)BATTLE_PLAYERESCAPE;
		}
	}
}

VOID SENTAKU2(VOID)
{
	SetFontSize(25);
	DrawString(0, 405, "君はどうする？", PEC);
	SetFontSize(58);
	DrawString(textX, textY, "1.銃", PEC);
	DrawString(textX + 240, textY, "2.ナイフ", PEC);
	SetFontSize(30);
	DrawFormatString(0, textY + 100, TC, "残弾数:%d", Tama);

	
	if (AllKeyState[KEY_INPUT_1] == 1 )
	{
		Tama--;
		if (missrand >= 8)
		{
			BattleSceneNow = (int)BATTLE_ATTACK_HAZURE;
		}
		else
		{
			BattleSceneNow = (int)BATTLE_ENEMYDOWN;
			hunt += 1;
		}
	}
	
	if (AllKeyState[KEY_INPUT_2] == 1)
	{
		if (missrand == 10)
		{
			BattleSceneNow = (int)BATTLE_ATTACK_HAZURE;
		}
		else
		{
			BattleSceneNow = (int)BATTLE_ENEMYDOWN;
			hunt += 1;
		}
	}

	BattleScenebefore = (int)BATTLE_SENTAKU2;
}

VOID ATTACKMISS(VOID)
{
	DrawString(textX, textY, "しかし、攻撃は外れた！", PEC);
	SetFontSize(30);
	DrawString(textX + 450, textY + 100, "Push Enter!", TC);
	
	if (AllKeyState[KEY_INPUT_RETURN] == 1)
	{
		if (BattleScenebefore == (int)BATTLE_SENTAKU2)
		{
			BattleSceneNow = (int)BATTLE_ENEMY_TURN;
		}
		else if (BattleScenebefore == (int)BATTLE_ENEMY_TURN)
		{
			BattleSceneNow = (int)BATTLE_SENTAKU1;
		}
	}
}

VOID ENEMY_TURN(VOID)
{
	BattleScenebefore = (int)BATTLE_ENEMY_TURN;
	SetFontSize(58);
	if (ran == 1)
	{
		DrawString(textX, textY, "パンダのひっかく！", PEC);
	}
	else if (ran == 2)
	{
		DrawString(textX, textY, "キリンのなぎはらい！", PEC);
	}
	else if (ran == 3)
	{
		DrawString(textX, textY, "ウシのつのでつく！", PEC);
	}
	else if (ran == 4)
	{
		DrawString(textX, textY, "ハムスターはにげだした！", PEC);
	}
	else if (ran == 5)
	{
		DrawString(textX, textY, "ハリネズミはにげだした！", PEC);
	}

	SetFontSize(30);
	DrawString(textX + 450, textY + 100, "Push Enter!", TC);
	if (AllKeyState[KEY_INPUT_RETURN] == 1)
	{
		if (ran >= 4)
		{
			GameSceneNow = (int)GAME_SCENE_IDOU;
		}
		else
		{
			if (missrand >= 6)
			{
				BattleSceneNow = (int)BATTLE_ATTACK_HAZURE;
			}
			else if(missrand<=5)
			{
				BattleSceneNow = (int)BATTLE_PLAYERDOWN;
			}
		}
	}
}

VOID PLAYERDOWN(VOID)
{
	SetFontSize(58);
	DrawString(textX, textY, "君は倒された！", PEC);
	SetFontSize(30);
	DrawString(textX + 450, textY + 100, "Push Enter!", TC);
	if (AllKeyState[KEY_INPUT_RETURN] == 1)
	{
		GameSceneNow = (int)GAME_SCENE_OVER;
	}
}

VOID ENEMYDOWN(VOID)
{
	SetFontSize(58);
	if (ran == 1)
	{
		DrawString(textX, textY, "パンダを倒した！", PEC);
	}
	else if (ran == 2)
	{
		DrawString(textX, textY, "キリンを倒した！", PEC);
	}
	else if (ran == 3)
	{
		DrawString(textX, textY, "ウシを倒した！", PEC);
	}
	else if (ran == 4)
	{
		DrawString(textX, textY, "ハムスターを倒した！", PEC);
	}
	else if (ran == 5)
	{
		DrawString(textX, textY, "ハリネズミを倒した！", PEC);
	}

	SetFontSize(30);
	DrawString(textX + 450, textY + 100, "Push Enter!", TC);
	if (hunt == 10)
	{
		if (AllKeyState[KEY_INPUT_RETURN] == 1)
		{
			GameSceneNow = (int)GAME_SCENE_CLEAR;
		}
	}
	else
	{
		if (AllKeyState[KEY_INPUT_RETURN] == 1)
		{
			GameSceneNow = (int)GAME_SCENE_IDOU;
		}
	}
}

VOID PLAYERESCAPE(VOID)
{
	SetFontSize(58);
	DrawString(textX, textY, "君は逃げられた！", PEC);
	SetFontSize(30);
	DrawString(textX + 450, textY + 100, "Push Enter!", TC);
	if (AllKeyState[KEY_INPUT_RETURN] == 1)
	{
		GameSceneNow = (int)GAME_SCENE_IDOU;
	}
}

VOID PLAYERNOESCAPE(VOID)
{
	SetFontSize(58);
	DrawString(textX-80, textY, "後ろに回り込まれてしまった！", PEC);
	SetFontSize(30);
	DrawString(textX + 450, textY + 100, "Push Enter!", TC);
	if (AllKeyState[KEY_INPUT_RETURN] == 1)
	{
		BattleSceneNow = (int)BATTLE_ENEMY_TURN;
	}
}

VOID ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];

	GetHitKeyStateAll(TempKey);

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)
		{
			AllKeyState[i]++;
		}
		else
		{
			AllKeyState[i] = 0;
		}
	}
	return;
}

VOID FPS_UPDATE(VOID)
{
	if (CountFps == 0)
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps)
	{
		int now = GetNowCount();
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

VOID FPS_DRAW(VOID)
{
	DrawFormatStringToHandle(0, 0, GetColor(0, 200, 255), HFontTanu_32, "FPS:%.1f", CalcFps);
	return;
}

VOID FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;
	int waitTime = CountFps * 1000 / GAME_FPS_SPEED - resultTime;

	if (waitTime > 0)
	{
		Sleep(waitTime);
	}
	return;
}

BOOL FONT_CREATE(const char* name, int* f, int pt, int bold, int type)
{
	*f = CreateFontToHandle(name, pt, bold, type);
	if (*f == -1) { return FALSE; }

	return TRUE;
}


BOOL GAZOU_LOAD(GAZOU* g, int x, int y, const char* path)
{
	wsprintf(g->FilePath, path);
	g->Handle = LoadGraph(g->FilePath);

	if (g->Handle == -1) { return FALSE; }

	GetGraphSize(g->Handle, &g->Width, &g->Height);
	g->X = x;
	g->Y = y;
	g->C_Width = g->Width / 2;
	g->C_Height = g->Height / 2;

	g->IsView = TRUE;

	return TRUE;
}

VOID MY_PLAYER_INITPOSI(VOID)
{
	Myplayer.X = 150;
	Myplayer.Y = 150;
}

////プレイヤーを表示する関数
//VOID MY_PLAY_PLAYER_DRAW(VOID)
//{
//	////引き延ばして描画
//	//DrawExtendGraph(
//	//	Myplayer.X,
//	//	Myplayer.Y,
//	//	Myplayer.X + Myplayer.Width,
//	//	Myplayer.Y + Myplayer.Height + 4,
//	//	Myplayer.Handle[Myplayer.NowHandleNum], TRUE
//	//);
//
//	//普通に描画
//	DrawGraph(
//		Myplayer.X,
//		Myplayer.Y,
//		Myplayer.Handle[Myplayer.NowHandleNum],
//	TRUE
//	);
//
//	DrawFormatString(0, 60, GetColor(255, 255, 255), "プレイヤーX位置: %d", Myplayer.X);
//	DrawFormatString(0, 80, GetColor(255, 255, 255), "プレイヤーY位置: %d", Myplayer.Y);
//
//	/*DrawBox(
//		Myplayer.atariRect.left,
//		Myplayer.atariRect.top,
//		Myplayer.atariRect.right,
//		Myplayer.atariRect.bottom,
//		GetColor(0, 0, 255), FALSE);*/
//
//	/*DrawFormatString(0, 40, GetColor(255, 255, 255), "プレイヤーの動いた距離 : %04d", Myplayer.MoveDist);	//動いた距離を表示
//	DrawFormatString(0, 60, GetColor(255, 255, 255), "プレイヤーX：%04d", Myplayer.X);	//プレイヤーのX位置を表示
//	DrawFormatString(0, 80, GetColor(255, 255, 255), "プレイヤーY：%04d", Myplayer.Y);	//プレイヤーのY位置を表示
//	DrawFormatString(0, 100, GetColor(255, 255, 255), "ジャンプ頂点：%04d", Myplayer.JumpMaxTop);	//プレイヤーのジャンプの頂点*/
//
//	return;
//}

//MAPを分割して読み込む設定をする関数
//引　数：MAP構造体　：設定する画像構造体(分割)の変数
//引　数：int　：画像の分割数
//引　数：int　：画像の横の分割数
//引　数：int　：画像の縦の分割数
//引　数：int　：画像の横の分割サイズ
//引　数：int　：画像の縦の分割サイズ
//引　数：const char *：読み込む画像のファイルパス
//戻り値：BOOL：エラー時：FALSE
BOOL MY_MAP_LOAD_BUNKATSU(MAP* m, int bun_num, int bun_x_num, int bun_y_num, int bun_width, int bun_height, const char* path)
{
	int ret = 0;	//ファイルパスをコピー
	wsprintf(m->FilePath, path);

	//元ネタはこちら　　　：https://brain.cc.kogakuin.ac.jp/~kanamaru/lecture/prog1/11-03.html
	//リファレンスはこちら：https://dxlib.xsrv.jp/function/dxfunc_graph1.html#R3N3

	//画像をメモリに分割して読み込み、ハンドルを取得
	ret = LoadDivGraph(m->FilePath, bun_num, bun_x_num, bun_y_num, bun_width, bun_height, m->Handle);

	if (ret == -1) { return FALSE; }	//画像読み込みエラー

	for (int cnt = 0; cnt < GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT; cnt++)
	{
		//画像サイズ取得
		GetGraphSize(
			m->Handle[cnt],		//ハンドルから、画像の幅と高さを取得
			&m->Width[cnt],		//MAP構造体の幅に、画像の幅を設定する
			&m->Height[cnt]);	//MAP構造体の高さに、画像の高さを設定

		m->C_Width[cnt] = m->Width[cnt] / 2;	//画像の横サイズの中心を取得
		m->C_Height[cnt] = m->Height[cnt] / 2;	//画像の縦サイズの中心を取得
	}
	m->charaStopFlag = FALSE;

	return TRUE;
}

//VOID MY_PLAY_MAP_DRAW(VOID)
//{
//	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
//	{
//		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
//		{
//			if ((yoko + 1) * GAME_MAP_YOKO_SIZE - ScrollCntYoko >= 0 &&
//				(yoko + 0) * GAME_MAP_YOKO_SIZE - ScrollCntYoko <= GAME_WIDTH)	//画面に写っているならばならば
//			{
//				DrawGraph(
//					yoko * GAME_MAP_YOKO_SIZE - ScrollCntYoko,			//位置からスクロール量を引く
//					tate * GAME_MAP_TATE_SIZE,
//					MapImage.Handle[MapData_saikaso[tate][yoko]], TRUE);
//
//				DrawGraph(
//					yoko * GAME_MAP_YOKO_SIZE - ScrollCntYoko,			//位置からスクロール量を引く
//					tate * GAME_MAP_TATE_SIZE,
//					MapImage.Handle[MapData_ki[tate][yoko]], TRUE);
//
//				DrawGraph(
//					yoko * GAME_MAP_YOKO_SIZE - ScrollCntYoko,			//位置からスクロール量を引く
//					tate * GAME_MAP_TATE_SIZE,
//					MapImage.Handle[MapData_kabe[tate][yoko]], TRUE);
//
//			}
//		}
//	}
//}

VOID MY_PLAY_DRAW(VOID)
{
	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			if ((yoko + 1) * GAME_MAP_YOKO_SIZE - ScrollCntYoko >= 0 &&
				(yoko + 0) * GAME_MAP_YOKO_SIZE - ScrollCntYoko <= GAME_WIDTH)	//画面に写っているならばならば
			{
				DrawGraph(
					yoko * GAME_MAP_YOKO_SIZE - ScrollCntYoko,			//位置からスクロール量を引く
					tate * GAME_MAP_TATE_SIZE,
					MapImage.Handle[MapData_saikaso[tate][yoko]], TRUE);
			}
		}
	}

	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			if ((yoko + 1) * GAME_MAP_YOKO_SIZE - ScrollCntYoko >= 0 &&
				(yoko + 0) * GAME_MAP_YOKO_SIZE - ScrollCntYoko <= GAME_WIDTH)	//画面に写っているならばならば
			{
				DrawGraph(
					yoko * GAME_MAP_YOKO_SIZE - ScrollCntYoko,			//位置からスクロール量を引く
					tate * GAME_MAP_TATE_SIZE,
					MapImage.Handle[MapData_nekko[tate][yoko]], TRUE);
			}
		}
	}

	//普通に描画
	DrawGraph(
		Myplayer.X,
		Myplayer.Y,
		Myplayer.Handle[Myplayer.NowHandleNum],
		TRUE
	);

	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			if ((yoko + 1) * GAME_MAP_YOKO_SIZE - ScrollCntYoko >= 0 &&
				(yoko + 0) * GAME_MAP_YOKO_SIZE - ScrollCntYoko <= GAME_WIDTH)	//画面に写っているならばならば
			{
				DrawGraph(
					yoko * GAME_MAP_YOKO_SIZE - ScrollCntYoko,			//位置からスクロール量を引く
					tate * GAME_MAP_TATE_SIZE,
					MapImage.Handle[MapData_ki[tate][yoko]], TRUE);
			}
		}
	}

	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			if ((yoko + 1) * GAME_MAP_YOKO_SIZE - ScrollCntYoko >= 0 &&
				(yoko + 0) * GAME_MAP_YOKO_SIZE - ScrollCntYoko <= GAME_WIDTH)	//画面に写っているならばならば
			{
				DrawGraph(
					yoko * GAME_MAP_YOKO_SIZE - ScrollCntYoko,			//位置からスクロール量を引く
					tate * GAME_MAP_TATE_SIZE,
					MapImage.Handle[MapData_kabe[tate][yoko]], TRUE);
			}
		}
	}
	DrawFormatString(0, textY + 100, TC, "倒した敵の数:%d", hunt);
	//DrawFormatString(0, 30, GetColor(255, 255, 255), "プレイヤーX位置: %d", Myplayer.X);
	//DrawFormatString(0, 50, GetColor(255, 255, 255), "プレイヤーY位置: %d", Myplayer.Y);

	return;
}


//########## CHARAを分割して読み込む設定をする関数 ##########
//引　数：CHARA構造体　：設定する画像構造体(分割)の変数
//引　数：int　：画像の分割数
//引　数：int　：画像の横の分割数
//引　数：int　：画像の縦の分割数
//引　数：int　：画像の横の分割サイズ
//引　数：int　：画像の縦の分割サイズ
//引　数：const char *：読み込む画像のファイルパス
//戻り値：BOOL：エラー時：FALSE
BOOL MY_CHARA_LOAD_BUNKATSU(CHARA* c, int bun_num, int bun_x_num, int bun_y_num, int bun_width, int bun_height, const char* path)
{
	int ret = 0;
	wsprintf(c->FilePath, path);	//ファイルのパスをコピー

	//元ネタはこちら　　　：https://brain.cc.kogakuin.ac.jp/~kanamaru/lecture/prog1/11-03.html
	//リファレンスはこちら：https://dxlib.xsrv.jp/function/dxfunc_graph1.html#R3N3

	//画像をメモリに分割して読み込み、ハンドルを取得
	ret = LoadDivGraph(c->FilePath, bun_num, bun_x_num, bun_y_num, bun_width, bun_height, c->Handle);

	if (ret == -1) { return FALSE; }//画像読み込みエラー

	for (int cnt = 0; cnt < GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT; cnt++)
	{
		//画像サイズを取得
		GetGraphSize(
			c->Handle[cnt],	//ハンドルから、画像の幅と高さを取得
			&c->Width[cnt],	//MAP構造体の幅に、画像の幅を設定する
			&c->Height[cnt]	//MAP構造体の高さに、画像の高さを設定する
		);

		c->C_Width[cnt] = c->Width[cnt] / 2;	//画像の横サイズの中心を取得
		c->C_Height[cnt] = c->Height[cnt] / 2;	//画像の縦サイズの中心を取得
	}
	return TRUE;
}

//########## プレイヤーを初期化する関数 ##########
//引　数：CHARA 構造体　  ：設定するPLAYER構造体
//引　数：PLAYER構造体　　：設定するCHARA構造体
//引　数：int* 　　 　　　：使用する画像の番号（配列の先頭アドレスを引数とする）
//引　数：int　	　　　　　：画像の初期位置X
//引　数：int　	　　　　　：画像の初期位置Y
//引　数：int　	　　　　　：移動スピード
//戻り値：BOOL：エラー時：FALSE
BOOL MY_INIT_PLAYER(PLAYER* p, CHARA c, int* num, int x, int y, int speed)
{
	//*(num+0)は配列の先頭アドレス

	for (int mot = 0; mot < GAME_CHARA_MOTION_NUM; mot++)
	{
		p->Handle[mot] = c.Handle[*(num + mot)];	//画像のハンドルを入れる
	}

	p->X = x;
	p->Y = y;
	p->Width = c.Width[*(num + 0)];			//最初の画像の横幅を設定
	p->Height = c.Height[*(num + 0)];		//最初の画像の高さを設定
	p->C_Width = c.C_Width[*(num + 0)];		//最初の画像の横幅の中心を設定
	p->C_Height = c.C_Height[*(num + 0)];	//最初の画像の高さの中心を設定

	//マップひとつあたりと同じ大きさにする
	p->Width = GAME_MAP_YOKO_SIZE;
	p->Height = GAME_MAP_TATE_SIZE;
	p->C_Width = GAME_MAP_YOKO_SIZE / 2;
	p->C_Height = GAME_MAP_TATE_SIZE / 2;

	p->NowHandleNum = 10;	//右向きの最初の画像		PlayerImageNumの番地（サーナイトだったら右向きは９～１１）
	p->NowHandleCnt = 0;	//画像カウンタを初期化

	p->NowHandleCntMAX = 4;	//画像カウンタMAXを初期化

	p->Speed = speed;	//移動速度を設定

	p->MoveDist = x;	//今の距離を、マップの最初から動いた距離として設定する
	p->MoveDist_Count = 0;	//動いた回数のカウントを0にする

	p->CanMoveLeft = TRUE;	//左に移動できる
	p->CanMoveRight = TRUE;	//右に移動できる

	p->atariX = 8;			//当たり判定のX位置を、イイ感じに設定	※画像表示と当たり判定のX位置が同じとは、限らない
	p->atariY = 6;			//当たり判定のY位置を、イイ感じに設定	※画像表示と当たり判定のY位置が同じとは、限らない
	p->atariWidth = 16;			//当たり判定の幅   を、イイ感じに設定	※画像表示と当たり判定の幅が同じとは、限らない
	p->atariHeight = 26;	//当たり判定の高さを、イイ感じに設定	※画像表示と当たり判定の高さが同じとは、限らない

	MY_SET_PLAYER_ATARI(p);	//プレイヤーの当たり判定の領域を設定する

	return TRUE;
}

//ゲーム画面のスクロールを行う関数
VOID MY_PLAY_SCROLL(VOID)
{
	ScrollCntYoko += ScrollDistPlusYoko;

	DrawFormatString(0, 20, GetColor(255, 255, 255), "スクロールした量（横）:%06d", ScrollCntYoko);

	return;
}

//ゲームマップの最下層のCSVを読み込む関数
BOOL MY_MAP_READ_CSV_NUM_SAIKASO(FILE* fp, const char* path)
{
	//最下層
	if ((fp = fopen(path, "r")) == NULL)	//ファイルを読み込みモード(r)で開く
	{
		return FALSE;
	}//異常終了

//ここから正常に読み込めた時の処理

	int result_saikaso = 0;		//ファイルの最後かチェック
	int LoopCnt_saikaso = 0;	//ループカウンタ
	while (result_saikaso != EOF)	//End Of File(ファイルの最後)ではないとき繰り返す
	{
		//ファイルから数値を一つ読み込み(%d,)、配列に格納する
		result_saikaso = fscanf(fp, "%d,", &MapData_saikaso[LoopCnt_saikaso / GAME_MAP_YOKO][LoopCnt_saikaso % GAME_MAP_YOKO]);

		MapData_saikaso_Init[LoopCnt_saikaso / GAME_MAP_YOKO][LoopCnt_saikaso % GAME_MAP_YOKO] = MapData_saikaso[LoopCnt_saikaso / GAME_MAP_YOKO][LoopCnt_saikaso % GAME_MAP_YOKO];	//値をコピー

		LoopCnt_saikaso++;
	}
	fclose(fp);//ファイルを閉じる

	int cnt;

	//マップを当たり判定の領域に変換
	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			for (cnt = 0; cnt < GAME_MAP_OKDOWN_KIND; cnt++)//下に行けるマップ
			{
				if (MapData_saikaso[tate][yoko] == MapOKDownKind[cnt])
				{
					rectMap_DownOK[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_DownOK[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_DownOK[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_DownOK[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_DownOK_First[tate][yoko] = rectMap_DownOK[tate][yoko];//初期位置を設定
				}
			}
		}
	}

	return TRUE;
}

//ゲームマップの壁のCSVを読み込む関数
BOOL MY_MAP_READ_CSV_NUM_KABE(FILE* fp, const char* path)
{
	//壁
	if ((fp = fopen(path, "r")) == NULL)	//ファイルを読み込みモード(r)で開く
	{
		return FALSE;
	}//異常終了

//ここから正常に読み込めた時の処理

	int result_kabe = 0;		//ファイルの最後かチェック
	int LoopCnt_kabe = 0;	//ループカウンタ
	while (result_kabe != EOF)	//End Of File(ファイルの最後)ではないとき繰り返す
	{
		//ファイルから数値を一つ読み込み(%d,)、配列に格納する
		result_kabe = fscanf(fp, "%d,", &MapData_kabe[LoopCnt_kabe / GAME_MAP_YOKO][LoopCnt_kabe % GAME_MAP_YOKO]);

		MapData_kabe_Init[LoopCnt_kabe / GAME_MAP_YOKO][LoopCnt_kabe % GAME_MAP_YOKO] = MapData_kabe[LoopCnt_kabe / GAME_MAP_YOKO][LoopCnt_kabe % GAME_MAP_YOKO];	//値をコピー

		LoopCnt_kabe++;
	}
	fclose(fp);//ファイルを閉じる

	int cnt;

	//マップを当たり判定の領域に変換
	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			for (cnt = 0; cnt <= GAME_MAP_NOTDOWN_KIND; cnt++)	//下に行けないマップ
			{
				if (MapData_kabe[tate][yoko] == MapNotDownKind[cnt])
				{
					rectMap_DownNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_DownNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_DownNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_DownNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_DownNG_First[tate][yoko] = rectMap_DownNG[tate][yoko];	//初期位置を設定

				}
			}

			for (cnt = 0; cnt < GAME_MAP_NOTLEFT_KIND; cnt++)			//左に行けないマップ
			{
				if (MapData_kabe[tate][yoko] == MapNotLeftKind[cnt])
				{
					rectMap_LeftNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_LeftNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_LeftNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_LeftNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_LeftNG_First[tate][yoko] = rectMap_LeftNG[tate][yoko];	//初期位置を設定

				}
			}
			for (cnt = 0; cnt < GAME_MAP_NOTRIGHT_KIND; cnt++)			//右に行けないマップ
			{
				if (MapData_kabe[tate][yoko] == MapNotRightKind[cnt])
				{
					rectMap_RightNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_RightNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_RightNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_RightNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_RightNG_First[tate][yoko] = rectMap_RightNG[tate][yoko];	//初期位置を設定

				}
			}
			for (cnt = 0; cnt < GAME_MAP_NOTUP_KIND; cnt++)			//上に行けないマップ
			{
				if (MapData_kabe[tate][yoko] == MapNotUpKind[cnt])
				{
					rectMap_UpNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_UpNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_UpNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_UpNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_UpNG_First[tate][yoko] = rectMap_UpNG[tate][yoko];	//初期位置を設定

				}
			}
		}
	}

	return TRUE;
}

//ゲームマップの木のCSVを読み込む関数
BOOL MY_MAP_READ_CSV_NUM_KI(FILE* fp, const char* path)
{
	//木
	if ((fp = fopen(path, "r")) == NULL)	//ファイルを読み込みモード(r)で開く
	{
		return FALSE;
	}//異常終了

//ここから正常に読み込めた時の処理

	int result_ki = 0;		//ファイルの最後かチェック
	int LoopCnt_ki = 0;	//ループカウンタ
	while (result_ki != EOF)	//End Of File(ファイルの最後)ではないとき繰り返す
	{
		//ファイルから数値を一つ読み込み(%d,)、配列に格納する
		result_ki = fscanf(fp, "%d,", &MapData_ki[LoopCnt_ki / GAME_MAP_YOKO][LoopCnt_ki % GAME_MAP_YOKO]);

		MapData_ki_Init[LoopCnt_ki / GAME_MAP_YOKO][LoopCnt_ki % GAME_MAP_YOKO] = MapData_ki[LoopCnt_ki / GAME_MAP_YOKO][LoopCnt_ki % GAME_MAP_YOKO];	//値をコピー

		LoopCnt_ki++;
	}
	fclose(fp);//ファイルを閉じる

	int cnt;

	//マップを当たり判定の領域に変換
	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			for (cnt = 0; cnt < GAME_MAP_NOTDOWN_KIND; cnt++)	//下に行けないマップ
			{
				if (MapData_ki[tate][yoko] == MapNotDownKind[cnt])
				{
					rectMap_DownNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_DownNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_DownNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_DownNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_DownNG_First[tate][yoko] = rectMap_DownNG[tate][yoko];	//初期位置を設定

				}
			}

			for (cnt = 0; cnt < GAME_MAP_OKDOWN_KIND; cnt++)//下に行けるマップ
			{
				if (MapData_ki[tate][yoko] == MapOKDownKind[cnt])
				{
					rectMap_DownOK[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_DownOK[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_DownOK[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_DownOK[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_DownOK_First[tate][yoko] = rectMap_DownOK[tate][yoko];//初期位置を設定
				}
			}

			for (cnt = 0; cnt < GAME_MAP_NOTLEFT_KIND; cnt++)			//左に行けないマップ
			{
				if (MapData_ki[tate][yoko] == MapNotLeftKind[cnt])
				{
					rectMap_LeftNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_LeftNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_LeftNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_LeftNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_LeftNG_First[tate][yoko] = rectMap_LeftNG[tate][yoko];	//初期位置を設定

				}
			}
			for (cnt = 0; cnt < GAME_MAP_NOTRIGHT_KIND; cnt++)			//右に行けないマップ
			{
				if (MapData_ki[tate][yoko] == MapNotRightKind[cnt])
				{
					rectMap_RightNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_RightNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_RightNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_RightNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_RightNG_First[tate][yoko] = rectMap_RightNG[tate][yoko];	//初期位置を設定

				}
			}
			for (cnt = 0; cnt < GAME_MAP_NOTUP_KIND; cnt++)			//上に行けないマップ
			{
				if (MapData_ki[tate][yoko] == MapNotUpKind[cnt])
				{
					rectMap_UpNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_UpNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_UpNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_UpNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_UpNG_First[tate][yoko] = rectMap_UpNG[tate][yoko];	//初期位置を設定

				}
			}

		}
	}

	return TRUE;
}

//ゲームマップの根っこのCSVを読み込む関数
BOOL MY_MAP_READ_CSV_NUM_NEKKO(FILE* fp, const char* path)
{
	//壁
	if ((fp = fopen(path, "r")) == NULL)	//ファイルを読み込みモード(r)で開く
	{
		return FALSE;
	}//異常終了

//ここから正常に読み込めた時の処理

	int result_nekko = 0;		//ファイルの最後かチェック
	int LoopCnt_nekko = 0;	//ループカウンタ
	while (result_nekko != EOF)	//End Of File(ファイルの最後)ではないとき繰り返す
	{
		//ファイルから数値を一つ読み込み(%d,)、配列に格納する
		result_nekko = fscanf(fp, "%d,", &MapData_nekko[LoopCnt_nekko / GAME_MAP_YOKO][LoopCnt_nekko % GAME_MAP_YOKO]);

		MapData_nekko_Init[LoopCnt_nekko / GAME_MAP_YOKO][LoopCnt_nekko % GAME_MAP_YOKO] = MapData_nekko[LoopCnt_nekko / GAME_MAP_YOKO][LoopCnt_nekko % GAME_MAP_YOKO];	//値をコピー

		LoopCnt_nekko++;
	}
	fclose(fp);//ファイルを閉じる

	int cnt;

	//マップを当たり判定の領域に変換
	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			for (cnt = 0; cnt <= GAME_MAP_NOTDOWN_KIND; cnt++)	//下に行けないマップ
			{
				if (MapData_nekko[tate][yoko] == MapNotDownKind[cnt])
				{
					rectMap_DownNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_DownNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_DownNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_DownNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_DownNG_First[tate][yoko] = rectMap_DownNG[tate][yoko];	//初期位置を設定

				}
			}

			for (cnt = 0; cnt < GAME_MAP_NOTLEFT_KIND; cnt++)			//左に行けないマップ
			{
				if (MapData_nekko[tate][yoko] == MapNotLeftKind[cnt])
				{
					rectMap_LeftNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_LeftNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_LeftNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_LeftNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_LeftNG_First[tate][yoko] = rectMap_LeftNG[tate][yoko];	//初期位置を設定

				}
			}
			for (cnt = 0; cnt < GAME_MAP_NOTRIGHT_KIND; cnt++)			//右に行けないマップ
			{
				if (MapData_nekko[tate][yoko] == MapNotRightKind[cnt])
				{
					rectMap_RightNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_RightNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_RightNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_RightNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_RightNG_First[tate][yoko] = rectMap_RightNG[tate][yoko];	//初期位置を設定

				}
			}
			for (cnt = 0; cnt < GAME_MAP_NOTUP_KIND; cnt++)			//上に行けないマップ
			{
				if (MapData_nekko[tate][yoko] == MapNotUpKind[cnt])
				{
					rectMap_UpNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_UpNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_UpNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_UpNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_UpNG_First[tate][yoko] = rectMap_UpNG[tate][yoko];	//初期位置を設定

				}
			}
		}
	}

	return TRUE;
}

//プレイヤーを操作する関数
VOID MY_PLAY_PLAYER_OPERATION(VOID)
{
	BOOL IsKeyDown = FALSE;

	BOOL CanMoveLeft = TRUE;	//左に行けるか
	BOOL CanMoveRight = TRUE;	//右に行けるか
	BOOL CanMoveUp = TRUE;		//上に行けるか
	BOOL CanMoveDown = TRUE;	//下に行けるか

	BOOL BOSSENGAGE = FALSE;

	//プレイヤーの位置がマップ配列のどこにいるか変換
	int PlayerToMapNumY;
	//プレイヤーの【移動した位置】がマップ配列のどこにいるか変換	※プレイヤーの位置は、スクロール時、変わらないため
	//キャラの位置を、１マップの半分の位置にする
	int PlayerToMapNumX;

	if (AllKeyState[KEY_INPUT_LEFT] != 0)	//左矢印キーが押されていたとき
	{
		IsKeyDown = TRUE;//キーを押された

		if (Myplayer.NowHandleCnt < Myplayer.NowHandleCntMAX)
		{
			Myplayer.NowHandleCnt++;
		}
		else
		{
			Myplayer.NowHandleCnt = 0;

			if (Myplayer.NowHandleNum >= 3 && Myplayer.NowHandleNum < 5)
			{
				Myplayer.NowHandleNum++;//次の左向きの画像
			}
			else
			{
				Myplayer.NowHandleNum = 3;	//一番最初の左向きの画像
			}
		}

		//左方向に、まだ動ける
		Myplayer.CanMoveLeft = TRUE;

		MY_SET_PLAYER_ATARI(&Myplayer);	//プレイヤーの当たり判定の領域を設定
		Myplayer.atariRect.left -= 12;	//少し、当たり判定の領域を左にずらす
		Myplayer.atariRect.right -= 12;	//少し、当たり判定の領域を左にずらす

		if (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_LeftNG) == TRUE)//左に行けないモノと当たったとき
		{
			Myplayer.CanMoveLeft = FALSE;//左に行けない
		}

		if (Myplayer.CanMoveLeft == TRUE)	//左に移動できるとき
		{
			if (MapImage.charaStopFlag == FALSE)	//プレイヤーが移動できるとき
			{
				if (Myplayer.X > 0)
				{
					Myplayer.X -= Myplayer.Speed;	//プレイヤーを左に移動
					Myplayer.MoveDist_Count++;				//動いた回数をカウント
				}
			}

			if (Myplayer.MoveDist > 0)
			{
				Myplayer.MoveDist -= Myplayer.Speed;	//動いた距離を計算
			}
		}

	}

	if (AllKeyState[KEY_INPUT_RIGHT] != 0)	//右矢印キーが押されていたとき
	{
		IsKeyDown = TRUE;//キーを押された

		if (Myplayer.NowHandleCnt < Myplayer.NowHandleCntMAX)
		{
			Myplayer.NowHandleCnt++;
		}
		else
		{
			Myplayer.NowHandleCnt = 0;

			if (Myplayer.NowHandleNum >= 9 && Myplayer.NowHandleNum < 11)
			{
				Myplayer.NowHandleNum++;//次の右向きの画像
			}
			else
			{
				Myplayer.NowHandleNum = 9;	//一番最初の右向きの画像
			}
		}

		//右方向に、まだ動ける
		Myplayer.CanMoveRight = TRUE;

		MY_SET_PLAYER_ATARI(&Myplayer);	//プレイヤーの当たり判定の領域を設定
		Myplayer.atariRect.left += 4;	//少し、当たり判定の領域を右にずらす
		Myplayer.atariRect.right += 4;	//少し、当たり判定の領域を右にずらす

		if (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_RightNG) == TRUE)//右に行けないモノと当たったとき
		{
			Myplayer.CanMoveRight = FALSE;//右に行けない
		}

		if (Myplayer.CanMoveRight == TRUE)	//右に移動できるとき
		{
			if (MapImage.charaStopFlag == FALSE)	//プレイヤーが移動できるとき
			{
				if (Myplayer.X + Myplayer.Width < GAME_WIDTH)
				{
					Myplayer.X += Myplayer.Speed;	//プレイヤーを右に移動
					Myplayer.MoveDist_Count++;				//動いた回数をカウント
				}
			}

			if (Myplayer.MoveDist < GAME_MAP_YOKO_SIZE * GAME_MAP_YOKO)
			{
				Myplayer.MoveDist += Myplayer.Speed;	//動いた距離を計算
			}
		}
	}

	if (AllKeyState[KEY_INPUT_UP] != 0)	//上矢印キーが押されていたとき
	{
		IsKeyDown = TRUE;//キーを押された

		if (Myplayer.NowHandleCnt < Myplayer.NowHandleCntMAX)
		{
			Myplayer.NowHandleCnt++;
		}
		else
		{
			Myplayer.NowHandleCnt = 0;

			if (Myplayer.NowHandleNum >= 0 && Myplayer.NowHandleNum < 2)
			{
				Myplayer.NowHandleNum++;//次の上向きの画像
			}
			else
			{
				Myplayer.NowHandleNum = 0;	//一番最初の上向きの画像
			}
		}

		//上方向に、まだ動ける
		Myplayer.CanMoveUp = TRUE;

		MY_SET_PLAYER_ATARI(&Myplayer);	//プレイヤーの当たり判定の領域を設定
		Myplayer.atariRect.top -= 4;	//少し、プレイヤーの当たり判定の領域を上にずらす
		Myplayer.atariRect.bottom -= 4;	//少し、プレイヤーの当たり判定の領域を上にずらす

		if (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_UpNG) == TRUE)//上に行けないモノと当たったとき
		{
			Myplayer.CanMoveUp = FALSE;//上に行けない
		}

		if (Myplayer.CanMoveUp == TRUE)	//上に移動できるとき
		{
			if (MapImage.charaStopFlag == FALSE)	//プレイヤーが移動できるとき
			{
				if (Myplayer.Y > 0)
				{
					Myplayer.Y -= Myplayer.Speed;	//プレイヤーを上に移動
					Myplayer.MoveDist_Count++;				//動いた回数をカウント
				}
			}

			if (Myplayer.MoveDist > 0)
			{
				Myplayer.MoveDist -= Myplayer.Speed;	//動いた距離を計算
			}
		}

	}

	if (AllKeyState[KEY_INPUT_DOWN] != 0)	//下矢印キーが押されていたとき
	{
		IsKeyDown = TRUE;//キーを押された

		if (Myplayer.NowHandleCnt < Myplayer.NowHandleCntMAX)
		{
			Myplayer.NowHandleCnt++;
		}
		else
		{
			Myplayer.NowHandleCnt = 0;

			if (Myplayer.NowHandleNum >= 6 && Myplayer.NowHandleNum < 8)
			{
				Myplayer.NowHandleNum++;//次の下向きの画像
			}
			else
			{
				Myplayer.NowHandleNum = 6;	//一番最初の下向きの画像
			}
		}

		//下方向に、まだ動ける
		Myplayer.CanMoveDown = TRUE;

		MY_SET_PLAYER_ATARI(&Myplayer);	//プレイヤーの当たり判定の領域を設定
		Myplayer.atariRect.top += 12;	//少し、プレイヤーの当たり判定の領域を下にずらす
		Myplayer.atariRect.bottom += 12;	//少し、プレイヤーの当たり判定の領域を下にずらす

		if (MY_CHECK_RECT_ATARI_CHARA_MAP(Myplayer.atariRect, rectMap_DownNG) == TRUE)//下に行けないモノと当たったとき
		{
			Myplayer.CanMoveDown = FALSE;//下に行けない
		}

		if (Myplayer.CanMoveDown == TRUE)	//下に移動できるとき
		{
			if (MapImage.charaStopFlag == FALSE)	//プレイヤーが移動できるとき
			{
				if (Myplayer.Y + Myplayer.Height < GAME_HEIGHT)
				{
					Myplayer.Y += Myplayer.Speed;	//プレイヤーを下に移動
					Myplayer.MoveDist_Count++;				//動いた回数をカウント
				}
			}

			if (Myplayer.MoveDist < GAME_MAP_TATE_SIZE * GAME_MAP_TATE)
			{
				Myplayer.MoveDist += Myplayer.Speed;	//動いた距離を計算
			}
		}
	}

	if (IsKeyDown == FALSE)
	{
		if (Myplayer.NowHandleNum >= 3 && Myplayer.NowHandleNum <= 5)
		{
			//押されてないときは右向きの立ちポーズ
			Myplayer.NowHandleNum = 4;
		}
		else if (Myplayer.NowHandleNum >= 9 && Myplayer.NowHandleNum <= 11)
		{
			//押されてないときは左向きの立ちポーズ
			Myplayer.NowHandleNum = 10;
		}
		else if (Myplayer.NowHandleNum >= 0 && Myplayer.NowHandleNum <= 2)
		{
			//押されてないときは上向きの立ちポーズ
			Myplayer.NowHandleNum = 1;
		}
		else if (Myplayer.NowHandleNum >= 6 && Myplayer.NowHandleNum <= 8)
		{
			//押されてないときは下向きの立ちポーズ
			Myplayer.NowHandleNum = 7;
		}
		Myplayer.NowHandleCnt = Myplayer.NowHandleCntMAX - 1;	//すぐ画像を変えられるようにする
	}

	//参考はこちら
	//https://dixq.net/g/30.html

		if (AllKeyState[KEY_INPUT_LEFT] != 0)	//左矢印キーが押されていたとき
		{
			IsKeyDown = TRUE;//キーを押された

			if (Myplayer.NowHandleCnt < Myplayer.NowHandleCntMAX)
			{
				Myplayer.NowHandleCnt++;
			}
			else
			{
				Myplayer.NowHandleCnt = 0;

				if (Myplayer.NowHandleNum >= 3 && Myplayer.NowHandleNum < 5)
				{
					Myplayer.NowHandleNum++;//次の左向きの画像
				}
				else
				{
					Myplayer.NowHandleNum = 3;	//一番最初の左向きの画像
				}
			}
		}

		if (AllKeyState[KEY_INPUT_RIGHT] != 0)	//右矢印キーが押されていたとき
		{
			IsKeyDown = TRUE;//キーを押された

			if (Myplayer.NowHandleCnt < Myplayer.NowHandleCntMAX)
			{
				Myplayer.NowHandleCnt++;
			}
			else
			{
				Myplayer.NowHandleCnt = 0;

				if (Myplayer.NowHandleNum >= 9 && Myplayer.NowHandleNum < 11)
				{
					Myplayer.NowHandleNum++;//次の右向きの画像
				}
				else
				{
					Myplayer.NowHandleNum = 9;	//一番最初の右向きの画像
				}
			}
		}

	return;
}

//プレイヤーの当たり判定の領域を設定する関数
VOID MY_SET_PLAYER_ATARI(PLAYER* p)
{
	//当たり判定の領域の設定
	p->atariRect.left = p->X + p->atariX + 2;						//現在のX位置 ＋ 当たり判定のX位置
	p->atariRect.top = p->Y + p->atariY + 2;						//現在のY位置 ＋ 当たり判定のY位置
	p->atariRect.right = p->X + p->atariX + p->atariWidth - 2;		//現在のX位置 ＋ 当たり判定のX位置 ＋ 当たり判定の幅
	p->atariRect.bottom = p->Y + p->atariY + p->atariHeight - 2;	//現在のY位置 ＋ 当たり判定のY位置 ＋ 当たり判定の高さ

	return;
}

//領域の当たり判定をする関数
BOOL MY_CHECK_RECT_ATARI(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;	//当たっている
	}

	return FALSE;		//当たっていない
}

//キャラクターとマップの当たり判定をする関数
//引　数　：RECT	：当たり判定をする領域(キャラクター)
//引　数　：RECT	：当たり判定をする領域(マップ)
//戻り値　：当たっている：TRUE / 当たっていない / FALSE
BOOL MY_CHECK_RECT_ATARI_CHARA_MAP(RECT chara, RECT map[GAME_MAP_TATE][GAME_MAP_YOKO])
{
	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			//キャラクターの当たっている場所を取得
			if (MY_CHECK_RECT_ATARI(chara, map[tate][yoko]) == TRUE)
			{
				return TRUE;
			}
		}
	}

	return FALSE;		//当たっていない
}

// ########## 音を読み込む設定をする関数 ##########
//引　数：音構造体　	:設定する音構造体の変数
//引　数：const char *	:読み込む画像のファイルパス
//戻り値：BOOL			:TRUE:正常 / FALSE:異常
BOOL MY_MUSIC_LOAD(MUSIC* m, const char* path)
{
	//音を読み込む
	m->Handle = LoadSoundMem(path);

	if (m->Handle == -1)
	{
		return FALSE;
	}
	return TRUE;
}