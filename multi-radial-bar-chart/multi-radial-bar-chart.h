#ifndef MULTI_RADIAL_BAR_CHART_H
#define MULTI_RADIAL_BAR_CHART_H


#include <QWidget>
#include <QParallelAnimationGroup>

class QPaintEvent;

class MultiRadialBarChart : public QWidget
{
    Q_OBJECT
public:
    explicit MultiRadialBarChart(QWidget *parent = nullptr);
    ~MultiRadialBarChart();
    bool drawBackground() const;
    void setDrawBackground(bool newDrawwBackground);
    void setEnableAnimation(bool newEnableAnimation);
    void setCustomValues(QList<QString > &vals);
    void setValues(QList<QPair<int, int> > &degree);
    void setColors(QList<QColor> &colors);
    QList<QColor> colors() const;
    QFont getCustomFont() const;
    void setCustomFont(const QFont &newCustomFont);
    void setDiscWidth(int width);
    int discWidth() const;

signals:
    void valueChanged();

protected:
    void resizeEvent(QResizeEvent * e) override;
    void paintEvent(QPaintEvent *) override;
private:
    void paintTextValue(QPainter *painter);

private:
    QList<QString > m_customValues;
    QList<QPair<int,int>> m_values;
    QList<QColor> m_colors;
    int m_width;

private:
    bool m_drawBackground;
    bool m_enableAnimation;
    bool m_drawQuarter;
    QFont customFont;
private:
//    int m_valueTimerId;
    //    QParallelAnimationGroup m_animations;
};


#endif // MULTI_RADIAL_BAR_CHART_H
