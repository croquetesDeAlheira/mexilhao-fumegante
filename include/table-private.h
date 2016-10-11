#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

#include "list-h"

struct table_t{
	struct list_t tabela[]; //um array de list_t
	int maxTabelaSize; // numero de linhas da tabela
	int tabelaSize; // tamanho da tabela com linhas preenchidas
}




#endif