#include "controlwidget.h"

/**
 * @brief Инициализация окна сообщений об ошибках
 * @param parent Родительский виджет, обычно nullptr
 */
ControlWidget::ControlWidget(QWidget *parent):
    QWidget(parent)
{
    setWindowTitle(tr("PartyZone launcher debug console"));

    setMinimumHeight(480);
    setMinimumWidth(640);

    treeWidget = new QTreeWidget(this);
    quitButton = new QPushButton(tr("Quit"), this);

    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    treeWidget->setColumnCount(3);
    treeWidget->setHeaderItem(new QTreeWidgetItem(QStringList() << "Category" << "Time" << "Message"));

    QGridLayout * la = new QGridLayout();

    la->addWidget(treeWidget, 0, 0);
    la->addWidget(quitButton, 1, 0);

    setLayout(la);
}

/**
 * @brief Сообщить об успешном или неудачном создании основного окна
 * @param obj Созданный объект QML-окна
 * @param objUrl URL файла с описанием QML-интерфейса
 *
 * В случае если основное QML-окно программы создано сделать окно сообщений об ошибках
 * невидимым. В противном случае сделать его видимым.
 */
void ControlWidget::reportObjectCreation(QObject *obj, const QUrl &objUrl)
{
    QString dt = QDateTime::currentDateTime().toString();

    if (obj) {
        treeWidget->addTopLevelItem(
                    new QTreeWidgetItem(
                        QStringList() << "Info"
                                      << dt
                                      << tr("Successfuly created QML object: %1").arg(objUrl.toString()))
                    );
        hide();
    } else {
        treeWidget->addTopLevelItem(
                    new QTreeWidgetItem(
                        QStringList() << "Critical"
                                      << dt
                                      << tr("Failed to create QML object: %1").arg(objUrl.toString()))
                    );
        show();
    }
}

/**
 * @brief Вывести в окно информацию о возникших ошибках QML
 * @param errors Список ошибок QML
 */
void ControlWidget::reportErrors(const QList<QQmlError> & errors)
{
    QString dt = QDateTime::currentDateTime().toString();

    for (const QQmlError & err: errors) {
        treeWidget->addTopLevelItem(
                    new QTreeWidgetItem(
                        QStringList() << "Warning"
                          << dt
                        << err.toString())
                    );
    }
}
