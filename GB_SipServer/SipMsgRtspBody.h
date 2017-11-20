#ifndef _SIP_MSG_RTSP_BODY_H_
#define _SIP_MSG_RTSP_BODY_H_
#include "SipMsgBody.h"

#define RTSP_VISION "RTSP/1.0"

class SipMsgRtspBody :
	public SipMsgBody
{
public:
	SipMsgRtspBody();
	~SipMsgRtspBody();

	virtual int CreateSipMsgBody(void* bodyParameter, std::string &bodyString);
	virtual int ParserSipMsgBody(const std::string bodyString, void* bodyParameter);

private:
	int GetLines(std::string src, std::vector<std::string> &dstLines);// ��rtsp��Ϣ���ȡÿһ������
	int GetRtspInfo(std::vector<std::string> dstLines, RtspControlParam & rtspInfo); // ����ÿһ�в���

	int GetScale(std::string scaleLine, float & scale);		// ��ȡ���ű�����ֵ
	int GetCseq(std::string cseqLine, int & cseq);			// ��ȡcseq
	int GetRange(std::string rangeLine, RtspControlParam & rtspInfo);	// ��ȡ�Ϸſ�ʼʱ��

};

#endif // _SIP_MSG_RTSP_BODY_H_
