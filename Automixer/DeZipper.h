#pragma once
#ifndef DezipperH
#define DezipperH

class DeZipper
{
public:
	DeZipper();
	~DeZipper();
	double smooth(double val);
private:
	double m_DZMM;
	double m_DZFB;
	double m_DZFF;
};
#endif

