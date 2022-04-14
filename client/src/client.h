#ifndef CLIENT_H_
#define CLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "utils.h"


t_log* iniciar_logger(void);
t_config* iniciar_config(void);
void persona(int conexion);
void coordenada(int conexion);
void terminar_programa(int, t_log*, t_config*);

#endif /* CLIENT_H_ */
