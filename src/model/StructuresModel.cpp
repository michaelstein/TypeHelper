#include "StructuresModel.h"

StructuresModel::StructuresModel()
	: QAbstractTableModel()
	, _list(nullptr)
{
}

StructuresModel::~StructuresModel() = default;

int StructuresModel::rowCount(const QModelIndex& parent) const
{
	return _list ? _list->size() : 0;
}

int StructuresModel::columnCount(const QModelIndex& parent) const
{
	return 3;
}

QVariant StructuresModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (!_list)
			return {};

		const auto& structure = _list->at(index.row());
		const auto col = index.column();

		if (col == 0)
			return structure.name;
		if (col == 1)
			return structure.description;
		if (col == 2)
			return structure.members.size();
	}

	return {};
}

QVariant StructuresModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		if (section == 0)
			return "Name";
		if (section == 1)
			return "Description";
		if (section == 2)
			return "Member Count";
	}
	return {};
}

void StructuresModel::setList(QList<Structure>* list)
{
	beginResetModel();
	_list = list;
	endResetModel();
}

int StructuresModel::newStructure(const QString& name)
{
	if (!_list)
		return -1;

	const auto row = _list->count();
	beginInsertRows({}, row, row);

	Structure s;
	s.name = name;
	_list->append(s);

	endInsertRows();
	return row;
}

void StructuresModel::updateRow(const int row)
{
	if (_list && row < _list->size())
		emit dataChanged(index(row, 0), index(row, columnCount() - 1));
}

Structure* StructuresModel::structureByRow(const int row)
{
	if (!_list)
		return nullptr;
	return &(*_list)[row];
}
