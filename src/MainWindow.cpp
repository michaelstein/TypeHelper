#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "humblelogging/api.h"
#include "model/MemberModel.h"
#include "model/StructuresModel.h"
#include "model/types.h"
#include "ClassGeneratorWidget.h"
#include <QtCore/QDebug>
#include <QtCore/QHash>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QSettings>

HUMBLE_LOGGER(L, "TypeHelper.MainWindow");

enum State
{
	NEW_STRUCTURE,
	STRUCTURE_NAME,
	STRUCTURE_DESCRIPTION,
	MEMBER_NAME,
	MEMBER_DESCRIPTION,
	MEMBER_TYPE
};

class MainWindow::Private
{
public:
	Private(MainWindow *owner)
		: q(owner)
	{}

	MainWindow *q;
	Ui_MainWindow ui;

	QClipboard* clipboard;

	std::unique_ptr<MemberModel> memberModel;
	std::unique_ptr<StructuresModel> structureModel;

	QList<Structure> structures;
	Structure* selectedStructure;

	QString previousInput;
	State state = NEW_STRUCTURE;

	std::unique_ptr<ClassGeneratorWidget> classGeneratorWidget;

	void updateStateLabel()
	{
		QHash<State, QString> mapping;
		mapping.insert(State::NEW_STRUCTURE, "NEW_STRUCTURE");
		mapping.insert(State::STRUCTURE_NAME, "STRUCTURE_NAME");
		mapping.insert(State::STRUCTURE_DESCRIPTION, "STRUCTURE_DESCRIPTION");
		mapping.insert(State::MEMBER_NAME, "MEMBER_NAME");
		mapping.insert(State::MEMBER_DESCRIPTION, "MEMBER_DESCRIPTION");
		mapping.insert(State::MEMBER_TYPE, "MEMBER_TYPE");
		ui.stateLabel->setText(mapping.value(state));
	}

	void selectStructure(const int row)
	{
		selectedStructure = structureModel->structureByRow(row);
		memberModel->setMembers(&structures[row].members);
	}

	void handleInput(const QString& value)
	{
		QString newValue = value;
		previousInput = ui.inputEdit->text();
		switch (state) {
			case NEW_STRUCTURE: {
				state = STRUCTURE_NAME;
				newValue.replace(" ", "");
				break;
			}
			case STRUCTURE_NAME: {
				const auto row = structureModel->newStructure(previousInput);
				ui.structureView->selectRow(row);
				selectStructure(row);

				state = STRUCTURE_DESCRIPTION;
				break;
			}
			case STRUCTURE_DESCRIPTION: {
				const auto row = ui.structureView->currentIndex().row();
				structures[row].description = previousInput;
				structureModel->updateRow(row);

				state = MEMBER_NAME;
				newValue.replace(" ", "");
				break;
			}
			case MEMBER_NAME: {
				const auto row = memberModel->newMember(previousInput);
				ui.memberView->selectRow(row);

				const auto srow = ui.structureView->currentIndex().row();
				structureModel->updateRow(row);

				state = MEMBER_DESCRIPTION;
				break;
			}
			case MEMBER_DESCRIPTION: {
				const auto srow = ui.structureView->currentIndex().row();
				const auto mrow = ui.memberView->currentIndex().row();

				structures[srow].members[mrow].description = previousInput;
				memberModel->updateRow(mrow);

				state = MEMBER_TYPE;
				newValue.replace(" ", "");
				break;
			}
			case MEMBER_TYPE: {
				const auto srow = ui.structureView->currentIndex().row();
				const auto mrow = ui.memberView->currentIndex().row();

				structures[srow].members[mrow].type = previousInput;
				memberModel->updateRow(mrow);

				state = MEMBER_NAME;
				break;
			}
		}

		ui.inputEdit->setText(newValue);
		updateStateLabel();
	}

	void saveJson()
	{
		const auto filepath = ui.fileEdit->text().trimmed();
		if (filepath.isEmpty())
			return;

		QJsonArray structuresJson;
		for (const auto& s : structures) {
			QJsonObject sjson;
			sjson["name"] = s.name;
			sjson["description"] = s.description;

			QJsonArray membersJson;
			for (const auto& m : s.members) {
				QJsonObject mjson;
				mjson["name"] = m.name;
				mjson["description"] = m.description;
				mjson["type"] = m.type;
				membersJson.append(mjson);
			}

			sjson["members"] = membersJson;
			structuresJson.append(sjson);
		}

		QJsonDocument json;
		json.setArray(structuresJson);

		QFile file(filepath);
		if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
			file.write(json.toJson(QJsonDocument::Indented));
	}

	void loadJson()
	{
		const auto filepath = ui.fileEdit->text().trimmed();
		if (filepath.isEmpty())
			return;

		QFile file(filepath);
		if (!file.open(QIODevice::ReadOnly))
			return;

		auto json = QJsonDocument::fromJson(file.readAll());
		if (json.isNull() || !json.isArray())
			return;

		auto structuresJson = json.array();
		for (const auto& srjson : structuresJson) {
			if (!srjson.isObject())
				continue;

			auto sjson = srjson.toObject();
			const auto keys = sjson.keys();
			if (!keys.contains("name") || !keys.contains("description") || !keys.contains("members"))
				continue;

			Structure s;
			s.name = sjson.value("name").toString();
			s.description = sjson.value("description").toString();

			auto membersJson = sjson["members"].toArray();
			for (const auto& mrjson : membersJson) {
				if (!mrjson.isObject())
					continue;

				auto mjson = mrjson.toObject();
				Member m;
				m.name = mjson["name"].toString();
				m.description = mjson["description"].toString();
				m.type = mjson["type"].toString();
				s.members.append(m);
			}

			structures.append(s);
		}

		memberModel->setMembers(nullptr);
		structureModel->setList(&structures);
	}
};

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent), d(std::make_unique<Private>(this))
{
	d->ui.setupUi(this);

	d->structureModel = std::make_unique<StructuresModel>();
	d->structureModel->setList(&d->structures);
	d->ui.structureView->setModel(d->structureModel.get());

	d->memberModel = std::make_unique<MemberModel>();
	d->ui.memberView->setModel(d->memberModel.get());

	d->clipboard = QGuiApplication::clipboard();
	QObject::connect(d->clipboard, &QClipboard::changed, this, &MainWindow::clipboardChanged);

	d->updateStateLabel();

	QSettings settings;
	d->ui.fileEdit->setText(settings.value("filepath", "").toString());
}

MainWindow::~MainWindow()
{
	QSettings settings;
	settings.setValue("filepath", d->ui.fileEdit->text());
}

void MainWindow::on_newStructureButton_clicked(bool checked)
{
	d->handleInput({});
	d->state = NEW_STRUCTURE;
	d->updateStateLabel();
}

void MainWindow::on_stateUpdateButton_clicked(bool checked)
{
	d->updateStateLabel();
}

void MainWindow::on_structureView_activated(const QModelIndex& index)
{
	d->selectStructure(index.row());
}

void MainWindow::clipboardChanged(QClipboard::Mode mode)
{
	if (!d->ui.captureButton->isChecked())
		return;
	if (mode != QClipboard::Mode::Clipboard)
		return;

	const QString value = d->clipboard->text(mode).trimmed().replace(QRegularExpression("\\s"), " ");
	if (!value.isEmpty())
		d->handleInput(value);
}

void MainWindow::on_loadButton_clicked(bool checked)
{
	d->loadJson();
}

void MainWindow::on_saveButton_clicked(bool checked)
{
	d->saveJson();
}

void MainWindow::on_classGeneratorButton_clicked(bool checked)
{
	if (!d->classGeneratorWidget)
		d->classGeneratorWidget = std::make_unique<ClassGeneratorWidget>();
	d->classGeneratorWidget->setList(&d->structures);
	d->classGeneratorWidget->show();
}
