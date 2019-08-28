
// LogReaderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LogReader.h"
#include "LogReaderDlg.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <list>
#include <set>
#include "DebugMessage.h"
#include "util/StringEx.h"
#include "util/DateTime.h"
#include "util/DateTimeSpan.h"
#include "util/File.h"
#include "util/Path.h"
#include "MyNineXUsr.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


std::map<std::string,std::vector<std::string>> m_mapLogers;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CLogReaderDlg �Ի���




CLogReaderDlg::CLogReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogReaderDlg::IDD, pParent)
	, m_strInfos(_T(""))
	, m_strToFind(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strInfos);
	DDX_Text(pDX, IDC_EDIT2, m_strToFind);
}

BEGIN_MESSAGE_MAP(CLogReaderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LoadFile, &CLogReaderDlg::OnBnClickedButtonLoadfile)
	ON_BN_CLICKED(IDC_BUTTON_Find, &CLogReaderDlg::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_OLD, &CLogReaderDlg::OnBnClickedButtonSelectOld)
	ON_BN_CLICKED(IDC_BUTTON_EXCHANGE, &CLogReaderDlg::OnBnClickedButtonExchange)
END_MESSAGE_MAP()


// CLogReaderDlg ��Ϣ�������

BOOL CLogReaderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CLogReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLogReaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLogReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

std::list<std::string> getFilesInDir( const std::string& mulu )
{
	return File(mulu).getFileListRecursive();
}

CMyNineXUsr* lineToCMyNineXUsrObj( const std::string& line,const std::string& name ){
	String infors(line);
	StringList li = infors.split("|");
	if (li.size() < 4){
		return NULL;
	}

	CMyNineXUsr* zhxx = new CMyNineXUsr;
	if (!zhxx){
		return NULL;
	}

	//��ʽ��xo1xo2xo3xo|4xo5xo6xo7o9xo10xo11|2013-11-11_0:0:0 �������кţ�Ӳ�����кţ�����ʱ�䡣
	//062279883|31|δ��|δ��|δ��
	//062279887|3|δ��|δ��|2013-10-11_0:0:0
	//123456789|11|xp1xp2xp3xp|4xp5xp6xp7xp9xpsxo1xpk|2013-11-11_0:0:0
	std::string kardId = li[0];
	std::string kardType = li[1];
	std::string harddisk = li[2];
	std::string dateend = li[3];

	zhxx->setName(name);
	zhxx->setAuthId(kardId);
	zhxx->setCardDaysType(kardType);
	zhxx->setHarddisk(harddisk);
	zhxx->setDate_of_end(dateend);

	return zhxx;
}

std::list<CMyNineXUsr*> congwenjianduqu( const std::string& zhanghaowenjianming )
{
	std::ifstream ifs;
	std::list<CMyNineXUsr*> zhanghaoliebiao;
	ifs.open(zhanghaowenjianming.c_str());

	File f(zhanghaowenjianming);
	std::string sfname = f.getFileName();
	StringList sl = String(sfname).split(".");
	if (!sl.empty()){
		sfname = sl[0];
	}else{
		DebugMessage("��ȡ�û�����ʧ���ˡ�");
		return zhanghaoliebiao;
	}

	if (ifs.is_open())
	{
		std::string line;
		int lines = 0;
		std::vector<std::string> str_lines;
		std::vector<std::string> tagBlock;
		while (getline(ifs,line)){
			DebugMessage("%d:%s",lines+1,line.c_str());
			String lineEx(line);	//�޳�LINE�еĿո�
			line = lineEx.trim();
			str_lines.push_back(line);
			lines ++;
		}
		ifs.close();

		if (lines == 0){
			return zhanghaoliebiao;
		}else{
			int sz = str_lines.size();
			for (int i=0; i!=sz; i++){
				CMyNineXUsr* zhxx= lineToCMyNineXUsrObj(str_lines[i],sfname); //����ת������Ȩ��Ϣ����
				if (zhxx){
					zhanghaoliebiao.push_back(zhxx);
				}
			}
		}
	}else{
		char xzxxe[255] = {0};
		sprintf(xzxxe,"���ļ�%sʧ����LastError=%d��",zhanghaowenjianming.c_str(),GetLastError());
		MessageBox(NULL,xzxxe,NULL,MB_OK);
	}
	return zhanghaoliebiao;
}

std::vector<CMyNineXUsr*> _usrs;

void addNineXUsrs( const std::list<CMyNineXUsr*>& NineXUsrs )
{
	for (std::list<CMyNineXUsr*>::const_iterator it=NineXUsrs.begin(); it != NineXUsrs.end(); ++it){
		_usrs.push_back(*it);
	}
}

BOOL load()
{
	_usrs.clear();
	std::string pa = Path::getApplicationDirPath();
	std::string pa1 = pa + "Enables\\";
	File::createPath(pa1);
	std::list<std::string> wenjians = getFilesInDir(pa1);

	int ics = 1,itts = wenjians.size();
	for (std::list<std::string>::const_iterator it = wenjians.begin(); it != wenjians.end(); ++it){
		std::stringstream ss; ss << "��ȡ�ļ�" << *it << "����" << ics++ << "������" << itts << "����" ;
		if (String(*it).endsWith(".txt")){
			std::list<CMyNineXUsr*> zhanghaoxinxis = congwenjianduqu(*it);
			if (!zhanghaoxinxis.empty()){
				addNineXUsrs(zhanghaoxinxis);
			}
		}
	}

	for (int i=0; i!=_usrs.size(); i++){
		DebugMessage( "��%d��,id = %s,type=%s,name=%s,harddiskid=%s,tm=%s",
			i+1,_usrs[i]->getAuthId().c_str(),
			_usrs[i]->getCardDaysType().c_str(),_usrs[i]->getName().c_str(),
			_usrs[i]->getHarddisk().c_str(),
			_usrs[i]->getDate_of_end().c_str());
	}

	return TRUE;
}
std::vector<std::string> findlog(const CString& tofind);
void resetFromLog(CMyNineXUsr* usrInfo){
	std::string pac = "#��.�û���Ī���˿�";
	pac += usrInfo->getAuthId();
	DebugMessage("����ʷ��¼�в���%s",pac.c_str());
	std::vector<std::string> foundlog = findlog((LPCTSTR)pac.c_str());
	if (foundlog.empty()){
		DebugMessage("û���ҵ�%s�ļ�¼",usrInfo->getAuthId().c_str());
	}else{
		DebugMessage("     **�ҵ���%s�ļ�¼**",usrInfo->getAuthId().c_str());
		DebugMessage("     **��¼Ϊ%s**",foundlog[1].c_str());
		std::string timeBand = foundlog[1].substr(7,19);
		DebugMessage("     **��ʱ��Ϊ%s**",timeBand.c_str());

		DateTime dt;
		DateTime::fromString(timeBand,dt);
		int ba = atoi(usrInfo->getCardDaysType().c_str());
		DateTimeSpan dtspan(ba,0,0,0);
		dt += dtspan;

		std::stringstream ss;
		//2013-11-11_0:0:0
		ss << dt.getYear() << "-" << dt.getMonth() << "-" << dt.getDay() 
			<<"_" << dt.getHour() << ":" << dt.getMinute() << ":" << dt.getSecond();
		usrInfo->setDate_of_end(ss.str());
		DebugMessage("**����������Ϊ%s**",ss.str().c_str());
	}
}

BOOL saveUsrListByName( const std::string& name )
{
	//���ȴ��û��б�������һ��ֻ������Ϊname�������б�
	std::vector<CMyNineXUsr*> v;
	for (std::vector<CMyNineXUsr*>::const_iterator it=_usrs.begin(); it != _usrs.end(); ++it){
		if ((*it)->getName() == name){
			v.push_back(*it);
		}
	}

	if (v.empty()){
		return FALSE;
	}

	std::vector<std::string> vs;

	for (int i=0; i!=v.size(); i++)
	{
		std::string line = v[i]->getAuthId(); line += "|";
		line += v[i]->getCardDaysType(); line += "|";
		line += v[i]->getHarddisk(); line += "|";
		line += v[i]->getDate_of_end();

		vs.push_back(line);
	}

	//����ȷ�ϡ�name��.txt�Ƿ����,����������򴴽�����
	std::string pa = Path::getApplicationDirPath();
	std::string pa1 = pa + "Enables\\";
	pa1 += name;
	pa1 += ".txt";

	File::createPath(pa1);
	//OK�������ļ�һ�����ڡ�������
	std::ofstream ofs;
	ofs.open(pa1.c_str());
	if (!ofs.is_open()){
		return FALSE; //��ʧ���ˡ�
	}

	for (int i=0; i!=vs.size(); i++)
	{
		//����д��
		ofs << vs[i] << "\n";
	}
	ofs.close();

	return TRUE;
}

void CLogReaderDlg::OnBnClickedButtonExchange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//2:(info) 2014-03-04 14:06:11 [Common] DoValidateAffair::OnDataRecv: #��.�û���Ī���˿�140316310,IP=123.130.175.88
	load();

	std::set<std::string> nameset;
	for (int i=0; i!=_usrs.size(); i++){
		DebugMessage( "�����%d��,id = %s,type=%s,name=%s,harddiskid=%s,tm=%s",i+1,_usrs[i]->getAuthId().c_str(),_usrs[i]->getCardDaysType().c_str(),_usrs[i]->getName().c_str(),_usrs[i]->getHarddisk().c_str(),_usrs[i]->getDate_of_end().c_str());
		resetFromLog(_usrs[i]);
		nameset.insert(_usrs[i]->getName());
	}

	for (std::set<std::string>::const_iterator it = nameset.begin(); it!=nameset.end(); ++it)
	{
		saveUsrListByName(*it);
	}
}


std::vector<std::string> findlog(const CString& tofind){
	std::vector<std::string> str_lines;
	for (std::map<std::string,std::vector<std::string> >::const_iterator itMap = m_mapLogers.begin(); itMap != m_mapLogers.end(); ++itMap){
		std::string fn = itMap->first;
		bool fnLoged = false;
		for (std::vector<std::string>::const_iterator itVec = itMap->second.begin(); itVec != itMap->second.end(); ++itVec){
			std::string logline = *itVec;
			if (logline.find((LPCTSTR)tofind) != std::string::npos){
				if (!fnLoged){
					fnLoged = true;
					str_lines.push_back(fn);
				}
				str_lines.push_back(logline);
			}
		}
	}
	return str_lines;
}

std::vector<std::string> readlogfile(const CString& name){
	std::vector<std::string> str_lines;
	std::ifstream ifs;
	ifs.open((LPCTSTR)name);
	if (ifs.is_open())
	{
		std::string line;
		int lines = 0;
		while (getline(ifs,line)){
//			DebugMessage("%d:%s",lines+1,line.c_str());
			String lineEx(line);	//�޳�LINE�еĿո�
			line = lineEx.trim();
			str_lines.push_back(line);
			lines ++;
		}
		ifs.close();
	}else{
		char xzxxe[255] = {0};
		sprintf(xzxxe,"���ļ�%sʧ����LastError=%d��",(LPCTSTR)name,GetLastError());
		MessageBox(NULL,xzxxe,NULL,MB_OK);
	}

	return str_lines;
}

void CLogReaderDlg::OnBnClickedButtonLoadfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog   dlg( TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, NULL, NULL );  
	DWORD MAXFILE = 2562; //2562 is the max  
	TCHAR* pc = new TCHAR[MAXFILE * (MAX_PATH+1) + 1];  
	pc[0] = pc[1] = 0;  
	dlg.m_ofn.nMaxFile = MAXFILE;  
	dlg.m_ofn.lpstrFile = pc;  
	dlg.m_ofn.lpstrFilter = _T("Text Files (*.txt)\0*.txt\0")
							_T("All Files (*.*)\0*.*\0")  ;
	int iReturn = dlg.DoModal();  
	std::vector<CString> files;
	if(iReturn ==  IDOK)  
	{  
		m_mapLogers.clear();
		int nCount = 0;  
		POSITION pos = dlg.GetStartPosition();  
		while (pos != NULL)  {  
			CString szPathName = dlg.GetNextPathName(pos);  
			files.push_back(szPathName);
			nCount++;  
		}  

		if (nCount){
			for(int i=0; i!=files.size();i++){

				DebugMessage("insert file %s",files[i]);
				std::vector<std::string> mbs = readlogfile(files[i]);
				if (!mbs.empty()){
					m_mapLogers.insert(std::make_pair((LPCTSTR)files[i],mbs));
				}
			}
		}
// 		CString str;  
// 		str.Format("Successfully opened %d files/n", nCount);  
// 		AfxMessageBox(str);  
	}  
	else if(iReturn == IDCANCEL)  
		AfxMessageBox("Cancel");  

	UpdateData(FALSE);
}

void CLogReaderDlg::OnBnClickedButtonFind()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_strInfos = "";
	std::vector<std::string> mbs = findlog((LPCTSTR)m_strToFind);
	for (int i = 0; i!= mbs.size(); i++){
		CString strIndex;
		strIndex.Format("%d:",i+1);
		m_strInfos += strIndex;
		m_strInfos += mbs[i].c_str();
		m_strInfos += "\r\n";
	}

	UpdateData(FALSE);
}

std::vector<std::string >FLold;
void exchange_old(const std::string& fp){
	std::vector<std::string> lines;
	DateTime dt_begin;
	/*
	Log file=D:\ħ��9�Ǹ�����֤������\server_logs\��������־_2013��12��21��17ʱ49��15��.txt
	Date=2013-12-21
	Time=17:49:15
	*/
	//(info) 17:49:15
	std::vector<std::string> str_lines;
	std::ifstream ifs;
	bool setfirstTime = false;
	ifs.open(fp.c_str());
	if (ifs.is_open()){
		std::string line;
		int lines = 0;
		int i = 0;
		int year,month,day,hour,minute,second;
		DateTime prev;
		while (getline(ifs,line)){
			DebugMessage("%d:%s",lines+1,line.c_str());
			String lineEx(line);	//�޳�LINEǰ��Ŀո�
			line = lineEx = lineEx.trim();
			lines ++;
			if (lines == 1){
				;
			}else if (lines == 2){
				if (lineEx.beginsWith("Date=")){
					sscanf(lineEx.c_str(),"Date=%d-%d-%d",&year,&month,&day);
				}else{
					MessageBox(NULL,"����ĵڶ���",NULL,MB_OK);
				}
			}else if (lines == 3){
				if (lineEx.beginsWith("Time=")){
					sscanf(lineEx.c_str(),"Time=%d:%d:%d",&hour,&minute,&second);
				}else{
					MessageBox(NULL,"����ĵ�����",NULL,MB_OK);
				}
				dt_begin.setYear(year);
				dt_begin.setMonth(month);
				dt_begin.setDay(day);
				dt_begin.setHour(0);
				dt_begin.setMinute(0);
				dt_begin.setSecond(0);

				prev = dt_begin;
				MessageBox(NULL,dt_begin.toString().c_str(),NULL,MB_OK);
			}else{
				//(info) 17:05:21
				Time first(0,0,0);
				int hour,minute,second;
				if (lineEx.length() >= 15){
					std::string tag;
					if (lineEx.beginsWith("(info)")){
						tag = lineEx.substr(0,15);
						sscanf(tag.c_str(),"(info) %d:%d:%d",&hour,&minute,&second);
						if (!setfirstTime){ //���ÿ�ʼʱ��
							setfirstTime = true;
							first = Time(hour,minute,second);
							char msg[256] = {0};
							sprintf(msg,"���ÿ�ʼʱ��Ϊ%s",dt_begin.toString().c_str());
							MessageBox(NULL,msg,NULL,MB_OK);
						}
						
						DateTime dt;
						dt.setYear(prev.getYear());
						dt.setMonth(prev.getMonth());
						dt.setDay(prev.getDay());
						dt.setHour(hour);
						dt.setMinute(minute);
						dt.setSecond(second);

						DateTimeSpan span = dt - prev;
						if (span.GetTotalSeconds() < 0){
							//�����ǰʱ��С��ǰһ��ʱ�䣬��ô˵���������һ�졣
							MessageBox(NULL,"�����ǰʱ��С��ǰһ��ʱ�䣬��ô˵���������һ�졣",NULL,MB_OK);
							DateTimeSpan spanAdd1(1,0,0,0);
							dt += spanAdd1;
						}
						prev = dt; //���浱ǰʱ�����ڱȽ���һ����
						
						//�滻ǰ15���ֽ�
						std::string replace = "(info) ";
						replace += dt.toString();
						std::string trimedString = lineEx.substr(15);
						lineEx = replace + trimedString;

					}else if (lineEx.beginsWith("(error)")){
						//(error) 17:07:47
						tag = lineEx.substr(0,16);
						sscanf(tag.c_str(),"(error) %d:%d:%d",&hour,&minute,&second);
						if (!setfirstTime){ //���ÿ�ʼʱ��
							setfirstTime = true;
							first = Time(hour,minute,second);
						}

						DateTime dt;
						dt.setYear(prev.getYear());
						dt.setMonth(prev.getMonth());
						dt.setDay(prev.getDay());
						dt.setHour(hour);
						dt.setMinute(minute);
						dt.setSecond(second);

						DateTimeSpan span = dt - prev;
						if (span.GetTotalSeconds() < 0){
							//�����ǰʱ��С�ڿ�ʼʱ�䣬��ô˵���������һ�졣
							DateTimeSpan spanAdd1(1,0,0,0);
							dt += spanAdd1;
						}

						prev = dt;
						//�滻ǰ15���ֽ�
						std::string replace = "(error) ";
						replace += dt.toString();
						std::string trimedString = lineEx.substr(16);
						lineEx = replace + trimedString;
					}

					//dt_begin ��¼�˵�һ��ʱ�䡣��������ʱ����dt_begin֮ǰ����ôdt_begin��Day�����һ��
					
				}

			}
			str_lines.push_back(lineEx);
		}
		ifs.close();

		std::ofstream ofs;
		ofs.open(fp.c_str());
		if (!ofs.is_open()){
			MessageBox(NULL,"����д���ļ�ʱ���ļ�ʧ�ܡ�",NULL,MB_OK);
			return; //��ʧ���ˡ�
		}

		for (int i=0; i!=str_lines.size(); i++){
			//����д���ļ�
			ofs << str_lines[i] << "\n";
		}
		ofs.close();

	}else{
		char xzxxe[255] = {0};
		sprintf(xzxxe,"���ļ�%sʧ����LastError=%d��",(LPCTSTR)fp.c_str(),GetLastError());
		MessageBox(NULL,xzxxe,NULL,MB_OK);
	}


}

void CLogReaderDlg::OnBnClickedButtonSelectOld()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FLold.clear();
	CFileDialog   dlg( TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, NULL, NULL );  
	DWORD MAXFILE = 2562; //2562 is the max  
	TCHAR* pc = new TCHAR[MAXFILE * (MAX_PATH+1) + 1];  
	pc[0] = pc[1] = 0;  
	dlg.m_ofn.nMaxFile = MAXFILE;  
	dlg.m_ofn.lpstrFile = pc;  
	dlg.m_ofn.lpstrFilter = _T("Text Files (*.txt)\0*.txt\0")
		_T("All Files (*.*)\0*.*\0")  ;
	int iReturn = dlg.DoModal();  
	if(iReturn ==  IDOK)  
	{  
		m_mapLogers.clear();
		int nCount = 0;  
		POSITION pos = dlg.GetStartPosition();  
		while (pos != NULL)  {  
			CString szPathName = dlg.GetNextPathName(pos);  
			FLold.push_back((LPCTSTR)szPathName);
			nCount++;  
		}  

		if (nCount){
			for(int i=0; i!=FLold.size();i++){
				DebugMessage("insert file %s",FLold[i].c_str());
				exchange_old(FLold[i]);
			}
		}
// 		CString str;  
// 		str.Format("Successfully opened %d files/n", nCount);  
// 		AfxMessageBox(str);  
	}  
	else if(iReturn == IDCANCEL)  
		AfxMessageBox("Cancel");  

	UpdateData(FALSE);
}
