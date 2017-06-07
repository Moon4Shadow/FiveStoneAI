
// FiveStoneAIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FiveStoneAI.h"
#include "FiveStoneAIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//CRect Rect;              //待重绘区域
//CRect RegretRect[2];     //悔棋，待重绘区域

CImage g_img_black;
CImage g_img_white;
CImage g_img_board;

deque<StonePos> g_stonemove;
int g_chessboard[19][19];
int g_stonecolor = BLACK;

// CFiveStoneAIDlg 对话框
CFiveStoneAIDlg::CFiveStoneAIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FIVESTONEAI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFiveStoneAIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFiveStoneAIDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CFiveStoneAIDlg 消息处理程序

BOOL CFiveStoneAIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	//初始化数据
	InitGame();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFiveStoneAIDlg::OnPaint()
{
	CPaintDC dc(this);

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 绘制棋盘
		DrawChess(&dc);
		CDialogEx::OnPaint();

		// 落子后
		AfterDraw();
	}
}

void CFiveStoneAIDlg::OnClose()
{
	if (!g_img_board.IsNull())
	{
		g_img_board.Destroy();
	}
	if (!g_img_black.IsNull())
	{
		g_img_black.Destroy();
	}
	if (!g_img_white.IsNull())
	{
		g_img_white.Destroy();
	}

	if (!g_stonemove.empty())
	{
		g_stonemove.clear();
	}

	OnCancel();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFiveStoneAIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//鼠标左键点击消息响应
void CFiveStoneAIDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	StonePos stone;

	//落子成功，则将落子点添加到落子队列中,发送重绘消息
	if (CheckStoneDown(stone, point.x, point.y))
	{
		//IsMyStoneDown = true;

		g_stonemove.push_back(stone);
		g_chessboard[stone.xPos][stone.yPos] = g_stonecolor;

		RedrawWindow();
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CFiveStoneAIDlg::InitGame()
{
	//加载图片资源
	if (!g_img_board.IsNull())
	{
		g_img_board.Destroy();
	}
	g_img_board.Load("res//ChessBoard.png");

	if (!g_img_black.IsNull())
	{
		g_img_black.Destroy();
	}
	g_img_black.Load("res//black.png");
	PreMultipliedAlpha(g_img_black);

	if (!g_img_white.IsNull())
	{
		g_img_white.Destroy();
	}
	g_img_white.Load("res//white.png");
	PreMultipliedAlpha(g_img_white);

	//清空落子队列
	g_stonemove.clear();

	//初始化棋盘状态
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
		{
			g_chessboard[i][j] = NOSTONE;
		}
}

void CFiveStoneAIDlg::DrawChess(CPaintDC *pDC)
{
	g_img_board.Draw(pDC->m_hDC, 0, 0, ChessBoardPicSize, ChessBoardPicSize);

	deque<StonePos>::const_iterator it = g_stonemove.begin();
	int index = 1;
	for (; it != g_stonemove.end(); it++, index++)
	{
		CRect tRect;
		tRect.left = 29 * it->xPos;
		tRect.top = 29 * it->yPos;
		tRect.right = 29 * it->xPos + StoneSize;
		tRect.bottom = 29 * it->yPos + StoneSize;

		//先重绘棋盘在Rect区域部分
		g_img_board.Draw(pDC->m_hDC, tRect, tRect);

		//再再该区域绘制棋子
		if (g_chessboard[it->xPos][it->yPos] == WHITE)
		{
			g_img_white.Draw(pDC->m_hDC, tRect);
		}
		else if (g_chessboard[it->xPos][it->yPos] == BLACK)
		{
			g_img_black.Draw(pDC->m_hDC, tRect);
		}

		//最后在棋子上绘制当前下棋步骤
		CString str;
		str.Format("%d", index);

		CFont font;
		CFont *pOldFont;                      // 保存设备上下文最初使用的字体对象
		CRect stepRect;

		//在stepRect内，落子步骤
		stepRect.left = tRect.left;
		stepRect.top = tRect.top + 7;
		stepRect.right = tRect.right;
		stepRect.bottom = tRect.bottom;

		font.CreatePointFont(100, "隶书");
		pOldFont = pDC->SelectObject(&font);
		pDC->SetBkMode(TRANSPARENT);             // 更改当前画笔
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->DrawText(str, &stepRect, DT_CENTER);
		pDC->SelectObject(pOldFont);             //还原画笔
	}
}

void CFiveStoneAIDlg::AfterDraw()
{
	if (g_stonemove.empty())
	{
		return;
	}

	// 判断胜负
	if (IsGameOver(g_stonemove.back()))
	{
		// 弹窗显示胜负
		if (g_stonecolor == WHITE)
		{
			AfxMessageBox("白棋获胜！");
		}
		else
		{
			AfxMessageBox("黑棋获胜！");
		}

		// 重置数据
		g_stonecolor = BLACK;
		InitGame();
		RedrawWindow();

		return;
	}
	else
	{
		// 交替落子
		g_stonecolor = -g_stonecolor;
	}
	
}

void PreMultipliedAlpha(CImage &img)
{
	for (int i = 0; i < img.GetWidth(); i++)
	{
		for (int j = 0; j < img.GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(img.GetPixelAddress(i, j));
			pucColor[0] = (pucColor[0] * pucColor[3] + 127) / 255;
			pucColor[1] = (pucColor[1] * pucColor[3] + 127) / 255;
			pucColor[2] = (pucColor[2] * pucColor[3] + 127) / 255;
		}
	}
}

bool CheckStoneDown(StonePos &stone, const int xpos, const int ypos)
{
	if (xpos <= ChessBoardSize && ypos <= ChessBoardSize)
	{
		//计算大致的落子点
		stone.xPos = xpos / StoneSize;
		stone.yPos = ypos / StoneSize;
		if (g_chessboard[stone.xPos][stone.yPos] != NOSTONE)
		{
			return false;
		}

		int xUp = 20 + StoneSize * stone.xPos;
		int xLow = 10 + StoneSize * stone.xPos;
		int yUp = 20 + StoneSize * stone.yPos;
		int yLow = 10 + StoneSize * stone.yPos;

		//确实落点是否位于可落子点范围内
		if (xpos >= xLow && xpos <= xUp &&
			ypos >= yLow && ypos <= yUp)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

int CountOfStones(StonePos &pos, Direction dir)
{
	int i = 1, count = 1;
	bool block[2];
	block[0] = false;
	block[1] = false;

	while (1)
	{
		if (pos.xPos - i < 0 || pos.yPos - i < 0)
		{
			block[0] = true;
		}
		if (pos.xPos + i > GRID_NUM - 1 || pos.yPos + i > GRID_NUM - 1)
		{
			block[1] = true;
		}

		switch (dir)
		{
		case L2R:
			if (!block[0] || !block[1])
			{
				if (!block[0] && g_chessboard[pos.xPos - i][pos.yPos] == g_chessboard[pos.xPos][pos.yPos])
				{
					count++;
				}
				else
				{
					block[0] = true;
				}

				if (!block[1] && g_chessboard[pos.xPos + i][pos.yPos] == g_chessboard[pos.xPos][pos.yPos])
				{
					count++;
				}
				else
				{
					block[1] = true;
				}
			}
			break;
		case U2D:
			if (!block[0] || !block[1])
			{
				if (!block[0] && g_chessboard[pos.xPos][pos.yPos - i] == g_chessboard[pos.xPos][pos.yPos])
				{
					count++;
				}
				else
				{
					block[0] = true;
				}

				if (!block[1] && g_chessboard[pos.xPos][pos.yPos + i] == g_chessboard[pos.xPos][pos.yPos])
				{
					count++;
				}
				else
					block[1] = true;
			}
			break;
		case LU2RD:
			if (!block[0] || !block[1])
			{
				if (!block[0] && g_chessboard[pos.xPos - i][pos.yPos - i] == g_chessboard[pos.xPos][pos.yPos])
					count++;
				else
					block[0] = true;

				if (!block[1] && g_chessboard[pos.xPos + i][pos.yPos + i] == g_chessboard[pos.xPos][pos.yPos])
					count++;
				else
				{
					block[1] = true;
				}
			}
			break;
		case LD2RU:
			if (!block[0] || !block[1])
			{
				if (!block[0] && g_chessboard[pos.xPos - i][pos.yPos + i] == g_chessboard[pos.xPos][pos.yPos])
				{
					count++;
				}
				else
				{
					block[0] = true;
				}

				if (!block[1] && g_chessboard[pos.xPos + i][pos.yPos - i] == g_chessboard[pos.xPos][pos.yPos])
				{
					count++;
				}
				else
				{
					block[1] = true;
				}
			}
			break;
		default:
			break;
		}
		if (block[0] && block[1])
		{
			break;
		}
		else
		{
			i++;
		}
	}

	return count;
}

bool IsGameOver(StonePos pos)
{
	//左右方向、上下方向、左上右下方向、左下右上方向
	if (CountOfStones(pos, L2R) >= 5 ||
		CountOfStones(pos, U2D) >= 5 ||
		CountOfStones(pos, LU2RD) >= 5 ||
		CountOfStones(pos, LD2RU) >= 5)
	{
		//battleStatus = GameOver;
		return true;
	}
	return false;
}