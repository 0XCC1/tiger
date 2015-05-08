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
			Ty_ty ty;//变量类型
		} var;
		struct 
		{
			Ty_tyList formals  ;//形参
			Ty_ty result;//返回值
		} fun;
	} u;
	
};

E_enventry E_VarEntry(Ty_ty);
E_enventry E_FunEntry(Ty_tyList formals,Ty_ty result);

S_table E_base_tenv();//类型表
S_table E_base_venv();//变量映射表