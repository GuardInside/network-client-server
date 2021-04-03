#pragma once

#include "unp.h"
#include "ReadableError.h"
#include <QString>
#include <QTextStream>
#include <QDateTime>
#include <QDataStream>

class Client: public ReadableError {
public:
	/// Настройка адреса сервера
	virtual bool setServerAddr(const QString &ipaddr, uint16_t port) = 0;

	/// Установка соединения с сервером
	virtual bool connect() = 0;

	/// Сброс состояния в исходное
	virtual void reset() = 0;

	/// Деструктор по умолчанию
	virtual ~Client() = default;
};

class DataTimeClinet: public Client {
public:
	/// Ответ сервера в виде даты и времени
	virtual QDateTime dataTime() = 0;

	/// Деструктор по умолчанию
	virtual ~DataTimeClinet() = default;
};

class DTClient: public DataTimeClinet {
public:
	DTClient() { reset(); }

	bool setServerAddr(const QString &ipaddr, uint16_t port) override
	{
		servaddr_.sin_family = AF_INET;
		servaddr_.sin_port = htons(port);

		if( inet_pton(AF_INET, ipaddr.toLocal8Bit().data(), &servaddr_.sin_addr) <= 0 ) {
			setErrorString("inet_pton error for " + ipaddr);
			return false;
		}

		return true;
	}

	bool connect() override
	{
		if( (socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
			setErrorString("socket error");
			return false;
		}

		if( ::connect(socket_, (struct sockaddr*)&servaddr_, sizeof(servaddr_)) < 0) {
			setErrorString("connect error");
			return false;
		}

		return true;
	}

	QDateTime dataTime() override
	{
		QString line;

		if( !read(&line) )
			return QDateTime();

		const QString format = "ddd MMM  d hh:mm:ss yyyy";
		return QDateTime::fromString(line.left(format.size()), format);
	}

	void reset() override
	{
		socket_ = 0;
		memset(&servaddr_, 0, sizeof(servaddr_));
	}

protected:
	virtual bool read(QString *line)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);

		/* Низкоуровневый код */
		int n = 0;
		char recvline[MAXLINE + 1];

		while( (n = ::read(socket_, recvline, MAXLINE)) > 0 )
			if( stream.writeRawData(recvline, n) != n ) {
				setErrorString("QByteArray write error");
				return false;
			}

		if( n < 0 ) {
			setErrorString("read error");
			return false;
		}

//		qDebug() << "Loop counter read: " << lpcnt;

		*line = QString::fromUtf8(buffer);
		return true;
	}

protected:
	int socket_;
	sockaddr_in servaddr_;
};
