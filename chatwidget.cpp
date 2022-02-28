#include "chatwidget.h"

ChatWidget::ChatWidget(QWidget *parent):
    QWidget(parent)
{
    setWindowTitle(tr("PartyZone chat"));

#if !WITHOUT_CHAT
    browserView = new QWebView(this);

    QGridLayout * la = new QGridLayout();
    la->addWidget(browserView, 0, 0);
    setLayout(la);
#endif
}

void ChatWidget::openUrl(const QUrl & url)
{
#if !WITHOUT_CHAT
    browserView->load(url);
#endif
}

void ChatWidget::showDelayed(int timeout)
{
#if !WITHOUT_CHAT
    QTimer::singleShot(timeout * 1000, this, &ChatWidget::show);
#endif
}

void ChatWidget::show()
{
#if !WITHOUT_CHAT
    QWidget::show();

    setFocus(Qt::OtherFocusReason);
#endif
}

void ChatWidget::keyPressEvent(QKeyEvent *event)
{
#if !WITHOUT_CHAT
    /* Скрыть окно по нажатию на Alt+C */
    if ((event->modifiers() & Qt::AltModifier) && event->key() == Qt::Key_C) {
        hide();
    }
#endif
}
