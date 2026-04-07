
#include "DigitCounter.h"
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPalette>
#include <QEvent>
#include <QFocusEvent>

DigitCounter::DigitCounter(QWidget* parent, int digits)
    : QWidget(parent), m_minValue(0), m_maxValue(9999),
    m_selectedDigit(0), m_numDigits(digits),
    m_digits(QString(digits, QLatin1Char('0'))),
    m_dotAfterDigit(digits >= 2 ? digits / 2 - 1 : -1)
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    m_display = new QLabel(this);
    m_display->setAlignment(Qt::AlignCenter);
    m_display->setWordWrap(false);
    m_display->setFrameStyle(QFrame::Box | QFrame::Sunken);
  m_display->setFont(QFont(QStringLiteral("Courier New"), 16, QFont::Bold));
    m_display->setFont(QFont(QStringLiteral("Courier New"), 16, QFont::Normal));

    m_display->installEventFilter(this);
    mainLayout->addWidget(m_display);
    setFocusPolicy(Qt::StrongFocus);

    updateDisplay();
}
bool DigitCounter::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_display && event->type() == QEvent::MouseButtonPress) {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        setFocus(Qt::MouseFocusReason);

        if (m_digits.length() != m_numDigits) {
            qWarning() << "DigitCounter::eventFilter: invalid m_digits length:" << m_digits;
            return true;
        }

        int totalWidth = m_display->width();
        if (totalWidth <= 0) {
            qDebug() << "DigitCounter::eventFilter: zero display width";
            return true;
        }

        // Расчёт ширины цифры с учётом отступов
        int padding = 10;
        int usableWidth = totalWidth - 2 * padding;
        const int cells = visualCellCount();
        int charWidth = qMax(1, usableWidth / cells);

        int clickX = mouseEvent->x();
        int cell = 0;
        if (clickX < padding) {
            cell = 0;
        }
        else if (clickX > totalWidth - padding) {
            cell = cells - 1;
        }
        else {
            cell = (clickX - padding) / charWidth;
            cell = qBound(0, cell, cells - 1);
        }

        const int digitIndex = cellIndexToDigitIndex(cell);
        if (digitIndex >= 0)
            m_selectedDigit = digitIndex;

        qDebug() << "Mouse click at x:" << mouseEvent->x()
            << "Total width:" << totalWidth
            << "Char width:" << charWidth
            << "Selected digit:" << m_selectedDigit;

        updateDisplay();
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void DigitCounter::keyPressEvent(QKeyEvent* event)
{
    const int key = event->key();
    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        const int d = key - Qt::Key_0;
        if (setDigit(m_selectedDigit, d) && m_selectedDigit < m_numDigits - 1) {
            ++m_selectedDigit;
            updateDisplay();
        }
        return;
    }

    switch (event->key()) {
    case Qt::Key_Plus:
    case Qt::Key_Equal:
    case Qt::Key_Greater:
        onIncrement();
        break;
    case Qt::Key_Minus:
    case Qt::Key_Underscore:
    case Qt::Key_Less:
        onDecrement();
        break;
    case Qt::Key_Left:
        if (m_selectedDigit > 0) {
            --m_selectedDigit;
            updateDisplay();
        }
        break;
    case Qt::Key_Right:
        if (m_selectedDigit < m_numDigits - 1) {
            ++m_selectedDigit;
            updateDisplay();
        }
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void DigitCounter::onIncrement()
{
    int currentValue = digitsToValue();
    if (currentValue >= m_maxValue) 
        return;

    QString numStr = m_digits;
    int digit = numStr.at(m_selectedDigit).digitValue();

    if (digit < 9) {
        setDigit(m_selectedDigit, digit + 1);
    }
    else {
        // Перенос в следующий разряд
        setDigit(m_selectedDigit, 0);
        if (m_selectedDigit > 0) {
            setDigit(m_selectedDigit - 1, numStr.at(m_selectedDigit - 1).digitValue() + 1);
        }
    }
}

void DigitCounter::onDecrement()
{
    int currentValue = digitsToValue();
    if (currentValue <= m_minValue) 
        return;

    QString numStr = m_digits;
    int digit = numStr.at(m_selectedDigit).digitValue();

    if (digit > 0) {
        setDigit(m_selectedDigit, digit - 1);
    }
    else {
        // Заём из следующего разряда
        setDigit(m_selectedDigit, 9);
        if (m_selectedDigit > 0) {
            setDigit(m_selectedDigit - 1, numStr.at(m_selectedDigit - 1).digitValue() - 1);
        }
    }
}

int DigitCounter::value() const
{
    return digitsToValue();
}

void DigitCounter::setValue(int val)
{
    val = qBound(m_minValue, val, m_maxValue);

    m_digits = formatValue(val);
    updateDisplay();

    emit valueChanged(val);
}

void DigitCounter::setRange(int min, int max)
{
    m_minValue = min;
    m_maxValue = max;
}

void DigitCounter::setDecimalSeparatorAfter(int digitIndex)
{
    if (digitIndex < -1 || digitIndex > m_numDigits - 2)
        return;
    m_dotAfterDigit = digitIndex;
    m_selectedDigit = qBound(0, m_selectedDigit, m_numDigits - 1);
    updateDisplay();
}

int DigitCounter::decimalSeparatorAfter() const
{
    return m_dotAfterDigit;
}

int DigitCounter::visualCellCount() const
{
    if (m_dotAfterDigit < 0)
        return m_numDigits;
    return m_numDigits + 1;
}

int DigitCounter::cellIndexToDigitIndex(int cellIndex) const
{
    if (cellIndex < 0 || cellIndex >= visualCellCount())
        return -1;
    if (m_dotAfterDigit < 0)
        return cellIndex;
    if (cellIndex <= m_dotAfterDigit)
        return cellIndex;
    if (cellIndex == m_dotAfterDigit + 1)
        return -1;
    return cellIndex - 1;
}

bool DigitCounter::setDigit(int digitIndex, int newValue)
{
    if (newValue < 0 || newValue > 9 || digitIndex < 0 || digitIndex >= m_numDigits)
        return false;

    const QString prev = m_digits;
    m_digits[digitIndex] = QLatin1Char(static_cast<char>('0' + newValue));

    bool ok = false;
    const int newValueInt = m_digits.toInt(&ok);
    if (!ok) {
        m_digits = prev;
        updateDisplay();
        return false;
    }
    if (newValueInt >= m_minValue && newValueInt <= m_maxValue) {
        emit valueChanged(newValueInt);
        updateDisplay();
        return true;
    }
    m_digits = prev;
    updateDisplay();
    return false;
}

int DigitCounter::digitsToValue() const
{
    if (m_digits.length() != m_numDigits) {
        qWarning() << "DigitCounter::digitsToValue: invalid m_digits length:" << m_digits;
        return 0;
    }
    bool ok = false;
    const int value = m_digits.toInt(&ok);
    if (!ok) {
        qWarning() << "DigitCounter::digitsToValue: invalid m_digits:" << m_digits;
        return 0;
    }
    return value;
}

QString DigitCounter::formatValue(int value) const
{
    value = qBound(m_minValue, value, m_maxValue);
    QString result = QString("%1").arg(value, m_numDigits, 10, QChar('0'));
    qDebug() << "formatValue:" << value << "->" << result;
    return result;
}

void DigitCounter::updateDisplay()
{
    if (m_digits.length() != m_numDigits) {
        bool ok = false;
        int v = m_digits.toInt(&ok);
        if (!ok)
            v = m_minValue;
        m_digits = formatValue(qBound(m_minValue, v, m_maxValue));
    }

    m_selectedDigit = qBound(0, m_selectedDigit, m_numDigits - 1);

    const QColor hiBg = palette().color(QPalette::Highlight);
    const QColor hiFg = palette().color(QPalette::HighlightedText);
    const QColor fg = palette().color(QPalette::WindowText);

    const int pt = m_display->font().pointSize();

    QString html = QStringLiteral("<span style=\"font-weight: bold; font-size: %1pt;\">")
        .arg(pt);

    const bool showSelection = hasFocus();

    for (int i = 0; i < m_numDigits; ++i) {
        const QString ch = QString(m_digits.at(i));
        if (showSelection && i == m_selectedDigit) {
            html += QStringLiteral("<span style='background-color: %1; color: %2; border-radius: 2px; padding: 0 2px;'>%3</span>")
                .arg(hiBg.name(), hiFg.name(), ch);
        } else {
            html += QStringLiteral("<span style='color: %1;'>%2</span>")
                .arg(fg.name(), ch);
        }
        if (m_dotAfterDigit >= 0 && i == m_dotAfterDigit) {
            html += QStringLiteral("<span style=\"color:%1;font-weight:bold;font-size:%2pt;\">.</span>")
                .arg(fg.name())
                .arg(pt);
        }
    }
    html += QStringLiteral("</span>");

    m_display->setTextFormat(Qt::RichText);
    m_display->setText(html);
}

int DigitCounter::calculateOptimalFontSize() const
{
    if (!m_display) return 12;

    int availableWidth = this->width() - 20; // отступ 10 px слева и справа
    int availableHeight = this->height() - 10; // отступ сверху и снизу

    if (availableWidth <= 0 || availableHeight <= 0) return 8;

    const int cells = visualCellCount();
    double widthPerDigit = static_cast<double>(availableWidth) / qMax(1, cells);
    double heightBased = availableHeight * 0.8; // 80 % высоты под шрифт

    // Берём минимум — ограничиваем по ширине или высоте
    double fontSizeDouble = qMin(widthPerDigit, heightBased);

    int fontSize = static_cast<int>(fontSizeDouble);
    fontSize = qBound(6, fontSize, 60); // разумные границы

    return fontSize;
}
void DigitCounter::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    int newFontSize = calculateOptimalFontSize();
    QFont font = m_display->font();
    font.setPointSize(newFontSize);
    m_display->setFont(font);

    // Перерисовываем, чтобы применить новый шрифт
    updateDisplay();
}

void DigitCounter::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::PaletteChange)
        updateDisplay();
    QWidget::changeEvent(event);
}

void DigitCounter::focusInEvent(QFocusEvent* event)
{
    QWidget::focusInEvent(event);
    updateDisplay();
}

void DigitCounter::focusOutEvent(QFocusEvent* event)
{
    QWidget::focusOutEvent(event);
    updateDisplay();
}