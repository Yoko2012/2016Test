#ifndef _SIP_SERVER_H_
#define _SIP_SERVER_H_
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>

#include "SipLog.h"
#include "SipMsgObj.h"
#include "SipCallback.h"
#include "SipDialogs.h"
#include "SipSocketServer.h"
#include "AppContext.h"

class SipServer
{
public:
	static SipServer* GetInstance();// ��ȡSipServerΨһ�ӿ�
	virtual ~SipServer();

	int StartSipServer();			// ����sip�������ں���

	osip_t* sip; // ȫ��Ψһ
 	SipLog* log;
	SipDialogs* mSipDialogs;				// ȫ��Ψһdialog����
 	SipSocketServer* mSocketServer;			// SocketServerΨһʵ��
 	SipDatabase* mSipDB;
 	std::vector<osip_event_t*> mSipEvents;	// ��Ҫ�����eventȫ��Ψһ

private:
	SipServer();				// ����Ϊprivate��ֹʵ��������
	SipServer(const SipServer&);// ����Ϊprivate��ֹ��������ʵ��������
	static SipServer* mInstance;// Ψһʵ��

	int InitSipServer();// ��ʼ��sip����
	int AutoRegister();	// ����ʱ�Զ����ϼ�ע��

	// �շ�sip��Ϣ������
	int ProcessInComingMessage(std::string message);
	int ProcessOutGoingMessage(osip_transaction_t* callTransaction, osip_event_t* &event);
	int CreateTransaction(osip_transaction_t* &transaction, osip_event_t* event);
	
	// ע��ص�����
	void RegisterSipMsgCallbacks();
	void RegisterCommonCallbacks();

	// �����̣߳�������·����
	int CheckPlatformStatus();
	void CheckUpperStatus(int timeout);
	void CheckLowerStatus(int timeout);
};

#endif // _SIP_SERVER_H_
