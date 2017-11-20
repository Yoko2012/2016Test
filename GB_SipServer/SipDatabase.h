#ifndef _SIP_DATABASE_H_
#define _SIP_DATABASE_H_

#include <iostream>
#include <string>
#include <vector>

#define OTL_ODBC_MYSQL
#include "otlv4.h"

typedef std::vector<std::string> VecStrings;

class SipDatabase
{
public:
	SipDatabase();
	virtual ~SipDatabase();

	int LoginDatabase();
	int LogoutDatabase();
	int CheckDB();

	int GetIntData(const std::string sql, int &data);
	std::string GetStringData(const std::string sql);
	int GetStringData(const std::string sql, std::string &data);

	// ��ȡ����ƽ̨��Ϣ
	int GetLocalPort();
	std::string GetLocalIP();
	std::string GetLocalSipID();
	std::string GetLocalSipDomain();
	int GetLocalPlatformParam(std::string &sipId, std::string &sipIP, std::string &sipPort, std::string &sipDomain);

	// ��ȡ�ϼ�ƽ̨���¼�ƽ̨SipID
	int GetUpperPlatformIDs(VecStrings &sipIds);
	int GetLowerPlatformIDs(VecStrings &sipIds);

	int GetPlatformIdByDeviceID(const std::string deviceID, int &platformID);
	bool IsGb28181Device(std::string deviceID);
	bool IsGb28181Device(int resID);

	// ��ȡƽ̨����
	std::string GetUpperPassword(const std::string sipID);
	std::string GetLowerPassword(const std::string sipID);

	// ƽ̨״̬��λ
	int ResetPlatformStatus();

	// ����ƽ̨/�豸״̬
	int SetDeviceStatus(const std::string deviceID, const std::string ststus);
	int GetDeviceStatus(const std::string deviceID, std::string &ststus);

	// ��ȡ�豸��Ϣ

	// ����tbl_group���ParentID�ֶ�
	int SetGroupParentID();
private:
	otl_connect db;

	int InsertData(std::string sqlStr); // ��������
	int DeleteData(std::string sqlStr); // ɾ������
	int UpdateData(std::string sqlStr); // ��������
	int ExecuteSQL(const std::string sql);

	int GetStringVec(VecStrings &vecStringDatas, std::string sql);
};
#endif