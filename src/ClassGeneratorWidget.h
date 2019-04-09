#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtWidgets/QWidget>

struct Structure;

class ClassGeneratorWidget : public QWidget
{
	Q_OBJECT

public:
	ClassGeneratorWidget(QWidget *parent = nullptr);
	~ClassGeneratorWidget();

	void setList(QList<Structure>* structures);

public slots:
	void on_generateButton_clicked(bool checked = false);

private:
	class Private;
	std::unique_ptr<Private> d;
};
