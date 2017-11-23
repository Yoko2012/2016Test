#ifndef _SIP_MSG_BODY_H_
#define _SIP_MSG_BODY_H_
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <map>

#include "SipDefine.h"
#include "SipUtilities.h"

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
	//bool isAudio;
	std::string mediaType;
	//bool isRecvOnly;
	std::string onlyType;
	int port;
	std::string rtpType;
	std::string transType;
	std::vector<std::string> payloads;
	std::vector<std::string> attributes;
	M_Param()
	{
		port = 0;
		//isAudio = false;
		mediaType = "video";
		//isRecvOnly = false;
		onlyType = "recvonly";
		rtpType = "RTP/AVP";
		transType = "";
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

typedef struct AlarmInfoParam
{
	std::string alarmMethod;
	std::string alarmType;
}AlarmInfoParam;

typedef struct ResetAlarmParam
{
	std::string resetAlarm;
	std::string alarmMethod;
	std::string alarmType;
	//std::vector<AlarmInfoParam> alarmInfoParam;
	ResetAlarmParam()
	{
		resetAlarm = "ResetAlarm";
	}
}ResetAlarmParam;

typedef struct DragZoomParam
{
	bool isZoomOut;
	int length;
	int width;
	int midPointX;
	int midPointY;
	int lengthX;
	int lengthY;
	DragZoomParam()
	{
		isZoomOut = true;
	}
}DragZoomParam;

typedef struct HomePositionParam
{
	int enabled;
	int resetTime;
	int presetIndex;
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
	ResetAlarmParam   alarmParam;       // ������λ�������չ����
	std::string       iFame;           // ǿ�ƹؼ�֡
	DragZoomParam     dragZoom;        // ����Ŵ���С
	HomePositionParam homePositionParam;	// ��������������
	DeviceConfigParam deviceConfigParam;	// �豸����
}ControlParam;

// Query
typedef struct QueryCatalog
{
	// �����豸��ʼ����ֹʱ�䣨��ѡ��
	std::string startTime;
	std::string endTime;
}QueryCatalog;

typedef struct QueryRecordInfoParam
{
	std::string startTime;	// ¼����ʼʱ��
	std::string endTime;	// ¼����ֹʱ��
	std::string filePath;	// �ļ�·������ѡ
	std::string address;	// ¼���ַ����ѡ
	std::string secrecy;	// ¼�������ԣ���ѡ
	std::string type;		// ¼��������ͣ���ѡ
	std::string recorderID;	// ¼�񴥷���id����ѡ
}QueryRecordInfoParam;

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
	QueryCatalog queryCatalogParam;// �豸Ŀ¼��Ϣ��ѯ
									// ???�豸��Ϣ��ѯ���������
	QueryRecordInfoParam queryRecordInfoParam;// �ļ�Ŀ¼��������
	QueryAlarmParam queryAlarmParam;	// ������ѯ
	std::string configType;			// �豸������Ϣ��ѯ
									// �豸Ԥ��λ��ѯ���������
	int mobileInterval;		// �ƶ��豸λ�����ݲ�ѯ
	QueryCmdParam()
	{
		mobileInterval = 5;
		configType = "BasicParam";
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

// ���¼���ȡĿ¼����ʱ�����ϼ��㱨Ŀ¼ʱʹ��
typedef struct CatalogInformation
{
	std::string tableName;
	std::map<std::string, std::string> valuesMap;
}CatalogItem, CatalogInfo;

typedef struct CatalogItemAndInfo			// ���ϼ��㱨��Դ��ʹ��
{
	CatalogItem resItem;					// �����/�豸��Ϣ
	CatalogInfo cameraInfo;					// �������չ��Ϣ
}CatalogItemAndInfo;
typedef  std::vector<CatalogItemAndInfo> RespCatalogParam;

typedef struct RespDeviceInfoParam
{
	std::string deviceName;	  // Ŀ���豸����
	std::string result;		  // ��ѯ���
	std::string manufacturer; // �豸������
	std::string model;		  // �豸�ͺ�
	std::string firmware;	  // �豸�̼��汾
	std::string channel;	  // ��Ƶ����ͨ����
	std::string info;		  // ��չ��Ϣ
}RespDeviceInfoParam;

typedef struct ResDevStatusInfoAlarmstatus
{
	std::string deviceID;
	std::string dutyStatus; // "ONDUTY" "OFFDUTY" "ALARM"
}Alarmstatus;

typedef struct RespDeviceStatusParam
{
	std::string result;	// ��ѯ���"OK"��"ERROR"
	std::string online;	// "ONLINE" "OFFLINE"
	std::string status;	// �Ƿ���������"OK"��"ERROR"
	std::string reason;	// ԭ��
	std::string encode;	// �Ƿ���� "ON"��"OFF"
	std::string record;	// �Ƿ�¼�� "ON"��"OFF"
	std::string deviceTime; // �豸ʱ�������
	std::vector<Alarmstatus> alarmStatus; // �����豸״̬�б�
	std::string info;       // ��չ��Ϣ
}RespDeviceStatusParam;

typedef QueryRecordInfoParam RespRecordInfoItem;
typedef struct RespRecordInfoParam
{
	std::string name;	// ���������
	std::string sumNum;	// ����ʷ��Ƶ��
	int recordListNum;	// �ô��յ�����ʷ��ƵƬ����
	std::vector<RespRecordInfoItem> recordInfoItems;
}RespRecordInfoParam;

typedef struct RespConfigDownloadParam
{
	std::string result;

	// deviceConfig������������
	std::string name;		        // �豸����
	std::string expiration;			// ע�����ʱ��
	std::string heartBeatInterval;	// �������ʱ��
	std::string heartBeatCount;		// ������ʱ����
	std::string positionCapaility;
	std::string Longitude;
	std::string Latitude;
}RespConfigDownloadParam;

typedef struct RespPresetParam
{
	std::string presetID;
	std::string presetName;
}RespPresetParam;
typedef std::vector<RespPresetParam> RespPresetParams;

typedef struct ResponseCmdParam
{
	std::string result; // �豸��������Ӧ��"OK"��"ERROR"
						// ����֪ͨӦ��"OK"��"ERROR"
						// Ŀ¼��Ϣ��ѯ�յ�Ӧ��"OK"��"ERROR"
						// �豸����Ӧ��"OK"��"ERROR"
						// �����㲥Ӧ��"OK"��"ERROR"

	RespCatalogParam        catalogParam;		 // �豸Ŀ¼��ѯӦ��
	RespDeviceInfoParam     devInfoParam;		 // �豸��Ϣ��ѯӦ��
	RespDeviceStatusParam   devStatusParam;		 // �豸״̬��Ϣ��ѯӦ��
	RespRecordInfoParam     recordInfoParam;	 // �ļ�Ŀ¼����Ӧ��
	RespConfigDownloadParam configDownloadParam; // �豸���ò�ѯӦ��
	RespPresetParams        presetList;          // �豸Ԥ��λ��ѯӦ��

	ResponseCmdParam()
	{
		result = "OK";
	}
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
