#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QSyntaxHighlighter>
#include <QFontDatabase>

class Highlighter : public QSyntaxHighlighter {

    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

    void setWordGroupFormat(QStringList keyword, QTextCharFormat format);
    void setLineCommentFormat(QString commentChar, QTextCharFormat format);
    void setNumberFormat(QTextCharFormat format);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:

    struct HighlightingRule {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

};

class CodeEditor : public QPlainTextEdit        
{
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    qint32 lineNumberAreaWidth();

    void setHighlightLineColor(QColor c){highlightColor = c;}

    Highlighter *syntaxHighlighter() const { return highlighter; }

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
    Highlighter *highlighter;
    QColor highlightColor;

};

class LineNumberArea : public QWidget {

public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor){
        codeEditor = editor;
    }

    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(codeEditor->lineNumberAreaWidth(),0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE{
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;

};




#endif // CODEEDITOR_H
