//**********************************************************************
// Copyright (c) 2017
// 迪斯特软件开发小组.
// 文件: Jinja2.cpp
// 内容: 本模型提供一个简化版的Jinja2模板引擎实现
// 历史:
//	序号		修改时间		修改人		修改内容
//	1			2017-09-08		胡乐秋		首次生成
//*********************************************************************

//包含头文件
#include "jinja2.h"
#include <memory>
#include<iomanip>
#include <sstream>

//jinjia2名字空间开始
namespace jinja2
{

///////////////////////////////////////////////// any类型基本查询 /////////////////////////////////////////////////

bool is_empty(const any_type& any_value)
{
	return any_value.empty();
}

bool is_bool(const any_type& any_value)
{
	return any_value.type() == typeid(bool);
}

bool is_char(const any_type& any_value)
{
	return any_value.type() == typeid(char);
}

bool is_unsigned_char(const any_type& any_value)
{
	return any_value.type() == typeid(unsigned char);
}

bool is_short(const any_type& any_value)
{
	return any_value.type() == typeid(short int);
}

bool is_unsigned_short(const any_type& any_value)
{
	return any_value.type() == typeid(unsigned short int);
}

bool is_int(const any_type& any_value)
{
	return any_value.type() == typeid(int);
}

bool is_unsigned_int(const any_type& any_value)
{
	return any_value.type() == typeid(unsigned int);
}

bool is_float(const any_type& any_value)
{
	return any_value.type() == typeid(float);
}

bool is_double(const any_type& any_value)
{
	return any_value.type() == typeid(double);
}

bool is_char_ptr(const any_type& any_value)
{
	try
	{
		boost::any_cast<const char *>(any_value);
		return true;
	}
	catch (const boost::bad_any_cast &)
	{
		return false;
	}
}

bool is_string(const any_type& any_value)
{
	try
	{
		boost::any_cast<std::string>(&any_value);
		return true;
	}
	catch (const boost::bad_any_cast &)
	{
		return false;
	}
}

bool is_dict(const any_type& any_value)
{
	try
	{
		return boost::any_cast<dict_type>(&any_value) != 0;
	}
	catch (const boost::bad_any_cast &)
	{
		return false;
	}
}

bool is_many(const any_type& any_value)
{
	try
	{
		return boost::any_cast<many_type>(&any_value) != 0;
	}
	catch (const boost::bad_any_cast &)
	{
		return false;
	}
}

bool is_string_record(const any_type& any_value)
{
	try
	{
		return boost::any_cast<string_record_type>(&any_value) != 0;
	}
	catch (const boost::bad_any_cast &)
	{
		return false;
	}
}

bool is_record_list(const any_type& any_value)
{
	try
	{
		return boost::any_cast<record_list_type>(&any_value) != 0;
	}
	catch (const boost::bad_any_cast &)
	{
		return false;
	}
}

bool is_char_series(const any_type& any_value)
{
	return is_char(any_value) || is_unsigned_char(any_value);
}

bool is_int_series(const any_type& any_value)
{
	return is_int(any_value) || is_unsigned_int(any_value) || is_short(any_value) || is_unsigned_short(any_value);
}

bool is_float_series(const any_type& any_value)
{
	return is_float(any_value) || is_double(any_value);
}

bool is_string_series(const any_type& any_value)
{
	return is_char_ptr(any_value) || is_string(any_value);
}

bool is_container(const any_type& any_value)
{
	return is_many(any_value) || is_dict(any_value) || is_string_record(any_value) || is_record_list(any_value);
}

///////////////////////////////////////////////// any值查询 /////////////////////////////////////////////////

bool get_bool(const any_type& any_value)
{
	return boost::any_cast<bool>(any_value);
}

char get_char(const any_type& any_value)
{
	if (is_char(any_value))
		return boost::any_cast<char>(any_value);
	else
		return boost::any_cast<unsigned char>(any_value);
}

int get_int(const any_type& any_value)
{
	if (is_int(any_value))
		return boost::any_cast<int>(any_value);
	else if (is_unsigned_int(any_value))
		return static_cast<int>(boost::any_cast<unsigned int>(any_value));
	else if (is_short(any_value))
		return static_cast<int>(boost::any_cast<short>(any_value));
	else
		return static_cast<int>(boost::any_cast<unsigned short>(any_value));
}

double get_float(const any_type& any_value)
{
	if (is_double(any_value))
		return boost::any_cast<double>(any_value);
	else
		return boost::any_cast<float>(any_value);
}

std::string get_string(const any_type& any_value)
{
	if (is_char_ptr(any_value))
		return boost::any_cast<const char*>(any_value);
	else
		return boost::any_cast<std::string>(any_value);
}

const many_type* get_many(const any_type& any_value)
{
	return boost::any_cast<many_type>(&any_value);
}

const dict_type* get_dict(const any_type& any_value)
{
	return boost::any_cast<dict_type>(&any_value);
}

const string_record_type* get_string_record(const any_type& any_value)
{
	return boost::any_cast<string_record_type>(&any_value);
}

const record_list_type* get_record_list(const any_type& any_value)
{
	return boost::any_cast<record_list_type>(&any_value);
}

bool visit_descendant(const any_type& any_value, std::function<bool(const any_type&)> visit_cb, bool bIncludeSelf, bool bTopDown)
{
	//前访问自身
	if (bIncludeSelf && bTopDown)
	{
		if (visit_cb(any_value))
			return true;
	}

	//访问子对象
	auto _visit = [&visit_cb, bTopDown](const any_type& any_value, unsigned, unsigned int)
	{
		if (visit_descendant(any_value, visit_cb, true, bTopDown))
			return true;
		return false;
	};
	if (visit_container(any_value, _visit))
		return true;

	//后访问自身
	if (bIncludeSelf && (!bTopDown))
	{
		if (visit_cb(any_value))
			return true;
	}
	return false;
}

bool visit_container(const any_type& any_value, std::function<bool(const any_type&,unsigned int, unsigned int)> visit_cb)
{
	if (is_many(any_value))
	{
		auto aContainer = *(get_many(any_value));
		for (unsigned int nIndex = 0, cbCount = aContainer.size(); nIndex < cbCount; ++nIndex)
		{
			if (visit_cb(aContainer[nIndex], nIndex, cbCount))
				return true;
		}
	}
	else if (is_dict(any_value))
	{
		auto aContainer = *(get_dict(any_value));
		unsigned int cbCount = aContainer.size(),nIndex = 0;
		for (auto p = aContainer.begin(), pEnd = aContainer.end(); p != pEnd; ++p,++nIndex)
		{
			if (visit_cb(p->first,nIndex, cbCount))
				return true;
		}
	}
	else if (is_string_record(any_value))
	{
		auto aContainer = *(get_string_record(any_value));
		unsigned int cbCount = aContainer.size(), nIndex = 0;
		for (auto p = aContainer.begin(), pEnd = aContainer.end(); p != pEnd; ++p, ++nIndex)
		{
			if (visit_cb(*p, nIndex, cbCount))
				return true;
		}
	}
	else if (is_record_list(any_value))
	{
		auto aContainer = *(get_record_list(any_value));
		unsigned int cbCount = aContainer.size(), nIndex = 0;
		for (auto p = aContainer.begin(), pEnd = aContainer.end(); p != pEnd; ++p, ++nIndex)
		{
			if (visit_cb(*p, nIndex, cbCount))
				return true;
		}
	}
	return false;
}

///////////////////////////////////////////////// any值转换 /////////////////////////////////////////////////

std::string to_string(const any_type& any_value)
{
	if (is_empty(any_value))
		return std::string();
	if (is_bool(any_value))
		return boost::any_cast<bool>(any_value) ? "True" : "False";
	std::stringstream ss;
	if (is_char(any_value))
	{
		ss << boost::any_cast<char>(any_value);
		return ss.str();
	}
	if (is_unsigned_char(any_value))
	{
		ss << boost::any_cast<unsigned char>(any_value);
		return ss.str();
	}
	if (is_short(any_value))
	{
		ss << boost::any_cast<short>(any_value);
		return ss.str();
	}
	if (is_unsigned_short(any_value))
	{
		ss << boost::any_cast<unsigned short>(any_value);
		return ss.str();
	}
	if (is_int(any_value))
	{
		ss << boost::any_cast<int>(any_value);
		return ss.str();
	}
	if (is_unsigned_int(any_value))
	{
		ss << boost::any_cast<unsigned int>(any_value);
		return ss.str();
	}
	if (is_float(any_value))
	{
		ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << boost::any_cast<float>(any_value);
		return ss.str();
	}
	if (is_double(any_value))
	{
		ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << boost::any_cast<double>(any_value);
		return ss.str();
	}
	if (is_char_ptr(any_value))
	{
		std::string value = boost::any_cast<const char *>(any_value);
		return value;
	}
	if (is_string(any_value))
	{
		std::string value = boost::any_cast<std::string>(any_value);
		return value;
	}
	if (is_many(any_value))
	{
		const many_type& values = boost::any_cast<many_type>(any_value);
		ss << '[';
		for (many_type::const_iterator pBegin = values.begin(), pEnd = values.end(), p = pBegin; p != pEnd; ++p)
		{
			if (p != pBegin)
				ss << ',';
			ss << to_string(*p);
		}
		ss << ']';
		return ss.str();
	}
	if (is_dict(any_value))
	{
		const dict_type* pValues = boost::any_cast<dict_type>(&any_value);
		ss << '{';
		for (dict_type::const_iterator pBegin = pValues->begin(), pEnd = pValues->end(), p = pBegin; p != pEnd; ++p)
		{
			if (p != pBegin)
				ss << ", ";
			ss << '\"' << p->first << "\": " << to_string(*p);
		}
		ss << '}';
		return ss.str();
	}
	if (is_string_record(any_value))
	{
		ss << '[';
		const string_record_type* pValues = boost::any_cast<string_record_type>(&any_value);
		for (string_record_type::const_iterator pBegin = pValues->begin(), pEnd = pValues->end(), p = pBegin; p != pEnd; ++p)
		{
			if (p != pBegin)
				ss << ", ";
			ss << to_string(*p);
		}
		ss << ']';
		return ss.str();
	}
	if (is_record_list(any_value))
	{
		ss << '[';
		const record_list_type* pRecordList = boost::any_cast<record_list_type>(&any_value);
		for (record_list_type::const_iterator pRowBegin = pRecordList->begin(), pRowEnd = pRecordList->end(), pRow = pRowBegin; pRow != pRowEnd; ++pRow)
		{
			if (pRow != pRowBegin)
				ss << ", ";
			ss << '[';
			for (auto pBegin = pRow->begin(), pEnd = pRow->end(),p = pBegin; p != pEnd; ++p)
			{
				if (p != pBegin)
					ss << ", ";
				ss << (*p);
			}
			ss << ']';
		}
		ss << ']';
		return ss.str();
	}
	return std::string();
}

bool to_bool(const any_type& any_value)
{
	if (is_empty(any_value))
		return false;
	if (is_bool(any_value))
		return boost::any_cast<bool>(any_value);
	if (is_char(any_value))
	{
		char ch = boost::any_cast<char>(any_value);
		return ch != 0;
	}
	if (is_unsigned_char(any_value))
	{
		unsigned char ch = boost::any_cast<unsigned char>(any_value);
		return ch != 0;
	}
	if (is_short(any_value))
	{
		short value = boost::any_cast<short>(any_value);
		return value != 0;
	}
	if (is_unsigned_short(any_value))
	{
		unsigned short value = boost::any_cast<unsigned short>(any_value);
		return value != 0;
	}
	if (is_int(any_value))
	{
		int value = boost::any_cast<int>(any_value);
		return value != 0;
	}
	if (is_unsigned_int(any_value))
	{
		int value = boost::any_cast<unsigned int>(any_value);
		return value != 0;
	}
	if (is_float(any_value))
	{
		float value = boost::any_cast<float>(any_value);
		return fabs(value) > 0.000001;
	}
	if (is_double(any_value))
	{
		double value = boost::any_cast<double>(any_value);
		return fabs(value) > 0.000001;
	}
	if (is_char_ptr(any_value))
	{
		std::string value = boost::any_cast<const char *>(any_value);
		return !value.empty();
	}
	if (is_string(any_value))
	{
		std::string value = boost::any_cast<std::string>(any_value);
		return !value.empty();
	}
	if (is_many(any_value))
	{
		many_type& value = boost::any_cast<many_type>(any_value);
		return !value.empty();
	}
	if (is_dict(any_value))
	{
		dict_type& value = boost::any_cast<dict_type>(any_value);
		return !value.empty();
	}
	if (is_string_record(any_value))
	{
		return !get_string_record(any_value)->empty();
	}
	if (is_record_list(any_value))
	{
		return !get_record_list(any_value)->empty();
	}
	return false;
}

///////////////////////////////////////////////// 辅助工具类 /////////////////////////////////////////////////

//**********************************************************************
// 类名: CStringStream 
// 目的: 字符串流, 用于错误输出,如: STRING_STREAM(szResultError) << "指定变量" << szVariable << "不存在!";
//*********************************************************************
class CStringStream
{
////类型声明
public:
	typedef CStringStream											my_type;

////构造、析构函数
public:
	//构造函数
	CStringStream(std::string& szResultString) : m_szResultString(szResultString){}
	//析构函数
	~CStringStream(void) { m_szResultString = m_aStream.str(); }

////基本查询
public:
	//得到输出流
	std::ostream& stream(void) { return m_aStream; }

////数据成员
private:
	std::string&			m_szResultString;	//结果字符串
	std::ostringstream		m_aStream;			//输出流
};

//字符串流宏定义
#define STRING_STREAM(szResultString)									CStringStream(szResultString).stream()

///////////////////////////////////////////////// 字符串辅助函数 /////////////////////////////////////////////////

//**********************************************************************
// 函数: TrimLeft
// 功能: 剔除左侧空格
//*********************************************************************
void TrimLeft(std::string& szResultContent)
{
	while (szResultContent.size())
	{
		const char& ch = *(szResultContent.begin());
		if ((ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n'))
			szResultContent.erase(szResultContent.begin());
		else
			break;
	}
}

//**********************************************************************
// 函数: TrimRight
// 功能: 剔除右侧空格
//*********************************************************************
void TrimRight(std::string& szResultContent)
{
	while (szResultContent.size())
	{
		const char& ch = *(szResultContent.rbegin());
		if ((ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n'))
			szResultContent.resize(szResultContent.size() - 1);
		else
			break;
	}
}

//**********************************************************************
// 函数: Trim
// 功能: 剔除空格
//*********************************************************************
void Trim(std::string& szResultContent)
{
	TrimLeft(szResultContent);
	TrimRight(szResultContent);
}

///////////////////////////////////////////////// 脚本变量类型查询函数 /////////////////////////////////////////////////

//**********************************************************************
// 函数: is_variable_char
// 功能: 是否为变量字符(即由字符、数字、下划线构成)
//*********************************************************************
bool is_variable_char(const char ch)
{
	return isalpha(ch) || isdigit(ch) || (ch == '_');
}

//**********************************************************************
// 函数: is_variable
// 功能: 是否为变量(即由字符、数字、下划线构成)
//*********************************************************************
bool is_variable(const std::string& szVariable)
{
	for (auto& ch : szVariable)
	{
		if (!is_variable_char(ch))
			return false;
	}
	return true;
}

//**********************************************************************
// 函数: is_digit_variable
// 功能: 是否为数字变量
//*********************************************************************
bool is_digit_variable(const std::string& szVariable)
{
	for (auto& ch : szVariable)
	{
		if (!isdigit(ch))
			return false;
	}
	return true;
}

//**********************************************************************
// 函数: is_text_variable
// 功能: 是否为文本变量(即首、尾为引号)
//*********************************************************************
bool is_text_variable(const std::string& szVariable)
{
	//检查尺寸
	if (szVariable.size() < 2)
		return false;

	//检查首、尾字符
	const char& chFirst = szVariable[0];
	const char& chLast = szVariable[szVariable.size() - 1];
	return ((chFirst == '\'') || (chFirst == '\"')) && (chFirst == chLast);
}

//**********************************************************************
// 函数: is_true_variable
// 功能: 是否为bool真常量
//*********************************************************************
bool is_true_variable(const std::string& szVariable)
{
	return _stricmp(szVariable.c_str(), "true") == 0;
}

//**********************************************************************
// 函数: is_false_variable
// 功能: 是否为bool假常量
//*********************************************************************
bool is_false_variable(const std::string& szVariable)
{
	return _stricmp(szVariable.c_str(), "false") == 0;
}

///////////////////////////////////////////////// 脚本变量解析辅助函数 /////////////////////////////////////////////////

//**********************************************************************
// 函数: variable_consume_to_next_operator
// 功能: 定位到下一个操作符(下一个非变量字符,nPos指向该位置)
//*********************************************************************
bool variable_consume_to_next_operator(const std::string& szVariable, unsigned int& nPos)
{
	for (unsigned int& nIndex = nPos, cbCount = szVariable.size(); nIndex < cbCount; ++nIndex)
	{
		const char& ch = szVariable[nIndex];
		if (!is_variable_char(ch))
			return true;
	}
	return false;
}

//**********************************************************************
// 函数: variable_consume_to_next_close_operator
// 功能: 定位到下一个结束操作符,如"(" -> ")","[" => "]"等,支持嵌套, nPos指向结束操作符位置
//*********************************************************************
bool variable_consume_to_next_close_operator(const std::string& szVariable, unsigned int& nPos,const char chLeft,const char chRight)
{
	for (unsigned int& nIndex = nPos, cbCount = szVariable.size(), cbLeft = 0, cbRight = 0; nIndex < cbCount; ++nIndex)
	{
		const char& ch = szVariable[nIndex];
		bool bFound = false;
		if (ch == chLeft)
		{
			++cbLeft;
			bFound = true;
		}
		else if (ch == chRight)
		{
			bFound = true;
			++cbRight;
		}
		if (bFound && (cbLeft == cbRight))
			return true;
	}
	return false;
}

///////////////////////////////////////////////// 脚本变量求值函数 /////////////////////////////////////////////////

//前向声明
bool EvalObjectVariable(const any_type& aObject, const std::string& szAction, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError);
bool EvalVariable(const std::string& szVariable, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError);

//**********************************************************************
// 函数: EvalSimpleVariable
// 功能: 变量求简单值,如: {{ value }}
//*********************************************************************
bool EvalSimpleVariable(const std::string& szVariable, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError, bool bStrict)
{
	//检查变量是否存在
	dict_type::const_iterator p = aRenderContent.find(szVariable);
	if (p == aRenderContent.end())
	{
		aResultValue = any_type();
		STRING_STREAM(szResultError) << "指定变量(\"" << szVariable << "\")不存在!";
		return bStrict ? false : true;
	}
	aResultValue = p->second;
	return true;
}

//**********************************************************************
// 函数: EvalObjectIndexCall
// 功能: 对象变量索引调用,返回参数在列表中的索引(0基)
//*********************************************************************
bool EvalObjectIndexCall(const any_type& aObject, const std::string& szMethod, const std::string& szParameters, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//检查是否为向量
	if ((!is_many(aObject)) && (!is_string_record(aObject)))
	{
		STRING_STREAM(szResultError) << "对象索引求值失败！指定对象不是集合:" << aObject.type().name();
		return false;
	}

	//处理文本参数
	std::string szTemp = szParameters;
	if (is_text_variable(szParameters))
		szTemp = szParameters.substr(1, szParameters.size() - 2);

	//遍历容器
	auto visit_cb = [&szParameters, &aResultValue, &szTemp](const any_type& any_value, unsigned int nIndex, unsigned int cbCount)
	{
		if (is_int_series(any_value))		//整数列表索引比较
		{
			if (get_int(any_value) == atoi(szParameters.c_str()))
			{
				aResultValue = nIndex;
				return true;
			}
		}
		else if (is_string_series(any_value))  //字符串列表索引比较
		{
			if (get_string(any_value) == szTemp)
			{
				aResultValue = nIndex;
				return true;
			}
		}
		return false;
	};
	if (!visit_container(aObject, visit_cb))
	{
		STRING_STREAM(szResultError) << "对象索引求值失败！找不到指定对象:" << szParameters;
		return false;
	}
	return true;
}

//**********************************************************************
// 函数: EvalObjectLengthCall
// 功能: 对象变量尺寸调用
//*********************************************************************
bool EvalObjectLengthCall(const any_type& aObject, const std::string& szMethod, const std::string& szParameters, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//检查对象是否为容器
	if (!is_container(aObject))
	{
		STRING_STREAM(szResultError) << "对象长度调用失败！指定对象不是容器对象:" << aObject.type().name();
		return false;
	}

	//得到子对象数量
	unsigned int cbCount = 0;
	visit_container(aObject, [&cbCount](const any_type& aObject,unsigned int,unsigned int){ ++cbCount; return false; });
	aResultValue = cbCount;
	return true;
}

//**********************************************************************
// 函数: EvalObjectMethodCall
// 功能: 对象变量方法调用
//*********************************************************************
bool EvalObjectMethodCall(const any_type& aObject, const std::string& szMethod, const std::string& szParameters, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	if (szMethod == "index")
		return EvalObjectIndexCall(aObject, szMethod, szParameters, aRenderContent, aResultValue, szResultError);
	else if ((szMethod == "length") || (szMethod == "size"))
		return EvalObjectLengthCall(aObject, szMethod, szParameters, aRenderContent, aResultValue, szResultError);
	STRING_STREAM(szResultError) << "不支持的对象方法(\"" << szMethod << "\")!";
	return false;
}

//**********************************************************************
// 函数: EvalObjectMethodVariable
// 功能: 对象变量方法求值
//*********************************************************************
bool EvalObjectMethodVariable(const any_type& aObject, const std::string& szAction, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//检索方法名称
	unsigned int nOperatorPos = 1, nBeginPos = nOperatorPos;
	bool bSuccess = variable_consume_to_next_operator(szAction, nOperatorPos);
	if (!bSuccess)
	{
		STRING_STREAM(szResultError) << "对象变量(\"" << szAction << "\")方法求值失败,定位操作符失败!";
		return false;
	}
	std::string szMethod = szAction.substr(nBeginPos, nOperatorPos - nBeginPos);

	//定位到方法结束位置,提取参数和剩余操作
	nBeginPos = nOperatorPos + 1;
	if (!variable_consume_to_next_close_operator(szAction, nOperatorPos, '(', ')'))
	{
		STRING_STREAM(szResultError) << "对象操作(\"" << szAction << "\")定位失败!";
		return false;
	}
	std::string szParameters = szAction.substr(nBeginPos, nOperatorPos - nBeginPos);
	Trim(szParameters);
	std::string szRemain = szAction.substr(nOperatorPos + 1);
	TrimRight(szRemain);

	//执行单次方法调用
	any_type aReturnValue;
	if (!EvalObjectMethodCall(aObject, szMethod, szParameters, aRenderContent, aReturnValue, szResultError))
		return false;

	//递归调用或返回
	if (szRemain.empty())
	{
		aResultValue = aReturnValue;
		return true;
	}
	else
	{
		any_type aNextValue;
		if (!EvalObjectVariable(aReturnValue, szRemain, aRenderContent, aNextValue, szResultError))
			return false;
		aResultValue = aNextValue;
		return true;
	}
}

//**********************************************************************
// 函数: EvalObjectAttributeCall
// 功能: 对象变量属性求值
//*********************************************************************
bool EvalObjectAttributeCall(const any_type& aObject, const std::string& szAttribute, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//内置属性支持
	if ((szAttribute == "length") || (szAttribute == "size"))
		return EvalObjectLengthCall(aObject, szAttribute, "", aRenderContent, aResultValue, szResultError);

	//检查对象是否是字典
	if (!is_dict(aObject))
	{
		STRING_STREAM(szResultError) << "属性(\"" << szAttribute << "\")提取失败！对象不是字典对象: " << aObject.type().name();
		return false;
	}

	//查找属性
	const dict_type* pDict = boost::any_cast<dict_type>(&aObject);
	dict_type::const_iterator p = pDict->find(szAttribute);
	if (p == pDict->end())
	{
		STRING_STREAM(szResultError) << "对象属性(\"" << szAttribute << "\")不存在!";
		return false;
	}
	else
	{
		aResultValue = p->second;
		return true;
	}
}

//**********************************************************************
// 函数: EvalObjectAttributeVariable
// 功能: 对象变量属性求值
//*********************************************************************
bool EvalObjectAttributeVariable(const any_type& aObject, const std::string& szAction, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//搜索属性结束位置
	unsigned int nOperatorPos = 1, nBeginPos = nOperatorPos;
	bool bSuccess = variable_consume_to_next_operator(szAction, nOperatorPos);
	std::string szAttribute = szAction.substr(nBeginPos, nOperatorPos - nBeginPos);
	Trim(szAttribute);
	std::string szRemain;
	if (bSuccess)
		szRemain = szAction.substr(nOperatorPos);
	TrimRight(szRemain);

	//计算本参数
	any_type aTempValue;
	if (!EvalObjectAttributeCall(aObject, szAttribute, aRenderContent, aTempValue, szResultError))
		return false;

	//返回
	if (szRemain.empty())
	{
		aResultValue = aTempValue;
		return true;
	}
	else
	{
		any_type aNextValue;
		if (!EvalObjectVariable(aTempValue, szRemain, aRenderContent, aNextValue, szResultError))
			return false;
		aResultValue = aNextValue;
		return true;
	}
}

//**********************************************************************
// 函数: EvalObjectSubscriptCall
// 功能: 对象变量下标求值
//*********************************************************************
bool EvalObjectSubscriptCall(const any_type& aObject, const std::string& szParameter, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//确定索引数据类型
	int nSubscriptIndex = 0;
	std::string szSubscriptIndex;
	bool bDigitValue = false;
	if (is_digit_variable(szParameter))	//下标为数字
	{
		bDigitValue = true;
		nSubscriptIndex = atoi(szParameter.c_str());
	}
	else if (is_text_variable(szParameter)) //下标为文本
	{
		szSubscriptIndex = szParameter.substr(1, szParameter.size() - 2);
		bDigitValue = false;
	}
	else	//下标需要求值计算, 如: aObject[field_names.index('name')
	{	
		//进行下标求值
		any_type aValue;		
		if (!EvalVariable(szParameter, aRenderContent, aValue, szResultError))
			return false;

		//检查下标求值结果的数据类型判断索引类型
		if (is_int_series(aValue))
		{
			nSubscriptIndex = get_int(aValue);
			bDigitValue = true;
		}
		else if (is_string_series(aValue))
		{
			std::string szTemp = get_string(aValue);
			if (is_digit_variable(szTemp))
			{
				bDigitValue = true;
				nSubscriptIndex = atoi(szTemp.c_str());
			}
			else
			{
				szSubscriptIndex = szTemp;
				bDigitValue = false;
			}
		}	
	}

	//处理数字下标
	if (bDigitValue)
	{
		auto visit_cb = [nSubscriptIndex, &aResultValue,&szResultError](const any_type& any_value, unsigned int nIndex, unsigned int cbCount)
		{
			//检查索引大小
			if (nSubscriptIndex >= static_cast<int>(cbCount))
				STRING_STREAM(szResultError) << "索引越界(" << nSubscriptIndex << '/' << cbCount << ')!';

			//设置值
			if (nIndex == nSubscriptIndex)
				aResultValue = any_value;
			return (nIndex == nSubscriptIndex);
		};
		return visit_container(aObject, visit_cb);	
	}
	else  //处理文字下标
	{
		//检查对象类型
		if (!is_dict(aObject))
		{
			STRING_STREAM(szResultError) << "对象不是字典,不支持文字索引: " << aObject.type().name();
			return false;
		}

		//检索对象
		const dict_type* pDict = boost::any_cast<dict_type>(&aObject);
		dict_type::const_iterator p = pDict->find(szSubscriptIndex);
		if (p == pDict->end())
		{
			STRING_STREAM(szResultError) << "对象属性不存在(" << szSubscriptIndex << ").";
			return false;
		}
		else
		{
			aResultValue = p->second;
			return true;
		}
	}
}

//**********************************************************************
// 函数: EvalObjectSubscriptVariable
// 功能: 对象变量下标求值
//*********************************************************************
bool EvalObjectSubscriptVariable(const any_type& aObject, const std::string& szAction, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//定位到小标结束位置,提取下标参数和剩余方法
	unsigned int nOperatorPos = 0;
	unsigned int nBeginPos = nOperatorPos + 1;
	if (!variable_consume_to_next_close_operator(szAction, nOperatorPos, '[', ']'))
	{
		STRING_STREAM(szResultError) << "对象下标求值解析失败:" << szAction;
		return false;
	}
	std::string szParameters = szAction.substr(nBeginPos, nOperatorPos - nBeginPos);
	Trim(szParameters);
	std::string szRemain = szAction.substr(nOperatorPos + 1);
	TrimRight(szRemain);

	//计算本参数
	any_type aTempValue;
	if (!EvalObjectSubscriptCall(aObject, szParameters, aRenderContent, aTempValue, szResultError))
		return false;
	if (szRemain.empty())
	{
		aResultValue = aTempValue;
		return true;
	}
	else
	{
		any_type aNextValue;
		if (!EvalObjectVariable(aTempValue, szRemain, aRenderContent, aNextValue, szResultError))
			return false;
		aResultValue = aNextValue;
		return true;
	}
}

//**********************************************************************
// 函数: EvalObjectVariable
// 功能: 对象变量求值
//*********************************************************************
bool EvalObjectVariable(const any_type& aObject, const std::string& szAction, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//检查参数
	if (szAction.empty())
	{
		STRING_STREAM(szResultError) << "对象变量(\"" << szAction << "\")求值失败,未指定对象操作!";
		return false;
	}

	//判断操作
	const char& ch = szAction[0];
	if (ch == '.')
	{
		unsigned int nOperatorPos = 1;
		bool bSuccess = variable_consume_to_next_operator(szAction, nOperatorPos);
		if (bSuccess && (szAction[nOperatorPos] == '('))
			return EvalObjectMethodVariable(aObject, szAction, aRenderContent, aResultValue, szResultError);
		else
			return EvalObjectAttributeVariable(aObject, szAction, aRenderContent, aResultValue, szResultError);
	}
	else if (ch == '[')
	{
		return EvalObjectSubscriptVariable(aObject, szAction, aRenderContent, aResultValue, szResultError);
	}

	//返回
	STRING_STREAM(szResultError) << "未知对象操作(\"" << szAction << "\")!";
	return false;
}

//**********************************************************************
// 函数: EvalVariable
// 功能: 变量求值
//*********************************************************************
bool EvalVariable(const std::string& szVariable, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//处理bool常量
	if (is_true_variable(szVariable))
	{
		aResultValue = true;
		return true;
	}
	if (is_false_variable(szVariable))
	{
		aResultValue = false;
		return true;
	}

	//处理文本常量
	if (is_text_variable(szVariable))
	{
		aResultValue = szVariable.substr(1, szVariable.size() - 2);
		return true;
	}

	//处理简单变量
	if (is_variable(szVariable))
		return EvalSimpleVariable(szVariable, aRenderContent, aResultValue, szResultError, false);

	//对象变量求值,解析对象
	unsigned int nOperatorPos = 0;
	bool bSuccess = variable_consume_to_next_operator(szVariable, nOperatorPos);
	if (!bSuccess)
	{
		STRING_STREAM(szResultError) << "变量(\"" << szVariable << "\")定位操作符错误!";
		return false;
	}

	//提取对象,检查对象是否存在
	std::string szObject = szVariable.substr(0, nOperatorPos);
	std::string szRemain = szVariable.substr(nOperatorPos);
	dict_type::const_iterator p = aRenderContent.find(szObject);
	if (p == aRenderContent.end())
	{
		STRING_STREAM(szResultError) << "变量(\"" << szVariable << "\")未定义!";
		return false;
	}
	return EvalObjectVariable(p->second, szRemain, aRenderContent, aResultValue, szResultError);
}

///////////////////////////////////////////////// 模板解析辅助函数 /////////////////////////////////////////////////

//**********************************************************************
// 函数: IsStartTag
// 功能: 是否为开始标签
//*********************************************************************
bool IsStartTag(const std::string& szTemplate, unsigned int& nParseStartPos)
{
	//检查尺寸
	if ((nParseStartPos + 1) >= szTemplate.size())
		return false;

	//检查第一个字符
	if (szTemplate[nParseStartPos] != '{')
		return false;

	//检查第二个字符
	const char& ch = szTemplate[nParseStartPos + 1];
	switch (ch)
	{
	case '{':
	case '#':
	case '%':
		return true;
	default:
		return false;
	}
}

//**********************************************************************
// 函数: ConsumeToNextStartTag
// 功能: 解析到下一个标签
//*********************************************************************
bool ConsumeToNextStartTag(const std::string& szTemplate, unsigned int& nParseStartPos)
{
	for (unsigned int& nPos = nParseStartPos, cbCount = szTemplate.size(); nPos < cbCount; ++nPos)
	{
		if (IsStartTag(szTemplate, nPos))
			return true;
	}
	return false;
}

//**********************************************************************
// 函数: ConsumeStartTag
// 功能: 解析到下一个标签
//*********************************************************************
bool ConsumeStartTag(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultTag, bool& bResultCompact, std::string& szResultError)
{
	//检查参数
	if (!IsStartTag(szTemplate, nParseStartPos))
	{
		STRING_STREAM(szResultError) << "指定位置(" << nParseStartPos << ")不是控制标签!";
		return false;
	}

	//提取开始标签
	szResultTag = szTemplate.substr(nParseStartPos, 2);
	nParseStartPos += 2;
	bResultCompact = (nParseStartPos < szTemplate.size()) && (szTemplate[nParseStartPos] == '-');
	if (bResultCompact)
		++nParseStartPos;
	return true;
}

//**********************************************************************
// 函数: ConsumeNextWord
// 功能: 解析下一个单词
//*********************************************************************
bool ConsumeNextWord(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultWord, std::string& szResultError)
{
	unsigned int cbCount = szTemplate.size();
	enum{ prpFindWord, prpWord };
	unsigned int nState = prpFindWord;
	unsigned int nBeginPos = nParseStartPos, nEndPos = nBeginPos;
	for (unsigned int& nPos = nParseStartPos; nPos < cbCount; ++nPos)
	{
		const char& ch = szTemplate[nPos];
		switch (nState)
		{
		case(prpFindWord) :
			switch (ch)
		{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				nBeginPos = nPos + 1;
				break;
			default:
				nState = prpWord;
				nEndPos = nBeginPos;
				break;
		}
						  break;
		case prpWord:
			switch (ch)
			{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
			case '#':
			case '%':
			case '}':
			case '=':
				if (nEndPos >= nBeginPos)
				{
					szResultWord = szTemplate.substr(nBeginPos, nEndPos + 1 - nBeginPos);
					return true;
				}
				break;
			default:
				++nEndPos;
				break;
			}
			break;
		}
	}

	//返回
	if (nEndPos >= nBeginPos)
	{
		szResultWord = szTemplate.substr(nBeginPos, nEndPos + 1 - nBeginPos);
		return true;
	}
	else
	{
		STRING_STREAM(szResultError) << "解析错误:" << nBeginPos;
		return false;
	}
}

//**********************************************************************
// 函数: ConsumeNextEndTag
// 功能: 解析下一个结束标签
//*********************************************************************
bool ConsumeNextEndTag(const std::string& szTemplate, unsigned int& nParseStartPos, const std::string& szEndTag, bool& bResultCompact, std::string& szResultError)
{
	bResultCompact = false;
	unsigned int cbCount = szTemplate.size();
	for (unsigned int& nPos = nParseStartPos; nPos < cbCount; ++nPos)
	{
		//剔除空格
		const char& ch = szTemplate[nPos];
		if ((ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n'))
			continue;

		//处理精简
		if ((!bResultCompact) && (ch == '-'))
		{
			bResultCompact = true;
			continue;
		}

		//检查是否等于
		std::string szPrefix = szTemplate.substr(nPos, szEndTag.size());
		if (szPrefix != szEndTag)
		{
			STRING_STREAM(szResultError) << "期望\"" << szEndTag << "\",实际\"" << szPrefix << "\":" << nParseStartPos;
			return false;
		}
		else
		{
			nPos += szEndTag.size();
			return true;
		}
	}

	//返回
	return false;
}

///////////////////////////////////////////////// 节点与解析器类声明 /////////////////////////////////////////////////

//节点赋值回调函数声明
typedef std::function<bool(const std::string&, const any_type&, std::string&, std::string&)> value_cb_type;

//**********************************************************************
// 类名: INode 
// 目的: 节点 
//*********************************************************************
class INode : public std::enable_shared_from_this<INode>
{
////类型声明
public:
	typedef INode												node_type;
	typedef std::weak_ptr<node_type>							node_weak_pointer_type;
	typedef std::shared_ptr<node_type>							node_pointer_type;
	typedef std::vector<node_pointer_type>						node_container_type;

////构造、析构函数
public:
	//默认构造函数
	INode(void);
	//构造函数
	INode(node_weak_pointer_type spParent);
	//虚拟析构函数
	virtual ~INode(void);

////基本查询
public:
	//得到父节点(常量)
	node_weak_pointer_type GetParent(void) const { return m_spParent; }
	//得到子节点数量
	unsigned int GetChildrenCount(void) const { return m_spChildren.size(); }
	//得到子节点
	node_pointer_type GetChild(unsigned int nChildIndex) const { return m_spChildren[nChildIndex]; }
	//是否左侧紧凑
	bool IsLeftCompact(void) const { return m_bLeftCompact; }
	//是否右侧紧凑
	bool IsRightCompact(void) const { return m_bRightCompact; }

////INode命令
public:
	//添加子节点
	void AppendChild(node_pointer_type spChild);
	//插入子节点
	void InsertChild(unsigned int nChildIndex, node_pointer_type spChild);
	//设置左侧紧凑
	void SetLeftCompact(bool bCompact = true) { m_bLeftCompact = bCompact; }
	//设置右侧紧凑
	void SetRightCompact(bool bCompact = true) { m_bRightCompact = bCompact; }

////INode接口实现
public:
	//渲染结果
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////数据成员
private:
	node_weak_pointer_type			m_spParent;			//父节点(弱指针)
	node_container_type				m_spChildren;		//子节点(强指针)
	bool							m_bLeftCompact;		//是否左侧紧凑
	bool							m_bRightCompact;	//是否右侧紧凑
};

//**********************************************************************
// 类名: IParser 
// 目的: 解析状态机接口
//*********************************************************************
class IParser : public std::enable_shared_from_this<IParser>
{
////类型声明
public:
	typedef IParser											my_type;
	typedef IParser											parser_type;
	typedef std::shared_ptr<parser_type>					parser_pointer_type;
	typedef std::weak_ptr<parser_type>						parser_weak_pointer_type;
	typedef INode::node_pointer_type						node_pointer_type;
	typedef INode::node_weak_pointer_type					node_weak_pointer_type;

////构造、析构函数
public:
	//构造函数
	IParser(void) : m_bLeftCompact(false){}
	//构造函数
	IParser(parser_weak_pointer_type spParent) : m_spParent(spParent), m_bLeftCompact(false) {}
	//虚拟析构函数
	virtual ~IParser(void){}

////基本查询
public:
	//得到父对象
	parser_weak_pointer_type GetParent(void) const { return m_spParent; }
	//是否左紧凑
	bool IsLeftCompact(void) const { return m_bLeftCompact; }

////命令
public:
	//设置左紧凑
	void SetLeftCompact(bool bCompact = true) { m_bLeftCompact = bCompact; }

////IParser接口
public:
	//解析模板
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError) = 0;
	//得到节点
	virtual std::shared_ptr<INode> GetNode(void) const = 0;

////数据成员
private:
	parser_weak_pointer_type					m_spParent;			//父解析对象
	bool										m_bLeftCompact;		//是否左紧凑
};

//**********************************************************************
// 类名: CTemplate 
// 目的: 赋值解析器
//*********************************************************************
class CTemplate : public IParser
{
////类型声明
public:
	typedef CTemplate										my_type;
	typedef IParser											base_type;
	typedef INode::node_pointer_type						node_pointer_type;

////构造、析构函数
public:
	//默认构造函数
	CTemplate(void);
	//析构函数
	~CTemplate(void);

////命令
public:
	//渲染结果
	bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////IParser接口实现
public:
	//解析模板
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//得到节点
	virtual std::shared_ptr<INode> GetNode(void) const { return m_spRootNode; }

////数据成员
private:
	std::shared_ptr<INode>					m_spRootNode;
};

//**********************************************************************
// 类名: CTextNode 
// 目的: 文本节点 
//*********************************************************************
class CTextNode : public INode
{
////类型声明
public:
	typedef CTextNode										my_type;
	typedef INode											base_type;

////构造、析构函数
public:
	//默认构造函数
	CTextNode(void) : base_type() {}
	//构造函数
	CTextNode(node_weak_pointer_type spParent) : base_type(spParent){}
	//析构函数
	~CTextNode(void){}

////命令
public:
	//设置文本
	void SetText(const std::string& szText){ m_szText = szText; }

////INode接口实现
public:
	//渲染结果
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////数据成员
private:
	std::string					m_szText;
};

//**********************************************************************
// 类名: CCommentNode 
// 目的: 注释节点 
//*********************************************************************
class CCommentNode : public INode
{
////类型声明
public:
	typedef CTextNode										my_type;
	typedef INode											base_type;

////构造、析构函数
public:
	//默认构造函数
	CCommentNode(void) : base_type() {}
	//构造函数
	CCommentNode(node_weak_pointer_type spParent) : base_type(spParent){}
	//析构函数
	~CCommentNode(void){}

////命令
public:
	//设置文本
	void SetText(const std::string& szText){ m_szText = szText; }

////INode接口实现
public:
	//渲染结果
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////数据成员
private:
	std::string					m_szText;
};

//**********************************************************************
// 类名: CVariableNode 
// 目的: 值节点 
//*********************************************************************
class CVariableNode : public INode
{
////类型声明
public:
	typedef CVariableNode									my_type;
	typedef INode											base_type;

////构造、析构函数
public:
	//默认构造函数
	CVariableNode(void) : base_type() {}
	//构造函数
	CVariableNode(node_weak_pointer_type spParent) : base_type(spParent){}
	//析构函数
	~CVariableNode(void){}

////命令
public:
	//设置值
	void SetValue(const std::string& szValue) { m_szValue = szValue; }

////INode接口实现
public:
	//渲染结果
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////执行函数
private:
	//求值
	static any_type EvalValue(const std::string& szValue, const dict_type& aRenderContent);

////数据成员
private:
	std::string      m_szValue;
};

//**********************************************************************
// 类名: CIfNode 
// 目的: bool节点 
//*********************************************************************
class CIfNode : public INode
{
////类型声明
public:
	typedef CIfNode											my_type;
	typedef INode											base_type;

	////构造、析构函数
public:
	//默认构造函数
	CIfNode(void);
	//构造函数
	CIfNode(node_weak_pointer_type spParent);
	//析构函数
	~CIfNode(void);

////基本查询
public:
	//得到if内容节点
	node_pointer_type GetIfContentNode(void) { return m_spIfContentNode; }
	//得到if内容节点(常量)
	node_pointer_type GetIfContentNode(void) const { return m_spIfContentNode; }
	//得到else内容节点
	node_pointer_type GetElseContentNode(void) { return m_spElseContentNode; }
	//得到else内容节点(常量)
	node_pointer_type GetElseContentNode(void) const { return m_spElseContentNode; }
	//查询If右侧紧凑
	bool IsIfRightCompact(void) const { return m_bIfRightCompact; }
	//查询Else左侧紧凑
	bool IsElseLeftCompact(void) const { return m_bElseLeftCompact; }
	//查询Else右侧紧凑
	bool IsElseRightCompact(void) const { return m_bElseRightCompact; }
	//查询Endif左侧紧凑
	bool IsEndifLeftCompact(void) const { return m_bEndifLeftCompact; }
	//查询是否有else
	bool HasElse(void) const { return m_bHasElse; }

////命令
public:
	//设置值
	void SetValue(bool bNot, const std::string& szExpress);
	//设置If右侧紧凑
	void SetIfRightCompact(bool bCompact = true) { m_bIfRightCompact = bCompact; }
	//设置Else左侧紧凑
	void SetElseLeftCompact(bool bCompact = true) { m_bElseLeftCompact = bCompact; }
	//设置Else右侧紧凑
	void SetElseRightCompact(bool bCompact = true) { m_bElseRightCompact = bCompact; }
	//设置Endif左侧紧凑
	void SetEndifLeftCompact(bool bCompact = true) { m_bEndifLeftCompact = bCompact; }
	//设置是否有else
	void SetHasElse(bool bHas = true) { m_bHasElse = bHas; }

////INode接口实现
public:
	//渲染结果
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////数据成员
private:
	bool						m_bNot;					//是否值取反
	std::string					m_szExpress;			//表达式
	node_pointer_type			m_spIfContentNode;		//if内容节点
	node_pointer_type			m_spElseContentNode;	//else内容节点
	bool						m_bIfRightCompact;		//If右侧紧凑
	bool						m_bElseLeftCompact;		//Else左侧紧凑
	bool						m_bElseRightCompact;	//Else右侧紧凑
	bool						m_bEndifLeftCompact;	//Endif左侧紧凑
	bool						m_bHasElse;				//是否有else
};

//**********************************************************************
// 类名: CForNode 
// 目的: 循环节点 
//*********************************************************************
class CForNode : public INode
{
////类型声明
public:
	typedef CForNode											my_type;
	typedef INode											base_type;

////构造、析构函数
public:
	//默认构造函数
	CForNode(void);
	//构造函数
	CForNode(node_weak_pointer_type spParent);
	//析构函数
	~CForNode(void);

////基本查询
public:
	//查询For右侧紧凑
	bool IsForRightCompact(void) const { return m_bForRightCompact; }
	//查询Else左侧紧凑
	bool IsEndForLeftCompact(void) const { return m_bEndForLeftCompact; }

////命令
public:
	//设置值
	void SetValue(const std::string& szVariable, const std::string& szContainer);
	//设置For右侧紧凑
	void SetForRightCompact(bool bCompact = true) { m_bForRightCompact = bCompact; }
	//设置EndFor左侧紧凑
	void SetEndForLeftCompact(bool bCompact = true) { m_bEndForLeftCompact = bCompact; }

////INode接口实现
public:
	//渲染结果
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////实现函数
private:
	//渲染一次
	bool RenderOnce(const std::string& szVariableName, any_type aVariableValue, unsigned int nLoopIndex, unsigned int nLoopCount, const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);
	//渲染一次实现
	bool RenderOnceImpl(const std::string& szVariableName, any_type aVariableValue, unsigned int nLoopIndex, unsigned int nLoopCount, dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////数据成员
private:
	std::string					m_szVariable;			//循环变量名
	std::string					m_szContainer;			//容器变量名
	bool						m_bForRightCompact;		//For右侧紧凑
	bool						m_bEndForLeftCompact;	//EndFor左侧紧凑
};

//**********************************************************************
// 类名: CSetNode 
// 目的: 赋值节点 
//*********************************************************************
class CSetNode : public INode
{
////类型声明
public:
	typedef CSetNode										my_type;
	typedef INode											base_type;

////构造、析构函数
public:
	//默认构造函数
	CSetNode(void);
	//构造函数
	CSetNode(node_weak_pointer_type spParent);
	//析构函数
	~CSetNode(void);

////命令
public:
	//设置值
	void SetValue(const std::string& szLeftVariable, const std::string& szRightVariable);

////INode接口实现
public:
	//渲染结果
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////数据成员
private:
	std::string				m_szLeftVariable;
	std::string				m_szRightVariable;
};

//**********************************************************************
// 类名: CTextParser 
// 目的: 文本解析器
//*********************************************************************
class CTextParser : public IParser
{
////类型声明
public:
	typedef CTextParser										my_type;
	typedef IParser											base_type;

////构造、析构函数
public:
	//默认构造函数
	CTextParser(void);
	//构造函数
	CTextParser(parser_weak_pointer_type spParent);
	//析构函数
	virtual ~CTextParser(void);

////IParser接口实现
public:
	//解析模板
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//得到节点
	virtual std::shared_ptr<INode> GetNode(void) const { return m_spTextNode; }

////执行函数
private:
	//处理解析到的文本
	void HandleText(const std::string& szText);

////数据成员
private:
	std::shared_ptr<CTextNode>					m_spTextNode;
};

//**********************************************************************
// 类名: CCommentParser 
// 目的: 注释解析器
//*********************************************************************
class CCommentParser : public IParser
{
////类型声明
public:
	typedef CCommentParser									my_type;
	typedef IParser											base_type;

////构造、析构函数
public:
	//默认构造函数
	CCommentParser(void);
	//构造函数
	CCommentParser(parser_weak_pointer_type spParent);
	//析构函数
	~CCommentParser(void);

////IParser接口实现
public:
	//解析模板
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//得到节点
	virtual std::shared_ptr<INode> GetNode(void) const { return std::shared_ptr<INode>(); }

////执行函数
private:
	//处理左紧凑
	void HandleLeftCompact(void);
	//处理右紧凑
	void HandleRightCompact(void);
	//处理注释文本
	void HandleCommentText(const std::string& szCommentText);
	//确保节点存在
	void VerifyNodeExists(void);

////数据成员
private:
	std::shared_ptr<CCommentNode>					m_spCommentNode;
};

//**********************************************************************
// 类名: CVariableParser 
// 目的: 命名值解析器
//*********************************************************************
class CVariableParser : public IParser
{
////类型声明
public:
	typedef CVariableParser									my_type;
	typedef IParser											base_type;

////构造、析构函数
public:
	//默认构造函数
	CVariableParser(void);
	//构造函数
	CVariableParser(parser_weak_pointer_type spParent);
	//析构函数
	~CVariableParser(void);

////IParser接口实现
public:
	//解析模板
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//得到节点
	virtual std::shared_ptr<INode> GetNode(void) const { return m_spValueNode; }

////执行函数
private:
	//处理解析的变量
	void HandleValue(const std::string& szValue);

////数据成员
private:
	std::shared_ptr<CVariableNode>		m_spValueNode;
};

//**********************************************************************
// 类名: CControlParser 
// 目的: 控制节点解析器
//*********************************************************************
class CControlParser : public IParser
{
////类型声明
public:
	typedef CControlParser									my_type;
	typedef IParser											base_type;

////构造、析构函数
public:
	//默认构造函数
	CControlParser(void);
	//构造函数
	CControlParser(parser_weak_pointer_type spParent);
	//析构函数
	~CControlParser(void);

////IParser接口实现
public:
	//解析模板
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//得到节点
	virtual std::shared_ptr<INode> GetNode(void) const { return std::shared_ptr<INode>(); }
};

//**********************************************************************
// 类名: CSetParser 
// 目的: 赋值解析器
//*********************************************************************
class CSetParser : public IParser
{
////类型声明
public:
	typedef CSetParser										my_type;
	typedef IParser											base_type;

////构造、析构函数
public:
	//默认构造函数
	CSetParser(void);
	//构造函数
	CSetParser(parser_weak_pointer_type spParent);
	//析构函数
	~CSetParser(void);

////IParser接口实现
public:
	//解析模板
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//得到节点
	virtual std::shared_ptr<INode> GetNode(void) const { return std::shared_ptr<INode>(); }

////实现函数
private:
	//处理解析的变量设置
	void HandleSetValue(const std::string& szLeftValue, const std::string& szRightValue);

////数据成员
private:
	std::shared_ptr<CSetNode>		m_spSetNode;
};

//**********************************************************************
// 类名: CSubBlockParser 
// 目的: 块解析器(辅助if或者for解析)
//*********************************************************************
class CBlockParser : public IParser
{
////类型声明
public:
	typedef CBlockParser									my_type;
	typedef IParser											base_type;

////构造、析构函数
public:
	//默认构造函数
	CBlockParser(void);
	//构造函数
	CBlockParser(parser_weak_pointer_type spParent);
	//析构函数
	~CBlockParser(void);

////命令
public:
	//绑定节点
	void SetNode(node_pointer_type spNode);

////IParser接口实现
public:
	//解析模板
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//得到节点
	virtual std::shared_ptr<INode> GetNode(void) const { return m_spNode; }

////数据成员
private:
	node_pointer_type				m_spNode;
};

//**********************************************************************
// 类名: CIfParser 
// 目的: bool解析器
//*********************************************************************
class CIfParser : public IParser
{
////类型声明
public:
	typedef CIfParser										my_type;
	typedef IParser											base_type;
	enum{ prpParseIf, prpParseIfContent, prpParseElse, prpParseElseContent, prpEndIf };

////构造、析构函数
public:
	//默认构造函数
	CIfParser(void);
	//构造函数
	CIfParser(parser_weak_pointer_type spParent);
	//析构函数
	~CIfParser(void);

////IParser接口实现
public:
	//解析模板
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//得到节点
	virtual std::shared_ptr<INode> GetNode(void) const { return std::shared_ptr<INode>(); }

////执行函数
private:
	//处理表达式
	void HandleBool(bool bLeftCompact, bool bNot, const std::string& szBoolExpress, bool bRightCompact);
	//解析if
	bool ConsumeIf(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//解析if内容
	bool ConsumeIfContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//解析else
	bool ConsumeElse(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//解析else内容
	bool ConsumeElseContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//解析if结束
	bool ConsumeEndIf(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);

////数据成员
private:
	unsigned int					m_nParseState;		//解析状态
	std::shared_ptr<CIfNode>		m_spNode;			//if节点
};

//**********************************************************************
// 类名: CForParser 
// 目的: 循环解析器
//*********************************************************************
class CForParser : public IParser
{
////类型声明
public:
	typedef CForParser										my_type;
	typedef IParser											base_type;
	enum{ prpParseFor, prpParseForContent, prpEndFor };

////构造、析构函数
public:
	//默认构造函数
	CForParser(void);
	//构造函数
	CForParser(parser_weak_pointer_type spParent);
	//析构函数
	~CForParser(void);

////IParser接口实现
public:
	//解析模板
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//得到节点
	virtual std::shared_ptr<INode> GetNode(void) const { return m_spNode; }

////执行函数
private:
	//解析for
	bool ConsumeFor(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//解析for内容
	bool ConsumeForContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//解析for结束
	bool ConsumeEndFor(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//处理for元素
	void HandleForValues(const std::string& szVariable, const std::string& szContainer);

////数据成员
private:
	unsigned int				m_nParseState;		//解析状态
	std::shared_ptr<CForNode>	m_spNode;			//for节点
};

///////////////////////////////////////////////// 节点与解析器辅助函数 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CreateParser
// 功能: 解析到下一个标签
//*********************************************************************
bool CreateParser(const std::string& szTag, std::weak_ptr<IParser> spParent, bool bCompact, std::shared_ptr<IParser>& spResultParser, std::string& szResultError)
{
	//生成解析器
	if (szTag == "{{")
		spResultParser = std::make_shared<CVariableParser>(spParent);
	else if (szTag == "{#")
		spResultParser = std::make_shared<CCommentParser>(spParent);
	else if (szTag == "{%")
		spResultParser = std::make_shared<CControlParser>(spParent);

	//检查生成结果
	if (!spResultParser)
	{
		STRING_STREAM(szResultError) << "不支持的标签(" << szTag << ")类型!";
		return false;
	}

	//设置左紧凑
	if (bCompact)
		spResultParser->SetLeftCompact(bCompact);
	return true;
}

//**********************************************************************
// 函数: VisitDescendant
// 功能: 访问节点后裔
//*********************************************************************
bool VisitDescendant(std::shared_ptr<INode> spRootNode, std::function<bool(std::shared_ptr<INode>)> pVisitor, bool bIncludeSelf = true, bool bTopToDown = true)
{
	//前访问自身
	if (bTopToDown && bIncludeSelf)
	{
		if (pVisitor(spRootNode))
			return true;
	}

	//访问子节点
	for (unsigned int nIndex = 0, cbCount = spRootNode->GetChildrenCount(); nIndex < cbCount; ++nIndex)
	{
		std::shared_ptr<INode> spChild = spRootNode->GetChild(nIndex);
		if (VisitDescendant(spChild, pVisitor, true, bTopToDown))
			return true;
	}

	//后访问自身
	if ((!bTopToDown) && bIncludeSelf)
	{
		if (pVisitor(spRootNode))
			return true;
	}
	return false;
}

//**********************************************************************
// 函数: GetRoot
// 功能: 得到根节点
//*********************************************************************
std::weak_ptr<INode> GetRoot(std::shared_ptr<INode> spNode)
{
	if (spNode->GetParent()._Get())
	{
		std::shared_ptr<INode> spParent = spNode->GetParent().lock();
		if (spParent)
			return GetRoot(spParent);
	}
	return spNode;
}

//**********************************************************************
// 函数: GetPrevNode
// 功能: 查询前节点
//*********************************************************************
std::shared_ptr<INode> GetPrevNode(std::shared_ptr<INode> spNode)
{
	struct Visit
	{
		static bool execute(std::shared_ptr<INode> spNode, std::shared_ptr<INode> spTargetNode, std::shared_ptr<INode>& spPrevNode)
		{
			if (spNode.get() == spTargetNode.get())
				return true;
			else
				spPrevNode = spNode;
			return false;
		}
	};
	std::shared_ptr<INode> spPrevNode;
	VisitDescendant(GetRoot(spNode).lock(), std::bind(Visit::execute, std::placeholders::_1, spNode, std::ref(spPrevNode)));
	return spPrevNode;
}

//**********************************************************************
// 函数: IsPrevNodeRightCompact
// 功能: 检查前节点是否右紧凑
//*********************************************************************
bool IsPrevNodeRightCompact(std::shared_ptr<INode> spNode)
{
	std::shared_ptr<INode> spPrevNode = GetPrevNode(spNode);
	if (spPrevNode)
		return spPrevNode->IsRightCompact();
	return false;
}

///////////////////////////////////////////////// INode类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: INode
// 功能: 默认构造函数
//*********************************************************************
INode::INode(void) : m_bLeftCompact(false), m_bRightCompact(false)
{
}

//**********************************************************************
// 函数: INode
// 功能: 构造函数
//*********************************************************************
INode::INode(node_weak_pointer_type spParent) : m_spParent(spParent), m_bLeftCompact(false), m_bRightCompact(false)
{
}

//**********************************************************************
// 函数: ~INode
// 功能: 虚拟析构函数
//*********************************************************************
INode::~INode(void)
{
}

//**********************************************************************
// 函数: AppendChild
// 功能: 添加子节点
//*********************************************************************
void INode::AppendChild(node_pointer_type spChild) 
{
	m_spChildren.push_back(spChild); 
	spChild->m_spParent = shared_from_this(); 
}

//**********************************************************************
// 函数: InsertChild
// 功能: 插入子节点
//*********************************************************************
void INode::InsertChild(unsigned int nChildIndex, node_pointer_type spChild)
{
	m_spChildren.insert(m_spChildren.begin() + nChildIndex, spChild);
	spChild->m_spParent = shared_from_this(); 
}

//**********************************************************************
// 函数: Render
// 功能: 渲染结果
//*********************************************************************
bool INode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	for (unsigned int nIndex = 0, cbCount = GetChildrenCount(); nIndex < cbCount; ++nIndex)
	{
		node_pointer_type spChildNode = GetChild(nIndex);
		if (!spChildNode->Render(aRenderContent, szResultContent, szResultError, pValueCB))
			return false;
	}
	return true;
}

///////////////////////////////////////////////// CTemplate类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CTemplate
// 功能: 默认构造函数
//*********************************************************************
CTemplate::CTemplate(void) : base_type(), m_spRootNode(std::make_shared<INode>())
{
}

//**********************************************************************
// 函数: ~CTemplate
// 功能: 析构函数
//*********************************************************************
CTemplate::~CTemplate(void)
{
}

//**********************************************************************
// 函数: Consume
// 功能: 解析模板
//*********************************************************************
bool CTemplate::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//循环解析直到结束
	while (nParseStartPos < szTemplate.size())
	{
		//构件解析器
		std::shared_ptr<IParser> spParser;
		if (IsStartTag(szTemplate, nParseStartPos))
		{
			//解析开始标签
			std::string szTag;
			bool bCompact = false;
			if (!ConsumeStartTag(szTemplate, nParseStartPos, szTag, bCompact, szResultError))
				return false;

			//生成解析器
			if (!CreateParser(szTag, shared_from_this(), bCompact, spParser, szResultError))
				return false;
		}
		else
			spParser = std::make_shared<CTextParser>(shared_from_this());

		//执行解析
		if (!spParser || !spParser->Consume(szTemplate, nParseStartPos, szResultError))
			return false;
	}
	return true;
}

//**********************************************************************
// 函数: Render
// 功能: 渲染结果
//*********************************************************************
bool CTemplate::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	return m_spRootNode->Render(aRenderContent, szResultContent, szResultError, pValueCB);
}

///////////////////////////////////////////////// CTextParser类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CTextParser
// 功能: 默认构造函数
//*********************************************************************
CTextParser::CTextParser(void) : base_type() 
{
}

//**********************************************************************
// 函数: CTextParser
// 功能: 构造函数
//*********************************************************************
CTextParser::CTextParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// 函数: ~CTextParser
// 功能: 析构函数
//*********************************************************************
CTextParser::~CTextParser(void)
{
}

//**********************************************************************
// 函数: Consume
// 功能: 解析模板
//*********************************************************************
bool CTextParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError) 
{
	//定位到下一个开始标签
	unsigned int nBeginPos = nParseStartPos;
	bool bSuccess = ConsumeToNextStartTag(szTemplate, nParseStartPos);
	if (nParseStartPos > nBeginPos)
		HandleText(szTemplate.substr(nBeginPos, nParseStartPos - nBeginPos));

	//返回
	return true;
}

//**********************************************************************
// 函数: HandleText
// 功能: 处理解析到的文本
//*********************************************************************
void CTextParser::HandleText(const std::string& szText)
{
	m_spTextNode = std::make_shared<CTextNode>();
	m_spTextNode->SetText(szText);
	parser_pointer_type spParent = GetParent().lock();
	if (spParent)
		spParent->GetNode()->AppendChild(m_spTextNode);
}

///////////////////////////////////////////////// CCommentParser类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CCommentParser
// 功能: 默认构造函数
//*********************************************************************
CCommentParser::CCommentParser(void) : base_type()
{
}

//**********************************************************************
// 函数: CCommentParser
// 功能: 构造函数
//*********************************************************************
CCommentParser::CCommentParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// 函数: ~CCommentParser
// 功能: 析构函数
//*********************************************************************
CCommentParser::~CCommentParser(void)
{
}

//**********************************************************************
// 函数: Consume
// 功能: 解析模板
//*********************************************************************
bool CCommentParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//查找结束标签
	unsigned int nBeginPos = nParseStartPos;
	std::string::size_type nFindPos = szTemplate.find("#}", nParseStartPos);
	if (nFindPos == std::string::npos)
	{
		STRING_STREAM(szResultError) << "注释无结束标签(" << nParseStartPos << ").";
		return false;
	}

	//处理右紧凑
	unsigned int nEndPos = nFindPos;
	if ((nFindPos > 0) && (szTemplate[nFindPos - 1] == '-'))
	{
		HandleRightCompact();
		--nEndPos;
	}

	//处理文本
	if (nEndPos > nBeginPos)
		HandleCommentText(szTemplate.substr(nBeginPos, nEndPos - nBeginPos));

	//设置结果并返回
	nParseStartPos = nFindPos + 2;
	return true;
}

//**********************************************************************
// 函数: HandleLeftCompact
// 功能: 处理左紧凑
//*********************************************************************
void CCommentParser::HandleLeftCompact(void) 
{
	VerifyNodeExists();
	m_spCommentNode->SetLeftCompact(true);
}

//**********************************************************************
// 函数: HandleRightCompact
// 功能: 处理右紧凑
//*********************************************************************
void CCommentParser::HandleRightCompact(void) 
{
	VerifyNodeExists();
	m_spCommentNode->SetRightCompact(true);
}

//**********************************************************************
// 函数: HandleCommentText
// 功能: 处理注释文本
//*********************************************************************
void CCommentParser::HandleCommentText(const std::string& szCommentText)
{
	VerifyNodeExists();
	m_spCommentNode->SetText(szCommentText);
}

//**********************************************************************
// 函数: VerifyNodeExists
// 功能: 确保节点存在
//*********************************************************************
void CCommentParser::VerifyNodeExists(void)
{
	if (!m_spCommentNode)
	{
		m_spCommentNode = std::make_shared<CCommentNode>();
		m_spCommentNode->SetLeftCompact(IsLeftCompact());
		parser_pointer_type spParent = GetParent().lock();
		if (spParent)
			spParent->GetNode()->AppendChild(m_spCommentNode);
	}
}

///////////////////////////////////////////////// CVariableParser类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CVariableParser
// 功能: 默认构造函数
//*********************************************************************
CVariableParser::CVariableParser(void) : base_type()
{
}

//**********************************************************************
// 函数: CVariableParser
// 功能: 构造函数
//*********************************************************************
CVariableParser::CVariableParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// 函数: ~CVariableParser
// 功能: 析构函数
//*********************************************************************
CVariableParser::~CVariableParser(void)
{
}

//**********************************************************************
// 函数: Consume
// 功能: 解析模板
//*********************************************************************
bool CVariableParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//解析下一个单词
	std::string szValue;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szValue, szResultError))
		return false;

	//检查下一个单词
	if (szValue.empty() || (!isalnum(szValue[0])))
	{
		STRING_STREAM(szResultError) << "变量未定义(" << nParseStartPos << ").";
		return false;
	}

	//处理变量
	HandleValue(szValue);

	//查找结束标签
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "}}", bCompact, szResultError))
		return false;
	m_spValueNode->SetRightCompact(bCompact);

	//返回
	return true;
}

//**********************************************************************
// 函数: HandleValue
// 功能: 处理解析的变量
//*********************************************************************
void CVariableParser::HandleValue(const std::string& szValue)
{
	m_spValueNode = std::make_shared<CVariableNode>();
	m_spValueNode->SetValue(szValue);
	m_spValueNode->SetLeftCompact(IsLeftCompact());
	parser_pointer_type spParent = GetParent().lock();
	if (spParent)
		spParent->GetNode()->AppendChild(m_spValueNode);
}

///////////////////////////////////////////////// CControlParser类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CControlParser
// 功能: 默认构造函数
//*********************************************************************
CControlParser::CControlParser(void) : base_type()
{
}

//**********************************************************************
// 函数: CControlParser
// 功能: 构造函数
//*********************************************************************
CControlParser::CControlParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// 函数: ~CControlParser
// 功能: 析构函数
//*********************************************************************
CControlParser::~CControlParser(void)
{
}

//**********************************************************************
// 函数: Consume
// 功能: 解析模板
//*********************************************************************
bool CControlParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//解析下一个命令
	std::string szNextWord;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szNextWord, szResultError))
		return false;

	//根据命令生产解析器
	std::shared_ptr<IParser> spNextParser;
	if (szNextWord == "for")
		spNextParser = std::make_shared<CForParser>(GetParent());
	else if (szNextWord == "if")
		spNextParser = std::make_shared<CIfParser>(GetParent());
	else if (szNextWord == "set")
		spNextParser = std::make_shared<CSetParser>(GetParent());
	if (spNextParser)
	{
		if (IsLeftCompact())
			spNextParser->SetLeftCompact(true);
		return spNextParser->Consume(szTemplate, nParseStartPos, szResultError);
	}
	else
	{
		STRING_STREAM(szResultError) << "未知命令(" << szNextWord << ':' << (nParseStartPos - szNextWord.size()) << ")!";
		return false;
	}
}

///////////////////////////////////////////////// CSetParser类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CSetParser
// 功能: 默认构造函数
//*********************************************************************
CSetParser::CSetParser(void) : base_type()
{
}

//**********************************************************************
// 函数: CSetParser
// 功能: 构造函数
//*********************************************************************
CSetParser::CSetParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// 函数: ~CSetParser
// 功能: 析构函数
//*********************************************************************
CSetParser::~CSetParser(void)
{
}

//**********************************************************************
// 函数: Consume
// 功能: 解析模板
//*********************************************************************
bool CSetParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//解析左变量名
	std::string szLeftValue;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szLeftValue, szResultError))
		return false;

	//解析等于符号
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "=", bCompact,szResultError))
		return false;

	//解析右变量名
	std::string szRightValue;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szRightValue, szResultError))
		return false;

	//解析命令结束标签
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;

	//处理结果
	HandleSetValue(szLeftValue, szRightValue);

	//返回
	return true;
}

//**********************************************************************
// 函数: HandleSetValue
// 功能: 处理解析的变量设置
//*********************************************************************
void CSetParser::HandleSetValue(const std::string& szLeftValue, const std::string& szRightValue)
{
	m_spSetNode = std::make_shared<CSetNode>();
	m_spSetNode->SetValue(szLeftValue, szRightValue);
	parser_pointer_type spParent = GetParent().lock();
	if (spParent)
		spParent->GetNode()->AppendChild(m_spSetNode);
}

///////////////////////////////////////////////// CBlockParser类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CBlockParser
// 功能: 默认构造函数
//*********************************************************************
CBlockParser::CBlockParser(void) : base_type()
{
}

//**********************************************************************
// 函数: CBlockParser
// 功能: 构造函数
//*********************************************************************
CBlockParser::CBlockParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// 函数: ~CBlockParser
// 功能: 析构函数
//*********************************************************************
CBlockParser::~CBlockParser(void)
{
}

//**********************************************************************
// 函数: SetNode
// 功能: 绑定节点
//*********************************************************************
void CBlockParser::SetNode(node_pointer_type spNode)
{
	m_spNode = spNode;
}

//**********************************************************************
// 函数: Consume
// 功能: 解析模板
//*********************************************************************
bool CBlockParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	return true;
}

///////////////////////////////////////////////// CIfParser类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CIfParser
// 功能: 默认构造函数
//*********************************************************************
CIfParser::CIfParser(void) : base_type(), m_nParseState(prpParseIf), m_spNode(std::make_shared<CIfNode>())
{
}

//**********************************************************************
// 函数: CIfParser
// 功能: 构造函数
//*********************************************************************
CIfParser::CIfParser(parser_weak_pointer_type spParent) : base_type(spParent), m_nParseState(prpParseIf), m_spNode(std::make_shared<CIfNode>())
{
	GetParent().lock()->GetNode()->AppendChild(m_spNode);
}

//**********************************************************************
// 函数: ~CIfParser
// 功能: 析构函数
//*********************************************************************
CIfParser::~CIfParser(void)
{
}

//**********************************************************************
// 函数: Consume
// 功能: 解析模板
//*********************************************************************
bool CIfParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	typedef bool(my_type::* consume_cb_type)(const std::string&, unsigned int&, std::string&);
	typedef struct{ unsigned int nParseState;  consume_cb_type pConsumeCB; } consume_pair_type;
	consume_pair_type s_aConsumePairs[] = 
	{   { prpParseIf, &my_type::ConsumeIf },
		{ prpParseIfContent, &my_type::ConsumeIfContent },
		{ prpParseElse, &my_type::ConsumeElse },
		{ prpParseElseContent, &my_type::ConsumeElseContent },
		{ prpEndIf, &my_type::ConsumeEndIf },
	};
	for (unsigned int nIndex = 0, cbCount = sizeof(s_aConsumePairs) / sizeof(s_aConsumePairs[0]); nIndex < cbCount; ++nIndex)
	{
		const consume_pair_type& consume_pair = s_aConsumePairs[nIndex];
		if (m_nParseState == consume_pair.nParseState)
			return (this->*(consume_pair.pConsumeCB))(szTemplate, nParseStartPos, szResultError);
	}
	STRING_STREAM(szResultError) << "未知解析状态:" << m_nParseState;
	return false;
}

//**********************************************************************
// 函数: ConsumeIf
// 功能: 解析if
//*********************************************************************
bool CIfParser::ConsumeIf(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//解析表达式值
	std::string szBoolExpress;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szBoolExpress, szResultError))
		return false;

	//如果存在取反
	bool bNot = false;
	if (szBoolExpress == "not")
	{
		bNot = true;
		if (!ConsumeNextWord(szTemplate, nParseStartPos, szBoolExpress, szResultError))
			return false;
	}
	else
	{
		if ((!szBoolExpress.empty()) && (szBoolExpress[0] == '!'))
		{
			bNot = true;
			szBoolExpress.erase(szBoolExpress.begin());
		}
	}

	//处理if结束标记
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;

	//处理值
	HandleBool(IsLeftCompact(), bNot, szBoolExpress, bCompact);
	m_nParseState = prpParseIfContent;
	return Consume(szTemplate, nParseStartPos, szResultError);
}

//**********************************************************************
// 函数: ConsumeIfContent
// 功能: 解析if内容
//*********************************************************************
bool CIfParser::ConsumeIfContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	while (nParseStartPos < szTemplate.size())
	{
		//构件解析器
		if (IsStartTag(szTemplate, nParseStartPos))
		{
			//解析开始标签
			std::string szTag;
			bool bCompact = false;
			if (!ConsumeStartTag(szTemplate, nParseStartPos, szTag, bCompact, szResultError))
				return false;

			//得到下一个命令
			unsigned int nStartTagPos = nParseStartPos;
			std::string szControl;
			if (!ConsumeNextWord(szTemplate, nParseStartPos, szControl, szResultError))
				return false;
			nParseStartPos = nStartTagPos;

			//根据命令解析
			if (szControl == "else")
			{
				m_nParseState = prpParseElse;
				m_spNode->SetElseLeftCompact(bCompact);
				return Consume(szTemplate, nParseStartPos, szResultError);
			}
			else if (szControl == "endif")
			{
				m_nParseState = prpEndIf;
				m_spNode->SetEndifLeftCompact(bCompact);
				return Consume(szTemplate, nParseStartPos, szResultError);
			}
			else
			{
				//生成并执行子解析器
				std::shared_ptr<CBlockParser> spBlockParser = std::make_shared<CBlockParser>();
				spBlockParser->SetNode(m_spNode->GetIfContentNode());
				std::shared_ptr<IParser> spParser;
				if (!CreateParser(szTag, spBlockParser, bCompact, spParser, szResultError))
					return false;
				if (!spParser || !spParser->Consume(szTemplate, nParseStartPos, szResultError))
					return false;
			}
		}
		else
		{
			//生成并执行子文本解析器
			std::shared_ptr<CBlockParser> spBlockParser = std::make_shared<CBlockParser>();
			spBlockParser->SetNode(m_spNode->GetIfContentNode());
			std::shared_ptr<IParser> spParser;
			spParser = std::make_shared<CTextParser>(spBlockParser);
			if (!spParser || !spParser->Consume(szTemplate, nParseStartPos, szResultError))
				return false;
		}
	}
	return true;
}

//**********************************************************************
// 函数: ConsumeElse
// 功能: 解析else
//*********************************************************************
bool CIfParser::ConsumeElse(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//解析else
	std::string szElse;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szElse, szResultError))
		return false;
	if (szElse != "else")
	{
		STRING_STREAM(szResultError) << "指定位置(" << nParseStartPos << ")期望\"else\",实际为(\"" << szElse << "\").";
		return false;
	}

	//解析else结束标签
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;
	m_nParseState = prpParseElseContent;
	m_spNode->SetHasElse(true);
	m_spNode->SetElseRightCompact(bCompact);
	return Consume(szTemplate, nParseStartPos, szResultError);
}

//**********************************************************************
// 函数: ConsumeElseContent
// 功能: 解析else内容
//*********************************************************************
bool CIfParser::ConsumeElseContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	while (nParseStartPos < szTemplate.size())
	{
		//构件解析器
		if (IsStartTag(szTemplate, nParseStartPos))
		{
			//解析开始标签
			std::string szTag;
			bool bCompact = false;
			if (!ConsumeStartTag(szTemplate, nParseStartPos, szTag, bCompact, szResultError))
				return false;

			//得到下一个命令
			unsigned int nStartTagPos = nParseStartPos;
			std::string szControl;
			if (!ConsumeNextWord(szTemplate, nParseStartPos, szControl, szResultError))
				return false;
			nParseStartPos = nStartTagPos;

			//根据命令解析
			if (szControl == "endif")
			{
				m_nParseState = prpEndIf;
				m_spNode->SetEndifLeftCompact(bCompact);
				return Consume(szTemplate, nParseStartPos, szResultError);
			}
			else
			{
				//生成并执行子解析器
				std::shared_ptr<CBlockParser> spBlockParser = std::make_shared<CBlockParser>();
				spBlockParser->SetNode(m_spNode->GetElseContentNode());
				std::shared_ptr<IParser> spParser;
				if (!CreateParser(szTag, spBlockParser, bCompact, spParser, szResultError))
					return false;
				if (!spParser || !spParser->Consume(szTemplate, nParseStartPos, szResultError))
					return false;
			}
		}
		else
		{
			//生成并执行子文本解析器
			std::shared_ptr<CBlockParser> spBlockParser = std::make_shared<CBlockParser>();
			spBlockParser->SetNode(m_spNode->GetElseContentNode());
			std::shared_ptr<IParser> spParser;
			spParser = std::make_shared<CTextParser>(spBlockParser);
			if (!spParser || !spParser->Consume(szTemplate, nParseStartPos, szResultError))
				return false;
		}
	}
	return true;
}

//**********************************************************************
// 函数: ConsumeEndIf
// 功能: 解析endif
//*********************************************************************
bool CIfParser::ConsumeEndIf(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//解析else
	std::string szEndIf;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szEndIf, szResultError))
		return false;
	if (szEndIf != "endif")
	{
		STRING_STREAM(szResultError) << "指定位置(" << nParseStartPos << ")期望\"endif\",实际为(\"" << szEndIf << "\").";
		return false;
	}

	//解析endif结束标签
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;
	m_spNode->SetRightCompact(bCompact);
	return true;
}

//**********************************************************************
// 函数: HandleBool
// 功能: 处理表达式
//*********************************************************************
void CIfParser::HandleBool(bool bLeftCompact, bool bNot, const std::string& szBoolExpress, bool bRightCompact)
{
	m_spNode->SetLeftCompact(bLeftCompact);
	m_spNode->SetValue(bNot, szBoolExpress);
	m_spNode->SetIfRightCompact(bRightCompact);
}

///////////////////////////////////////////////// CForParser类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CForParser
// 功能: 默认构造函数
//*********************************************************************
CForParser::CForParser(void) : base_type(), m_nParseState(prpParseFor), m_spNode(std::make_shared<CForNode>())
{
}

//**********************************************************************
// 函数: CForParser
// 功能: 构造函数
//*********************************************************************
CForParser::CForParser(parser_weak_pointer_type spParent) : base_type(spParent), m_nParseState(prpParseFor), m_spNode(std::make_shared<CForNode>())
{
	spParent.lock()->GetNode()->AppendChild(m_spNode);
}

//**********************************************************************
// 函数: ~CForParser
// 功能: 析构函数
//*********************************************************************
CForParser::~CForParser(void)
{
}

//**********************************************************************
// 函数: Consume
// 功能: 解析模板
//*********************************************************************
bool CForParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	typedef bool(my_type::* consume_cb_type)(const std::string&, unsigned int&, std::string&);
	typedef struct{ unsigned int nParseState;  consume_cb_type pConsumeCB; } consume_pair_type;
	consume_pair_type s_aConsumePairs[] =
	{ 
		{ prpParseFor, &my_type::ConsumeFor },
		{ prpParseForContent, &my_type::ConsumeForContent },
		{ prpEndFor, &my_type::ConsumeEndFor },
	};
	for (unsigned int nIndex = 0, cbCount = sizeof(s_aConsumePairs) / sizeof(s_aConsumePairs[0]); nIndex < cbCount; ++nIndex)
	{
		const consume_pair_type& consume_pair = s_aConsumePairs[nIndex];
		if (m_nParseState == consume_pair.nParseState)
			return (this->*(consume_pair.pConsumeCB))(szTemplate, nParseStartPos, szResultError);
	}
	STRING_STREAM(szResultError) << "未知解析状态:" << m_nParseState;
	return false;
}

//**********************************************************************
// 函数: ConsumeFor
// 功能: 解析for
//*********************************************************************
bool CForParser::ConsumeFor(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//解析容器元素
	std::string szVariable;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szVariable, szResultError))
		return false;

	//解析In
	std::string szNextWord;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szNextWord, szResultError))
		return false;
	if (szNextWord != "in")
	{
		STRING_STREAM(szResultError) << "期望\"in\",实际为\"" << szNextWord << "\":" << nParseStartPos;
		return false;
	}

	//解析容器
	std::string szContainer;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szContainer, szResultError))
		return false;
	if (!szContainer.empty() && !szVariable.empty())
		HandleForValues(szVariable, szContainer);

	//解析结束标签
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;
	m_spNode->SetForRightCompact(bCompact);

	//返回
	m_nParseState = prpParseForContent;
	return Consume(szTemplate, nParseStartPos, szResultError);
}

//**********************************************************************
// 函数: ConsumeForContent
// 功能: 解析for内容
//*********************************************************************
bool CForParser::ConsumeForContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	while (nParseStartPos < szTemplate.size())
	{
		//构件解析器
		if (IsStartTag(szTemplate, nParseStartPos))
		{
			//解析开始标签
			std::string szTag;
			bool bCompact = false;
			if (!ConsumeStartTag(szTemplate, nParseStartPos, szTag, bCompact, szResultError))
				return false;

			//得到下一个命令
			unsigned int nStartTagPos = nParseStartPos;
			std::string szControl;
			if (!ConsumeNextWord(szTemplate, nParseStartPos, szControl, szResultError))
				return false;
			nParseStartPos = nStartTagPos;

			//根据命令解析
			if (szControl == "endfor")
			{
				m_nParseState = prpEndFor;
				m_spNode->SetEndForLeftCompact(bCompact);
				return Consume(szTemplate, nParseStartPos, szResultError);
			}
			else
			{
				//生成并执行子解析器
				std::shared_ptr<IParser> spParser;
				if (!CreateParser(szTag, shared_from_this(), bCompact, spParser, szResultError))
					return false;
				if (!spParser || !spParser->Consume(szTemplate, nParseStartPos, szResultError))
					return false;
			}
		}
		else
		{
			//生成并执行子文本解析器
			std::shared_ptr<IParser> spParser;
			spParser = std::make_shared<CTextParser>(shared_from_this());
			if (!spParser || !spParser->Consume(szTemplate, nParseStartPos, szResultError))
				return false;
		}
	}
	return true;
}

//**********************************************************************
// 函数: ConsumeEndFor
// 功能: 解析for结束
//*********************************************************************
bool CForParser::ConsumeEndFor(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//解析endfor
	std::string szEndFor;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szEndFor, szResultError))
		return false;
	if (szEndFor != "endfor")
	{
		STRING_STREAM(szResultError) << "指定位置(" << nParseStartPos << ")期望\"endfor\",实际为(\"" << szEndFor << "\").";
		return false;
	}

	//解析endfor结束标签
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;
	m_spNode->SetRightCompact(bCompact);
	return true;
}

//**********************************************************************
// 函数: HandleForValues
// 功能: 处理for元素
//*********************************************************************
void CForParser::HandleForValues(const std::string& szVariable, const std::string& szContainer)
{
	m_spNode->SetValue(szVariable, szContainer);
	m_spNode->SetLeftCompact(IsLeftCompact());
}

///////////////////////////////////////////////// 节点类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: Render
// 功能: 渲染结果
//*********************************************************************
bool CTextNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	if (!m_szText.empty())
	{
		if (IsPrevNodeRightCompact(shared_from_this()))
			TrimLeft(m_szText);
		szResultContent += m_szText;
	}
	return true;
}

//**********************************************************************
// 函数: Render
// 功能: 渲染结果
//*********************************************************************
bool CCommentNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//处理左紧凑
	if (IsLeftCompact())
		TrimRight(szResultContent);
	return true;
}

//**********************************************************************
// 函数: Render
// 功能: 渲染结果
//*********************************************************************
bool CVariableNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//变量求值
	any_type aResultValue;
	if (!EvalVariable(m_szValue, aRenderContent, aResultValue, szResultError))
		return false;

	//调用回调函数
	std::string szValue;
	if (!pValueCB(m_szValue, aResultValue, szValue, szResultError))
		return false;

	//设置结果
	if (!szValue.empty())
		szResultContent += szValue;
	return true;
}

///////////////////////////////////////////////// CIfNode类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CIfNode
// 功能: 默认构造函数
//*********************************************************************
CIfNode::CIfNode(void) : base_type(), m_spIfContentNode(std::make_shared<node_type>()), m_spElseContentNode(std::make_shared<node_type>()), m_bNot(false),
m_bIfRightCompact(false), m_bElseLeftCompact(false), m_bElseRightCompact(false), m_bEndifLeftCompact(false), m_bHasElse(false)
{
}

//**********************************************************************
// 函数: CIfNode
// 功能: 构造函数
//*********************************************************************
CIfNode::CIfNode(node_weak_pointer_type spParent) : base_type(spParent), m_spIfContentNode(std::make_shared<node_type>()), m_spElseContentNode(std::make_shared<node_type>()), m_bNot(false),
m_bIfRightCompact(false), m_bElseLeftCompact(false), m_bElseRightCompact(false), m_bEndifLeftCompact(false), m_bHasElse(false)
{
}

//**********************************************************************
// 函数: ~CIfNode
// 功能: 虚拟析构函数
//*********************************************************************
CIfNode::~CIfNode(void)
{
}

//**********************************************************************
// 函数: SetValue
// 功能: 设置值
//*********************************************************************
void CIfNode::SetValue(bool bNot, const std::string& szExpress)
{
	m_bNot = bNot;
	m_szExpress = szExpress;
}

//**********************************************************************
// 函数: Render
// 功能: 渲染结果
//*********************************************************************
bool CIfNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB) 
{
	//处理if左缩进
	if (IsLeftCompact())
		TrimRight(szResultContent);

	//bool表达式求值
	any_type aResultValue;
	if (!EvalVariable(m_szExpress, aRenderContent, aResultValue, szResultError))
		return false;
	bool bBoolValue = to_bool(aResultValue);
	if (m_bNot)
		bBoolValue = !bBoolValue;

	//分支渲染
	if (bBoolValue)
	{
		//渲染分支
		std::string szTempResult;
		if (!m_spIfContentNode->Render(aRenderContent, szTempResult, szResultError, pValueCB))
			return false;

		//处理紧凑
		if (IsIfRightCompact())
			TrimLeft(szTempResult);
		if ((HasElse() && IsElseLeftCompact()) || IsEndifLeftCompact())
			TrimRight(szTempResult);

		//设置结果
		if (!szTempResult.empty())
			szResultContent += szTempResult;
		return true;
	}
	else
	{
		//检查是否有else分支
		if (!HasElse())
			return true;

		//渲染分支
		std::string szTempResult;
		if (!m_spElseContentNode->Render(aRenderContent, szTempResult, szResultError, pValueCB))
			return false;

		//处理紧凑
		if (IsElseRightCompact())
			TrimLeft(szTempResult);
		if (IsEndifLeftCompact())
			TrimRight(szTempResult);

		//设置结果
		if (!szTempResult.empty())
			szResultContent += szTempResult;
		return true;
	}
}

///////////////////////////////////////////////// CForNode类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CForNode
// 功能: 默认构造函数
//*********************************************************************
CForNode::CForNode(void) : base_type(), m_bForRightCompact(false), m_bEndForLeftCompact(false)
{
}

//**********************************************************************
// 函数: CForNode
// 功能: 构造函数
//*********************************************************************
CForNode::CForNode(node_weak_pointer_type spParent) : base_type(spParent), m_bForRightCompact(false), m_bEndForLeftCompact(false)
{
}

//**********************************************************************
// 函数: ~CForNode
// 功能: 虚拟析构函数
//*********************************************************************
CForNode::~CForNode(void)
{
}

//**********************************************************************
// 函数: SetValue
// 功能: 设置值
//*********************************************************************
void CForNode::SetValue(const std::string& szVariable, const std::string& szContainer)
{
	m_szVariable = szVariable;
	m_szContainer = szContainer;
}

//**********************************************************************
// 函数: Render
// 功能: 渲染结果
//*********************************************************************
bool CForNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//处理FOR左紧凑
	if (IsLeftCompact())
		TrimRight(szResultContent);

	//变量求值
	any_type aValue;
	if(!EvalVariable(m_szContainer, aRenderContent, aValue, szResultError))
		return false;

	//检查值是否在场景字典中
	if (aValue.empty())
	{
		STRING_STREAM(szResultError) << "变量(" << m_szContainer << ")不存在!";
		return false;
	}

	//访问容器
	auto visit_cb = [this, &aRenderContent, &szResultContent, &szResultError, pValueCB](const any_type& any_value, unsigned int nIndex, unsigned int cbCount)
	{
		return !RenderOnce(m_szVariable, any_value, nIndex, cbCount, aRenderContent, szResultContent, szResultError, pValueCB);
	};
	if (visit_container(aValue, visit_cb))
		return false;
	/*
	//得到容器值
	if (is_many(aValue))
	{
		many_type aContainer = boost::any_cast<many_type>(aValue);
		for (unsigned int nIndex = 0, cbCount = aContainer.size(); nIndex < cbCount; ++nIndex)
		{
			if (!RenderOnce(m_szVariable, aContainer[nIndex], nIndex, cbCount, aRenderContent, szResultContent, szResultError, pValueCB))
				return false;
		}
	}
	else if (is_dict(aValue))
	{
		dict_type aContainer = boost::any_cast<dict_type>(aValue);
		unsigned int nIndex = 0;
		unsigned int cbCount = aContainer.size();
		for (dict_type::const_iterator p = aContainer.begin(), pEnd = aContainer.end(); p != pEnd; ++p, ++nIndex)
		{
			if (!RenderOnce(m_szVariable, p->first, nIndex, cbCount, aRenderContent, szResultContent, szResultError, pValueCB))
				return false;
		}
	}
	*/
	return true;
}

//**********************************************************************
// 函数: RenderOnce
// 功能: 渲染一次
//*********************************************************************
bool CForNode::RenderOnce(const std::string& szVariableName, any_type aVariableValue, unsigned int nLoopIndex, unsigned int nLoopCount, const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	std::string szSubResult;
	if (!RenderOnceImpl(szVariableName, aVariableValue, nLoopIndex, nLoopCount, const_cast<dict_type&>(aRenderContent), szSubResult, szResultError, pValueCB))
		return false;
	if (IsForRightCompact())
		TrimLeft(szSubResult);
	if (IsEndForLeftCompact())
		TrimRight(szSubResult);
	if (!szSubResult.empty())
		szResultContent += szSubResult;
	return true;
}

//**********************************************************************
// 函数: RenderOnceImpl
// 功能: 渲染一次实现
//*********************************************************************
bool CForNode::RenderOnceImpl(const std::string& szVariableName, any_type aVariableValue, unsigned int nLoopIndex, unsigned int nLoopCount, dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//更新循环字典
	dict_type loop = 
	{ 
		{ "index0", nLoopIndex }, 
		{ "index", nLoopIndex + 1 },
		{ "revindex", nLoopCount - nLoopIndex }, 
		{ "revindex0", nLoopCount - nLoopIndex-1 },
		{ "first", (nLoopIndex == 0) },
		{ "last", (nLoopIndex == (nLoopCount-1)) },
		{ "length", nLoopCount },
	};
	aRenderContent["loop"] = loop;

	//设置变量值
	aRenderContent[szVariableName] = aVariableValue;

	//渲染场景
	for (unsigned int nIndex = 0, cbCount = GetChildrenCount(); nIndex < cbCount; ++nIndex)
	{
		auto spChild = GetChild(nIndex);
		if (!spChild->Render(aRenderContent, szResultContent, szResultError, pValueCB))
			return false;
	}
	return true;
}

///////////////////////////////////////////////// CSetNode类实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: CSetNode
// 功能: 默认构造函数
//*********************************************************************
CSetNode::CSetNode(void) : base_type()
{
}

//**********************************************************************
// 函数: CSetNode
// 功能: 构造函数
//*********************************************************************
CSetNode::CSetNode(node_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// 函数: ~CSetNode
// 功能: 虚拟析构函数
//*********************************************************************
CSetNode::~CSetNode(void)
{
}

//**********************************************************************
// 函数: SetValue
// 功能: 设置值
//*********************************************************************
void CSetNode::SetValue(const std::string& szLeftVariable, const std::string& szRightVariable)
{
	m_szLeftVariable = szLeftVariable;
	m_szRightVariable = szRightVariable;
}

//**********************************************************************
// 函数: Render
// 功能: 渲染结果
//*********************************************************************
bool CSetNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//处理左紧凑
	if (IsLeftCompact())
		TrimRight(szResultContent);

	//设置变量
	any_type aResultValue;
	if (!EvalVariable(m_szRightVariable, aRenderContent, aResultValue, szResultError))
		return false;

	//设置变量
	const_cast<dict_type&>(aRenderContent)[m_szLeftVariable] = aResultValue;
	return true;
}

///////////////////////////////////////////////// 接口函数实现 /////////////////////////////////////////////////

//**********************************************************************
// 函数: renderImpl
// 功能: 渲染模板实现
//*********************************************************************
bool renderImpl(const std::string& szTemplate, dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//解析模板
	std::shared_ptr<CTemplate> spTemplate = std::make_shared<CTemplate>();
	unsigned int nParseStartPos = 0;
	bool bSuccess = spTemplate->Consume(szTemplate, nParseStartPos, szResultError);
	if (!bSuccess)
		return bSuccess;

	//渲染文本
	return spTemplate->Render(aRenderContent, szResultContent, szResultError, pValueCB);
}

//**********************************************************************
// 函数: default_value_cb
// 功能: 缺省脚本变量渲染赋值回调函数
//*********************************************************************
bool default_value_cb(const std::string& szName, const any_type& aValue, std::string& szResultContent, std::string& szResultError)
{
	szResultContent = to_string(aValue);
	return true;
}

//**********************************************************************
// 函数: sql_value_cb
// 功能: SQL模板渲染脚本变量渲染赋值回调函数(将脚本变量位置用"?"赋值，并将变量保存到结果参数表中)
//*********************************************************************
bool sql_value_cb(const std::string& szName, const any_type& aValue, const std::string& szPlaceHolder, std::string& szResultContent, many_type& aResultParameters, std::string& szResultError)
{
	szResultContent = szPlaceHolder;
	aResultParameters.push_back(aValue);
	return true;
}

//**********************************************************************
// 函数: render
// 功能: 渲染模板
//*********************************************************************
bool render(const std::string& szTemplate, dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError)
{
	return renderImpl(szTemplate, aRenderContent, szResultContent, szResultError, default_value_cb);
}

//**********************************************************************
// 函数: render_sql
// 功能: 渲染SQL模板
//*********************************************************************
bool render_sql(const std::string& szTemplate, dict_type& aRenderContent, const std::string& szPlaceHolder, std::string& szResultSQL, many_type& aResultSQLParameters, std::string& szResultError)
{
	return renderImpl(szTemplate, aRenderContent, szResultSQL, szResultError, std::bind(sql_value_cb, std::placeholders::_1, std::placeholders::_2, szPlaceHolder, std::placeholders::_3, std::ref(aResultSQLParameters), std::placeholders::_4));
}

}//jinjia2名字空间结束