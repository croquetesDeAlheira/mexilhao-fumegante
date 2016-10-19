/*
	Programa cliente para manipular tabela de hash remota.
	Os comandos introduzido no programa não deverão exceder
	80 carateres.

	Uso: table-client <ip servidor>:<porta servidor>
	Exemplo de uso: ./table_client 10.101.148.144:54321
*/

#include <string.h>
#include <stdio.h>

#include "../inc/network_client-private.h"

// Definindo os tipos de comandos
#define BADKEY -1
#define PUT 1
#define KEY 2
#define UPDATE 3
#define DEL 4
#define SIZE 5

const char space[2] = " ";

// A
static struct commands_t lookUpTabble[] = {
	{"put", PUT}, 
	{"key", KEY}, 
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
	struct commands_t *p;

  for (i = 0; i < NKEYS; i++) {
  	p = lookUpTabble[i];
		if (strcmp(p->key, key) == 0)
    	return p->val;
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

/************************************************************
 *                    Main Function                         *
 ************************************************************/
int main(int argc, char **argv){
	struct server_t *server;
	char input[81];
	struct message_t *msg_out, *msg_resposta;	
	int i, stop, sigla, size;
	char *command, token, dataToNetwork;
	char **arguments;
	struct data_t data*;
	struct entry_t entry;

	const char quit[5] = "quit";

	/* Testar os argumentos de entrada */
	// Luis
	if (argc != 1 || argv == NULL || argv[0] == NULL) { return 0; }

	/* Usar network_connect para estabelecer ligação ao servidor */
	server = network_connect(/* */);
	// Luis	
	stop = 0;
	/* Fazer ciclo até que o utilizador resolva fazer "quit" */
 	while (stop == 0){ 

		printf(">>> "); // Mostrar a prompt para receber comando

		/* Receber o comando introduzido pelo utilizador
		   Sugestão: usar fgets de stdio.h
		   Quando pressionamos enter para finalizar a entrada no
		   comando fgets, o carater \n é incluido antes do \0.
		   Convém retirar o \n substituindo-o por \0.
		*/
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
					break;

				case PUT :
					// argumentos do put
					arguments = getTokens(token);
					size = strlen(arguments[1]) + 1;
					// Criar o data 
					data = data_create2(size, arguments[1]);
					//Criar o entry
					entry = entry_create(arguments[0], data);
					// Definir a mensagem
					msg_out = (struct message_t*)malloc(sizeof(struct message_t));
					// Atributos de msg
					msg_out->opcode = OC_PUT;
					msg_out->c_type = CT_ENTRY;
					msg_out->content->entry = entry;

					// manda a msg para a rede
					// size noutro contexto
					size = message_to_buffer(msg_out. dataToNetwork);

					// A partir daqui não faço ideia...

					// Libertar memória
					data_destroy(data);
					entry_destroy(entry);
					break;

				case KEY :
					arguments = getTokens(token);
					msg_out = (struct message_t*)malloc(sizeof(struct message_t));
					// Atributos de msg
					msg_out->opcode = OC_GET;
					msg_out->c_type = CT_KEY;
					msg_out->content->key = strdup(arguments[0]);

					// manda a msg para a rede
					// size noutro contexto
					size = message_to_buffer(msg_out. dataToNetwork);

					// A partir daqui não faço ideia...

					case UPDATE :
						// argumentos do put
					arguments = getTokens(token);
					size = strlen(arguments[1]) + 1;
					// Criar o data 
					data = data_create2(size, arguments[1]);
					//Criar o entry
					entry = entry_create(arguments[0], data);
					// Definir a mensagem
					msg_out = (struct message_t*)malloc(sizeof(struct message_t));
					// Atributos de msg
					msg_out->opcode = OC_UPDATE;
					msg_out->c_type = CT_ENTRY;
					msg_out->content->entry = entry;

					// manda a msg para a rede
					// size noutro contexto
					size = message_to_buffer(msg_out. dataToNetwork);

					// A partir daqui não faço ideia...

					// Libertar memória
					data_destroy(data);
					entry_destroy(entry);
					break;

					case DEL : 
						arguments = getTokens(token);
						msg_out = (struct message_t*)malloc(sizeof(struct message_t));
						msg_out->opcode = OC_DEL;
						msg_out->c_type = CT_KEY;
						msg_out->content->key = strdup(arguments[0]);

						// manda a msg para a rede
						// size noutro contexto
						size = message_to_buffer(msg_out. dataToNetwork);

						// A partir daqui não faço ideia...
						break;

					case SIZE :
						arguments = getTokens(token);
						msg_out = (struct message_t*)malloc(sizeof(struct message_t));
						msg_out->opcode = OC_SIZE;
						msg_out->c_type = CT_RESULT; // NOT SURE!!!!
						msg_out->content->result = arguments[0];

						// manda a msg para a rede
						// size noutro contexto
						size = message_to_buffer(msg_out. dataToNetwork);

						// A partir daqui não faço ideia...
						break;
			}
			// Liberta memoria dos argumentos e da memoria
			free_message(msg_out);			
			list_free_keys(arguments);
		}
	}
  	return network_close(server);
}




