#ifndef _PARAM_ED_H_
#define _PARAM_ED_H_
 
#include <QtWidgets/QtWidgets>


class param_ed: public QLineEdit
{
Q_OBJECT
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* pe);
	virtual void keyPressEvent(QKeyEvent * pe);
	int i_max;
	int i_min;
	int i_inc_step;
	unsigned char *p_cdata;
	unsigned short *p_sdata;
	unsigned long *p_ldata;
	bool any_elem;
	int num_dig;
	int coeff;
	int num_dights;
	int shift;
	bool ena_enter_on_off;
	bool ena_edit;


	inline void ApplyNewText();

	inline int GetMinimalWidth() const;

private:

	/// +1 / −1: шаг соответствующего разряда под курсором; false — нечего менять
	bool adjustDigitAtCursor(int direction);
	void restoreCursorAfterDigitAdjust(int savedCursorPos);
	void navigateToAdjacentField(bool goUp);

	template<typename T> inline void inc_par_val(T *value, const int count);
	template<typename T> inline void dec_par_val(T *value, const int count);


public:

    param_ed(QWidget *parent=0);

	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;


	void set_ena_edit(bool iena_edit);

	void inc_par(const int count = 1);
	void dec_par(const int count = 1);


	void set_ena_enter_on_off(bool ena);
	void set_min_max(int i_min,int i_max);
	void set_inc_step(int istep);
	void set_num_dig(int inum_dig);
	void set_coeff(int icoeff);
	void set_data(unsigned char *p_data);
	void set_data(unsigned short *p_data);
	void set_data(unsigned long *p_data);
	void set_colors(const char *col);
	void show_par();

	void set_shift(int);

	void apply_current();


signals:
	void on_off();
	void param_changed();
};



template<typename T> void param_ed::inc_par_val(T *value, const int count)
{
	const long long stepLL = static_cast<long long>(i_inc_step) * count;
	const long long v = static_cast<long long>(*value);
	const long long inc_data = v + stepLL;

	if (inc_data > static_cast<long long>(i_max))
		*value = static_cast<T>(i_max);
	else if (inc_data < static_cast<long long>(i_min))
		*value = static_cast<T>(i_min);
	else
		*value = static_cast<T>(inc_data);
}

template<typename T> void param_ed::dec_par_val(T *value, const int count)
{
	const long long stepLL = static_cast<long long>(i_inc_step) * count;
	const long long v = static_cast<long long>(*value);
	const long long iMax = static_cast<long long>(i_max);
	const long long iMin = static_cast<long long>(i_min);

	if (v >= iMax + stepLL)
		*value = static_cast<T>(i_max);
	else if (v >= iMin + stepLL)
		*value = static_cast<T>(v - stepLL);
	else
		*value = static_cast<T>(i_min);
}

 
#endif // _PARAM_ED_H_