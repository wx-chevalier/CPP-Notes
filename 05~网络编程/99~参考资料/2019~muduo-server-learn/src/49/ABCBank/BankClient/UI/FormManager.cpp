#include "FormManager.h"

#include "LoginForm.h"
#include "MainMenuForm.h"
#include "OpenAccountForm.h"
#include "DepositForm.h"
#include "WithdrawalForm.h"
#include "TransferForm.h"
#include "BalanceInquiryForm.h"
#include "ChangePasswordForm.h"
#include "DetailStatementForm.h"
#include "CloseAccountForm.h"

#include "QueryDayBillForm.h"
#include "QueryHistoryBillForm.h"
#include "QueryAccountHistoryBillForm.h"

#include "ReceiptForm.h"
#include "ReportForm.h"


using namespace UI;

FormManager::FormManager()
{
	forms_["LoginForm"] = new LoginForm(0, 0, 80, 25);
	forms_["MainMenuForm"] = new MainMenuForm(0, 0, 80, 25);
	forms_["OpenAccountForm"] = new OpenAccountForm(0, 0, 80, 25, "Open Account");
	forms_["DepositForm"] = new DepositForm(0, 0, 80, 25, "Deposit");
	forms_["WithdrawalForm"] = new WithdrawalForm(0, 0, 80, 25, "Withdraw");
	forms_["TransferForm"] = new TransferForm(0, 0, 80, 25, "Transfer");
	forms_["BalanceInquiryForm"] = new BalanceInquiryForm(0, 0, 80, 25, "BalanceInquiry");
	forms_["ChangePasswordForm"] = new ChangePasswordForm(0, 0, 80, 25, "ChangePassword");
	forms_["DetailStatementForm"] = new DetailStatementForm(0, 0, 80, 25, "Detail Statement");
	forms_["CloseAccountForm"] = new CloseAccountForm(0, 0, 80, 25, "Close Account");
	forms_["QueryDayBillForm"] = new QueryDayBillForm(13, 5, 54, 13);
	forms_["QueryHistoryBillForm"] = new QueryHistoryBillForm(13, 5, 54, 13);
	forms_["QueryAccountHistoryBillForm"] = new QueryAccountHistoryBillForm(13, 5, 54, 15);

	forms_["ReportForm"] = new ReportForm(0, 0, 80, 25);

	char* items[] =
	{
		"��������",
		"��    ��",
		"��    ��",
		"��    ��",
	};
	std::vector<std::string> v(items, items+4);
	// �����ص�
	forms_["OpenAccountReceiptForm"] = new ReceiptForm(10, 6, 60, 13, v, "�����ɹ�");


	char* items2[] =
	{
		"��������",
		"��    ��",
		"��    ��",
		"���׽��",
		"ժ    Ҫ",
		"��    ��",
	};
	std::vector<std::string> v2(items2, items2+6);
	// ��ȡ��ص�
	forms_["ReceiptForm"] = new ReceiptForm(10, 4, 60, 17, v2);

	char* items3[] =
	{
		"��������",
		"��    ��",
		"��    ��",
		"�Է��ʺ�",
		"���׽��",
		"ժ    Ҫ",
		"��    ��",
	};
	std::vector<std::string> v3(items3, items3+7);
	// ת�ʻص�
	forms_["TransferReceiptForm"] = new ReceiptForm(10, 3, 60, 19, v3, "ת�ʳɹ�");

	char* items4[] =
	{
		"��������",
		"��    ��",
		"��    ��",
		"��    ��",
	};
	std::vector<std::string> v4(items4, items4+4);
	// ����ѯ�ص�
	forms_["BalanceInquiryReceiptForm"] = new ReceiptForm(10, 6, 60, 13, v4, "����ѯ�ɹ�");

	char* items5[] =
	{
		"��������",
		"��    ��",
		"��    ��",
		"��    ��",
		"��    Ϣ",
		"��    ��",
	};
	std::vector<std::string> v5(items5, items5+6);
	// �����ص�
	forms_["CloseAccountReceiptForm"] = new ReceiptForm(10, 4, 60, 17, v5);
}

FormManager::~FormManager()
{
}
