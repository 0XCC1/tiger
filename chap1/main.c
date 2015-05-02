#include "util.h"
#include "slp.h"
#include "prog1.h"
#include "stdio.h"

int maxargs(A_stm stm){
	int result=0;
	if (stm->kind==A_printStm)
	{//打印语句
		result=1;
		
	}else if (stm->kind==A_assignStm)
	{//赋值语句
		result+=maxexpargs(stm->u.assign.exp);
		 
	}else if (stm->kind==A_compoundStm)
	{//复合语句
		result+=maxargs(stm->u.compound.stm1);
		result+=maxargs(stm->u.compound.stm2);

	}

	return result;
}

int maxexpargs(A_exp exp){
	int result=0;
	if (exp->kind==A_eseqExp)
	{
		result+=maxargs(exp->u.eseq.stm);
		result+=maxexpargs(exp->u.eseq.exp);
	}
	return result;
}




int main(int argc ,char **argv){
	
	A_stm stm= prog();
	int r=maxargs(stm);
	printf("%d\n", r);
}