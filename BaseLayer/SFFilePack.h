#pragma once
#include <LiteZip/LiteZip.h>
#include <LiteZip/LiteUnzip.h>

class SFFilePack
{

public:
	SFFilePack(void);
	virtual ~SFFilePack(void);

	BOOL Initialize();
	BOOL Finally();

	void show_errmsg(void);

	bool UnPackMem( TCHAR* pZipFileName, TCHAR* pFileName );
	bool PackMemory(TCHAR* pZipFileName, TCHAR* pFileName, unsigned char* pBuffer, int BufferSize);

	bool PackMemoryRaw(TCHAR* pZipFileName, unsigned char* pBuffer, int UnCompressedSize, int& CompressedSize);
	unsigned char* UnPackMemoryRaw( TCHAR* pZipFileName, unsigned char* pBuffer, int BufferSize );


	////////////////////////////////////////////////////////////////////////////////////////////
	//PackDir : ������ ���丮 ���� �������� �����Ѵ�. �����ڵ� �Լ��� ����� �������� �ʴ� �� ��
	////////////////////////////////////////////////////////////////////////////////////////////
	bool PackFile( TCHAR* pZipFileName, TCHAR* pTargetFileName, bool bNewZipFile = true);

	////////////////////////////////////////////////////////////////////////////////////////////
	//PackDir : ������ ���丮 ���� �������� �����Ѵ�. �����ڵ� �Լ��� ����� �������� �ʴ� �� ��
	////////////////////////////////////////////////////////////////////////////////////////////
	bool PackDir(TCHAR* pFileName, TCHAR* pDirFullPath, int DirFullPathLen );

	////////////////////////////////////////////////////////////////////////////////////////////
	//UnPackRes : ���������� ���ҽ��� �ִ� zip���Ͽ��� �����͸� ��� �´�. �̸��� �����ϸ� �ϳ��� ���ϸ� ����� �� �ִ�.
	////////////////////////////////////////////////////////////////////////////////////////////
	bool UnPackRes(TCHAR* pExtractedFileName = NULL);
	unsigned char* UnPackResMem( TCHAR* pExtractedFileName, int& BufferSize);
	bool UnPackResMemAll( TCHAR* pResourceName);

	////////////////////////////////////////////////////////////////////////////////////////////
	//UnPackFile : Zip ������ ������ Ǯ� ���Ϸ� �����Ѵ�.
	//pExtractedFileName �̸��� �����ϸ� �������Ͽ��� �ϳ��� ���ϸ� ������ ���� ������ �� �ִ�.
	////////////////////////////////////////////////////////////////////////////////////////////
	bool UnPackFile(TCHAR* pZipFileName, TCHAR* pExtractedFileName = NULL);

protected:

private:
	// Where we store the pointers to LiteZip.dll's functions that we call
	ZipCreateFilePtr		*lpZipCreateFile;
	ZipAddDirPtr			*lpZipAddDir;
	ZipClosePtr				*lpZipClose;
	ZipFormatMessagePtr		*lpZipFormatMessage;
	ZipAddFilePtr			*lpZipAddFile;
	ZipAddBufferPtr			*lpZipAddBuffer;
	ZipCreateBufferPtr		*lpZipCreateBuffer;
	ZipAddBufferRawPtr		*lpZipAddBufferRaw;
	ZipGetMemoryPtr			*lpZipGetMemory;

	UnzipOpenFilePtr		*lpUnzipOpenFile;
	UnzipGetItemPtr			*lpUnzipGetItem;
	UnzipItemToBufferPtr	*lpUnzipItemToBuffer;
	UnzipClosePtr			*lpUnzipClose;
	UnzipFormatMessagePtr	*lpUnzipFormatMessage;		
	UnzipOpenBufferPtr		*lpUnzipOpenBuffer;				
	UnzipItemToFilePtr		*lpUnzipItemToFile;		
	UnzipFindItemPtr		*lpUnzipFindItem;		
	UnzipOpenFileRawPtr		*lpUnzipOpenFileRaw;


	HMODULE		m_zipDll;
	HMODULE		m_unzipDll;
};