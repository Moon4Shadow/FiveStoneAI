#pragma once
#include <deque>
using namespace std;

//����궨�������Դ��������͵�����
#define STWO		1  //�߶�s
#define TWO			2  //���
#define STHREE		3  //����
#define THREE		4  //����
#define SFOUR		5  //����
#define FOUR		6  //����
#define FIVE		7  //����

#define SCORE_STWO		    25     //�߶�
#define SCORE_TWO			90     //���
#define SCORE_STHREE		190    //����
#define SCORE_THREE		    1250   //����
#define SCORE_SFOUR		    2500   //����
#define SCORE_FOUR		    5000   //����
#define SCORE_FIVE		    10000  //����

#define MATE 20000

#define NOTYPE		11 //δ����
#define ANALSISED   255//�ѷ�������
#define TOBEANALSIS 0  //�ѷ�������

#define GRID_NUM 19
#define GRID_COUNT 361

#define StoneSize 29
#define ChessBoardSize 550
#define ChessBoardPicSize 566

#define BLACK  1
#define WHITE  -1
#define NOSTONE 0    //û������

//�ػ�����
enum RePaintType {
	NewGame = 0x1000,    //����Ϸ��
	StoneDown,           //����
	Regret               //����
};

enum Direction
{
	L2R = 1,
	U2D,
	LU2RD,
	LD2RU
};


//�ýṹ���ڼ�¼����λ��
typedef struct Position {
	int xPos;             //��Ӧ������ĸ������
	int yPos;             //��Ӧ��������������
}StonePos, *pStonePos;

typedef struct StoneMove
{
	StonePos	sPos;
	int		    score;
}StoneMove, *pStoneMove;
