#include "server.h"

int main(void) {
	pthread_t thread;
	int cliente_fd;
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");

	while (1) {
		cliente_fd = esperar_cliente(server_fd);

		if (cliente_fd == -1) {
			close(cliente_fd);
			continue;
		}

		pthread_create(&thread, NULL, (void*) atender_cliente, (void*) cliente_fd);
		pthread_detach(thread);
	}
	log_destroy(logger);
	return EXIT_SUCCESS;
}

void atender_cliente(int cliente_fd)
{
	puts("Cliente conectado");

	int cod_op;

	while ((cod_op = recibir_operacion(cliente_fd)) != -1) {
		switch (cod_op) {
			case PERSONA:
				recibir_persona(cliente_fd);
				break;
			case COORDENADA:
				recibir_coordenada(cliente_fd);
				break;
			case -1:
				log_error(logger, "El cliente se desconecto.");
				log_destroy(logger);
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}

	puts("Cliente desconectado");
}