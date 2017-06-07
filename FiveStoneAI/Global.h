#pragma once
#include <deque>
using namespace std;

//这组宏定义了用以代表几种棋型的数字
#define STWO		1  //眠二s
#define TWO			2  //活二
#define STHREE		3  //眠三
#define THREE		4  //活三
#define SFOUR		5  //冲四
#define FOUR		6  //活四
#define FIVE		7  //五连

#define SCORE_STWO		    25     //眠二
#define SCORE_TWO			90     //活二
#define SCORE_STHREE		190    //眠三
#define SCORE_THREE		    1250   //活三
#define SCORE_SFOUR		    2500   //冲四
#define SCORE_FOUR		    5000   //活四
#define SCORE_FIVE		    10000  //五连

#define MATE 20000

#define NOTYPE		11 //未定义
#define ANALSISED   255//已分析过的
#define TOBEANALSIS 0  //已分析过的

#define GRID_NUM 19
#define GRID_COUNT 361

#define StoneSize 29
#define ChessBoardSize 550
#define ChessBoardPicSize 566

#define BLACK  1
#define WHITE  -1
#define NOSTONE 0    //没有棋子

//重绘条件
enum RePaintType {
	NewGame = 0x1000,    //新游戏，
	StoneDown,           //落子
	Regret               //悔棋
};

enum Direction
{
	L2R = 1,
	U2D,
	LU2RD,
	LD2RU
};


//该结构用于记录落子位置
typedef struct Position {
	int xPos;             //对应棋盘字母坐标轴
	int yPos;             //对应棋盘数字坐标轴
}StonePos, *pStonePos;

typedef struct StoneMove
{
	StonePos	sPos;
	int		    score;
}StoneMove, *pStoneMove;
