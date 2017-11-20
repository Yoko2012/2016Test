#include "SipCallback.h"

int SipCallback::osip_send_message_cb(osip_transaction_t * trn, osip_message_t * sip_msg, char * dest_socket_str, int dest_prot, int send_sock)
{
	char* message = nullptr;
	osip_message_to_str(sip_msg, &message, 0);
	std::string ss = message;
	if (message)
	{
		if (strlen(message) < 100)
		{
			std::cout << "�����sip��Ϣ���޷����ͣ�����" << std::endl;
			return -1;
		}

		// �ж���UDP���ͻ���TCP����
		int type = 0;
		if (!strcmp(trn->topvia->protocol, "TCP")) // TCP����
		{
			type = 1;
		}

		SipServer* sipServer = SipServer::GetInstance();
		//sipServer->mSocketServer->SendSipMsg(message, dest_socket_str, dest_port, type);

		// �ͷ�message�ڴ�
		osip_free(message);
	}
}

void SipCallback::osip_nict_rcv_2xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
	if (trn == nullptr || trn->your_instance == nullptr || sipMsg == nullptr)
	{
		return;
	}
	
	SipServer* sipServer = SipServer::GetInstance();
	SipDialogs* sipDialogs = sipServer->mSipDialogs;
	SipDatabase* sipDB = sipServer->mSipDB;
	AppContext* appContext = (AppContext*)trn->your_instance;

	SipMsgParser sipMsgParser;
	const char* sipMsgMethod = trn->orig_request->sip_method;
	if (SipUtilities::CompareStrCase(sipMsgMethod, "REGISTER")) // ע�ᡢע��
	{
		std::string expire = "";
		sipMsgParser.GetSipMsgExpires(trn->orig_request, expire);
		if (expire == "0") // ע��
		{
			osip_dialog_t* registerDialog = nullptr;
			registerDialog = sipDialogs->FindRegisterDialog(trn->orig_request->req_uri->username, DialogHint::UAC);

			if (registerDialog != nullptr)
			{
				DialogInfo* registerDialogInfo = (DialogInfo*)registerDialog->your_instance;

				// ֹͣ�ϼ�������������Ƶ
				osip_dialog_t* upperInviteDialog = nullptr;
				DialogInfo* upperInviteDialogInfo = nullptr;
				for each (int upperInviteDialogID in registerDialogInfo->inviteDialogIDs)
				{
					upperInviteDialog = sipServer->mSipDialogs->FindSipDialogByDialogID(upperInviteDialogID, DialogType::PLAY_INVITE_DIALOG);
					if (upperInviteDialog)
					{
						upperInviteDialogInfo = (DialogInfo*)upperInviteDialog->your_instance;

						// ���ϼ�����bye


						// ���¼�����bye

					}
				}

				// ɾ���Ѵ���ע��dialog
				sipDialogs->RemoveSipDialogByDialogID(registerDialogInfo->dialogID, REGISTER_DIALOG);
				sipDialogs->DestorySipDialog(registerDialog);
			}

			// ���ϼ�״̬����Ϊ��OFF��
			sipDB->SetDeviceStatus(trn->orig_request->req_uri->username, "OFF");
			std::cout << "ע���ɹ�..." << std::endl;
		} 
		else // ע��
		{
			osip_dialog_t* registerDialog = nullptr;
			registerDialog = sipDialogs->FindRegisterDialog(trn->orig_request->req_uri->username, DialogHint::UAC);

			if (registerDialog == nullptr) // �״�ע�ᡢ�쳣�˳�����ע��
			{				
				registerDialog = sipDialogs->CreateSipDialog(trn, sipMsg, REGISTER_DIALOG);// ����dialog
				sipDialogs->InsertSipDialog(registerDialog, REGISTER_DIALOG);
			} 
			else // ��Ч�ڽ���������ע����Ҫ����birthTime��cesqNum
			{
				DialogInfo* dialogInfo = (DialogInfo*)registerDialog->your_instance;
				dialogInfo->lastResponseTime = time(NULL);
				dialogInfo->birthTime = time(NULL);

				//registerDialog->local_cseq = std::stoi(trn->orig_request->cseq->number);
				++registerDialog->local_cseq;
			}

			// ���ϼ�״̬����Ϊ��ON��
			sipDB->SetDeviceStatus(trn->orig_request->req_uri->username, "ON");
			std::cout << "ע��ɹ�..." << std::endl;
		}
	}
	else if (SipUtilities::CompareStrCase(sipMsgMethod, "BYE"))
	{
		
	}
	else if (SipUtilities::CompareStrCase(sipMsgMethod, "MESSAGE"))
	{
	}
	else if (SipUtilities::CompareStrCase(sipMsgMethod, "SUBSCRIBE")) // ����
	{
	}
	else if (SipUtilities::CompareStrCase(sipMsgMethod, "INFO")) // ��ʷ����
	{
	}
}

void SipCallback::osip_nict_rcv_3xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_rcv_4xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
	if (trn == nullptr || trn->your_instance == nullptr || sipMsg == nullptr)
	{
		return;
	}

	SipServer* sipServer = SipServer::GetInstance();
	SipDialogs* sipDialogs = sipServer->mSipDialogs;
	SipDatabase* sipDB = sipServer->mSipDB;
	AppContext* appContext = (AppContext*)trn->your_instance;

	SipMsgParser sipMsgParser;
	const char* sipMsgMethod = trn->orig_request->sip_method;
	if (SipUtilities::CompareStrCase(sipMsgMethod, "REGISTER")) // ע�ᡢע��
	{
		SipMsgBuilder sipMsgBuilder;
		osip_message_t* resSipMsg = nullptr;
		if (sipMsg->status_code == 401)
		{
			// ��������Ȩ��SipMsg
			sipMsgBuilder.CreateAuthRegisterMsg(trn->orig_request, trn->last_response, resSipMsg);

			// ������Ϣ
			osip_event_t* event = osip_new_outgoing_sipmessage(resSipMsg);
			sipServer->mSipEvents.push_back(event);
		}
		else
		{
			std::string expires = "";
			sipMsgParser.GetSipMsgExpires(trn->orig_request, expires);
			if (expires == "0")
			{
				std::cout << "ע��ʧ�ܣ�����" << std::endl;
			}
			else
			{
				std::cout << "ע��ʧ�ܣ�����" << std::endl;
			}
		}
	}
	else
	{

	}
}

void SipCallback::osip_nict_send_register_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_send_notify_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nict_send_subscribe_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_rcv_2xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_rcv_3xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_rcv_45xx_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_send_invite_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ist_rcv_invite_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ist_rcv_ack_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_register_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
	if (trn == nullptr || trn->your_instance == nullptr || sipMsg == nullptr)
	{
		return;
	}

	SipServer* sipServer = SipServer::GetInstance();
	SipDialogs* sipDialogs = sipServer->mSipDialogs;
	SipDatabase* sipDB = sipServer->mSipDB;
	AppContext* appContext = (AppContext*)trn->your_instance;

	// �ж���ƽ̨�����豸
	bool isPlatform = true;
	if (isPlatform)
	{
		// �Ǳ������¼�ƽ̨����ƽֱ̨�ӻظ�403
		int connectState = 0;
		sipDB->GetIntData("", connectState);
		if (connectState != 2 && connectState != 3)
		{
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* respSipMsg = nullptr;
			sipMsgBuilder.CreateBadRequestSipMsg(sipMsg, respSipMsg);
			osip_event_t* event = osip_new_outgoing_sipmessage(respSipMsg);	// ������Ϣ������Ϣ�¼�
			if (event)
			{
				event->transactionid = trn->transactionid; // �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(event);
			}
			return;
		}
	} 
	else // ������Ҫ����28181�豸����
	{
		// ...
		if (true) // �Ǳ���ƽ̨�����豸
		{
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* respSipMsg = nullptr;
			sipMsgBuilder.CreateBadRequestSipMsg(sipMsg, respSipMsg);
			osip_event_t* event = osip_new_outgoing_sipmessage(respSipMsg);	// ������Ϣ������Ϣ�¼�
			if (event)
			{
				event->transactionid = trn->transactionid; // �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(event);
			}
			return;
		}
	}
	
	SipMsgParser sipMsgParser;
	if (0) // ע��
	{
	} 
	else // ע��
	{
	}
}

void SipCallback::osip_nist_rcv_notify_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_subscribe_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_bye_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_nist_rcv_info_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_rcv_message_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
	// �ж�sip��Ϣ���Ƿ����
	if (trn == nullptr || trn->your_instance == nullptr || sipMsg == nullptr)
	{
		return;
	}
	
	SipServer* sipServer = SipServer::GetInstance();
	SipDialogs* sipDialogs = sipServer->mSipDialogs;
	SipDatabase* sipDB = sipServer->mSipDB;
	AppContext* appContext = (AppContext*)trn->your_instance;

	SipMsgParser sipMsgParser;
	if (sipMsgParser.ParserSipMsgBody(sipMsg)) // ������Ϣ��ʧ��
	{
		osip_message_t* respSipMsg = nullptr;
		sipMsgParser.CreateBadRequestSipMsg(sipMsg, respSipMsg);
		osip_event_t* event = osip_new_outgoing_sipmessage(respSipMsg);	// ������Ϣ������Ϣ�¼�
		if (event)
		{
			event->transactionid = trn->transactionid; // �����¼���transactionidΪ���λص�transaction��id
			sipServer->mSipEvents.push_back(event);
		}
		return;
	}

	if (SipUtilities::CompareStrCase("Control", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DEVICE_PTZ || // PTZ������Ϣ����Ӧ�����
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_TELE_BOOT ||  // Զ����������Ӧ�����
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_IFAME ||      // ǿ�ƹؼ�֡����Ӧ�����
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DRAG_ZOOM)    // ����Ŵ���С����Ӧ�����
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_RECORD || // �ֶ�¼���ֶ�ֹͣ¼����Ӧ�����
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_GUARD ||  // ������������������Ӧ�����
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_ALARM ||  // ������λ����Ӧ�����
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_HOME_POSITION || // ����λ����Ӧ�����
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DEVICE_CONFIG)   // �豸���ã���Ӧ�����
		{
		}
	} 
	else if (SipUtilities::CompareStrCase("Query", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_STATUS ||   // �豸״̬��ѯ����
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_INFO ||     // �豸��Ϣ��ѯ����
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_CONFIG_DOWNLOAD || // �豸���ò�ѯ
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_PRESET_QUERY)      // Ԥ��λ��ѯ
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_DEVICE_CATALOG)// �յ��ϼ���ѯ����Ŀ¼
		{

		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_RECORD_INFO) // ��ʷ����Ƶ�ļ�����
		{

		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_ALARM) // ������Ϣ���Ĵ�
		{

		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_MOBILE_POSITION) // �ƶ��豸��Ϣ���Ĵ�
		{

		}
	} 
	else if (SipUtilities::CompareStrCase("Notify", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_KEEPALIVE)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_ALARM)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_MEDIA_STATUS)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_BROADCAST)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_MOBILE_POSITION)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_CATALOG)
		{
		}
	} 
	else if (SipUtilities::CompareStrCase("Response", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONTROL || // ��Ӧ���豸���ƣ����ϼ�ת�������¼��ظ�
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG)    // ��Ӧ���豸���ƣ��豸����
		{ 
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CATALOG_QUERY ||  // �յ��¼��ظ���Ŀ¼��Ϣ
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_CATALOG_QUERY_RECEIVED|| // �յ��¼�Ŀ¼��ѯ��Ӧ
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_CATALOG_RECEIVED ||      // �յ��¼�Ŀ¼��Ϣ��Ӧ
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_BROADCAST ||             // �����㲥Ӧ��
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_ALARM_NOTIFY)
		{
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_INFO_QUERY ||     // �յ��¼��豸��Ϣ��ѯ��Ӧ
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_STATUS_QUERY ||   // �յ��¼��豸״̬��ѯ��Ӧ
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG_DOWNLOAD|| // �豸���ò�ѯӦ��
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_PRESET_QUERY ||   // Ԥ��λ��ѯӦ��
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DOCUMENT_RECORD_INFO)    // ��ʷ�ļ���ѯ
		{
		}
	}
}

void SipCallback::osip_nict_status_timeout_cb(int type, osip_transaction_t * trn, osip_message_t * sipMsg)
{
}

void SipCallback::osip_ict_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_ist_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_nict_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_nist_kill_transaction_cb(int type, osip_transaction_t * trn)
{
}

void SipCallback::osip_ict_transport_error_cb(int type, osip_transaction_t *, int error)
{
}

void SipCallback::osip_ist_transport_error_cb(int type, osip_transaction_t *, int error)
{
}

void SipCallback::osip_nict_transport_error_cb(int type, osip_transaction_t *, int error)
{
}

void SipCallback::osip_nist_transport_error_cb(int type, osip_transaction_t *, int error)
{
}
