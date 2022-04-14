#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>

typedef enum
{
	PERSONA,
	COORDENADA
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct
{
    uint32_t dni;
    uint8_t edad;
	uint32_t pasaporte;
    uint32_t nombre_length;
	char* nombre;
} t_persona;

typedef struct
{
	uint32_t latitud;
	uint32_t longitud;
} t_coordenada;

int crear_conexion(char* ip, char* puerto);

void enviar_estructura(int socket_cliente, void* estructura, op_code codigo_operacion);
void* serializar_estructura(op_code codigo_operacion, void* structura, int* tamanio);

void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);

#endif /* UTILS_H_ */
