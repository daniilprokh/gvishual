#include "edgewidget.h"
#include "ui_edgewidget.h"

#include <QMessageBox>

EdgeWidget::EdgeWidget(QWidget* parent) :
	QWidget(parent), ui_(new Ui::EdgeWidget) {
	setWindowIcon(QIcon(":general/graph.png"));
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint
		| Qt::MSWindowsFixedSizeDialogHint);
	ui_->setupUi(this);
}

EdgeWidget::~EdgeWidget() {

}

void EdgeWidget::on_b_edge_clicked() {
	int left_edge(ui_->sb_left_edge->value()), right_edge(ui_->sb_right_edge->value());
	if (left_edge != right_edge) {
		emit addEdge(Edge{ ui_->sb_left_edge->value(), ui_->sb_right_edge->value() });
		close();
	}
	else {
		QMessageBox::warning(this, QString::fromLocal8Bit("Предупреждение"), QString::fromLocal8Bit("Связь между одной вершиной!"));
	}	
}