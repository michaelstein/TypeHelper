#pragma once
#include <memory>
#include "QtCore/QAbstractTableModel"
#include "types.h"

class StructuresModel : public QAbstractTableModel
{
public:
	StructuresModel();
	~StructuresModel() override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	void setList(QList<Structure>* list);
	int newStructure(const QString& name);
	void updateRow(const int row);
	Structure* structureByRow(const int row);

private:
	QList<Structure>* _list;
};
