#ifndef _SIP_DIALOGS_H_
#define _SIP_DIALOGS_H_
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <mutex>

#include <osip2/osip_dialog.h>

#define SipDialogList std::list<osip_dialog_t*>
#define IntList std::list<int>

typedef enum DialogType
{
	REGISTER_DIALOG       = 0, // ע��
	PLAY_INVITE_DIALOG    = 1, // ʵʱ��Ƶ
	PLAYBACK_DIALOG       = 2, // ��ʷ�ط�
	DOWNLOAD_DIALOG       = 3, // ��ʷ����
	THIRD_PARTY_DIALOG    = 4, // ����������
	AUDIOBROADCAST_DIALOG = 5, // �����㲥
	SUBSCRIBE_DIALOG      = 6  // �¼�����
}DialogType;

typedef enum SocketType
{
	UDP = 0,
	TCP = 1
}SocketType;

typedef enum DialogHint
{
	UAC = 0,
	UAS = 1
}DialogHint;

typedef enum StreamDirection
{
	SENDER = 0,
	RECEIVER = 1
}StreamDirection;

typedef enum StreamType
{
	AVSS = 0,
	GB28181 = 1
}StreamType;

typedef struct DialogInfo
{
	int dialogID;
	int sn;
	int httpsHandleID;

	DialogHint hint;		// UAC,UAS
	DialogType dialogType;	// dialog���ͣ�ע�ᡢʵʱ����ʷ�����ء��������������㲥������

	time_t lastResponseTime;// ����յ�������Ӧʱ�䣬�����ж��Ƿ�ʱ
	time_t birthTime;		// dialog�Ĵ���ʱ�䣬�����ж�ע��dialog�Ƿ�����Ч����

	std::string remoteSipID;
	std::string localSipID;
	std::string dstDeviceID; // Ŀ���豸��ƽ̨DeviceID

	int callerDialogID;	    // �ϼ�dialogID
	int calleeDialogID;	    // �¼�dialogID

	std::string recvIP;	    // ����������ip
	int recvPort;		    // ����������port
	std::string sendIP;	    // ����������ip
	int sendPort;		    // ����������port

	SocketType socketType;	// TCP��UDP
	StreamType streamType;  // ����28181�Խӡ�����˽��Э��Խ�
	StreamDirection streamDirection; // �ڸ�dialog�б��������������ǽ�������
	IntList inviteDialogIDs;         // ƽ̨������ƵdialogID,ֻ��registerDialogʹ�ø��ֶ�

	DialogInfo()
	{
		sn = 0;
		dialogID = 0;
		httpsHandleID = 0;
		callerDialogID = 0;
		calleeDialogID = 0;

		recvIP = "";
		recvPort = 0;
		sendIP = "";
		sendPort = 0;
	}
}DialogInfo;

class SipDialogs
{
public:
	virtual ~SipDialogs();
	static SipDialogs* GetDialogsInstance();

	// ����������dialog
	osip_dialog_t* CreateSipDialog(osip_transaction_t* trn, osip_message_t* respSipMsg, DialogType dialogType);
	void DestorySipDialog(osip_dialog_t* &dialog);

	// ���롢�Ƴ�dialog
	void InsertSipDialog(osip_dialog_t* dialog, DialogType dialogType);
	int RemoveSipDialogByDialogID(int dialogID, DialogType dialogType);
	int RemoveDialogIdFromRegisterDialog(int dialogID, osip_dialog_t* &registerDialog);

	// ����dialog
	osip_dialog_t* FindRegisterDialog(std::string remoteDeviceID, DialogHint hint); // ͨ���Է�DeviceID����ע��Dialog
	osip_dialog_t* FindSipDialogByDialogID(int dialogID, DialogType dialogType);	// ͨ��DialogID��dialogType����dialog
	osip_dialog_t* FindSipDialogByDialogID(int dialogID);	// ͨ��DialogID����dialog
	osip_dialog_t* FindInviteDialog(std::string fromTag, std::string callId, DialogHint hint); // ͨ��fromTag��callID����InviteDialog
	osip_dialog_t* FindInviteDialog(int httpsHandle); // ͨ��httpsHandle����InviteDialog
	osip_dialog_t* FindAudioDialog(std::string audioDeviceID);

private:
	SipDialogs();					// ����Ϊprivate��ֹʵ��������
	SipDialogs(const SipDialogs&);	// ����Ϊprivate��ֹ��������ʵ��������
	static SipDialogs* mInstance;	// Ψһʵ��

	// dialogID��dialogID��
	static int mDialogID;
	std::mutex mDialogIdMutex;

	// register Dialog��ر���
	std::mutex mRegisterMutex;
	SipDialogList mRegisterDialogs;

	std::mutex mInviteMutex;
	SipDialogList mInviteDialogs;

	std::mutex mSubcribeMutex;
	SipDialogList mSubcribeDialogs;

	// �ڲ�ʹ��
	osip_dialog_t* FindDialogByDialogID(SipDialogList dialogList, int dialogID); // ����dialog	
	int RemoveDialogByDialogID(SipDialogList &dialogList, int dialogID);// ��dialog��dialogs�б���ɾ��
};
#endif // _SIP_DIALOGS_H_

