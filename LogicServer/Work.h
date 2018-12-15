#ifndef _WORK_H_
#define _WORK_H_

#include "LRunnable.h"
#include "LSingleton.h"
#include "LTime.h"
#include "LMsg.h"
#include "LNet.h"
#include "LMsgS2S.h"
#include "TCPClient.h"
#include "GarbageCollectionService.h"
#include "LCardsOperType.h"
#include "DeskManager.h"

class User;

struct UserLoginInfo
{
	Lint	m_seed;
	Lint	m_time;
	LUser	m_user;
};

class Work:public LRunnable,public LSingleton<Work>
{
public:
	virtual bool	Init();
	virtual bool	Final();

	virtual void	Start();
	virtual void	Run();
	virtual void	Clear();
	virtual	void	Join();
	virtual	void	Stop();

	void			Tick(LTime& cur);
	void			HanderMsg(LMsg* msg);

	LTime&			GetCurTime();
	//md5�����㷨
	Lstring			Md5Hex(const void* data,Lsize size);
public:
	//����http��Ϣ 
	void			HanderHttp(LMsgHttp* msg);	
	//����ͻ�������������Ϣ
	void			HanderClientIn(LMsgIn* msg);
	//����ͻ��˵��ߵ���Ϣ 
	void			HanderUserKick(LMsgKick* msg);
	void			ConnectToLogicManager();
	void			SendToLogicManager(LMsg& msg);
	
public:
	void			HanderGateLogin(LMsgG2ServerLogin* msg);
	void			HanderGateLogout(LMsgKick* msg);
	GateInfo*		GetGateInfoBySp(LSocketPtr sp);
	GateInfo*		GetGateInfoById(Lint id);
	void			DelGateInfo(Lint id);
	void			HanderGateUserMsg(LMsgG2LUserMsg* msg, GateInfo* gate);
	void			HanderKeepAlive(LMsgS2SKeepAlive* msg);
	void			HanderKeepAliveAck(LMsgS2SKeepAliveAck* msg);
private:
	void			SendLogicLoginToLM();

public:
	//void			HanderCreateDesk(LMsgLM2LCreateDesk* msg);
	void			HanderDeskOpt(LMsgLM2LDeskOpt* msg);
	void			HanderDeskUserReadyOpt(LMsgC2SReadyOpt* msg);
	void			HanderChessMove(LMsgC2SChessMove* msg);
	void			HanderChessAttack(LMsgC2SChessAttack* msg);
	void			HanderChessUpgrade(LMsgC2SChessUpgrade* msg);
public:
	void			SendMessageToUser(LSocketPtr sp, Lint user_id, LMsg& msg);
	void			FillDeskMsg(LMsgL2LMDeskOpt& send, LDeskPtr desk);
private:
	LTime		m_tickTimer;

	Llong		m_30MSceTick;//30����ѭ��һ��

	Llong		m_200MSceTick;//200����ѭ��һ��

	Llong		m_1SceTick;//1��ѭ��һ��

	Llong		m_5SceTick;//5��ѭ��һ��

	Llong		m_30SceTick;//30��ѭ��һ��

	Llong		m_600SceTick;//5������ѭ��һ��

private:
	TCPClientPtr	m_logicManager;
	std::map<Lint, GateInfo> m_gateInfo;
	bool				m_loginLMFirst;
	GarbageCollectionService m_gcService;
};

#define gWork Work::Instance()

#endif