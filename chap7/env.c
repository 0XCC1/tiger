#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "types.h"
#include "env.h"



S_table E_base_tenv(void){
	S_table init_t = S_empty();
	S_enter(init_t,S_Symbol("int"),Ty_Int());
	S_enter(init_t, S_Symbol("string"), Ty_String());

	return init_t;
}


S_table E_base_venv(void){
	S_table t = S_empty();

	S_enter(
		t,
		S_Symbol("print"),
		E_FunEntry(Tr_outermost(), Temp_newlabel(), Ty_TyList(Ty_String(), NULL), Ty_Void())
		);
	S_enter(
		t,
		S_Symbol("flush"),
		E_FunEntry(Tr_outermost(), Temp_newlabel(), NULL, Ty_Void())
		);
	S_enter(
		t,
		S_Symbol("getchar"),
		E_FunEntry(Tr_outermost(), Temp_newlabel(), NULL, Ty_String())
		);
	S_enter(
		t,
		S_Symbol("ord"),
		E_FunEntry(Tr_outermost(), Temp_newlabel(), Ty_TyList(Ty_String(), NULL), Ty_Int())
		);
	S_enter(
		t,
		S_Symbol("chr"),
		E_FunEntry(Tr_outermost(), Temp_newlabel(), Ty_TyList(Ty_Int(), NULL), Ty_String())
		);
	S_enter(
		t,
		S_Symbol("size"),
		E_FunEntry(Tr_outermost(), Temp_newlabel(), Ty_TyList(Ty_String(), NULL), Ty_Int())
		);
	S_enter(
		t,
		S_Symbol("substring"),
		E_FunEntry(
		Tr_outermost(), 
		Temp_newlabel(), 
		Ty_TyList(Ty_String(), Ty_TyList(Ty_Int(), Ty_TyList(Ty_Int(), NULL))),
		Ty_String())
		);
	S_enter(
		t,
		S_Symbol("concat"),
		E_FunEntry(
		Tr_outermost(), 
		Temp_newlabel(), 
		Ty_TyList(Ty_String(), Ty_TyList(Ty_String(), NULL)),
		Ty_String())
		);
	S_enter(
		t,
		S_Symbol("not"),
		E_FunEntry(Tr_outermost(), Temp_newlabel(), Ty_TyList(Ty_Int(), NULL), Ty_Int())
		);
	S_enter(
		t,
		S_Symbol("exit"),
		E_FunEntry(Tr_outermost(), Temp_newlabel(), Ty_TyList(Ty_Int(), NULL), Ty_Void())
		);
	return t;
}



E_enventry E_VarEntry(Tr_access access,Ty_ty ty){
	E_enventry entry;
	entry=checked_malloc(sizeof(*entry));
	entry->kind=E_varEntry;
	entry->u.var.ty = ty;
	entry->u.var.access=access;
	return entry;
}

E_enventry E_FunEntry(Tr_level level, Temp_label label,Ty_tyList fms, Ty_ty resl) {
	E_enventry entry = checked_malloc(sizeof(*entry));
	entry->kind = E_funEntry;
	entry->u.fun.formals = fms;
	entry->u.fun.result  = resl;
	entry->u.fun.level=level;
	entry->u.fun.label=label;
	return entry;
}