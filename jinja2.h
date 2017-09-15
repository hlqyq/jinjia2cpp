//**********************************************************************
// Copyright (c) 2017
// ��˹���������С��.
// �ļ�: Jinja2.h
// ����: ��ģ���ṩһ���򻯰��Jinja2ģ������ʵ��
// ��ʷ:
//	���		�޸�ʱ��		�޸���		�޸�����
//	1			2017-09-08		������		�״�����
//*********************************************************************

//������ͷ�ļ���    
#ifndef _JINJA2_H
#define _JINJA2_H

//����ͷ�ļ�
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <boost/any.hpp>

//jinjia2���ֿռ俪ʼ
namespace jinja2
{

///////////////////////////////////////////////// �������� /////////////////////////////////////////////////

typedef boost::any									any_type;
typedef std::vector<any_type>						many_type;
typedef std::map<std::string, any_type>				dict_type;
typedef std::vector<std::string>					string_record_type;
typedef std::vector<string_record_type>				record_list_type;

///////////////////////////////////////////////// �ӿں��� /////////////////////////////////////////////////

//��Ⱦģ��
bool render(const std::string& szTemplate, dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError);

//��ȾSQLģ��
bool render_sql(const std::string& szTemplate, dict_type& aRenderContent, const std::string& szPlaceHolder, std::string& szResultSQL, many_type& aResultSQLParameters, std::string& szResultError);

///////////////////////////////////////////////// any���Ͳ�ѯ /////////////////////////////////////////////////

bool is_empty(const any_type& any_value);
bool is_bool(const any_type& any_value);
bool is_char(const any_type& any_value);
bool is_unsigned_char(const any_type& any_value);
bool is_short(const any_type& any_value);
bool is_unsigned_short(const any_type& any_value);
bool is_int(const any_type& any_value);
bool is_unsigned_int(const any_type& any_value);
bool is_float(const any_type& any_value);
bool is_double(const any_type& any_value);
bool is_char_ptr(const any_type& any_value);
bool is_string(const any_type& any_value);
bool is_dict(const any_type& any_value);
bool is_many(const any_type& any_value);
bool is_string_record(const any_type& any_value);
bool is_record_list(const any_type& any_value);
bool is_char_series(const any_type& any_value);		//�Ƿ�Ϊ�ַ�ϵ��(�з���/�޷����ַ�)
bool is_int_series(const any_type& any_value);		//�Ƿ�Ϊ����ϵ��(�з���/�޷��Ŷ���������������������)
bool is_float_series(const any_type& any_value);	//�Ƿ�Ϊ������ϵ��(����˫����)
bool is_string_series(const any_type& any_value);	//�Ƿ�Ϊ�ַ���ϵ��(const char*��std::string)
bool is_container(const any_type& any_value);		//�Ƿ�Ϊ����(�б���ֵ�)

///////////////////////////////////////////////// anyֵ��ѯ /////////////////////////////////////////////////

bool get_bool(const any_type& any_value);
char get_char(const any_type& any_value);
int get_int(const any_type& any_value);
double get_float(const any_type& any_value);
std::string get_string(const any_type& any_value);
const many_type* get_many(const any_type& any_value);
const dict_type* get_dict(const any_type& any_value);
const string_record_type* get_string_record(const any_type& any_value);
const record_list_type* get_record_list(const any_type& any_value);
bool visit_descendant(const any_type& any_value, std::function<bool(const any_type&)> visit_cb, bool bIncludeSelf = true, bool bTopDown = true); //��������(�������е�ÿһ��Ԫ�ؽ��лص�)
bool visit_container(const any_type& any_value, std::function<bool(const any_type&,unsigned int,unsigned int)> visit_cb); //��������(�������е�ÿһ��Ԫ�ؽ��лص�)

///////////////////////////////////////////////// anyֵת�� /////////////////////////////////////////////////

std::string to_string(const any_type& any_value);
bool to_bool(const any_type& any_value);

}//jinjia2���ֿռ����
#endif //����δ����_JINJA2_H��
