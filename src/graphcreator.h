#ifndef GRAPHCREATOR_H
#define GRAPHCREATOR_H

#include "edgewidget.h"

#include <QWidget>

#include <memory>
#include <vector>
#include <unordered_set>

///@brief Структура, хранящая информацию о графе
struct Graph {
	size_t vertex_count_; ///< Количество вершин в графе
	std::vector<std::vector<int16_t>> adj_matrix_; ///< Матрица смежности графа
};

///@brief Класс, проверяющий граф на связность
class GraphChecker {
public:
	//! Конструктор
	GraphChecker(const Graph& graph);
	//! Деструктор по умолчанию
	~GraphChecker() = default;
	/*!
	 * Данный метод выдаёт количество смежных вершин
	 */
	size_t realCountVertexes() const;
	/*!
	 * Данный метод сравнивает количество смежных вершин и общее число вершин графа
	 */
	bool isGraph() const;
private:
	/*!
	 *  Рекурсивный поиск смежных вершин графа
	 *  @param graph граф
	 *  @param vertex_number номер вершины
	 */
	void checkVertexOnGraph(const Graph& graph, const size_t vertex_number);
private:
	bool is_graph; ///< Флаг проверки
	std::unordered_set<size_t> vertexes; ///< Множество смежных вершин графа
};

QT_BEGIN_NAMESPACE
namespace Ui { class GraphCreator; }
QT_END_NAMESPACE

///@brief Класс-виджет, служащий для создания графа
class GraphCreator : public QWidget {
	Q_OBJECT
public:
	//! Конструктор виджета
	GraphCreator(QWidget* parent = nullptr);
	//! Деструктор
	~GraphCreator();
signals:
	/*!
	 * Сигнал, который передаёт граф для отрисовки
	 * @param graph граф
	 */
	void visualizeGraph(const Graph& graph);
public slots:
	/*!
	 * Данный слот добавляет связь между вершинами
	 * @param edge ребро
	 */
	void addEdge(const Edge& edge);
	/*!
	 * Данный слот создаёт граф
	 */
	void on_b_create_clicked();
	/*!
     * Данный слот удаляет связь между вершинами
     */
	void on_b_del_edge_clicked();
	/*!
	 * Данный слот запускает виджет для создания связи между вершинами
	 */
	void on_b_new_edge_clicked();
	/*!
	 * Данный слот даёт возможность удалить выбранную связь
	 * @param currentRow выбранная строка, в списке рёбер-связей
	 */
	void on_list_edges_currentRowChanged(int currentRow);
private:
	/*!
	* Обработчик событий закрытия виджета
	* @param event событие закрытия
	*/
	void closeEvent(QCloseEvent* event);
private:
	std::unique_ptr<Ui::GraphCreator> ui_; ///< Форма виджета
	EdgeWidget edge_widget_; ///< Виджет, служащий для создания ребра между вершинами
	QList<Edge> edges_; ///< Список рёбер
};

#endif // !GRAPHCREATOR_H


