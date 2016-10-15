#ifndef _MESSAGE_PRIVATE_H
#define _MESSAGE_PRIVATE_H

#define _SHORT = 2; //tamanho short
#define _INT = 4; //tamanho inteiro

#include "table-private.h" /* For table_free_keys() */
#include "message.h"
#include "data.h"


/*
*	verifica se um dado opcode eh valido
*	@return 1 se sim, senao 0
*/
int opIsValid(short opcode);

/*
*	verifica se um dado c_type eh valido
*	@return 1 se sim, senao 0
*/
int ctIsValid(short c_type);

#endif
