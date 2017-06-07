
// FiveStoneAIDlg.h : ͷ�ļ�
//

#pragma once

#include "Global.h"


// CFiveStoneAIDlg �Ի���
class CFiveStoneAIDlg : public CDialogEx
{
// ����
public:
	CFiveStoneAIDlg(CWnd* pParent = NULL);	// ��׼���캯��
										
	void InitGame();                        //��ʼ����Ϸ
	void DrawChess(CPaintDC *pDC);          //�������̺�����
	void AfterDraw();                       //�ж�ʤ������������

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIVESTONEAI_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};

// ��PNGͼƬ����AlphaԤ��,��ԭ��͸����ʾ
void PreMultipliedAlpha(CImage &img);

//��֤������Ч��
bool CheckStoneDown(StonePos &stone, const int xpos, const int ypos);

//����ĳλ�������ڲ�ͬ�����ϵ����ͬɫ����������
int CountOfStones(StonePos &pos, Direction dir);

//�ж϶����Ƿ����
bool IsGameOver(StonePos pos);