#pragma once
// Source - https://translated.turbopages.orghttps://translated.turbopages.org/proxy_u/en-ru.ru.0dc0c163-69d2473f-7176d641-74722d776562/https/stackoverflow.com/a
// Posted by IMAN4K, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-05, License - CC BY-SA 4.0

#pragma once
#include <QtWidgets>

class Switch : public QAbstractButton {
    Q_OBJECT
        Q_PROPERTY(int offset READ offset WRITE setOffset)
        Q_PROPERTY(QBrush brush READ brush WRITE setBrush)

public:
    Switch(QWidget* parent = nullptr);
    Switch(const QBrush& brush, QWidget* parent = nullptr);

    QSize sizeHint() const override;

    QBrush brush() const {
        return _brush;
    }
    void setBrush(const QBrush& brsh) {
        _brush = brsh;
    }

    int offset() const {
        return _x;
    }
    void setOffset(int o) {
        _x = o;
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void enterEvent(QEvent*) override;

private:
    bool _switch;
    qreal _opacity;
    int _x, _y, _height, _margin;
    QBrush _thumb, _track, _brush;
    QPropertyAnimation* _anim = nullptr;
};
