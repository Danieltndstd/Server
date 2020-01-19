#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>

class IniConfig
{
public:
	/*构造函数*/
	IniConfig(std::string path = "win.ini") : path(path) { }

	/*读取INI*/
	bool ReadIni(std::string& path);

	/*写入INI*/
	bool	WriteIni(std::string& path);

	/*写入字符串*/
	bool PutString(std::string& path, std::string value);

	/*获取key对应的value*/
	template<typename T>
	T GetValue(std::string& path, T default)
	{ return conf.get<T>(path, default); }

	/*寻找某个键值*/
	size_t Count(std::string& path) const
	{ return conf.count(path); }

private:
	std::string								path;		//	ini路径
	boost::property_tree::ptree	conf;		// ini读写接口
};