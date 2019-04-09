#pragma once
#include <memory>
#include "QtCore/QAbstractTableModel"
#include "types.h"

class MemberModel : public QAbstractTableModel
{
public:
	MemberModel();
	~MemberModel() override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	void setMembers(QList<Member>* list);
	int newMember(const QString& name);
	void updateRow(const int row);

private:
	QList<Member>* _list;
};
