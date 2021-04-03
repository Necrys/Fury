#pragma once

#include <string>

//-----------------------------------------------------------------------------

inline float str2float(const std::string& value)
{
	return (float)atof(value.c_str());
}
//-----------------------------------------------------------------------------

inline int str2int(const std::string& value)
{
	return (int)atoi(value.c_str());
}
//-----------------------------------------------------------------------------

inline std::string float2str(const float& value)
{
	char result_c[20];
	sprintf(result_c, "%f", value);
	return std::string(result_c);
}
//-----------------------------------------------------------------------------

inline std::string int2str(const int& value)
{
	char result_c[20];
	sprintf(result_c, "%d", value);
	return std::string(result_c);
}
//-----------------------------------------------------------------------------

inline std::string uint2str(const unsigned& value)
{
	char result_c[20];
	sprintf(result_c, "%u", value);
	return std::string(result_c);
}
//-----------------------------------------------------------------------------

inline std::string byte2hexwstr(const char& value)
{
	char result_c[20];
	sprintf(result_c, "%X", value);
	return std::string(result_c);
}
//-----------------------------------------------------------------------------

inline char* byte2hexstr(const char& value)
{
	static char buffer[4];
	memset(buffer, 0, sizeof(char) * 4);
	sprintf_s(buffer, sizeof(char) * 4, "%X", value);
	return buffer;
}
//-----------------------------------------------------------------------------