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

// �v���C���[�ϐ�
int pos_player[2] = { 9, 8 };	// �v���C���[�̍��W���i�[����z��
int mouth = 0;			// �v���C���[�̕\����Ԃ�ς���ϐ�
int direction = 0;		// �v���C���[�̌�����ۑ�����ϐ�
int mutekiFlag = 0;
int mutekiCount = 0;
int zanki = 3;

// �G
int pos_enemy[ENEMY_NUM][2];
int enemyID;
GLfloat colEnemy[ENEMY_NUM][3];

//�����蔻��
int crush = 0;
int precrush = 0;

// �e�t���O
int ClearFlag = 0;
int OverFlag = 0;

// �L�[�̏�Ԃ�\���O���[�o���ϐ�(0: ������Ă��Ȃ�, 1: ������Ă���)
int keyUp = 0, keyDown = 0, keyLeft = 0, keyRight = 0, keySpace = 0, keyC = 0;

// �Œ�u���b�N
int stage;
double colwall[3] = { 0.3, 0.3, 1.0 };
int wall[STAGE_NUM][HEIGHT][WIDTH] = {		// x,y���W�����킹�邽�߂ɖ{�����ɂ�����̂��ɏ�������
									//	0:�����Ȃ��ʘH 1:�� 2:�G�T	3:�p���[�A�b�v
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
		// �ǂ����݂��Ȃ����Wx, y���m�ۂ��C�G�̍��W�ɑ������(�v���C���[�o���ꏊ�ɂ��o�������Ȃ�)

		pos_enemy[enemyID][0] = x;
		pos_enemy[enemyID][1] = y;

		for (i = 0; i < 3; i++)
			colEnemy[enemyID][i] = (double)rand() / RAND_MAX / 2 + 0.4;		// �w�i�Ɠ������Ȃ��悤�ɖ��邢�F�ɂ���
	}
	// �v���C���[�ϐ�
	pos_player[0] = 9;
	pos_player[1] = 8;	// �v���C���[�̍��W���i�[����z��
	mouth = 0;			// �v���C���[�̕\����Ԃ�ς���ϐ�
	direction = 0;		// �v���C���[�̌�����ۑ�����ϐ�
	mutekiFlag = 0;
	mutekiCount = 0;
	zanki = 3;

	//�����蔻��
	crush = 0;
	precrush = 0;

	// �e�t���O
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

// �X�y�[�X�L�[, C�������ꂽ���ǂ���
void KeyPressed(unsigned char key, int x, int y) {
	if (key == 32)	keySpace = 1;
	if (key == 99)	keyC = 1;
}
// �X�y�[�X�L�[, C�������ꂽ���ǂ���
void KeyReleased(unsigned char key, int x, int y) {
/*	if (key == 32)	keySpace = 0;	*/
	if (key == 99)	keyC = 0;
}

// ����L�[�������ꂽ�ۂ̏������s���֐����w��
void checkSpecialKeyPressed(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: keyUp = 1; break;
	case GLUT_KEY_DOWN: keyDown = 1; break;
	case GLUT_KEY_LEFT: keyLeft = 1; break;
	case GLUT_KEY_RIGHT: keyRight = 1; break;
	}
}

// ����L�[���߂��ꂽ�ۂ̏������s���֐����w��
void checkSpecialKeyReleased(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: keyUp = 0; break;
	case GLUT_KEY_DOWN: keyDown = 0; break;
	case GLUT_KEY_LEFT: keyLeft = 0; break;
	case GLUT_KEY_RIGHT: keyRight = 0; break;
	}
}

// �u���b�N��`��
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

//�~�`��֐�(�h��Ԃ�)
void drawCircle(double x, double y, double radius, int startAngle, int endAngle) {
	double pos_x = -1 + 2 * GRID_SIZE * x + GRID_SIZE;
	double pos_y = -1 + 2 * GRID_SIZE * y + GRID_SIZE;
	double size = radius * GRID_SIZE;
	int i;
	glBegin(GL_POLYGON); //�܂���̕`��J�n
	for (i = startAngle; i <= endAngle; i += 360.0 / 36) {
		glVertex2d(
			pos_x + size * cos(3.14 * i / 180), //������x���W
			pos_y + size * sin(3.14 * i / 180)  //������y���W
			);
	}
	glEnd(); //�`��I��
}

// PAC ��\��
void drawPAC(double x, double y, double direc) {
	double pos_x = -1 + 2 * GRID_SIZE * x + GRID_SIZE;
	double pos_y = -1 + 2 * GRID_SIZE * y + GRID_SIZE;
	double size = GRID_SIZE;
	int i;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(pos_x, pos_y);
	for (i = 40 + 90 * direc; i <= 320 + 90 * direc; i += 360.0 / 36) {
		glVertex2d(
			pos_x + size * cos(3.14 * i / 180), //������x���W
			pos_y + size * sin(3.14 * i / 180)  //������y���W
			);
	}
	glEnd();

}

// �G��`��
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

		// �ǂ�\��
		glColor3d(colwall[0], colwall[1], colwall[2]);
		for (x = 0; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				if (wall[stage][y][x] == 1)		// �z��wall��1�Ȃ�ǂ�\��
					drawBlock(x, y);
			}
		}

		// �G�T��\��
		glColor3d(0.9, 0.9, 0.5);
		for (x = 0; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				if (wall[stage][y][x] == 2)		//�z��wall��2�Ȃ�G�T��\��
					drawCircle(x, y, 0.2, 0, 360);
			}
		}

		// �p���[�G�T��\��
		glColor3d(0.9, 0.9, 0.5);
		for (x = 0; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				if (wall[stage][y][x] == 3)		//�z��wall��3�Ȃ�p���[�G�T��\��
					drawCircle(x, y, 0.4, 0, 360);
			}
		}

		// �v���C���[��\��
		if(mutekiFlag == 0)	glColor3d(1.0, 1.0, 0.4);
		else				glColor3d(1.0, 0.4, 0.4);
		mouth++;
		if (mutekiCount % 2 == 0) {			// ���G��Ԃ̂Ƃ��_�ł�����
			if (mouth % 5 == 0) {				// �����J���߂���
				drawCircle(pos_player[0], pos_player[1], 1, 15, 350);
				mouth = 0;
			}
			else {
				drawPAC(pos_player[0], pos_player[1], direction);
			}
		}

		// �v���C���[���G�T��H�ׂ�`��
		if (wall[stage][pos_player[1]][pos_player[0]] == 2)	// �G�T�̍��W�Ƀv���C���[������ƃG�T������
			wall[stage][pos_player[1]][pos_player[0]] = 0;

		// �v���C���[���p���[�G�T��H�ׂ�`�� -> ���G���
		if (wall[stage][pos_player[1]][pos_player[0]] == 3) {	// �p���[�G�T�̍��W�Ƀv���C���[������ƃp���[�G�T������
			wall[stage][pos_player[1]][pos_player[0]] = 0;
			mutekiFlag = 1;				// ���G�t���Oon
			mutekiCount = 0;			// �p���[�G�T���擾����x�ɃJ�E���g�����Z�b�g����
			zanki++;					// �c�@ + 1����
		}
		// ���G�I��
		if (mutekiCount == 15) {		// 15�J�E���g��ɖ��G��ԏI��
			mutekiFlag = 0;
			mutekiCount = 0;
		}

		// �G��`��
		for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++) {
			if (mutekiFlag == 0) glColor3d(colEnemy[enemyID][0], colEnemy[enemyID][1], colEnemy[enemyID][2]);
			else				 glColor3d(colEnemy[enemyID][0] - 0.4, colEnemy[enemyID][1] - 0.4, colEnemy[enemyID][2] - 0.4);
			drawEnemy(pos_enemy[enemyID][0], pos_enemy[enemyID][1]);
		}

		 // �N���A��͔�����Ȃ����@���G�̂Ƃ���
		crush = 0;
		for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++)
			if ((pos_player[0] == pos_enemy[enemyID][0] && pos_player[1] == pos_enemy[enemyID][1]))
				if (ClearFlag == 0 && mutekiFlag == 0)
					crush = 1;

		//�G�ɏՓ˂��Ă���Ƃ��͔w�i���ԂɂȂ� ���G��Ԃ̎��͊D�F
		if (mutekiFlag > 0) {
			glClearColor(0.5, 0.5, 0.5, 1.0);
		}else if (crush > 0 && precrush == 0) {
			glClearColor(1.0, 0.0, 0.0, 0.0);
			zanki--;								// �G�ɂ��������Ƃ��c�@�����炷
		}else {
			glClearColor(0.0, 0.0, 0.0, 1.0);
		}
		precrush = crush;
		// ���G�J�E���g��\��
		if (mutekiFlag == 1) {
			mutekiCount++;
			sprintf(str, "muteki time:%03d", 15 - mutekiCount);
			glColor3f(1.0, 0.5, 0.6);
			glRasterPos2i(0.9, -1.0);
			for (i = 0; i < strlen(str); i++) {
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
			}
		}

		// �G�T���Ȃ��}�X���J�E���g����
		for (x = 0; x < WIDTH; x++) {
			for (y = 0; y < HEIGHT; y++) {
				if (wall[stage][y][x] == 0 || wall[stage][y][x] == 1)
					count++;
			}
		}

		// �N���A����
		if (count == 441) {					// ���ׂẴ}�X�ɃG�T���Ȃ���΁C�N���A�Ɣ��肷��
			sprintf(str, "Game Clear!!");
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(0.1, 0.2);
			for (i = 0; i < strlen(str); i++) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
			}
			ClearFlag = 1;
		}

		// �c�@���Ȃ��Ȃ����Ƃ��Q�[���I�[�o�[
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

		// �X�^�[�g��ʕ\��
		if (keySpace == 0) {
			sprintf(str, "Start : Space Key");
			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(0.1, 0.2);
			for (i = 0; i < strlen(str); i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
			}
			///////////�}�b�v�ύX/////////
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
		
		// �c�@�\��
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

	///////////�}�b�v�ύX���C�G�̈ʒu�ɕǂ��������Ƃ����[�v������////////////
	for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++) {
		if (wall[stage][pos_enemy[enemyID][1]][pos_enemy[enemyID][0]] == 1){
			do {
				x = rand() % 22;
				y = rand() % 22;
			} while (wall[stage][y][x] == 1 || (x == 9 && y == 8));
			// �ǂ����݂��Ȃ����Wx, y���m�ۂ��C�G�̍��W�ɑ������(�v���C���[�o���ꏊ�ɂ��o�������Ȃ�)

			pos_enemy[enemyID][0] = x;
			pos_enemy[enemyID][1] = y;
		}
	}

	///////////////////�X�y�[�X�L�[�������ꂽ��S�̂����삷��////////////////////
	if (keySpace == 1) {

		//////////// �v���C���[�̓��������E����//////////////////////
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

		// ��ʂ̍��E���s��������
		if (pos_player[0] == 0)			pos_player[0] = 20;
		else if (pos_player[0] == 20)	pos_player[0] = 0;

		for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++) {
			if (pos_enemy[enemyID][0] == 0)			pos_enemy[enemyID][0] = 20;
			else if (pos_enemy[enemyID][0] == 20)	pos_enemy[enemyID][0] = 0;
		}

		////////////// �G�̓��� //////////////////////////////////
		srand((unsigned)time(NULL));

		for (enemyID = 0; enemyID < ENEMY_NUM; enemyID++) {
			MoveFlag = 0;

			while (MoveFlag == 0) {
				MoveFlag = 1;
				randMove = rand() % 12;
				if(mutekiFlag == 1)	randMove = rand() % 5 + 12;

				switch (randMove) {			// �����_����4����������I�сC�����ɕǂ��Ȃ���ΐi��
				case 0:
					if (wall[stage][pos_enemy[enemyID][1] + 1][pos_enemy[enemyID][0]] != 1)
						pos_enemy[enemyID][1]++;
					else MoveFlag = 0;		// �i�ނ��Ƃ��o���Ȃ���΁C������I�ԂƂ��납���蒼��
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

			// �G�����@�Ɍ������Ă���i�m��1/4�j
				case 4:
				case 5:
					if (wall[stage][pos_enemy[enemyID][1] + 1][pos_enemy[enemyID][0]] != 1 && pos_enemy[enemyID][1] < pos_player[1])
						pos_enemy[enemyID][1]++;
					else MoveFlag = 0;		// �i�ނ��Ƃ��o���Ȃ���΁C������I�ԂƂ��납���蒼��
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

			//���G��Ԃ̎��̓G�̓���
				case 12:
					if (wall[stage][pos_enemy[enemyID][1] + 1][pos_enemy[enemyID][0]] != 1 && pos_enemy[enemyID][1] > pos_player[1])
						pos_enemy[enemyID][1]++;
					else MoveFlag = 0;		// �i�ނ��Ƃ��o���Ȃ���΁C������I�ԂƂ��납���蒼��
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

	////////////////////�R���e�B�j���[////////////////////////////////
	if((ClearFlag == 1 || OverFlag == 1) && keyC == 1)
		init();

	// 150ms��ɂ��̊֐������s
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

	// ����L�[�������ꂽ�i�����ꂽ�j�ۂ̏������s���֐����w��
	glutSpecialFunc(checkSpecialKeyPressed);
	glutSpecialUpFunc(checkSpecialKeyReleased);
	glutKeyboardFunc(KeyPressed);
	glutKeyboardUpFunc(KeyReleased);

	init();
	glutMainLoop();
	return EXIT_SUCCESS;
}
