#pragma once

#include <boost/property_tree/ptree.hpp>
#include <string>

class IniConfig
{
public:
	/*���캯��*/
	IniConfig(std::string path = "win.ini") : path(path) { }

	/*��ȡINI*/
	bool ReadIni(std::string& path);

	/*д��INI*/
	bool	WriteIni(std::string& path);

	/*д���ַ���*/
	bool PutString(std::string& path, std::string value);

	/*��ȡkey��Ӧ��value*/
	template<typename T>
	T GetValue(std::string& path, T default)
	{ return conf.get<T>(path, default); }

	/*Ѱ��ĳ����ֵ*/
	size_t Count(std::string& path) const
	{ return conf.count(path); }

private:
	std::string								path;		//	ini·��
	boost::property_tree::ptree	conf;		// ini��д�ӿ�
};