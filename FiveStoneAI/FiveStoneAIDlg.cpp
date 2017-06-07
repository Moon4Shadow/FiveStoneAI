
// FiveStoneAIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiveStoneAI.h"
#include "FiveStoneAIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//CRect Rect;              //���ػ�����
//CRect RegretRect[2];     //���壬���ػ�����

CImage g_img_black;
CImage g_img_white;
CImage g_img_board;

deque<StonePos> g_stonemove;
int g_chessboard[19][19];
int g_stonecolor = BLACK;

// CFiveStoneAIDlg �Ի���
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


// CFiveStoneAIDlg ��Ϣ�������

BOOL CFiveStoneAIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	//��ʼ������
	InitGame();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFiveStoneAIDlg::OnPaint()
{
	CPaintDC dc(this);

	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// ��������
		DrawChess(&dc);
		CDialogEx::OnPaint();

		// ���Ӻ�
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFiveStoneAIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�����������Ϣ��Ӧ
void CFiveStoneAIDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	StonePos stone;

	//���ӳɹ��������ӵ���ӵ����Ӷ�����,�����ػ���Ϣ
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
	//����ͼƬ��Դ
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

	//������Ӷ���
	g_stonemove.clear();

	//��ʼ������״̬
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

		//���ػ�������Rect���򲿷�
		g_img_board.Draw(pDC->m_hDC, tRect, tRect);

		//���ٸ������������
		if (g_chessboard[it->xPos][it->yPos] == WHITE)
		{
			g_img_white.Draw(pDC->m_hDC, tRect);
		}
		else if (g_chessboard[it->xPos][it->yPos] == BLACK)
		{
			g_img_black.Draw(pDC->m_hDC, tRect);
		}

		//����������ϻ��Ƶ�ǰ���岽��
		CString str;
		str.Format("%d", index);

		CFont font;
		CFont *pOldFont;                      // �����豸���������ʹ�õ��������
		CRect stepRect;

		//��stepRect�ڣ����Ӳ���
		stepRect.left = tRect.left;
		stepRect.top = tRect.top + 7;
		stepRect.right = tRect.right;
		stepRect.bottom = tRect.bottom;

		font.CreatePointFont(100, "����");
		pOldFont = pDC->SelectObject(&font);
		pDC->SetBkMode(TRANSPARENT);             // ���ĵ�ǰ����
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->DrawText(str, &stepRect, DT_CENTER);
		pDC->SelectObject(pOldFont);             //��ԭ����
	}
}

void CFiveStoneAIDlg::AfterDraw()
{
	if (g_stonemove.empty())
	{
		return;
	}

	// �ж�ʤ��
	if (IsGameOver(g_stonemove.back()))
	{
		// ������ʾʤ��
		if (g_stonecolor == WHITE)
		{
			AfxMessageBox("�����ʤ��");
		}
		else
		{
			AfxMessageBox("�����ʤ��");
		}

		// ��������
		g_stonecolor = BLACK;
		InitGame();
		RedrawWindow();

		return;
	}
	else
	{
		// ��������
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
		//������µ����ӵ�
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

		//ȷʵ����Ƿ�λ�ڿ����ӵ㷶Χ��
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
	//���ҷ������·����������·����������Ϸ���
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