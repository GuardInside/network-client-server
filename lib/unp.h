#pragma once

#define QT_FATAL_CRITICALS 1
#include <QDebug>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>	// неблокируемые сокеты
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h> // Доменные сокеты

#define MAXLINE 256

#define err_sys(msg)\
	printf(#msg);

