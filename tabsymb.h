

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "analyseur_s.h"



typedef struct {
    char* name;
    char* type;
    bool initialized;
    double value;
    int line;
}symb;

extern void init_tab_sym();

extern bool add_symb(char *name, char *type, bool initialized, double value, int line);

extern bool check_type_assignment(char *type, typetoken value_type);

