#pragma once
#include "ReaderEvent.h"
#include <istream>

void ReadIniFile(std::istream& is, void (*evtCallback)(const ReaderEvent& evt));