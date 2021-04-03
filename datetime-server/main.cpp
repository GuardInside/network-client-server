#include "server.h"

int main(int, char **)
{
	QTextStream stdostream(stdout, QIODevice::WriteOnly);
	QTextStream errostream(stderr, QIODevice::WriteOnly);
	QScopedPointer<Server> dtServer( new DTServer( new DateTimeHandler ) );

	if( !dtServer->listen(23) )
		goto error;

	if( !dtServer->run() )
		goto error;

	return 0;

	/* Обработчик ошибок */
	error:
		errostream << dtServer->errorString();
	return 1;
}
