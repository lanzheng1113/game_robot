#include "summerinject.h"
#include <QtGui/QApplication>
#include <util/Logger.h>
#include <util/DateTime.h>
#include <string>
using std::string;

string generalString(){
    Date tm;
    string temp = tm.toString();
    temp += ".txt";
    return temp;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    string fileName = generalString();
    Logger::getInstance()->setLogFileName(fileName);
    LOG_INFO("logger setup %s",fileName.c_str());
	summerInject w;
	w.show();
	return a.exec();
}
