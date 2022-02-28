#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QGridLayout>
#include <QTimer>
#include <QKeyEvent>
#include <QAction>

#if !WITHOUT_CHAT
#include <QtWebKitWidgets/QWebView>
#endif


/**
 * @brief Окно игрового чата
 *
 * Появляется автоматически через 15 секунд после старта игры, может быть скрыто
 * и показано снова по Alt+C
 */
class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = nullptr);

public slots:
    void openUrl(const QUrl & url);
    void showDelayed(int timeout=15);
    void show();

signals:

protected:
    virtual void keyPressEvent(QKeyEvent * event);

private:
#if !WITHOUT_CHAT
    QWebView * browserView;
#endif
};

#endif // CHATWIDGET_H
