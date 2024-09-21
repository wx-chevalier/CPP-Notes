#include "CloseAccount.h"

#include "../../Public/Logging.h"
#include "../../Public/JUtil.h"

using namespace PUBLIC;
using namespace CMD;

void CloseAccount::Execute(BankSession& session)
{
	JOutStream jos;
	// ��ͷ����
	uint16 cmd = CMD_CLOSE_ACCOUNT;
	jos<<cmd;
	// Ԥ�������ֽڰ�ͷlen(����+��β����)
	size_t lengthPos = jos.Length();
	jos.Skip(2);
	// �ʺ�
	string account_id = session.GetAttribute("account_id");
	jos.WriteBytes(account_id.c_str(), 6);

	// �ʺ�����
	string pass = session.GetAttribute("pass");
	unsigned char ideaKey[16];
	unsigned char buf[2];
	buf[0] = (cmd >> 8) & 0xff;
	buf[1] = cmd & 0xff;
	MD5 md5;
	md5.MD5Make(ideaKey, buf, 2);
	for (int i=0; i<8; ++i)
	{
		ideaKey[i] = ideaKey[i] ^ ideaKey[i+8];
		ideaKey[i] = ideaKey[i] ^ ((cmd >> (i%2)) & 0xff);
		ideaKey[i+8] = ideaKey[i] ^ ideaKey[i+8];
		ideaKey[i+8] = ideaKey[i+8] ^ ((cmd >> (i%2)) & 0xff);
	}
	char encryptedPass[16];
	Idea idea;
	idea.Crypt(ideaKey,(const unsigned char*)pass.c_str(), (unsigned char *)encryptedPass, 16, true);	// ����
	jos.WriteBytes(encryptedPass, 16);

	// ��ͷlen�ֶ�
	size_t tailPos = jos.Length();
	jos.Reposition(lengthPos);
	jos<<(uint16)(tailPos + 8 - sizeof(RequestHead));

	// ��β
	jos.Reposition(tailPos);
	unsigned char hash[16];
	md5.MD5Make(hash, (unsigned char const*)jos.Data(), jos.Length());
	for (int i=0; i<8; ++i)
	{
		hash[i] = hash[i] ^ hash[i+8];
		hash[i] = hash[i] ^ ((cmd >> (i%2)) & 0xff);
	}
	jos.WriteBytes(hash, 8);

	session.Send(jos.Data(), jos.Length());	// ���������
	session.Recv();	// ����Ӧ���
	JInStream jis((const char*)session.GetResponsePack(), session.GetResponsePack()->head.len+sizeof(ResponseHead));
	// ����cmd��len
	jis.Skip(4);
	uint16 cnt;
	uint16 seq;
	int16 error_code;
	jis>>cnt>>seq>>error_code;

	char error_msg[31];
	jis.ReadBytes(error_msg, 30);

	session.SetErrorCode(error_code);
	session.SetErrorMsg(error_msg);
	if (error_code == 0)
	{
		string name;
		string balance;
		string interest;
		char close_date[20] = {0};

		jis>>name>>balance>>interest;
		jis.ReadBytes(close_date, 19);
		session.SetResponse("name", name);
		session.SetResponse("balance", balance);
		session.SetResponse("interest", interest);
		session.SetResponse("close_date", close_date);


	}
}

