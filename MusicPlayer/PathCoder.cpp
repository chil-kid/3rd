#include "PathCoder.h"

std::ostringstream PathCoder::CombinePath(const std::string& Path, char* Name) {
	std::ostringstream oss;
	oss << Path << '\\' << Name;
	return oss;
}