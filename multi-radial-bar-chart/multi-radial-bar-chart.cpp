#include "multi-radial-bar-chart.h"

#include <QPaintEvent>
#include <QPainter>
#include <QConicalGradient>
#include <QPen>
#include <QPropertyAnimation>
#include <QDebug>
#include <QResizeEvent>

MultiRadialBarChart::MultiRadialBarChart(QWidget *parent) :
    QWidget(parent),
    m_values(QList<QPair<int,int>>()),
    m_width(5),
    m_drawBackground(false),
    m_enableAnimation(false),
    m_drawQuarter(true)
//    m_valueTimerId(0)
{
    this->setFixedSize(QSize(100,100));
}

MultiRadialBarChart::~MultiRadialBarChart()
{
}

void MultiRadialBarChart::resizeEvent(QResizeEvent *e)
{
    qDebug() << e->oldSize() << e->size() << this->parentWidget() << this->parentWidget()->frameSize();
    if( e->size().width() != e->size().height() ){
        if(e->size().width() < e->size().height()){
            this->resize( QSize(e->size().width(), e->size().width() ));
            e->ignore();
            return;
        }else{
            this->resize( QSize(e->size().height(), e->size().height() ));
            e->ignore();
            return;
        }
    }
    QWidget::resizeEvent(e);
}

void MultiRadialBarChart::setValues(QList<QPair<int,int>>& percentage)
{
    this->m_values = QList<QPair<int,int>>(percentage);
    update();
    emit valueChanged();
}

void MultiRadialBarChart::setColors(QList<QColor> &colors)
{
    m_colors = colors;
}

void MultiRadialBarChart::setDiscWidth(int width)
{
    m_width = width;
}

int MultiRadialBarChart::discWidth() const
{
    return m_width;
}

void MultiRadialBarChart::paintTextValue(QPainter * painter)
{   int offset = 0;
    for(int i=0; i<this->m_values.size(); i++){
        const QRect rectangle = QRect( rect().x() , rect().y() + offset , rect().width() / 2 - m_width , (rect().height() / 2 / this->m_values.size()) - m_width);
        const QString text = this->m_customValues.isEmpty() || this->m_customValues.at(i).isNull() || this->m_customValues.at(i).isEmpty() ? QString::number(this->m_values.at(i).first) : this->m_customValues.at(i);
//        float value = (float )this->m_loadingAngles.at(i).first;
        QPen pen = painter->pen();
        pen.setColor( this->colors().at(i));
        QFont font  = this->getCustomFont();//QFont("Nunito", 13);
        int size    = font.pointSize();
        bool finish = false;
        QFontMetrics fontMetrics(font);
        QRect boundingRect      = fontMetrics.boundingRect(rectangle, Qt::AlignCenter | Qt::AlignVCenter, text);
        const int step          = boundingRect.height() > rectangle.height() ? -1 : 1;
        const bool increaseFont = step > 0;
        do{
            if( increaseFont && (boundingRect.height() > rectangle.height() || (boundingRect.width() > rectangle.width() ) )){
                finish = true;
            }
            else if( !increaseFont && (boundingRect.height() < rectangle.height() || boundingRect.width() < rectangle.width() ) ){
                finish = true;
            }else{
                size += step;
                font.setPointSize(size + step);
                fontMetrics     = QFontMetrics(font);
                boundingRect    = fontMetrics.boundingRect(rectangle, Qt::AlignLeft | Qt::AlignVCenter, text);
            }
        }while( !finish );
        font.setPointSize(size);
        painter->setFont(font);
        painter->setPen(pen);
        painter->drawText(rectangle, Qt::AlignLeft | Qt::AlignVCenter, text);
        offset += rect().height() / 2 / this->m_values.size();
    }
}

QFont MultiRadialBarChart::getCustomFont() const
{
    return customFont;
}

void MultiRadialBarChart::setCustomFont(const QFont &newCustomFont)
{
    customFont = newCustomFont;
}

QList<QColor> MultiRadialBarChart::colors() const
{
    return m_colors;
}

void MultiRadialBarChart::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    paintTextValue(&painter);
    painter.restore();
    for(int i=0; i<this->m_values.size(); i++){
        float value = (float )this->m_values.at(i).first;
        value = value * (m_drawQuarter ? 270 : 360) / (float )(this->m_values.at(i).second == 0 ? (float )this->m_values.at(i).first * 2: (float )this->m_values.at(i).second);

        int offset =  4 * i ;
        int rectWidth = m_width + (m_width * i) ;
        QRect drawingRect;
        drawingRect.setX(rect().x() + rectWidth + offset);
        drawingRect.setY(rect().y() + rectWidth + offset);
        drawingRect.setWidth(rect().width() - (rectWidth+ offset) * 2);
        drawingRect.setHeight(rect().height() - (rectWidth+ offset) * 2);

        if(m_drawBackground){
            QColor background = (m_colors.at(i));
            background.setAlpha(64);
            int arcLengthApproximation = m_width + m_width;
            QPen pen = QPen(QBrush(background), m_width);
            pen.setCapStyle(Qt::RoundCap);
            painter.setPen(pen);
            painter.drawArc(drawingRect, 90 * 16 - arcLengthApproximation, -(m_drawQuarter ? 270 : 360) * 16);
        }

        QColor color = (m_colors.at(i));
        int arcLengthApproximation = m_width + m_width / 3;
        QPen pen = QPen(QBrush(color), m_width);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        painter.drawArc(drawingRect, 90 * 16 - arcLengthApproximation, -value * 16);
    }
}

bool MultiRadialBarChart::drawBackground() const
{
    return m_drawBackground;
}

void MultiRadialBarChart::setDrawBackground(bool newDrawwBackground)
{
    m_drawBackground = newDrawwBackground;
}

void MultiRadialBarChart::setCustomValues(QList<QString> &vals)
{
    this->m_customValues = QList<QString>(vals);
}
