#include "ReaderEvent.h"

ReaderEvent::ReaderEvent()
{
}

ReaderEvent::~ReaderEvent()
{
}

std::ostream& operator<<(std::ostream& os, const ReaderEvent& evt)
{
	evt.print(os);
	return os;
}
