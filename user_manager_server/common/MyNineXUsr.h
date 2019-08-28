#pragma once
#include <string>
/*
Index 自动编号
User_ID 文本
UserDeadLine 日期/时间
AccType 数字
UseAble 是/否
AccSuccessCount 数字
AccUseCount 数字
UserSystemInfo 文本
UserCreateTime 日期/时间
UserRegistTime 日期/时间
ReActiveTime 日期/时间
ProxyName 文本
*/
class CMyNineXUsr
{
public:
	CMyNineXUsr();
	~CMyNineXUsr();
public:
	/*UserCreateTime 日期/时间*/
	std::string getUserCreateTime() const { return _userCreateTime; }
	void setUserCreateTime(std::string val) { _userCreateTime = val; }
	/*UserRegistTime 日期/时间*/
	std::string getUserRegistTime() const { return _userRegistTime; }
	void setUserRegistTime(std::string val) { _userRegistTime = val; }
	/*ReActiveTime 日期/时间*/
	std::string getReActiveTime() const { return _reActiveTime; }
	void setReActiveTime(std::string val) { _reActiveTime = val; }
	/*AccUseCount 数字*/
	int getAccUseCount() const { return _accUseCount; }
	void setAccUseCount(int val) { _accUseCount = val; }
	/*AccSuccessCount 数字*/
	int getAccessSuccessCount() const { return _accessSuccessCount; }
	void setAccessSuccessCount(int val) { _accessSuccessCount = val; }
	/*UseAble 是/否*/
	bool IsEnable() const { return _isEnable; } 
	void IsEnable(bool val) { _isEnable = val; }
	/*Index 自动编号*/
	int getIndex() const { return _index; } 
	void setIndex(int val) { _index = val; }
	/*ProxyName 文本*/
	std::string getName() const { return _name; } 
	void setName(std::string val) { _name = val; }
	/*User_ID 文本*/
	std::string getAuthId() const { return _authId; } 
	void setAuthId(std::string val) { _authId = val; }
	/*AccType 数字*/
	std::string getCardDaysType() const { return _cardDaysType; } 
	void setCardDaysType(std::string val) { _cardDaysType = val; }
	/*UserSystemInfo 文本*/
	std::string getHarddisk() const { return _harddisk; }
	void setHarddisk(std::string val) { _harddisk = val; }
	/*UserDeadLine 日期/时间*/
	std::string getDate_of_end() const { return _date_of_end; }
	void setDate_of_end(std::string val) { _date_of_end = val; }
	 /*脏标志*/
	bool getIsDirty() const { return _isDirty; }
	void setIsDirty(bool val) { _isDirty = val; }
	/*加密用的种子*/
	int getHigh_seed() const { return _high_seed; }
	void setHigh_seed(int val) { _high_seed = val; }
	int getLow_seed() const { return _low_seed; }
	void setLow_seed(int val) { _low_seed = val; }
	/*最后解绑的时间*/
	std::string getDate_of_last_unbind() const { return _date_of_last_unbind; }
	void setDate_of_last_unbind(std::string val) { _date_of_last_unbind = val; }
private:
	std::string _name;
	std::string _authId;
	std::string _cardDaysType;
	std::string _harddisk;
	std::string _date_of_end;
	std::string _date_of_last_unbind;
	bool _isDirty;
	int _index;
	bool _isEnable;
	int _accessSuccessCount;
	int _accUseCount;
	std::string _userCreateTime;
	std::string _userRegistTime;
	std::string _reActiveTime;
	int _high_seed;
	int _low_seed;
};
