
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "absyn.h"
#include "types.h"
#include "translate.h"
#include "env.h"
#include "semant.h"

struct expty expTy(Tr_exp e,Ty_ty t){
	struct expty et;
	et.exp=e;
	et.ty=t;
	return et;
}



static struct 	expty transVar(Tr_level,Tr_exp ,  S_table venv,S_table tenv,A_var v);

static struct 	expty transExp(Tr_level,Tr_exp ,  S_table venv,S_table tenv,A_exp a);
//static void 	transDec(S_table venv,S_table tenv,A_dec d);
static    Tr_exp 	transDec(Tr_level,Tr_exp ,  S_table v, S_table t, A_dec d);

static Ty_ty	transTy(S_table tenv,A_ty a);

static bool 	args_match(Tr_level,Tr_exp, S_table v, S_table tt, A_expList ell, Ty_tyList fll, A_exp fun);
static bool 	ty_match(Ty_ty tt, Ty_ty ee);
static Ty_ty    actual_ty(Ty_ty ty);
static bool 	efields_match(Tr_level,Tr_exp,S_table v, S_table t, Ty_ty tyy, A_exp e);



static Ty_tyList makeFormalTyList(S_table t,A_fieldList fl);
static Ty_fieldList makeFieldTys(S_table t, A_fieldList fs);
static U_boolList makeFormals(A_fieldList);

F_fragList SEM_transProg(A_exp exp){
	struct expty et;

	S_table t = E_base_tenv();
	S_table v = E_base_venv();
	et = transExp(Tr_outermost(),NULL,v, t, exp);
	printf("@this expr return:\n"); /* check the return result (use Ty_ty->kind stand) */
	F_fragList resl = Tr_getResult();
	return resl;
}

static struct expty transVar(Tr_level level,Tr_exp breakk, S_table venv,S_table tenv,A_var v){
	if (!v) {
		return expTy(Tr_noExp(), Ty_Void());
	}
	E_enventry x;
	struct expty et,et2;
	Ty_fieldList fl;
	Tr_exp trans;

	switch(v->kind){
		case A_simpleVar: /*var id*/
			x=S_look(venv,v->u.simple);
			trans = Tr_noExp();
			if (x&&x->kind==E_varEntry){
				trans= Tr_simpleVar(x->u.var.access, level);
				return expTy(trans,actual_ty(x->u.var.ty));
			}else{
				//变量未定义
				EM_error(v->pos, "undefined var %s", S_name(v->u.simple));
				return expTy(trans, Ty_Int());
			}
		break;
		case A_fieldVar:/* a.b   a.b.c*/
			//得到 a  的type 是 record还是 array还是 simplevar
			et=transVar(level,breakk,venv,tenv,v->u.field.var);
			trans = Tr_noExp();
			if (et.ty->kind!=Ty_record)
			{
				EM_error(v->pos, "not a record type");
				return expTy(NULL, Ty_Record(NULL));
			}else{
				int i=0;
				for (fl = et.ty->u.record;fl;fl=fl->tail,i++)
				{
					//地址比较
					if (fl->head->name==v->u.field.sym)
					{
						trans = Tr_fieldVar(et.exp,i);
						return expTy(trans, actual_ty(fl->head->ty));
					}
				}
				EM_error(v->pos, "no such field in record %s", S_name(v->u.field.sym));
			}
			return expTy(trans, Ty_Int());	
		break;
		case A_subscriptVar:/*a[i]*/
			et  = transVar(level,breakk,venv, tenv, v->u.subscript.var);
			trans = Tr_noExp();
			if (et.ty->kind!=Ty_array){
				EM_error(v->pos, "not a array type");
			}else{
				et2=transExp(level,breakk,venv,tenv,v->u.subscript.exp);
				if (et2.ty->kind != Ty_int) {
					EM_error(v->pos, "int required");
				}else{
					trans = Tr_subscriptVar(et.exp, et2.exp);
					return expTy(trans, actual_ty(et.ty->u.array));
				}
			}
			return expTy(trans, Ty_Int());
		break;
	}

}
//here
static struct expty transExp(Tr_level level,Tr_exp breakk,S_table v, S_table t, A_exp e){
	A_oper oper;
	struct expty left,right,final,final2,final3,final4,final5,lo,hi;
	A_expList list;
	A_decList decs;
	E_enventry callinfo;
	Ty_ty recty,arrayty;
	if (!e) { 
		return expTy(Tr_noExp(), Ty_Void()); 
	}

	switch(e->kind){
		case A_varExp:
			return transVar(level,breakk,v,t,e->u.var);
			break;
		case A_nilExp:
			return expTy(Tr_nilExp(),Ty_Nil());
			break;
		case A_callExp:
			callinfo =S_look(v,e->u.call.func);
			A_expList args=NULL;
			Tr_expList argList=NULL;
			for (args=e->u.call.args;args;args=args->tail){
				struct expty arg = transExp(level, breakk, v, t, args->head);
				Tr_expList_prepend(arg.exp, &argList);	
			}

			Tr_exp trans = Tr_noExp();
			if (callinfo&&callinfo->kind==E_funEntry)
			{
				trans = Tr_callExp(callinfo->u.fun.label, callinfo->u.fun.level, level, &argList);
				//检查参数个数、类型匹配
				if (args_match(level, breakk, v, t, e->u.call.args, callinfo->u.fun.formals, e)) {/*check params is matched*/
					if (callinfo->u.fun.result) {
						return expTy(trans, actual_ty(callinfo->u.fun.result));
					} 
				} 
				/*
				if (args_match(level,v,t,e->u.call.args,callinfo->u.fun.formals,e)){
					return expTy(NULL, actual_ty(callinfo->u.fun.result));
				}else{
					return expTy(NULL, Ty_Void());
				}
				*/
			}else {
				EM_error(e->pos, "undefined function %s\n", S_name(e->u.call.func));
			}
			return expTy(trans, Ty_Void());
			break;
		case A_recordExp:
			recty = actual_ty(S_look(t, e->u.record.typ));
			if (!recty) { /*cant find record-type in table tenv*/ 
				EM_error(e->pos, "undefined type %s (debug recordExp)", S_name(e->u.record.typ)); 
			}else{
				if (recty->kind != Ty_record){
					EM_error(e->pos, "%s is not a record type", S_name(e->u.record.typ));	
					return expTy(Tr_noExp(), Ty_Record(NULL));
				}

				if (efields_match(level,breakk,v, t, recty, e)) {/*check record field is matched*/
					Tr_expList l=NULL;
					int n=0;
					A_efieldList el;
					for (el=e->u.record.fields;el;el=el->tail,n++){
						struct expty val = transExp(level, breakk, v, t, el->head->exp);
						Tr_expList_prepend(val.exp,&l);
					}
					
					return expTy(Tr_recordExp(n, l), recty);
				}
			}
			return expTy(Tr_noExp(), Ty_Record(NULL));
			break;
		case A_arrayExp:
			arrayty=actual_ty(S_look(t,e->u.array.typ));
			if (!arrayty) {
				EM_error(e->pos, "undeined array type %s", S_name(e->u.array.typ));
				return expTy(Tr_noExp(), Ty_Array(NULL));
			}
			if (arrayty->kind != Ty_array) {
				EM_error(e->pos, "%s is not a array type", S_name(e->u.array.typ));
				return expTy(Tr_noExp(), Ty_Array(NULL));
			}
			final2 = transExp(level,breakk,v, t, e->u.array.size);//数组大小 表达式
			final3 = transExp(level,breakk,v, t, e->u.array.init);//数组初始化 表达式
			if (final2.ty->kind != Ty_int) {
				EM_error(e->pos, "array size should be int %s", S_name(e->u.array.typ));
			}else if (!ty_match(final3.ty, arrayty->u.array)){
				EM_error(e->pos, "unmatched array type in %s", S_name(e->u.array.typ));
			} else {
				return expTy(Tr_arrayExp(final2.exp, final3.exp), arrayty);
			}
			return expTy(Tr_noExp(), Ty_Int());
			break;
		case A_seqExp:{
			Tr_expList l = NULL;
			list = e->u.seq;
			struct expty seqone;
			if (!list) {
				return expTy(Tr_noExp(), Ty_Void());
			}
			
			/*while (list->tail) {
				seqone= transExp(level,breakk,v, t, list->head);
				Tr_expList_prepend(seqone.exp, &l);
				list = list->tail;
			}
			*/
			for (; list; list = list->tail) {
				seqone = transExp(level, breakk, v, t, list->head);
				Tr_expList_prepend(seqone.exp, &l);
			}

			printf("A_seqExp\n");
			return expTy(Tr_seqExp(l), seqone.ty);
		}
			break;
		case A_whileExp:
			final = transExp(level,breakk,v, t, e->u.whilee.test);
			if (final.ty->kind != Ty_int) {
				EM_error(e->pos, "int required");
			}
			Tr_exp done = Tr_doneExp();
			struct expty body=transExp(level,done,v, t, e->u.whilee.body);
			return expTy(Tr_whileExp(final.exp, body.exp, done), Ty_Void());
			break;
		case A_assignExp:
			final4 = transVar(level,breakk,v, t, e->u.assign.var);
			final5 = transExp(level,breakk,v, t, e->u.assign.exp);
			if (!ty_match(final4.ty, final5.ty)) {
				EM_error(e->pos, "unmatched assign exp");
			}
			return expTy(Tr_assignExp(final4.exp, final5.exp), Ty_Void());
		case A_breakExp:
			if (!breakk) return expTy(Tr_noExp(), Ty_Void());
			return expTy(Tr_breakExp(breakk), Ty_Void());
		case A_forExp:{
			/*
			struct expty lo = transExp(level,v, t, e->u.forr.lo);
			struct expty hi = transExp(level,v, t, e->u.forr.hi);
			struct expty body;

			if (lo.ty != Ty_Int() || hi.ty != Ty_Int()) {
				EM_error(e->pos, "low or high range type is not integer");
			}

			S_beginScope(v);
			transDec(level,v, t, A_VarDec(e->pos, e->u.forr.var, S_Symbol("int"), e->u.forr.lo));
			body = transExp(level,v, t, e->u.forr.body);
			S_endScope(v);
			return expTy(NULL, Ty_Void());
			*/

			EM_error(e->pos, "\nsome one said for is better than while\nmake them unhappy \nahahaha");
			return expTy(Tr_noExp(), Ty_Int());
		}
		break;
		case A_letExp:{
			Tr_expList l = NULL;
			S_beginScope(v);
			S_beginScope(t);
			for (decs=e->u.let.decs;decs;decs=decs->tail){
				//transDec(level,v,t,decs->head);
			
				;
				Tr_expList_prepend(transDec(level, breakk, v, t, decs->head), &l);
			}
			final=transExp(level,breakk,v,t,e->u.let.body);
			Tr_expList_prepend(final.exp, &l);
			S_endScope(t);
			S_endScope(v);
			printf("A_letExp\n");
			return expTy(Tr_seqExp(l), final.ty);;
		}
		break;
		case A_opExp:{
			A_oper oper = e->u.op.oper;
			struct expty left  = transExp(level, breakk, v, t, e->u.op.left); 
			struct expty right = transExp(level, breakk, v, t, e->u.op.right);
			if (0 <= oper && oper < 4) {/* check +,-,*,/ */
				if (left.ty->kind != Ty_int  ){
					EM_error(e->u.op.left->pos, "int or double required(op)");	
				}  else if (left.ty->kind == Ty_int && right.ty->kind == Ty_int) {
					return expTy(Tr_arithExp(oper, left.exp, right.exp), Ty_Int());
				} 
				return expTy(Tr_noExp(), Ty_Int());
			} else if (3 < oper && oper < 10) {
				Tr_exp translation = Tr_noExp();
				if (oper == 4 || oper == 5) {/*check record type can be nil(=, <>)*/
					switch(left.ty->kind) {
					case Ty_int:
					//case Ty_double:/*see is double query like int TODO*/
						if (right.ty->kind == Ty_int  ) translation = Tr_eqExp(oper, left.exp, right.exp);
						else {EM_error(e->u.op.right->pos, "unexpected type in comparsion");}
						break;
					case Ty_string:
						if (ty_match(right.ty, left.ty)) translation = Tr_eqStringExp(oper, left.exp, right.exp);
						else {EM_error(e->u.op.right->pos, "unexpected type in comparsion");}
						break;
					case Ty_array:
						if (ty_match(right.ty, left.ty)) translation = Tr_eqRef(oper, left.exp, right.exp);
						else {EM_error(e->u.op.right->pos, "unexpected type in comparsion");}
						break;
					case Ty_record:
						if (ty_match(right.ty, left.ty) || right.ty->kind == Ty_nil) translation = Tr_eqRef(oper, left.exp, right.exp);
						else {EM_error(e->u.op.right->pos, "unexpected type in comparsion");}
						break;
					default:
						EM_error(e->u.op.right->pos, "unexpected expression in comparsion");
					}
					return expTy(translation, Ty_Int());
				} else {
					switch(left.ty->kind) {
					//case Ty_double:
					case Ty_int:
						if ( right.ty->kind == Ty_int) translation = Tr_relExp(oper, left.exp, right.exp); 
						else {EM_error(e->u.op.right->pos, "unexpected type in comparsion");}
						break;
					case Ty_string:
						if (right.ty->kind == Ty_string) translation = Tr_eqStringExp(oper, left.exp, right.exp);
						else {EM_error(e->u.op.right->pos, "unexpected type in comparsion");}
						break;
					default:
						EM_error(e->u.op.right->pos, "unexpected type in comparsion");
					}
					return expTy(translation, Ty_Int());
				}
			} else {
				assert(0);	
			}
			 
			} 

			break;
		case A_ifExp:
			final = transExp(level,breakk,v, t, e->u.iff.test);
			final2 = transExp(level,breakk,v, t, e->u.iff.then);
			//final3 = {NULL, NULL};
			if (e->u.iff.elsee) { /*no else-part*/
				final3 = transExp(level,breakk,v, t, e->u.iff.elsee);
				if (final.ty->kind != Ty_int){
					EM_error(e->u.iff.test->pos, "int required");
				} else if(!ty_match(final2.ty, final3.ty)) {
					EM_error(e->pos, "if-else sentence must return same type");
				} else { }
			}
			return expTy(Tr_ifExp(final.exp, final2.exp, final3.exp), final2.ty);
		case A_stringExp:
			return expTy(Tr_stringExp(e->u.stringg), Ty_String());
		case A_intExp:
			return expTy(Tr_intExp(e->u.intt), Ty_Int());
			/*
		case A_doubleExp:
			return expTy(NULL, Ty_Double());
			*/
		/*待续*/

		default :
			printf("err in transExp switch\n");
			exit(0);
	}


}

//向符号表里添加各种声明数据
static Tr_exp transDec(Tr_level level, Tr_exp breakk,S_table v, S_table t, A_dec d) {
	struct expty final;
	A_fundec f;
	Ty_ty resTy,namety,isname;
	Ty_tyList formalTys,s;
	A_fieldList l;
	A_nametyList nl;
	A_fundecList fcl;
	E_enventry fun;
	int iscyl,isset;

	Tr_access ac;

	switch(d->kind){
		case A_varDec:
			final=transExp(level,breakk,v,t,d->u.var.init);

			#ifdef F_P
			puts("@@:");
			printf("id: %s\n", S_name(d->u.var.var));
			#endif
			ac=Tr_allocLocal(level,d->u.var.escape);
			if (!d->u.var.typ){
				if (final.ty->kind==Ty_nil||final.ty->kind==Ty_void)
				{
					/*默认int类型了*/
					EM_error(d->pos, "init should not be nil without type in %s", S_name(d->u.var.var));
					S_enter(v, d->u.var.var, E_VarEntry(ac,Ty_Int()));
				}else{
					/*类型推倒*/
					S_enter(v, d->u.var.var, E_VarEntry(ac,final.ty));
				}
			}else{
				resTy = S_look(t, d->u.var.typ);
				if (!resTy){
					EM_error(d->pos, "undifined type %s", S_name(d->u.var.typ));
				}else{
					//声明的类型和实际的类型是否匹配
					if (!ty_match(resTy, final.ty)) {
						EM_error(d->pos, "unmatched type in %s", S_name(d->u.var.typ));
						S_enter(v, d->u.var.var, E_VarEntry(ac,resTy));
					} else {
						S_enter(v, d->u.var.var, E_VarEntry(ac,resTy));
					}
				}
			}
			Tr_assignExp(Tr_simpleVar(ac, level), final.exp);
			break;
		case A_functionDec:
			//所有函数签名先进入 t队列，以便递归使用
			for (fcl=d->u.function;fcl;fcl=fcl->tail)
			{
				if (fcl->head->result)
				{
					//返回值类型
					resTy=S_look(t,fcl->head->result);
					if (!resTy)
					{
						EM_error(fcl->head->pos, "undefined type for return type");
						resTy = Ty_Void();
					}
				}else{
					resTy = Ty_Void();
				}
				formalTys=makeFormalTyList(t, fcl->head->params);
				
				{
					Temp_label funLabel=Temp_newlabel();
					#ifdef F_P
					puts("-------------------");
					printf("function: %s\n", S_name(fcl->head->name));
					#endif
					//Tr_level Tr_newLevel(Tr_level parent, Temp_label name, U_boolList formals)
					Tr_level l = Tr_newLevel(level, funLabel, makeFormals(fcl->head->params));/* create a new level */
					S_enter(v, fcl->head->name, E_FunEntry(l, funLabel,formalTys, resTy));
				}
				
			}

			for (fcl = d->u.function; fcl; fcl = fcl->tail){
				f = fcl->head;
				//开始一个函数新的作用域
				E_enventry funEntry = S_look(v, f->name); /*get fun-info*/
				S_beginScope(v);
				formalTys =funEntry->u.fun.formals; //makeFormalTyList(t, f->params);
				
				/*add params-name to venv*/
				Tr_accessList acls = Tr_formals(funEntry->u.fun.level);
				//形参名入 v队列
				for (l = f->params, s = formalTys; l && s; l = l->tail, s = s->tail) {
					S_enter(v, l->head->name, E_VarEntry(acls->head,s->head));
				}

				//翻译函数体
				final = transExp(funEntry->u.fun.level,breakk,v, t, f->body);
				fun = S_look(v, f->name);
				//最后的返回值和 函数声明的返回类型是否匹配
				if (!ty_match(fun->u.fun.result, final.ty)) {
					EM_error(f->pos, "incorrect return type in function '%s'", S_name(f->name));
				}
				Tr_procEntryExit(funEntry->u.fun.level, final.exp, acls);
				S_endScope(v);
			}
			return Tr_noExp();
		break;
		case A_typeDec:
			for (nl = d->u.type; nl; nl = nl->tail) {
				S_enter(t, nl->head->name, Ty_Name(nl->head->name,NULL));
			} /* add name to tenv, Ty_ty set NULL*/
			iscyl = TRUE;
			for (nl = d->u.type; nl; nl = nl->tail) {
				resTy = transTy(t, nl->head->ty);
				if (iscyl) {
					if (resTy->kind != Ty_name) {
						iscyl = FALSE;
					}
				}
				if (!nl->tail && resTy->kind != Ty_name && isset) {
					/*line num is some bug*/
					EM_error(d->pos,"warning: actual type should declare brefore nameTy type");
				}
				namety = S_look(t, nl->head->name);
				namety->u.name.ty = resTy;
			}
			if (iscyl) EM_error(d->pos,"illegal type cycle: cycle must contain record, array");
			return Tr_noExp();
		break;
		default:
		assert(0);
	}
}

static Ty_ty transTy(S_table tb, A_ty ty) {
	Ty_ty final = NULL;
	Ty_fieldList fieldTys;

	switch (ty->kind) {
	case A_nameTy:
		final = S_look(tb, ty->u.name);
		if (!final) {
			EM_error(ty->pos, "undefined type %s", S_name(ty->u.name));
			return Ty_Int();
		}
		return final;
	case A_recordTy:
		fieldTys = makeFieldTys(tb, ty->u.record);
		return Ty_Record(fieldTys);
	case A_arrayTy:
		final = S_look(tb, ty->u.array);
		if (!final) EM_error(ty->pos, "undefined type %s", S_name(ty->u.array));
		return Ty_Array(final);
	default:
		assert(0);
	}
}

/*
ell 实参
fll 形参
*/
static bool args_match(Tr_level level,Tr_exp breakk,S_table v, S_table tt, A_expList ell, Ty_tyList fll, A_exp fun){

	struct expty t;
	A_expList el=ell;
	Ty_tyList fl=fll;

	while(el&&fl){
		t=transExp(level,breakk,v,tt,el->head);
		if (!ty_match(t.ty,fl->head)){
			EM_error(fun->pos, "unmatched params in function %s\n", S_name(fun->u.call.func));
			return FALSE;
		}
		el=el->tail;
		fl=fl->tail;
	}


	if (el && !fl) {
		//实参太多
		EM_error(fun->pos,"too many params in function %s\n", S_name(fun->u.call.func));
		return FALSE;
	} else if (!el && fl) {
		//实参太少
		EM_error(fun->pos, "less params in function %s\n", S_name(fun->u.call.func));
		return FALSE;
	} else {
		return TRUE;
	}

	return FALSE;
}

static bool ty_match(Ty_ty tt, Ty_ty ee) {
	Ty_ty t = actual_ty(tt);
	Ty_ty e = actual_ty(ee);
	int tk = t->kind;
	int ek = e->kind;
	
	return (((tk == Ty_record || tk == Ty_array) && t == e) ||
			 (tk == Ty_record && ek == Ty_nil) ||
			 (ek == Ty_record && tk == Ty_nil) ||
			 (tk != Ty_record && tk != Ty_array && tk == ek));
}

static bool efields_match(Tr_level level,Tr_exp breakk,S_table v, S_table t, Ty_ty tyy, A_exp e) {
	struct expty et;
	Ty_fieldList ty = tyy->u.record;
	A_efieldList fl = e->u.record.fields;
	while (ty && fl) {
		et = transExp(level,breakk,v, t, fl->head->exp);
		if (!(ty->head->name == fl->head->name) || !ty_match(ty->head->ty, et.ty)){
			EM_error(e->pos, "unmatched name: type in %s", S_name(e->u.record.typ));
			return FALSE;
		}
		ty = ty->tail;
		fl = fl->tail;
	}
	if (ty && !fl) {
		EM_error(e->pos, "less fields in %s", S_name(e->u.record.typ));
		return FALSE;
	} else if (!ty && fl) {
		EM_error(e->pos, "too many field in %s", S_name(e->u.record.typ));
		return FALSE;
	}
	return TRUE;
}

static Ty_tyList makeFormalTyList(S_table t,A_fieldList fl){
	Ty_tyList final=NULL,head =final;
	A_fieldList l=fl;
	Ty_ty ty;

	for(;l;l=l->tail){
		ty=S_look(t,l->head->typ);
		if (!ty){
			EM_error(l->head->pos, "undefined type %s", S_name(l->head->typ));
			ty = Ty_Int();
		}
		if (!final){
			final=Ty_TyList(ty,NULL);
			head=final;
		}else{
			final->tail=Ty_TyList(ty,NULL);
			final=final->tail;
		}
	}
	return head;
}

static Ty_fieldList makeFieldTys(S_table t, A_fieldList fs) {
	A_fieldList f;
	Ty_fieldList tys = NULL, head;
	Ty_ty ty;
	Ty_field tmp;

	for (f = fs; f; f = f->tail) {
		ty = S_look(t, f->head->typ);
		if (!ty) {
			EM_error(f->head->pos, "undefined type %s", S_name(f->head->typ));
		} else {
			tmp = Ty_Field(f->head->name, ty);
			if (tys) {
				tys->tail = Ty_FieldList(tmp, NULL);
				tys = tys->tail;
			} else {
				tys = Ty_FieldList(tmp, NULL);
				head = tys;
			}

		}
	}
	return head;
}

static Ty_ty  actual_ty(Ty_ty ty){

	if (ty&&ty->kind==Ty_name)
	{
		actual_ty(ty->u.name.ty);
	}else {
		return ty;
	}
}

static U_boolList makeFormals(A_fieldList params) {
	/*TODO is escape-var judge*/
	U_boolList head = NULL, tail = NULL;
	A_fieldList p = params;
	for (; p; p = p->tail) {
		if (head) {
			tail->tail = U_BoolList(TRUE, NULL);
			tail = tail->tail;
		} else {
			head = U_BoolList(TRUE, NULL);
			tail = head;
		}
	}
	return head;
}


