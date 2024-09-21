#ifndef _JAPPLICATION_H_
#define _JAPPLICATION_H_

namespace JFC
{

class JWindow;

// һ������ĵ���ģʽ��ģ��QT
class JApplication
{
public:
	JApplication();
	~JApplication();

	static JApplication* Instance() { return self_; }

	void ShowCursor() const;
	void HideCursor() const;

	JWindow* GetCurrent() const { return current_; }
	void SetCurrent(JWindow* win) { current_ = win; }
	JWindow* Root() const { return root_; }

	int Exec();				// ��Ϣѭ��

private:
	static JApplication* self_;
	JWindow* current_;		// ��ǰ���հ����Ĵ���
	JWindow* root_;			// ��
};

#define jApp JApplication::Instance()
}
#endif // _JAPPLICATION_H_