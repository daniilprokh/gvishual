#include "graphcreator.h"
#include "ui_graphcreator.h"

#include <QCloseEvent>
#include <QMessageBox>

#include <algorithm>

GraphChecker::GraphChecker(const Graph& graph) {
	checkVertexOnGraph(graph, 1);
	is_graph = vertexes.size() == graph.vertex_count_ ? true : false;
}

bool GraphChecker::isGraph() const {
	return is_graph;
}

size_t GraphChecker::realCountVertexes() const {
	return vertexes.size();
}

void GraphChecker::checkVertexOnGraph(const Graph& graph, const size_t vertex_number) {
	vertexes.insert(vertex_number);
	for (size_t other_vertex_number(0); other_vertex_number < graph.vertex_count_; other_vertex_number += 1) {
		if (vertexes.find(other_vertex_number + 1) == vertexes.end() &&
			graph.adj_matrix_.at(vertex_number - 1).at(other_vertex_number) == 1) {
			checkVertexOnGraph(graph, other_vertex_number + 1);
		}
	}
}

GraphCreator::GraphCreator(QWidget* parent) :
	QWidget(parent), ui_(new Ui::GraphCreator) {
	setWindowIcon(QIcon(":general/graph.png"));

	ui_->setupUi(this);

	connect(&edge_widget_, &EdgeWidget::addEdge, this, &GraphCreator::addEdge);

	ui_->b_del_edge->setEnabled(false);
}

GraphCreator::~GraphCreator() {
}

void GraphCreator::closeEvent(QCloseEvent* event) {
	if (edge_widget_.isWindow()) {
		edge_widget_.close();
	}
	event->accept();
}

void GraphCreator::addEdge(const Edge& edge) {
	ui_->list_edges->addItem(QString::fromStdString(std::to_string(edge.left_vertex_) + "-" + std::to_string(edge.right_vertex_)));
	edges_.push_back(edge);
}

void GraphCreator::on_b_create_clicked() {
	Graph graph;
	QString message_title(QString::fromLocal8Bit("Ошибка"));
	if (edges_.empty()) {
		QMessageBox::critical(this, message_title, QString::fromLocal8Bit("Нет связей между вершинами графа!"));
	}
	else {
		bool check(true);
		graph.vertex_count_ = ui_->sb_vertex_count->value();
		graph.adj_matrix_.assign(graph.vertex_count_, std::vector<int16_t>(graph.vertex_count_, 0));
		for (const Edge& edge : edges_) {
			if (edge.left_vertex_ > graph.vertex_count_ || edge.right_vertex_ > graph.vertex_count_) {
				QMessageBox::critical(this, message_title, QString::fromLocal8Bit("Обнаружена связь с несуществующей вершиной!"));
				check = false;
				break;
			}
			else {
				int vertex_number(edge.left_vertex_ - 1), other_vertex_number(edge.right_vertex_ - 1);
				if (graph.adj_matrix_[vertex_number][other_vertex_number] != 1) {
					graph.adj_matrix_[vertex_number][other_vertex_number] = 1;
					graph.adj_matrix_[other_vertex_number][vertex_number] = 1;
				} 
			}
		}
		if (check) {
			GraphChecker graph_checker(graph);
			if (graph_checker.isGraph()) {
				emit visualizeGraph(graph);
				close();
			}
			else {
				QMessageBox::critical(this, message_title, QString::fromLocal8Bit("Несвязный граф!"));
			}
		}
	}
}

void GraphCreator::on_b_del_edge_clicked() {
	edges_.removeAt(ui_->list_edges->currentRow());
	delete ui_->list_edges->currentItem();

	if (ui_->list_edges->count() == 0) {
		ui_->b_del_edge->setEnabled(false);
	}
}

void GraphCreator::on_b_new_edge_clicked() {
	if (!edge_widget_.isActiveWindow()) {
		edge_widget_.show();
	}
}

void GraphCreator::on_list_edges_currentRowChanged(int currentRow) {
	if (currentRow >= 0) {
		ui_->b_del_edge->setEnabled(true);
	}
}
