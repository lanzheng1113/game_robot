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

//判断地图
bool BOOL_PROXY_CALL jugeMap(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() < 2)
	{
		DebugMessage("jugeMap 函数错误，参数的数量小于2");
		return false;
	}
	else
	{
		TAG_OF_TEMP;
		DebugMessage("jugeMap 目标地图ID=%s,未知参=%s",param[0].c_str(),param[1].c_str());
		return (rand() % 2 == 0);
	}
}

bool BOOL_PROXY_CALL jugeHero(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (0 == param.size())
	{
		DebugMessage("jugeHero 函数错误，参数数量为0");
		return false;
	}
	else
	{
		//如果 [判断人物 SP差值 <= 80 0] 那么 跳到标记 [ 9X开始_SP补满 ]
		if (param[0] == "SP差值")
		{
			if (param.size() < 4)
			{
				DebugMessage("jugeHero 函数错误，参数数量小于4");
				return false;
			}
			else{
				TAG_OF_TEMP;
				DebugMessage("jugeHero SP差值");
				return (rand()%2 == 0);
			}
		}
		else{
			CMessageBoxA("未知的参数 %s",param[0].c_str());
			return false;
		}
	}
}
bool BOOL_PROXY_CALL jugeItem(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("jugeItem 函数错误，参数的大小为0");
		return false;
	}
	else
	{
		if (param[0] == "合成幻魔之心")
		{
			if (param.size() < 5 )
			{
				DebugMessage("jugeItem 合成幻魔之心的参数错误");
				return false;
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("jugeItem 合成幻魔之心,类型=%s,判断子=%s,数量=%s,未知参=%s",param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
				return (rand()%2 == 0);
			}
		}
		else if (param[0] == "合成灵魂")
		{
			if (param.size() < 5)
			{
				DebugMessage("jugeItem 合成灵魂的参数错误");
				return false;
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("jugeItem 合成灵魂,类型=%s,判断子=%s,数量=%s,未知参=%s",param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
				return (rand()%2 == 0);
			}
		}
		else if (param[0] == "幻魔晶石")
		{
			if (param.size() < 5)
			{
				DebugMessage("jugeItem 幻魔晶石的参数错误");
				return false;
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("jugeItem 幻魔晶石,类型 = %s,判断子=%s,数量=%s,未知参=%s",param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
				return (rand()%2==0);
			}
		}
		else if (param[0] == "灵魂晶石")
		{
			if (param.size() < 5)
			{
				DebugMessage("jugeItem 灵魂晶石的参数错误");
				return false;
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("jugeItem 灵魂晶石,类型 = %s,判断子=%s,数量=%s,未知参=%s",param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
				return (rand()%2==0);
			}
		}
		else 
		{
			TAG_OF_TEMP;
			DebugMessage("jugeItem 未知的参数");
			return false;
		}
	}
}

void VOID_PROXY_CALL sc_sleep(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("sc_sleep 函数错误，参数的大小为0");
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
		DebugMessage("sc_sleep 函数错误，参数的大小为0");
	}
	else
	{
		if (param[0] == "获得队友信息")
		{
			TAG_OF_TEMP;
			DebugMessage("队伍操作 - 获得队伍信息"); 
			return;
		}
		else if (param[0] == "谁能进副本")
		{
			TAG_OF_TEMP;
			DebugMessage("队伍操作- 谁能进副本");
		}
		else if (param[0] == "邀请队友")
		{
			TAG_OF_TEMP;
			DebugMessage("队伍操作 - 邀请队友");
		}
		else if (param[0] == "跳转")
		{
			//队伍操作 [ 邀请队友 0 0 0 0 0 0] 
			//队伍操作 [ 跳转 31 9X副本-队员进入 0 0 0 0] 
			TAG_OF_TEMP;
			DebugMessage("队伍操作 - 跳转");
		}
		else if (param[0] == "异能者")
		{
			//队伍操作 [ 异能者 血包 5 0 0 0 0] 
			if (param.size() < 3)
			{
				CMessageBoxA("[错误]队伍操作-异能者，参数过少");
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("队伍操作 异能者 血包");
			}
		}
		else if (param[0] == "复活宠物")
		{
			TAG_OF_TEMP;
			DebugMessage("队伍操作 复活宠物");
			////队伍操作 [ 复活宠物 战神金刚 1 法师佩恩 2 0 0 ]
		}
		else{
			TAG_OF_TEMP;
			CMessageBoxA("[错误]队伍操作-未知的命令",param[0].c_str());
		}
	}
}

void VOID_PROXY_CALL setItems(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("setItems 函数错误，参数的大小为0");
	}
	else
	{
		if (param[0] == "地面物品")
		{
			if (param.size() < 6)
			{
				DebugMessage("setitem-地面物品 的参数个数有误");
			}
			else
			{
				DebugMessageA("执行设置物品【地面物品】");
				std::string listType = param[1];
				if (listType == "黑名单")
				{
					std::string listBegin = param[2];
					std::string listEnd = param[3];
					std::string countRemain = param[4];
					std::string name = param[5];
					TAG_OF_TEMP;
					DebugMessageA("开始类型 = %s,结束类型 = %s,保留个数 = %s,说明 = %s",listBegin.c_str(),listEnd.c_str(),countRemain.c_str(),name.c_str());
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
		DebugMessage("setHero 函数错误，参数的大小为0");
	}
	else
	{
		if (param[0] == "自动复活")
		{
			if (param.size() < 2)
			{
				DebugMessage("setHero【自动复活】函数错误，参数的大小小于2");
			}else{
				TAG_OF_TEMP;
				if (param[1] == "关闭")
				{
					DebugMessage("执行命令-关闭自动复活");
				}else{
					DebugMessage("执行命令 - 开启自动复活");
				}
			}
		}
		else if (param[0] == "治疗")
		{
			if (param.size() < 3)
			{
				DebugMessage("setHero【治疗】 函数错误，参数过少");
			}
			else
			{
				TAG_OF_TEMP;
				if (param[1] == "1")
				{
					DebugMessage("开启治疗");
				}
				else
				{
					DebugMessage("关闭治疗");
				}
			}
		}
		else if (param[0] == "自动循环")
		{
			if (param.size() < 3)
			{
				DebugMessage("setHero【自动循环】 函数错误，参数过少");
			}
			else
			{
				//设置人物 [  自动循环 关闭 0 ]
				TAG_OF_TEMP;
				if (param[1] == "开启")
				{
					DebugMessage("开启自动循环");
				}
				else
				{
					DebugMessage("关闭自动循环");
				}
			}
		}
		else if (param[0] == "自动存仓库")
		{
			if (param.size() < 2)
			{
				DebugMessage("setHero【自动存仓库】 函数错误，参数过少");
			}
			else{
				TAG_OF_TEMP;
				if (param[1] == "1")
				{
					DebugMessage("开启自动存仓库");
				}
				else
				{
					DebugMessage("关闭自动存仓库");
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
		DebugMessage("petBattle 函数错误，参数的大小为0");
	}
	else
	{
		if (param[0] == "战神金刚")
		{
			if (param.size() < 3)
			{
				DebugMessage("petBattle战神金刚的参数有误，参数太少。");
			}
			else
			{
				TAG_OF_TEMP;
				if (param[1] == "全部召回")
				{
					DebugMessage("执行命令，全部召回战宠");
				}
				else if (param[1] == "出战")
				{
					DebugMessage("执行命令，全部出征战宠。");
				}
				else if (param[1] == "自动合体")
				{
					DebugMessage("执行命令，全部自动合体幻兽");
				}
			}
			DebugMessage("petBattle 出战战宠。");
		}
		else if (param[0] == "法师佩恩")
		{
			if (param.size() < 3)
			{
				DebugMessage("petBattle战神金刚的参数有误，参数太少。");
			}
			else
			{
				TAG_OF_TEMP;
				if (param[1] == "全部召回")
				{
					DebugMessage("执行命令，全部召回法宠");
				}
				else if (param[1] == "出战")
				{
					DebugMessage("执行命令，全部出征法宠。");
				}
			}
			DebugMessage("petBattle 出战法宠。");
		}
	}
}

void VOID_PROXY_CALL useItem(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("useItem 函数错误，参数的大小为0");
	}
	else
	{
		if (param[0] == "星辰之眼")
		{
			if (param.size() < 5)
			{
				DebugMessage("useItem 星辰之眼的参数错误");
			}
			else
			{
				TAG_OF_TEMP;
				DebugMessage("useItem 星辰之眼 Type=%s,Offset=%s,X=%s,Y=%s",param[1].c_str(),param[2].c_str(),param[3].c_str(),param[4].c_str());
			}
		}
	}
}

void VOID_PROXY_CALL MoveTo(std::vector<std::string>* pParams)
{
	//走到地图 [1000, 307, 468, 0, 1]
	std::vector<std::string> param = *pParams;
	if (param.size() < 5)
	{
		DebugMessage("MoveTo 函数错误，参数的大小为0");
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
		DebugMessage("SaveVip 函数错误，参数的大小为0");
	}
	else{
		TAG_OF_TEMP;
		DebugMessage("save vip,p1=%s,p2=%s,p3=%s",param[0].c_str(),param[1].c_str(),param[2].c_str());
	}
}
void VOID_PROXY_CALL OpenNPC(std::vector<std::string>* pParams)
{
	//打开NPC [ 海伦娜 0 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("OpenNPC 函数错误，参数的大小<3");
	}
	else{
		TAG_OF_TEMP;
		DebugMessage("OpenNPC,p1=%s,p2=%s,p3=%s",param[0].c_str(),param[1].c_str(),param[2].c_str());
	}
}

void VOID_PROXY_CALL SelTwoMenu(std::vector<std::string>* pParams)
{
	//选择菜单 [ 0 0 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("SelTwoMenu SelTwoMenu，参数的大小<3");
	}
	else
	{
		TAG_OF_TEMP;
		DebugMessage("SelTwoMenu p1=%s,p2=%s,p3=%s",param[0].c_str(),param[1].c_str(),param[2].c_str());
	}
}

void VOID_PROXY_CALL SellItem(std::vector<std::string>* pParams)
{
	//出售货品 [ 所有垃圾 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 2)
	{
		DebugMessage("SelTwoMenu SelTwoMenu，参数的大小<3");
	}
	else
	{
		if (param[0] == "所有垃圾")
		{
			TAG_OF_TEMP;
			DebugMessage("SellItem[所有垃圾],p2=%s",param[1].c_str());
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
	//仓库提取 [ 银两 1000000 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("CheckOutItem，参数的大小<3");
	}
	else
	{
		if (param[0] == "银两")
		{
			TAG_OF_TEMP;
			DebugMessage("CheckOutItem,从仓库提取钱 p1=%s,p2=%s",param[1].c_str(),param[2].c_str());
		}
		else if (param[0] == "物品")
		{
			TAG_OF_TEMP;
			DebugMessage("CheckOutItem,从仓库提取物品 类型=%s,数量=%s",param[1].c_str(),param[2].c_str());
		}
		else
		{
			TAG_OF_TEMP;
			DebugMessage("[错误]CheckOutItem 未知的参数类型 p0=%s,p1=%s,p2=%s",param[0].c_str(),param[1].c_str(),param[2].c_str());
		}
	}
}

void VOID_PROXY_CALL RepairEquip(std::vector<std::string>* pParams)
{
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("RepairEquip，参数的大小<3");
	}
	else
	{
		if (param[0] == "所有的")
		{
			TAG_OF_TEMP;
			DebugMessage("修理所有装备 p1=%s,p2=%s",param[1].c_str(),param[2].c_str());
		}
		else
		{
			TAG_OF_TEMP;
			DebugMessage("未知修理装备命令");
		}
	}
}

void VOID_PROXY_CALL ClearBag(std::vector<std::string>* pParams)
{
	////清空背包 [ 0 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 2)
	{
		DebugMessage("ClearBag，参数的大小<2");
	}
	else
	{
		TAG_OF_TEMP;
		DebugMessage("清空背包，p0=%s,p1=%s",param[0].c_str(),param[1].c_str());
	}
}

void VOID_PROXY_CALL CloseFaBaoBand(std::vector<std::string>* pParams)
{
	//关闭法宝绑定 [ 进9X副本 0 0 ]
	std::vector<std::string> param = *pParams;
	if (param.size() < 3)
	{
		DebugMessage("CloseFaBaoBand，参数的大小<2");
	}
	else
	{
		TAG_OF_TEMP;
		DebugMessage("关闭法宝绑定,p0=%s,p1=%s",param[0].c_str(),param[1].c_str());
	}
}

void VOID_PROXY_CALL CheckBagItem(std::vector<std::string>* pParams)
{
	////背包物品 [ 扔物品 血包 0x000b5593 8 ]
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("CheckBagItem 参数过少");
	}
	else
	{
		if (param[0] == "扔物品")
		{
			DebugMessage("CheckBagItem 丢物品");
		}
	}
}

//
void VOID_PROXY_CALL HeroBattleStay(std::vector<std::string>* pParams)
{
	////背包物品 [ 扔物品 血包 0x000b5593 8 ]
	std::vector<std::string> param = *pParams;
	if (param.size() == 0)
	{
		DebugMessage("HeroBattleStay 参数过少");
	}

}
int itpcore::init_call_table()
{

	//判断物品 合成幻魔之心 0x000fd372 >= 15 1
	callmap_internal.addcall("判断物品",funtype_c,&jugeItem,NULL);
	//[判断地图 1 0x2261]
	callmap_internal.addcall("判断地图",funtype_c,&jugeMap,NULL);
	//如果 [判断人物 SP差值 <= 80 0] 那么 跳到标记 [ 9X开始_SP补满 ]
	callmap_internal.addcall("判断人物",funtype_c,&jugeHero,NULL);
	//---------------------------华丽的24K钛合金分割线--------------------------------
	callmap_internal.addcall("等待",funtype_c,&sc_sleep,NULL);
	callmap_internal.addcall("队伍操作",funtype_c,&team_op,NULL);
	callmap_internal.addcall("设置物品",funtype_c,&setItems,NULL);
	callmap_internal.addcall("设置人物",funtype_c,&setHero,NULL);
	//宠物出战 [ 战神金刚 全部召回 500 0 0 0 ]
	callmap_internal.addcall("宠物出战",funtype_c,&petBattle,NULL);
	//使用物品  [ 星辰之眼 0x000c8384  0x03E8  292  466 ]
	callmap_internal.addcall("使用物品",funtype_c,&useItem,NULL);
	//走到地图 [1000, 307, 468, 0, 1]
	callmap_internal.addcall("走到地图",funtype_c,&MoveTo,NULL);
	//仓库保存 [ 0 0 0 ]
	callmap_internal.addcall("仓库保存",funtype_c,&SaveVip,NULL);
	//打开NPC [ 海伦娜 0 0 ]
	callmap_internal.addcall("打开NPC",funtype_c,&OpenNPC,NULL);
	//选择菜单 [ 0 0 0 ]
	callmap_internal.addcall("选择菜单",funtype_c,&SelTwoMenu,NULL);
	//出售货品 [ 所有垃圾 0 ]
	callmap_internal.addcall("出售货品",funtype_c,&SellItem,NULL);
	//仓库提取 [ 银两 1000000 0 ]
	callmap_internal.addcall("仓库提取",funtype_c,&CheckOutItem,NULL);
	//修理装备 [ 所有的 0 0 ]
	callmap_internal.addcall("修理装备",funtype_c,&RepairEquip,NULL);
	//清空背包 [ 0 0 ]
	callmap_internal.addcall("清空背包",funtype_c,&ClearBag,NULL);
	//关闭法宝绑定 [ 进9X副本 0 0 ]
	callmap_internal.addcall("关闭法宝绑定",funtype_c,&CloseFaBaoBand,NULL);
	//背包物品 [ 扔物品 血包 0x000b5593 8 ]
	callmap_internal.addcall("背包物品",funtype_c,&CheckBagItem,NULL);
	//参数1-8:未知，
	//参数9:目标对象X，参数10:目标对象Y
	//参数10-14 未知
	//参数15 对象名
	//参数N 使用技能
	//参数N+1 使用技能后延迟。
	//原地战斗 [ 1 1 8 1 0 0 0 0 0 0 0 0 0 0 芒硝爆破者 血域风暴 1400 无技能 100 无技能 100 无技能 100]
	//原地战斗 [ 1 0 100 0 0 1 3 793 933 0 0 0 0 0 地狱魔犬 血影星芒 5800 无技能 100 无技能 100 无技能 100 ]
	//原地战斗 [ 1 1 6 0 0 60 0 0 0 0 0 0 0 0 炎刃偷袭者 破灵咒 200 爆雷术 200 血域风暴 1400 无技能 1400 ]
	//原地战斗 [ 1 0 100 0 0 2 2 4500000 737 0 0 60000 75 0 烈焰精英骑士 血袭 300 血影星芒 180 无技能 100 无技能 100 ] 
	callmap_internal.addcall("原地战斗",funtype_c,&HeroBattleStay,NULL);
	return 0;
}
