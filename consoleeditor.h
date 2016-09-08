#ifndef CONSOLEEDITOR_H
#define CONSOLEEDITOR_H

#include <QPlainTextEdit>
#include <QTimer>
#include <QPainter>
class QPainter;
class QTimer;

class consoleEditor : public QPlainTextEdit
{
Q_OBJECT
    public:
        consoleEditor(QWidget* parent = 0);
        ~consoleEditor();
    public slots:
        void slot_BlinkCursor( );

    protected:
        void paintEvent( QPaintEvent* pEvent );

    private:
        bool bIsCursorVisible;
        QTimer* CursorTimer;
};

#endif // CONSOLEEDITOR_H
