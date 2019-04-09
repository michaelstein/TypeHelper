#include "MemberModel.h"

MemberModel::MemberModel()
	: QAbstractTableModel()
	, _list(nullptr)
{
}

MemberModel::~MemberModel() = default;

int MemberModel::rowCount(const QModelIndex& parent) const
{
	return _list ? _list->size() : 0;
}

int MemberModel::columnCount(const QModelIndex& parent) const
{
	return 3;
}

QVariant MemberModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (!_list)
			return {};

		const auto& member = _list->at(index.row());
		const auto col = index.column();

		if (col == 0)
			return member.name;
		if (col == 1)
			return member.description;
		if (col == 2)
			return member.type;
	}

	return {};
}

QVariant MemberModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		if (section == 0)
			return "Name";
		if (section == 1)
			return "Description";
		if (section == 2)
			return "Type";
	}

	return {};
}

void MemberModel::setMembers(QList<Member>* list)
{
	beginResetModel();
	_list = list;
	endResetModel();
}

int MemberModel::newMember(const QString& name)
{
	if (!_list)
		return -1;

	const auto row = _list->count();
	beginInsertRows({}, row, row);

	Member m;
	m.name = name;
	_list->append(m);

	endInsertRows();
	return row;
}

void MemberModel::updateRow(const int row)
{
	if (_list && row < _list->size())
		emit dataChanged(index(row, 0), index(row, columnCount() - 1));
}
