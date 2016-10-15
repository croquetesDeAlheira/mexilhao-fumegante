#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#include "../include/message-private.h"


//constants
const int OK = 0;
const int ERROR = -1;



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

	int vetorSize =(_SHORT + _SHORT); //comeca sempre 2 shorts.
	uint16_t short_value;
	uint32_t int_value;
	short keySize;
	short strSize;
	int dataSize;
	char **keys;

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
			vetorSize += msg->content.data->datasize;
			break;

		case CT_KEY :
			keySize = strlen(msg->content.key);
			vetorSize += _SHORT + keySize;
			break;

		case CT_KEYS :
			vetorSize += _INT; //numKeys
			keys = msg->content.keys;
			int i = 0;
			while(*(keys + i) != NULL){
				strSize = strlen(*(keys + i));
				vetorSize += _SHORT + strSize;
				i++;
			}
			break;

		case CT_ENTRY :
			vetorSize += _SHORT; //keysize
			keySize = strlen(msg->content.entry->key);
			vetorSize += keySize; //Tamanho da key
			vetorSize += _INT; //tamanho data
			dataSize = msg->content.entry->value->datasize;
			vetorSize += dataSize; //tamanho da mem do data
			break;

		default:
			return ERROR; // se o type nao for nenhum dos esperados
	}



	/* Alocar quantidade de memória determinada antes 
	*msg_buf = ....
	*/
	msg_buf = (char **)malloc(vetorSize); 

	/* Inicializar ponteiro auxiliar com o endereço da memória alocada */
	char *ptr = *msg_buf;

	short_value = htons(msg->opcode);
	memcpy(ptr, &short_value, _SHORT);
	ptr += _SHORT;

	short_value = htons(msg->c_type);
	memcpy(ptr, &short_value, _SHORT);
	ptr += _SHORT;

	/* Consoante o conteúdo da mensagem, continuar a serialização da mesma */

	switch(type){
		case CT_RESULT :
			int_value = htonl(msg->content.result);
			memcpy(ptr, &int_value, _INT);
			ptr += _INT;
			break;

		case CT_VALUE :
			dataSize = msg->content.data->datasize;
			int_value = htonl(dataSize);
			memcpy(ptr, &int_value, _INT);
			ptr += _INT;
			memcpy(ptr, &msg->content.data->data, dataSize);
			ptr += dataSize;
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
			keys = msg->content.keys;
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
			strSize = strlen(msg->content.entry->key);
			short_value = htons(strSize);
			memcpy(ptr, &short_value, _SHORT);
			ptr += _SHORT;
			memcpy(ptr, msg->content.entry->key, strSize);
			ptr += strSize;
			//copy data now.
			dataSize = msg->content.entry->value->datasize;
			int_value = htonl(dataSize);
			memcpy(ptr, &int_value, _INT);
			ptr += _INT;
			memcpy(ptr, msg->content.entry->value->data, dataSize);
			ptr += dataSize;
			break;
	}



	return vetorSize;
}

struct message_t *buffer_to_message(char *msg_buf, int msg_size){

	/* Verificar se msg_buf é NULL */
	/* msg_size tem tamanho mínimo ? */
	if(msg_buf == NULL || msg_size < 6){ return NULL;}

	//variaveis uteis
	uint16_t short_aux;
	uint32_t int_aux;
	short keySize;
	short strSize;
	int dataSize;
	int numKeys;
	void *data;
	struct data_t *value;

	/* Alocar memória para uma struct message_t */
	struct message_t *msg = (struct message_t *)malloc(sizeof(struct message_t));
	if(msg == NULL){return NULL; }
	/* Recuperar o opcode e c_type */
	memcpy(&short_aux, msg_buf, _SHORT);
	msg->opcode = ntohs(short_aux);
	msg_buf += _SHORT;

	memcpy(&short_aux, msg_buf, _SHORT);
	msg->c_type = ntohs(short_aux);
	msg_buf += _SHORT;

	/* A mesma coisa que em cima mas de forma compacta, ao estilo C! 
	msg->opcode = ntohs(*(short *) msg_buf++);
	msg->c_type = ntohs(*(short *) ++msg_buf);
	msg_buf += _SHORT;
	*/
	/* O opcode e c_type são válidos? */
	if(opIsValid(msg->opcode) && ctIsValid(msg->c_type)){
		/* Consoante o c_type, continuar a recuperação da mensagem original */
		switch(msg->c_type){
			case CT_RESULT :
				memcpy(&int_aux, msg_buf, _INT);
				msg->content.result = ntohl(int_aux);
				msg_buf += _INT;
				break;
			case CT_VALUE :
				memcpy(&int_aux, msg_buf, _INT);
				dataSize = ntohl(int_aux);
				msg_buf += _INT;
				data = malloc(dataSize);
				if(data == NULL){return NULL; }
				memcpy(data, msg_buf, dataSize);
				msg_buf += dataSize;
				value = data_create2(dataSize, data);
				if(value == NULL){ return NULL; }
				msg->content.data = value;
				break;
			case CT_KEY :
				memcpy(&short_aux, msg_buf, _SHORT);
				strSize = ntohs(short_aux);
				msg_buf += _SHORT;
				msg->content.key = (char *)malloc(strSize + 1);
				memcpy(msg->content.key, msg_buf, strSize);
				msg->content.key[strSize] = '\0';
				msg_buf += strSize;
				break;
			case CT_KEYS :
				memcpy(&int_aux, msg_buf, _INT);
				numKeys = ntohl(int_aux);
				msg_buf += _INT;
				char **keys = (char **)malloc(sizeof(char *) * numKeys);
				int i = 0;
				for (i = 0; i < numKeys; i++){
					memcpy(&short_aux, msg_buf, _SHORT);
					strSize = htons(short_aux);
					msg_buf += _SHORT;
					(keys + i) = (char *)malloc(strSize +1);
					memcpy((keys + i), msg_buf, strSize);
					((keys + i) + strSize) = '\0';
					msg_buf += strSize;
				}
				msg->content.keys = keys;
				break;
			
			case CT_ENTRY :
				memcpy(&short_aux, msg_buf, _SHORT);
				strSize = ntohs(short_aux);
				msg_buf += _SHORT;
				char *key = (char *)malloc(strSize + 1);
				memcpy(key, msg_buf, strSize);
				(key+strlen)= '\0';
				msg_buf += strSize;
				memcpy(&int_aux, msg_buf, _INT);
				dataSize = ntohl(int_aux);
				msg_buf += _INT;
				data = malloc(dataSize);
				memcpy(data, msg_buf, dataSize);
				value = data_create2(dataSize, data);
				if(value == NULL){return NULL; }
				struct entry_t *entry = entry_create(key, value);
				if(entry == NULL){return NULL; }
				msg->content.entry = entry;
				break;
			
			default :
				return 0;

		}
	}else{
		return NULL;
	}

	

	return msg;
}


int opIsValid(short opcode){
	switch(opcode){
		case OC_SIZE :
			return 1;
			break;
		case OC_DEL :
			return 1;
			break;
		case OC_UPDATE :
			return 1;
			break;
		case OC_GET :
			return 1;
			break;
		case OC_PUT :
			return 1;
			break;
		default :
			return 0;
	}
}

int ctIsValid(short c_type){
	switch(c_type){
		case CT_RESULT :
			return 1;
			break;
		case CT_VALUE :
			return 1;
			break;
		case CT_KEY :
			return 1;
			break;
		case CT_KEYS :
			return 1;
			break;
		case CT_ENTRY :
			return 1;
			break;
		default :
			return 0;

	}
}

