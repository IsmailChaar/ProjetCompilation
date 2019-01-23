#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "analyseur_s.h"

#define debug false


extern int yylex();
typetoken token;

// lit le prochain token s'il n'a pas déjà été lu par le prédicat d'un nullable
typetoken _lire_token(){
		return (typetoken) yylex();
	
}




/* 
 PROG : LISTE_DECL begin LISTE_INST end
 LISTE_INST : INST LISTE_INSTAUX
 LISTE_INSTAUX : LISTE_INST  | epsilon
 LISTE_DECL : DECL LISTE_DECLAUX 
 LISTE_DECLAUX : LISTE_DECLAUX : LISTE_DECL | epsilon
 DECL : idf TYPE DECL_AUX
 DECL_AUX : CONST ';' | ';'
 TYPE : int | bool |float | char | string ; 
 CONST : inumber | dnumber | true | false
 INST : INST : idf = ADDSUB | true | false ';' 
      | if ‘(‘ idf == ADDSUB ‘)’ then LISTE_INST IF_INSTAUX 
      | print IDF ';'
      | for IDF = dnumber to dnumber do LISTE_INST endfor
 IF_INSTAUX :  endif  | else LISTE_INST endif

 ADDSUB : MULTDIV ADDSUBAUX

 ADDSUBAUX : – MULTDIV ADDSUBAUX
 ADDSUBAUX : + MULTDIV ADDSUBAUX
 ADDSUBAUX : epsilon

 MULTDIV : AUX MULTDIVAUX

 MULTDIVAUX : * AUX MULTDIVAUX ==> * MULTDIV
 MULTDIVAUX : / AUX MULTDIVAUX ==> / MULTDIV
 MULTDIVAUX : epsilon

 AUX : idf
 AUX : num
 AUX : ( ADDSUB )

*/


bool _list_inst(){
	return true;
}



// TYPE : int | bool |float | char | string ; 
bool _type() {
	bool result;
	if (debug) printf("type()\n");

	if (token == INT) {result = true;}
	else if (token == FLOAT) {result = true;}
	else if (token == BOOL) {result = true;}
    else if (token == CHAR) {result = true;}
    else if (token == STRING) {result = true;}
	else result = false;

	if (debug) printf("out of type()\n");
	return result;
}

//CONST : inumber | dnumber | true | false | character | string; 
bool _const() {
	bool result;
	if (debug) printf("const()\n");

	if (token == INUMBER) {
		
		result = true;	
	}else if (token == DNUMBER) {
		
		result = true;
	}else if (token == CHARACTER) {
		
		result = true;
	}else if (token == SSTRING) {
		result = true;
	}	
     else if ( (token == B_TRUE) || (token == B_FALSE) ){
		
		result = true;	
	} else result = false;

	if (debug) printf("out of const()\n");
	return result;
}










// DECL : idf TYPE DECL_AUX
bool _decl() {
	bool result;

	if ( token == IDF ) {
		token = _lire_token();
		if (token == COLUMN ){
			result=true;
			token = _lire_token();
				if (_type() == true){
				token = _lire_token();
					if (token == COLUMN_EQ){
						result=true;
						token = _lire_token();
						if (_decl_aux() == true){
							result=true;
						}else result = false;
					}else result = false;
				}else result = false;
		}else result = false;
	}else result = false;
	return result;
}
		
// DECL_AUX : CONST ';' | ';'
bool _decl_aux() {
	bool result;
	
	if ( token == PVIRG ) { 
		result = true; 
	}else if (_const()) {
		token = _lire_token();
		if (token == PVIRG) {
			result = true;
		} else result = false;
	}else result = false;

	
	return result;
}

// LISTE_DECL : DECL LISTE_DECLAUX
bool _liste_decl(){
	bool result;
	if (debug) printf("liste_decl()\n");

	if (_decl()){
		token = _lire_token();
		result = _liste_decl_aux();
	}else result = false;

	if (debug) printf("out of liste_decl()\n");
	return result;
}

// LISTE_DECLAUX : LISTE_DECL | '.' ==> LISTE_DECLAUX : LISTE_DECL | epsilon
// NULLABLE(LISTE_DECLAUX) = true
// follow(LISTE_DECLAUX) = {begin}
// first(LISTE_DECLAUX) = {idf}
bool _liste_decl_aux(){
	bool result;
	if (debug) printf("liste_decl_aux()\n");

	if (token == BEG_IN){
		result = true;
	}else if (_liste_decl()){
		result = true;
	}else{
		result = false;
	}

	if (debug) printf("out of liste_decl_aux()\n");
	return result;
}

bool decl_proc(){
	bool result;
	if (token == PROCEDURE){
		result=true;
		token=_lire_token();
		if(token == IDF){
			result=true;
			token=_lire_token();
			if(token == IS){
			result=true;
			token=_lire_token();
				if(_liste_decl() || token == BEG_IN ){
				result=true;
				token=_lire_token();
					if (_list_inst()){
						result=true;
						token=_lire_token();
						if (token==END){
							result=true;
							token=_lire_token();
							if (token==IDF||token ==PVIRG){
								result=true;
							}else result = false;
						}else result = false;
					}else result = false;
				}else result = false;
			}else result = false;
		}else result = false;
	}else result = false;
	return result;
}


int main(){
	token=_lire_token();
	if(decl_proc())
		printf("ok \n");
	else
		printf("not ok \n");
	return 0;
}