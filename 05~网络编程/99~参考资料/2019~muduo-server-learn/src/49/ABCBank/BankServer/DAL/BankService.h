#ifndef _BANK_SERVICE_H_
#define _BANK_SERVICE_H_

#include "IBankService.h"

namespace DAL
{


class BankService : public IBankService
{
public:
	// �û���¼
	int UserLogin(const string& user, const string& pass);
	// ����
	int OpenAccount(Account& account);
	// ����
	int CloseAccount(Account& account, double& interest);
	// �޸�����
	int ChangePassword(Account& account, const string& newPass);
	// ���
	int Deposit(Account& account);
	// ȡ��
	int WithDraw(Account& account);
	// ת��
	int Transfer(Account& account, const string& otherAccountId);
	// ��ѯ���
	int BalanceInquiry(Account& account);
	// ��ѯĳ�ձ���
	int QueryDayBill(list<TransDetail>& result, int page, const string& date);
	// �������ڲ�ѯ��ϸ
	int QueryHistoryBill(list<TransDetail>& result, int page, const string& begin, const string& end);
	// �����ʺŲ�ѯ��ϸ
	int QueryAccountHistoryBill(list<TransDetail>& result, int page, const string& accountId, const string& begin, const string& end);
};

}
#endif /* _BANK_SERVICE_H_ */