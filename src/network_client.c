#include "network_client-private.h"

#include <stdlib.h>


struct server_t *network_connect(const char *address_port){
	struct server_t *server = malloc(sizeof(struct server_t));
	
	/* Verificar parâmetro da função e alocação de memória */
	if(address_port == NULL){ return NULL; }
	if(server == NULL){ return NULL; }
	

	// Separar os elementos da string, ip : porto
	// Pode estar errado ou ser desnecessário, ainda em estudo xD
	char *ip, *port;
	int i = 0, p = 0;
	int doispontos = 0;
	while(*address_port != NULL){
		if(*address_port != ':' && doispontos == 0){
			*(ip + i) = *(address_port + i);	
		}else{
			i++;
			*(port + p) = *(address_port + i);
			doispontos = 1;
		}
	}
	
	int inet_res = inet_pton(AF_INET,	

	/* Estabelecer ligação ao servidor:

		Preencher estrutura struct sockaddr_in com dados do
		endereço do servidor.

		Criar a socket.

		Estabelecer ligação.
	
	*/

	

	/* Se a ligação não foi estabelecida, retornar NULL */

	return server;
}

struct message_t *network_send_receive(struct server_t *server, struct message_t *msg){
	char *message_out;
	int message_size, msg_size, result;
	struct message_t msg_resposta;

	/* Verificar parâmetros de entrada */

	/* Serializar a mensagem recebida */
	message_size = message_to_buffer(msg, &message_out);

	/* Verificar se a serialização teve sucesso */

	/* Enviar ao servidor o tamanho da mensagem que será enviada
	   logo de seguida
	*/
	msg_size = htonl(message_size);
 	result = write_all(server->/*atributo*/, (char *) &msg_size, _INT);

	/* Verificar se o envio teve sucesso */

	/* Enviar a mensagem que foi previamente serializada */

	result = write_all(server->/*atributo*/, message_out, message_size);

	/* Verificar se o envio teve sucesso */

	/* De seguida vamos receber a resposta do servidor:

		Com a função read_all, receber num inteiro o tamanho da 
		mensagem de resposta.

		Alocar memória para receber o número de bytes da
		mensagem de resposta.

		Com a função read_all, receber a mensagem de resposta.
		
	*/

	/* Desserializar a mensagem de resposta */
	msg_resposta = buffer_to_message( /* */, /* */ );

	/* Verificar se a desserialização teve sucesso */

	/* Libertar memória */

	return msg_resposta;
}

int network_close(struct server_t *server){
	/* Verificar parâmetros de entrada */

	/* Terminar ligação ao servidor */

	/* Libertar memória */
}

/* Função que garante o envio de len bytes armazenados em buf,
   através da socket sock.
*/
int write_all(int sock, char *buf, int len){
	int bufsize = len;
	while(len > 0){
		int res = write(sock, buf, len);
		if(res < 0){
			if(errno == EINTR) continue;
			perror("write failed:");
			return res;
		}
		buf+= res;
		len-= res;
	}
	return bufsize;
}

/* Função que garante a receção de len bytes através da socket sock,
   armazenando-os em buf.
*/
int read_all(int sock, char *buf, int len){
	int bufsize = len;
	while(len > 0){
		int res = read(sock, buf, len);
		if(res < 0){
			if(errno == EINTR) continue;
			perror("write failed:");
			return res;
		}
		buf+= res;
		len-= res;
	}
	return bufsize;
}