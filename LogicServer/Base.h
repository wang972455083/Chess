#pragma once
#include "memory"
#include "Point.h"

#define PropertyBuilderByName(type, name, access_permission)\
    access_permission:\
        type m_##name;\
    public:\
    inline void set##name(type v) {\
        m_##name = v;\
    }\
    inline type get##name() {\
        return m_##name;\
    }\



//�췽���±ߣ��ڷ����ϱߡ����������½�Ϊ��0,0��

enum CHESS_COLOR
{
	CHESS_RED,
	CHESS_BLACK,
	CHESS_ERROR_COLOR,
};

enum PosLimit
{
	POS_LIMIT_X = 8,
	POS_LIMIT_Y = 9
};

enum TYPE_CHESS
{
	CHESS_NULL,
	CHESS_JIANG,
	CHESS_SHI,
	CHESS_XIANG,
	CHESS_MA,
	CHESS_CHE,
	CHESS_PAO,
	CHESS_BING,
	CHESS_MAX
};

//��̬ǰ�����������ֵ����Сֵ�ķ�Χ������ƶ�
enum TYPE_OPERATOR_MOVE
{
	MOVE_NULL ,							//û�ж���
	MOVE_ADVANCE_FIXED ,				//ǰ���̶�����
	MOVE_ADVANCE_DYNAMIC,				//��̬ǰ��
	MOVE_BACK_FIXED,					//����
	MOVE_BACK_DYNAMIC,					//��̬����
	MOVE_HORIZONTAL_FIXED,				//ˮƽ�̶��ƶ�
	MOVE_HORIZONTAL_DYNAMIC,			//ˮƽ��̬�ƶ�
	MOVE_LEAN_FIXED,					//��б�̶��ƶ�
	MOVE_LEAN_DYNAMIC,					//��б�̶��ƶ�
	MOVE_RI_ZI_GE						//�����ƶ�

};

//����
enum MOVE_DIREC
{
	MT_HORIZONTAL = 1,	//ˮƽ����
	MT_VERTICAL,		//��ֱ����
	MT_LEAN,			//��б����
	
};

