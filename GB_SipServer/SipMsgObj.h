#ifndef _SIP_MSG_OBJ_H_
#define _SIP_MSG_OBJ_H_
#include "SipMsgHeader.h"
#include "SipMsgRtspBody.h"
#include "SipMsgSdpBody.h"
#include "SipMsgXmlBody.h"

#include "SipDatabase.h"
#include "SipUtilities.h"

typedef class SipMsgObj
{
public:
	SipMsgObj();
	~SipMsgObj();

	// ��ȡϵͳ��Ϣ
	int GetLocalPlatformInfo();
	int GetRemotePlatformInfo(std::string remoteDeviceID);
	
	// Register
	int CreateRegisterMsg(osip_message_t* &dstSipMsg, const std::string remoteSipID, const std::string expires = "3600"); // ������Ȩע�ắ��
	int CreateRegMsgAgain(osip_message_t* &dstSipMsg,
		                  const std::string remoteSipID,
		                  const std::string callID,
		                  const std::string fromTag,
		                  const std::string toTag,
		                  const std::string cesq); // ��Ч�ڵ�������ע�ắ��
	int CreateUnRegistrtMsg(osip_message_t* &dstSipMsg, const std::string remoteSipID, const std::string expires = "0");	// ע��
	int CreateAuthRegisterMsg(const osip_message_t* orgRequest, const osip_message_t* lastResponse, osip_message_t* &dstSipMsg);
	int CreateUnauthrizedMsg(const osip_message_t* srcMessage, osip_message_t* &dstMessage, const std::string toTag = "");// 401 Unauthrized

	// Message��Subscribe��NotifyMessage, xml��Ϣ�壬
	int CreateSipMsgXml(osip_message_t* &dstSipMsg, std::string remoteDeviceID);
	int SetSipMsgProtol(osip_message_t* dstSipMsg, std::string protol = "TCP");

	// Invite, sdp��Ϣ��
	int CreateInviteSipMsg();
	int CreateThirdInviteSipMsg();
	int CreateAckSipMsg();
	int CreateThirdAckSipMsg();
	int CreateByeSipMsg();

	// rtsp��Ϣ��
	int CreateSipMsgRtsp();

	// 1xx

	// 2xx
	int Create200OKSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 200
	int CreateInvite200OkSipMsg();
	int CreateRegister200OKSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 200

	// 3xx

	// 4xx
	int CreateBadRequestSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 400
	int CreateForbiddenSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 403
	int CreateNotFoundSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg); // 404

	// ����SipMsg�ĳ���ͷ�ֶ�
	int GetSipMsgTagAndCallID(const osip_message_t* sipMsg, std::string &fromTag, std::string &toTag, std::string &callID);
	int GetSipMsgExpires(const osip_message_t* sipMsg, std::string & expires);

	// ����SipMsg��Ϣ��
	int ParserSipMsgBody(const osip_message_t* sipMsg);
	XmlParam  mXmlParam;
	SdpParam  mSdpParam;
	RtspParam mRtspParam;

public:
	// ƽ̨������Ϣ
	int mLocalPlatformID;
	std::string mLocalSipID;		// ����ƽ̨28181�������
	std::string mLocalSipDomain;	// ����ƽ̨���������
	std::string mLocalIP;			// ����ƽ̨sip����IP��ַ
	std::string mLocalPort;			// ����ƽ̨sip����˿ں�
	std::string mLocalDeviceID;		// ����ƽ̨/�豸DeviceID

	int mRemotePlatformID;
	std::string mRemoteSipID;		// ����ƽ̨28181�������
	std::string mRemoteSipDomain;	// ����ƽ̨���������
	std::string mRemoteIP;			// ����ƽ̨sip����IP��ַ
	std::string mRemotePort;		// ����ƽ̨sip����˿ں�
	std::string mRemoteDeviceID;	// ����ƽ̨/�豸DeviceID������Ϊƽ̨SipID��

private:
	// ���ݿ�����
	static SipDatabase* mSipDB;

	SipMsgHeader* mSipMsgHeader;
	SipMsgBody* mSipMsgBody;// ��Ϣ��

	// ������ϢStart-Line
	int SetSipMsgRequestStartLine(osip_message_t* &dstMessage, std::string method);
	int SetSipMsgResponseStartLine(osip_message_t* &dstMessage, int statusCode, std::string reasonPhrase);

	int CreateBaseResponseSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);

	// CreateNewSipMsg
	int CreateNewSipMsg(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg, osip_fsm_type_t msgType);
	int CreateNewSipMsg_ICT(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	int CreateNewSipMsg_IST(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	int CreateNewSipMsg_NICT(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);
	int CreateNewSipMsg_NIST(const osip_message_t* srcSipMsg, osip_message_t* &dstSipMsg);

}SipMsgBuilder, SipMsgParser;
#endif // _SIP_MSG_OBJ_H_
