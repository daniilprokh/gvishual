#ifndef EDGEWIDGET_H
#define EDGEWIDGET_H

#include <QWidget>

#include <memory>

///@brief Структура, хранящая номера вершин, между которыми есть связь  
struct Edge {
	int left_vertex_; ///< Левая вершина
	int right_vertex_; ///< Правая вершина
};

QT_BEGIN_NAMESPACE
namespace Ui { class EdgeWidget; }
QT_END_NAMESPACE

///@brief Класс-виджет, служащий для создания ребра между вершинами
class EdgeWidget : public QWidget {
	Q_OBJECT
public:
	//! Конструктор виджета
	EdgeWidget(QWidget* parent = nullptr);
	//! Деструктор
	~EdgeWidget();
signals:
	/*!
	 * Сигнал, который передаёт связь между вершинами
	 * @param edge ребро
	 */
	void addEdge(const Edge& edge);
public slots:
	/*!
	 * Данный слот добавляет ребро
	 */
	void on_b_edge_clicked();
private:
	std::unique_ptr<Ui::EdgeWidget> ui_; ///< Форма виджета
};

#endif // !EDGEWIDGET_H
