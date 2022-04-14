#include "utils.h"


void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void* serializar_persona(t_persona* persona, int* tamanio)
{
	*tamanio = sizeof(uint32_t) * 3 + sizeof(uint8_t) + persona->nombre_length;
	void* stream = malloc(*tamanio);
	int desplazamiento = 0;

	memcpy(stream + desplazamiento, &persona->dni, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &persona->edad, sizeof(uint8_t));
	desplazamiento += sizeof(uint8_t);
	memcpy(stream + desplazamiento, &persona->pasaporte, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &persona->nombre_length, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, persona->nombre, persona->nombre_length);

	return stream;
}

void* serializar_coordenada(t_coordenada* coordenada, int* tamanio)
{
	*tamanio = sizeof(uint32_t) * 2;
	void* stream = malloc(*tamanio);
	int desplazamiento = 0;

	memcpy(stream + desplazamiento, &coordenada->latitud, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &coordenada->longitud, sizeof(uint32_t));

	return stream;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1) {
		printf("Error al conectar cliente");
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_estructura(int socket_cliente, void* estructura, op_code codigo_operacion)
{
	int tamanio = 0; // De la estructura a enviar
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = codigo_operacion;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->stream = serializar_estructura(codigo_operacion, estructura, &tamanio);
	paquete->buffer->size = tamanio;

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}

void* serializar_estructura(op_code codigo_operacion, void* estructura, int* tamanio)
{
	void* stream;

	switch (codigo_operacion) {
		case PERSONA:
			stream = serializar_persona(estructura, tamanio);
			break;
		case COORDENADA:
			stream = serializar_coordenada(estructura, tamanio);
			break;
	}

	return stream;
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
