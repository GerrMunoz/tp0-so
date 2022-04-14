#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>

#define IP "127.0.0.1"
#define PUERTO "4444"

typedef enum
{
	PERSONA,
	COORDENADA
}op_code;

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

t_log* logger;

int iniciar_servidor(void);
int esperar_cliente(int);
int recibir_operacion(int);
void* recibir_estructura(int, op_code);
void* deserializar_estructura(op_code, void*);

void recibir_persona(int);
void recibir_coordenada(int);

t_persona* deserializar_persona(void*);
t_coordenada* deserializar_coordenada(void*);

#endif /* UTILS_H_ */
