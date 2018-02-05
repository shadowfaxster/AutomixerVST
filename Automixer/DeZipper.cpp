#include "DeZipper.h"

DeZipper::DeZipper()
{
	m_DZMM = 0.f;
	m_DZFB = 0.999f;
	m_DZFF = 1 - m_DZFB;
}

double DeZipper::smooth (double val)
{
	double temp = m_DZFF * val + m_DZMM*m_DZFB;
	m_DZMM = temp;

	return temp;
}

DeZipper::~DeZipper()
{
}
