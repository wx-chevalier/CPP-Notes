#include "OpenAccount.h"

#include "../DAL/BankService.h"

#include "../../Public/Logging.h"
#include "../../Public/JUtil.h"
#include "../../Public/JInStream.h"
#include "../../Public/JOutStream.h"

using namespace PUBLIC;
using namespace CMD;
using namespace DAL;

void OpenAccount::Execute(BankSession& session)
{
	JInStream jis(session.GetRequestPack()->buf, session.GetRequestPack()->head.len);
	uint16 cmd = session.GetCmd();

	// �ͻ�����
	string name;
	jis>>name;
	// ����
	char pass[16];
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
	jis.ReadBytes(encryptedPass, 16);
	Idea idea;
	// ����
	idea.Crypt(ideaKey, (const unsigned char*)encryptedPass, (unsigned char *)pass, 16, false);

	// ���֤����
	char id[19] = {0};
	jis.ReadBytes(id, 18);

	// �������
	string money;
	jis>>money;

	int16 error_code = 0;
	char error_msg[31] = {0};


	// ʵ�ʵĿ�������
	BankService dao;
	int ret;

	Account acc;
	acc.id = id;
	acc.name = name;
	acc.pass = pass;
	acc.balance = Convert::StringToDouble(money);
	ret = dao.OpenAccount(acc);
	if (ret == 0)
	{
		LOG_INFO<<"�����ɹ�";
	}
	else if (ret == 1)
	{
		error_code = 1;
		strcpy_s(error_msg, "�û������������");
		LOG_INFO<<error_msg;
	}
	else if (ret == -1)
	{
		error_code = -1;
		strcpy_s(error_msg, "���ݿ����");
	}

	JOutStream jos;
	// ��ͷ����
	jos<<cmd;
	size_t lengthPos = jos.Length();
	jos.Skip(2);
	// ��ͷcnt��seq��error_code��error_msg
	uint16 cnt = 0;
	uint16 seq = 0;
	jos<<cnt<<seq<<error_code;
	jos.WriteBytes(error_msg, 30);

	// ����account_id + date
	stringstream ss;
	ss<<setw(6)<<setfill('0')<<acc.account_id;
	jos.WriteBytes(ss.str().c_str(), 6);
	jos<<acc.op_date;

	// ��ͷlen
	size_t tailPos = jos.Length();
	jos.Reposition(lengthPos);
	jos<<static_cast<uint16>(tailPos + 8 - sizeof(ResponseHead)); // ���峤�� + ��β����

	// ��β
	jos.Reposition(tailPos);
	// �����β
	unsigned char hash[16];
	md5.MD5Make(hash, (const unsigned char*)jos.Data(), jos.Length());
	for (int i=0; i<8; ++i)
	{
		hash[i] = hash[i] ^ hash[i+8];
		hash[i] = hash[i] ^ ((cmd >> (i%2)) & 0xff);
	}
	jos.WriteBytes(hash, 8);

	session.Send(jos.Data(), jos.Length());
}


