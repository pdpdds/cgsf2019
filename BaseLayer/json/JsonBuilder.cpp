#include "StdAfx.h"
#include "JsonBuilder.h"
#include "JsonNode.h"

//const char* Seperator = "\r\n";
//const int SeperatorLen = strlen(Seperator);

JsonBuilder::JsonBuilder() 
	: m_bufferPtr(NULL)
	, m_writtenOffset(0)
	, m_readOffset(0)
{
	
}

void JsonBuilder::PrepareBuffer(int ioSize)
{
	m_bufferPtr = new char[ioSize];
	memset(m_bufferPtr, 0, ioSize);
	m_ioSize = ioSize;
}

JsonBuilder::~JsonBuilder()
{
	if (NULL != m_bufferPtr)
	{
		delete [] m_bufferPtr;
		m_bufferPtr = NULL;
	}
}

bool JsonBuilder::PushBuffer( const char* buffer, size_t bufferLen )
{
	return AddBuffer(buffer, (UINT)bufferLen);
}

bool JsonBuilder::PopCompleteNode( JsonObjectNode& node, unsigned short dataSize )
{
	//std::string totalContent(&m_bufferPtr[m_readOffset], m_writtenOffset);
	//size_t pos = totalContent.find(Seperator);
	//if (std::string::npos == pos)
		//return false;

	unsigned int usedSize = node.Parse(&m_bufferPtr[m_readOffset], dataSize);
	if (0 == usedSize)
	{
		m_readOffset += dataSize;
		return false;
	}

	/*if (usedSize != dataSize)
	{
		SFASSERT(0);
		return false;
	}*/
	
	m_readOffset += dataSize;
	if (m_readOffset == m_writtenOffset)
	{
		m_readOffset = 0;
		m_writtenOffset = 0;
	}
	return true;
}

bool JsonBuilder::AddBuffer( const char* buffer, unsigned int bufferLen )
{
	if ((INT)(m_writtenOffset + bufferLen) > m_ioSize)
		return false;

	memcpy(&m_bufferPtr[m_writtenOffset], buffer, bufferLen);
	m_writtenOffset += bufferLen;
	return true;
}

unsigned int JsonBuilder::MakeBuffer( const JsonObjectNode& node, char* buffer, unsigned int bufferLen )
{
	tstring contentStr = node.ToString();
	if (bufferLen < contentStr.length())
		return 0;
	memcpy(buffer, contentStr.c_str(), contentStr.length());
	return (unsigned int)(contentStr.length());
	//memcpy(&buffer[contentStr.length()], Seperator, SeperatorLen);
	//return (unsigned int)(contentStr.length()+SeperatorLen);
}