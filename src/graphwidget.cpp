#include "graphwidget.h"
#include "ui_graphwidget.h"

#include <QCloseEvent>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>

#include <cmath>

double degreeToRadian(double degree) {
	return degree * PI / 180;
}

GraphWidget::GraphWidget(QWidget* parent) :
	QMainWindow(parent), ui_(new Ui::GraphWidget) {
	setWindowIcon(QIcon(":general/graph.png"));
	ui_->setupUi(this);

	connect(ui_->a_exit, &QAction::triggered, this, &GraphWidget::close);

	connect(ui_->a_create, &QAction::triggered, this, &GraphWidget::createGraph);
	connect(ui_->a_open, &QAction::triggered, this, &GraphWidget::openGraph);
	connect(ui_->a_save, &QAction::triggered, this, &GraphWidget::saveGraph);
	connect(ui_->a_save_as, &QAction::triggered, this, &GraphWidget::saveGraphAs);

	connect(&graph_creator_, &GraphCreator::visualizeGraph, this, &GraphWidget::visualizeGraph);

	loadSettings();

	ui_->graphicsView->setScene(scene_.get());
	ui_->graphicsView->show();
}

GraphWidget::~GraphWidget() {
	saveSettings();
}

void GraphWidget::closeEvent(QCloseEvent* event) {
	switch (isOldGraphSave()) {
	case Saving::Yes:
		saveGraphAs();
	case Saving::No:
		if(graph_creator_.isWindow()){
			graph_creator_.close();
		}
		event->accept();
		break;
	case Saving::Cancel:
		event->ignore();
		break;
	}
}

GraphWidget::Saving GraphWidget::isOldGraphSave() {
	if (!is_graph_save_) {
		return static_cast<Saving>(
			QMessageBox::information(this, windowTitle(),
				QString::fromLocal8Bit("Хотите сохранить граф?"),
				QString::fromLocal8Bit("Сохранить"),
				QString::fromLocal8Bit("Не сохранять"),
				QString::fromLocal8Bit("Отмена"),
				static_cast<int>(Saving::Yes),
				static_cast<int>(Saving::Cancel)));
	}
	else {
		return Saving::No;
	}
}

void GraphWidget::createGraph() {
	if (!graph_creator_.isActiveWindow()) {
		switch (isOldGraphSave()) {
		case Saving::Yes:
			saveGraphAs();
		case Saving::No:
			graph_creator_.show();
			break;
		case Saving::Cancel:
			break;
		}
	}
}

void GraphWidget::openGraph() {
	switch (isOldGraphSave()) {
	case Saving::Yes:
		saveGraphAs();
	case Saving::No:
	{
		QString file_str(QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("Открытие"), QString(), "Graph (*.g)"));
		QSettings open_file(QSettings(file_str, QSettings::IniFormat, this));
		Graph graph;
		QString message_title(QString::fromLocal8Bit("Ошибка"));
		open_file.beginGroup("graph");
		if (open_file.contains("vertex_count")) {
			int count(open_file.value("vertex_count", 2).toInt());
			if (count >= 2) {
				graph.vertex_count_ = count;
				graph.adj_matrix_.assign(graph.vertex_count_, std::vector<int16_t>(graph.vertex_count_, 0));
				bool check(true);
				int edge_count(open_file.beginReadArray("edges"));
				if (edge_count != 0) {
					for (size_t i(0); i < edge_count; i += 1) {
						open_file.setArrayIndex(i);
						if (open_file.contains("left_vertex") && open_file.contains("right_vertex")) {
							Edge edge{ open_file.value("left_vertex").toInt(), open_file.value("right_vertex").toInt() };
							if (edge.left_vertex_ > graph.vertex_count_ || edge.right_vertex_ > graph.vertex_count_) {
								QMessageBox::critical(this, message_title, QString::fromLocal8Bit("Обнаружена связь с несуществующей вершиной!"));
								check = false;
								break;
							}
							int vertex_number(open_file.value("left_vertex").toInt() - 1),
								other_vertex_number(open_file.value("right_vertex").toInt() - 1);
							graph.adj_matrix_[vertex_number][other_vertex_number] = 1;
							graph.adj_matrix_[other_vertex_number][vertex_number] = 1;
						}
						else {
							QMessageBox::critical(this, message_title, QString::fromLocal8Bit("Неправильный формат записи связи!"));
							check = false;
							break;
						}
					}

					if (check) {
						GraphChecker graph_checker(graph);
						if (graph_checker.isGraph()) {
							visualizeGraph(graph);
						}
						else {
							QMessageBox::critical(this, message_title, QString::fromLocal8Bit("Несвязный граф!"));
						}
					}
				}
				else {
					QMessageBox::critical(this, message_title, QString::fromLocal8Bit("Нет связей между вершинами графа!"));
				}
			}
			else {
				QMessageBox::critical(this, message_title, QString::fromLocal8Bit("Некорректное количество вершин!"));
			}
		}
		else {
			QMessageBox::critical(this, message_title, QString::fromLocal8Bit("Неправильный формат записи количества вершин!"));
		}
		break;
	}
	case Saving::Cancel:
		break;
	}
	is_graph_save_ = true;
}

void GraphWidget::saveGraphFile(const QString& filename) {
	if (!graph_.adj_matrix_.empty()) {
		QSettings save_file(QSettings(filename, QSettings::IniFormat, this));
		save_file.beginGroup("graph");
		save_file.setValue("vertex_count", graph_.vertex_count_);
		save_file.beginWriteArray("edges");
		int edges_count(0);
		for (size_t i(0); i < graph_.vertex_count_; i += 1) {
			for (size_t j(i + 1); j < graph_.vertex_count_; j += 1) {
				if (graph_.adj_matrix_[i][j] == 1) {
					save_file.setArrayIndex(edges_count);
					save_file.setValue("left_vertex", i + 1);
					save_file.setValue("right_vertex", j + 1);
					edges_count += 1;
				}
			}
		}
		save_file.endArray();
		save_file.endGroup();
		is_graph_save_ = true;
	}
	else {
		QMessageBox::warning(this, QString::fromLocal8Bit("Предупреждение"), QString::fromLocal8Bit("Нет графа для сохранения!"));
	}
}

void GraphWidget::saveGraph() {
	if (!is_graph_save_) {
		QString datetime_str(QDateTime::currentDateTime().toString("hh-mm-ss dd.MM.yy"));
		saveGraphFile("graph " + datetime_str + ".g");
	}
}

void GraphWidget::saveGraphAs() {
	QString file_str(QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("Открытие"), QString(), "Graph (*.g)"));
	saveGraphFile(file_str);
}

void GraphWidget::loadSettings() {
	setGeometry(settings_->value("Geometry", QRect(600, 600, 600, 600)).toRect());
}

void GraphWidget::saveSettings() {
	settings_->setValue("Geometry", geometry());
}

QPointF GraphWidget::vertexCoordinate(double radius, size_t vertex_number, double degree) {
	return { radius * std::cos(degreeToRadian(90 - vertex_number * degree)) ,
		radius * std::sin(degreeToRadian(90 - vertex_number * degree)) };
}

void GraphWidget::visualizeGraph(const Graph& graph) {
	const qreal HALF_VERTEX_SIZE(static_cast<qreal>(VERTEX_SIZE) / 2);
	scene_->clear();
	double radius(20 * graph.vertex_count_);
	double degree(ROUND / graph.vertex_count_);
	std::vector<QPointF> vertex_points;
	for (size_t number_vertex(1); number_vertex <= graph.vertex_count_; number_vertex += 1) {
		QPointF point;
		if (number_vertex == 1) {
			point = { 0, radius };
		}
		else {
			point = vertexCoordinate(radius, number_vertex - 1, degree);
		}
		QGraphicsEllipseItem* ellipse_item(new QGraphicsEllipseItem(0, 0, VERTEX_SIZE, VERTEX_SIZE));
		ellipse_item->setPos(point);
		ellipse_item->setPen(QPen(QBrush(Qt::black), 2));
		ellipse_item->setBrush(QBrush(Qt::blue));
		vertex_points.push_back(point);
		QGraphicsSimpleTextItem* text_item(new QGraphicsSimpleTextItem(QString::fromStdString(std::to_string(number_vertex)), ellipse_item));
		scene_->addItem(ellipse_item);
		text_item->setPos(HALF_VERTEX_SIZE - 4, HALF_VERTEX_SIZE - 8);
		text_item->setBrush(QBrush(Qt::white));
	}
	for (size_t i(0); i < graph.vertex_count_; i += 1) {
		for (size_t j(i+1); j < graph.vertex_count_; j += 1) {
			if (graph.adj_matrix_.at(i).at(j) == 1) {
				QLineF line(vertex_points[i], vertex_points[j]);
				line.setLength(line.length() - HALF_VERTEX_SIZE);
				line.setPoints(line.p2(), line.p1());
				line.setLength(line.length() - HALF_VERTEX_SIZE);
				QGraphicsLineItem* line_item(new QGraphicsLineItem(line));
				line_item->setPos(HALF_VERTEX_SIZE, HALF_VERTEX_SIZE);
				line_item->setPen(QPen(QBrush(Qt::black), 2));
				scene_->addItem(line_item);
			}
		}
	}
	scene_->update();

	graph_ = graph;
	is_graph_save_ = false;
}