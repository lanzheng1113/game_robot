#ifndef _CONST_DEFINE_H
#define _CONST_DEFINE_H

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if(p){delete(p);(p)=NULL;}}
#endif


//const DWORD const_AccTypeYueKa = 1;
//const DWORD const_AccTypeZhouKa = 2;
//const DWORD const_AccTypeCeShi = 3;
#define GameType_DNF 0x1FFFF
typedef enum{
	AccTypeUnknow,
	AccTypeYueKa = 1,
	AccTypeZhouKa,
	AccTypeCeShi
} enum_Acctype;

#endif