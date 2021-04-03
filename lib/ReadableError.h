#pragma once

#include <QString>

class ReadableError {
public:
	/// Описание последней ошибки
	QString errorString() const { return errorString_; }

	/// Деструктор по умолчанию
	virtual ~ReadableError() = default;
protected:
	void setErrorString(const QString &str) { errorString_ = QString("%1\nerrno = %2\n").arg(str).arg(errno); }
private:
	QString errorString_;
};

