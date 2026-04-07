#ifndef TWOPOSITIONSWITCH_H
#define TWOPOSITIONSWITCH_H

#include <QWidget>

class TwoPositionSwitch : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY toggled USER true)

public:
    explicit TwoPositionSwitch(QWidget *parent = nullptr);

    bool isChecked() const { return m_checked; }
    void setChecked(bool on);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void flip();

    bool m_checked;
};

#endif
