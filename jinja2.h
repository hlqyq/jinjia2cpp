//**********************************************************************
// Copyright (c) 2017
// 迪斯特软件开发小组.
// 文件: Jinja2.h
// 内容: 本模型提供一个简化版的Jinja2模板引擎实现
// 历史:
//	序号		修改时间		修改人		修改内容
//	1			2017-09-08		胡乐秋		首次生成
//*********************************************************************

//声明本头文件宏    
#ifndef _JINJA2_H
#define _JINJA2_H

//包含头文件
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <boost/any.hpp>

//jinjia2名字空间开始
namespace jinja2
{

///////////////////////////////////////////////// 类型声明 /////////////////////////////////////////////////

typedef boost::any									any_type;
typedef std::vector<any_type>						many_type;
typedef std::map<std::string, any_type>				dict_type;
typedef std::vector<std::string>					string_record_type;
typedef std::vector<string_record_type>				record_list_type;

///////////////////////////////////////////////// 接口函数 /////////////////////////////////////////////////

//渲染模板
bool render(const std::string& szTemplate, dict_type& aRenderContent, std::string& szResultContent, std::string& szResultError);

//渲染SQL模板
bool render_sql(const std::string& szTemplate, dict_type& aRenderContent, const std::string& szPlaceHolder, std::string& szResultSQL, many_type& aResultSQLParameters, std::string& szResultError);

///////////////////////////////////////////////// any类型查询 /////////////////////////////////////////////////

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
bool is_char_series(const any_type& any_value);		//是否为字符系列(有符号/无符号字符)
bool is_int_series(const any_type& any_value);		//是否为整数系列(有符号/无符号短整数、整数、长整数等)
bool is_float_series(const any_type& any_value);	//是否为浮点数系列(单、双精度)
bool is_string_series(const any_type& any_value);	//是否为字符串系列(const char*或std::string)
bool is_container(const any_type& any_value);		//是否为容器(列表或字典)

///////////////////////////////////////////////// any值查询 /////////////////////////////////////////////////

bool get_bool(const any_type& any_value);
char get_char(const any_type& any_value);
int get_int(const any_type& any_value);
double get_float(const any_type& any_value);
std::string get_string(const any_type& any_value);
const many_type* get_many(const any_type& any_value);
const dict_type* get_dict(const any_type& any_value);
const string_record_type* get_string_record(const any_type& any_value);
const record_list_type* get_record_list(const any_type& any_value);
bool visit_descendant(const any_type& any_value, std::function<bool(const any_type&)> visit_cb, bool bIncludeSelf = true, bool bTopDown = true); //访问容器(对容器中的每一个元素进行回调)
bool visit_container(const any_type& any_value, std::function<bool(const any_type&,unsigned int,unsigned int)> visit_cb); //访问容器(对容器中的每一个元素进行回调)

///////////////////////////////////////////////// any值转换 /////////////////////////////////////////////////

std::string to_string(const any_type& any_value);
bool to_bool(const any_type& any_value);

}//jinjia2名字空间结束
#endif //假如未定义_JINJA2_H宏
