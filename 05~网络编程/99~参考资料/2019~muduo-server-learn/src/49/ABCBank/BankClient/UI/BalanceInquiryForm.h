#ifndef _BALANCE_INQUIRY_FORM_H_
#define _BALANCE_INQUIRY_FORM_H_

#include "../JFC/JForm.h"
#include "../JFC/JLabel.h"
#include "../JFC/JEdit.h"
#include "../JFC/JButton.h"

#include <string>

using namespace JFC;

namespace UI
{

class BalanceInquiryForm : public JForm
{
public:
	BalanceInquiryForm();
	~BalanceInquiryForm();
	BalanceInquiryForm(SHORT x, SHORT y, SHORT w, SHORT h,
		const std::string& title);

	virtual void Draw();
	virtual void OnKeyEvent(JEvent* e);

private:
	void DrawBorder();

	void Reset();
	void Submit();

	std::string title_;

	JLabel* lblAccountId_;
	JEdit* editAccountId_;
	JLabel* lblAccountIdTip_;

	JLabel* lblPass_;
	JEdit* editPass_;
	JLabel* lblPassTip_;

	JButton* btnSubmit_;
	JButton* btnReset_;
	JButton* btnCancel_;
};

}
#endif // _BALANCE_INQUIRY_FORM_H_
