#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "message-private.h"


//constants
const int OK = 0;
const int ERROR = -1;
const int NOTHING_CHANGED = 1;
const int EQUALS =0;

const int _SHORT = 2; //tamanho short
const int _INT = 4; //tamanho inteiro

#define CT_RESULT	10
#define CT_VALUE	20
#define CT_KEY		30
#define CT_KEYS		40
#define CT_ENTRY	50

void free_message(struct message_t *msg){

/* Verificar se msg é NULL */

/* Se msg->c_type for:
VALOR, libertar msg->content.data
ENTRY, libertar msg->content.entry_create
CHAVES, libertar msg->content.keys
CHAVE, libertar msg->content.key
*/

/* libertar msg */

}

int message_to_buffer(struct message_t *msg, char **msg_buf){

	/* Verificar se msg é NULL */
	if(msg == NULL){ return  ERROR; }

	int vetorSize = _SHORT + _SHORT; //comeca sempre 2 shorts.
	uint16_t short_value;
	uint32_t int_value;
	short keySize;
	short strSize;

	/* Consoante o msg->c_type, determinar o tamanho do vetor de bytes
	que tem de ser alocado antes de serializar msg
	*/
	int type = msg->c_type;
	switch(type){
		case CT_RESULT :
			vetorSize += _INT; // + 4 bytes do result
			break;

		case CT_VALUE :
			vetorSize += _INT; // datasize int
			//soma o valor do datasize
			vertorSize += msg->content.data->datasize;
			break;

		case CT_KEY :
			keySize = strlen(msg->content.key);
			vetorSize += _SHORT + keySize;
			break;

		case CT_KEYS :
			vetorSize += _INT; //numKeys
			char **keys = msg->content.keys;
			int i = 0;
			while(*(keys + i) != NULL){
				strSize = strlen(*(keys + i);
				vetorSize += _SHORT + strSize;
				i++;
			}
			break;

		case CT_ENTRY :
			vetorSize += _SHORT; //keysize
			keySize = strlen(msg->content.entry->key);
			vetorSize += keySize; //Tamanho da key
			vetorSize += iSize; //tamanho data
			int dataSize += msg->content.entry->value->datasize;
			vetorSize += (short)dataSize; //int -> short
			break;

		default:
			return ERROR; // se o type nao for nenhum dos esperados
	}



	/* Alocar quantidade de memória determinada antes 
	*msg_buf = ....
	*/
	char *msg_buf = (char *)malloc(vetorSize); 

	/* Inicializar ponteiro auxiliar com o endereço da memória alocada */
	char *ptr = *msg_buf;

	short_value = htons(msg->opcode);
	memcpy(ptr, &short_value, _SHORT);
	ptr += _SHORT;

	short_value = htons(msg->c_type);
	memcpy(ptr, &short_value, _SHORT);
	ptr += _SHORT;

	switch(type){
		case CT_RESULT :
			int_value = htonl(msg->content.result);
			memcpy(ptr, &int_value, _INT);
			ptr += _INT;
			break;

		case CT_VALUE :

			break;

		case CT_KEY :
			strSize = strlen(msg->content.key); //supondo q retorna short
			short_value = htons(strSize);
			memcpy(ptr, &short_value, _SHORT);
			ptr += _SHORT;
			memcpy(ptr, msg->content.key , strSize); //sem o \0
			ptr += strSize;
			break;

		case CT_KEYS :
			char **keys = msg->content.keys;
			int j = 0;
			while(keys != NULL){
				strSize = strlen(*(keys + j));
				short_value = htons(strSize);
				memcpy(ptr, &short_value, _SHORT);
				ptr += _SHORT;
				memcpy(ptr,(keys + j), strSize);
				ptr += strSize;
				j++;
			}
			break;

		case CT_ENTRY :

			break;
	}

	/* Consoante o conteúdo da mensagem, continuar a serialização da mesma */

	return buffer_size;
}

struct message_t *buffer_to_message(char *msg_buf, int msg_size){

/* Verificar se msg_buf é NULL */

/* msg_size tem tamanho mínimo ? */

/* Alocar memória para uma struct message_t */

/* Recuperar o opcode e c_type */
memcpy(&short_aux, msg_buf, _SHORT);
msg->opcode = ntohs(short_aux);
msg_buf += _SHORT;

memcpy(&short_aux, msg_buf, _SHORT);
msg->c_type = ntohs(short_aux);
msg_buf += _SHORT;

/* A mesma coisa que em cima mas de forma compacta, ao estilo C! */
msg->opcode = ntohs(*(short *) msg_buf++);
msg->c_type = ntohs(*(short *) ++msg_buf);
msg_buf += _SHORT;

/* O opcode e c_type são válidos? */

/* Consoante o c_type, continuar a recuperação da mensagem original */

return msg;
}

