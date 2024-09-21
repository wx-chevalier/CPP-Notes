#include "Validator.h"

namespace UI
{

// �û�������<10λ����ĸ
bool ValidateName(const std::string& str, char ch)
{
	return str.length() < 10 && isalnum(ch);
}

// ����6-8λ
bool ValidatePass(const std::string& str, char ch)
{
	// �������������ɴ�ӡ�ַ�
	return str.length() < 8;
}

// ���֤18λ
bool ValidateId(const std::string& str, char ch)
{
	if (str.length() >= 18)
		return false;

	if (!isdigit(ch) && ch != 'x' && ch != 'X')
		return false;

	if ((ch == 'x' || ch == 'X') && str.length() != 17)	// ֻ�����һλ������x����X
		return false;

	// TODO���֤��������Ч����֤

	return true;
}

bool ValidateAccountId(const std::string& str, char ch)
{
	return str.length() < 6 && isdigit(ch);
}

// decimal(10,2),�������ֲ�����8λ��С�����ֲ�����2λ���ܳ��Ȳ�����11λ������С���㣩
bool ValidateMoney(const std::string& str, char ch)
{
	if (str.length() >= 11 || (!isdigit(ch) && ch != '.'))
		return false;


	bool findDot = false;
	size_t dotPos = std::string::npos;
	dotPos = str.find('.');
	if (dotPos != std::string::npos)
		findDot = true;


	if (findDot)
	{
		if (ch == '.')
			return false;	// ���ܳ����������
		else if (str.length()-1>=dotPos+2)	// С��������2λ
			return false;
	}
	else if (ch != '.' && str.length() >= 8)	// С����ǰ�����8λ
		return false;

	return true;
}

bool ValidateDate(const std::string& str, char ch)
{
	// ���ڲ�����10λ��ֻ�������ֻ��ߡ�-��
	// TODO������Ч����֤
	return str.length() < 10 && (isdigit(ch) || ch == '-');
}

}
