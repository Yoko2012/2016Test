#ifndef _SIP_MSG_XML_BODY_H_
#define _SIP_MSG_XML_BODY_H_
#include "tinyxml2.h"
#include "SipMsgBody.h"
#include "SipDatabase.h"

typedef int(*CreateBodyCallback)(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);
typedef int(*ParserBodyCallback)(tinyxml2::XMLElement* &rootEle, XmlCmdParam &xmlParam);

class SipMsgXmlBody :
	public SipMsgBody
{
public:
	SipMsgXmlBody();
	virtual ~SipMsgXmlBody();

	virtual int CreateSipMsgBody(void* bodyParameter, std::string &bodyString);
	virtual int ParserSipMsgBody(const std::string bodyString, void* bodyParameter);

private:
	static bool mIsRegistered; // �ж��Ƿ������ûص�����
	static CreateBodyCallback CreateBodyCallbacks[XML_COUNT];
	static ParserBodyCallback ParserCallbacks[XML_COUNT];
	static void SetCreateCallback(int type, CreateBodyCallback cb);
	static void SetParserCallback(int type, ParserBodyCallback cb);
	static void RegisterCreateCallback();
	static void RegisterParserCallback();

private:
	// ���ݿ�
	static SipDatabase* mSipDB;

	// Control
	static int ControlPtzCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	   // �豸Ptz����
	static int ControlTeleBoot(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	   // �豸Զ������
	static int ControlRecordCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);    // �豸¼�����
	static int ControlGuardCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	   // �豸����/����
	static int ControlAlarmCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	   // �豸������λ
	static int ControlIFameCmd(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	   // �ؼ�֡
	static int ControlDragZoom(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	   // ����Ŵ�/��С
	static int ControlHomePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam); // ��������λ
	static int ControlDeviceConfig(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam); // �豸����

	// Query
	static int QueryDeviceStatus(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);  // �豸Ŀ¼��Ϣ��ѯ
	static int QueryCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	      // �豸Ŀ¼��Ϣ��ѯ
	static int QueryDeviceInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	  // �豸Ŀ¼��Ϣ��ѯ
	static int QueryRecordInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	  // ��ʷ¼���ļ���ѯ
	static int QueryAlarm(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);		  // ������ѯ
	static int QueryConfigDownload(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);// �豸���ò�ѯ
	static int QueryPreset(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);        // �豸���ò�ѯ
	static int QueryMobilePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);// �ƶ��豸λ�ò�ѯ

	// Notify
	static int NotifyKeepalive(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	   // �豸״̬֪ͨ
	static int NotifyAlarm(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);		   // ����֪ͨ
	static int NotifyMediaStutas(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);   // ý��״̬֪ͨ
	static int NotifyBroadcast(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	   // �����㲥֪ͨ
	static int NotifyMobilePosition(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);// �ƶ��豸λ������֪ͨ
	static int NotifyCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);       // Ŀ¼���֪ͨ

	// Response
	static int ResponseDeviceControl(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	// �豸����Ӧ��
	static int ResponseAlarmNotify(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);		// ����֪ͨӦ��
	static int ResponseCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);			// Ŀ¼��ѯӦ��
	static int ResponseRcvCatalogQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	// �յ�Ŀ¼��Ϣ��ѯ��Ӧ��
	static int ResponseRcvCatalog(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);		// Ŀ¼��Ϣ��ѯ�յ������յ��¼����͵�Ŀ¼��Ϣ��Ӧ��
	static int ResponseDeviceInfoQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	// �豸��Ϣ��ѯӦ��
	static int ResponseDeviceStatusQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);// �豸״̬��Ϣ��ѯӦ��
	static int ResponseRecordInfo(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);		// �ļ�Ŀ¼����Ӧ��
	static int ResponseDeviceConfig(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	    // �豸����Ӧ��
	static int ResponseDeviceDownload(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);	// �豸���ò�ѯӦ��
	static int ResponsePresetQuery(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);		// �豸Ԥ��λ��ѯӦ��
	static int ResponseBroadcast(tinyxml2::XMLDocument* &xmlDoc, XmlCmdParam &xmlParam);		// �����㲥Ӧ��
	
	// Xml�ĵ�����
	static void InsertChildElement(tinyxml2::XMLDocument *& xmlDoc,
		                           tinyxml2::XMLElement* &parentElement,
		                           std::string childElementName,
		                           std::string childElementValue);
	template<typename T>
	static void InsertChildElement(tinyxml2::XMLDocument *& xmlDoc,
		                           tinyxml2::XMLElement* &parentElement,
		                           std::string childElementName,
		                           T childElementValue);
	static void InsertChildElementWithAttr(tinyxml2::XMLDocument *& xmlDoc,
		                                   tinyxml2::XMLElement* &parentElement,
		                                   std::string childElementName,
		                                   int childElementAttribute);

	static int GetChildElementValue(tinyxml2::XMLElement* parentElement,
		                            std::string elementName,
		                            std::string &elementValue);
	static int GetChildElementValue(tinyxml2::XMLElement* parentElement,
		                            std::string elementName,
		                            int &elementValue);
	static int GetChildElementValue(tinyxml2::XMLElement* parentElement,
		                            std::string elementName,
		                            double &elementValue);
	static int GetElementAttr(tinyxml2::XMLElement* element,
		                           std::string elementAttrName,
		                           int &elementAttrValue);
};
#endif // _SIP_MSG_XML_BODY_H_

template<typename T>
inline void SipMsgXmlBody::InsertChildElement(tinyxml2::XMLDocument *& xmlDoc, 
	                                          tinyxml2::XMLElement *& parentElement, 
	                                          std::string childElementName, 
	                                          T childElementValue)
{
	if (xmlDoc)
	{
		tinyxml2::XMLElement* childElement = xmlDoc->NewElement(childElementName.c_str());
		parentElement->InsertEndChild(childElement);
		childElement->SetText(childElementValue);
	}
}
