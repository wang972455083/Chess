#include "GateUser.h"
#include "LLog.h"
#include "LTime.h"
bool GateUserManager::Init()
{
	
	return true;
}

bool GateUserManager::Final()
{
	return true;
}

GUserPtr GateUserManager::GetUserBySp(LSocketPtr sp)
{
	auto iter = m_gateUserSpMap.find(sp);
	if (iter != m_gateUserSpMap.end())
	{
		return iter->second;
	}

	return nullptr;
}


GUserPtr GateUserManager::GetUserById(Lint user_id)
{
	auto iter = m_gateUserIdMap.find(user_id);
	if (iter!= m_gateUserIdMap.end())
	{
		return iter->second;
	}

	return NULL;
}

void GateUserManager::AddUser(GUserPtr user)
{
	
	m_gateUserIdMap[user->m_user_id] = user;

	m_gateUserSpMap[user->m_sp] = user;
}

void GateUserManager::DelUser(GUserPtr user)
{
	if (nullptr == user)
		return;

	int user_id = user->getUserId();
	LSocketPtr sp = user->m_sp;
	auto iter1 = m_gateUserIdMap.find(user_id);
	
	if (iter1!=m_gateUserIdMap.end())
	{
		m_gateUserIdMap.erase(iter1);
	}
	
	auto iter2 = m_gateUserSpMap.find(sp);
	if (iter2 != m_gateUserSpMap.end())
	{
		m_gateUserSpMap.erase(iter2);
	}

}

GUserPtr GateUserManager::CreateUser(Lint user_id,LSocketPtr sp)
{
	GUserPtr user = std::make_shared<GateUser>();
	
	user->m_user_id = user_id;
	user->m_sp = sp;
	user->m_login = 0;

	AddUser(user);
	LLOG_DEBUG("GateUserManager::CreateUser AddUser ");
	return user;
}



