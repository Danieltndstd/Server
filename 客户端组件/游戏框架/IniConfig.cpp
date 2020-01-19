#include "Stdafx.h"
#include "IniConfig.h"
#include <boost/property_tree/ini_parser.hpp>

bool IniConfig::ReadIni(std::string& path)
{
	this->path = path;

	try
	{
		read_ini(path, conf);
	}
	catch (std::exception&)
	{
		return false;
	}

	return true;
}

bool IniConfig::WriteIni(std::string& path)
{
	this->path = path;

	try
	{
		write_ini(path, conf);
	}
	catch (std::exception&)
	{
		return false;
	}

	return true;
}

bool IniConfig::PutString(std::string& path, std::string value)
{
	try
	{
		conf.put(path, value);
	}
	catch (std::exception&)
	{
		return false;
	}

	return true;
}