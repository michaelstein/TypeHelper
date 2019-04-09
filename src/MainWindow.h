#pragma once
#include <memory>
#include <QtCore/QObject>
#include <QtGui/QClipboard>
#include <QtWidgets/QWidget>

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

public slots:
	void on_newStructureButton_clicked(bool checked = false);
	void on_stateUpdateButton_clicked(bool checked = false);
	void on_structureView_activated(const QModelIndex& index);
	void clipboardChanged(QClipboard::Mode mode);
	void on_loadButton_clicked(bool checked = false);
	void on_saveButton_clicked(bool checked = false);
	void on_classGeneratorButton_clicked(bool checked = false);

private:
	class Private;
	std::unique_ptr<Private> d;
};
