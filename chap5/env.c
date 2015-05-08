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
	S_table init_v = S_empty();


	return init_v;
}



E_enventry E_VarEntry(Ty_ty ty){
	E_enventry entry;
	entry=checked_malloc(sizeof(*entry));
	entry->kind=E_varEntry;
	entry->u.var.ty = ty;
	return entry;
}

E_enventry E_FunEntry(Ty_tyList fms, Ty_ty resl) {
	E_enventry entry = checked_malloc(sizeof(*entry));
	entry->kind = E_funEntry;
	entry->u.fun.formals = fms;
	entry->u.fun.result  = resl;
	return entry;
}