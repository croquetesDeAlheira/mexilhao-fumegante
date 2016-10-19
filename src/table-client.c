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

int main(int argc, char **argv){
	struct server_t *server;
	char input[81];
	struct message_t *msg_out, *msg_resposta;
	// Luis	
	int i;
	int stop;
	char *command; 
	char *token;
	const char space[2] = " ";

	/* Testar os argumentos de entrada */
	// Luis
	if (argc < 1 || argv == NULL) { return 0; }
 	// Luis
	for (i = 0; i < argc; i++)
		if (argv[i] == NULL) { return 0; }

	/* Usar network_connect para estabelecer ligação ao servidor */
	server = network_connect(/* */);
	// Luis	
	stop = 0;
	/* Fazer ciclo até que o utilizador resolva fazer "quit" */
 	while (stop == 0){ // Condicao do while luis

		printf(">>> "); // Mostrar a prompt para inserção de comando

		/* Receber o comando introduzido pelo utilizador
		   Sugestão: usar fgets de stdio.h
		   Quando pressionamos enter para finalizar a entrada no
		   comando fgets, o carater \n é incluido antes do \0.
		   Convém retirar o \n substituindo-o por \0.
		*/
		// Luis
		fgets(input,80,stdin);

		// Retirar o caracter \n
		command = input;
		while (*command != '\n') { command++; }
		// Actualiza
		*command = '\0';


		/* Verificar se o comando foi "quit". Em caso afirmativo
		   não há mais nada a fazer a não ser terminar decentemente.
		 */
		// Luis: Ler o primeiro token para avalioar
		token = strtok(input, space);

		
		if (strcmp(input,token) == 0) { 
			stop = 1; 
		} else {
			/* Caso contrário:

			Preparar msg_out;

			Usar network_send_receive para enviar msg_out para
			o server e receber msg_resposta.
		*/

				// Luis: Switch case com o token para que comando se trata
				switch(token) {
					case "put":
						
						break;
					
					case "get:
						break;

					case "update" :
						break;
					
					case "del" :
						break;
					
					case "size" :
						break;

					default :
						printf("O comando inserido (%s) nao existe\n", token);
						//stop = 1;
						break;
							
				}
			}
		

	}
  	return network_close(server);
}




