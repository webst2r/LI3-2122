#ifndef VIEW_H
#define VIEW_H

#include <glib-2.0/glib.h>

#define MAXPAGE 6

void printMenu();

void printLowerLoading();

void printLowerLoadingTests();

void paginacao(GArray* arrStr,int p, int total);

void selector(int numeroPaginas, GArray *arrayStrs);

void printOpcaoInvalida();

void printNumeroUtilizadores();

void printNumeroLinguagens();

void printInsiraData(int i);

void printInsiraOpcao();

void limparTerminal();

void printInsiraLinguagem();

void printCPUTime(int query,double cpu_time_used);

void printErroAbrirFicheiro();




#endif