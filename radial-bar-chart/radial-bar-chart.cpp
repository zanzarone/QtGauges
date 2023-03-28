#include "radial-bar-chart.h"
//#include "ui_radial_bar_chart.h"

#include <QPaintEvent>
#include <QPainter>
#include <QConicalGradient>
#include <QPen>
#include <QDebug>

RadialBarChart::RadialBarChart(QWidget *parent) :
    QWidget(parent),
    m_value(0),
    m_width(5),
    m_drawBackground(false),
    m_showPercentage(true),
    m_color(Qt::red),
    m_background(Qt::black),
    m_valueTimerId(0), type(RadialBarChart::Type::Radial)
{
}

RadialBarChart::~RadialBarChart()
{
}

void RadialBarChart::setValue(int value, int total)
{
    this->m_endAngle = total;
    this->m_value = value;
    update();
    emit valueChanged();
}

void RadialBarChart::setDiscWidth(int width)
{
    m_width = width;
}

int RadialBarChart::discWidth() const
{
    return m_width;
}

void RadialBarChart::paintTextValue(QPainter * painter)
{
    const QRect rectangle = QRect(rect().x() + m_width,
                                  rect().center().y() - (( rect().height() / 4) / 2),
                                  rect().width() - m_width * 2,
                                  rect().height() / 4 );
    const QString text = this->m_customValue.isNull() || this->m_customValue.isEmpty() ? QString::number(this->m_value) : this->m_customValue;
    QPen pen = painter->pen();
    pen.setBrush(QBrush( this->getColor()) );
    QFont font  = this->getCustomFont();
    int size    = font.pointSize();
    bool finish = false;
    QFontMetrics fontMetrics(font);
    QRect boundingRect = fontMetrics.boundingRect(rectangle, Qt::AlignHCenter | Qt::AlignVCenter, text);
    const int step = boundingRect.height() > rectangle.height() ? -1 : 1;
    const bool increaseFont = step > 0;
    do{
        if( increaseFont && boundingRect.height() > rectangle.height() ){
            finish = true;
        }
        else if( !increaseFont && boundingRect.height() < rectangle.height() ){
            finish = true;
        }else{
            size += step;
            font.setPointSize(size + step);
            fontMetrics     = QFontMetrics(font);
            boundingRect    = fontMetrics.boundingRect(rectangle, Qt::AlignHCenter | Qt::AlignVCenter, text);
        }
    }while( !finish );
    font.setPointSize(size);
    painter->setFont(font);
    painter->setPen(pen);
    painter->drawText(rectangle, Qt::AlignHCenter | Qt::AlignVCenter, text);
}

void RadialBarChart::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    this->paintTextValue(&painter);
    painter.restore();

    QRect drawingRect;
    drawingRect.setX(rect().x() + m_width);
    drawingRect.setY(rect().y() + m_width);
    drawingRect.setWidth(rect().width() - m_width * 2);
    drawingRect.setHeight(rect().height() - m_width * 2);

    float value = (float )this->m_value;
    if(type == Radial)
        value = value * 360.0 / (float )m_endAngle;
    else
        value = value * 270.0 / (float )m_endAngle;

    if(m_drawBackground){
        QColor background = this->getColor();
        background.setAlpha(64);
        int arcLengthApproximation = m_width + m_width;
        QPen pen = QPen(QBrush(background), m_width);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        if(type == Radial)
            painter.drawArc(drawingRect, 90 * 16 - arcLengthApproximation, -360 * 16);
        else
            painter.drawArc(drawingRect, 225 * 16 - arcLengthApproximation, -270 * 16);
    }

    int arcLengthApproximation = m_width + m_width / 3;
    QPen pen = QPen(QBrush( this->getColor() ), m_width);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    if( type == Radial )
        painter.drawArc(drawingRect, 90 * 16 - arcLengthApproximation, -value * 16);
    else
        painter.drawArc(drawingRect, 230 * 16 - arcLengthApproximation, -value * 16);
}

QFont RadialBarChart::getCustomFont() const
{
    return customFont;
}

void RadialBarChart::setCustomFont(const QFont &newCustomFont)
{
    customFont = newCustomFont;
}

RadialBarChart::Type RadialBarChart::getType() const
{
    return type;
}

void RadialBarChart::setType(RadialBarChart::Type newType)
{
    type = newType;
}


void RadialBarChart::setCustomValue(const QString &newCustomValue)
{
    m_customValue = newCustomValue;
}

const QColor &RadialBarChart::getColor() const
{
    return m_color;
}

void RadialBarChart::setColor(const QColor &newForeground)
{
    m_color = newForeground;
}

const QColor &RadialBarChart::background() const{
    return m_background;
}

void RadialBarChart::setBackground(const QColor &newBackground){
    setDrawBackground(true);
    m_background = newBackground;
}

bool RadialBarChart::drawBackground() const{
    return m_drawBackground;
}

void RadialBarChart::setDrawBackground(bool newDrawwBackground){
    m_drawBackground = newDrawwBackground;
}
