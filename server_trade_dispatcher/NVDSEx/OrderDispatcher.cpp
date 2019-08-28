#include "stdafx.h"
#include "OrderDispatcher.h"
#include <stdexcept>
#include "Settings.h"

using std::logic_error;

void COrderDispatcher::exit(){
	for (vector<CTradeOrderControler*>::const_iterator it = _controlers.begin(); it!=_controlers.end(); ++it){
		(*it)->exit();
		delete (*it);
	}
	_controlers.clear();
}

CTradeOrder* COrderDispatcher::GetNextOrder( int gameType)
{
	for (vector<CTradeOrderControler*>::const_iterator it = _controlers.begin(); it != _controlers.end(); ++it)
	{
		if((*it)->getGameTypeId() == gameType)
		{
			return (*it)->GetNextOrder();
		}
	}
	return NULL;
}

void COrderDispatcher::AddControler( CTradeOrderControler* controler )
{
	for (vector<CTradeOrderControler*>::const_iterator it = _controlers.begin(); it != _controlers.end();++it)
	{
		if ((*it)->getGameTypeId() == controler->getGameTypeId())
		{
			return;
		}
	}

	_controlers.push_back(controler);
}

void COrderDispatcher::PreLoadOrders()
{
	for (vector<CTradeOrderControler*>::const_iterator it = _controlers.begin(); it!=_controlers.end();++it)
	{
		(*it)->loadOrders(CSettings::getInstance().getDefGetOrderCountFromDbPerBatch((*it)->getGameTypeId()));
	}
}

void COrderDispatcher::EnableDispatcher( BOOL enable )
{
	for (vector<CTradeOrderControler*>::const_iterator it = _controlers.begin(); it != _controlers.end(); ++it)
	{
		(*it)->Enable(enable);
	}
}

int COrderDispatcher::getNowTradeListCount()
{
	int iret = 0;
	for (vector<CTradeOrderControler*>::const_iterator it = _controlers.begin();it!=_controlers.end();++it)
	{
		iret += (*it)->getTradeListCount();
	}
	return iret;
}

void COrderDispatcher::ECBK_TradeOrderLoaded( IDatabaseObject* sender,int game_type,list<CTradeOrder*> orderList )
{
	CTradeOrderControler* ss = NULL;
	for (int i = 0; i!=_controlers.size(); i++)
	{
		if (_controlers[i]->getGameTypeId() == game_type)
		{
			ss = _controlers[i];
			break;
		}
	}
	ASSERT(ss!=NULL);
	ss->tradeOrderLoaded(orderList);
}
