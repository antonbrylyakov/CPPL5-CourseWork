#pragma once
#include "Constants.h"
#include <ostream>

class ReaderEvent
{
public:
	ReaderEvent();
	virtual ~ReaderEvent();
	friend std::ostream& operator<<(std::ostream& os, const ReaderEvent& evt);
protected:
	virtual void print(std::ostream& os) const = 0;
};