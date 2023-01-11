#include "ReadFinishEvent.h"

ReadFinishEvent::ReadFinishEvent(size_t line, size_t col): ReaderEvent(line, col)
{
}

void ReadFinishEvent::print(std::ostream& os) const
{
	os << "Конец входного потока";
}
