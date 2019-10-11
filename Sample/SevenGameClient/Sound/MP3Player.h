#pragma once
#include <dshow.h>

#define WM_GRAPHNOTIFY  WM_USER+13 //���ͱ׷��� �޼��� �߻��� HandleGraphEvent ȣ��!!

class CMP3Player
{
public:
	CMP3Player(void);
	virtual ~CMP3Player(void);

	BOOL Initialize(HWND hWnd);
	BOOL Finally();

	BOOL OnPlayAudio(TCHAR* szName, BOOL bLooped = FALSE );

//������ �̺�Ʈ�� �޾ƾ� �ǹǷ� �ֿܼ����� ó���� ���� ����.
//���̾�α׸� �����ؼ� �޼����� ���� ��Ȳ�� �Ǹ� �ڼ��� �����ϵ��� �Ѵ�.
	BOOL HandleGraphEvent();

	static CMP3Player* GetInstance()
	{
		if (NULL == m_pMP3Manager)
		{
			m_pMP3Manager = new CMP3Player();
		}
		return m_pMP3Manager;
	}

protected:

private:
	HWND m_hWnd;

	// DirectShow Graph, Filter & Pins used
	IGraphBuilder *m_pGraphBuilder;
	IMediaControl *m_pMediaControl;
	IMediaSeeking *m_pMediaSeeking;
	IBaseFilter   *m_pSourceCurrent;
	IBaseFilter   *m_pSourceNext;
	TCHAR          m_szCurrentFile[128];
	IMediaEventEx *m_pME;

	static CMP3Player* m_pMP3Manager;
};
