#ifndef _SIP_DEF_H
#define _SIP_DEF_H
/************************************************************************/
/* ���ļ����ݲ����޸�
/************************************************************************/
enum MESSAGE_MANSCDP_TYPE
{
	MESSAGE_MANSCDP_ERROR,	// ���������

	//���ƣ�10
	CONTROL_DEVICE_PTZ,		// �豸ptz����
	CONTROL_TELE_BOOT,		// Զ����������
	CONTROL_RECORD,			// ¼����ƣ���ʼ¼��ֹͣ¼��
	CONTROL_GUARD,			// ����������
	CONTROL_ALARM,			// ������λ����
	CONTROL_IFAME,			// ǿ�ƹؼ�֡
	CONTROL_DRAG_ZOOM,	    // ����Ŵ�\��С
	CONTROL_HOME_POSITION,	// ��������������Զ���λ
	CONTROL_DEVICE_CONFIG,	// �豸����

	//��ѯ����8
	QUERY_DEVICE_STATUS,	// �豸״̬��ѯ����
	QUERY_DEVICE_CATALOG,	// �豸Ŀ¼��Ϣ��ѯ����
	QUERY_DEVICE_INFO,		// �豸��Ϣ��ѯ����
	QUERY_RECORD_INFO,		// �ļ�Ŀ¼��������
	QUERY_ALARM,			// ������ѯ
	QUERY_CONFIG_DOWNLOAD,	// �豸���ò�ѯ
	QUERY_PRESET_QUERY,		// �豸Ԥ��λ��ѯ
	QUERY_MOBILE_POSITION,	// �ƶ��豸λ�����ݲ�ѯ

	//֪ͨ,5
	NOTIFY_KEEPALIVE,		// ״̬��Ϣ���ͣ�����
	NOTIFY_ALARM,			// ����֪ͨ
	NOTIFY_MEDIA_STATUS,	// ý��֪ͨ��filetoend
	NOTIFY_BROADCAST,		// �����㲥֪ͨ
	NOTIFY_MOBILE_POSITION,	// �ƶ��豸λ������֪ͨ
	NOTIFY_CATALOG,

	//Ӧ��12
	RESPONSE_DEVICE_CONTROL,			// �豸����Ӧ��
	RESPONSE_ALARM_NOTIFY,				// ����֪ͨӦ��
	RESPONSE_DEVICE_CATALOG_QUERY,		// �豸Ŀ¼��Ϣ��ѯӦ��Ŀ¼֪ͨ��
	RESPONSE_CATALOG_QUERY_RECEIVED,	// Ŀ¼��Ϣ��ѯ�յ�Ӧ��Ŀ¼����Ӧ��
	RESPONSE_CATALOG_RECEIVED,			// Ŀ¼�յ�Ӧ��Ŀ¼֪ͨӦ��
	RESPONSE_DEVICE_INFO_QUERY,			// �豸��Ϣ��ѯӦ��
	RESPONSE_DEVICE_STATUS_QUERY,		// �豸״̬��Ϣ��ѯӦ��
	RESPONSE_DOCUMENT_RECORD_INFO,		// �ļ�Ŀ¼����Ӧ��
	RESPONSE_DEVICE_CONFIG,				// �豸����Ӧ��
	RESPONSE_DEVICE_CONFIG_DOWNLOAD,	// �豸���ò�ѯӦ��
	RESPONSE_DEVICE_PRESET_QUERY,		// �豸Ԥ��λ��ѯӦ��
	RESPONSE_BROADCAST,					// �����㲥Ӧ��

	CATALOG_CHANGED,

	XML_COUNT
};

enum CONTROL_TYPE
{
	PTZ_STOP		= 0x1009,	//�����ֹͣ����
	VIDEO_SWITCH	= 0x1010,	//������л�
	PTZ_UP			= 0x1011,	//������ת����ͷ
	PTZ_UP_STOP		= 0x1012,	//����ͷ������תֹͣ
	PTZ_DOWN		= 0x1013,	//������ת����ͷ
	PTZ_DOWN_STOP	= 0x1014,	//����ͷ������תֹͣ
	PTZ_LEFT		= 0x1015,	//������ת����ͷ
	PTZ_LEFT_STOP	= 0x1016,	//����ͷ������תֹͣ
	PTZ_RIGHT		= 0x1017,	//������ת����ͷ
	PTZ_RIGHT_STOP	= 0x1018,	//����ͷ������תֹͣ
	ZOOM_IN			= 0x1019,	//��ͷ�䱶�Ŵ�
	ZOOM_IN_STOP	= 0x1020,	//��ͷ�䱶�Ŵ�ֹͣ
	ZOOM_OUT		= 0x1021,	//��ͷ�䱶��С
	ZOOM_OUT_STOP	= 0x1022,	//��ͷ�䱶��Сֹͣ

	FOCUS_FAR		= 0x1023,	//��ͷ�����Զ
	FOCUS_FAR_STOP	= 0x1024,	//��ͷ�����Զֹͣ
	FOCUS_NEAR		= 0x1025,	//��ͷ�������
	FOCUS_NEAR_STOP = 0x1026,	//��ͷ�������ֹͣ
	IRIS_OPEN		= 0x1027,	//��ͷ��Ȧ�Ŵ�
	IRIS_OPEN_STOP	= 0x1028,	//��ͷ��Ȧ�Ŵ�ֹͣ
	IRIS_CLOSE		= 0x1029,	//��ͷ��Ȧ��С
	IRIS_CLOSE_STOP = 0x1030,	//��ͷ��Ȧ��Сֹͣ

	GET_PRESET		= 0x1031,	//��ѯԤ��λ,20170912���
	GOTO_PRESET		= 0x1032,	//����Ԥ��λ
	ADD_PRESET		= 0x1033,	//���Ԥ��λ\shezhi
	DEL_PRESET		= 0x1034,	//ɾ��Ԥ��λ
	RENAME_PRESET	= 0x1035,	//������Ԥ��λ	,20170912��� 

	//////////////////////////////////////////////////////////////////////////
// 	GET_CRUISE		= 0x1036,	// ��ѯѲ��·��
 	RUN_CRUISE		= 0x1037,	// ����Ѳ��·��
 	STOP_CRUISE		= 0x1038,	// ֹͣѲ��
// 	RENAME_CRUISE	= 0x1039,	// ������Ѳ��·��
 	ADD_CRUISE		= 0x103A,	// ���Ѳ��
 	DEL_CRUISE		= 0x103B,	// ɾ��Ѳ��		
// 	STA_MEM_TRACK	= 0x103C,	// ��ʼ��¼�켣		
// 	STO_MEM_TRACK	= 0x103D,	// ֹͣ��¼�켣		
// 	RUN_TRACK		= 0x103E,	// ��ʼ�켣		
// 	RUN_TRACK_STOP	= 0x103F,	// ֹͣ�켣		


	//ADD_CRUISE = 0x1A,		//����Ѳ����
	//DEL_CRUISE = 0x1B,		//ɾ��һ��Ѳ����
	//RUN_CRUISE = 0x1E,		//��ʼѲ��
	//STOP_CRUISE = 0x1F,		//ֹͣѲ��
	SET_NAVI_SPEED		= 0x1C,	//����Ѳ���ٶ�
	SET_NAVI_STOP_TIME	= 0x1D, //����Ѳ��ͣ��ʱ��

	//////////////////////////////////////////////////////////////////////////
	PTZ_LEFT_UP			= 0x1040,	// ��������ת����ͷ		 
	PTZ_LEFT_UP_STOP	= 0x1041,	// ����ͷ��������תֹͣ	
	PTZ_LEFT_DOWN		= 0x1042,	// ��������ת����ͷ		
	PTZ_LEFT_DOWN_STOP	= 0x1043,	// ����ͷ��������תֹͣ		
	PTZ_RIGHT_UP		= 0x1044,	// ��������ת����ͷ		
	PTZ_RIGHT_UP_STOP	= 0x1045,	// ����ͷ��������תֹͣ		 
	PTZ_RIGHT_DOWN		= 0x1046,	// ��������ת����ͷ		
	PTZ_RIGHT_DOWN_STOP = 0x1047,	// ����ͷ��������תֹͣ	

	//////////////////////////////////////////////////////////////////////////
	//RUN_SCAN = 0x1051,			//��ʼ�Զ�ɨ��
	AUTO_SCANNING_LEFT  = 0x21,		//�����Զ�ɨ����߽�
	AUTO_SCANNING_RIGHT = 0x22,		//�����Զ�ɨ���ұ߽�
	SET_AUTO_SCANNING_SPEED = 0x23,	//�����Զ�ɨ���ٶ�
	//STOP_SCAN = 0x1052,			//ֹͣɨ��

// 	GET_SCAN = 0x1050,				// ��ѯɨ��		
 	RUN_SCAN = 0x1051,				// ����ɨ��		
 	STOP_SCAN = 0x1052,				// ֹͣɨ��		
// 	RENAME_SCAN = 0x1053,			// ������ɨ��		
// 	ADD_SCAN = 0x1054,				// ���ɨ��		
// 	DEL_SCAN = 0x1055,				// ɾ��ɨ��
	//////////////////////////////////////////////////////////////////////////

	SET_GUARD     = 0x1060,	// ����		
	RESET_GUARD   = 0x1061,	// ����		
	REMOTE_START  = 0X1070,	// Զ������
	IFAME         = 0X1071,	// ǿ�ƹؼ�֡
	SYSTEM_ALARM  = 0X1072,	// ϵͳ����
	RESET_ALARM   = 0X1073,	// ������λ
	DRAG_ZOON_IN  = 0X1074,	// ����Ŵ�
	DRAG_ZOON_OUT = 0X1075,	// ������С
	HOME_POSITION = 0X1076,	// ����λ����

	OPEN_CAMERA = 0X25,
	CLOSE_CAMERA = 0X26,

};
#endif