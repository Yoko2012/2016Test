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
		if (SipUtilities::CompareStrCase(trn->topvia->protocol, "TCP")) // TCP����
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
		osip_message_t* regSipMsg = nullptr;
		if (sipMsg->status_code == 401)
		{
			// ��������Ȩ��SipMsg
			sipMsgBuilder.CreateAuthRegisterMsg(trn->orig_request, trn->last_response, regSipMsg);

			// ������Ϣ
			osip_event_t* regEvent = osip_new_outgoing_sipmessage(regSipMsg);
			sipServer->mSipEvents.push_back(regEvent);
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
	else if (SipUtilities::CompareStrCase(sipMsgMethod, "MESSAGE"))
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
			sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, respSipMsg);
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
			sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, respSipMsg);
			osip_event_t* event = osip_new_outgoing_sipmessage(respSipMsg);	// ������Ϣ������Ϣ�¼�
			if (event)
			{
				event->transactionid = trn->transactionid; // �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(event);
			}
			return;
		}
	}
	
	//int expires = 0;
	SipMsgParser sipMsgParser;
	osip_header_t *expires = nullptr;
	osip_message_get_expires(sipMsg, 0, &expires);
	if (expires == nullptr) // ������expires�ֶ�
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
	} 
	else
	{
		// ����registerDialog�Ƿ����
		std::string fromSipID = sipMsg->from->url->username;
		osip_dialog_t* registerDialog = sipDialogs->FindRegisterDialog(fromSipID, DialogHint::UAS);

		SipMsgBuilder sipMsgBuilder;
		if (!strcmp(expires->hvalue, "0")) // ע��
		{
			sipDB->SetDeviceStatus(fromSipID, "OFF");// ����ƽ̨�Ƿ�����״̬Ϊ'OFF'
		
			osip_message_t* dstMsg = nullptr;
			if (registerDialog != nullptr) // �¼����豸��ע��
			{				
				if (sipMsg->authorizations.node == nullptr) // ������Ȩ��401
				{
					sipMsgBuilder.CreateUnauthrizedMsg(sipMsg, dstMsg);
				}
				else // ����Ȩ
				{
					if (true) // ��Ȩ��֤ͨ����200
					{
						// �ظ���data��200OK
						sipMsgBuilder.CreateRegister200OKSipMsg(sipMsg, dstMsg);
					}
					else // ��Ȩ��֤δͨ����403
					{
						sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, dstMsg);
					}
				}
			} 
			else// �¼����豸δע�ᣬ�ظ�404
			{
				sipMsgBuilder.CreateNotFoundSipMsg(sipMsg, dstMsg);
			}

			osip_event_t* event = osip_new_outgoing_sipmessage(dstMsg); // ������Ϣ������Ϣ�¼�
			event->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
			sipServer->mSipEvents.push_back(event);	// ����Ϣ�¼����뵽��transaction�������¼�������
		}
		else // ע��
		{
			osip_message_t* dstMsg = nullptr;
			if (registerDialog != nullptr) // ƽ̨�Ѿ�ע��,ˢ��ע��
			{
				// ��ȡdialog��callID��fromTag
				std::string dialogCallID = registerDialog->call_id;
				std::string dialogFromTag = registerDialog->remote_tag;
				std::string dialogToTag = registerDialog->local_tag;

				// ��ȡsipMsg��callID��fromTag
				std::string sipMsgCallID = "";
				std::string sipMsgFromTag = "";
				//sipMessageObj.GetSipMsgCallIdAndFromTag(sipMsg, sipMsgCallID, sipMsgFromTag);

				// �ж�ƽ̨�Ƿ��Ѿ�ע��
				if ((dialogCallID == sipMsgCallID) && (dialogFromTag == sipMsgFromTag)) // �Ѿ�ע��ƽ̨ˢ��ע��
				{ 
					if (sipMsg->authorizations.node == nullptr) // ������Ȩ��401
					{
						sipMsgBuilder.CreateUnauthrizedMsg(sipMsg, dstMsg, dialogToTag);
					}
					else // ����Ȩ
					{
						if (true) // ��Ȩ��֤ͨ����200
						{
							// �ظ���data��200OK
							sipMsgBuilder.CreateRegister200OKSipMsg(sipMsg, dstMsg);

							// ����ƽ̨״̬Ϊ��ON��
							sipDB->SetDeviceStatus(fromSipID, "ON");

							// ����registerDialog
							osip_dialog_t* newRegDialog = sipDialogs->CreateSipDialog(trn, dstMsg, REGISTER_DIALOG);
							sipDialogs->InsertSipDialog(newRegDialog, REGISTER_DIALOG);
						}
						else // ��Ȩ��֤δͨ����403
						{
							sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, dstMsg);
						}
					}
				}
				else // ��ע��ƽ̨�쳣�˳�������ע��
				{
					DialogInfo* regDialogInfo = (DialogInfo*)registerDialog->your_instance;

					// ɾ���Ѿ�����dialog
					sipDialogs->RemoveSipDialogByDialogID(regDialogInfo->dialogID, REGISTER_DIALOG);
					sipDialogs->DestorySipDialog(registerDialog);

					// ͬ ��ƽ̨��һ��ע��
					if (sipMsg->authorizations.node == nullptr) // ������Ȩ��401
					{
						sipMsgBuilder.CreateUnauthrizedMsg(sipMsg, dstMsg);
					}
					else // ����Ȩ
					{
						if (true) // ��Ȩ��֤ͨ����200
						{
							// �ظ���data��200OK
							sipMsgBuilder.CreateRegister200OKSipMsg(sipMsg, dstMsg);

							// ����ƽ̨״̬Ϊ��ON��
							sipDB->SetDeviceStatus(fromSipID, "ON");

							// ����registerDialog
							osip_dialog_t* newRegDialog = sipDialogs->CreateSipDialog(trn, dstMsg, REGISTER_DIALOG);
							sipDialogs->InsertSipDialog(newRegDialog, REGISTER_DIALOG);
						}
						else // ��Ȩ��֤δͨ����403
						{
							sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, dstMsg);
						}
					}
				}
			} 
			else // ƽ̨��һ���򱾼�ע��
			{
				if (sipMsg->authorizations.node == nullptr) // ������Ȩ��401
				{
					sipMsgBuilder.CreateUnauthrizedMsg(sipMsg, dstMsg);
				} 
				else // ����Ȩ
				{
					if (true) // ��Ȩ��֤ͨ����200
					{
						// �ظ���data��200OK
						sipMsgBuilder.CreateRegister200OKSipMsg(sipMsg, dstMsg);

						// ����ƽ̨״̬Ϊ��ON��
						sipDB->SetDeviceStatus(fromSipID, "ON");

						// ����registerDialog
						osip_dialog_t* newRegDialog = sipDialogs->CreateSipDialog(trn, dstMsg, REGISTER_DIALOG);
						sipDialogs->InsertSipDialog(newRegDialog, REGISTER_DIALOG);
					} 
					else // ��Ȩ��֤δͨ����403
					{
						sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, dstMsg);
					}
				}
			}
			osip_event_t* event = osip_new_outgoing_sipmessage(dstMsg); // ������Ϣ������Ϣ�¼�
			event->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
			sipServer->mSipEvents.push_back(event);	// ����Ϣ�¼����뵽��transaction�������¼�������
		}
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
		osip_event_t* badRequestEvent = osip_new_outgoing_sipmessage(respSipMsg);	// ������Ϣ������Ϣ�¼�
		if (badRequestEvent)
		{
			badRequestEvent->transactionid = trn->transactionid; // �����¼���transactionidΪ���λص�transaction��id
			sipServer->mSipEvents.push_back(badRequestEvent);
		}
		return;
	}

	if (SipUtilities::CompareStrCase("Control", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DEVICE_PTZ || // PTZ������Ϣ����Ӧ�����
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_TELE_BOOT ||  // Զ����������Ӧ�����
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_IFAME ||      // ǿ�ƹؼ�֡����Ӧ�����
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DRAG_ZOOM)    // ����Ŵ���С����Ӧ�����
		{	// ��Ӧ����ƣ���̨��Զ��������ǿ�ƹؼ�֡������Ŵ�������С
			bool isUpper = true;
			if (isUpper) // �����ϼ�
			{
				// ���ϼ��ظ�200OK

				bool is28181 = true;
				int platformID = 1;
				if (platformID == 1 && !is28181)// �����ǹ����豸
				{
				}
				else // ���������豸���¼��豸
				{
				}
			} 
			else // �Ǳ����ϼ�
			{
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_RECORD || // �ֶ�¼���ֶ�ֹͣ¼����Ӧ�����
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_GUARD ||  // ������������������Ӧ�����
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_ALARM ||  // ������λ����Ӧ�����
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_HOME_POSITION || // ����λ����Ӧ�����
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::CONTROL_DEVICE_CONFIG)   // �豸���ã���Ӧ�����
		{// ��Ӧ����ƣ�¼����ơ���������/������������λ������λ���ơ��豸����

			bool isUpper = true;
			if (isUpper) // �����ϼ�
			{
				// ���ϼ��ظ�200OK

				// �ж��豸����������豸����
				bool is28181 = true;
				int platformID = 1;
				if (platformID == 1 && !is28181)// �����ǹ����豸
				{
					switch (sipMsgParser.mXmlParam.manscdpType)// ����������ͨ�ţ����ϼ���Ӧ
					{
					case CONTROL_RECORD:
						break;
					case CONTROL_GUARD:
						break;
					case CONTROL_ALARM:
						break;
					case CONTROL_HOME_POSITION:
						break;
					case CONTROL_DEVICE_CONFIG:
						break;
					default:
						break;
					}
				}
				else // ����28181�����豸���¼��豸
				{
					// ���¼�ת��

					// ��¼��Ϣ
					switch (sipMsgParser.mXmlParam.manscdpType)
					{
					case CONTROL_DEVICE_CONFIG: // �豸��������
						//xmlDialog.cmdType = RESPONSE_DEVICE_CONFIG; // �豸������Ӧ
						break;
					default:// �豸����
						//xmlDialog.cmdType = RESPONSE_DEVICE_CONTROL; // �豸������Ӧ
						break;
					}
				}
			}
			else // �Ǳ����ϼ�
			{
			}
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
			bool isUpper = true;
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* respSipMsg = nullptr;

			if (isUpper) // �ϼ�
			{				
				sipMsgBuilder.Create200OKSipMsg(sipMsg, respSipMsg);// ���¼��ظ�200OK
				sipDB->SetDeviceStatus(sipMsg->from->url->username, "ON");

				// ����������Ŀ¼��֯�ܹ������͵�һ��Ŀ¼��Ϣ
				std::string protol = "";
				protol = trn->topvia->protocol;
				//StartResponseCatalog(fromSipID, sipMessageObj.mXmlCmdInfo.deviceID, sipMessageObj.mXmlCmdInfo.sn, protol);
			}
			else // ���ϼ�
			{
				sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, respSipMsg);// Forbidden
				sipDB->SetDeviceStatus(sipMsg->from->url->username, "OFF");
			}
			osip_event_t* keepaliveEvent = osip_new_outgoing_sipmessage(respSipMsg); // ������Ϣ������Ϣ�¼�
			if (keepaliveEvent)
			{
				keepaliveEvent->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(keepaliveEvent);    // ����Ϣ�¼����뵽��transaction�������¼�������
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::QUERY_RECORD_INFO) // ��ʷ����Ƶ�ļ�����
		{
			bool isUpper = true;
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* respSipMsg = nullptr;
			if (isUpper) // ���ϼ��ظ�200OK
			{
				sipMsgBuilder.Create200OKSipMsg(sipMsg, respSipMsg);// ���¼��ظ�200OK
				osip_event_t* recordEvent = osip_new_outgoing_sipmessage(respSipMsg); // ������Ϣ������Ϣ�¼�
				if (recordEvent)
				{
					recordEvent->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
					sipServer->mSipEvents.push_back(recordEvent);		// ����Ϣ�¼����뵽��transaction�������¼�������
				}
			}

			// ����Ǽ��д洢����ǰ���豸�洢
			bool isDeviceStore = true;
			if (isDeviceStore) // ǰ���豸�洢
			{
				// ���豸���¼�ת��

				// ��¼��ѯ��Ϣ

			} 
			else // ���д洢����ѯ���ݿ�
			{
			}
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
			osip_dialog_t* registerDialog = nullptr;
			registerDialog = sipDialogs->FindRegisterDialog(sipMsg->from->url->username, DialogHint::UAS);

			SipMsgBuilder sipMsgBuilder;
			osip_message_t* respSipMsg = nullptr;
			if (registerDialog != nullptr) // �¼��Ѿ�ע��
			{
				// ����LastResponseTime
				DialogInfo* dialogInfo = (DialogInfo*)(registerDialog->your_instance);
				dialogInfo->lastResponseTime = time(NULL);				
				sipMsgBuilder.Create200OKSipMsg(sipMsg, respSipMsg);// ���¼��ظ�200OK
				sipDB->SetDeviceStatus(sipMsg->from->url->username, "ON");
			} 
			else // �¼�δע��
			{				
				sipMsgBuilder.CreateForbiddenSipMsg(sipMsg, respSipMsg);// Forbidden
				sipDB->SetDeviceStatus(sipMsg->from->url->username, "OFF");
			}

 			osip_event_t* keepaliveEvent = osip_new_outgoing_sipmessage(respSipMsg); // ������Ϣ������Ϣ�¼�
			if (keepaliveEvent)
			{
				keepaliveEvent->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(keepaliveEvent);    // ����Ϣ�¼����뵽��transaction�������¼�������
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_ALARM)
		{// �յ�����֪ͨ�������ظ�200OK���ٻظ�������Ӧ����Ŀ���豸ת������֪ͨ������֪ͨ�����д���
			// ��������Ϣд�뵽���ݿ�

			// ���ͷ��ظ�200OK
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // ������Ϣ������Ϣ�¼�
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(okEvent);		// ����Ϣ�¼����뵽��transaction�������¼�������
			}

			// ���¼��ظ�������Ӧ
			osip_message_t* responseBroadcastNotify = nullptr;
			sipMsgBuilder.mXmlParam.manscdpType = RESPONSE_ALARM_NOTIFY;
			sipMsgBuilder.mXmlParam.deviceID = sipMsgParser.mXmlParam.deviceID;
			sipMsgBuilder.mXmlParam.sn = sipMsgParser.mXmlParam.sn;
			sipMsgBuilder.mXmlParam.responseParam.result = "OK";
			sipMsgBuilder.CreateSipMsgXml(responseBroadcastNotify, sipMsg->from->url->username);
			osip_event_t* respAlarmNotifyEvent = osip_new_outgoing_sipmessage(responseBroadcastNotify);
			if (respAlarmNotifyEvent)
			{
				sipServer->mSipEvents.push_back(respAlarmNotifyEvent); // ����Ϣ�¼����뵽��transaction�������¼�������
			}

			// ���ϼ��ظ�����֪ͨ
			if (true)
			{
			} 
			else //��ͻ��˻ظ�����֪ͨ
			{
			}

		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_MEDIA_STATUS)
		{
			// ���ͷ��ظ�200OK
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // ������Ϣ������Ϣ�¼�
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(okEvent);		// ����Ϣ�¼����뵽��transaction�������¼�������
			}

			// ���ϼ�/�ͻ��˷���MediaStutas
			osip_dialog_t* playbackDialog = nullptr;
			DialogInfo* playbackDialogInfo = nullptr;
			if (playbackDialog != nullptr)// ����ʷ��Ƶ����/���ش���
			{
				playbackDialogInfo = (DialogInfo*)playbackDialog->your_instance;

				// �������ϼ�������dialog
				osip_dialog_t* callerDialog = nullptr;
				DialogInfo* callerDialogInfo = nullptr;
				int callerDialogID = playbackDialogInfo->callerDialogID;

				callerDialog = sipDialogs->FindSipDialogByDialogID(callerDialogID, DialogType::PLAYBACK_DIALOG);
				if (callerDialog)// ��ƽ̨�Ǽ���ƽ̨���ϼ��������ϼ�ƽ̨ת��fileToEnd
				{
					callerDialogInfo = (DialogInfo*)callerDialog->your_instance;

				} 
				else // ��streaming����fileToEnd,��ƽ̨Ϊ����ƽ̨���ϼ������¼�����bye
				{
					// ��streamingͨ��

					// ���¼�����bye��
				}
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_BROADCAST)
		{
			// ���ͷ��ظ�200OK
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // ������Ϣ������Ϣ�¼�
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(okEvent);		// ����Ϣ�¼����뵽��transaction�������¼�������
			}

			// ���ͷ��ظ������㲥Ӧ��
			osip_message_t* responseBroadcastNotify = nullptr;
			sipMsgBuilder.mXmlParam.manscdpType = RESPONSE_BROADCAST;
			sipMsgBuilder.mXmlParam.deviceID = sipMsgParser.mXmlParam.notifyParam.broadcast.targetID; // ��������豸����
			sipMsgBuilder.mXmlParam.responseParam.result = "OK";
			sipMsgBuilder.CreateSipMsgXml(responseBroadcastNotify, sipMsg->from->url->username);
			osip_event_t* respBroadcastNotifyEvent = osip_new_outgoing_sipmessage(responseBroadcastNotify);
			if (respBroadcastNotifyEvent)
			{
				sipServer->mSipEvents.push_back(respBroadcastNotifyEvent); // ����Ϣ�¼����뵽��transaction�������¼�������
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_MOBILE_POSITION)
		{// �ڶ���֪ͨ�յ�������Ϣ������
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::NOTIFY_CATALOG)
		{// �ڶ���֪ͨ�յ�������Ϣ������
		}
	} 
	else if (SipUtilities::CompareStrCase("Response", sipMsgParser.mXmlParam.reqRespType.c_str()))
	{
		if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONTROL || // ��Ӧ���豸���ƣ����ϼ�ת�������¼��ظ�
			sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG)    // ��Ӧ���豸���ƣ��豸����
		{ 
			// ���¼��ظ�200OK
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // ������Ϣ������Ϣ�¼�
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(okEvent);		// ����Ϣ�¼����뵽��transaction�������¼�������
			}

			// ����xmlDialog
			//XmlDialog xmlDialog;
			//xmlDialog.cmdType = sipMessageObj.mXmlCmdInfo.manscdpType;
			//xmlDialog.dstDeviceID = sipMessageObj.mXmlCmdInfo.deviceID;

			//if (!sipServer->GetXmlCmdDialog(xmlDialog))// �ϼ�������Ŀ���������ϼ�ת������Ӧ��
			//{
			//	osip_message_t* dstMsg = nullptr;
			//	sipMessageObj.CreateSipMsgCloneBody(sipMsg, dstMsg, xmlDialog.srcDeviceID, xmlDialog.toTag, xmlDialog.fromTag);
			//	osip_event_t* event = osip_new_outgoing_sipmessage(dstMsg);
			//	sipServer->mSipEvents.push_back(event);
			//}
			//return;
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CATALOG_QUERY ||  // �յ��¼��ظ���Ŀ¼��Ϣ
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_CATALOG_QUERY_RECEIVED|| // �յ��¼�Ŀ¼��ѯ��Ӧ
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_CATALOG_RECEIVED ||      // �յ��¼�Ŀ¼��Ϣ��Ӧ
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_BROADCAST ||             // �����㲥Ӧ��
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_ALARM_NOTIFY)
		{
			// ���ͷ��ظ�200OK
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			//SipMsgHeader::SetSipMsgViaProtocal(responseMsg, trn->topvia->protocol);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // ������Ϣ������Ϣ�¼�
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(okEvent);		// ����Ϣ�¼����뵽��transaction�������¼�������
			}
		}
		else if (sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_INFO_QUERY ||     // �յ��¼��豸��Ϣ��ѯ��Ӧ
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_STATUS_QUERY ||   // �յ��¼��豸״̬��ѯ��Ӧ
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_CONFIG_DOWNLOAD|| // �豸���ò�ѯӦ��
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DEVICE_PRESET_QUERY ||   // Ԥ��λ��ѯӦ��
			     sipMsgParser.mXmlParam.manscdpType == MESSAGE_MANSCDP_TYPE::RESPONSE_DOCUMENT_RECORD_INFO)    // ��ʷ�ļ���ѯ
		{
			SipMsgBuilder sipMsgBuilder;
			osip_message_t* okSipMsg = nullptr;
			sipMsgBuilder.Create200OKSipMsg(sipMsg, okSipMsg);
			//SipMsgHeader::SetSipMsgViaProtocal(responseMsg, trn->topvia->protocol);
			osip_event_t* okEvent = osip_new_outgoing_sipmessage(okSipMsg); // ������Ϣ������Ϣ�¼�
			if (okEvent)
			{
				okEvent->transactionid = trn->transactionid;	// �����¼���transactionidΪ���λص�transaction��id
				sipServer->mSipEvents.push_back(okEvent);		// ����Ϣ�¼����뵽��transaction�������¼�������
			}

			// ����xmlDialog
			//XmlDialog xmlDialog;
			switch (sipMsgParser.mXmlParam.manscdpType)
			{
			case RESPONSE_DEVICE_INFO_QUERY:
				//xmlDialog.cmdType = QUERY_DEVICE_INFO;
				break;
			case RESPONSE_DEVICE_STATUS_QUERY:
				//xmlDialog.cmdType = QUERY_DEVICE_STATUS;
				break;
			case RESPONSE_DEVICE_CONFIG_DOWNLOAD:
				//xmlDialog.cmdType = QUERY_CONFIG_DOWNLOAD;
				break;
			case RESPONSE_DEVICE_PRESET_QUERY:
				//xmlDialog.cmdType = QUERY_PRESET_QUERY;
				break;
			case RESPONSE_DOCUMENT_RECORD_INFO:
				//xmlDialog.cmdType = QUERY_RECORD_INFO;
				break;
			}

			if (true) // �ϼ������ѯ�����ϼ�ת��
			{
			} 
			else // �ͻ��˷�������
			{
				switch (sipMsgParser.mXmlParam.manscdpType)
				{
				case RESPONSE_DEVICE_INFO_QUERY:
				{
					break;
				}					
				case RESPONSE_DEVICE_STATUS_QUERY:
				{
					break;
				}
				case RESPONSE_DEVICE_CONFIG_DOWNLOAD:
				{
					break;
				}
				case RESPONSE_DEVICE_PRESET_QUERY:
				{
					break;
				}
				case RESPONSE_DOCUMENT_RECORD_INFO:
				{
					break;
				}
				}
			}
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
