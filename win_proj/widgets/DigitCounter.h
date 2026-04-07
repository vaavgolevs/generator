#ifndef DIGITCOUNTER_H
#define DIGITCOUNTER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class QFocusEvent;

class DigitCounter : public QWidget
{
    Q_OBJECT

public:
    explicit DigitCounter(QWidget* parent = nullptr, int digits = 4);

    int value() const;
    void setValue(int val);
    void setRange(int min, int max);

    /// Dot after digit index \a digitIndex (0 � numDigits-2), or -1 to hide.
    /// Default when digits>=2: middle (e.g. 12.34 for 4 digits).
    void setDecimalSeparatorAfter(int digitIndex);
    int decimalSeparatorAfter() const;

signals:
    void valueChanged(int newValue);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void changeEvent(QEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

private:
    int m_minValue;
    int m_maxValue;
    int m_selectedDigit;
    QLabel* m_display;
    int m_numDigits;
    QString m_digits;
    int m_dotAfterDigit;

    int visualCellCount() const;
    int cellIndexToDigitIndex(int cellIndex) const;

    void updateDisplay();
    int digitsToValue() const;
    bool setDigit(int digitIndex, int newValue);
    QString formatValue(int value) const;
    void onIncrement();
    void onDecrement();
    int calculateOptimalFontSize() const;
};


#endif // DIGITCOUNTER_H
