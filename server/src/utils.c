#include"utils.h"

int iniciar_servidor(void)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

	for (p=servinfo; p != NULL; p = p->ai_next) {
		// Creamos el socket de escucha del servidor
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        // Asociamos el socket a un puerto
        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            continue;
        }
        break;
    }

    // Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);
    log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;
	socklen_t tam_direccion = sizeof(struct sockaddr_in);

	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else {
		close(socket_cliente);
		return -1;
	}
}

void* recibir_estructura(int socket_cliente, op_code codigo_operacion)
{
	int size;
	void* buffer;

	recv(socket_cliente, &size, sizeof(int), MSG_WAITALL);
	buffer = malloc(size);
	recv(socket_cliente, buffer, size, MSG_WAITALL);

	void* estructura = deserializar_estructura(codigo_operacion, buffer);

	free(buffer);
	return estructura;
}

void* deserializar_estructura(op_code codigo_operacion, void* estructura)
{
	void* stream;

	switch (codigo_operacion) {
		case PERSONA:
			stream = deserializar_persona(estructura);
			break;
		case COORDENADA:
			stream = deserializar_coordenada(estructura);
			break;
	}

	return stream;
}

void recibir_persona(int socket_cliente)
{
	log_info(logger, "Me llego una persona");

	t_persona* persona = (t_persona*) recibir_estructura(socket_cliente, PERSONA);

	printf("Dni : %d\n", persona->dni);
	printf("Edad : %d\n", persona->edad);
	printf("Pasaporte : %d\n", persona->pasaporte);
	printf("Nombre : %s\n", persona->nombre);

	free(persona->nombre);
	free(persona);
}

void recibir_coordenada(int socket_cliente)
{
	log_info(logger, "Me llego una coordenada");

	t_coordenada* coordenada = (t_coordenada*) recibir_estructura(socket_cliente, COORDENADA);

	printf("Latitud : %d\n", coordenada->latitud);
	printf("Longitud : %d\n", coordenada->longitud);

	free(coordenada);
}

t_coordenada* deserializar_coordenada(void* stream)
{
	t_coordenada* coordenada = malloc(sizeof(t_coordenada));

	// Deserializamos los campos que tenemos en el buffer
	memcpy(&(coordenada->latitud), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(coordenada->longitud), stream, sizeof(uint32_t));

	return coordenada;
}

t_persona* deserializar_persona(void* stream)
{
    t_persona* persona = malloc(sizeof(t_persona));
    
    // Deserializamos los campos que tenemos en el buffer
    memcpy(&(persona->dni), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(persona->edad), stream, sizeof(uint8_t));
    stream += sizeof(uint8_t);
    memcpy(&(persona->pasaporte), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    
    // Por último, para obtener el nombre, primero recibimos el tamaño y luego el texto en sí:
    memcpy(&(persona->nombre_length), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    persona->nombre = malloc(persona->nombre_length);
    memcpy(persona->nombre, stream, persona->nombre_length);

    return persona;
}