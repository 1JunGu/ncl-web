#include <stdio.h>
#include <ncarg/hlu/hlu.h>
#include <ncarg/hlu/NresDB.h>
#include <ncarg/ncl/defs.h>
#include <ncarg/ncl/NclDataDefs.h>
#include "WSymbol.h"
#include "Keywords.h"
#include "fstrings.h"

#ifdef YYDEBUG
int yydebug = 1;
#endif

char *wrapname;

WWrapRecList *wrap_list = NULL;
WWrapRec *current = NULL;
FILE *thefile;

WSrcList *WNewAdditionalSrc(char *b,int order)
{
	WSrcList *new = malloc(sizeof(WSrcList));
	new->src = malloc(strlen(b)+1);
	new->order = order;
	strcpy(new->src,b);
	new->next = NULL;
	return(new);
}

NclScopeRec* WNewFScope()
{
	int i;
	NclScopeRec *thetable;
	thetable = (NclScopeRec*)malloc((unsigned)sizeof(NclScopeRec));
	thetable->this_scope = (NclSymTableElem*)malloc((unsigned)sizeof(NclSymTableElem)* NCL_SYM_TAB_SIZE);
	thetable->cur_offset = 0;
	thetable->level = 0;
        for(i = 0; i< NCL_SYM_TAB_SIZE; i++) {
                thetable->this_scope[i].nelem = 0;
                thetable->this_scope[i].thelist = NULL;
        }
	i = 0;	
	while(fkeytab[i].token != (int)NULL) {
		if(fkeytab[i].uppercase != NULL) {
			_NclAddInScope(thetable,fkeytab[i].uppercase,fkeytab[i].token);
		}
		if(fkeytab[i].lowercase != NULL) {
			_NclAddInScope(thetable,fkeytab[i].lowercase,fkeytab[i].token);
		}
	
		i++;	
	}
	return(thetable);
}
NclScopeRec* WNewWScope()
{
	int i;
	NclScopeRec *thetable;
	thetable = (NclScopeRec*)malloc((unsigned)sizeof(NclScopeRec));
	thetable->this_scope = (NclSymTableElem*)malloc((unsigned)sizeof(NclSymTableElem)* NCL_SYM_TAB_SIZE);
	thetable->cur_offset = 0;
	thetable->level = 0;
        for(i = 0; i< NCL_SYM_TAB_SIZE; i++) {
                thetable->this_scope[i].nelem = 0;
                thetable->this_scope[i].thelist = NULL;
        }
	i = 0;	
	while(wkeytab[i].token != (int)NULL) {
		if(wkeytab[i].uppercase != NULL) {
			_NclAddInScope(thetable,wkeytab[i].uppercase,wkeytab[i].token);
		}
		if(wkeytab[i].lowercase != NULL) {
			_NclAddInScope(thetable,wkeytab[i].lowercase,wkeytab[i].token);
		}
		i++;	
	}
	return(thetable);
}

void WNewWrap()
{
	WWrapRecList* tmp;
	int i;

	tmp = wrap_list;
	wrap_list = (WWrapRecList*)malloc(sizeof(WWrapRecList));
	wrap_list->next = tmp;
	wrap_list->therec = (WWrapRec*)malloc(sizeof(WWrapRec));
	current = wrap_list->therec;
	current->frec = WNewFScope();
	current->wrec = WNewWScope();
	current->crec = WNewWScope();
	current->c_defstring = NULL;
	current->c_argval= NULL;
	current->c_tmpval= NULL;
	current->c_vdefs = NULL;
	current->c_callrec = NULL;
	current->rtrn = NULL;
}

WGetArgValue *WNewArgVal()
{
	WArgValList* tmp;

	tmp = current->c_argval;
	current->c_argval = (WArgValList*)malloc(sizeof(WArgValList));
	current->c_argval->arec = (WGetArgValue*)malloc(sizeof(WGetArgValue));
	current->c_argval->arec->assign_to = NULL;
	current->c_argval->arec->pnum = -1;
	current->c_argval->arec->totalp = -1;
	current->c_argval->arec->ndims= NULL;
	current->c_argval->arec->dimsizes= NULL;
	current->c_argval->arec->missing= NULL;
	current->c_argval->arec->hasmissing= NULL;
	current->c_argval->arec->type= NULL;
	current->c_argval->arec->rw = -1;
	current->c_argval->arec->additional_src = NULL;
	current->c_argval->next = tmp;
	return(current->c_argval->arec);
}


WCentry *WNewVDef(char* name,int datatype,int isptr,char* array_spec,int order)
{
	WVDefList *tmp;
	NclSymbol *s;

	if(current->c_vdefs != NULL ) {	
		tmp = current->c_vdefs;
		while(tmp->next != NULL) tmp = tmp->next;
		tmp->next = (WVDefList*)malloc(sizeof(WVDefList));
		tmp->next->next = NULL;
		tmp->next->def = (WCentry*)malloc(sizeof(WCentry));
		tmp->next->def->string = (char*)malloc(strlen(name)+1);
		tmp->next->def->datatype = datatype;
		tmp->next->def->array_spec = NULL;
		tmp->next->def->order = order;
		tmp->next->def->additional_src = NULL;
		strcpy(tmp->next->def->string,name);
		if(array_spec != NULL) {
			tmp->next->def->array_spec= malloc(strlen(array_spec)+1);
			strcpy(tmp->next->def->array_spec,array_spec);
		} else {
			tmp->next->def->array_spec = NULL;
		}
		tmp->next->def->isptr = isptr;
		s = _NclAddInScope(current->crec,name,CVAR);
		s->u.centry = tmp->next->def;	
		return(tmp->next->def);
	} else {
		current->c_vdefs = (WVDefList*)malloc(sizeof(WVDefList));
		current->c_vdefs->next = NULL;
		current->c_vdefs->def = (WCentry*)malloc(sizeof(WCentry));
		current->c_vdefs->def->string = (char*)malloc(strlen(name)+1);
		current->c_vdefs->def->datatype = datatype;
		current->c_vdefs->def->array_spec = NULL;
		current->c_vdefs->def->order = order;
		current->c_vdefs->def->additional_src = NULL;
		strcpy(current->c_vdefs->def->string,name);
		if(array_spec != NULL) {
			current->c_vdefs->def->array_spec= malloc(strlen(array_spec)+1);
			strcpy(current->c_vdefs->def->array_spec,array_spec);
		}
		current->c_vdefs->def->isptr = isptr;
		s = _NclAddInScope(current->crec,name,CVAR);
		s->u.centry = current->c_vdefs->def;	
		return(current->c_vdefs->def);
	}
}

#define WNCL_MAX_ARGS  100

WCallRec* WNewCallRec(char* fort_name,int n_args) {
	int i;
	current->c_callrec = (WCallRec*)malloc(sizeof(WCallRec));
/*
	current->c_callrec->fname = malloc(strlen(fort_name)+1);
	strcpy(current->c_callrec->fname,fort_name);
*/
	current->c_callrec->n_args = n_args;
	current->c_callrec->nstrs = 0;
	current->c_callrec->arg_strings = (WCentry**)malloc(WNCL_MAX_ARGS*sizeof(WCentry*));
	for(i = 0; i < WNCL_MAX_ARGS; i++) {
		current->c_callrec->arg_strings[i] = NULL;
	}
	return(current->c_callrec);
}

void DoDimsizes(NclSymbol* sym) {
	char buffer[BUFFSIZE];

	sprintf(buffer,"%s_dimsizes",sym->name);
	if( _NclLookUpInScope(current->crec,buffer) == NULL) {
		(void)WNewVDef(buffer,NCL_int,0,"[NCL_MAX_DIMENSIONS]",0);
		sym->u.wparam->getarg->dimsizes = (char*)malloc(strlen(buffer)+1);
		strcpy(sym->u.wparam->getarg->dimsizes,buffer);
	}
	sprintf(buffer,"%s_ndims",sym->name);
	if( _NclLookUpInScope(current->crec,buffer) == NULL) {
		(void)WNewVDef(buffer,NCL_int,0,NULL,0);
		sprintf(buffer,"&%s_ndims",sym->name);
		sym->u.wparam->getarg->ndims= (char*)malloc(strlen(buffer)+1);
		strcpy(sym->u.wparam->getarg->ndims,buffer);
	}
}

void DoTotal(NclSymbol *sym) {
	char buffer[BUFFSIZE];
	WCentry *tmp;


	sprintf(buffer,"%s_total",sym->name);
	if( _NclLookUpInScope(current->crec,buffer) == NULL) {
		tmp = WNewVDef(buffer,NCL_int,0,NULL,0);
		sprintf(buffer,"\t%s_total = 1;\n\tfor(i = 0; i < %s_ndims; i++) {\n\t\t%s_total *= %s_dimsizes[i];\n\t}\n", sym->name,sym->name,sym->name,sym->name);
		tmp->additional_src = WNewAdditionalSrc(buffer,0);
	}
	DoDimsizes(sym);
}


WParamLoc* NewParamLoc(NclSymbol *sym) {
	
	WParamLoc *loc = (WParamLoc*)malloc(sizeof(WParamLoc));
	int i;

	if(sym != NULL) {	
		loc->type = FARGNEW;
		loc->xfarg = sym;
		loc->wsym = NULL;
		loc->cdef = NULL;
		loc->n_dims = sym->u.xref->u.farg->n_dims;
		loc->datatype = sym->u.xref->u.farg->datatype;
		for(i = 0; i < loc->n_dims; i++) {
			loc->dim_sizes[i] = sym->u.xref->u.farg->dim_sizes[i];
			sprintf(loc->dim_refs[i],"(%d)",sym->u.xref->u.farg->dim_sizes[i]);
		}
		return(loc);
	} else {
		loc->type = RETURN;
		loc->xfarg = NULL;
		loc->wsym = NULL;
                loc->cdef = NULL;
		loc->n_dims = -1;
		loc->datatype = NCL_none;
		return(loc);
	}
}

main(int argc,char* argv[])
{
	WWrapRecList* tmp;
	WArgValList *arg_list;
	int i,j;
	NclSymbol* s;
	char *buffer[1024];
	char upper[1024];
	char lower[1024];
	char *type = "NclANY";
	

	fprintf(stdout,initial_fmt);
	if(!yyparse()) {
/*
		tmp = wrap_list;
		while(tmp!= NULL) {
			fprintf(stdout,"extern NhlErrorTypes %s_W(\n#if NhlNeedProto\nvoid\n#endif\n);\n\n",tmp->therec->c_defstring);
			tmp = tmp->next;
			
		}
*/
		fprintf(stdout,init_fmt);
		tmp = wrap_list;
		while(tmp!= NULL) {
			fprintf(stdout,"\tnargs = 0;\n");
			fprintf(stdout,"\targs = NewArgs(%d);\n",tmp->therec->c_callrec->n_args);
			arg_list = tmp->therec->c_argval;
			i = 0;
			while(arg_list != NULL) {
				s = _NclLookUpInScope(tmp->therec->wrec,arg_list->arec->assign_to);
				if(s != NULL) {
					switch (s->u.wparam->datatype) {
						case NCL_int:
							type = "integer";
							break;
						case NCL_long:
							type = "long";
							break;
						case NCL_short:
							type = "short";
							break;
						case NCL_float:
							type = "float";
							break;
						case NCL_double:
							type = "double";
							break;
						case NCL_byte:
							type = "byte";
							break;
						case NCL_char:
							type = "character";
							break;
						case NCL_string:
							type = "string";
							break;
						case NCL_logical:
							type = "logical";
							break;	
					}
					if(s->u.wparam->n_dims > 0 ) {
						for(j = 0; j < s->u.wparam->n_dims; j++) {
							fprintf(stdout,"\tdimsizes[%d] = %d;\n",j,s->u.wparam->dim_sizes[j]);
						}
						fprintf(stdout,"\tSetArgTemplate(args,%d,\"%s\",%d,dimsizes);nargs++;\n",arg_list->arec->pnum,type,s->u.wparam->n_dims);
					} else {
						fprintf(stdout,"\tSetArgTemplate(args,%d,\"%s\",NclANY,NclANY);nargs++;\n",arg_list->arec->pnum,type);
					}
					
				}
				i++;	
				arg_list = arg_list->next;
			}
			for(j = 0; j < strlen(tmp->therec->c_defstring); j++) {
				if((!isdigit(tmp->therec->c_defstring[j]))&&(isalpha(tmp->therec->c_defstring[j]))) {
					if(isupper(tmp->therec->c_defstring[j])) {
						upper[j] = tmp->therec->c_defstring[j];
						lower[j] = (char)((int)tmp->therec->c_defstring[j] + 32) ;
					} else {
						lower[j] = tmp->therec->c_defstring[j];
						upper[j] = (char)((int)tmp->therec->c_defstring[j] - 32) ;
					}
				} else {
					upper[j] = lower[j] = tmp->therec->c_defstring[j];
				}
			}
			upper[j] = lower[j] = '\0';

			if(tmp->therec->f_or_p) {
				fprintf(stdout,"\tNclRegisterFunc(%s_W,args,\"%s\",nargs);\n\n",tmp->therec->c_defstring,upper);
			} else {
				fprintf(stdout,"\tNclRegisterProc(%s_W,args,\"%s\",nargs);\n\n",tmp->therec->c_defstring,upper);
			}
			if(tmp->therec->f_or_p) {
				fprintf(stdout,"\tNclRegisterFunc(%s_W,args,\"%s\",nargs);\n\n",tmp->therec->c_defstring,lower);
			} else {
				
				fprintf(stdout,"\tNclRegisterProc(%s_W,args,\"%s\",nargs);\n\n",tmp->therec->c_defstring,lower);
			}
			tmp = tmp->next;
		}
		fprintf(stdout,endf_fmt);
		exit(0);
	} else {
		fprintf(stdout,"Everything is NOT ok!\n");
		exit(1);
	}
}
