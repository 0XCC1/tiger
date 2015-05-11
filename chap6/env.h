#ifndef ENV_H
#define ENV_H
#include "temp.h"
#include "frame.h"
#include "translate.h"
typedef struct E_enventry_ *E_enventry;
struct E_enventry_
{
	enum 
	{
		E_varEntry,
		E_funEntry
	} kind;
	union{
		struct 
		{
			Tr_access access;
			Ty_ty ty;//变量类型
		} var;
		struct 
		{
			Tr_level level;
			Temp_label label;
			Ty_tyList formals  ;//形参
			Ty_ty result;//返回值
		} fun;
	} u;

};

E_enventry E_VarEntry(Tr_access access,Ty_ty);
E_enventry E_FunEntry(Tr_level level, Temp_label label, Ty_tyList formals,Ty_ty result);

S_table E_base_tenv();//类型表
S_table E_base_venv();//变量映射表

#endif

