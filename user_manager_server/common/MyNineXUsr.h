#pragma once
#include <string>
/*
Index �Զ����
User_ID �ı�
UserDeadLine ����/ʱ��
AccType ����
UseAble ��/��
AccSuccessCount ����
AccUseCount ����
UserSystemInfo �ı�
UserCreateTime ����/ʱ��
UserRegistTime ����/ʱ��
ReActiveTime ����/ʱ��
ProxyName �ı�
*/
class CMyNineXUsr
{
public:
	CMyNineXUsr();
	~CMyNineXUsr();
public:
	/*UserCreateTime ����/ʱ��*/
	std::string getUserCreateTime() const { return _userCreateTime; }
	void setUserCreateTime(std::string val) { _userCreateTime = val; }
	/*UserRegistTime ����/ʱ��*/
	std::string getUserRegistTime() const { return _userRegistTime; }
	void setUserRegistTime(std::string val) { _userRegistTime = val; }
	/*ReActiveTime ����/ʱ��*/
	std::string getReActiveTime() const { return _reActiveTime; }
	void setReActiveTime(std::string val) { _reActiveTime = val; }
	/*AccUseCount ����*/
	int getAccUseCount() const { return _accUseCount; }
	void setAccUseCount(int val) { _accUseCount = val; }
	/*AccSuccessCount ����*/
	int getAccessSuccessCount() const { return _accessSuccessCount; }
	void setAccessSuccessCount(int val) { _accessSuccessCount = val; }
	/*UseAble ��/��*/
	bool IsEnable() const { return _isEnable; } 
	void IsEnable(bool val) { _isEnable = val; }
	/*Index �Զ����*/
	int getIndex() const { return _index; } 
	void setIndex(int val) { _index = val; }
	/*ProxyName �ı�*/
	std::string getName() const { return _name; } 
	void setName(std::string val) { _name = val; }
	/*User_ID �ı�*/
	std::string getAuthId() const { return _authId; } 
	void setAuthId(std::string val) { _authId = val; }
	/*AccType ����*/
	std::string getCardDaysType() const { return _cardDaysType; } 
	void setCardDaysType(std::string val) { _cardDaysType = val; }
	/*UserSystemInfo �ı�*/
	std::string getHarddisk() const { return _harddisk; }
	void setHarddisk(std::string val) { _harddisk = val; }
	/*UserDeadLine ����/ʱ��*/
	std::string getDate_of_end() const { return _date_of_end; }
	void setDate_of_end(std::string val) { _date_of_end = val; }
	 /*���־*/
	bool getIsDirty() const { return _isDirty; }
	void setIsDirty(bool val) { _isDirty = val; }
	/*�����õ�����*/
	int getHigh_seed() const { return _high_seed; }
	void setHigh_seed(int val) { _high_seed = val; }
	int getLow_seed() const { return _low_seed; }
	void setLow_seed(int val) { _low_seed = val; }
	/*������ʱ��*/
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
