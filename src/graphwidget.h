#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include "graphcreator.h"

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QSettings>

#include <memory>

#define PI 3.14159265 ///< Число Пи

/*!
 * Данный метод переводит градус в радиан
 * @param degree граду
 */
double degreeToRadian(double degree);

QT_BEGIN_NAMESPACE
namespace Ui { class GraphWidget; }
QT_END_NAMESPACE

///@brief Класс-виджет, визуализирующий граф
class GraphWidget : public QMainWindow {
	Q_OBJECT
public:
	//! Конструктор виджета
	GraphWidget(QWidget* parent = nullptr);
	//! Деструктор
	~GraphWidget();
public slots:
	/*!
	 * Данный слот запускает виджет для создания графа
	 */
	void createGraph();
	/*!
	 * Данный слот открывает файл для визуализации графа
	 */
	void openGraph();
	/*!
	 * Данный слот сохраняет граф в виде файла рядом с исполняемым файлом
	 */
	void saveGraph();
	/*!
	 * Данный слот сохраняет граф в виде файла в нужной директории
	 */
	void saveGraphAs();
	/*!
	 * Данный слот отрисовывает граф
	 * @param graph граф
	 */
	void visualizeGraph(const Graph& graph);
private:
	/// Перечисление вариантов сохранения
	enum class Saving {
		Yes = 0,
		No = 1,
		Cancel = 2
	};
private:
	/*!
	* Обработчик событий закрытия виджета
	* @param event событие закрытия
	*/
	void closeEvent(QCloseEvent* event);
	/*!
	* Данный метод проверят был ли сохранён прошлый граф
	*/
	Saving isOldGraphSave();
	/*!
	* Данный метод присваивает сохранённые параметры в реестре
	*/
	void loadSettings();
	/*!
	* Данный метод сохраняет информацию о графе в файле
	* @param filename имя файла
	*/
	void saveGraphFile(const QString& filename);
	/*!
	 * Данный метод сохраняет заданные параметры в реестре
	 */
	void saveSettings();
	/*!
	 * Данный метод вычисляет центр вершины на сцене
	 * @param radius радиус
	 * @param vertex_number номер вершины
	 * @param degree угол
	 */
	QPointF vertexCoordinate(double radius, const size_t vertex_number, const double degree);
private:
	const double ROUND{ 360.0 }; ///< Оборот
	const size_t VERTEX_SIZE{ 50 }; ///< Размер вершины

	bool is_graph_save_{ true }; ///< Состояние сохранения графа
	Graph graph_; ///< Граф
	std::unique_ptr <Ui::GraphWidget> ui_; ///< Форма виджета
	std::unique_ptr<QSettings> settings_{ new QSettings("settings.ini", QSettings::IniFormat, this) }; ///< Настройки
	GraphCreator graph_creator_; ///< Виджет для создания графа
	std::shared_ptr<QGraphicsScene> scene_{ new QGraphicsScene() }; ///< Сцена для отрисовки графа
};

#endif // !GRAPHWIDGET_H