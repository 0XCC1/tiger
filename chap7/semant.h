#ifndef SEMANT_H
#define SEMANT_H



//typedef void * Tr_exp;
struct expty
{
	Tr_exp exp;
	Ty_ty ty;
};

//F_fragList SEM_transProg(A_exp exp);
F_fragList SEM_transProg(A_exp exp);
#endif



