#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent):QPlainTextEdit(parent)
{

    lineNumberArea = new LineNumberArea(this);
    highlighter = new Highlighter(this->document());

    connect(this,&CodeEditor::blockCountChanged,this,&CodeEditor::updateLineNumberAreaWidth);
    connect(this,&CodeEditor::updateRequest,this,&CodeEditor::updateLineNumberArea);
    connect(this,&CodeEditor::cursorPositionChanged,this,&CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

}

void CodeEditor::updateLineNumberAreaWidth(int newBlockCount){
    Q_UNUSED(newBlockCount);
    setViewportMargins(lineNumberAreaWidth(),0,0,0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy){

    if (dy > 0){
        lineNumberArea->scroll(0,dy);
    }
    else{
        lineNumberArea->update(0,rect.y(),lineNumberArea->width(),rect.height());
    }

    if (rect.contains(viewport()->rect())){
        updateLineNumberAreaWidth(0);
    }

}

void CodeEditor::resizeEvent(QResizeEvent *event){

    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();

    // Resizing the line number area.
    lineNumberArea->setGeometry(QRect(cr.left(),cr.top(),lineNumberAreaWidth(),cr.height()));

}

void CodeEditor::keyPressEvent(QKeyEvent *e){

    if (e->key() == Qt::Key_Return){
        QString prevtab = "";
        QString cline = this->textCursor().block().text();

        for (qint32 i = 0; i < cline.size(); i++){
            if (cline.at(i) != ' '){
                break;
            }
            else{
                prevtab = prevtab + " ";
            }
        }
        QPlainTextEdit::keyPressEvent(e);
        this->insertPlainText(prevtab);
    }
    else{
        QPlainTextEdit::keyPressEvent(e);
    }
}

void CodeEditor::highlightCurrentLine(){

    QList<QTextEdit::ExtraSelection> exSel;

    if (!isReadOnly()){

        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(highlightColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection,true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        exSel.append(selection);

    }

    setExtraSelections(exSel);

}

qint32 CodeEditor::lineNumberAreaWidth(){

    qint32 digits = 1;
    qint32 max = qMax(1,blockCount());
    while (max >= 10){
        max = max / 10;
        digits++;
    }

    qint32 space = 3 + fontMetrics().width(QLatin1Char('9'))*digits;
    return space;
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event){

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(),Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    qint32 blockNumber = block.blockNumber();
    qint32 top = (qint32)(blockBoundingGeometry(block).translated(contentOffset()).top());
    qint32 bottom = top + (qint32)(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()){

        if (block.isVisible() && bottom > event->rect().top()){

            QString number = QString::number(blockNumber+1);
            painter.setPen(Qt::black);
            painter.drawText(0,top,lineNumberAreaWidth(),fontMetrics().height(), Qt::AlignRight|Qt::AlignVCenter, number);

        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        blockNumber++;

    }

}

Highlighter::Highlighter(QTextDocument *parent):QSyntaxHighlighter(parent){

}

void Highlighter::highlightBlock(const QString &text){

    foreach (const HighlightingRule rule, highlightingRules){

        QRegExp expression(rule.pattern);
        qint32 index = expression.indexIn(text);
        while (index >= 0) {

            qint32 length = expression.matchedLength();
            setFormat(index,length,rule.format);
            index = expression.indexIn(text,index+length);

        }

    }

}

void Highlighter::setWordGroupFormat(QStringList keyword, QTextCharFormat format){

    foreach (QString word, keyword){
        HighlightingRule rule;
        rule.pattern = QRegExp("\\b" + word + "\\b");
        rule.format = format;
        highlightingRules << rule;
    }

}

void Highlighter::setLineCommentFormat(QString commentChar, QTextCharFormat format){
    HighlightingRule rule;
    rule.pattern = QRegExp(commentChar+"[^\n]*");
    rule.format = format;
    highlightingRules << rule;
}

void Highlighter::setNumberFormat(QTextCharFormat format){

    HighlightingRule rule;
    rule.pattern = QRegExp("[^#]\\b[0-9]+\\b");
    rule.format = format;
    highlightingRules << rule;

}
