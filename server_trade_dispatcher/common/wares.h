#pragma once


struct goods_character_table
{
	int character_type;
	union{
		char character_c[32];
		int character_i[8];
		short character_s[16];
	}character_value;
};



struct goods_character 
{
	int character_count;
	goods_character_table character_table[16];
};


struct Wares
{
	char _goods_name[32];
	int _id;
	goods_character _character;//物品属性表
	int _sell_value;
};

