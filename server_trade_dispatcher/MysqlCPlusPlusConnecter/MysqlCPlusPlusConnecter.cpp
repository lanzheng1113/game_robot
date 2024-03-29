// MysqlCPlusPlusConnecter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cppconn/driver.h"
#include "cppconn/connection.h"
#include "mysql_connection.h"   
#include "mysql_driver.h"   

#include <cppconn/driver.h>   
#include <cppconn/exception.h>   
#include <cppconn/resultset.h>   
#include <cppconn/statement.h>   
#include <cppconn/prepared_statement.h>   
#include <Windows.h>
#include <vector>
#include <list>
using std::list;
using std::vector;

using namespace sql;  
using namespace std; 
int _tmain(int argc, _TCHAR* argv[])
{ 
	list <int> c1, c2;
	list <int>::iterator Iter;

	c1.push_back( 10 );
	c1.push_back( 20 );
	c1.push_back( 30 );
	c2.push_back( 40 );
	c2.push_back( 50 );
	c2.push_back( 60 );

	cout << "c1 =";
	for ( Iter = c1.begin( ); Iter != c1.end( ); Iter++ )
		cout << " " << *Iter;
	cout << endl;

	c1.insert( c1.end( ), c2.begin( ),c2.end( ) );

	cout << "c1 =";
	for ( Iter = c1.begin( ); Iter != c1.end( ); Iter++ )
		cout << " " << *Iter;
	cout << endl;

	return 1;

	sql::mysql::MySQL_Driver *driver;
	sql::Connection *con;

	driver = sql::mysql::get_mysql_driver_instance();
	sql::ConnectOptionsMap opt;
	opt["hostName"] = string("tcp://127.0.0.1:3306");
	opt["userName"] = string("root");
	opt["password"] = string("4260201asdf");
	opt["schema"] = string("tradeorder");
	opt["NAMES"] = string("gb2312");
	con = driver->connect(opt);
	if (con)
	{
		printf("connect success\n");
	}
	else{
		printf("connect failed!\n");
		return 1;
	}


	try {
		/* Connect to the MySQL test database */
		sql::Statement *stmt = con->createStatement();
		stmt->execute("SET NAMES \"GB2312\"");
		stmt->executeUpdate("update qqhx2confirm set logs = \"xx你个哦哦哦！\" where order_id =\"test_confirm_id_0\"");
		//con->setSchema("tradeorder");
		unsigned long start = GetTickCount();
		sql::ResultSet* rs = stmt->executeQuery("SELECT COUNT(*) FROM qqhx2order where GetState = 0  AND ResultSeller <> 0 AND ResultBuyer <> 0");
		//sql::ResultSet* rs = stmt->executeQuery("call get_none_confirm_count()");
	
		rs->next();
		printf("select count %d in %d ms\n",rs->getInt(1),GetTickCount() - start);

		start = GetTickCount();
		sql::ResultSet *rst = stmt->executeQuery("SELECT Indexs, TradeId, World, Seller, SellerPswd, SellerRole, Buyer,       BuyerPswd, BuyerRole, GetTime, DispatchTime, TradeTime, GoodCharactic,       TradePosition, isMoneyTrade, ResultBuyer,ResultSeller,TradeMoney, Getstate,TradeLogs FROM qqhx2order where GetState = 0  AND ResultSeller <> 0 AND ResultBuyer <> 0 ORDER BY GetTime limit 5;");
		if (rst)
		{
			vector<int> idVec;
			while (rst->next())
			{
				idVec.push_back(rst->getUInt("Indexs"));
				string strtest1 = rst->getString("GetTime");
				printf("the result:%s\n",strtest1.c_str());
			}

			unsigned long end = GetTickCount();
			printf("select use time = %d ms\n",end-start);

			/* '?' is the supported placeholder syntax */
			bool oldvaule = con->getAutoCommit();
			con->setAutoCommit(false);
// 			sql::PreparedStatement *pstmt = con->prepareStatement("update qqhx2order set getstate = 1 where Indexs = ?;");
// 			for (int i = 0; i < idVec.size(); i++) {
// 				pstmt->setInt(1, idVec[i]);
// 				pstmt->executeUpdate();
// 			}

 			sql::PreparedStatement *pstmt = con->prepareStatement("call set_get_state(?);");
 			for (int i = 0; i < idVec.size(); i++) {
 				pstmt->setInt(1, idVec[i]);
 				pstmt->execute();
 			}

 			con->commit();
 			con->setAutoCommit(oldvaule);
			
			printf("update %d record\n",idVec.size());
			delete pstmt;
			delete rst;
		}

		unsigned long end = GetTickCount();

		printf("select and update use time = %d ms\n",end-start);
		delete stmt;
	} catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line "
			<< __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	if (con)
	{
		delete con;
	}
	return 0;
}

