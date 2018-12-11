#pragma once
#include "LBase.h"
#include "LSingleton.h"

#include <vector>



class User
{
public:
	User();
	~User();
public:
	void	Clear();
	Lint	GetUserId();
	void	SetUserId(Lint user_id);
	

	Lint	GetStatus();
	void	SetStatus(Lint status);

	Lint	GetGateServerId()
	{
		return m_gate_id;
	}
	void	SetGateServerId(Lint gate_id)
	{
		m_gate_id = gate_id;
	}

	//Lint	GetGateServerId();
	//void	SetGateServerId(Lint gate_id);

	//std::vector<Lint>&	GetInitChessBoard();
	//void	SetInitChessBoard(std::vector<Lint> init_chess);

	

private:
	Lint	m_user_id;
	Lint	m_status;
	std::vector<Lint>	m_init_chess;  //上场的棋子
	Lint	m_gate_id;
	
};

typedef std::shared_ptr<User>	LUserPtr;

