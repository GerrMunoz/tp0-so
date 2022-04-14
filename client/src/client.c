#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger, "Soy un Log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip     = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor  = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO: %s", puerto);
	log_info(logger, "VALOR: %s", valor);

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Armamos y enviamos la persona
	persona(conexion);

	// Armamos y enviamos la coordenada
	coordenada(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	if (!(nuevo_logger = log_create("tp0.log", "tp0", 1, LOG_LEVEL_INFO))) {
		printf("No se pudo iniciar logger\n");
		exit(1);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	if (!(nuevo_config = config_create("../cliente.config"))) {
		printf("No se pudo crear config\n");
		exit(1);
	}

	return nuevo_config;
}

void persona(int conexion)
{
	// Armamos la persona
	t_persona* persona = (t_persona*) malloc(sizeof(t_persona)); 
	persona->dni = 1234;
	persona->edad = 25;
	persona->pasaporte = 1111;
	persona->nombre_length = strlen("Gerardo") + 1;
	persona->nombre = malloc(persona->nombre_length);
	strcpy(persona->nombre, "Gerardo");

	enviar_estructura(conexion, persona, PERSONA);

	free(persona->nombre);
	free(persona);
}

void coordenada(int conexion)
{
	// Armamos la coordenada
	t_coordenada* coordenada = (t_coordenada*) malloc(sizeof(t_coordenada));
	coordenada->latitud = 10;
	coordenada->longitud = 20;

	enviar_estructura(conexion, coordenada, COORDENADA);

	free(coordenada);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
