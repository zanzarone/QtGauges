#ifndef RADIAL_BAR_CHART_H
#define RADIAL_BAR_CHART_H

#include <QWidget>
#include <QPropertyAnimation>

class QPaintEvent;

class RadialBarChart : public QWidget
{
    Q_OBJECT
public:
    enum Type {Radial, Gauge};
public:
    explicit RadialBarChart(QWidget *parent = nullptr);
    ~RadialBarChart();
    void setValue(int value, int total);
    void setDiscWidth(int width);
    int discWidth() const;
    bool drawBackground() const;
    void setDrawBackground(bool newDrawwBackground);
    const QColor &getColor() const;
    void setColor(const QColor &newForeground);
    void setCustomValue(const QString &newCustomValue);
    Type getType() const;
    void setType(Type newType);
    QFont getCustomFont() const;
    void setCustomFont(const QFont &newCustomFont);

private:
    const QColor &background() const;
    void setBackground(const QColor &newBackground);
    void paintTextValue(QPainter *painter);

signals:
    void valueChanged();

protected:
    void paintEvent(QPaintEvent *) override;

protected:
    int     m_value;
    int     m_endAngle;
    int     m_width;
    bool    m_drawBackground;
    bool    m_showPercentage;
    QColor  m_color;
    QColor  m_background;
    QString m_customValue;

protected:
    int m_valueTimerId;
    QPropertyAnimation m_animation;
    QFont customFont;
private:
    Type type;
};

#endif // RADIAL_BAR_CHART_H
