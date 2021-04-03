#pragma once

#include "unp.h"
#include "ReadableError.h"
#include <QString>
#include <QTextStream>
#include <QDateTime>

class Handler {
public:
	/// Обработчик запроса
	virtual bool handle(QString &response) = 0;

	/// Деструктор по умолчанию
	virtual ~Handler() = default;
};

class DateTimeHandler: public Handler {
public:
	/// Конструктор
	DateTimeHandler(const QString &format = QString("ddd MMM  d hh:mm:ss yyyy"))
		: dateTimeFormat_( format ) {}

	 bool handle(QString &response) override {
		response = QDateTime::currentDateTime().toString(dateTimeFormat_) + "\r\n";

		return true;
	}
protected:
	QString dateTimeFormat_;
};
class Server: public ReadableError {
public:
	/// Задать порт сервера
	virtual bool bind(uint16_t) = 0;

	/// Прослушивать порт
	virtual bool listen() = 0;

	/// Сброс состояния в исходное
	virtual void reset() = 0;

	/// Деструктор по умолчанию
	virtual ~Server() = default;
};

class DTServer: public Server {
public:
	DTServer(Handler *handler)
		: handler_( handler ) { reset(); }

	bool bind(uint16_t port) override {
		if( (listenSocket_ = socket(AF_INET, SOCK_STREAM, 0)) <= 0 ) {
			setErrorString("socket error");
			return false;
		}

		serveraddr_.sin_addr.s_addr = htonl(INADDR_ANY);
		serveraddr_.sin_port = htons(port);

		const int on = 1;
		if( setsockopt(listenSocket_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0 ) {
			setErrorString("reuse addr error");
			return false;
		}

		if( ::bind(listenSocket_, (sockaddr*)(&serveraddr_), sizeof(serveraddr_)) < 0) {
			setErrorString("bind error");
			return false;
		}

		return true;
	}

	bool listen() override {
		if( ::listen(listenSocket_, SOMAXCONN) < 0 ) {
			setErrorString("listen error");
			return false;
		}

		while( true ) {
			if( (connectedSocket_ = accept(listenSocket_, (sockaddr*)0, 0)) <= 0 ) {
				setErrorString("accept error");
				return false;
			}

			QString response;

			if( !handler_->handle(response) )
				return false;

			if( !write(response) )
				return false;
		}

		return true;
	}

	void reset() override {
		listenSocket_ = 0;
		connectedSocket_ = 0;
		bzero(&serveraddr_, sizeof(serveraddr_));
	}

protected:
	/// Записать строку в сокет и закрыть его
	virtual bool write(const QString &line) {
		const auto buffer = line.toUtf8();

		if( ::write(connectedSocket_, buffer.data(), buffer.size()) < 0 ) {
			setErrorString("write error");
			return false;
		}

		if( close(connectedSocket_) < 0 ) {
			setErrorString("close socket error");
			return false;
		}

		return true;
	}

private:
	int listenSocket_;
	int connectedSocket_;
	sockaddr_in serveraddr_;
	QScopedPointer<Handler> handler_;
};

