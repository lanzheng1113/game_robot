#pragma once
#include <string>
class CMyNineXUsr
{
public:
	CMyNineXUsr();
	~CMyNineXUsr();
public:
	std::string getName() const { return _name; }
	void setName(std::string val) { _name = val; }
	std::string getAuthId() const { return _authId; }
	void setAuthId(std::string val) { _authId = val; }
	std::string getCardDaysType() const { return _cardDaysType; }
	void setCardDaysType(std::string val) { _cardDaysType = val; }
	std::string getHarddisk() const { return _harddisk; }
	void setHarddisk(std::string val) { _harddisk = val; }
	std::string getDate_of_end() const { return _date_of_end; }
	void setDate_of_end(std::string val) { _date_of_end = val; }
	bool isDirty(){ return _isDirty; }
	void setIsDirty(bool val){ _isDirty = val; }
private:
	std::string _name;
	std::string _authId;
	std::string _cardDaysType;
	std::string _harddisk;
	std::string _date_of_end;
	bool _isDirty;
};
