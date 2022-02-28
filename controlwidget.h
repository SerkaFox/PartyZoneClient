#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QPushButton>
#include <QApplication>
#include <QGridLayout>
#include <QDateTime>
#include <QUrl>
#include <QQmlError>

/**
 * @brief Окно вывода сообщений об ошибках
 *
 * Окно вывода сообщений об ошибках при работе лаунчера. Обычно оно не отображается
 * на экране и появляется только в случае ошибки в работе приложения или невозможности
 * отобразить основное QML-окно
 */
class ControlWidget: public QWidget
{
    Q_OBJECT
public:
    /* Создать окно вывода сообщений об ошибках */
    explicit ControlWidget(QWidget *parent = nullptr);

public slots:
    /* Добавить сообщение об успешном или неудачном создании основного QML-окна */
    void reportObjectCreation(QObject *obj, const QUrl &objUrl);
    /* Добавить сообщение об ошибке при инициализации основного окна */
    void reportErrors(const QList<QQmlError> & errors);

private:
    /* Виджет с сообщениями об ошибках */
    QTreeWidget * treeWidget;

    /* Кнопка выхода из программы */
    QPushButton * quitButton;
};

#endif // CONTROLWIDGET_H
