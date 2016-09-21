
#include "consoleeditor.h"
#include <QDebug>


consoleEditor::consoleEditor(QWidget* parent)
: QPlainTextEdit( parent )
  , localEchoEnabled(false)
{
    bool bBlinkCursor=true;
    if (bBlinkCursor) {
        bIsCursorVisible = true;
        //BlinkCursor
        CursorTimer = new QTimer( this );
        CursorTimer->setInterval( 500 );
        connect( CursorTimer, SIGNAL( timeout() ), this, SLOT( slot_BlinkCursor() ));
        CursorTimer->start();
    }
    m_fontColor = Qt::black;
    //font size
    QFont font = this->font();
    //QFont font = QWidget.font();
    //font.setPointSize();
    m_pointSize = font.pointSize();
    //connect(this,SIGNAL(zoomIn(int)),this, SLOT(updatePointSize(int)));
    //connect(this,SIGNAL(zoomOut(int)),this, SLOT(updatePointSize(int)));
}

consoleEditor::~consoleEditor()
{
//nothing
}

void consoleEditor::updatePointSize(int point)
{
    m_pointSize = point;
}

void consoleEditor::set_theme(QColor fontColor, QColor bgColor)
{
    m_fontColor = fontColor;
    m_bgColor = bgColor;
    QPalette p = palette();
    p.setColor(QPalette::Base, m_bgColor);
    p.setColor(QPalette::Text, m_fontColor);
    setPalette(p);
}

void consoleEditor::putData(const QByteArray &data)
{
    insertPlainText(QString(data));
    if (bAutoScroll) {
        moveCursor(QTextCursor::End);
        ensureCursorVisible();
    //QScrollBar *bar = verticalScrollBar();
    //bar->setValue(bar->maximum());
    }
}
void consoleEditor::setAutoScroll(bool autoScroll)
{
    bAutoScroll = autoScroll;
}

void consoleEditor::slot_BlinkCursor( )
{
    if( bIsCursorVisible )
    {
        bIsCursorVisible = false;
    }
    else
    {
        bIsCursorVisible = true;
    }

    viewport()->update();
}

void consoleEditor::paintEvent( QPaintEvent* pEvent )
{
    QPainter oPainter( viewport() );

    if( bIsCursorVisible )
    {
        QRect r = cursorRect();
        //TODO: adjust curser width to char width
        //qDebug() << "m_pointSize:"<<m_pointSize;
        r.setWidth(m_pointSize);

        //TODO: show curser on dark theme
        oPainter.fillRect( r, Qt::SolidPattern );
        oPainter.fillRect( r, m_fontColor );

    }
    QPlainTextEdit::paintEvent( pEvent );

}

void consoleEditor::keyPressEvent(QKeyEvent *e)
{
    //TODO: ctrl+c
    switch (e->key()) {
    case Qt::Key_Backspace:
        qDebug() << "Key_Backspace"<<e->text().toLatin1();//.toLocal8Bit();
        emit getData(e->text().toLatin1());
        break;
    case Qt::Key_Left:
        qDebug() << "Key_Left";
        break;
    case Qt::Key_Right:
        qDebug() << "Key_Right";
        break;
    case Qt::Key_Up:
        qDebug() << "Key_Up";
        break;
    case Qt::Key_Down:
        qDebug() << "Key_Down";
        break;
    default:
        if (localEchoEnabled)
            QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
    }
}
