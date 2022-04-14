#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "utils.h"
#include <unistd.h>
#include <pthread.h>

void atender_cliente(int cliente_fd);

#endif /* SERVER_H_ */
