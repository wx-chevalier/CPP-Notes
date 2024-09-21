#include "OpenAccountForm.h"

#include "FormManager.h"
#include "MainMenuForm.h"
#include "ReceiptForm.h"
#include "Validator.h"

#include "../BankSession.h"
#include "../TransactionManager.h"

#include "../JFC/JMessageBox.h"

#include "../../Public/Exception.h"

using namespace UI;

OpenAccountForm::OpenAccountForm()
	: JForm(0, 0, 0, 0, 0)
{
}

OpenAccountForm::~OpenAccountForm()
{
}

OpenAccountForm::OpenAccountForm(SHORT x, SHORT y, SHORT w, SHORT h,
								 const std::string& title)
	: JForm(x, y, w, h),
	  title_(title)
{
	lblName_ = new JLabel(11, 5, 13, 1, "CUSTOMER NAME:", this);
	editName_ = new JEdit(26, 5, 21, 1, "", this);
	lblNameTip_ = new JLabel(50, 5, 21, 1, "����3-10λ����ĸ������", this);

	lblId_ = new JLabel(13, 8, 12, 1, "IDENTIFY ID:", this);
	editId_ = new JEdit(26, 8, 21, 1, "", this);
	lblIdTip_ = new JLabel(50, 8, 12, 1, "����18λ", this);

	lblPass_ = new JLabel(8, 11, 17, 1, "ACCOUNT PASSWORD:", this);
	editPass_ = new JEdit(26, 11, 21, 1, "", this, JEdit::EM_PASSWORD);
	lblPassTip_ = new JLabel(50, 11, 11, 1, "����6-8λ", this);

	lblPass2_ = new JLabel(9, 14, 16, 1, "REPEAT PASSWORD:", this);
	editPass2_ = new JEdit(26, 14, 21, 1, "", this, JEdit::EM_PASSWORD);
	lblPass2Tip_ = new JLabel(50, 14, 16, 1, "ͬ��", this);

	lblMoney_ = new JLabel(19, 17, 16, 1, "MONEY:", this);
	editMoney_ = new JEdit(26, 17, 21, 1, "", this);
	lblMoneyTip_ = new JLabel(50, 17, 19, 1, "С�������λ", this);


	btnSubmit_ = new JButton(5, 20, 10, 3, "SUBMIT", this);
	btnReset_ =	new JButton(50, 20, 10, 3,"RESET", this);
	btnCancel_ = new JButton(65, 20, 10, 3,"CANCEL", this);

	editName_->SetValidator(ValidateName);
	editId_->SetValidator(ValidateId);
	editPass_->SetValidator(ValidatePass);
	editPass2_->SetValidator(ValidatePass);
	editMoney_->SetValidator(ValidateMoney);
}

void OpenAccountForm::Draw()
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

void OpenAccountForm::DrawBorder()
{
	SetTextColor(FCOLOR_YELLO);
	SetBkColor(BCOLOR_RED);
	DrawHLine(0, 0, Width()-1, '-');
	DrawHLine(Height()-1, 0, Width()-1, '-');
	DrawVLine(0, 1, Height()-2, ' ');
	DrawVLine(Width()-1, 1, Height()-2, ' ');
}

void OpenAccountForm::OnKeyEvent(JEvent* e)
{
	int key = e->GetEventCode();
	if (key == KEY_ESC)
	{
		Reset();
		JForm* form;
		form = Singleton<FormManager>::Instance().Get("MainMenuForm");
		dynamic_cast<MainMenuForm*>(form)->GetItems()[0]->SetCurrent();
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
			form = Singleton<FormManager>::Instance().Get("MainMenuForm");
			dynamic_cast<MainMenuForm*>(form)->GetItems()[0]->SetCurrent();
			form->ClearWindow();
			form->Show();
			e->Done();
		}
		else if (e->GetSender() == btnReset_)
		{
			Reset();
			editName_->SetCurrent();
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

void OpenAccountForm::Reset()
{
	editName_->SetText("");
	editId_->SetText("");
	editPass_->SetText("");
	editPass2_->SetText("");
	editMoney_->SetText("");
}

void OpenAccountForm::Submit()
{
	if (editName_->GetText().length() < 3)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "����С��3λ";

		JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editName_->Show();
		return;
	}
	if (editId_->GetText().length() != 18)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "���֤�ű�����18λ";

		JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editId_->Show();
		return;
	}
	if (editPass_->GetText().length() < 6)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "����С��6λ";

		JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editPass_->Show();
		return;
	}
	if (editPass2_->GetText().length() < 6)
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "����С��6λ";

		JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editPass2_->Show();
		return;
	}

	if (editPass_->GetText() != editPass2_->GetText())
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "ȷ�����벻һ��";

		JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editPass2_->Show();
		return;
	}
	if (editMoney_->GetText().empty())
	{
		std::vector<std::string> v;
		v.push_back(" YES ");
		std::string msg = "����Ϊ��";

		JMessageBox::Show("-ERROR-", msg,v);
		ClearWindow();
		Show();
		editMoney_->Show();
		return;
	}

	// ����Ϊʵ�ʵĿ�������
	try
	{
		BankSession bs;
		bs.SetCmd(CMD_OPEN_ACCOUNT);
		bs.SetAttribute("name", editName_->GetText());
		bs.SetAttribute("pass", editPass_->GetText());
		bs.SetAttribute("id", editId_->GetText());
		bs.SetAttribute("money", editMoney_->GetText());

		Singleton<TransactionManager>::Instance().DoAction(bs);
		if (bs.GetErrorCode() == 0)
		{
			Reset();

			ReceiptForm* form;
			form = dynamic_cast<ReceiptForm*>(Singleton<FormManager>::Instance().Get("OpenAccountReceiptForm"));
			form->SetReceiptFormType(ReceiptForm::RFT_OPEN_ACCOUNT);
			form->SetItemText("��������", bs.GetResponse("open_date"));
			form->SetItemText("��    ��", bs.GetAttribute("name"));
			form->SetItemText("��    ��", bs.GetResponse("account_id"));
			form->SetItemText("��    ��", bs.GetAttribute("money"));
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

		JMessageBox::Show("-ERROR-", e.what(), v);
		ClearWindow();
		Show();

		return;
	}
}