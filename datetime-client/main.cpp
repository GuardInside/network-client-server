#include "client.h"
#include <QScopedPointer>

int main(int argc, char **argv)
{
	QTextStream stdostream(stdout, QIODevice::WriteOnly);
	QTextStream errostream(stderr, QIODevice::WriteOnly);
	QScopedPointer<DataTimeClinet> dtClient( new DTClient );

	QString ipaddr = "127.0.0.1", port = "23";
	QDateTime dateTime;

	if( argc > 1 )
		ipaddr = argv[1];

	if( argc > 2 )
		port = argv[2];

	if( !dtClient->setServerAddr(ipaddr, port.toUShort()) )
		goto error;

	if( !dtClient->connect() )
		goto error;

	dateTime = dtClient->dataTime();

	stdostream << (dateTime.isValid() ? dateTime.toString("ddd MMMM  d hh:mm:ss yyyy") : "Invalid date or time") << "\n";
	return 0;

	/* Обработчик ошибок */
	error:
		errostream << dtClient->errorString();
	return 1;
}
