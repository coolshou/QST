#ifndef CONSOLEEDITOR_H
#define CONSOLEEDITOR_H

#include <QPlainTextEdit>
#include <QTimer>
#include <QPainter>
#include <QPalette>

class QPainter;
class QTimer;

class consoleEditor : public QPlainTextEdit
{
Q_OBJECT
    signals:
        void getData(const QByteArray &data);

    public:
        explicit consoleEditor(QWidget* parent = 0);
        ~consoleEditor();
        void set_theme(QColor fontColor, QColor bgColor);
        void putData(const QByteArray &data);
        void setAutoScroll(bool autoScroll);

    public slots:
        void slot_BlinkCursor( );

    protected:
        void paintEvent( QPaintEvent* pEvent );
        virtual void keyPressEvent(QKeyEvent *e);

    private slots:
        void updatePointSize(int point);

    private:
        bool localEchoEnabled;
        bool bIsCursorVisible;
        bool bAutoScroll;
        QTimer* CursorTimer;
        QColor m_fontColor;
        QColor m_bgColor;
        int m_pointSize;
};

#endif // CONSOLEEDITOR_H
