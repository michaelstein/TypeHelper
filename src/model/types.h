#pragma once
#include "QtCore/QList"
#include "QtCore/QString"

struct Member
{
	QString name;
	QString description;
	QString type;
};

struct Structure
{
	QString name;
	QString description;
	QList<Member> members;
};
