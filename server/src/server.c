#include "server.h"

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);

	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
			case PERSONA:
				recibir_persona(cliente_fd);
				break;
			case COORDENADA:
				recibir_coordenada(cliente_fd);
				break;
			case -1:
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				log_destroy(logger);
				return EXIT_FAILURE;
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
	log_destroy(logger);
	return EXIT_SUCCESS;
}
