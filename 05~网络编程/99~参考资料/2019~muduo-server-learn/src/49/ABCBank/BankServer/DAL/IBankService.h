#ifndef _IBANK_SERVICE_H_
#define _IBANK_SERVICE_H_

#include "Account.h"
#include "TransDetail.h"

#include <list>
using namespace std;

namespace DAL
{

class IBankService
{
public:
	// �û���¼
	virtual int UserLogin(const string& user, const string& pass) = 0;
	// ����
	virtual int OpenAccount(Account& account) = 0;
	// ����
	virtual int CloseAccount(Account& account, double& interest) = 0;
	// �޸�����
	virtual int ChangePassword(Account& account, const string& newPass) = 0;
	// ���
	virtual int Deposit(Account& account) = 0;
	// ȡ��
	virtual int WithDraw(Account& account) = 0;
	// ת��
	virtual int Transfer(Account& account, const string& otherAccountId) = 0;
	// ��ѯ���
	virtual int BalanceInquiry(Account& account) = 0;
	// ��ѯĳ�ձ���
	virtual int QueryDayBill(list<TransDetail>& result, int page, const string& date) = 0;
	// �������ڲ�ѯ��ϸ
	virtual int QueryHistoryBill(list<TransDetail>& result, int page, const string& begin, const string& end) = 0;
	// �����ʺŲ�ѯ��ϸ
	virtual int QueryAccountHistoryBill(list<TransDetail>& result, int page, const string& accountId, const string& begin, const string& end) = 0;
};

}
#endif /* _IBANK_SERVICE_H_ */