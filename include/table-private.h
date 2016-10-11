#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

#include "table.h"

struct table_t{
	struct list_t *tabela[]; /* continuar definição */;
	int size; /* Dimensão da tabela com dados*/
	int maxSize; /* Dimensão da tabela com e sem dados*/
};

int key_hash(char *key, int l);

#endif
