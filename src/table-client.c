/*
*	Grupo 12
* @author Daniel Santos 44887
* @author Luis Barros  47082
* @author Marcus Dias 44901
*/


/*
	Programa cliente para manipular tabela de hash remota.
	Os comandos introduzido no programa não deverão exceder
	80 carateres.

	Uso: table-client <ip servidor>:<porta servidor>
	Exemplo de uso: ./table_client 10.101.148.144:54321
*/

#include <string.h>
#include <stdio.h>

#include "../include/network_client-private.h"

// Definindo os tipos de comandos
#define BADKEY -1
#define PUT 1
#define KEY 2
#define UPDATE 3
#define DEL 4
#define SIZE 5
#define GET 6

const char space[2] = " ";

// Estrutura para poder associar os comandos
// aos comandos
static struct commands_t lookUpTabble[] = {
	{"put", PUT}, 
	{"get", GET}, 
	{"update", UPDATE}, 
	{"del", DEL}, 
	{"size", SIZE} 
};

// Numero de comandos definidos de maneira automatica
#define NKEYS (sizeof(lookUpTabble) / sizeof(struct commands_t))

// Funcao que me dá o valor de um comando
// correspondente a uma string para usar no switch
int keyfromstring(char *key) {
	int i;
	// Estrutura definida em network_client-private.h
	struct commands_t p;

  for (i = 0; i < NKEYS; i++) {
  	p = lookUpTabble[i];
	if (strcmp(p.key, key) == 0)
    	return p.val;
   }
    return BADKEY;
}

// Devolve um apontador de apontadores
// contendo o resto dos tokens
char ** getTokens (char* token) {
	char **tokens;
	int i;

	// Calloc inicializa a NULL os nao atribuidos
	tokens = (char**)calloc(3, sizeof(char*));
	if (tokens == NULL) { return NULL; }
	// Le o próximo token
	token = strtok(NULL, space);
	
	i = 0;
	while (token != NULL) {
		tokens[i] = strdup(token);
		token = strtok(NULL, space);
		i++;
	}
	return tokens;
}

// Função que imprime uma mensagem 
void print_msg(struct message_t *msg,const char* title) {
	printf("started print_msg\n");
	int i;
	
	printf("%s\n", title);
	printf("opcode: %d, c_type: %d\n", msg->opcode, msg->c_type);
	switch(msg->c_type) {
		case CT_ENTRY:{
			printf("key: %s\n", msg->content.entry->key);
			printf("datasize: %d\n", msg->content.entry->value->datasize);
		}break;
		case CT_KEY:{
			printf("key: %s\n", msg->content.key);
		}break;
		case CT_KEYS:{
			for(i = 0; msg->content.keys[i] != NULL; i++) {
				printf("key[%d]: %s\n", i, msg->content.keys[i]);
			}
		}break;
		case CT_VALUE:{
			printf("datasize: %d\n", msg->content.data->datasize);
		}break;
		case CT_RESULT:{
			printf("result: %d\n", msg->content.result);
		};
	}
	printf("-------------------\n");
}

void prt(char *str){
	printf("%s\n", str);
}





/************************************************************
 *                    Main Function                         *
 ************************************************************/
int main(int argc, char **argv){
	prt("started cliente");

	struct server_t *server;
	char input[81];
	struct message_t *msg_out, *msg_resposta, *msg_size;
	int i, stop, sigla, m_size, size;
	char *command, *token;
	char **arguments, **dataToNetwork;
	struct data_t *data;

	const char quit[5] = "quit";
	const char ip_port_seperator[2] = ":";
	const char get_all_keys[2] = "!";
	const char msg_title_out[31] = "Mensagem enviada para servidor";
	const char msg_title_in[30] = "Mensagem recebida do servidor";

	/* Testar os argumentos de entrada */
	// Luis: o nome do programa conta sempre como argumento
	if (argc != 2 || argv == NULL || argv[1] == NULL) { 
		printf("Erro de argumentos.\n");
		printf("Exemplo de uso: /table_client 10.101.148.144:54321\n");
		return -1; 
	}
	prt("received args");
	prt(argv[1]);

	/* Usar network_connect para estabelecer ligação ao servidor */
	// Passa ip:porto
	server = network_connect(argv[1]);
	if(server == NULL){exit(0);}
	prt("connected to network");
	/* Fazer ciclo até que o utilizador resolva fazer "quit" */
	stop = 0;
 	while (stop == 0){ 

		printf(">>> "); // Mostrar a prompt para receber comando

		/* Receber o comando introduzido pelo utilizador
		   Sugestão: usar fgets de stdio.h
		   Quando pressionamos enter para finalizar a entrada no
		   comando fgets, o carater \n é incluido antes do \0.
		   Convém retirar o \n substituindo-o por \0.
		k*/
		fgets(input,80,stdin);

		// Retirar o caracter \n
		command = input;
		while (*command != '\n') { command++; }
		// Actualiza
		*command = '\0';


		/* Verificar se o comando foi "quit". Em caso afirmativo
		   não há mais nada a fazer a não ser terminar decentemente.
		 */
		// Luis: Ler o primeiro token para avaliar
		token = strtok(input, space);

		// Leu quit sai do ciclo while
		if (strcmp(quit,token) == 0) { 
			stop = 1; 
		} else {
			/* Caso contrário:
			Preparar msg_out;
			Usar network_send_receive para enviar msg_out para
			o server e receber msg_resposta.
			*/

			// Sigla do comando para poder correr o switch
			sigla = keyfromstring(token);
			// Inicializa a mensagem
			msg_out = (struct message_t*)malloc(sizeof(struct message_t));
			if (msg_out == NULL) { break; } // Salta o ciclo

			switch(sigla) {
				case BADKEY :
					// Algo como
					printf("Comando nao conhecido, por favor tente de novo\n");
					printf("Exemplo de uso: put <key> <data>\n");
					printf("Exemplo de uso: get <key>\n");
					printf("Exemplo de uso: get !\n");
					printf("Exemplo de uso: del <key>\n");
					printf("Exemplo de uso: size\n");
					printf("Exemplo de uso: quit\n");
					break;

				case PUT :
					printf("put\n");
					// argumentos do put
					arguments = getTokens(token);
					size = strlen(arguments[1]) + 1;
					// Criar o data 
					data = data_create2(size, arguments[1]);
					//Criar o entry
					// Atributos de msg
					msg_out->opcode = OC_PUT;
					msg_out->c_type = CT_ENTRY;
					msg_out->content.entry = 
						entry_create(arguments[0], data);
					// Libertar memória
					data_destroy(data);
					break;

				case GET :
				printf("get\n");
					arguments = getTokens(token);
					// Atributos de msg
					msg_out->opcode = OC_GET;
					msg_out->c_type = CT_KEY;
					msg_out->content.key = strdup(arguments[0]);

				case UPDATE :
				printf("update\n");
					// argumentos do put
					arguments = getTokens(token);
					size = strlen(arguments[1]) + 1;
					// Criar o data 
					data = data_create2(size, arguments[1]);
					// Atributos de msg
					msg_out->opcode = OC_UPDATE;
					msg_out->c_type = CT_ENTRY;
					msg_out->content.entry = 
						entry_create(arguments[0], data);
					// Libertar memória
					data_destroy(data);
					break;

				case DEL : 
				printf("del\n");
					arguments = getTokens(token);		
					msg_out->opcode = OC_DEL;
					msg_out->c_type = CT_KEY;
					msg_out->content.key = strdup(arguments[0]);
					break;

				case SIZE :	
				printf("size\n");
					msg_out->opcode = OC_SIZE;
					msg_out->c_type = CT_RESULT; 
					break;
			}

			// Mensagens carregadas
			// Trata de enviar e receber 
			// Faz os prints necessários
			if (sigla != BADKEY) {
				/*// Serializa a msg
				m_size = message_to_buffer(msg_out, dataToNetwork);
				// Prepara a msg com o tamanho o pedido a enviar
				msg_size = (struct message_t*)malloc(sizeof(struct message_t));
				if (msg_size == NULL) { 
					free_message(msg_out);			
					list_free_keys(arguments);	
					break; 
				}
				// Inicilializa atributos
				msg_size->opcode = OC_SIZE;
				msg_size->c_type = CT_RESULT;
				// 
				msg_size->content.result = m_size;
				// Envia msg com o tamanho do pedido
				//network_send_receive(server, msg_out);
				// Imprime msg a enviar
				print_msg(msg_out, msg_title_out);
				*/
				// Envia a msg com o pedido e aguarda resposta
				prt("aqui");
				msg_resposta = network_send_receive(server, msg_out);
				// Imprime a msg recebida
				print_msg(msg_resposta, msg_title_in);
				// Liberta memoria dos argumentos e da memoria
				
				
				free_message(msg_out);
				free_message(msg_size);			
				list_free_keys(arguments);	
			}
			
		}
	}
  	return network_close(server);
}