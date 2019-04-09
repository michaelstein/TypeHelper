#include "ClassGeneratorWidget.h"
#include "ui_ClassGenerator.h"
#include "model/types.h"
#include "inja/inja.hpp"

class ClassGeneratorWidget::Private
{
public:
	Private(ClassGeneratorWidget *owner)
		: q(owner)
	{}

	ClassGeneratorWidget* q;
	Ui_ClassGenerator ui;

	QList<Structure>* structures;
};

ClassGeneratorWidget::ClassGeneratorWidget(QWidget *parent)
	: QWidget(parent), d(std::make_unique<Private>(this))
{
	d->ui.setupUi(this);

	QFontMetrics metrics(d->ui.headerTemplateEdit->font());
	const auto tabWidth = 4 * metrics.width(' ');
	d->ui.headerTemplateEdit->setTabStopWidth(tabWidth);
	d->ui.cppTemplateEdit->setTabStopWidth(tabWidth);
	d->ui.generatedHeaderEdit->setTabStopWidth(tabWidth);
	d->ui.generatedCppEdit->setTabStopWidth(tabWidth);
}

ClassGeneratorWidget::~ClassGeneratorWidget()
{
}

void ClassGeneratorWidget::setList(QList<Structure>* structures)
{
	d->structures = structures;
}

void ClassGeneratorWidget::on_generateButton_clicked(bool checked)
{
	const auto cppTemplate = d->ui.cppTemplateEdit->toPlainText();
	const auto headerTemplate = d->ui.headerTemplateEdit->toPlainText();

	QString cppContent;
	QString headerContent;

	inja::Environment env;

	env.add_callback("toSnakeCase", 1, [](inja::Arguments& args) {
		auto val = QString::fromStdString(args.at(0)->get<std::string>());
		return val.toLower().replace(" ", "_").toStdString();
	});

	env.add_callback("dashesClass", 1, [](inja::Arguments& args) {
		auto val = QString::fromStdString(args.at(0)->get<std::string>());
		auto length = val.length() + 4;
		if (length >= 80)
			return std::string();
		return std::string(80u - length, '-');
	});

	env.add_callback("dashesNamespace", 1, [](inja::Arguments& args) {
		auto val = QString::fromStdString(args.at(0)->get<std::string>());
		auto length = (val.length() * 2) + 16;
		if (length >= 80)
			return std::string();
		return " " + std::string(80u - length - 1u, '-');
	});

	for (const auto& s : *d->structures) {
		std::vector<nlohmann::json> members;
		for (const auto& m : s.members) {
			nlohmann::json mdata;
			mdata["name"] = m.name.toStdString();
			mdata["description"] = m.description.toStdString();
			mdata["type"] = m.type.toStdString();
			members.push_back(mdata);
		}

		nlohmann::json sdata;
		sdata["name"] = s.name.toStdString();
		sdata["description"] = s.description.toStdString();
		sdata["members"] = members;

		headerContent += QString::fromStdString(env.render(headerTemplate.toStdString(), sdata));
		cppContent += QString::fromStdString(env.render(cppTemplate.toStdString(), sdata));
	}

	d->ui.generatedHeaderEdit->setText(headerContent);
	d->ui.generatedCppEdit->setText(cppContent);
}
