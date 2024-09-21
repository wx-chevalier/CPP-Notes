#include "JMessageBox.h"
#include "JForm.h"
#include "JButton.h"
#include "JApplication.h"
#include <conio.h>

using namespace JFC;

int JMessageBox::Show(const std::string& title,
					  const std::string& text,
					  std::vector<std::string>& buttons)
{
	short btn_distance = static_cast<short>(buttons.size()) - 1;

	for (int i=0; i<static_cast<int>(buttons.size()); ++i)
		btn_distance += buttons[i].length() + 4;		// ��ť���=�ı�����+4

	short width = 4 + btn_distance;		// ��С��߾�=2����С�ұ߾�=2
	//short width = 4 + static_cast<short>(buttons.size()) - 1;	// ��߾�2 + �ұ߾�2 + ��ť��ࣨ1*n)

	if (width < 40)
		width = 40;			// MessageBox��С���Ϊ40

	// ÿ���ַ�����width-6;
	// ����ó��ı�����
	short lines = (text.length() + width-6 -1) / (width-6);
	//short lines =
	//	(text.length() % (width-6)) ? (text.length() / (width-6)+1) : (text.length() / (width-6));

	short height = lines + 9;

	short x = (80 - width) / 2;
	short y = (25 - height) / 2;

	JMessageBoxImpl jmbl(x, y, width, height, title, text, buttons, btn_distance, lines);
	JWindow* w = jApp->GetCurrent();

	JRECT rect = {x, y, x+width-1, y+height-1};
	ScreenSaver sc(rect);
	sc.Dump();		// ���汻��Ϣ���ڸǲ��ֵ���Ļ����
	jmbl.Show();
	int ret = jmbl.Exec();
	sc.Restore();	// ���������Ļ���ݻ�ԭ
	w->SetCurrent();
	return ret;
}

JMessageBoxImpl::JMessageBoxImpl(SHORT x, SHORT y, SHORT w, SHORT h,
								 const std::string& title, const std::string& text,
								 std::vector<std::string>& buttons, SHORT btnDistance,
								 int lines, JWindow* parent)
	: JWindow(x, y, w, h),
	  title_(title),
	  text_(text),
	  btnDistance_(btnDistance),
	  lines_(lines)
{
	short btnX = x + (width_ - btnDistance_) / 2;
	short btnY = y + h - 5;

	for (int i=0; i<static_cast<int>(buttons.size()); ++i)
	{
		JButton* btn = new JButton(btnX, btnY, buttons[i].length()+4, 3, buttons[i], this);
		btnX += buttons[i].length()+4+1;
	}

}

void JMessageBoxImpl::Draw()
{
	ClearWindow();
	
	SetTextColor(FCOLOR_YELLO);
	SetBkColor(BCOLOR_MAGENTA);
	DrawHLine(0, 0, Width()-1, '-');
	DrawHLine(Height()-1, 0, Width()-1, '-');
	DrawVLine(0, 1, Height()-2, ' ');
	DrawVLine(Width()-1, 1, Height()-2, ' ');


	SetBkColor(BCOLOR_CYAN);
	JRECT rect = { 1, 1, Width()-2, Height()-2 };
	FillRect(rect);

	DrawText(2, 1, title_);
	if (lines_ == 1)
		DrawText(width_/2-text_.length()/2, 3, text_);
	else
	{
		for (int i=0; i<lines_; ++i)
		{
			std::string s;
			if (i == lines_ - 1)
				s = text_.substr(i*(width_-6));
			else
				s = text_.substr(i*(width_-6), width_-6);
			DrawText(3, 3+i, s);
		}
	}

	std::vector<JWindow*>::iterator it;
	for (it = childs_.begin(); it !=childs_.end(); ++it)
	{
		(*it)->Draw();
	}
}

int JMessageBoxImpl::Exec()
{
	int pos = 0;
	while (1)
	{
		int key = _getch();
		if(key == 224)
		{
			key = (key << 8) + _getch();
		}

		JWindow* p;
		JWindow* w = jApp->GetCurrent();
		switch (key)
		{
		case KEY_ESC:
			return -1;
		case KEY_ENTER:
			return pos;
		case KEY_LEFT:
		case KEY_UP:
			p = FindPrev(jApp->GetCurrent());
			p->SetCurrent();
			p->Draw();
			p->Refresh();

			w->Draw();
			w->Refresh();
			pos--;
			//pos += childs_.size();
			//pos %= childs_.size();
			if (pos < 0)
				pos = childs_.size() - 1;
			break;
		case KEY_RIGHT:
		case KEY_TAB:
		case KEY_DOWN:
			p = FindNext(jApp->GetCurrent());
			p->SetCurrent();
			p->Draw();
			p->Refresh();

			w->Draw();
			w->Refresh();
			pos++;
			//pos += childs_.size();
			//pos %= childs_.size();
			if (pos == childs_.size())
				pos = 0;
		default:
			break;
		}
	}
}
