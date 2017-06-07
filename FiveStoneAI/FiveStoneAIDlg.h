
// FiveStoneAIDlg.h : 头文件
//

#pragma once

#include "Global.h"


// CFiveStoneAIDlg 对话框
class CFiveStoneAIDlg : public CDialogEx
{
// 构造
public:
	CFiveStoneAIDlg(CWnd* pParent = NULL);	// 标准构造函数
										
	void InitGame();                        //初始化游戏
	void DrawChess(CPaintDC *pDC);          //绘制棋盘和棋子
	void AfterDraw();                       //判断胜负，交替落子

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIVESTONEAI_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};

// 对PNG图片进行Alpha预乘,还原其透明显示
void PreMultipliedAlpha(CImage &img);

//验证落子有效性
bool CheckStoneDown(StonePos &stone, const int xpos, const int ypos);

//计算某位置棋子在不同方向上的最大同色棋子相连数
int CountOfStones(StonePos &pos, Direction dir);

//判断对弈是否结束
bool IsGameOver(StonePos pos);