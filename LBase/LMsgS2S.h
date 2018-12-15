#ifndef _LMSG_S2S_H_
#define _LMSG_S2S_H_

#include "LMsg.h"
#include "LUser.h"
#include "LCharge.h"
#include "GameDefine.h"
#include "TCPClient.h"


//////////////////////////////////////////////////////////////////////////
//logic �� center 

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

struct LogicInfo
{
	Lint		m_id;
	Lint		m_deskCount;
	Lstring		m_ip;
	Lshort		m_port;
	LSocketPtr	m_sp;
	TCPClientPtr	m_client;
	Lint		m_flag;
	Lint		m_exclusive_play;	//���������淨, 0:��ʾû��,���ڰ�ĳ���淨�鵽������,�������淨����ʱ����(�Уߩ�)

public:
	LogicInfo()
		:m_id(0)
		,m_deskCount(0)
		,m_port(0)
		,m_flag(0)
		,m_exclusive_play(0)
	{

	}
};



struct LMsgLMG2GateLogicInfo :public LMsg
{
	Lint		m_ID;
	Lint		m_count;
	LogicInfo	m_logic[500];

	LMsgLMG2GateLogicInfo() :LMsg(MSG_LMG_2_G_SYNC_LOGIC)
		,m_ID(0)
		,m_count(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_ID);
		buff.Read(m_count);
		for (Lint i = 0; i < m_count && i < 500; ++i)
		{
			buff.Read(m_logic[i].m_id);
			buff.Read(m_logic[i].m_deskCount);
			buff.Read(m_logic[i].m_ip);
			buff.Read(m_logic[i].m_port);
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_ID);
		buff.Write(m_count);
		for (Lint i = 0; i < m_count && i < 500; ++i)
		{
			buff.Write(m_logic[i].m_id);
			buff.Write(m_logic[i].m_deskCount);
			buff.Write(m_logic[i].m_ip);
			buff.Write(m_logic[i].m_port);
		} 
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLMG2GateLogicInfo();
	}
};






struct GateInfo
{
	Lint		m_id;
	Lint		m_userCount;
	Lstring		m_ip;
	Lshort		m_port;
	LSocketPtr	m_sp;

public:
	GateInfo()
		:m_id(0)
		,m_userCount(0)
		,m_port(0)
	{

	}
};



//////////////////////////////////////////////////////////////////////////
//center�������id��Ϣ��

//��һ�����Ϣ
struct UserIdInfo
{
	Lint		m_id;
	Lstring		m_openId;
	Lstring		m_nike;
	Lstring		m_headImageUrl;
	Lint		m_sex;
	Lint		m_blockEndTime;

public:
	UserIdInfo()
		:m_id(0)
		,m_sex(0)
		,m_blockEndTime(0)
	{

	}
};



struct Season
{
	Lint            m_level; //�û���ȡ�Ĳ���ȯ����
	Lint            m_time; //�û���ȡ����ȯʱ��
	Lint            m_playState; //�淨
	string          m_playType; //�淨ѡ��
};














//////////////////////////////////////////////////////////////////////////
//gate ���������Ϣ �� logicserver
struct LMsgG2LUserMsg :public LMsg
{
	Lint			m_userId;
	Lint			m_userMsgId;
	Lstring			m_ip;
	Lint            m_userDataID; //�û����ݿ�id,���ڱ������й��߼�����
	LBuffPtr		m_dataBuff;
	LMsg*			m_userMsg;

	LMsgG2LUserMsg() :LMsg(MSG_G_2_L_USER_MSG), m_userMsg(NULL)
		, m_userId(0)
		,m_userMsgId(0)
		, m_userDataID(0)
	{
	}

	virtual~LMsgG2LUserMsg()
	{
		if (m_userMsg)
			delete m_userMsg;
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_userId);
		buff.Read(m_userMsgId);
		buff.Read(m_ip);
		buff.Read(m_userDataID);

		int msgid = MSG_ERROR_MSG;

		try{
			msgpack::unpacked  unpack;
			msgpack::unpack(&unpack, buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
			msgpack::object obj = unpack.get();

			ReadMapData(obj, "m_msgId", msgid);
			m_userMsg = LMsgFactory::Instance().CreateMsg(m_userMsgId);
			if (m_userMsg)
			{
				m_userMsg->Read(obj);
			}
			else
			{
				LLOG_ERROR("LMsgG2LUserMsg read msgId not exiest %d", m_userMsgId);
			}
		}
		catch (...)
		{
			LLOG_ERROR("MSG_G_2_L_USER_MSG::RecvMsgPack error");
		}
		
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_userId);
		buff.Write(m_userMsgId);
		buff.Write(m_ip);
		buff.Write(m_userDataID);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2LUserMsg();
	}
};


//////////////////////////////////////////////////////////////////////////
//logic ���������Ϣ�� gate
struct LMsgL2GUserMsg : public LMsg
{
	Lint			m_user_id;
	Lint			m_user_msg_id;
	LBuffPtr		m_dataBuff;
	
	LMsgL2GUserMsg() :LMsg(MSG_L_2_G_USER_MSG)
		, m_user_id(0),m_user_msg_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);
		buff.Read(m_user_msg_id);
		m_dataBuff = LBuffPtr(new LBuff);
		m_dataBuff->Write(buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);
		buff.Write(m_user_msg_id);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2GUserMsg();
	}
};

//////////////////////////////////////////////////////////////////////////
//gate ���������Ϣ �� match
//struct LMsgG2MUserMsg :public LMsg
//{
//	/*
//	�����ֶ���gate�������
//	*/
//	Llong			m_userGateId;  //user��gateid
//	Lstring			m_ip;
//	LBuffPtr		m_dataBuff;
//
//	/*
//	�����ֶ���read�н���
//	*/
//	Lint			m_userMsgId;
//	LMsg*			m_userMsg;
//
//	LMsgG2MUserMsg() :LMsg(MSG_G_2_M_USER_MSG)
//	{
//		m_userMsgId = 0;
//		m_userMsg = NULL;
//		m_userGateId = 0;
//	}
//
//	virtual~LMsgG2MUserMsg()
//	{
//		if (m_userMsg)
//			delete m_userMsg;
//	}
//
//	virtual bool Read(LBuff& buff)
//	{
//		buff.Read(m_userGateId);
//		buff.Read(m_userMsgId);
//		buff.Read(m_ip);
//
//		int msgid = MSG_ERROR_MSG;
//		try
//		{
//			msgpack::unpacked  unpack;
//			msgpack::unpack(&unpack, buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
//			msgpack::object obj = unpack.get();
//
//			ReadMapData(obj, "m_msgId", msgid);
//			m_userMsg = LMsgFactory::Instance().CreateMsg(m_userMsgId);
//			if (m_userMsg)
//			{
//				m_userMsg->Read(obj);
//			}
//			else
//			{
//				LLOG_ERROR("LMsgG2LUserMsg read msgId not exiest %d", m_userMsgId);
//			}
//		}
//		catch (...)
//		{
//			LLOG_ERROR("MSG_G_2_L_USER_MSG::RecvMsgPack error");
//		}
//
//		return true;
//	}
//
//	virtual bool Write(LBuff& buff)
//	{
//		buff.Write(m_userGateId);
//		buff.Write(m_userMsgId);
//		buff.Write(m_ip);
//		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
//		return true;
//	}
//
//	virtual LMsg* Clone()
//	{
//		return new LMsgG2MUserMsg();
//	}
//};



//////////////////////////////////////////////////////////////////////////
//gate ������ҵ�����Ϣ �� logic
struct LMsgG2LUserOutMsg :public LMsg
{
	Llong			m_userGateId;
	
	LMsgG2LUserOutMsg():LMsg(MSG_G_2_L_USER_OUT_MSG)
		,m_userGateId(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_userGateId);

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_userGateId);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2LUserOutMsg();
	}
};


//////////////////////////////////////////////////////////////////////////
//logic ���������Ϣ�� gate
struct LMsgL2GUserOutMsg : public LMsg
{
	Llong			m_id;

	LMsgL2GUserOutMsg():LMsg(MSG_L_2_G_USER_OUT_MSG)
		,m_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_id);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_id);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2GUserOutMsg();
	}
};






struct LMsgS2SKeepAlive : public LMsg
{
	LMsgS2SKeepAlive() : LMsg(MSG_S_2_S_KEEP_ALIVE) {}

	virtual bool Read(LBuff& buff)
	{
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2SKeepAlive();
	}
};

struct LMsgG2LLogin :public LMsg
{
	Lint		m_id;
	Lstring		m_key;
	Lstring		m_ip;
	Lshort		m_port;

	LMsgG2LLogin() :LMsg(MSG_G_2_L_LOGIN)
		, m_id(0)
		, m_port(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_id);
		buff.Read(m_key);
		buff.Read(m_ip);
		buff.Read(m_port);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_id);
		buff.Write(m_key);
		buff.Write(m_ip);
		buff.Write(m_port);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2LLogin();
	}
};

struct LMsgL2LMGLogin :public LMsg
{
	Lint		m_id;
	Lstring		m_key;
	Lstring		m_ip;
	Lshort		m_port;
	Lint		m_exclusive_play;	//��������ר���淨

	LMsgL2LMGLogin() :LMsg(MSG_L_2_LMG_LOGIN)
		, m_id(0)
		, m_port(0)
		, m_exclusive_play(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_id);
		buff.Read(m_key);
		buff.Read(m_ip);
		buff.Read(m_port);
		buff.Read(m_exclusive_play);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_id);
		buff.Write(m_key);
		buff.Write(m_ip);
		buff.Write(m_port);
		buff.Write(m_exclusive_play);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2LMGLogin();
	}
};

struct LMsgG2ServerLogin :public LMsg
{
	Lint		m_id;
	Lstring		m_key;
	Lstring		m_ip;
	Lshort		m_port;

	LMsgG2ServerLogin() :LMsg(MSG_G_2_SERVER_LOGIN)
		, m_id(0)
		, m_port(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_id);
		buff.Read(m_key);
		buff.Read(m_ip);
		buff.Read(m_port);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_id);
		buff.Write(m_key);
		buff.Write(m_ip);
		buff.Write(m_port);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2ServerLogin();
	}
};

struct LMsgS2SKeepAliveAck : public LMsg
{
	LMsgS2SKeepAliveAck() : LMsg(MSG_S_2_S_KEEP_ALIVE_ACK) {}

	virtual bool Read(LBuff& buff)
	{
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgS2SKeepAliveAck();
	}
};


//�ָ���

struct LMsgGame2LMGLogin :public LMsg
{
	Lint		m_id;
	//Lstring		m_key;
	Lstring		m_ip;
	Lshort		m_port;

	LMsgGame2LMGLogin() :LMsg(MSG_GAME_2_LM_LOGIN)
		, m_id(0)
		, m_port(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_id);
		//buff.Read(m_key);
		buff.Read(m_ip);
		buff.Read(m_port);
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_id);
		//buff.Write(m_key);
		buff.Write(m_ip);
		buff.Write(m_port);
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgGame2LMGLogin();
	}
};

struct GameServerInfo
{
	Lint		m_id;
	Lstring		m_ip;
	Lshort		m_port;
	LSocketPtr	m_sp;

	TCPClientPtr	m_client;
};

struct LMsgLMG2GateGameServerInfo :public LMsg
{
	Lint		m_ID;
	Lint		m_count;
	GameServerInfo	m_logic[500];

	LMsgLMG2GateGameServerInfo() :LMsg(MSG_LMG_2_G_SYNC_GAME_SERVER)
		, m_ID(0)
		, m_count(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_ID);
		buff.Read(m_count);
		for (Lint i = 0; i < m_count && i < 500; ++i)
		{
			buff.Read(m_logic[i].m_id);
			
			buff.Read(m_logic[i].m_ip);
			buff.Read(m_logic[i].m_port);
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_ID);
		buff.Write(m_count);
		for (Lint i = 0; i < m_count && i < 500; ++i)
		{
			buff.Write(m_logic[i].m_id);
			
			buff.Write(m_logic[i].m_ip);
			buff.Write(m_logic[i].m_port);
		}
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLMG2GateGameServerInfo();
	}
}; 

/*
struct LMsgG2GameUserMsg :public LMsg
{
	
	Lint			m_userMsgId;
	Lstring			m_ip;
	Lint            m_userDataID; //�û����ݿ�id,���ڱ������й��߼�����
	LBuffPtr		m_dataBuff;
	LMsg*			m_userMsg;

	LMsgG2GameUserMsg() :LMsg(MSG_G_2_GAME_USER_MSG), m_userMsg(NULL)
		, m_userMsgId(0)
		, m_userDataID(0)
	{
	}

	virtual~LMsgG2GameUserMsg()
	{
		if (m_userMsg)
			delete m_userMsg;
	}

	virtual bool Read(LBuff& buff)
	{
		
		buff.Read(m_userMsgId);
		buff.Read(m_ip);
		buff.Read(m_userDataID);

		int msgid = MSG_ERROR_MSG;

		try {
			msgpack::unpacked  unpack;
			msgpack::unpack(&unpack, buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
			msgpack::object obj = unpack.get();

			ReadMapData(obj, "m_msgId", msgid);
			m_userMsg = LMsgFactory::Instance().CreateMsg(m_userMsgId);
			if (m_userMsg)
			{
				m_userMsg->Read(obj);
			}
			else
			{
				LLOG_ERROR("LMsgG2LUserMsg read msgId not exiest %d", m_userMsgId);
			}
		}
		catch (...)
		{
			LLOG_ERROR("MSG_G_2_L_USER_MSG::RecvMsgPack error");
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		
		buff.Write(m_userMsgId);
		buff.Write(m_ip);
		buff.Write(m_userDataID);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2GameUserMsg();
	}
};*/


struct LMsgG2GameUserMsg :public LMsg
{
	
	Lint			m_userMsgId;
	Lstring			m_ip;
	Lint            m_userDataID; //�û����ݿ�id,���ڱ������й��߼�����
	LBuffPtr		m_dataBuff;
	LMsg*			m_userMsg;

	LMsgG2GameUserMsg() :LMsg(MSG_G_2_GAME_USER_MSG), m_userMsg(NULL)
		, m_userMsgId(0)
		, m_userDataID(0)
	{
	}

	virtual~LMsgG2GameUserMsg()
	{
		if (m_userMsg)
			delete m_userMsg;
	}

	virtual bool Read(LBuff& buff)
	{
		
		buff.Read(m_userMsgId);
		buff.Read(m_ip);
		buff.Read(m_userDataID);

		int msgid = MSG_ERROR_MSG;

		try {
			msgpack::unpacked  unpack;
			msgpack::unpack(&unpack, buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
			msgpack::object obj = unpack.get();

			ReadMapData(obj, "m_msgId", msgid);
			m_userMsg = LMsgFactory::Instance().CreateMsg(m_userMsgId);
			if (m_userMsg)
			{
				m_userMsg->Read(obj);
			}
			else
			{
				LLOG_ERROR("LMsgG2LUserMsg read msgId not exiest %d", m_userMsgId);
			}
		}
		catch (...)
		{
			LLOG_ERROR("MSG_G_2_L_USER_MSG::RecvMsgPack error");
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		
		buff.Write(m_userMsgId);
		buff.Write(m_ip);
		buff.Write(m_userDataID);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2GameUserMsg();
	}
};


//////////////////////////////////////////////////////////////////////////
//logic ���������Ϣ�� gate
struct LMsgGame2GUserMsg : public LMsg
{
	Lint			m_user_id;
	Lint			m_user_msg_id;
	LBuffPtr		m_dataBuff;

	LMsgGame2GUserMsg() :LMsg(MSG_GAME_2_G_USER_MSG)
		, m_user_id(0),m_user_msg_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);
		buff.Read(m_user_msg_id);
		m_dataBuff = LBuffPtr(new LBuff);
		m_dataBuff->Write(buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);
		buff.Write(m_user_msg_id);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgGame2GUserMsg();
	}
};



//gate��logicmanager
struct LMsgG2LMUserMsg :public LMsg
{

	Lint			m_userMsgId;
	Lstring			m_ip;
	Lint            m_userDataID; //�û����ݿ�id,���ڱ������й��߼�����
	LBuffPtr		m_dataBuff;
	LMsg*			m_userMsg;

	LMsgG2LMUserMsg() :LMsg(MSG_G_2_LOGIC_MANAGER_USER_MSG), m_userMsg(NULL)
		, m_userMsgId(0)
		, m_userDataID(0)
	{
	}

	virtual~LMsgG2LMUserMsg()
	{
		if (m_userMsg)
			delete m_userMsg;
	}

	virtual bool Read(LBuff& buff)
	{

		buff.Read(m_userMsgId);
		buff.Read(m_ip);
		buff.Read(m_userDataID);

		int msgid = MSG_ERROR_MSG;

		try {
			msgpack::unpacked  unpack;
			msgpack::unpack(&unpack, buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
			msgpack::object obj = unpack.get();

			ReadMapData(obj, "m_msgId", msgid);
			m_userMsg = LMsgFactory::Instance().CreateMsg(m_userMsgId);
			if (m_userMsg)
			{
				m_userMsg->Read(obj);
			}
			else
			{
				LLOG_ERROR("LMsgG2LMUserMsg read msgId not exiest %d", m_userMsgId);
			}
		}
		catch (...)
		{
			LLOG_ERROR("MSG_G_2_L_USER_MSG::RecvMsgPack error");
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{

		buff.Write(m_userMsgId);
		buff.Write(m_ip);
		buff.Write(m_userDataID);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgG2LMUserMsg();
	}
};


//////////////////////////////////////////////////////////////////////////
//logic ���������Ϣ�� gate
struct LMsgLM2GUserMsg : public LMsg
{
	Lint			m_user_id;
	Lint			m_user_msg_id;
	LBuffPtr		m_dataBuff;

	LMsgLM2GUserMsg() :LMsg(MSG_LOGIC_MANAGER_2_G_USER_MSG)
		, m_user_id(0), m_user_msg_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);
		buff.Read(m_user_msg_id);
		m_dataBuff = LBuffPtr(new LBuff);
		m_dataBuff->Write(buff.Data() + buff.GetOffset(), buff.Size() - buff.GetOffset());
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);
		buff.Write(m_user_msg_id);
		buff.Write(m_dataBuff->Data() + m_dataBuff->GetOffset(), m_dataBuff->Size() - m_dataBuff->GetOffset());
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLM2GUserMsg();
	}
};

//����




struct LMsgLM2LDeskOpt : public LMsg
{
	Lint			m_user_id; 
	Lint			m_type; // 1���� 2��������  3�˳�����
	Lint			m_desk_id;
	Lint			m_desk_type;
	Lint			m_cost;

	MsgUserInfo		m_self;


	LMsgLM2LDeskOpt() :LMsg(MSG_LM_2_L_DESK_OPT)
		, m_user_id(0),m_type(0),m_desk_id(0),m_desk_type(0),m_cost(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
	
		buff.Read(m_user_id);
		buff.Read(m_type);
		buff.Read(m_desk_id);
		buff.Read(m_desk_type);
		buff.Read(m_cost);

		buff.Read(m_self.m_user_id);
		buff.Read(m_self.m_name);
		buff.Read(m_self.m_head_icon);
		//buff.Read(m_self.m_pos);
		
		return true;
	}

	virtual bool Write(LBuff& buff)
	{
	
		buff.Write(m_user_id);
		buff.Write(m_type);
		buff.Write(m_desk_id);
		buff.Write(m_desk_type);
		buff.Write(m_cost);

		buff.Write(m_self.m_user_id);
		buff.Write(m_self.m_name);
		buff.Write(m_self.m_head_icon);
		//buff.Write(m_self.m_pos);
		
		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLM2LDeskOpt();
	}
};

struct LMsgL2LMDeskOpt : public LMsg
{
	Lint			m_result;			//���ؽ�� :0�ɹ�  1��ɫ�Ѿ����������� 2,�����Ѿ�����  3�������Ѿ����˼���ʧ��
										// 4,���Ӳ�����  5����ɫ����������
	Lint			m_user_id;
	Lint			m_type;
	Lint			m_desk_id;

	Lint			m_desk_type;		//��������  �����  ��ʯ��
	Lint			m_cost;				//��ʯ�����ĵ���ʯ

	bool			m_del_desk;			//�Ƿ�ɾ������

	int				m_user_cnt;			//��������
	std::vector<MsgUserInfo>	m_users;	//��������Ϣ



	LMsgL2LMDeskOpt() :LMsg(MSG_L_2_LM_DESK_OPT)
		, m_result(0), m_user_id(0),m_type(0), m_desk_id(0), m_desk_type(0), m_cost(0), m_del_desk(0), m_user_cnt(0)
	{
		m_users.clear();
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_result);
		buff.Read(m_user_id);
		buff.Read(m_type);
		buff.Read(m_desk_id);
		buff.Read(m_desk_type);
		buff.Read(m_cost);
		buff.Read(m_del_desk);

		buff.Read(m_user_cnt);

		for (int i = 0; i < m_user_cnt; ++i)
		{
			MsgUserInfo user;
			buff.Read(user.m_user_id);
			buff.Read(user.m_name);
			buff.Read(user.m_head_icon);
			//buff.Read(user.m_pos);

			m_users.push_back(user);
		}

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_result);
		buff.Write(m_user_id);
		buff.Write(m_type);
		buff.Write(m_desk_id);
		buff.Write(m_desk_type);
		buff.Write(m_cost);
		buff.Write(m_del_desk);

		int user_cnt = m_users.size();
		buff.Write(user_cnt);
		for (int i = 0; i < m_users.size(); ++i)
		{
			MsgUserInfo& user = m_users[i];
			buff.Write(user.m_user_id);
			buff.Write(user.m_name);
			buff.Write(user.m_head_icon);
			//buff.Write(user.m_pos);
		}

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgL2LMDeskOpt();
	}
};


struct LMsgLM2GUserStatusModify : public LMsg
{
	Lint			m_user_id;	
	Lint			m_status;		//״̬0������  1������
	Lint			m_logic_server_id;  


	LMsgLM2GUserStatusModify() :LMsg(MSG_LM_2_G_USER_STATUS_MODIFY)
		, m_user_id(0), m_status(0), m_logic_server_id(0)
	{
	}

	virtual bool Read(LBuff& buff)
	{
		buff.Read(m_user_id);
		buff.Read(m_status);
		buff.Read(m_logic_server_id);
		

		return true;
	}

	virtual bool Write(LBuff& buff)
	{
		buff.Write(m_user_id);
		buff.Write(m_status);
		buff.Write(m_logic_server_id);
	

		return true;
	}

	virtual LMsg* Clone()
	{
		return new LMsgLM2GUserStatusModify();
	}
};





#endif