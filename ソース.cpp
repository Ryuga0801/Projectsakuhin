#include "DxLib.h"
#include<math.h>

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
#define GAME_SOUSA "gazou\\sousasetumei.png"

#define GAME_PLAYER "chara\\chara1.png"

#define GAME_MAP_PNG			 "map\\mapdata2.png"
#define GAME_MAP_CSV_SAIKASO	 "map\\mapdata_saikaso.csv"

#define GAME_MAP_BUN_YOKO_CNT 86	//�}�b�v�̕������i���j
#define GAME_MAP_BUN_TATE_CNT 46    //�}�b�v�̕������i�c�j

#define GAME_MAP_YOKO	100			//�}�b�v�̐��i���j	
#define GAME_MAP_TATE	80			//�}�b�v�̐��i�c�j

#define GAME_MAP_YOKO_SIZE 8		//�}�b�v�̑傫���i���j
#define GAME_MAP_TATE_SIZE 8		//�}�b�v�̑傫���i�c)

#define GAME_CHARA_BUN_YOKO_CNT 12	//�L�����̕������i���j
#define GAME_CHARA_BUN_TATE_CNT 8	//�L�����̕������i�c�j

#define GAME_CHARA_YOKO_SIZE 25		//�L�����̑傫���i���j	//�c�`�m�R�̏ꍇ�F48	�T�[�i�C�g�̏ꍇ�F24
#define GAME_CHARA_TATE_SIZE 32		//�L�����̑傫���i�c�j	//�c�`�m�R�̏ꍇ�F48	�T�[�i�C�g�̏ꍇ�F32

#define GAME_CHARA_MOTION_NUM 12	//�L�����̉摜��

#define GAME_CHARA_KIND_YOKO 3		//�L�����̉��̎��
#define GAME_CHARA_KIND_TATE 4		//�L�����̏c�̎��

#define GAME_CHARA_SPEED 3			//�L�����̃X�s�[�h

#define FNT_TANU_NAME TEXT("���ʂ������}�W�b�N")

#define GAME_MAP_NOTDOWN_KIND	1		//���ɍs���Ȃ��}�b�v�̎��
#define GAME_MAP_OKDOWN_KIND	4		//���ɍs����}�b�v�̎��

#define GAME_MAP_NOTUP_KIND		1		//��ɍs���Ȃ��}�b�v�̎��

#define GAME_MAP_NOTLEFT_KIND	1	//���ɍs���Ȃ��}�b�v�̎��
#define GAME_MAP_NOTRIGHT_KIND	1	//�E�ɍs���Ȃ��}�b�v�̎��

#define GAME_MAP_NO_KIND		1

#define GAME_MAP_GOAL_KIND		1	//�S�[���̎��

enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_SOUSA,
	GAME_SCENE_IDOU,
	GAME_SCENE_OVER,
	GAME_SCENE_CLEAR
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
};//�L�����̎�ނ̗񋓌^

char AllKeyState[256];

int GameSceneNow = (int)GAME_SCENE_TITLE;

int ScrollCntYoko = 0;			//�X�N���[���J�E���^�i���j

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
	char FilePath[128];	//�t�@�C���̃p�X
	int Handle[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];	//�摜�̃n���h��
	int X[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];			//���̈ʒu
	int Y[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];			//�c�̈ʒu
	int Width[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];		//���̃T�C�Y
	int Height[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];	//�c�̃T�C�Y
	int C_Width[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];	//���̒��S�ʒu
	int C_Height[GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT];	//�c�̒��S�ʒu

	BOOL charaStopFlag;	//�L�����̈ړ�����t���O

	//BOOL GoalFlg;		//���̃}�b�v���S�[��������

	//int GameEndKind;	//�Q�[���I���̎��

	//BOOL GoalEffect1;	//�S�[���������̉��o�P

};//MAP�\����

struct STRUCT_CHARACTOR
{
	char FilePath[128];	//�t�@�C���̃p�X
	int Handle[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];	//�摜�̃n���h��
	int X[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];		//���̈ʒu
	int Y[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];		//�c�̈ʒu
	int Width[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];	//���̃T�C�Y
	int Height[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];	//�c�̃T�C�Y
	int C_Width[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];	//���̒��S�ʒu
	int C_Height[GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT];	//�c�̒��S�ʒu
};//CHARA�\����

struct STRUCT_PLAYER
{
	int Handle[GAME_CHARA_MOTION_NUM];	//�摜�̃n���h��
	int X;								//���̈ʒu
	int Y;								//�c�̈ʒu
	int Width;							//���̃T�C�Y
	int Height;							//�c�̃T�C�Y
	int C_Width;						//���̒��S�ʒu
	int C_Height;						//�c�̒��S�ʒu
	int NowHandleNum;					//���݂̉摜�̃n���h��
	int NowHandleCnt;					//���݂̃n���h���J�E���g
	int NowHandleCntMAX;				//���݂̃n���h���J�E���gMAX

	int Speed;							//���x

	int MoveDist;						//�ړ�����

	BOOL CanMoveLeft;					//���ɍs���邩
	BOOL CanMoveRight;					//�E�ɍs���邩

	int atariX;			//�����蔻���X�ʒu
	int atariY;			//�����蔻���Y�ʒu
	int atariWidth;		//�����蔻��̕�
	int atariHeight;	//�����蔻��̍���

	RECT atariRect;		//�����蔻��̋�`�̈�

};//PLAYER�\����

typedef STRUCT_GAZOU GAZOU;
typedef STRUCT_MAP MAP;

typedef STRUCT_CHARACTOR CHARA;
typedef STRUCT_PLAYER PLAYER;

//�E�B���h�E�֌W
WNDPROC WndProc;			//�E�B���h�E�v���V�[�W���̃A�h���X
BOOL IsWM_CREATE = FALSE;	//WM_CREATE������ɓ��삵�������肷��

GAZOU title;
GAZOU sousa;
GAZOU over;
GAZOU clear;
GAZOU map;

MAP MapImage;	//�}�b�v�̉摜

CHARA CharaImage;	//�L�����̉摜
PLAYER Myplayer;	//�v���C���[

// �v���C���[�̃C���[�W�摜�̔ԍ���ݒ肷��
int PlayerImageNum[GAME_CHARA_MOTION_NUM] = {		//�c�`�m�R�̏ꍇ		//�T�[�i�C�g�̏ꍇ
	9,10,11,	//��									//39,40,41,	//��		//84,85,86,	//��	
	45,46,47,	//��									//15,16,17,	//��		//60,61,62,	//��	
	33,34,35,	//��										//3,4,5,	//��		//48,49,50,	//��
	21,22,23	//�E									//27,28,29	//�E		//72,73,74,	//�E	

};

int StartTimeFps;
int CountFps;
float CalcFps;
int SampleNumFps = GAME_FPS_SPEED;

int HFontTanu_32;

BOOL GAZOU_LOAD(GAZOU*, int, int, const char*);

BOOL FONT_CREATE(const char*, int*, int, int, int);

BOOL MY_MAP_READ_CSV_NUM(FILE*, const char*);

BOOL MY_MAP_LOAD_BUNKATSU(MAP*, int, int, int, int, int, const char*);	//MAP�𕪊����ēǂݍ��ސݒ������֐�

BOOL MY_CHARA_LOAD_BUNKATSU(CHARA*, int, int, int, int, int, const char*);

BOOL MY_INIT_PLAYER(PLAYER*, CHARA, int*, int, int, int);					//�v���C���[������������֐�

VOID ALL_KEYDOWN_UPDATE(VOID);

BOOL IsActiveCheck = FALSE;	//��ʂ̃A�N�e�B�u�`�F�b�N�L����

int StopActiveStartTime = 0;	//��A�N�e�B�u���̊J�n����
int StopActiveStopTime = 0;		//��A�N�e�B�u���̏I������
int StopActiveTotalTime = 0;	//��A�N�e�B�u���̌o�ߎ���

VOID FPS_UPDATE(VOID);
VOID FPS_DRAW(VOID);
VOID FPS_WAIT(VOID);

VOID MY_PLAY_PLAYER_DRAW(VOID);		//�v���C���[��\������֐�

VOID MY_PLAY_MAP_DRAW(VOID);	//�}�b�v��\������֐�

VOID MY_GAME_PLAY(VOID);		//�v���C��ʂ̊֐�

int MapData[GAME_MAP_TATE][GAME_MAP_YOKO];	//�}�b�v�̃f�[�^
int MapData_Init[GAME_MAP_TATE][GAME_MAP_YOKO];//�}�b�v�̃f�[�^�i�������p�j

FILE* fp_map_csv;		//�t�@�C���̊Ǘ��ԍ��i�|�C���^���n���h���j

int MapNotDownKind[GAME_MAP_NOTDOWN_KIND] = {};	//���ɍs���Ȃ��}�b�v�̔ԍ�
int MapOKDownKind[GAME_MAP_OKDOWN_KIND] = {594,595,681,682};

int MapNoKind[GAME_MAP_NO_KIND] = {};

int MapNotUpKind[GAME_MAP_NOTUP_KIND] = {};

int MapNotLeftKind[GAME_MAP_NOTLEFT_KIND] = {};		//���ɍs���Ȃ��}�b�v�̔ԍ�
int MapNotRightKind[GAME_MAP_NOTRIGHT_KIND] = {};	//�E�ɍs���Ȃ��}�b�v�̔ԍ�

int MapGoalKind[GAME_MAP_GOAL_KIND] = {};//�S�[���̃}�b�v�̔ԍ�


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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(GAME_WINDOW_MODECHANGE);
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	SetWindowStyleMode(SET_WINDOW_ST_MODE_DEFAULT);
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));

	if (DxLib_Init() == -1) { return -1; }

	SetDrawScreen(DX_SCREEN_BACK);

	if (GAZOU_LOAD(&title, 0, 0, GAME_TITLE) == FALSE) { return -1; }
	if (GAZOU_LOAD(&sousa, 0, 0, GAME_SOUSA) == FALSE) { return -1; }
	if (GAZOU_LOAD(&over, 0, 0, GAME_OVER) == FALSE) { return -1; }
	if (GAZOU_LOAD(&clear, 0, 0, GAME_CLEAR) == FALSE) { return -1; }
	if (GAZOU_LOAD(&map, 0, 0, GAME_MAP_PNG) == FALSE) { return -1; }

	//if (MY_MAP_LOAD_BUNKATSU(&MapImage, GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT, GAME_MAP_BUN_TATE_CNT, GAME_MAP_BUN_YOKO_CNT, 8, 8, GAME_MAP_PNG) == FALSE) { MessageBox(NULL, GAME_MAP_PNG, "NotFound", MB_OK); return -1; }	//MAP��ǂݍ���

	if (MY_CHARA_LOAD_BUNKATSU(&CharaImage, GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT, GAME_CHARA_BUN_YOKO_CNT, GAME_CHARA_BUN_TATE_CNT, GAME_CHARA_YOKO_SIZE, GAME_CHARA_TATE_SIZE, GAME_PLAYER) == FALSE) { MessageBox(NULL, GAME_PLAYER, "NotFound", MB_OK); return -1; }	//CHARA��ǂݍ���

	if (MY_MAP_READ_CSV_NUM(fp_map_csv, GAME_MAP_CSV_SAIKASO) == FALSE) { MessageBox(NULL, GAME_MAP_CSV_SAIKASO, "NotFound", MB_OK); return -1; }	//CSV��ǂݍ���

	if (FONT_CREATE(FNT_TANU_NAME, &HFontTanu_32, 32, 1, DX_FONTTYPE_ANTIALIASING_EDGE) == FALSE) { return -1; }

	//��ʂ̃A�N�e�B�u�`�F�b�N�L����
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

			DrawGraph(title.X, title.Y, title.Handle, TRUE);

			if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_SOUSA;
			}

			break;

		case(int)GAME_SCENE_SOUSA:

			DrawGraph(sousa.X, sousa.Y, sousa.Handle, TRUE);

			if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_IDOU;
			}

			break;

		case(int)GAME_SCENE_IDOU:

			if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_CLEAR;
			}

			if (AllKeyState[KEY_INPUT_BACK] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_OVER;
			}
			MY_PLAY_PLAYER_DRAW();
			//MY_GAME_PLAY();		//�v���C��ʂ̏���

			break;

		case(int)GAME_SCENE_OVER:

			DrawGraph(over.X, over.Y, over.Handle, TRUE);

			if (AllKeyState[KEY_INPUT_BACK] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_TITLE;
			}

			break;

		case(int)GAME_SCENE_CLEAR:

			DrawGraph(clear.X, clear.Y, clear.Handle, TRUE);

			if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				GameSceneNow = (int)GAME_SCENE_TITLE;
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
		DeleteGraph(MapImage.Handle[mapCnt]);	//�}�b�v�̃n���h�����폜
	}

	for (int charaCnt = 0; charaCnt < GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT; charaCnt++)
	{
		DeleteGraph(MapImage.Handle[charaCnt]);	//�L�����̃n���h�����폜
	}

	DxLib_End();		//�c�w���C�u�����g�p�̏I������
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

VOID MY_PLAY_MAP_DRAW(VOID)
{
	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			if ((yoko + 1) * GAME_MAP_YOKO_SIZE - ScrollCntYoko >= 0 &&
				(yoko + 0) * GAME_MAP_YOKO_SIZE - ScrollCntYoko <= GAME_WIDTH)	//��ʂɎʂ��Ă���Ȃ�΂Ȃ��
			{
				DrawGraph(
					yoko * GAME_MAP_YOKO_SIZE,			//�ʒu����X�N���[���ʂ�����
					tate * GAME_MAP_TATE_SIZE,
					MapImage.Handle[tate * GAME_CHARA_BUN_YOKO_CNT + yoko], TRUE);	//�}�b�v���X�N���[�����Ȃ���`��

				//DrawBox(
				//	rectMap_DownNG[tate][yoko].left,
				//	rectMap_DownNG[tate][yoko].top,
				//	rectMap_DownNG[tate][yoko].right,
				//	rectMap_DownNG[tate][yoko].bottom,
				//	GetColor(255, 0, 0), FALSE//���ɍs���Ȃ��}�b�v
				//);
				//DrawBox(
				//	rectMap_DownOK[tate][yoko].left,
				//	rectMap_DownOK[tate][yoko].top,
				//	rectMap_DownOK[tate][yoko].right,
				//	rectMap_DownOK[tate][yoko].bottom,
				//	GetColor(0, 255, 255), FALSE//���ɍs���Ȃ��}�b�v
				//);
				//DrawBox(
				//	rectMap_Coin[tate][yoko].left,
				//	rectMap_Coin[tate][yoko].top,
				//	rectMap_Coin[tate][yoko].right,
				//	rectMap_Coin[tate][yoko].bottom,
				//	GetColor(255, 255, 0), FALSE//���ɍs����}�b�v
				//);

			}
		}
	}
}

//�v���C���[��\������֐�
VOID MY_PLAY_PLAYER_DRAW(VOID)
{
	DrawExtendGraph(
		Myplayer.X,
		Myplayer.Y,
		Myplayer.X + Myplayer.Width,
		Myplayer.Y + Myplayer.Height + 4,
		Myplayer.Handle[Myplayer.NowHandleNum], TRUE
	);

	DrawBox(
		Myplayer.atariRect.left,
		Myplayer.atariRect.top,
		Myplayer.atariRect.right,
		Myplayer.atariRect.bottom,
		GetColor(0, 0, 255), FALSE);

	/*DrawFormatString(0, 40, GetColor(255, 255, 255), "�v���C���[�̓��������� : %04d", Myplayer.MoveDist);	//������������\��
	DrawFormatString(0, 60, GetColor(255, 255, 255), "�v���C���[X�F%04d", Myplayer.X);	//�v���C���[��X�ʒu��\��
	DrawFormatString(0, 80, GetColor(255, 255, 255), "�v���C���[Y�F%04d", Myplayer.Y);	//�v���C���[��Y�ʒu��\��
	DrawFormatString(0, 100, GetColor(255, 255, 255), "�W�����v���_�F%04d", Myplayer.JumpMaxTop);	//�v���C���[�̃W�����v�̒��_

	//�W�����v��
	if (Myplayer.IsJumpNow == TRUE)
	{
		DrawString(0, 120, "�W�����v�t���OON", GetColor(255, 255, 255));	//�W�����v��
	}

	//���_�ɒB�����Ƃ�
	if (Myplayer.IsJumpTop == TRUE)
	{
		DrawString(0, 140, "���_�t���OON", GetColor(255, 255, 255));//�W�����v��
	}*/

	return;
}

BOOL MY_MAP_LOAD_BUNKATSU(MAP* m, int bun_num, int bun_x_num, int bun_y_num, int bun_width, int bun_height, const char* path)
{
	int ret = 0;	//�t�@�C���p�X���R�s�[
	wsprintf(m->FilePath, path);

	//���l�^�͂�����@�@�@�Fhttps://brain.cc.kogakuin.ac.jp/~kanamaru/lecture/prog1/11-03.html
	//���t�@�����X�͂�����Fhttps://dxlib.xsrv.jp/function/dxfunc_graph1.html#R3N3

	//�摜���������ɕ������ēǂݍ��݁A�n���h�����擾
	ret = LoadDivGraph(m->FilePath, bun_num, bun_x_num, bun_y_num, bun_width, bun_height, m->Handle);

	if (ret == -1) { return FALSE; }	//�摜�ǂݍ��݃G���[

	for (int cnt = 0; cnt < GAME_MAP_BUN_YOKO_CNT * GAME_MAP_BUN_TATE_CNT; cnt++)
	{
		//�摜�T�C�Y�擾
		GetGraphSize(
			m->Handle[cnt],	//�n���h������A�摜�̕��ƍ������擾
			&m->Width[cnt],	//MAP�\���̂̕��ɁA�摜�̕���ݒ肷��
			&m->Height[cnt]);	//MAP�\���̂̍����ɁA�摜�̍�����ݒ�

		m->C_Width[cnt] = m->Width[cnt] / 2;	//�摜�̉��T�C�Y�̒��S���擾
		m->C_Height[cnt] = m->Height[cnt] / 2;	//�摜�̏c�T�C�Y�̒��S���擾
	}
	m->charaStopFlag = FALSE;

	return TRUE;
}

//########## CHARA�𕪊����ēǂݍ��ސݒ������֐� ##########
//���@���FCHARA�\���́@�F�ݒ肷��摜�\����(����)�̕ϐ�
//���@���Fint�@�F�摜�̕�����
//���@���Fint�@�F�摜�̉��̕�����
//���@���Fint�@�F�摜�̏c�̕�����
//���@���Fint�@�F�摜�̉��̕����T�C�Y
//���@���Fint�@�F�摜�̏c�̕����T�C�Y
//���@���Fconst char *�F�ǂݍ��މ摜�̃t�@�C���p�X
//�߂�l�FBOOL�F�G���[���FFALSE
BOOL MY_CHARA_LOAD_BUNKATSU(CHARA* c, int bun_num, int bun_x_num, int bun_y_num, int bun_width, int bun_height, const char* path)
{
	int ret = 0;
	wsprintf(c->FilePath, path);	//�t�@�C���̃p�X���R�s�[

	//���l�^�͂�����@�@�@�Fhttps://brain.cc.kogakuin.ac.jp/~kanamaru/lecture/prog1/11-03.html
	//���t�@�����X�͂�����Fhttps://dxlib.xsrv.jp/function/dxfunc_graph1.html#R3N3

	//�摜���������ɕ������ēǂݍ��݁A�n���h�����擾
	ret = LoadDivGraph(c->FilePath, bun_num, bun_x_num, bun_y_num, bun_width, bun_height, c->Handle);

	if (ret == -1) { return FALSE; }//�摜�ǂݍ��݃G���[

	for (int cnt = 0; cnt < GAME_CHARA_BUN_YOKO_CNT * GAME_CHARA_BUN_TATE_CNT; cnt++)
	{
		//�摜�T�C�Y���擾
		GetGraphSize(
			c->Handle[cnt],	//�n���h������A�摜�̕��ƍ������擾
			&c->Width[cnt],	//MAP�\���̂̕��ɁA�摜�̕���ݒ肷��
			&c->Height[cnt]	//MAP�\���̂̍����ɁA�摜�̍�����ݒ肷��
		);

		c->C_Width[cnt] = c->Width[cnt] / 2;	//�摜�̉��T�C�Y�̒��S���擾
		c->C_Height[cnt] = c->Height[cnt] / 2;	//�摜�̏c�T�C�Y�̒��S���擾
	}
	return TRUE;
}

//########## �v���C���[������������֐� ##########
//���@���FCHARA �\���́@  �F�ݒ肷��PLAYER�\����
//���@���FPLAYER�\���́@�@�F�ݒ肷��CHARA�\����
//���@���Fint* �@�@ �@�@�@�F�g�p����摜�̔ԍ��i�z��̐擪�A�h���X�������Ƃ���j
//���@���Fint�@	�@�@�@�@�@�F�摜�̏����ʒuX
//���@���Fint�@	�@�@�@�@�@�F�摜�̏����ʒuY
//���@���Fint�@	�@�@�@�@�@�F�ړ��X�s�[�h
//�߂�l�FBOOL�F�G���[���FFALSE
BOOL MY_INIT_PLAYER(PLAYER* p, CHARA c, int* num, int x, int y, int speed)
{
	//*(num+0)�͔z��̐擪�A�h���X

	for (int mot = 0; mot < GAME_CHARA_MOTION_NUM; mot++)
	{
		p->Handle[mot] = c.Handle[*(num + mot)];	//�摜�̃n���h��������
	}

	p->X = x;
	p->Y = y;
	p->Width = c.Width[*(num + 0)];			//�ŏ��̉摜�̉�����ݒ�
	p->Height = c.Height[*(num + 0)];		//�ŏ��̉摜�̍�����ݒ�
	p->C_Width = c.C_Width[*(num + 0)];		//�ŏ��̉摜�̉����̒��S��ݒ�
	p->C_Height = c.C_Height[*(num + 0)];	//�ŏ��̉摜�̍����̒��S��ݒ�

	//�}�b�v�ЂƂ�����Ɠ����傫���ɂ���
	p->Width = GAME_MAP_YOKO_SIZE;
	p->Height = GAME_MAP_TATE_SIZE;
	p->C_Width = GAME_MAP_YOKO_SIZE / 2;
	p->C_Height = GAME_MAP_TATE_SIZE / 2;

	p->NowHandleNum = 10;	//�E�����̍ŏ��̉摜		PlayerImageNum�̔Ԓn�i�T�[�i�C�g��������E�����͂X�`�P�P�j
	p->NowHandleCnt = 0;	//�摜�J�E���^��������

	p->NowHandleCntMAX = 4;	//�摜�J�E���^MAX��������

	p->Speed = speed;	//�ړ����x��ݒ�

	p->MoveDist = x;	//���̋������A�}�b�v�̍ŏ����瓮���������Ƃ��Đݒ肷��

	p->CanMoveLeft = TRUE;	//���Ɉړ��ł���
	p->CanMoveRight = TRUE;	//�E�Ɉړ��ł���

	p->atariX = 8;			//�����蔻���X�ʒu���A�C�C�����ɐݒ�	���摜�\���Ɠ����蔻���X�ʒu�������Ƃ́A����Ȃ�
	p->atariY = 6;			//�����蔻���Y�ʒu���A�C�C�����ɐݒ�	���摜�\���Ɠ����蔻���Y�ʒu�������Ƃ́A����Ȃ�
	p->atariWidth = 16;			//�����蔻��̕�   ���A�C�C�����ɐݒ�	���摜�\���Ɠ����蔻��̕��������Ƃ́A����Ȃ�
	p->atariHeight = 26;	//�����蔻��̍������A�C�C�����ɐݒ�	���摜�\���Ɠ����蔻��̍����������Ƃ́A����Ȃ�

	//MY_SET_PLAYER_ATARI(p);	//�v���C���[�̓����蔻��̗̈��ݒ肷��

	return TRUE;
}

//�Q�[���}�b�v��CSV��ǂݍ��ފ֐�
BOOL MY_MAP_READ_CSV_NUM(FILE* fp, const char* path)
{
	if ((fp = fopen(path, "r")) == NULL)	//�t�@�C����ǂݍ��݃��[�h(r)�ŊJ��
	{
		return FALSE;
	}//�ُ�I��

//�������琳��ɓǂݍ��߂����̏���

	int result = 0;		//�t�@�C���̍Ōォ�`�F�b�N
	int LoopCnt = 0;	//���[�v�J�E���^
	while (result != EOF)	//End Of File(�t�@�C���̍Ō�)�ł͂Ȃ��Ƃ��J��Ԃ�
	{
		//�t�@�C�����琔�l����ǂݍ���(%d,)�A�z��Ɋi�[����
		result = fscanf(fp, "%d,", &MapData[LoopCnt / GAME_MAP_YOKO][LoopCnt % GAME_MAP_YOKO]);

		MapData_Init[LoopCnt / GAME_MAP_YOKO][LoopCnt % GAME_MAP_YOKO] = MapData[LoopCnt / GAME_MAP_YOKO][LoopCnt % GAME_MAP_YOKO];	//�l���R�s�[

		LoopCnt++;
	}
	fclose(fp);//�t�@�C�������

	int cnt;

	//�}�b�v�𓖂��蔻��̗̈�ɕϊ�
	for (int tate = 0; tate < GAME_MAP_TATE; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO; yoko++)
		{
			for (cnt = 0; cnt < GAME_MAP_NOTDOWN_KIND; cnt++)	//���ɍs���Ȃ��}�b�v
			{
				if (MapData[tate][yoko] == MapNotDownKind[cnt])
				{
					rectMap_DownNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_DownNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_DownNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_DownNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_DownNG_First[tate][yoko] = rectMap_DownNG[tate][yoko];	//�����ʒu��ݒ�

				}
			}

			for (cnt = 0; cnt < GAME_MAP_OKDOWN_KIND; cnt++)//���ɍs����}�b�v
			{
				if (MapData[tate][yoko] == MapOKDownKind[cnt])
				{
					rectMap_DownOK[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_DownOK[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_DownOK[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_DownOK[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_DownOK_First[tate][yoko] = rectMap_DownOK[tate][yoko];//�����ʒu��ݒ�
				}
			}

			for (cnt = 0; cnt < GAME_MAP_NOTLEFT_KIND; cnt++)			//���ɍs���Ȃ��}�b�v
			{
				if (MapData[tate][yoko] == MapNotLeftKind[cnt])
				{
					rectMap_LeftNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_LeftNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_LeftNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_LeftNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_LeftNG_First[tate][yoko] = rectMap_LeftNG[tate][yoko];	//�����ʒu��ݒ�

				}
			}
			for (cnt = 0; cnt < GAME_MAP_NOTRIGHT_KIND; cnt++)			//�E�ɍs���Ȃ��}�b�v
			{
				if (MapData[tate][yoko] == MapNotRightKind[cnt])
				{
					rectMap_RightNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_RightNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_RightNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_RightNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_RightNG_First[tate][yoko] = rectMap_RightNG[tate][yoko];	//�����ʒu��ݒ�

				}
			}
			for (cnt = 0; cnt < GAME_MAP_NOTUP_KIND; cnt++)			//��ɍs���Ȃ��}�b�v
			{
				if (MapData[tate][yoko] == MapNotUpKind[cnt])
				{
					rectMap_UpNG[tate][yoko].left = yoko * GAME_MAP_YOKO_SIZE + 1;
					rectMap_UpNG[tate][yoko].top = tate * GAME_MAP_TATE_SIZE + 1;
					rectMap_UpNG[tate][yoko].right = (yoko + 1) * GAME_MAP_YOKO_SIZE - 1;
					rectMap_UpNG[tate][yoko].bottom = (tate + 1) * GAME_MAP_TATE_SIZE - 1;

					rectMap_UpNG_First[tate][yoko] = rectMap_UpNG[tate][yoko];	//�����ʒu��ݒ�

				}
			}

		}
	}

	return TRUE;
}

//�v���C��ʂ̊֐�
VOID MY_GAME_PLAY(VOID)
{
	MY_PLAY_MAP_DRAW();			//�}�b�v��`��
	MY_PLAY_PLAYER_DRAW();		//�v���C���[��`��
}