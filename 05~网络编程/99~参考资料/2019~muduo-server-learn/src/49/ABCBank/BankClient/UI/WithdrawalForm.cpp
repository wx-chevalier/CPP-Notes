#include "WithdrawalForm.h"

#include "FormManager.h"
#include "MainMenuForm.h"
#include "ReceiptForm.h"
#include "Validator.h"

#include "../BankSession.h"
#include "../TransactionManager.h"

#include "../JFC/JMessageBox.h"

#include "../../Public/Exception.h"

using namespace UI;

WithdrawalForm::WithdrawalForm()
	: JForm(0, 0, 0, 0, 0)
{
}

WithdrawalForm::~WithdrawalForm()
{
}

WithdrawalForm::WithdrawalForm(SHORT x, SHORT y, SHORT w, SHORT h,
							   const std::string& title)
	: JForm(x, y, w, h),
	  title_(title)
{

	lblAccountId_ = new JLabel(14, 5, 11, 1, "ACCOUNT ID:", this);
	editAccountId_ = new JEdit(26, 5, 21, 1, "", this);
	lblAccountIdTip_ = new JLabel(50, 5, 11, 1, "����6λ������", this);

	lblPass_ = new JLabel(8, 8, 17, 1, "ACCOUNT PASSWORD:", this);
	editPass_ = new JEdit(26, 8, 21, 1, "", this, JEdit::EM_PASSWORD);
	lblPassTip_ = new JLabel(50, 8, 11, 1, "����6-8λ", this);

	lblMoney_ = new JLabel(19, 11, 12, 1, "MONEY:", this);
	editMoney_ = new JEdit(26, 11, 21, 1, "", this);
	lblMoneyTip_ = new JLabel(50, 11, 15, 1, "С�������λ", this);


	btnSubmit_ = new JButton(5, 20, 10, 3, "SUBMIT", this);
	btnReset_ =	new JButton(50, 20, 10, 3,"RESET", this);
	btnCancel_ = new JButton(65, 20, 10, 3,"CANCEL", this);

	editAccountId_->SetValidator(ValidateAccountId);
	editPass_->SetValidator(ValidatePass);
	editMoney_->SetValidator(ValidateMoney);


}

void WithdrawalForm::Draw()
{
	DrawBorder();
	SetTextColor(FCOLOR_BLUE);
	SetBkColor(BCOLOR_WHITE);
	JRECT rect = { 1, 1, Width()-2, Height()-2 };
	FillRect(rect);

	DrawText(5, 2, title_);
	DrawHLine(3, 2, Width()-3, '-');

	JForm::Draw();
}

void WithdrawalForm::DrawBorder()
{
	SetTextColor(FCOLOR_YELLO);
	SetBkColor(BCOLOR_RED);
	DrawHLine(0, 0, Width()-1, '-');
	DrawHLine(Height()-1, 0, Width()-1, '-');
	DrawVLine(0, 1, Height()-2, ' ');
	DrawVLine(Width()-1, 1, Height()-2, ' ');
}

void WithdrawalForm::OnKeyEvent(JEvent* e)
{
	int key = e->GetEventCode();
	if (key == KEY_ESC)
	{
		Reset();
		JForm* form;
		//ClearWindow();
		form = Singleton<FormManager>::Instance().Get("MainMenuForm");
		dynamic_cast<MainMenuForm*>(form)->GetItems()[2]->SetCurrent();
		form->ClearWindow();
		form->Show();
		e->Done();
	}
	else if (key == KEY_ENTER)
	{
		if (e->GetSender() == btnCancel_)
		{
			Reset();
			JForm* form;
			//ClearWindow();
			form = Singleton<FormManager>::Instance().Get("MainMenuForm");
			dynamic_cast<MainMenuForm*>(form)->GetItems()[2]->SetCurrent();
			form->ClearWindow();
			form->Show();
			e->Done();
		}
		else if (e->GetSender() == btnReset_)
		{
			Reset();
			editAccountId_->SetCurrent();
			Show();
			e->Done();
		}
		else if (e->GetSender() == btnSubmit_)
		{
			Submit();
			e->Done();
		}
	}

	JForm::OnKeyEvent(e);
}

void WithdrawalForm::Reset()
{
	editAccountId_->SetText("");
	editPass_->SetText("");
	editMoney_->SetText("");

}

void WithdrawalForm::Submit()
{
	if (editAccountId_->GetText().length() < 6)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "�ʺ�С��6λ";

		int result = JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editAccountId_->Show();
		return;
	}
	if (editPass_->GetText().length() < 6)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "����С��6λ";

		int result = JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editPass_->Show();
		return;
	}
	if (editMoney_->GetText().empty())
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "����Ϊ��";

		int result = JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editPass_->Show();
		return;
	}

	// ����Ϊʵ�ʵ�ȡ�����
	try
	{
		BankSession bs;
		bs.SetCmd(CMD_WITHDRAW);
		bs.SetAttribute("account_id", editAccountId_->GetText());
		bs.SetAttribute("pass", editPass_->GetText());
		bs.SetAttribute("money", editMoney_->GetText());

		Singleton<TransactionManager>::Instance().DoAction(bs);
		if (bs.GetErrorCode() == 0)
		{
			Reset();
			std::vector<std::string> v;
			v.push_back(" YES ");
			std::string msg = "ȡ��ɹ�"+bs.GetResponse("money");

			/*int result = JMessageBox::Show("-MESSAGE-", msg,v);
			ClearWindow();
			Show();*/

			ReceiptForm* form;
			form = dynamic_cast<ReceiptForm*>(Singleton<FormManager>::Instance().Get("ReceiptForm"));
			form->SetReceiptFormType(ReceiptForm::RFT_WITHDRAW);
			form->SetTitle("ȡ��ɹ�");

			form->SetItemText("��������", bs.GetResponse("trans_date"));
			form->SetItemText("��    ��", bs.GetResponse("name"));
			form->SetItemText("��    ��", bs.GetAttribute("account_id"));
			form->SetItemText("���׽��", bs.GetAttribute("money"));
			form->SetItemText("ժ    Ҫ", "ȡ��");
			form->SetItemText("��    ��", bs.GetResponse("balance"));

			form->Show();
		}
		else
		{
			std::vector<std::string> v;
			v.push_back(" YES ");

			JMessageBox::Show("-ERROR-", bs.GetErrorMsg(), v);
			ClearWindow();
			Show();
			return;
		}
	}

	catch (Exception& e)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");

		int result = JMessageBox::Show("-ERROR-", e.what(), v);
		ClearWindow();
		Show();

		return;
	}
}