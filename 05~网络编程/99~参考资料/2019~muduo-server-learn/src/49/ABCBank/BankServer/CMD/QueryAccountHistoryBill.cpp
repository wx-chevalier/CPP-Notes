#include "QueryAccountHistoryBill.h"

#include "../DAL/BankService.h"

#include "../../Public/Logging.h"
#include "../../Public/JUtil.h"
#include "../../Public/JInStream.h"
#include "../../Public/JOutStream.h"

using namespace PUBLIC;
using namespace CMD;
using namespace DAL;

void QueryAccountHistoryBill::Execute(BankSession& session)
{
	JInStream jis(session.GetRequestPack()->buf, session.GetRequestPack()->head.len);
	uint16 cmd = session.GetCmd();

	// ҳ��
	uint32 page;
	jis>>page;
	// �ʺ�id
	char account_id[7] = {0};
	jis.ReadBytes(account_id, 6);
	// ��ʼ����
	char begin_date[11] = {0};
	jis.ReadBytes(begin_date, 10);
	// ��������
	char end_date[11] = {0};
	jis.ReadBytes(end_date, 10);

	// ����Ϊ��ѯ����
	BankService dao;
	int16 error_code = 0;
	char error_msg[31] = {0};

	list<TransDetail> details;
	int ret = dao.QueryAccountHistoryBill(details, page, account_id, begin_date, end_date);
	if (ret == 6)
	{
		error_code = 6;
		strcpy_s(error_msg, "û���ҵ�����");
		LOG_INFO<<error_msg;
	}
	else if (ret == -1)
	{
		error_code = -1;
		strcpy_s(error_msg, "���ݿ����");
	}

	if (error_code == 0)
	{
		LOG_INFO<<"��ѯ�ɹ�";
		uint16 cnt = static_cast<uint16>(details.size());
		uint16 seq = 0;
		list<TransDetail>::const_iterator it;
		for (it=details.begin(); it!=details.end(); ++it)
		{
			JOutStream jos;
			// ��ͷcmd+len+cnt+seq+error_code+error_msg
			jos<<cmd;
			size_t lengthPos = jos.Length();	// lenλ��
			jos.Skip(2);						// ΪlenԤ�������ֽ�
			jos<<cnt<<seq<<error_code;
			seq++;
			jos.WriteBytes(error_msg, 30);

			// ����date+account_id+other_account_id+money+abstract_name+balance+total
			jos.WriteBytes(it->trans_date.c_str(), 19);
			jos.WriteBytes(it->account_id.c_str(), 6);
			jos.WriteBytes(it->other_account_id.c_str(), 6);
			string money = Convert::DoubleToString(it->money);
			jos<<money;
			jos<<it->abstract_name;
			string balance = Convert::DoubleToString(it->balance);
			jos<<balance;
			jos<<it->total;

			// ��ͷlen�ֶ�
			size_t tailPos = jos.Length();
			jos.Reposition(lengthPos);
			jos<<(uint16)(tailPos + 8 - sizeof(ResponseHead));	// ����+��β����

			// ��β
			jos.Reposition(tailPos);
			unsigned char hash[16];
			MD5 md5;
			md5.MD5Make(hash, (unsigned char const*)jos.Data(), jos.Length());
			for (int i=0; i<8; ++i)
			{
				hash[i] = hash[i] ^ hash[i+8];
				hash[i] = hash[i] ^ ((cmd >> (i%2)) & 0xff);
			}
			jos.WriteBytes(hash, 8);
			session.Send(jos.Data(), jos.Length());

		}
	}
	else
	{
		JOutStream jos;
		// ��ͷcmd+len+cnt+seq+error_code+error_msg
		uint16 cnt = 0;
		uint16 seq = 0;
		jos<<cmd;
		size_t lengthPos = jos.Length();	// lenλ��
		jos.Skip(2);						// ΪlenԤ�������ֽ�
		jos<<cnt<<seq<<error_code;
		jos.WriteBytes(error_msg, 30);

		// ����

		// ��ͷlen�ֶ�
		size_t tailPos = jos.Length();
		jos.Reposition(lengthPos);
		jos<<(uint16)(tailPos + 8 - sizeof(ResponseHead));	// ����+��β����

		// ��β
		jos.Reposition(tailPos);
		unsigned char hash[16];
		MD5 md5;
		md5.MD5Make(hash, (unsigned char const*)jos.Data(), jos.Length());
		for (int i=0; i<8; ++i)
		{
			hash[i] = hash[i] ^ hash[i+8];
			hash[i] = hash[i] ^ ((cmd >> (i%2)) & 0xff);
		}
		jos.WriteBytes(hash, 8);
		session.Send(jos.Data(), jos.Length());
	}
}


