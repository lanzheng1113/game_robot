#include "stdafx.h"
#include "TDataBaseObject.h"

#include "cppconn/driver.h"
#include "cppconn/connection.h"
#include "mysql_connection.h"   
#include "mysql_driver.h"   

#include <cppconn/driver.h>   
#include <cppconn/exception.h>   
#include <cppconn/resultset.h>   
#include <cppconn/statement.h>   
#include <cppconn/prepared_statement.h>   

static CTDatabaseObject dbobject;

CTDatabaseObject::CTDatabaseObject()
{
	m_driver = NULL;
	m_connection = NULL;
}

CTDatabaseObject::~CTDatabaseObject()
{
	if (m_connection)
	{
		delete m_connection;
		m_connection = NULL;
	}

	if (m_driver)
	{
		delete m_driver;
		m_driver = NULL;
	}
}

void CTDatabaseObject::Close()
{
	if (m_connection)
	{
		delete m_connection;
		m_connection = NULL;
	}

	if (m_driver)
	{
		delete m_driver;
		m_driver = NULL;
	}
}

void CTDatabaseObject::exitRecyleTradeOrder( CTradeOrder* order )
{
	
}

void CTDatabaseObject::exitRecyleConfirmOrder( CConfirmOrder* confirm )
{

}

void CTDatabaseObject::flush()
{

}

list<CTradeOrder*> CTDatabaseObject::GetTradeNoneGetOrder( int game_type_id,unsigned int max_size )
{
	list<CTradeOrder*> ret;
	return ret;
}

list<CConfirmOrder*> CTDatabaseObject::GetNoneConfirmOrder( int game_type_id,unsigned int max_size )
{
	list<CConfirmOrder*> ret;
	return ret;
}

BOOL CTDatabaseObject::Init( const CString& strDbName,const CString& strUserName, const CString& strPassword,ADODB_DataBaseType dbType )
{
	return FALSE;
}

int CTDatabaseObject::InsertTradeRecord( const CTradeOrder& order )
{
	return 0;
}

int CTDatabaseObject::InsertConfirmOrder( const CConfirmOrder& confirm )
{
	return 0;
}

void CTDatabaseObject::saveTradeOrder( CTradeOrder* order,bool can_be_fetch )
{
	return;
}

void CTDatabaseObject::saveConfirm( CConfirmOrder* order,bool can_be_fetch )
{
	return;
}

void CTDatabaseObject::saveTradeOrderLog( CTradeOrder* orderSeller,CTradeOrder* orderBuyer )
{
	return;
}

void CTDatabaseObject::saveConfirmLog( CConfirmOrder* confirm )
{
	return;
}

void CTDatabaseObject::saveFatalFailedTradeRecord( CTradeOrder* seller,CTradeOrder* buyer )
{
	return;
}

UINT CTDatabaseObject::threadfun_exec_database( LPVOID thread_param )
{
	return 0;
}

UINT CTDatabaseObject::threadfun_flush( LPVOID thread_param )
{
	return 0;
}

string CTDatabaseObject::GetRecordTableName( int game_type_id ) const
{
	return string("");
}

string CTDatabaseObject::GetOrderTableName( int game_type_id ) const
{
	return string("");
}

string CTDatabaseObject::GetConfirmTableName( int game_type_id ) const
{
	return string("");
}

list<CTradeOrder*> CTDatabaseObject::GetTradeNoneGetOrder_in_db_thread( int game_type_id,unsigned int max_size )
{
	list<CTradeOrder*> ret;
	return ret;
}

list<CConfirmOrder*> CTDatabaseObject::GetNoneConfirmOrderInternal( int game_type_id,unsigned int max_size )
{
	list<CConfirmOrder*> ret;
	return ret;
}

void CTDatabaseObject::pushSqlSaveBuffer( const CString& strSql )
{
	return;
}

void CTDatabaseObject::saveOrder_exec_sql( const CStringArray& m_saved_sql )
{
	return;
}

void CTDatabaseObject::saveFatalRec_exec_sql( const CStringArray& m_saved_sql )
{
	return;
}

void CTDatabaseObject::test1()
{
	return;
}

CTDatabaseObject * afkGetDbObject()
{
	return &dbobject;
}
