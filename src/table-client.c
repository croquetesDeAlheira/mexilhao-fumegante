/*
	Programa cliente para manipular tabela de hash remota.
	Os comandos introduzido no programa não deverão exceder
	80 carateres.

	Uso: table-client <ip servidor>:<porta servidor>
	Exemplo de uso: ./table_client 10.101.148.144:54321
*/

#include "stdio.h"
#include "network_client-private.h"

int main(int argc, char **argv){
	struct server_t *server;
	char input[81];
	struct message_t *msg_out, *msg_resposta;
	int i;
	int stop = 0;
	char *command, ip, port;

	/* Testar os argumentos de entrada */
	// Luis
	if (argc < 1 || argv == NULL) { return NULL; }
 	// Luis
	for (i = 0; i < argc; i++)
		if (argv[i] == NULL) { return NULL; }

	/* Usar network_connect para estabelcer ligação ao servidor */
	server = network_connect(/* */);

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
		i = 0;
		command = input;
		while (*command != '\n') { command++; }
		// Actualiza
		*command = '\0';


		/* Verificar se o comando foi "quit". Em caso afirmativo
		   não há mais nada a fazer a não ser terminar decentemente.
		 */
		// Luis
		if (input != "quit") { 
			stop = 1; 
		} else {
			/* Caso contrário:

			Verificar qual o comando;


			Preparar msg_out;

			Usar network_send_receive para enviar msg_out para
			o server e receber msg_resposta.
		*/
			}
		

	}
  	return network_close(server);
}




