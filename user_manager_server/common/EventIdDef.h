#ifndef _EVENT_ID_DEF_H
#define _EVENT_ID_DEF_H
													//�¼����ID
#define Event_caledge_runtime_error				0x07		//���������ڳ����쳣��׽
#define Event_caledge_DoValidate_fail			0x08		//��֤ʧ����Ϣ
#define Event_caledge_commom					0x09		//��ͨ��Ϣ

													//�¼�ID
#define Eid_Client_SystemInfo_is_NULL			0x00c1		//�û�������ϢΪ��
#define Eid_3Second_Login_TimeOut				0x00c2		//��¼ʱ3��Υ��
#define Eid_UserLoginSuccess					0x00c3		//�û���¼�ɹ�
#define Eid_DoValidate_SystemInfo_Incorrect		0x00c4		//�󶨴���
//#define Eid_DoValidate_1st_UserCrash			0x00c5		//�û�ִ��thdini����
#define Eid_DoValidate_2nd_UserCrash			0x00c6		//�û�ִ��getidentify����
//#define Eid_DoValidate_3rd_UserCrash			0x00c7		//�û�ִ�е���������ƫ��ʱ��������
#define Eid_UserAlreayOnline					0x00c8

#endif