#define _CRT_SECURE_NO_WARNINGS
#include "glut.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define WIDTH 21
#define HEIGHT 21
#define GRID_SIZE (1.0 / 21)
#define ENEMY_NUM 4
#define STAGE_NUM 3

// プレイヤー変数
int pos_player[2] = { 9, 8 };	// プレイヤーの座標を格納する配列
int mouth = 0;			// プレイヤーの表示状態を変える変数
int direction = 0;		// プレイヤーの向きを保存する変数
int mutekiFlag = 0;
int mutekiCount = 0;
int zanki = 3;

// 敵
int pos_enemy[ENEMY_NUM][2];
int enemyID;
GLfloat colEnemy[ENEMY_NUM][3];

//当たり判定
int crush = 0;
int precrush = 0;

// 各フラグ
int ClearFlag = 0;
int OverFlag = 0;

// キーの状態を表すグローバル変数(0: 押されていない, 1: 押されている)
int keyUp = 0, keyDown = 0, keyLeft = 0, keyRight = 0, keySpace = 0, keyC = 0;

// 固定ブロック
int stage;
double colwall[3] = { 0.3, 0.3, 1.0 };
int wall[STAGE_NUM][HEIGHT][WIDTH] = {		// x,y座標を合わせるために本来下にあるものを先に書き込む
									//	0:何もない通路 1:壁 2:エサ	3:パワーアップ
	{
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1 },
		{ 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
		{ 1, 2, 2, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 2, 0, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
		{ 0, 2, 1, 2, 2, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 0 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
		{ 1, 2, 2, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1 },
		{ 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	},
	{
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
		{ 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1 },
		{ 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1 },
		{ 1, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1 },
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
		{ 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1 },
		{ 0, 2, 2, 2, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 0 },
		{ 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1 },
		{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1 },
		{ 1, 3, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	},
	{
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
		{ 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 2, 2, 2, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 2, 2, 2, 2, 2, 0, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 2, 1, 2, 1 },
		{ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 1, 1, 2, 2, 2, 0 },
		{ 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 2, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1 },
		{ 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1 },
		{ 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	}
};

void init() {
	int i,j;
	int x, y;
	char str[20];
	glClearColor(0.0, 0.0, 0.0, 1.0);

	srand((unsigned)time(NULL));

	for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++) {
		do {
			x = rand() % 22;
			y = rand() % 22;
		} while (wall[stage][y][x] == 1 || (x == 9 && y == 8));		
		// 壁が存在しない座標x, yを確保し，敵の座標に代入する(プレイヤー出現場所にも出現させない)

		pos_enemy[enemyID][0] = x;
		pos_enemy[enemyID][1] = y;

		for (i = 0; i < 3; i++)
			colEnemy[enemyID][i] = (double)rand() / RAND_MAX / 2 + 0.4;		// 背景と同化しないように明るい色にする
	}
	// プレイヤー変数
	pos_player[0] = 9;
	pos_player[1] = 8;	// プレイヤーの座標を格納する配列
	mouth = 0;			// プレイヤーの表示状態を変える変数
	direction = 0;		// プレイヤーの向きを保存する変数
	mutekiFlag = 0;
	mutekiCount = 0;
	zanki = 3;

	//当たり判定
	crush = 0;
	precrush = 0;

	// 各フラグ
	ClearFlag = 0;
	OverFlag = 0;
	keySpace = 0;
	
	for(stage = 0; stage<STAGE_NUM; stage++){
		for(i=0; i<21; i++)
			for(j=0; j<21; j++)
				if(wall[stage][j][i] == 0)
					wall[stage][j][i] = 2;
		wall[stage][10][0] = 0;
		wall[stage][10][20] = 0;
		wall[stage][1][1] = 3;
		wall[stage][1][19] = 3;
		wall[stage][19][1] = 3;
		wall[stage][19][19] = 3;
	}
	stage = rand() % 3;
}

// スペースキー, Cが押されたかどうか
void KeyPressed(unsigned char key, int x, int y) {
	if (key == 32)	keySpace = 1;
	if (key == 99)	keyC = 1;
}
// スペースキー, Cが押されたかどうか
void KeyReleased(unsigned char key, int x, int y) {
/*	if (key == 32)	keySpace = 0;	*/
	if (key == 99)	keyC = 0;
}

// 特殊キーが押された際の処理を行う関数を指定
void checkSpecialKeyPressed(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: keyUp = 1; break;
	case GLUT_KEY_DOWN: keyDown = 1; break;
	case GLUT_KEY_LEFT: keyLeft = 1; break;
	case GLUT_KEY_RIGHT: keyRight = 1; break;
	}
}

// 特殊キーが戻された際の処理を行う関数を指定
void checkSpecialKeyReleased(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: keyUp = 0; break;
	case GLUT_KEY_DOWN: keyDown = 0; break;
	case GLUT_KEY_LEFT: keyLeft = 0; break;
	case GLUT_KEY_RIGHT: keyRight = 0; break;
	}
}

// ブロックを描画
void drawBlock(int x, int y) {
	double pos_x = -1 + 2 * GRID_SIZE * x;
	double pos_y = -1 + 2 * GRID_SIZE * y;
	double size = 2 * GRID_SIZE;

	glBegin(GL_POLYGON);
	glVertex2d(pos_x, pos_y);
	glVertex2d(pos_x + size, pos_y);
	glVertex2d(pos_x + size, pos_y + size);
	glVertex2d(pos_x, pos_y + size);
	glEnd();
}

//円描画関数(塗りつぶし)
void drawCircle(double x, double y, double radius, int startAngle, int endAngle) {
	double pos_x = -1 + 2 * GRID_SIZE * x + GRID_SIZE;
	double pos_y = -1 + 2 * GRID_SIZE * y + GRID_SIZE;
	double size = radius * GRID_SIZE;
	int i;
	glBegin(GL_POLYGON); //折れ線の描画開始
	for (i = startAngle; i <= endAngle; i += 360.0 / 36) {
		glVertex2d(
			pos_x + size * cos(3.14 * i / 180), //線分のx座標
			pos_y + size * sin(3.14 * i / 180)  //線分のy座標
			);
	}
	glEnd(); //描画終了
}

// PAC を表示
void drawPAC(double x, double y, double direc) {
	double pos_x = -1 + 2 * GRID_SIZE * x + GRID_SIZE;
	double pos_y = -1 + 2 * GRID_SIZE * y + GRID_SIZE;
	double size = GRID_SIZE;
	int i;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(pos_x, pos_y);
	for (i = 40 + 90 * direc; i <= 320 + 90 * direc; i += 360.0 / 36) {
		glVertex2d(
			pos_x + size * cos(3.14 * i / 180), //線分のx座標
			pos_y + size * sin(3.14 * i / 180)  //線分のy座標
			);
	}
	glEnd();

}

// 敵を描画
void drawEnemy(double x, double y) {
	double pos_x = -1 + 2 * GRID_SIZE * x + GRID_SIZE;
	double pos_y = -1 + 2 * GRID_SIZE * y + GRID_SIZE;
	double size = 0.8*GRID_SIZE;

	drawCircle(x, y, 0.8, 0, 180);
	glBegin(GL_POLYGON);
	glVertex2d(pos_x - size, pos_y);
	glVertex2d(pos_x + size, pos_y);
	glVertex2d(pos_x + size, pos_y - size);
	glVertex2d(pos_x - size, pos_y - size);
	glEnd();

	glColor3d(1.0, 1.0, 1.0);
	drawCircle(x, y, 0.4, 0, 360);
	glColor3d(0.0, 0.0, 0.0);
	drawCircle(x, y, 0.3, 0, 360);
}

void display(void) {
	int x, y;
	char str[20];
	int i;
	int count = 0;

	if(zanki > 0){

		glClear(GL_COLOR_BUFFER_BIT);

		// 壁を表示
		glColor3d(colwall[0], colwall[1], colwall[2]);
		for (x = 0; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				if (wall[stage][y][x] == 1)		// 配列wallが1なら壁を表示
					drawBlock(x, y);
			}
		}

		// エサを表示
		glColor3d(0.9, 0.9, 0.5);
		for (x = 0; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				if (wall[stage][y][x] == 2)		//配列wallが2ならエサを表示
					drawCircle(x, y, 0.2, 0, 360);
			}
		}

		// パワーエサを表示
		glColor3d(0.9, 0.9, 0.5);
		for (x = 0; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				if (wall[stage][y][x] == 3)		//配列wallが3ならパワーエサを表示
					drawCircle(x, y, 0.4, 0, 360);
			}
		}

		// プレイヤーを表示
		if(mutekiFlag == 0)	glColor3d(1.0, 1.0, 0.4);
		else				glColor3d(1.0, 0.4, 0.4);
		mouth++;
		if (mutekiCount % 2 == 0) {			// 無敵状態のとき点滅させる
			if (mouth % 5 == 0) {				// 口を開け閉めする
				drawCircle(pos_player[0], pos_player[1], 1, 15, 350);
				mouth = 0;
			}
			else {
				drawPAC(pos_player[0], pos_player[1], direction);
			}
		}

		// プレイヤーがエサを食べる描画
		if (wall[stage][pos_player[1]][pos_player[0]] == 2)	// エサの座標にプレイヤーが来るとエサを消す
			wall[stage][pos_player[1]][pos_player[0]] = 0;

		// プレイヤーがパワーエサを食べる描画 -> 無敵状態
		if (wall[stage][pos_player[1]][pos_player[0]] == 3) {	// パワーエサの座標にプレイヤーが来るとパワーエサを消す
			wall[stage][pos_player[1]][pos_player[0]] = 0;
			mutekiFlag = 1;				// 無敵フラグon
			mutekiCount = 0;			// パワーエサを取得する度にカウントをリセットする
			zanki++;					// 残機 + 1する
		}
		// 無敵終了
		if (mutekiCount == 15) {		// 15カウント後に無敵状態終了
			mutekiFlag = 0;
			mutekiCount = 0;
		}

		// 敵を描画
		for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++) {
			if (mutekiFlag == 0) glColor3d(colEnemy[enemyID][0], colEnemy[enemyID][1], colEnemy[enemyID][2]);
			else				 glColor3d(colEnemy[enemyID][0] - 0.4, colEnemy[enemyID][1] - 0.4, colEnemy[enemyID][2] - 0.4);
			drawEnemy(pos_enemy[enemyID][0], pos_enemy[enemyID][1]);
		}

		 // クリア後は判定をなくす　無敵のときも
		crush = 0;
		for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++)
			if ((pos_player[0] == pos_enemy[enemyID][0] && pos_player[1] == pos_enemy[enemyID][1]))
				if (ClearFlag == 0 && mutekiFlag == 0)
					crush = 1;

		//敵に衝突しているときは背景が赤になる 無敵状態の時は灰色
		if (mutekiFlag > 0) {
			glClearColor(0.5, 0.5, 0.5, 1.0);
		}else if (crush > 0 && precrush == 0) {
			glClearColor(1.0, 0.0, 0.0, 0.0);
			zanki--;								// 敵にあたったとき残機を減らす
		}else {
			glClearColor(0.0, 0.0, 0.0, 1.0);
		}
		precrush = crush;
		// 無敵カウントを表示
		if (mutekiFlag == 1) {
			mutekiCount++;
			sprintf(str, "muteki time:%03d", 15 - mutekiCount);
			glColor3f(1.0, 0.5, 0.6);
			glRasterPos2i(0.9, -1.0);
			for (i = 0; i < strlen(str); i++) {
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
			}
		}

		// エサがないマスをカウントする
		for (x = 0; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				if (wall[stage][y][x] == 0 || wall[stage][y][x] == 1)
					count++;
			}
		}

		// クリア判定
		if (count == 441) {					// すべてのマスにエサがなければ，クリアと判定する
			sprintf(str, "Game Clear!!");
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(0.1, 0.2);
			for (i = 0; i < strlen(str); i++) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			}
			ClearFlag = 1;
		}

		// 残機がなくなったときゲームオーバー
		if (zanki == 0) {
			glClearColor(0.0, 0.0, 0.0, 1.0);
			sprintf(str, "Game Over!!");
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(0.1, 0.2);
			for (i = 0; i < strlen(str); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			}
			sprintf(str, "Continue : C key");
			glColor3f(0.0, 1.0, 0.0);
			glRasterPos2f(0.1, 0.0);
			for (i = 0; i < strlen(str); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			}
			OverFlag = 1;
		}

		// スタート画面表示
		if (keySpace == 0) {
			sprintf(str, "Start : Space Key");
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(0.1, 0.2);
			for (i = 0; i < strlen(str); i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
			}
			///////////マップ変更/////////
			sprintf(str, "<-Map Select->");
			glColor3f(0.0, 1.0, 0.0);
			glRasterPos2f(0.1, 0.1);
			for (i = 0; i < strlen(str); i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
			}

			if (keyLeft == 1) {
				if (stage == 0)	stage=  STAGE_NUM -1;
				else			stage--;
			}
			if (keyRight == 1) {
				if(stage == STAGE_NUM -1)	stage = 0;
				else			stage++;
			}
		}
		
		// 残機表示
		for(i = 0; i < zanki; i++){
			glColor3d(1.0, 1.0, 0.4);
			drawPAC(0 + i, 0, 0);
		}

	glutSwapBuffers();

	}

}

void update(void) {
	int randMove = 0;
	int MoveFlag;
	int x, y;

	glutPostRedisplay();

	///////////マップ変更時，敵の位置に壁があったときワープさせる////////////
	for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++) {
		if (wall[stage][pos_enemy[enemyID][1]][pos_enemy[enemyID][0]] == 1){
			do {
				x = rand() % 22;
				y = rand() % 22;
			} while (wall[stage][y][x] == 1 || (x == 9 && y == 8));
			// 壁が存在しない座標x, yを確保し，敵の座標に代入する(プレイヤー出現場所にも出現させない)

			pos_enemy[enemyID][0] = x;
			pos_enemy[enemyID][1] = y;
		}
	}

	///////////////////スペースキーが押されたら全体が動作する////////////////////
	if (keySpace == 1) {

		//////////// プレイヤーの動く方向・向き//////////////////////
		if (keyUp == 1 && wall[stage][pos_player[1] + 1][pos_player[0]] != 1) {
			pos_player[1]++;
			direction = 1;
		}
		else if (keyDown == 1 && wall[stage][pos_player[1] - 1][pos_player[0]] != 1) {
			pos_player[1]--;
			direction = 3;
		}
		else if (keyLeft == 1 && wall[stage][pos_player[1]][pos_player[0] - 1] != 1) {
			pos_player[0]--;
			direction = 2;
		}
		else if (keyRight == 1 && wall[stage][pos_player[1]][pos_player[0] + 1] != 1) {
			pos_player[0]++;
			direction = 0;
		}

		// 画面の左右を行き来する
		if (pos_player[0] == 0)			pos_player[0] = 20;
		else if (pos_player[0] == 20)	pos_player[0] = 0;

		for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++) {
			if (pos_enemy[enemyID][0] == 0)			pos_enemy[enemyID][0] = 20;
			else if (pos_enemy[enemyID][0] == 20)	pos_enemy[enemyID][0] = 0;
		}

		////////////// 敵の動き //////////////////////////////////
		srand((unsigned)time(NULL));

		for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++) {
			MoveFlag = 0;

			while (MoveFlag == 0) {
				MoveFlag = 1;
				randMove = rand() % 12;
				if(mutekiFlag == 1)	randMove = rand() % 5 + 12;

				switch (randMove) {			// ランダムで4方向から一つを選び，そこに壁がなければ進む
				case 0:
					if (wall[stage][pos_enemy[enemyID][1] + 1][pos_enemy[enemyID][0]] != 1)
						pos_enemy[enemyID][1]++;
					else MoveFlag = 0;		// 進むことが出来なければ，方向を選ぶところからやり直す
					break;
				case 1:
					if (wall[stage][pos_enemy[enemyID][1] - 1][pos_enemy[enemyID][0]] != 1)
						pos_enemy[enemyID][1]--;
					else MoveFlag = 0;
					break;
				case 2:
					if (wall[stage][pos_enemy[enemyID][1]][pos_enemy[enemyID][0] - 1] != 1)
						pos_enemy[enemyID][0]--;
					else MoveFlag = 0;
					break;
				case 3:
					if (wall[stage][pos_enemy[enemyID][1]][pos_enemy[enemyID][0] + 1] != 1)
						pos_enemy[enemyID][0]++;
					else MoveFlag = 0;
					break;

			// 敵が自機に向かってくる（確率1/4）
				case 4:
				case 5:
					if (wall[stage][pos_enemy[enemyID][1] + 1][pos_enemy[enemyID][0]] != 1 && pos_enemy[enemyID][1] < pos_player[1])
						pos_enemy[enemyID][1]++;
					else MoveFlag = 0;		// 進むことが出来なければ，方向を選ぶところからやり直す
					break;
				case 6:
				case 7:
					if (wall[stage][pos_enemy[enemyID][1] - 1][pos_enemy[enemyID][0]] != 1 && pos_enemy[enemyID][1] > pos_player[1])
						pos_enemy[enemyID][1]--;
					else MoveFlag = 0;
					break;
				case 8:
				case 9:
					if (wall[stage][pos_enemy[enemyID][1]][pos_enemy[enemyID][0] - 1] != 1 && pos_enemy[enemyID][0] > pos_player[0])
						pos_enemy[enemyID][0]--;
					else MoveFlag = 0;
					break;
				case 10:
				case 11:
					if (wall[stage][pos_enemy[enemyID][1]][pos_enemy[enemyID][0] + 1] != 1 && pos_enemy[enemyID][0] < pos_player[0])
						pos_enemy[enemyID][0]++;
					else MoveFlag = 0;
					break;

			//無敵状態の時の敵の動き
				case 12:
					if (wall[stage][pos_enemy[enemyID][1] + 1][pos_enemy[enemyID][0]] != 1 && pos_enemy[enemyID][1] > pos_player[1])
						pos_enemy[enemyID][1]++;
					else MoveFlag = 0;		// 進むことが出来なければ，方向を選ぶところからやり直す
					break;
				case 13:
					if (wall[stage][pos_enemy[enemyID][1] - 1][pos_enemy[enemyID][0]] != 1 && pos_enemy[enemyID][1] < pos_player[1])
						pos_enemy[enemyID][1]--;
					else MoveFlag = 0;
					break;
				case 14:
					if (wall[stage][pos_enemy[enemyID][1]][pos_enemy[enemyID][0] - 1] != 1 && pos_enemy[enemyID][0] < pos_player[0])
						pos_enemy[enemyID][0]--;
					else MoveFlag = 0;
					break;
				case 15:
					if (wall[stage][pos_enemy[enemyID][1]][pos_enemy[enemyID][0] + 1] != 1 && pos_enemy[enemyID][0] > pos_player[0])
						pos_enemy[enemyID][0]++;
					else MoveFlag = 0;
					break;
				case 16:
					MoveFlag = 1;
				}
			}
		}
	}

	////////////////////コンティニュー////////////////////////////////
	if((ClearFlag == 1 || OverFlag == 1) && keyC == 1)
		init();

	// 150ms後にこの関数を実行
	glutTimerFunc(150, update, 0);
}


int main(int argc, char *argv[]) {
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(450, 450);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutTimerFunc(100, update, 0);

	// 特殊キーが押された（離された）際の処理を行う関数を指定
	glutSpecialFunc(checkSpecialKeyPressed);
	glutSpecialUpFunc(checkSpecialKeyReleased);
	glutKeyboardFunc(KeyPressed);
	glutKeyboardUpFunc(KeyReleased);

	init();
	glutMainLoop();
	return EXIT_SUCCESS;
}
