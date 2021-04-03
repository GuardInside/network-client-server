#include "server.h"

int main(int, char **)
{
	QTextStream stdostream(stdout, QIODevice::WriteOnly);
	QTextStream errostream(stderr, QIODevice::WriteOnly);
	QScopedPointer<Server> dtServer( new DTServer( new DateTimeHandler ) );

	if( !dtServer->bind(19) )
		goto error;

	if( !dtServer->listen() )
		goto error;

	return 0;

	/* Обработчик ошибок */
	error:
		errostream << dtServer->errorString();
	return 1;
}
