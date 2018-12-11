#pragma once

//#include "BaseMove.h"
#include "ChessGame.h"
#include "vector"


//��������������
class OperatorEvent
{
public:
	virtual void OnMoveEvent() = 0;
	virtual void OnAttackEvent() = 0;
	virtual void OnAttackDeathEvent() = 0;
	virtual void OnDamageEvent() = 0;
	virtual void OnDamageDeathEvent() = 0;

	virtual ~OperatorEvent()
	{

	}
};

class ChessBlock : public OperatorEvent
{
public:
	ChessBlock();
	virtual ~ChessBlock();

public:
	virtual void OnMoveEvent() ;
	virtual void OnAttackEvent() ;
	virtual void OnAttackDeathEvent();
	virtual void OnDamageEvent() ;
	virtual void OnDamageDeathEvent() ;


public:

	virtual bool	IsCanAttack(std::shared_ptr<ChessBlock> pBlock);
	//
	virtual bool			IsCanMove(const Point& pos)
	{
		return false;
	}
	
public:
	void			MoveToPos(const Point& pos);
	bool			IsInBoundary(Point point);
	bool			IsRed() { return m_Color == int(CHESS_RED); }
	void			GetDamage(int damage);
	

	PropertyBuilderByName(int, Id, protected);//int				m_Id;		
	PropertyBuilderByName(int, TableId, protected);//int				��Ӧ���ID;
	PropertyBuilderByName(int, Color, protected);//CHESS_COLOR		m_Color;	//�� ��
	PropertyBuilderByName(int, Type, protected);//TYPE_CHESS		m_Type;		//����
	PropertyBuilderByName(int, Hp, protected);//int				m_Hp;		//Ѫ��
	PropertyBuilderByName(int, Attack, protected);//intm_Attack;	//������
	PropertyBuilderByName(Point, Pos, protected);	// λ��

	PropertyBuilderByName(int, MaxHp, protected);//int				m_Hp;		//Ѫ������
	PropertyBuilderByName(int, MaxAttack, protected);//intm_Attack;	//����������

	PropertyBuilderByName(int, CoinLevel, protected);//
	PropertyBuilderByName(int, MaxCoinLevel, protected);//

	PropertyBuilderByName(ChessGame*,Game, protected)

};

class ChessJiang : public ChessBlock
{

public:
	bool			IsCanMove(const Point& pos);
	//bool			IsCanMove(const Point& pos);
	
};

class ChessShi : public ChessBlock
{
public:
	bool IsCanMove(const Point& pos);
};

class ChessXiang : public ChessBlock
{
public:
	bool IsCanMove(const Point& pos);
};

class ChessMa : public ChessBlock
{
public:
	bool IsCanMove(const Point& pos);
};


class ChessChe : public ChessBlock
{
public:
	bool IsCanMove(const Point& pos);
};

class ChessPao : public ChessBlock
{
public:
	bool IsCanMove(const Point& pos);
	bool IsCanAttack(std::shared_ptr<ChessBlock> pBlock);


};

class ChessBing : public ChessBlock
{
public:
	bool IsCanMove(const Point& pos);
};
