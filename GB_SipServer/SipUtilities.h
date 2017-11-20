#ifndef _SIP_UTILITIES_H_
#define _SIP_UTILITIES_H_
#include <string>
#include <random>
#include <chrono>
#include <bitset>
#include <time.h>

#ifdef WIN32
#include <objbase.h>
#else
//#include <uuid/uuid.h>
#endif

namespace SipUtilities
{
	bool CompareStrCase(const char* src, const char* dst);

	int Str2Int(std::string str, int &num);       // ���ַ���ת��Ϊint������ֵ: 0�ɹ���-1ʧ��
	int Str2Double(std::string str, double &num); // ���ַ���ת��Ϊdouble������ֵ: 0�ɹ���-1ʧ��

	std::string CreatUuid();                     // �����ַ���8-4-4-4-12
	std::string RandLengthStr(const int length); // ��������ַ������ַ���

	std::string BitsetToHex(std::bitset<8> bite);            // ��bitset<8>ת��Ϊ16���Ʊ�ʾ,ÿһλΪ�����ַ���16���ַ�
	int HexToBitset(std::bitset<8> &bitset, std::string hex);// ��16���Ʊ�ʾ��1��2���ַ�ת��Ϊbitset<8>����
	int HexStr2Int(int &num, std::string hex);               // ���ַ���ʾ��16����ת��Ϊ10��������
	int GetBiteByNumber(const std::string cmd, const int pos, std::bitset<8> &bitset);// ���ַ�����ȡposλ�õ��ַ�

	int GBTime2NptTime(std::string gbTime, std::string & nptTime);

	std::string GetLocalTime();

};
#endif // _SIP_UTILITIES_H_

