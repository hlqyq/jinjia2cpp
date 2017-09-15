//**********************************************************************
// Copyright (c) 2017
// ��˹���������С��.
// �ļ�: Jinja2.cpp
// ����: ��ģ���ṩһ���򻯰��Jinja2ģ������ʵ��
// ��ʷ:
//	���		�޸�ʱ��		�޸���		�޸�����
//	1			2017-09-08		������		�״�����
//*********************************************************************

//����ͷ�ļ�
#include "jinja2.h"
#include <memory>
#include<iomanip>
#include <sstream>

//jinjia2���ֿռ俪ʼ
namespace jinja2
{

///////////////////////////////////////////////// any���ͻ�����ѯ /////////////////////////////////////////////////

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

///////////////////////////////////////////////// anyֵ��ѯ /////////////////////////////////////////////////

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
	//ǰ��������
	if (bIncludeSelf && bTopDown)
	{
		if (visit_cb(any_value))
			return true;
	}

	//�����Ӷ���
	auto _visit = [&visit_cb, bTopDown](const any_type& any_value, unsigned, unsigned int)
	{
		if (visit_descendant(any_value, visit_cb, true, bTopDown))
			return true;
		return false;
	};
	if (visit_container(any_value, _visit))
		return true;

	//���������
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

///////////////////////////////////////////////// anyֵת�� /////////////////////////////////////////////////

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

///////////////////////////////////////////////// ���������� /////////////////////////////////////////////////

//**********************************************************************
// ����: CStringStream 
// Ŀ��: �ַ�����, ���ڴ������,��: STRING_STREAM(szResultError) << "ָ������" << szVariable << "������!";
//*********************************************************************
class CStringStream
{
////��������
public:
	typedef CStringStream											my_type;

////���졢��������
public:
	//���캯��
	CStringStream(std::string& szResultString) : m_szResultString(szResultString){}
	//��������
	~CStringStream(void) { m_szResultString = m_aStream.str(); }

////������ѯ
public:
	//�õ������
	std::ostream& stream(void) { return m_aStream; }

////���ݳ�Ա
private:
	std::string&			m_szResultString;	//����ַ���
	std::ostringstream		m_aStream;			//�����
};

//�ַ������궨��
#define STRING_STREAM(szResultString)									CStringStream(szResultString).stream()

///////////////////////////////////////////////// �ַ����������� /////////////////////////////////////////////////

//**********************************************************************
// ����: TrimLeft
// ����: �޳����ո�
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
// ����: TrimRight
// ����: �޳��Ҳ�ո�
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
// ����: Trim
// ����: �޳��ո�
//*********************************************************************
void Trim(std::string& szResultContent)
{
	TrimLeft(szResultContent);
	TrimRight(szResultContent);
}

///////////////////////////////////////////////// �ű��������Ͳ�ѯ���� /////////////////////////////////////////////////

//**********************************************************************
// ����: is_variable_char
// ����: �Ƿ�Ϊ�����ַ�(�����ַ������֡��»��߹���)
//*********************************************************************
bool is_variable_char(const char ch)
{
	return isalpha(ch) || isdigit(ch) || (ch == '_');
}

//**********************************************************************
// ����: is_variable
// ����: �Ƿ�Ϊ����(�����ַ������֡��»��߹���)
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
// ����: is_digit_variable
// ����: �Ƿ�Ϊ���ֱ���
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
// ����: is_text_variable
// ����: �Ƿ�Ϊ�ı�����(���ס�βΪ����)
//*********************************************************************
bool is_text_variable(const std::string& szVariable)
{
	//���ߴ�
	if (szVariable.size() < 2)
		return false;

	//����ס�β�ַ�
	const char& chFirst = szVariable[0];
	const char& chLast = szVariable[szVariable.size() - 1];
	return ((chFirst == '\'') || (chFirst == '\"')) && (chFirst == chLast);
}

//**********************************************************************
// ����: is_true_variable
// ����: �Ƿ�Ϊbool�泣��
//*********************************************************************
bool is_true_variable(const std::string& szVariable)
{
	return _stricmp(szVariable.c_str(), "true") == 0;
}

//**********************************************************************
// ����: is_false_variable
// ����: �Ƿ�Ϊbool�ٳ���
//*********************************************************************
bool is_false_variable(const std::string& szVariable)
{
	return _stricmp(szVariable.c_str(), "false") == 0;
}

///////////////////////////////////////////////// �ű����������������� /////////////////////////////////////////////////

//**********************************************************************
// ����: variable_consume_to_next_operator
// ����: ��λ����һ��������(��һ���Ǳ����ַ�,nPosָ���λ��)
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
// ����: variable_consume_to_next_close_operator
// ����: ��λ����һ������������,��"(" -> ")","[" => "]"��,֧��Ƕ��, nPosָ�����������λ��
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

///////////////////////////////////////////////// �ű�������ֵ���� /////////////////////////////////////////////////

//ǰ������
bool EvalObjectVariable(const any_type& aObject, const std::string& szAction, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError);
bool EvalVariable(const std::string& szVariable, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError);

//**********************************************************************
// ����: EvalSimpleVariable
// ����: �������ֵ,��: {{ value }}
//*********************************************************************
bool EvalSimpleVariable(const std::string& szVariable, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError, bool bStrict)
{
	//�������Ƿ����
	dict_type::const_iterator p = aRenderContent.find(szVariable);
	if (p == aRenderContent.end())
	{
		aResultValue = any_type();
		STRING_STREAM(szResultError) << "ָ������(\"" << szVariable << "\")������!";
		return bStrict ? false : true;
	}
	aResultValue = p->second;
	return true;
}

//**********************************************************************
// ����: EvalObjectIndexCall
// ����: ���������������,���ز������б��е�����(0��)
//*********************************************************************
bool EvalObjectIndexCall(const any_type& aObject, const std::string& szMethod, const std::string& szParameters, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//����Ƿ�Ϊ����
	if ((!is_many(aObject)) && (!is_string_record(aObject)))
	{
		STRING_STREAM(szResultError) << "����������ֵʧ�ܣ�ָ�������Ǽ���:" << aObject.type().name();
		return false;
	}

	//�����ı�����
	std::string szTemp = szParameters;
	if (is_text_variable(szParameters))
		szTemp = szParameters.substr(1, szParameters.size() - 2);

	//��������
	auto visit_cb = [&szParameters, &aResultValue, &szTemp](const any_type& any_value, unsigned int nIndex, unsigned int cbCount)
	{
		if (is_int_series(any_value))		//�����б������Ƚ�
		{
			if (get_int(any_value) == atoi(szParameters.c_str()))
			{
				aResultValue = nIndex;
				return true;
			}
		}
		else if (is_string_series(any_value))  //�ַ����б������Ƚ�
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
		STRING_STREAM(szResultError) << "����������ֵʧ�ܣ��Ҳ���ָ������:" << szParameters;
		return false;
	}
	return true;
}

//**********************************************************************
// ����: EvalObjectLengthCall
// ����: ��������ߴ����
//*********************************************************************
bool EvalObjectLengthCall(const any_type& aObject, const std::string& szMethod, const std::string& szParameters, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//�������Ƿ�Ϊ����
	if (!is_container(aObject))
	{
		STRING_STREAM(szResultError) << "���󳤶ȵ���ʧ�ܣ�ָ����������������:" << aObject.type().name();
		return false;
	}

	//�õ��Ӷ�������
	unsigned int cbCount = 0;
	visit_container(aObject, [&cbCount](const any_type& aObject,unsigned int,unsigned int){ ++cbCount; return false; });
	aResultValue = cbCount;
	return true;
}

//**********************************************************************
// ����: EvalObjectMethodCall
// ����: ���������������
//*********************************************************************
bool EvalObjectMethodCall(const any_type& aObject, const std::string& szMethod, const std::string& szParameters, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	if (szMethod == "index")
		return EvalObjectIndexCall(aObject, szMethod, szParameters, aRenderContent, aResultValue, szResultError);
	else if ((szMethod == "length") || (szMethod == "size"))
		return EvalObjectLengthCall(aObject, szMethod, szParameters, aRenderContent, aResultValue, szResultError);
	STRING_STREAM(szResultError) << "��֧�ֵĶ��󷽷�(\"" << szMethod << "\")!";
	return false;
}

//**********************************************************************
// ����: EvalObjectMethodVariable
// ����: �������������ֵ
//*********************************************************************
bool EvalObjectMethodVariable(const any_type& aObject, const std::string& szAction, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//������������
	unsigned int nOperatorPos = 1, nBeginPos = nOperatorPos;
	bool bSuccess = variable_consume_to_next_operator(szAction, nOperatorPos);
	if (!bSuccess)
	{
		STRING_STREAM(szResultError) << "�������(\"" << szAction << "\")������ֵʧ��,��λ������ʧ��!";
		return false;
	}
	std::string szMethod = szAction.substr(nBeginPos, nOperatorPos - nBeginPos);

	//��λ����������λ��,��ȡ������ʣ�����
	nBeginPos = nOperatorPos + 1;
	if (!variable_consume_to_next_close_operator(szAction, nOperatorPos, '(', ')'))
	{
		STRING_STREAM(szResultError) << "�������(\"" << szAction << "\")��λʧ��!";
		return false;
	}
	std::string szParameters = szAction.substr(nBeginPos, nOperatorPos - nBeginPos);
	Trim(szParameters);
	std::string szRemain = szAction.substr(nOperatorPos + 1);
	TrimRight(szRemain);

	//ִ�е��η�������
	any_type aReturnValue;
	if (!EvalObjectMethodCall(aObject, szMethod, szParameters, aRenderContent, aReturnValue, szResultError))
		return false;

	//�ݹ���û򷵻�
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
// ����: EvalObjectAttributeCall
// ����: �������������ֵ
//*********************************************************************
bool EvalObjectAttributeCall(const any_type& aObject, const std::string& szAttribute, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//��������֧��
	if ((szAttribute == "length") || (szAttribute == "size"))
		return EvalObjectLengthCall(aObject, szAttribute, "", aRenderContent, aResultValue, szResultError);

	//�������Ƿ����ֵ�
	if (!is_dict(aObject))
	{
		STRING_STREAM(szResultError) << "����(\"" << szAttribute << "\")��ȡʧ�ܣ��������ֵ����: " << aObject.type().name();
		return false;
	}

	//��������
	const dict_type* pDict = boost::any_cast<dict_type>(&aObject);
	dict_type::const_iterator p = pDict->find(szAttribute);
	if (p == pDict->end())
	{
		STRING_STREAM(szResultError) << "��������(\"" << szAttribute << "\")������!";
		return false;
	}
	else
	{
		aResultValue = p->second;
		return true;
	}
}

//**********************************************************************
// ����: EvalObjectAttributeVariable
// ����: �������������ֵ
//*********************************************************************
bool EvalObjectAttributeVariable(const any_type& aObject, const std::string& szAction, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//�������Խ���λ��
	unsigned int nOperatorPos = 1, nBeginPos = nOperatorPos;
	bool bSuccess = variable_consume_to_next_operator(szAction, nOperatorPos);
	std::string szAttribute = szAction.substr(nBeginPos, nOperatorPos - nBeginPos);
	Trim(szAttribute);
	std::string szRemain;
	if (bSuccess)
		szRemain = szAction.substr(nOperatorPos);
	TrimRight(szRemain);

	//���㱾����
	any_type aTempValue;
	if (!EvalObjectAttributeCall(aObject, szAttribute, aRenderContent, aTempValue, szResultError))
		return false;

	//����
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
// ����: EvalObjectSubscriptCall
// ����: ��������±���ֵ
//*********************************************************************
bool EvalObjectSubscriptCall(const any_type& aObject, const std::string& szParameter, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//ȷ��������������
	int nSubscriptIndex = 0;
	std::string szSubscriptIndex;
	bool bDigitValue = false;
	if (is_digit_variable(szParameter))	//�±�Ϊ����
	{
		bDigitValue = true;
		nSubscriptIndex = atoi(szParameter.c_str());
	}
	else if (is_text_variable(szParameter)) //�±�Ϊ�ı�
	{
		szSubscriptIndex = szParameter.substr(1, szParameter.size() - 2);
		bDigitValue = false;
	}
	else	//�±���Ҫ��ֵ����, ��: aObject[field_names.index('name')
	{	
		//�����±���ֵ
		any_type aValue;		
		if (!EvalVariable(szParameter, aRenderContent, aValue, szResultError))
			return false;

		//����±���ֵ��������������ж���������
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

	//���������±�
	if (bDigitValue)
	{
		auto visit_cb = [nSubscriptIndex, &aResultValue,&szResultError](const any_type& any_value, unsigned int nIndex, unsigned int cbCount)
		{
			//���������С
			if (nSubscriptIndex >= static_cast<int>(cbCount))
				STRING_STREAM(szResultError) << "����Խ��(" << nSubscriptIndex << '/' << cbCount << ')!';

			//����ֵ
			if (nIndex == nSubscriptIndex)
				aResultValue = any_value;
			return (nIndex == nSubscriptIndex);
		};
		return visit_container(aObject, visit_cb);	
	}
	else  //���������±�
	{
		//����������
		if (!is_dict(aObject))
		{
			STRING_STREAM(szResultError) << "�������ֵ�,��֧����������: " << aObject.type().name();
			return false;
		}

		//��������
		const dict_type* pDict = boost::any_cast<dict_type>(&aObject);
		dict_type::const_iterator p = pDict->find(szSubscriptIndex);
		if (p == pDict->end())
		{
			STRING_STREAM(szResultError) << "�������Բ�����(" << szSubscriptIndex << ").";
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
// ����: EvalObjectSubscriptVariable
// ����: ��������±���ֵ
//*********************************************************************
bool EvalObjectSubscriptVariable(const any_type& aObject, const std::string& szAction, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//��λ��С�����λ��,��ȡ�±������ʣ�෽��
	unsigned int nOperatorPos = 0;
	unsigned int nBeginPos = nOperatorPos + 1;
	if (!variable_consume_to_next_close_operator(szAction, nOperatorPos, '[', ']'))
	{
		STRING_STREAM(szResultError) << "�����±���ֵ����ʧ��:" << szAction;
		return false;
	}
	std::string szParameters = szAction.substr(nBeginPos, nOperatorPos - nBeginPos);
	Trim(szParameters);
	std::string szRemain = szAction.substr(nOperatorPos + 1);
	TrimRight(szRemain);

	//���㱾����
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
// ����: EvalObjectVariable
// ����: ���������ֵ
//*********************************************************************
bool EvalObjectVariable(const any_type& aObject, const std::string& szAction, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//������
	if (szAction.empty())
	{
		STRING_STREAM(szResultError) << "�������(\"" << szAction << "\")��ֵʧ��,δָ���������!";
		return false;
	}

	//�жϲ���
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

	//����
	STRING_STREAM(szResultError) << "δ֪�������(\"" << szAction << "\")!";
	return false;
}

//**********************************************************************
// ����: EvalVariable
// ����: ������ֵ
//*********************************************************************
bool EvalVariable(const std::string& szVariable, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError)
{
	//����bool����
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

	//�����ı�����
	if (is_text_variable(szVariable))
	{
		aResultValue = szVariable.substr(1, szVariable.size() - 2);
		return true;
	}

	//����򵥱���
	if (is_variable(szVariable))
		return EvalSimpleVariable(szVariable, aRenderContent, aResultValue, szResultError, false);

	//���������ֵ,��������
	unsigned int nOperatorPos = 0;
	bool bSuccess = variable_consume_to_next_operator(szVariable, nOperatorPos);
	if (!bSuccess)
	{
		STRING_STREAM(szResultError) << "����(\"" << szVariable << "\")��λ����������!";
		return false;
	}

	//��ȡ����,�������Ƿ����
	std::string szObject = szVariable.substr(0, nOperatorPos);
	std::string szRemain = szVariable.substr(nOperatorPos);
	dict_type::const_iterator p = aRenderContent.find(szObject);
	if (p == aRenderContent.end())
	{
		STRING_STREAM(szResultError) << "����(\"" << szVariable << "\")δ����!";
		return false;
	}
	return EvalObjectVariable(p->second, szRemain, aRenderContent, aResultValue, szResultError);
}

///////////////////////////////////////////////// ģ������������� /////////////////////////////////////////////////

//**********************************************************************
// ����: IsStartTag
// ����: �Ƿ�Ϊ��ʼ��ǩ
//*********************************************************************
bool IsStartTag(const std::string& szTemplate, unsigned int& nParseStartPos)
{
	//���ߴ�
	if ((nParseStartPos + 1) >= szTemplate.size())
		return false;

	//����һ���ַ�
	if (szTemplate[nParseStartPos] != '{')
		return false;

	//���ڶ����ַ�
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
// ����: ConsumeToNextStartTag
// ����: ��������һ����ǩ
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
// ����: ConsumeStartTag
// ����: ��������һ����ǩ
//*********************************************************************
bool ConsumeStartTag(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultTag, bool& bResultCompact, std::string& szResultError)
{
	//������
	if (!IsStartTag(szTemplate, nParseStartPos))
	{
		STRING_STREAM(szResultError) << "ָ��λ��(" << nParseStartPos << ")���ǿ��Ʊ�ǩ!";
		return false;
	}

	//��ȡ��ʼ��ǩ
	szResultTag = szTemplate.substr(nParseStartPos, 2);
	nParseStartPos += 2;
	bResultCompact = (nParseStartPos < szTemplate.size()) && (szTemplate[nParseStartPos] == '-');
	if (bResultCompact)
		++nParseStartPos;
	return true;
}

//**********************************************************************
// ����: ConsumeNextWord
// ����: ������һ������
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

	//����
	if (nEndPos >= nBeginPos)
	{
		szResultWord = szTemplate.substr(nBeginPos, nEndPos + 1 - nBeginPos);
		return true;
	}
	else
	{
		STRING_STREAM(szResultError) << "��������:" << nBeginPos;
		return false;
	}
}

//**********************************************************************
// ����: ConsumeNextEndTag
// ����: ������һ��������ǩ
//*********************************************************************
bool ConsumeNextEndTag(const std::string& szTemplate, unsigned int& nParseStartPos, const std::string& szEndTag, bool& bResultCompact, std::string& szResultError)
{
	bResultCompact = false;
	unsigned int cbCount = szTemplate.size();
	for (unsigned int& nPos = nParseStartPos; nPos < cbCount; ++nPos)
	{
		//�޳��ո�
		const char& ch = szTemplate[nPos];
		if ((ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n'))
			continue;

		//������
		if ((!bResultCompact) && (ch == '-'))
		{
			bResultCompact = true;
			continue;
		}

		//����Ƿ����
		std::string szPrefix = szTemplate.substr(nPos, szEndTag.size());
		if (szPrefix != szEndTag)
		{
			STRING_STREAM(szResultError) << "����\"" << szEndTag << "\",ʵ��\"" << szPrefix << "\":" << nParseStartPos;
			return false;
		}
		else
		{
			nPos += szEndTag.size();
			return true;
		}
	}

	//����
	return false;
}

///////////////////////////////////////////////// �ڵ�������������� /////////////////////////////////////////////////

//�ڵ㸳ֵ�ص���������
typedef std::function<bool(const std::string&, const any_type&, std::string&, std::string&)> value_cb_type;

//**********************************************************************
// ����: INode 
// Ŀ��: �ڵ� 
//*********************************************************************
class INode : public std::enable_shared_from_this<INode>
{
////��������
public:
	typedef INode												node_type;
	typedef std::weak_ptr<node_type>							node_weak_pointer_type;
	typedef std::shared_ptr<node_type>							node_pointer_type;
	typedef std::vector<node_pointer_type>						node_container_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	INode(void);
	//���캯��
	INode(node_weak_pointer_type spParent);
	//������������
	virtual ~INode(void);

////������ѯ
public:
	//�õ����ڵ�(����)
	node_weak_pointer_type GetParent(void) const { return m_spParent; }
	//�õ��ӽڵ�����
	unsigned int GetChildrenCount(void) const { return m_spChildren.size(); }
	//�õ��ӽڵ�
	node_pointer_type GetChild(unsigned int nChildIndex) const { return m_spChildren[nChildIndex]; }
	//�Ƿ�������
	bool IsLeftCompact(void) const { return m_bLeftCompact; }
	//�Ƿ��Ҳ����
	bool IsRightCompact(void) const { return m_bRightCompact; }

////INode����
public:
	//����ӽڵ�
	void AppendChild(node_pointer_type spChild);
	//�����ӽڵ�
	void InsertChild(unsigned int nChildIndex, node_pointer_type spChild);
	//����������
	void SetLeftCompact(bool bCompact = true) { m_bLeftCompact = bCompact; }
	//�����Ҳ����
	void SetRightCompact(bool bCompact = true) { m_bRightCompact = bCompact; }

////INode�ӿ�ʵ��
public:
	//��Ⱦ���
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////���ݳ�Ա
private:
	node_weak_pointer_type			m_spParent;			//���ڵ�(��ָ��)
	node_container_type				m_spChildren;		//�ӽڵ�(ǿָ��)
	bool							m_bLeftCompact;		//�Ƿ�������
	bool							m_bRightCompact;	//�Ƿ��Ҳ����
};

//**********************************************************************
// ����: IParser 
// Ŀ��: ����״̬���ӿ�
//*********************************************************************
class IParser : public std::enable_shared_from_this<IParser>
{
////��������
public:
	typedef IParser											my_type;
	typedef IParser											parser_type;
	typedef std::shared_ptr<parser_type>					parser_pointer_type;
	typedef std::weak_ptr<parser_type>						parser_weak_pointer_type;
	typedef INode::node_pointer_type						node_pointer_type;
	typedef INode::node_weak_pointer_type					node_weak_pointer_type;

////���졢��������
public:
	//���캯��
	IParser(void) : m_bLeftCompact(false){}
	//���캯��
	IParser(parser_weak_pointer_type spParent) : m_spParent(spParent), m_bLeftCompact(false) {}
	//������������
	virtual ~IParser(void){}

////������ѯ
public:
	//�õ�������
	parser_weak_pointer_type GetParent(void) const { return m_spParent; }
	//�Ƿ������
	bool IsLeftCompact(void) const { return m_bLeftCompact; }

////����
public:
	//���������
	void SetLeftCompact(bool bCompact = true) { m_bLeftCompact = bCompact; }

////IParser�ӿ�
public:
	//����ģ��
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError) = 0;
	//�õ��ڵ�
	virtual std::shared_ptr<INode> GetNode(void) const = 0;

////���ݳ�Ա
private:
	parser_weak_pointer_type					m_spParent;			//����������
	bool										m_bLeftCompact;		//�Ƿ������
};

//**********************************************************************
// ����: CTemplate 
// Ŀ��: ��ֵ������
//*********************************************************************
class CTemplate : public IParser
{
////��������
public:
	typedef CTemplate										my_type;
	typedef IParser											base_type;
	typedef INode::node_pointer_type						node_pointer_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CTemplate(void);
	//��������
	~CTemplate(void);

////����
public:
	//��Ⱦ���
	bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////IParser�ӿ�ʵ��
public:
	//����ģ��
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//�õ��ڵ�
	virtual std::shared_ptr<INode> GetNode(void) const { return m_spRootNode; }

////���ݳ�Ա
private:
	std::shared_ptr<INode>					m_spRootNode;
};

//**********************************************************************
// ����: CTextNode 
// Ŀ��: �ı��ڵ� 
//*********************************************************************
class CTextNode : public INode
{
////��������
public:
	typedef CTextNode										my_type;
	typedef INode											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CTextNode(void) : base_type() {}
	//���캯��
	CTextNode(node_weak_pointer_type spParent) : base_type(spParent){}
	//��������
	~CTextNode(void){}

////����
public:
	//�����ı�
	void SetText(const std::string& szText){ m_szText = szText; }

////INode�ӿ�ʵ��
public:
	//��Ⱦ���
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////���ݳ�Ա
private:
	std::string					m_szText;
};

//**********************************************************************
// ����: CCommentNode 
// Ŀ��: ע�ͽڵ� 
//*********************************************************************
class CCommentNode : public INode
{
////��������
public:
	typedef CTextNode										my_type;
	typedef INode											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CCommentNode(void) : base_type() {}
	//���캯��
	CCommentNode(node_weak_pointer_type spParent) : base_type(spParent){}
	//��������
	~CCommentNode(void){}

////����
public:
	//�����ı�
	void SetText(const std::string& szText){ m_szText = szText; }

////INode�ӿ�ʵ��
public:
	//��Ⱦ���
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////���ݳ�Ա
private:
	std::string					m_szText;
};

//**********************************************************************
// ����: CVariableNode 
// Ŀ��: ֵ�ڵ� 
//*********************************************************************
class CVariableNode : public INode
{
////��������
public:
	typedef CVariableNode									my_type;
	typedef INode											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CVariableNode(void) : base_type() {}
	//���캯��
	CVariableNode(node_weak_pointer_type spParent) : base_type(spParent){}
	//��������
	~CVariableNode(void){}

////����
public:
	//����ֵ
	void SetValue(const std::string& szValue) { m_szValue = szValue; }

////INode�ӿ�ʵ��
public:
	//��Ⱦ���
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////ִ�к���
private:
	//��ֵ
	static any_type EvalValue(const std::string& szValue, const dict_type& aRenderContent);

////���ݳ�Ա
private:
	std::string      m_szValue;
};

//**********************************************************************
// ����: CIfNode 
// Ŀ��: bool�ڵ� 
//*********************************************************************
class CIfNode : public INode
{
////��������
public:
	typedef CIfNode											my_type;
	typedef INode											base_type;

	////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CIfNode(void);
	//���캯��
	CIfNode(node_weak_pointer_type spParent);
	//��������
	~CIfNode(void);

////������ѯ
public:
	//�õ�if���ݽڵ�
	node_pointer_type GetIfContentNode(void) { return m_spIfContentNode; }
	//�õ�if���ݽڵ�(����)
	node_pointer_type GetIfContentNode(void) const { return m_spIfContentNode; }
	//�õ�else���ݽڵ�
	node_pointer_type GetElseContentNode(void) { return m_spElseContentNode; }
	//�õ�else���ݽڵ�(����)
	node_pointer_type GetElseContentNode(void) const { return m_spElseContentNode; }
	//��ѯIf�Ҳ����
	bool IsIfRightCompact(void) const { return m_bIfRightCompact; }
	//��ѯElse������
	bool IsElseLeftCompact(void) const { return m_bElseLeftCompact; }
	//��ѯElse�Ҳ����
	bool IsElseRightCompact(void) const { return m_bElseRightCompact; }
	//��ѯEndif������
	bool IsEndifLeftCompact(void) const { return m_bEndifLeftCompact; }
	//��ѯ�Ƿ���else
	bool HasElse(void) const { return m_bHasElse; }

////����
public:
	//����ֵ
	void SetValue(bool bNot, const std::string& szExpress);
	//����If�Ҳ����
	void SetIfRightCompact(bool bCompact = true) { m_bIfRightCompact = bCompact; }
	//����Else������
	void SetElseLeftCompact(bool bCompact = true) { m_bElseLeftCompact = bCompact; }
	//����Else�Ҳ����
	void SetElseRightCompact(bool bCompact = true) { m_bElseRightCompact = bCompact; }
	//����Endif������
	void SetEndifLeftCompact(bool bCompact = true) { m_bEndifLeftCompact = bCompact; }
	//�����Ƿ���else
	void SetHasElse(bool bHas = true) { m_bHasElse = bHas; }

////INode�ӿ�ʵ��
public:
	//��Ⱦ���
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////���ݳ�Ա
private:
	bool						m_bNot;					//�Ƿ�ֵȡ��
	std::string					m_szExpress;			//���ʽ
	node_pointer_type			m_spIfContentNode;		//if���ݽڵ�
	node_pointer_type			m_spElseContentNode;	//else���ݽڵ�
	bool						m_bIfRightCompact;		//If�Ҳ����
	bool						m_bElseLeftCompact;		//Else������
	bool						m_bElseRightCompact;	//Else�Ҳ����
	bool						m_bEndifLeftCompact;	//Endif������
	bool						m_bHasElse;				//�Ƿ���else
};

//**********************************************************************
// ����: CForNode 
// Ŀ��: ѭ���ڵ� 
//*********************************************************************
class CForNode : public INode
{
////��������
public:
	typedef CForNode											my_type;
	typedef INode											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CForNode(void);
	//���캯��
	CForNode(node_weak_pointer_type spParent);
	//��������
	~CForNode(void);

////������ѯ
public:
	//��ѯFor�Ҳ����
	bool IsForRightCompact(void) const { return m_bForRightCompact; }
	//��ѯElse������
	bool IsEndForLeftCompact(void) const { return m_bEndForLeftCompact; }

////����
public:
	//����ֵ
	void SetValue(const std::string& szVariable, const std::string& szContainer);
	//����For�Ҳ����
	void SetForRightCompact(bool bCompact = true) { m_bForRightCompact = bCompact; }
	//����EndFor������
	void SetEndForLeftCompact(bool bCompact = true) { m_bEndForLeftCompact = bCompact; }

////INode�ӿ�ʵ��
public:
	//��Ⱦ���
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////ʵ�ֺ���
private:
	//��Ⱦһ��
	bool RenderOnce(const std::string& szVariableName, any_type aVariableValue, unsigned int nLoopIndex, unsigned int nLoopCount, const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);
	//��Ⱦһ��ʵ��
	bool RenderOnceImpl(const std::string& szVariableName, any_type aVariableValue, unsigned int nLoopIndex, unsigned int nLoopCount, dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////���ݳ�Ա
private:
	std::string					m_szVariable;			//ѭ��������
	std::string					m_szContainer;			//����������
	bool						m_bForRightCompact;		//For�Ҳ����
	bool						m_bEndForLeftCompact;	//EndFor������
};

//**********************************************************************
// ����: CSetNode 
// Ŀ��: ��ֵ�ڵ� 
//*********************************************************************
class CSetNode : public INode
{
////��������
public:
	typedef CSetNode										my_type;
	typedef INode											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CSetNode(void);
	//���캯��
	CSetNode(node_weak_pointer_type spParent);
	//��������
	~CSetNode(void);

////����
public:
	//����ֵ
	void SetValue(const std::string& szLeftVariable, const std::string& szRightVariable);

////INode�ӿ�ʵ��
public:
	//��Ⱦ���
	virtual bool Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB);

////���ݳ�Ա
private:
	std::string				m_szLeftVariable;
	std::string				m_szRightVariable;
};

//**********************************************************************
// ����: CTextParser 
// Ŀ��: �ı�������
//*********************************************************************
class CTextParser : public IParser
{
////��������
public:
	typedef CTextParser										my_type;
	typedef IParser											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CTextParser(void);
	//���캯��
	CTextParser(parser_weak_pointer_type spParent);
	//��������
	virtual ~CTextParser(void);

////IParser�ӿ�ʵ��
public:
	//����ģ��
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//�õ��ڵ�
	virtual std::shared_ptr<INode> GetNode(void) const { return m_spTextNode; }

////ִ�к���
private:
	//������������ı�
	void HandleText(const std::string& szText);

////���ݳ�Ա
private:
	std::shared_ptr<CTextNode>					m_spTextNode;
};

//**********************************************************************
// ����: CCommentParser 
// Ŀ��: ע�ͽ�����
//*********************************************************************
class CCommentParser : public IParser
{
////��������
public:
	typedef CCommentParser									my_type;
	typedef IParser											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CCommentParser(void);
	//���캯��
	CCommentParser(parser_weak_pointer_type spParent);
	//��������
	~CCommentParser(void);

////IParser�ӿ�ʵ��
public:
	//����ģ��
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//�õ��ڵ�
	virtual std::shared_ptr<INode> GetNode(void) const { return std::shared_ptr<INode>(); }

////ִ�к���
private:
	//���������
	void HandleLeftCompact(void);
	//�����ҽ���
	void HandleRightCompact(void);
	//����ע���ı�
	void HandleCommentText(const std::string& szCommentText);
	//ȷ���ڵ����
	void VerifyNodeExists(void);

////���ݳ�Ա
private:
	std::shared_ptr<CCommentNode>					m_spCommentNode;
};

//**********************************************************************
// ����: CVariableParser 
// Ŀ��: ����ֵ������
//*********************************************************************
class CVariableParser : public IParser
{
////��������
public:
	typedef CVariableParser									my_type;
	typedef IParser											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CVariableParser(void);
	//���캯��
	CVariableParser(parser_weak_pointer_type spParent);
	//��������
	~CVariableParser(void);

////IParser�ӿ�ʵ��
public:
	//����ģ��
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//�õ��ڵ�
	virtual std::shared_ptr<INode> GetNode(void) const { return m_spValueNode; }

////ִ�к���
private:
	//��������ı���
	void HandleValue(const std::string& szValue);

////���ݳ�Ա
private:
	std::shared_ptr<CVariableNode>		m_spValueNode;
};

//**********************************************************************
// ����: CControlParser 
// Ŀ��: ���ƽڵ������
//*********************************************************************
class CControlParser : public IParser
{
////��������
public:
	typedef CControlParser									my_type;
	typedef IParser											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CControlParser(void);
	//���캯��
	CControlParser(parser_weak_pointer_type spParent);
	//��������
	~CControlParser(void);

////IParser�ӿ�ʵ��
public:
	//����ģ��
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//�õ��ڵ�
	virtual std::shared_ptr<INode> GetNode(void) const { return std::shared_ptr<INode>(); }
};

//**********************************************************************
// ����: CSetParser 
// Ŀ��: ��ֵ������
//*********************************************************************
class CSetParser : public IParser
{
////��������
public:
	typedef CSetParser										my_type;
	typedef IParser											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CSetParser(void);
	//���캯��
	CSetParser(parser_weak_pointer_type spParent);
	//��������
	~CSetParser(void);

////IParser�ӿ�ʵ��
public:
	//����ģ��
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//�õ��ڵ�
	virtual std::shared_ptr<INode> GetNode(void) const { return std::shared_ptr<INode>(); }

////ʵ�ֺ���
private:
	//��������ı�������
	void HandleSetValue(const std::string& szLeftValue, const std::string& szRightValue);

////���ݳ�Ա
private:
	std::shared_ptr<CSetNode>		m_spSetNode;
};

//**********************************************************************
// ����: CSubBlockParser 
// Ŀ��: �������(����if����for����)
//*********************************************************************
class CBlockParser : public IParser
{
////��������
public:
	typedef CBlockParser									my_type;
	typedef IParser											base_type;

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CBlockParser(void);
	//���캯��
	CBlockParser(parser_weak_pointer_type spParent);
	//��������
	~CBlockParser(void);

////����
public:
	//�󶨽ڵ�
	void SetNode(node_pointer_type spNode);

////IParser�ӿ�ʵ��
public:
	//����ģ��
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//�õ��ڵ�
	virtual std::shared_ptr<INode> GetNode(void) const { return m_spNode; }

////���ݳ�Ա
private:
	node_pointer_type				m_spNode;
};

//**********************************************************************
// ����: CIfParser 
// Ŀ��: bool������
//*********************************************************************
class CIfParser : public IParser
{
////��������
public:
	typedef CIfParser										my_type;
	typedef IParser											base_type;
	enum{ prpParseIf, prpParseIfContent, prpParseElse, prpParseElseContent, prpEndIf };

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CIfParser(void);
	//���캯��
	CIfParser(parser_weak_pointer_type spParent);
	//��������
	~CIfParser(void);

////IParser�ӿ�ʵ��
public:
	//����ģ��
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//�õ��ڵ�
	virtual std::shared_ptr<INode> GetNode(void) const { return std::shared_ptr<INode>(); }

////ִ�к���
private:
	//������ʽ
	void HandleBool(bool bLeftCompact, bool bNot, const std::string& szBoolExpress, bool bRightCompact);
	//����if
	bool ConsumeIf(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//����if����
	bool ConsumeIfContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//����else
	bool ConsumeElse(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//����else����
	bool ConsumeElseContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//����if����
	bool ConsumeEndIf(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);

////���ݳ�Ա
private:
	unsigned int					m_nParseState;		//����״̬
	std::shared_ptr<CIfNode>		m_spNode;			//if�ڵ�
};

//**********************************************************************
// ����: CForParser 
// Ŀ��: ѭ��������
//*********************************************************************
class CForParser : public IParser
{
////��������
public:
	typedef CForParser										my_type;
	typedef IParser											base_type;
	enum{ prpParseFor, prpParseForContent, prpEndFor };

////���졢��������
public:
	//Ĭ�Ϲ��캯��
	CForParser(void);
	//���캯��
	CForParser(parser_weak_pointer_type spParent);
	//��������
	~CForParser(void);

////IParser�ӿ�ʵ��
public:
	//����ģ��
	virtual bool Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//�õ��ڵ�
	virtual std::shared_ptr<INode> GetNode(void) const { return m_spNode; }

////ִ�к���
private:
	//����for
	bool ConsumeFor(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//����for����
	bool ConsumeForContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//����for����
	bool ConsumeEndFor(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError);
	//����forԪ��
	void HandleForValues(const std::string& szVariable, const std::string& szContainer);

////���ݳ�Ա
private:
	unsigned int				m_nParseState;		//����״̬
	std::shared_ptr<CForNode>	m_spNode;			//for�ڵ�
};

///////////////////////////////////////////////// �ڵ���������������� /////////////////////////////////////////////////

//**********************************************************************
// ����: CreateParser
// ����: ��������һ����ǩ
//*********************************************************************
bool CreateParser(const std::string& szTag, std::weak_ptr<IParser> spParent, bool bCompact, std::shared_ptr<IParser>& spResultParser, std::string& szResultError)
{
	//���ɽ�����
	if (szTag == "{{")
		spResultParser = std::make_shared<CVariableParser>(spParent);
	else if (szTag == "{#")
		spResultParser = std::make_shared<CCommentParser>(spParent);
	else if (szTag == "{%")
		spResultParser = std::make_shared<CControlParser>(spParent);

	//������ɽ��
	if (!spResultParser)
	{
		STRING_STREAM(szResultError) << "��֧�ֵı�ǩ(" << szTag << ")����!";
		return false;
	}

	//���������
	if (bCompact)
		spResultParser->SetLeftCompact(bCompact);
	return true;
}

//**********************************************************************
// ����: VisitDescendant
// ����: ���ʽڵ����
//*********************************************************************
bool VisitDescendant(std::shared_ptr<INode> spRootNode, std::function<bool(std::shared_ptr<INode>)> pVisitor, bool bIncludeSelf = true, bool bTopToDown = true)
{
	//ǰ��������
	if (bTopToDown && bIncludeSelf)
	{
		if (pVisitor(spRootNode))
			return true;
	}

	//�����ӽڵ�
	for (unsigned int nIndex = 0, cbCount = spRootNode->GetChildrenCount(); nIndex < cbCount; ++nIndex)
	{
		std::shared_ptr<INode> spChild = spRootNode->GetChild(nIndex);
		if (VisitDescendant(spChild, pVisitor, true, bTopToDown))
			return true;
	}

	//���������
	if ((!bTopToDown) && bIncludeSelf)
	{
		if (pVisitor(spRootNode))
			return true;
	}
	return false;
}

//**********************************************************************
// ����: GetRoot
// ����: �õ����ڵ�
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
// ����: GetPrevNode
// ����: ��ѯǰ�ڵ�
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
// ����: IsPrevNodeRightCompact
// ����: ���ǰ�ڵ��Ƿ��ҽ���
//*********************************************************************
bool IsPrevNodeRightCompact(std::shared_ptr<INode> spNode)
{
	std::shared_ptr<INode> spPrevNode = GetPrevNode(spNode);
	if (spPrevNode)
		return spPrevNode->IsRightCompact();
	return false;
}

///////////////////////////////////////////////// INode��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: INode
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
INode::INode(void) : m_bLeftCompact(false), m_bRightCompact(false)
{
}

//**********************************************************************
// ����: INode
// ����: ���캯��
//*********************************************************************
INode::INode(node_weak_pointer_type spParent) : m_spParent(spParent), m_bLeftCompact(false), m_bRightCompact(false)
{
}

//**********************************************************************
// ����: ~INode
// ����: ������������
//*********************************************************************
INode::~INode(void)
{
}

//**********************************************************************
// ����: AppendChild
// ����: ����ӽڵ�
//*********************************************************************
void INode::AppendChild(node_pointer_type spChild) 
{
	m_spChildren.push_back(spChild); 
	spChild->m_spParent = shared_from_this(); 
}

//**********************************************************************
// ����: InsertChild
// ����: �����ӽڵ�
//*********************************************************************
void INode::InsertChild(unsigned int nChildIndex, node_pointer_type spChild)
{
	m_spChildren.insert(m_spChildren.begin() + nChildIndex, spChild);
	spChild->m_spParent = shared_from_this(); 
}

//**********************************************************************
// ����: Render
// ����: ��Ⱦ���
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

///////////////////////////////////////////////// CTemplate��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CTemplate
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CTemplate::CTemplate(void) : base_type(), m_spRootNode(std::make_shared<INode>())
{
}

//**********************************************************************
// ����: ~CTemplate
// ����: ��������
//*********************************************************************
CTemplate::~CTemplate(void)
{
}

//**********************************************************************
// ����: Consume
// ����: ����ģ��
//*********************************************************************
bool CTemplate::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//ѭ������ֱ������
	while (nParseStartPos < szTemplate.size())
	{
		//����������
		std::shared_ptr<IParser> spParser;
		if (IsStartTag(szTemplate, nParseStartPos))
		{
			//������ʼ��ǩ
			std::string szTag;
			bool bCompact = false;
			if (!ConsumeStartTag(szTemplate, nParseStartPos, szTag, bCompact, szResultError))
				return false;

			//���ɽ�����
			if (!CreateParser(szTag, shared_from_this(), bCompact, spParser, szResultError))
				return false;
		}
		else
			spParser = std::make_shared<CTextParser>(shared_from_this());

		//ִ�н���
		if (!spParser || !spParser->Consume(szTemplate, nParseStartPos, szResultError))
			return false;
	}
	return true;
}

//**********************************************************************
// ����: Render
// ����: ��Ⱦ���
//*********************************************************************
bool CTemplate::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	return m_spRootNode->Render(aRenderContent, szResultContent, szResultError, pValueCB);
}

///////////////////////////////////////////////// CTextParser��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CTextParser
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CTextParser::CTextParser(void) : base_type() 
{
}

//**********************************************************************
// ����: CTextParser
// ����: ���캯��
//*********************************************************************
CTextParser::CTextParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// ����: ~CTextParser
// ����: ��������
//*********************************************************************
CTextParser::~CTextParser(void)
{
}

//**********************************************************************
// ����: Consume
// ����: ����ģ��
//*********************************************************************
bool CTextParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError) 
{
	//��λ����һ����ʼ��ǩ
	unsigned int nBeginPos = nParseStartPos;
	bool bSuccess = ConsumeToNextStartTag(szTemplate, nParseStartPos);
	if (nParseStartPos > nBeginPos)
		HandleText(szTemplate.substr(nBeginPos, nParseStartPos - nBeginPos));

	//����
	return true;
}

//**********************************************************************
// ����: HandleText
// ����: ������������ı�
//*********************************************************************
void CTextParser::HandleText(const std::string& szText)
{
	m_spTextNode = std::make_shared<CTextNode>();
	m_spTextNode->SetText(szText);
	parser_pointer_type spParent = GetParent().lock();
	if (spParent)
		spParent->GetNode()->AppendChild(m_spTextNode);
}

///////////////////////////////////////////////// CCommentParser��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CCommentParser
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CCommentParser::CCommentParser(void) : base_type()
{
}

//**********************************************************************
// ����: CCommentParser
// ����: ���캯��
//*********************************************************************
CCommentParser::CCommentParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// ����: ~CCommentParser
// ����: ��������
//*********************************************************************
CCommentParser::~CCommentParser(void)
{
}

//**********************************************************************
// ����: Consume
// ����: ����ģ��
//*********************************************************************
bool CCommentParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//���ҽ�����ǩ
	unsigned int nBeginPos = nParseStartPos;
	std::string::size_type nFindPos = szTemplate.find("#}", nParseStartPos);
	if (nFindPos == std::string::npos)
	{
		STRING_STREAM(szResultError) << "ע���޽�����ǩ(" << nParseStartPos << ").";
		return false;
	}

	//�����ҽ���
	unsigned int nEndPos = nFindPos;
	if ((nFindPos > 0) && (szTemplate[nFindPos - 1] == '-'))
	{
		HandleRightCompact();
		--nEndPos;
	}

	//�����ı�
	if (nEndPos > nBeginPos)
		HandleCommentText(szTemplate.substr(nBeginPos, nEndPos - nBeginPos));

	//���ý��������
	nParseStartPos = nFindPos + 2;
	return true;
}

//**********************************************************************
// ����: HandleLeftCompact
// ����: ���������
//*********************************************************************
void CCommentParser::HandleLeftCompact(void) 
{
	VerifyNodeExists();
	m_spCommentNode->SetLeftCompact(true);
}

//**********************************************************************
// ����: HandleRightCompact
// ����: �����ҽ���
//*********************************************************************
void CCommentParser::HandleRightCompact(void) 
{
	VerifyNodeExists();
	m_spCommentNode->SetRightCompact(true);
}

//**********************************************************************
// ����: HandleCommentText
// ����: ����ע���ı�
//*********************************************************************
void CCommentParser::HandleCommentText(const std::string& szCommentText)
{
	VerifyNodeExists();
	m_spCommentNode->SetText(szCommentText);
}

//**********************************************************************
// ����: VerifyNodeExists
// ����: ȷ���ڵ����
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

///////////////////////////////////////////////// CVariableParser��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CVariableParser
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CVariableParser::CVariableParser(void) : base_type()
{
}

//**********************************************************************
// ����: CVariableParser
// ����: ���캯��
//*********************************************************************
CVariableParser::CVariableParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// ����: ~CVariableParser
// ����: ��������
//*********************************************************************
CVariableParser::~CVariableParser(void)
{
}

//**********************************************************************
// ����: Consume
// ����: ����ģ��
//*********************************************************************
bool CVariableParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//������һ������
	std::string szValue;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szValue, szResultError))
		return false;

	//�����һ������
	if (szValue.empty() || (!isalnum(szValue[0])))
	{
		STRING_STREAM(szResultError) << "����δ����(" << nParseStartPos << ").";
		return false;
	}

	//�������
	HandleValue(szValue);

	//���ҽ�����ǩ
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "}}", bCompact, szResultError))
		return false;
	m_spValueNode->SetRightCompact(bCompact);

	//����
	return true;
}

//**********************************************************************
// ����: HandleValue
// ����: ��������ı���
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

///////////////////////////////////////////////// CControlParser��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CControlParser
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CControlParser::CControlParser(void) : base_type()
{
}

//**********************************************************************
// ����: CControlParser
// ����: ���캯��
//*********************************************************************
CControlParser::CControlParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// ����: ~CControlParser
// ����: ��������
//*********************************************************************
CControlParser::~CControlParser(void)
{
}

//**********************************************************************
// ����: Consume
// ����: ����ģ��
//*********************************************************************
bool CControlParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//������һ������
	std::string szNextWord;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szNextWord, szResultError))
		return false;

	//������������������
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
		STRING_STREAM(szResultError) << "δ֪����(" << szNextWord << ':' << (nParseStartPos - szNextWord.size()) << ")!";
		return false;
	}
}

///////////////////////////////////////////////// CSetParser��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CSetParser
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CSetParser::CSetParser(void) : base_type()
{
}

//**********************************************************************
// ����: CSetParser
// ����: ���캯��
//*********************************************************************
CSetParser::CSetParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// ����: ~CSetParser
// ����: ��������
//*********************************************************************
CSetParser::~CSetParser(void)
{
}

//**********************************************************************
// ����: Consume
// ����: ����ģ��
//*********************************************************************
bool CSetParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//�����������
	std::string szLeftValue;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szLeftValue, szResultError))
		return false;

	//�������ڷ���
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "=", bCompact,szResultError))
		return false;

	//�����ұ�����
	std::string szRightValue;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szRightValue, szResultError))
		return false;

	//�������������ǩ
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;

	//������
	HandleSetValue(szLeftValue, szRightValue);

	//����
	return true;
}

//**********************************************************************
// ����: HandleSetValue
// ����: ��������ı�������
//*********************************************************************
void CSetParser::HandleSetValue(const std::string& szLeftValue, const std::string& szRightValue)
{
	m_spSetNode = std::make_shared<CSetNode>();
	m_spSetNode->SetValue(szLeftValue, szRightValue);
	parser_pointer_type spParent = GetParent().lock();
	if (spParent)
		spParent->GetNode()->AppendChild(m_spSetNode);
}

///////////////////////////////////////////////// CBlockParser��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CBlockParser
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CBlockParser::CBlockParser(void) : base_type()
{
}

//**********************************************************************
// ����: CBlockParser
// ����: ���캯��
//*********************************************************************
CBlockParser::CBlockParser(parser_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// ����: ~CBlockParser
// ����: ��������
//*********************************************************************
CBlockParser::~CBlockParser(void)
{
}

//**********************************************************************
// ����: SetNode
// ����: �󶨽ڵ�
//*********************************************************************
void CBlockParser::SetNode(node_pointer_type spNode)
{
	m_spNode = spNode;
}

//**********************************************************************
// ����: Consume
// ����: ����ģ��
//*********************************************************************
bool CBlockParser::Consume(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	return true;
}

///////////////////////////////////////////////// CIfParser��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CIfParser
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CIfParser::CIfParser(void) : base_type(), m_nParseState(prpParseIf), m_spNode(std::make_shared<CIfNode>())
{
}

//**********************************************************************
// ����: CIfParser
// ����: ���캯��
//*********************************************************************
CIfParser::CIfParser(parser_weak_pointer_type spParent) : base_type(spParent), m_nParseState(prpParseIf), m_spNode(std::make_shared<CIfNode>())
{
	GetParent().lock()->GetNode()->AppendChild(m_spNode);
}

//**********************************************************************
// ����: ~CIfParser
// ����: ��������
//*********************************************************************
CIfParser::~CIfParser(void)
{
}

//**********************************************************************
// ����: Consume
// ����: ����ģ��
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
	STRING_STREAM(szResultError) << "δ֪����״̬:" << m_nParseState;
	return false;
}

//**********************************************************************
// ����: ConsumeIf
// ����: ����if
//*********************************************************************
bool CIfParser::ConsumeIf(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//�������ʽֵ
	std::string szBoolExpress;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szBoolExpress, szResultError))
		return false;

	//�������ȡ��
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

	//����if�������
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;

	//����ֵ
	HandleBool(IsLeftCompact(), bNot, szBoolExpress, bCompact);
	m_nParseState = prpParseIfContent;
	return Consume(szTemplate, nParseStartPos, szResultError);
}

//**********************************************************************
// ����: ConsumeIfContent
// ����: ����if����
//*********************************************************************
bool CIfParser::ConsumeIfContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	while (nParseStartPos < szTemplate.size())
	{
		//����������
		if (IsStartTag(szTemplate, nParseStartPos))
		{
			//������ʼ��ǩ
			std::string szTag;
			bool bCompact = false;
			if (!ConsumeStartTag(szTemplate, nParseStartPos, szTag, bCompact, szResultError))
				return false;

			//�õ���һ������
			unsigned int nStartTagPos = nParseStartPos;
			std::string szControl;
			if (!ConsumeNextWord(szTemplate, nParseStartPos, szControl, szResultError))
				return false;
			nParseStartPos = nStartTagPos;

			//�����������
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
				//���ɲ�ִ���ӽ�����
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
			//���ɲ�ִ�����ı�������
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
// ����: ConsumeElse
// ����: ����else
//*********************************************************************
bool CIfParser::ConsumeElse(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//����else
	std::string szElse;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szElse, szResultError))
		return false;
	if (szElse != "else")
	{
		STRING_STREAM(szResultError) << "ָ��λ��(" << nParseStartPos << ")����\"else\",ʵ��Ϊ(\"" << szElse << "\").";
		return false;
	}

	//����else������ǩ
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;
	m_nParseState = prpParseElseContent;
	m_spNode->SetHasElse(true);
	m_spNode->SetElseRightCompact(bCompact);
	return Consume(szTemplate, nParseStartPos, szResultError);
}

//**********************************************************************
// ����: ConsumeElseContent
// ����: ����else����
//*********************************************************************
bool CIfParser::ConsumeElseContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	while (nParseStartPos < szTemplate.size())
	{
		//����������
		if (IsStartTag(szTemplate, nParseStartPos))
		{
			//������ʼ��ǩ
			std::string szTag;
			bool bCompact = false;
			if (!ConsumeStartTag(szTemplate, nParseStartPos, szTag, bCompact, szResultError))
				return false;

			//�õ���һ������
			unsigned int nStartTagPos = nParseStartPos;
			std::string szControl;
			if (!ConsumeNextWord(szTemplate, nParseStartPos, szControl, szResultError))
				return false;
			nParseStartPos = nStartTagPos;

			//�����������
			if (szControl == "endif")
			{
				m_nParseState = prpEndIf;
				m_spNode->SetEndifLeftCompact(bCompact);
				return Consume(szTemplate, nParseStartPos, szResultError);
			}
			else
			{
				//���ɲ�ִ���ӽ�����
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
			//���ɲ�ִ�����ı�������
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
// ����: ConsumeEndIf
// ����: ����endif
//*********************************************************************
bool CIfParser::ConsumeEndIf(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//����else
	std::string szEndIf;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szEndIf, szResultError))
		return false;
	if (szEndIf != "endif")
	{
		STRING_STREAM(szResultError) << "ָ��λ��(" << nParseStartPos << ")����\"endif\",ʵ��Ϊ(\"" << szEndIf << "\").";
		return false;
	}

	//����endif������ǩ
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;
	m_spNode->SetRightCompact(bCompact);
	return true;
}

//**********************************************************************
// ����: HandleBool
// ����: ������ʽ
//*********************************************************************
void CIfParser::HandleBool(bool bLeftCompact, bool bNot, const std::string& szBoolExpress, bool bRightCompact)
{
	m_spNode->SetLeftCompact(bLeftCompact);
	m_spNode->SetValue(bNot, szBoolExpress);
	m_spNode->SetIfRightCompact(bRightCompact);
}

///////////////////////////////////////////////// CForParser��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CForParser
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CForParser::CForParser(void) : base_type(), m_nParseState(prpParseFor), m_spNode(std::make_shared<CForNode>())
{
}

//**********************************************************************
// ����: CForParser
// ����: ���캯��
//*********************************************************************
CForParser::CForParser(parser_weak_pointer_type spParent) : base_type(spParent), m_nParseState(prpParseFor), m_spNode(std::make_shared<CForNode>())
{
	spParent.lock()->GetNode()->AppendChild(m_spNode);
}

//**********************************************************************
// ����: ~CForParser
// ����: ��������
//*********************************************************************
CForParser::~CForParser(void)
{
}

//**********************************************************************
// ����: Consume
// ����: ����ģ��
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
	STRING_STREAM(szResultError) << "δ֪����״̬:" << m_nParseState;
	return false;
}

//**********************************************************************
// ����: ConsumeFor
// ����: ����for
//*********************************************************************
bool CForParser::ConsumeFor(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//��������Ԫ��
	std::string szVariable;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szVariable, szResultError))
		return false;

	//����In
	std::string szNextWord;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szNextWord, szResultError))
		return false;
	if (szNextWord != "in")
	{
		STRING_STREAM(szResultError) << "����\"in\",ʵ��Ϊ\"" << szNextWord << "\":" << nParseStartPos;
		return false;
	}

	//��������
	std::string szContainer;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szContainer, szResultError))
		return false;
	if (!szContainer.empty() && !szVariable.empty())
		HandleForValues(szVariable, szContainer);

	//����������ǩ
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;
	m_spNode->SetForRightCompact(bCompact);

	//����
	m_nParseState = prpParseForContent;
	return Consume(szTemplate, nParseStartPos, szResultError);
}

//**********************************************************************
// ����: ConsumeForContent
// ����: ����for����
//*********************************************************************
bool CForParser::ConsumeForContent(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	while (nParseStartPos < szTemplate.size())
	{
		//����������
		if (IsStartTag(szTemplate, nParseStartPos))
		{
			//������ʼ��ǩ
			std::string szTag;
			bool bCompact = false;
			if (!ConsumeStartTag(szTemplate, nParseStartPos, szTag, bCompact, szResultError))
				return false;

			//�õ���һ������
			unsigned int nStartTagPos = nParseStartPos;
			std::string szControl;
			if (!ConsumeNextWord(szTemplate, nParseStartPos, szControl, szResultError))
				return false;
			nParseStartPos = nStartTagPos;

			//�����������
			if (szControl == "endfor")
			{
				m_nParseState = prpEndFor;
				m_spNode->SetEndForLeftCompact(bCompact);
				return Consume(szTemplate, nParseStartPos, szResultError);
			}
			else
			{
				//���ɲ�ִ���ӽ�����
				std::shared_ptr<IParser> spParser;
				if (!CreateParser(szTag, shared_from_this(), bCompact, spParser, szResultError))
					return false;
				if (!spParser || !spParser->Consume(szTemplate, nParseStartPos, szResultError))
					return false;
			}
		}
		else
		{
			//���ɲ�ִ�����ı�������
			std::shared_ptr<IParser> spParser;
			spParser = std::make_shared<CTextParser>(shared_from_this());
			if (!spParser || !spParser->Consume(szTemplate, nParseStartPos, szResultError))
				return false;
		}
	}
	return true;
}

//**********************************************************************
// ����: ConsumeEndFor
// ����: ����for����
//*********************************************************************
bool CForParser::ConsumeEndFor(const std::string& szTemplate, unsigned int& nParseStartPos, std::string& szResultError)
{
	//����endfor
	std::string szEndFor;
	if (!ConsumeNextWord(szTemplate, nParseStartPos, szEndFor, szResultError))
		return false;
	if (szEndFor != "endfor")
	{
		STRING_STREAM(szResultError) << "ָ��λ��(" << nParseStartPos << ")����\"endfor\",ʵ��Ϊ(\"" << szEndFor << "\").";
		return false;
	}

	//����endfor������ǩ
	bool bCompact = false;
	if (!ConsumeNextEndTag(szTemplate, nParseStartPos, "%}", bCompact, szResultError))
		return false;
	m_spNode->SetRightCompact(bCompact);
	return true;
}

//**********************************************************************
// ����: HandleForValues
// ����: ����forԪ��
//*********************************************************************
void CForParser::HandleForValues(const std::string& szVariable, const std::string& szContainer)
{
	m_spNode->SetValue(szVariable, szContainer);
	m_spNode->SetLeftCompact(IsLeftCompact());
}

///////////////////////////////////////////////// �ڵ���ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: Render
// ����: ��Ⱦ���
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
// ����: Render
// ����: ��Ⱦ���
//*********************************************************************
bool CCommentNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//���������
	if (IsLeftCompact())
		TrimRight(szResultContent);
	return true;
}

//**********************************************************************
// ����: Render
// ����: ��Ⱦ���
//*********************************************************************
bool CVariableNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//������ֵ
	any_type aResultValue;
	if (!EvalVariable(m_szValue, aRenderContent, aResultValue, szResultError))
		return false;

	//���ûص�����
	std::string szValue;
	if (!pValueCB(m_szValue, aResultValue, szValue, szResultError))
		return false;

	//���ý��
	if (!szValue.empty())
		szResultContent += szValue;
	return true;
}

///////////////////////////////////////////////// CIfNode��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CIfNode
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CIfNode::CIfNode(void) : base_type(), m_spIfContentNode(std::make_shared<node_type>()), m_spElseContentNode(std::make_shared<node_type>()), m_bNot(false),
m_bIfRightCompact(false), m_bElseLeftCompact(false), m_bElseRightCompact(false), m_bEndifLeftCompact(false), m_bHasElse(false)
{
}

//**********************************************************************
// ����: CIfNode
// ����: ���캯��
//*********************************************************************
CIfNode::CIfNode(node_weak_pointer_type spParent) : base_type(spParent), m_spIfContentNode(std::make_shared<node_type>()), m_spElseContentNode(std::make_shared<node_type>()), m_bNot(false),
m_bIfRightCompact(false), m_bElseLeftCompact(false), m_bElseRightCompact(false), m_bEndifLeftCompact(false), m_bHasElse(false)
{
}

//**********************************************************************
// ����: ~CIfNode
// ����: ������������
//*********************************************************************
CIfNode::~CIfNode(void)
{
}

//**********************************************************************
// ����: SetValue
// ����: ����ֵ
//*********************************************************************
void CIfNode::SetValue(bool bNot, const std::string& szExpress)
{
	m_bNot = bNot;
	m_szExpress = szExpress;
}

//**********************************************************************
// ����: Render
// ����: ��Ⱦ���
//*********************************************************************
bool CIfNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB) 
{
	//����if������
	if (IsLeftCompact())
		TrimRight(szResultContent);

	//bool���ʽ��ֵ
	any_type aResultValue;
	if (!EvalVariable(m_szExpress, aRenderContent, aResultValue, szResultError))
		return false;
	bool bBoolValue = to_bool(aResultValue);
	if (m_bNot)
		bBoolValue = !bBoolValue;

	//��֧��Ⱦ
	if (bBoolValue)
	{
		//��Ⱦ��֧
		std::string szTempResult;
		if (!m_spIfContentNode->Render(aRenderContent, szTempResult, szResultError, pValueCB))
			return false;

		//�������
		if (IsIfRightCompact())
			TrimLeft(szTempResult);
		if ((HasElse() && IsElseLeftCompact()) || IsEndifLeftCompact())
			TrimRight(szTempResult);

		//���ý��
		if (!szTempResult.empty())
			szResultContent += szTempResult;
		return true;
	}
	else
	{
		//����Ƿ���else��֧
		if (!HasElse())
			return true;

		//��Ⱦ��֧
		std::string szTempResult;
		if (!m_spElseContentNode->Render(aRenderContent, szTempResult, szResultError, pValueCB))
			return false;

		//�������
		if (IsElseRightCompact())
			TrimLeft(szTempResult);
		if (IsEndifLeftCompact())
			TrimRight(szTempResult);

		//���ý��
		if (!szTempResult.empty())
			szResultContent += szTempResult;
		return true;
	}
}

///////////////////////////////////////////////// CForNode��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CForNode
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CForNode::CForNode(void) : base_type(), m_bForRightCompact(false), m_bEndForLeftCompact(false)
{
}

//**********************************************************************
// ����: CForNode
// ����: ���캯��
//*********************************************************************
CForNode::CForNode(node_weak_pointer_type spParent) : base_type(spParent), m_bForRightCompact(false), m_bEndForLeftCompact(false)
{
}

//**********************************************************************
// ����: ~CForNode
// ����: ������������
//*********************************************************************
CForNode::~CForNode(void)
{
}

//**********************************************************************
// ����: SetValue
// ����: ����ֵ
//*********************************************************************
void CForNode::SetValue(const std::string& szVariable, const std::string& szContainer)
{
	m_szVariable = szVariable;
	m_szContainer = szContainer;
}

//**********************************************************************
// ����: Render
// ����: ��Ⱦ���
//*********************************************************************
bool CForNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//����FOR�����
	if (IsLeftCompact())
		TrimRight(szResultContent);

	//������ֵ
	any_type aValue;
	if(!EvalVariable(m_szContainer, aRenderContent, aValue, szResultError))
		return false;

	//���ֵ�Ƿ��ڳ����ֵ���
	if (aValue.empty())
	{
		STRING_STREAM(szResultError) << "����(" << m_szContainer << ")������!";
		return false;
	}

	//��������
	auto visit_cb = [this, &aRenderContent, &szResultContent, &szResultError, pValueCB](const any_type& any_value, unsigned int nIndex, unsigned int cbCount)
	{
		return !RenderOnce(m_szVariable, any_value, nIndex, cbCount, aRenderContent, szResultContent, szResultError, pValueCB);
	};
	if (visit_container(aValue, visit_cb))
		return false;
	/*
	//�õ�����ֵ
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
// ����: RenderOnce
// ����: ��Ⱦһ��
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
// ����: RenderOnceImpl
// ����: ��Ⱦһ��ʵ��
//*********************************************************************
bool CForNode::RenderOnceImpl(const std::string& szVariableName, any_type aVariableValue, unsigned int nLoopIndex, unsigned int nLoopCount, dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//����ѭ���ֵ�
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

	//���ñ���ֵ
	aRenderContent[szVariableName] = aVariableValue;

	//��Ⱦ����
	for (unsigned int nIndex = 0, cbCount = GetChildrenCount(); nIndex < cbCount; ++nIndex)
	{
		auto spChild = GetChild(nIndex);
		if (!spChild->Render(aRenderContent, szResultContent, szResultError, pValueCB))
			return false;
	}
	return true;
}

///////////////////////////////////////////////// CSetNode��ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: CSetNode
// ����: Ĭ�Ϲ��캯��
//*********************************************************************
CSetNode::CSetNode(void) : base_type()
{
}

//**********************************************************************
// ����: CSetNode
// ����: ���캯��
//*********************************************************************
CSetNode::CSetNode(node_weak_pointer_type spParent) : base_type(spParent)
{
}

//**********************************************************************
// ����: ~CSetNode
// ����: ������������
//*********************************************************************
CSetNode::~CSetNode(void)
{
}

//**********************************************************************
// ����: SetValue
// ����: ����ֵ
//*********************************************************************
void CSetNode::SetValue(const std::string& szLeftVariable, const std::string& szRightVariable)
{
	m_szLeftVariable = szLeftVariable;
	m_szRightVariable = szRightVariable;
}

//**********************************************************************
// ����: Render
// ����: ��Ⱦ���
//*********************************************************************
bool CSetNode::Render(const dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//���������
	if (IsLeftCompact())
		TrimRight(szResultContent);

	//���ñ���
	any_type aResultValue;
	if (!EvalVariable(m_szRightVariable, aRenderContent, aResultValue, szResultError))
		return false;

	//���ñ���
	const_cast<dict_type&>(aRenderContent)[m_szLeftVariable] = aResultValue;
	return true;
}

///////////////////////////////////////////////// �ӿں���ʵ�� /////////////////////////////////////////////////

//**********************************************************************
// ����: renderImpl
// ����: ��Ⱦģ��ʵ��
//*********************************************************************
bool renderImpl(const std::string& szTemplate, dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError, value_cb_type pValueCB)
{
	//����ģ��
	std::shared_ptr<CTemplate> spTemplate = std::make_shared<CTemplate>();
	unsigned int nParseStartPos = 0;
	bool bSuccess = spTemplate->Consume(szTemplate, nParseStartPos, szResultError);
	if (!bSuccess)
		return bSuccess;

	//��Ⱦ�ı�
	return spTemplate->Render(aRenderContent, szResultContent, szResultError, pValueCB);
}

//**********************************************************************
// ����: default_value_cb
// ����: ȱʡ�ű�������Ⱦ��ֵ�ص�����
//*********************************************************************
bool default_value_cb(const std::string& szName, const any_type& aValue, std::string& szResultContent, std::string& szResultError)
{
	szResultContent = to_string(aValue);
	return true;
}

//**********************************************************************
// ����: sql_value_cb
// ����: SQLģ����Ⱦ�ű�������Ⱦ��ֵ�ص�����(���ű�����λ����"?"��ֵ�������������浽�����������)
//*********************************************************************
bool sql_value_cb(const std::string& szName, const any_type& aValue, const std::string& szPlaceHolder, std::string& szResultContent, many_type& aResultParameters, std::string& szResultError)
{
	szResultContent = szPlaceHolder;
	aResultParameters.push_back(aValue);
	return true;
}

//**********************************************************************
// ����: render
// ����: ��Ⱦģ��
//*********************************************************************
bool render(const std::string& szTemplate, dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError)
{
	return renderImpl(szTemplate, aRenderContent, szResultContent, szResultError, default_value_cb);
}

//**********************************************************************
// ����: render_sql
// ����: ��ȾSQLģ��
//*********************************************************************
bool render_sql(const std::string& szTemplate, dict_type& aRenderContent, const std::string& szPlaceHolder, std::string& szResultSQL, many_type& aResultSQLParameters, std::string& szResultError)
{
	return renderImpl(szTemplate, aRenderContent, szResultSQL, szResultError, std::bind(sql_value_cb, std::placeholders::_1, std::placeholders::_2, szPlaceHolder, std::placeholders::_3, std::ref(aResultSQLParameters), std::placeholders::_4));
}

}//jinjia2���ֿռ����