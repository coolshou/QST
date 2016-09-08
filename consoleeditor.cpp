#include <QDebug>
#include "consoleeditor.h"

consoleEditor::consoleEditor(QWidget* parent)
: QPlainTextEdit( parent )
{
    bIsCursorVisible = true;
    CursorTimer = new QTimer( this );
    CursorTimer->setInterval( 500 );
    connect( CursorTimer, SIGNAL( timeout() ), this, SLOT( slot_BlinkCursor() ));

    CursorTimer->start();
}

consoleEditor::~consoleEditor()
{
//nothing
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
        r.setWidth(1);
        oPainter.fillRect( r, Qt::SolidPattern );
    }
    QPlainTextEdit::paintEvent( pEvent );

}
