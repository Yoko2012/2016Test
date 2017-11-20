#ifndef _SIP_MSG_BODY_H_
#define _SIP_MSG_BODY_H_
#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#include "SipDefine.h"

////////////////////////////////RTSP//////////////////////////////////////////
enum RTSP_TYPE
{
	PLAY,		// ����
	PAUSE,		// ��ͣ
	SCALE,		// ���������
	RANGE,		// ����Ϸ�
	TEARDOWN	// ֹͣ
};

typedef struct RtspControlParam
{
	RTSP_TYPE rtspType; // ��������
	std::string method; // "PLAY", "PAUSE"
	int cseq;
	float scale;        // ���ٲ���
	long  startTime;    // �Ϸſ�ʼʱ��
	long  endTime;      // �ϷŽ���ʱ��

	std::string bodyString;

	RtspControlParam()
	{
		rtspType = RTSP_TYPE::PLAY;
		method = "PAUSE";
		cseq = 0;
		scale = 0;
		startTime = 0;
		endTime = 0;
	}
}RtspParam;

////////////////////////////////SDP//////////////////////////////////////////
typedef struct O_Param
{
	std::string userName;
	std::string sessID;
	std::string sessVersion;
	std::string netType;
	std::string addrType;
	std::string addr;
	O_Param()
	{
		userName = "";
		sessID = "0";
		sessVersion = "0";
		netType = "IN";
		addrType = "IP4";
		addr = "";
	}
}O_Param;

typedef struct U_Param
{
	std::string deviceID;
	int num;
	U_Param()
	{
		deviceID = "";
		num = 3;
	}
}U_Param;

typedef struct C_Param
{
	std::string netType;
	std::string addrType;
	std::string addr;
	C_Param()
	{
		netType = "IN";
		addrType = "IP4";
		addr = "";
	}
}C_Param;

typedef struct T_Param
{
	std::string startTime;
	std::string endTime;
	T_Param()
	{
		startTime = "0";
		endTime = "0";
	}
}T_Param;

typedef struct M_Param
{
	bool isAudio;
	bool isRecvOnly;
	int port;
	std::string transType;
	std::vector<std::string> payloads;
	std::vector<std::string> mAttributes;
	M_Param()
	{
		port = 0;
		isAudio = false;
		isRecvOnly = false;
		transType = "UDP";
	}
}M_Param;

typedef struct SdpMediaParam
{
	std::string version;
	O_Param oParam;
	std::string sName; // ��Play������Playback������Download��
	U_Param uParam;
	C_Param cParam;
	T_Param tParam;
	M_Param mParam;
	std::string y;
	std::string f;

	std::string bodyString;
	SdpMediaParam()
	{
		version = "0";
		sName = "Play";
		y = "";
		f = "v/////a///";
	}
}SdpParam;

////////////////////////////////XML//////////////////////////////////////////
// Control
typedef struct PtzControlParam
{
	std::string ptzCmdValue;
};

typedef struct ResetAlarmParam
{
	std::string alarmMethod;
	std::string alarmType;
}ResetAlarmParam;

typedef struct DragZoonParam
{
	bool isOut;
	int length;
	int width;
	int midPointX;
	int midPointY;
	int lengthX;
	int lengthY;
	DragZoonParam()
	{
		isOut = true;
	}
};

typedef struct HomePositionParam
{
	std::string enabled;
	std::string resetTime;
	std::string presetIndex;
}HomePositionParam;

typedef struct DeviceConfigParam
{
	// deviceConfig������������
	std::string name;		// �豸����
	std::string expiration;			// ע�����ʱ��
	std::string heartBeatInterval;	// �������ʱ��
	std::string heartBeatCount;		// ������ʱ����

	// svac ��������
	// svac ��������
}DeviceConfigParam;

typedef struct ControlCmdParam
{
	PtzControlParam   ptzParam;        // Ptz��������
	std::string       teleBootParam;   // Զ����������
	std::string       recordType;      // ¼��������ͣ�Record/StopRecord
	std::string       guardType;       // �����������������ͣ�SetGuard/ResetGuard
	ResetAlarmParam   alarmPara;       // ������λ�������չ����
	std::string       iFame;           // ǿ�ƹؼ�֡
	DragZoonParam     dragZoon;        // ����Ŵ���С
	HomePositionParam homePositionPara;	// ��������������
	DeviceConfigParam deviceConfigPara;	// �豸����
}ControlParam;

// Query
typedef struct QueryCatalog
{
	// �����豸��ʼ����ֹʱ�䣨��ѡ��
	std::string startTime;
	std::string endTime;
}QueryCatalog;

typedef struct QueryRecordInfo
{
	std::string startTime;	// ¼����ʼʱ��
	std::string endTime;	// ¼����ֹʱ��
	std::string filePath;	// �ļ�·������ѡ
	std::string address;	// ¼���ַ����ѡ
	std::string secrecy;	// ¼�������ԣ���ѡ
	std::string type;		// ¼��������ͣ���ѡ
	std::string recorderID;	// ¼�񴥷���id����ѡ
}QueryRecordInfo;

typedef struct QueryAlarmParam
{
	std::string startAlarmPriority; // ������ʼ����
	std::string endAlarmPriority;	// ������������
	std::string alarmMethod;		// ������ʽ����
	std::string alarmType;			// ��������
	std::string startAlarmTime;		// ������ʼʱ��
	std::string endAlarmTime;		// ��������ʱ��
}QueryAlarmParam;

typedef struct QueryCmdParam
{
	// ???�豸״̬��ѯ���������	
	QueryCatalog queryDeviceCatalog;// �豸Ŀ¼��Ϣ��ѯ
									// ???�豸��Ϣ��ѯ���������
	QueryRecordInfo queryRecordInfo;// �ļ�Ŀ¼��������
	QueryAlarmParam queryAlarmPara;	// ������ѯ
	std::string configType;			// �豸������Ϣ��ѯ
									// �豸Ԥ��λ��ѯ���������
	int mobileInterval;		// �ƶ��豸λ�����ݲ�ѯ
	QueryCmdParam()
	{
		mobileInterval = 5;
	}
}QueryParam;

// Notify
typedef struct NotifyKeepaliveDevList // ������ѡ����
{
	std::vector<std::string> info; // �����豸�б�
}NotifyKeepaliveDevList;

typedef struct NotifyAlarmInfo
{
	std::string alarmType;
	std::string eventType;
}NotifyAlarmInfo;

typedef struct NotifyAlarmParam // ����֪ͨ
{
	std::string alarmPriority;	// �������𣬱�ѡ
	std::string alarmMethod;	// ������ʽ����ѡ
	std::string alarmTime;		// ����ʱ�䣬��ѡ
	std::string alarmType;
	std::string alarmTypeParam;
	std::string deviceID;
	std::string alarmDescription;// ����������������ѡ
	std::string longitude;	// ���ȣ���ѡ
	std::string latitude;	// γ�ȣ���ѡ
	std::vector<NotifyAlarmInfo> alarmInfo; // ��չ��Я���������ͣ���������
}NotifyAlarmParam;

typedef struct NotifyBroadcastParam
{
	std::string sourceID;
	std::string targetID;
}NotifyBroadcastParam;

struct NotifyCatalogInfo
{
	std::string deviceID;
	std::string event;
};

typedef struct NotifyMobilePositionParam
{
	std::string time;		// ����֪ͨʱ�䣬��ѡ
	std::string longtitude;	// ���ȣ���ѡ
	std::string latitude;	// γ�ȣ���ѡ
	std::string speed;		// �ٶȣ���ѡ
	std::string direction;	// ���򣬿�ѡ
	std::string altitude;	// ���θ߶ȣ���ѡ
}NotifyMobilePositionParam;

typedef struct NotifyCmdParam
{
	NotifyKeepaliveDevList keepalive;	// ״̬��Ϣ���ͣ�������Ϣ
	NotifyAlarmParam alarm;			// ����֪ͨ
	std::string mediaStatus;// "121",��ʾ��ʷý���ļ����ͽ���
	NotifyBroadcastParam broadcast;	// �����㲥֪ͨ
	NotifyMobilePositionParam mobilePosition; // �ƶ��豸λ������֪ͨ
	std::vector<NotifyCatalogInfo> deviceList; // Ŀ¼���֪ͨ
	NotifyCmdParam()
	{
		mediaStatus = "121";
	}
}NotifyParam;

// Response
typedef struct ResponseCmdParam
{

}ResponseParam;


typedef struct XmlCmdParam
{
	MESSAGE_MANSCDP_TYPE manscdpType;
	std::string reqRespType; // Control, Query, Notify, Response
	std::string cmdType;
	int sn;
	std::string deviceID;

	ControlParam  controlParam;
	QueryParam    queryParam;
	NotifyParam   notifyParam;
	ResponseParam responseParam;

	std::string localSipID;
	std::string remoteSipID;

	std::string bodyString;
	XmlCmdParam()
	{

	}
}XmlParam;
//////////////////////////////////////////////////////////////////////////
class SipMsgBody
{
public:
	SipMsgBody();
	virtual ~SipMsgBody();

	virtual int CreateSipMsgBody(void* bodyParameter, std::string &bodyString) = 0;
	virtual int ParserSipMsgBody(const std::string bodyString, void* bodyParameter) = 0;
};

#endif // _SIP_MSG_BODY_H_
