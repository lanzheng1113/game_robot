#include "StdAfx.h"
#include "samplecall.h"
#include "itpcore.h"
#include "debugmessage.h"

#define TAG_OF_TEMP

samplecall::samplecall(void)
{
}

samplecall::~samplecall(void)
{
}

//�жϵ�ͼ
bool BOOL_PROXY_CALL jugeMap(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() < 2)
	{
		DebugMessage("jugeMap �������󣬲���������С��2");
		return false;
	}
	else
	{
		TAG_OF_TEMP;
		DebugMessage("jugeMap Ŀ���ͼID=%s,δ֪��=%s",param[0].c_str(),param[1].c_str());
		return (rand() % 2 == 0);
	}
}

bool BOOL_PROXY_CALL jugeHero(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (0 == param.size())
	{
		DebugMessage("jugeHero �������󣬲�������Ϊ0");
		return false;
	}
	else
	{
		//��� [�ж����� SP��ֵ <= 80 0] ��ô ������� [ 9X��ʼ_SP���� ]
		if (param[0] == "SP��ֵ")
		{
			if (param.size() < 4)
			{
				DebugMessage("jugeHero �������󣬲�������С��4");
				return false;
			}
			else{
				TAG_OF_TEMP;
				DebugMessage("jugeHero SP��ֵ");
				return (rand()%2 == 0);
			}
		}
		else{
			CMessageBoxA("δ֪�Ĳ��� %s",param[0].c_str());
			return false;
		}
	}
}
bool BOOL_PROXY_CALL jugeItem(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("jugeItem �������󣬲����Ĵ�СΪ0");
		return false;
	}
	else
	{
		if (param[0] == "�ϳɻ�ħ֮��")
		{
			if (param.size() < 5 )
			{
				DebugMessage("jugeItem �ϳɻ�ħ֮�ĵĲ�������");
				return false;
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("jugeItem �ϳɻ�ħ֮��,����=%s,�ж���=%s,����=%s,δ֪��=%s",param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
				return (rand()%2 == 0);
			}
		}
		else if (param[0] == "�ϳ����")
		{
			if (param.size() < 5)
			{
				DebugMessage("jugeItem �ϳ����Ĳ�������");
				return false;
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("jugeItem �ϳ����,����=%s,�ж���=%s,����=%s,δ֪��=%s",param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
				return (rand()%2 == 0);
			}
		}
		else if (param[0] == "��ħ��ʯ")
		{
			if (param.size() < 5)
			{
				DebugMessage("jugeItem ��ħ��ʯ�Ĳ�������");
				return false;
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("jugeItem ��ħ��ʯ,���� = %s,�ж���=%s,����=%s,δ֪��=%s",param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
				return (rand()%2==0);
			}
		}
		else if (param[0] == "��꾧ʯ")
		{
			if (param.size() < 5)
			{
				DebugMessage("jugeItem ��꾧ʯ�Ĳ�������");
				return false;
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("jugeItem ��꾧ʯ,���� = %s,�ж���=%s,����=%s,δ֪��=%s",param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
				return (rand()%2==0);
			}
		}
		else 
		{
			TAG_OF_TEMP;
			DebugMessage("jugeItem δ֪�Ĳ���");
			return false;
		}
	}
}

void VOID_PROXY_CALL sc_sleep(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("sc_sleep �������󣬲����Ĵ�СΪ0");
	}
	else
	{
		DebugMessage("sc_sleep(%s)",param[0].c_str());
		Sleep(atoi(param[0].c_str()));
	}
}

void VOID_PROXY_CALL team_op(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("sc_sleep �������󣬲����Ĵ�СΪ0");
	}
	else
	{
		if (param[0] == "��ö�����Ϣ")
		{
			TAG_OF_TEMP;
			DebugMessage("������� - ��ö�����Ϣ"); 
			return;
		}
		else if (param[0] == "˭�ܽ�����")
		{
			TAG_OF_TEMP;
			DebugMessage("�������- ˭�ܽ�����");
		}
		else if (param[0] == "�������")
		{
			TAG_OF_TEMP;
			DebugMessage("������� - �������");
		}
		else if (param[0] == "��ת")
		{
			//������� [ ������� 0 0 0 0 0 0] 
			//������� [ ��ת 31 9X����-��Ա���� 0 0 0 0] 
			TAG_OF_TEMP;
			DebugMessage("������� - ��ת");
		}
		else if (param[0] == "������")
		{
			//������� [ ������ Ѫ�� 5 0 0 0 0] 
			if (param.size() < 3)
			{
				CMessageBoxA("[����]�������-�����ߣ���������");
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("������� ������ Ѫ��");
			}
		}
		else if (param[0] == "�������")
		{
			TAG_OF_TEMP;
			DebugMessage("������� �������");
			////������� [ ������� ս���� 1 ��ʦ��� 2 0 0 ]
		}
		else{
			TAG_OF_TEMP;
			CMessageBoxA("[����]�������-δ֪������",param[0].c_str());
		}
	}
}

void VOID_PROXY_CALL setItems(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("setItems �������󣬲����Ĵ�СΪ0");
	}
	else
	{
		if (param[0] == "������Ʒ")
		{
			if (param.size() < 6)
			{
				DebugMessage("setitem-������Ʒ �Ĳ�����������");
			}
			else
			{
				DebugMessageA("ִ��������Ʒ��������Ʒ��");
				std::string listType = param[1];
				if (listType == "������")
				{
					std::string listBegin = param[2];
					std::string listEnd = param[3];
					std::string countRemain = param[4];
					std::string name = param[5];
					TAG_OF_TEMP;
					DebugMessageA("��ʼ���� = %s,�������� = %s,�������� = %s,˵�� = %s",listBegin.c_str(),listEnd.c_str(),countRemain.c_str(),name.c_str());
				}
			}
		}
	}
}

void VOID_PROXY_CALL setHero(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("setHero �������󣬲����Ĵ�СΪ0");
	}
	else
	{
		if (param[0] == "�Զ�����")
		{
			if (param.size() < 2)
			{
				DebugMessage("setHero���Զ�����������󣬲����Ĵ�СС��2");
			}else{
				TAG_OF_TEMP;
				if (param[1] == "�ر�")
				{
					DebugMessage("ִ������-�ر��Զ�����");
				}else{
					DebugMessage("ִ������ - �����Զ�����");
				}
			}
		}
		else if (param[0] == "����")
		{
			if (param.size() < 3)
			{
				DebugMessage("setHero�����ơ� �������󣬲�������");
			}
			else
			{
				TAG_OF_TEMP;
				if (param[1] == "1")
				{
					DebugMessage("��������");
				}
				else
				{
					DebugMessage("�ر�����");
				}
			}
		}
		else if (param[0] == "�Զ�ѭ��")
		{
			if (param.size() < 3)
			{
				DebugMessage("setHero���Զ�ѭ���� �������󣬲�������");
			}
			else
			{
				//�������� [  �Զ�ѭ�� �ر� 0 ]
				TAG_OF_TEMP;
				if (param[1] == "����")
				{
					DebugMessage("�����Զ�ѭ��");
				}
				else
				{
					DebugMessage("�ر��Զ�ѭ��");
				}
			}
		}
		else if (param[0] == "�Զ���ֿ�")
		{
			if (param.size() < 2)
			{
				DebugMessage("setHero���Զ���ֿ⡿ �������󣬲�������");
			}
			else{
				TAG_OF_TEMP;
				if (param[1] == "1")
				{
					DebugMessage("�����Զ���ֿ�");
				}
				else
				{
					DebugMessage("�ر��Զ���ֿ�");
				}
			}
		}
	}
}

void VOID_PROXY_CALL petBattle(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("petBattle �������󣬲����Ĵ�СΪ0");
	}
	else
	{
		if (param[0] == "ս����")
		{
			if (param.size() < 3)
			{
				DebugMessage("petBattleս���յĲ������󣬲���̫�١�");
			}
			else
			{
				TAG_OF_TEMP;
				if (param[1] == "ȫ���ٻ�")
				{
					DebugMessage("ִ�����ȫ���ٻ�ս��");
				}
				else if (param[1] == "��ս")
				{
					DebugMessage("ִ�����ȫ������ս�衣");
				}
				else if (param[1] == "�Զ�����")
				{
					DebugMessage("ִ�����ȫ���Զ��������");
				}
			}
			DebugMessage("petBattle ��սս�衣");
		}
		else if (param[0] == "��ʦ���")
		{
			if (param.size() < 3)
			{
				DebugMessage("petBattleս���յĲ������󣬲���̫�١�");
			}
			else
			{
				TAG_OF_TEMP;
				if (param[1] == "ȫ���ٻ�")
				{
					DebugMessage("ִ�����ȫ���ٻط���");
				}
				else if (param[1] == "��ս")
				{
					DebugMessage("ִ�����ȫ���������衣");
				}
			}
			DebugMessage("petBattle ��ս���衣");
		}
	}
}

void VOID_PROXY_CALL useItem(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("useItem �������󣬲����Ĵ�СΪ0");
	}
	else
	{
		if (param[0] == "�ǳ�֮��")
		{
			if (param.size() < 5)
			{
				DebugMessage("useItem �ǳ�֮�۵Ĳ�������");
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("useItem �ǳ�֮�� Type=%s,Offset=%s,X=%s,Y=%s",param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
			}
		}
	}
}

void VOID_PROXY_CALL MoveTo(std::vector<std::string>* pParams)
{
	//�ߵ���ͼ [1000, 307, 468, 0, 1]
	std::vector<std::string> param = *pParams;
	if (param.size() < 5)
	{
		DebugMessage("MoveTo �������󣬲����Ĵ�СΪ0");
	}
	else
	{
		TAG_OF_TEMP;
		DebugMessage("MoveTo MAPID=%s,destX=%s,destY=%s,Unknow0=%s,UnKnow1=%s",param[0].c_str(),param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
	}
}

void VOID_PROXY_CALL SaveVip(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("SaveVip �������󣬲����Ĵ�СΪ0");
	}
	else{
		TAG_OF_TEMP;
		DebugMessage("save vip,p1=%s,p2=%s,p3=%s",param[0].c_str(),param[1].c_str(),param[2].c_str());
	}
}
void VOID_PROXY_CALL OpenNPC(std::vector<std::string>* pParams)
{
	//��NPC [ ������ 0 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("OpenNPC �������󣬲����Ĵ�С<3");
	}
	else{
		TAG_OF_TEMP;
		DebugMessage("OpenNPC,p1=%s,p2=%s,p3=%s",param[0].c_str(),param[1].c_str(),param[2].c_str());
	}
}

void VOID_PROXY_CALL SelTwoMenu(std::vector<std::string>* pParams)
{
	//ѡ��˵� [ 0 0 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("SelTwoMenu SelTwoMenu�������Ĵ�С<3");
	}
	else
	{
		TAG_OF_TEMP;
		DebugMessage("SelTwoMenu p1=%s,p2=%s,p3=%s",param[0].c_str(),param[1].c_str(),param[2].c_str());
	}
}

void VOID_PROXY_CALL SellItem(std::vector<std::string>* pParams)
{
	//���ۻ�Ʒ [ �������� 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 2)
	{
		DebugMessage("SelTwoMenu SelTwoMenu�������Ĵ�С<3");
	}
	else
	{
		if (param[0] == "��������")
		{
			TAG_OF_TEMP;
			DebugMessage("SellItem[��������],p2=%s",param[1].c_str());
		}
		else
		{
			TAG_OF_TEMP;
			DebugMessage("SellItem [?]p1=%s,p2=%s",param[0].c_str(),param[1].c_str());
		}
	}
}

void VOID_PROXY_CALL CheckOutItem(std::vector<std::string>* pParams)
{
	//�ֿ���ȡ [ ���� 1000000 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("CheckOutItem�������Ĵ�С<3");
	}
	else
	{
		if (param[0] == "����")
		{
			TAG_OF_TEMP;
			DebugMessage("CheckOutItem,�Ӳֿ���ȡǮ p1=%s,p2=%s",param[1].c_str(),param[2].c_str());
		}
		else if (param[0] == "��Ʒ")
		{
			TAG_OF_TEMP;
			DebugMessage("CheckOutItem,�Ӳֿ���ȡ��Ʒ ����=%s,����=%s",param[1].c_str(),param[2].c_str());
		}
		else
		{
			TAG_OF_TEMP;
			DebugMessage("[����]CheckOutItem δ֪�Ĳ������� p0=%s,p1=%s,p2=%s",param[0].c_str(),param[1].c_str(),param[2].c_str());
		}
	}
}

void VOID_PROXY_CALL RepairEquip(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("RepairEquip�������Ĵ�С<3");
	}
	else
	{
		if (param[0] == "���е�")
		{
			TAG_OF_TEMP;
			DebugMessage("��������װ�� p1=%s,p2=%s",param[1].c_str(),param[2].c_str());
		}
		else
		{
			TAG_OF_TEMP;
			DebugMessage("δ֪����װ������");
		}
	}
}

void VOID_PROXY_CALL ClearBag(std::vector<std::string>* pParams)
{
	////��ձ��� [ 0 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 2)
	{
		DebugMessage("ClearBag�������Ĵ�С<2");
	}
	else
	{
		TAG_OF_TEMP;
		DebugMessage("��ձ�����p0=%s,p1=%s",param[0].c_str(),param[1].c_str());
	}
}

void VOID_PROXY_CALL CloseFaBaoBand(std::vector<std::string>* pParams)
{
	//�رշ����� [ ��9X���� 0 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("CloseFaBaoBand�������Ĵ�С<2");
	}
	else
	{
		TAG_OF_TEMP;
		DebugMessage("�رշ�����,p0=%s,p1=%s",param[0].c_str(),param[1].c_str());
	}
}

void VOID_PROXY_CALL CheckBagItem(std::vector<std::string>* pParams)
{
	////������Ʒ [ ����Ʒ Ѫ�� 0x000b5593 8 ]
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("CheckBagItem ��������");
	}
	else
	{
		if (param[0] == "����Ʒ")
		{
			DebugMessage("CheckBagItem ����Ʒ");
		}
	}
}

//
void VOID_PROXY_CALL HeroBattleStay(std::vector<std::string>* pParams)
{
	////������Ʒ [ ����Ʒ Ѫ�� 0x000b5593 8 ]
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("HeroBattleStay ��������");
	}

}
int itpcore::init_call_table()
{

	//�ж���Ʒ �ϳɻ�ħ֮�� 0x000fd372 >= 15 1
	callmap_internal.addcall("�ж���Ʒ",funtype_c,&jugeItem,NULL);
	//[�жϵ�ͼ 1 0x2261]
	callmap_internal.addcall("�жϵ�ͼ",funtype_c,&jugeMap,NULL);
	//��� [�ж����� SP��ֵ <= 80 0] ��ô ������� [ 9X��ʼ_SP���� ]
	callmap_internal.addcall("�ж�����",funtype_c,&jugeHero,NULL);
	//---------------------------������24K�ѺϽ�ָ���--------------------------------
	callmap_internal.addcall("�ȴ�",funtype_c,&sc_sleep,NULL);
	callmap_internal.addcall("�������",funtype_c,&team_op,NULL);
	callmap_internal.addcall("������Ʒ",funtype_c,&setItems,NULL);
	callmap_internal.addcall("��������",funtype_c,&setHero,NULL);
	//�����ս [ ս���� ȫ���ٻ� 500 0 0 0 ]
	callmap_internal.addcall("�����ս",funtype_c,&petBattle,NULL);
	//ʹ����Ʒ  [ �ǳ�֮�� 0x000c8384  0x03E8  292  466 ]
	callmap_internal.addcall("ʹ����Ʒ",funtype_c,&useItem,NULL);
	//�ߵ���ͼ [1000, 307, 468, 0, 1]
	callmap_internal.addcall("�ߵ���ͼ",funtype_c,&MoveTo,NULL);
	//�ֿⱣ�� [ 0 0 0 ]
	callmap_internal.addcall("�ֿⱣ��",funtype_c,&SaveVip,NULL);
	//��NPC [ ������ 0 0 ]
	callmap_internal.addcall("��NPC",funtype_c,&OpenNPC,NULL);
	//ѡ��˵� [ 0 0 0 ]
	callmap_internal.addcall("ѡ��˵�",funtype_c,&SelTwoMenu,NULL);
	//���ۻ�Ʒ [ �������� 0 ]
	callmap_internal.addcall("���ۻ�Ʒ",funtype_c,&SellItem,NULL);
	//�ֿ���ȡ [ ���� 1000000 0 ]
	callmap_internal.addcall("�ֿ���ȡ",funtype_c,&CheckOutItem,NULL);
	//����װ�� [ ���е� 0 0 ]
	callmap_internal.addcall("����װ��",funtype_c,&RepairEquip,NULL);
	//��ձ��� [ 0 0 ]
	callmap_internal.addcall("��ձ���",funtype_c,&ClearBag,NULL);
	//�رշ����� [ ��9X���� 0 0 ]
	callmap_internal.addcall("�رշ�����",funtype_c,&CloseFaBaoBand,NULL);
	//������Ʒ [ ����Ʒ Ѫ�� 0x000b5593 8 ]
	callmap_internal.addcall("������Ʒ",funtype_c,&CheckBagItem,NULL);
	//����1-8:δ֪��
	//����9:Ŀ�����X������10:Ŀ�����Y
	//����10-14 δ֪
	//����15 ������
	//����N ʹ�ü���
	//����N+1 ʹ�ü��ܺ��ӳ١�
	//ԭ��ս�� [ 1 1 8 1 0 0 0 0 0 0 0 0 0 0 â�������� Ѫ��籩 1400 �޼��� 100 �޼��� 100 �޼��� 100]
	//ԭ��ս�� [ 1 0 100 0 0 1 3 793 933 0 0 0 0 0 ����ħȮ ѪӰ��â 5800 �޼��� 100 �޼��� 100 �޼��� 100 ]
	//ԭ��ս�� [ 1 1 6 0 0 60 0 0 0 0 0 0 0 0 ����͵Ϯ�� ������ 200 ������ 200 Ѫ��籩 1400 �޼��� 1400 ]
	//ԭ��ս�� [ 1 0 100 0 0 2 2 4500000 737 0 0 60000 75 0 ���澫Ӣ��ʿ ѪϮ 300 ѪӰ��â 180 �޼��� 100 �޼��� 100 ] 
	callmap_internal.addcall("ԭ��ս��",funtype_c,&HeroBattleStay,NULL);
	return 0;
}
