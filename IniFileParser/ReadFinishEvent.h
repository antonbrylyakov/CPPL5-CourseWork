#include "ReaderEvent.h"

class ReadFinishEvent : public ReaderEvent
{
public:
	ReadFinishEvent(size_t line, size_t col);
protected:
	void print(std::ostream& os) const override;
};