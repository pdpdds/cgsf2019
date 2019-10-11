
#pragma once
#include "DXUTguiIME.h"
#include "DXUTgui.h"
#include "ScrollBarCtrl.h"
#include <vector>

using namespace std;

// [6/24/2008 Psj] �ؽ�Ʈ ���δ� ��������, ���� ĥ�� nEnterW�� L"\r\n" ���̰� ����.
typedef struct _TStrLineInfo
{
	int nTextW;
	int nTextNum;
	int nEnterW;
	_TStrLineInfo(int _nTextW=0,int _nTextNum=0,int _nEnterW=0)
	{
		nTextW = _nTextW;
		nTextNum = _nTextNum;
		nEnterW = _nEnterW;
	}
}TStrLineInfo;

#define VSTRLINEINFO vector<TStrLineInfo>
#define ITSTRLINEINFO vector<TStrLineInfo>::iterator

class CTextBoxCtrl : public CDXUTIMEEditBox
{

protected:

	/*/////////////////////////////////////////////////////////////////////
	m_nBeforeStrNumSav - [6/26/2008 Psj] ���� ���۽�Ʈ�� �����
	m_nSBWidth - ��ũ�ѹ� ����
	m_nSelected - Index of the selected item for single selection list box
	*///////////////////////////////////////////////////////////////////////

	int m_nShadow;
	int m_nCaretNowCol;
	int m_nHeightPassCaret;
	int m_nbuffWordSiz;
	int m_nSelStartSizX;
	int m_nBeforeStrNumSav;
	int m_nTextWidth;
	int m_nTextHeight;
	int m_mEnterWidth;
	int m_nSBWidth;     
	int m_nSelected;   
	int m_nCaretX;
	int m_nCaretLine;

	bool m_bSBInit;
	RECT m_RcCaret;
	D3DCOLOR m_ShadowFontColor;
	CScrollBarCtrl  m_ScrollBar;
	VSTRLINEINFO  m_vStrLineInfo;

public:

	/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	GetScrollPosToCaretShow // ��ũ����ġ�� ���� caret�� ���������� Ȯ�����ش�.
	SetScrollPosMovCaretPos // ��ũ�� ��ġ�� caret�� �ִ°����� �̵���Ų��.
	SetCaretNowCol // ������ ���ΰ��� �ٷ� �����Ѵ�.
	*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CTextBoxCtrl(CDXUTDialog *pDialog);
	~CTextBoxCtrl();
	int GetCaretNowCol(){return m_nCaretNowCol;}
	void Init(LPCWSTR pwszText, int nX, int nY, int nW, int nH, UINT nHotKey=NULL, bool bDefault=NULL);
	void Init(LPCWSTR pwszText, RECT &rcCtrl, UINT nHotKey=NULL, bool bDefault=NULL);
	void SetSpriteToScrollBar(DWORD nPackageID, LPCWSTR wszAnimation,int nBackImg, int nSBarBtnUp,int nSBarBtnDown,int nSBarBtnMv);
	void SetScrollRange();
	void SetCaretNowCol(int nCol){m_nCaretNowCol = nCol;}
	void SetScrollPosMovCaretPos();
	bool GetScrollPosToCaretShow();
	bool BaseHandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );

	virtual void Render( float fElapsedTime );
	virtual void EditRender( float fElapsedTime );
	virtual void RenderComposition(float fElapsedTime);
	virtual void UpdateRects();
	virtual bool MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
	virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:

	/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetCaretPosNewLine() - bCaretNowCol: ������ ��ġ�� ��Ŀ������ Caret  | �Լ� nLine: 1 ��ĭ�Ʒ� , -1 ��ĭ ��, n nĭ��ŭ
	GetLineStrEnd() -  [6/19/2008 Psj] ������ ������ ��Ʈ���� ����Ѵ�
	SetCaretPosNewLine() //caret�� ������ġ�� �����Ѵ�. nLine ������ġ bCaretNowCol������ ������ġ ���� bMode ���ιٷ����� �Ǵ�
	                     //���ΰ��� ������ �д�.
	*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void SetCaretPosNewLine(int nLine = 1, bool bCaretNowCol = true ,bool bMode = false);
	void OtherMsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	void SetNowLineDel();
	void SetArryLineCaretX();
	SIZE GetCharSiz(const WCHAR* pChar);
	WCHAR GetLineStrEnd(int nLine); 
	
};