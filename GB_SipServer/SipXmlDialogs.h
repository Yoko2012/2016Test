#ifndef _SIP_XML_DIALOG_H_
#define _SIP_XML_DIALOG_H_
#include <iostream>
#include <string>
#include <deque>
#include <mutex>

typedef struct XmlDialogParam
{
	int xmlDialogID;
	int manscdpType; // XML��������
	std::string srcDeviceID; // Դ�豸
	std::string dstDeviceID; // Ŀ���豸

	std::string fromTag;
	std::string toTag;
	std::string callID;

	// ����ʹ��
	int diffTime; // ���ϼ��㱨ʱ����
	time_t lastResponseTime;// �ϴλ㱨ʱ��

	// ��ʷ����Ƶ��ѯʹ��
	std::string startTime;
	std::string endTime;
	std::string recordStartTime;
	std::string recordEndTime;
	std::string recordTimes;

	// ����Ӧ���ơ���ѯ��֪ͨʱʹ��
	time_t birthTime; // dialog�Ĵ���ʱ�䣬�����ж�ע��dialog�Ƿ�����Ч����
	int expires;

	XmlDialogParam()
	{
		xmlDialogID = 0;
		manscdpType = -1;
		expires = -1;
	}
}XmlDialogParam;


class SipXmlDialogs
{
public:
	SipXmlDialogs* GetInstance();
	virtual ~SipXmlDialogs();
	
	void InsertSipXmlDialog(XmlDialogParam* sipXmlDialog);
	void RemoveSipXmlDialog(XmlDialogParam* sipXmlDialog);

	XmlDialogParam* CreateSipXmlDialog();
	int DestroySipXmlDialog(XmlDialogParam* &sipXmlDialog);
	XmlDialogParam* FindSipXmlDialog(int manscdpType, std::string dstDevice);
	
private:
	std::mutex mDialogIdMutex;
	static int mSipXmlDialogID;

	std::mutex mDialogDequeMutex;
	std::deque<XmlDialogParam*> mSipXmlDialogDeque;

private:
	SipXmlDialogs();
	SipXmlDialogs(const SipXmlDialogs&);
	static SipXmlDialogs* mInstance;
};

#endif // _SIP_XML_DIALOG_H_


