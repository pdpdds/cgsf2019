// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

//additional header
#include <windows.h>
#include "CommonHeader.h"
#include "SFEngine.h"

#include "../../Common/SFPacketStore/ProtocolPacket.pb.h"
#include "../../Common/SFPacketStore/ProtocolPacketID.h"

#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"