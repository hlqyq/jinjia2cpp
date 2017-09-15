//**********************************************************************
// Copyright (c) 2017
// 迪斯特软件开发小组.
// 文件: Jinja2_unittest.cpp
// 内容: 简化版Jinja2模板引擎的单元测试
// 历史:
//	序号		修改时间		修改人		修改内容
//	1			2017-09-08		胡乐秋		首次生成
//*********************************************************************

//包含头文件
#define BOOST_TEST_MODULE jinja2
#include <boost/test/included/unit_test.hpp>
#include "jinja2.h"

//jinjia2名字空间开始
namespace jinja2
{

//外部函数声明
bool EvalVariable(const std::string& szVariable, const dict_type& aRenderContent, any_type& aResultValue, std::string& szResultError);

BOOST_AUTO_TEST_CASE(render_sql_native_Test)
{
	//声明变量
	std::string szTemplate, szResultContent, szResultError, szResultSQL;
	bool bSuccess = false;
	many_type aResultParameters;

	//测试原始数据
	string_record_type field_names = { "name", "age" };
	string_record_type record_0 = { "1", "2" };
	string_record_type record_1 = { "10", "20" };
	string_record_type record_2 = { "100", "200" };
	record_list_type record_list = { record_0, record_1, record_2 };
	dict_type aRenderDict = { { "record_list", record_list }, { "field_names", field_names }, };
	szTemplate = "SELECT * FROM tbl_person WHERE {% for record in record_list %}{% if loop.index0 %} OR {% endif %}(name={{record[field_names.index(\'name\')]}} AND age={{record[field_names.index(\'age\')]}}){% endfor %}";
	bSuccess = render_sql(szTemplate, aRenderDict, "?", szResultSQL, aResultParameters, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultSQL, "SELECT * FROM tbl_person WHERE (name=? AND age=?) OR (name=? AND age=?) OR (name=? AND age=?)");
	BOOST_CHECK_EQUAL(aResultParameters.size(), 6);
	BOOST_CHECK_EQUAL(get_string(aResultParameters[2]), "10");
	BOOST_CHECK_EQUAL(get_string(aResultParameters[5]), "200");
}

BOOST_AUTO_TEST_CASE(render_sql_Test)
{
	//声明变量
	std::string szTemplate, szResultContent, szResultError, szResultSQL;
	bool bSuccess = false;
	many_type aResultParameters;
	many_type record0 = { 1, 2, 3 };
	many_type record1 = { 10, 20, 30 };
	many_type record2 = { 100, 200, 300 };
	many_type record_list = { record0, record1, record2 };
	many_type field_names = { "first","second","third" };
	dict_type aRenderDict = { { "record_list", record_list }, { "field_names", field_names },};

	//基本测试
	szResultContent.clear();
	szTemplate = "SELECT * FROM tbl_person WHERE {% for record in record_list %}{% if loop.index0 %} OR {% endif %}(name={{record[field_names.index(\'first\')]}} AND age={{record[field_names.index(\'second\')]}}){% endfor %}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "SELECT * FROM tbl_person WHERE (name=1 AND age=2) OR (name=10 AND age=20) OR (name=100 AND age=200)");

	//SQL参数测试
	szResultContent.clear();
	szTemplate = "SELECT * FROM tbl_person WHERE {% for record in record_list %}{% if loop.index0 %} OR {% endif %}(name={{record[field_names.index(\'first\')]}} AND age={{record[field_names.index(\'second\')]}}){% endfor %}";
	bSuccess = render_sql(szTemplate, aRenderDict, "?", szResultSQL, aResultParameters, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultSQL, "SELECT * FROM tbl_person WHERE (name=? AND age=?) OR (name=? AND age=?) OR (name=? AND age=?)");
	BOOST_CHECK_EQUAL(aResultParameters.size(), 6);
	BOOST_CHECK_EQUAL(get_int(aResultParameters[2]), 10);
	BOOST_CHECK_EQUAL(get_int(aResultParameters[5]), 200);
}

BOOST_AUTO_TEST_CASE(render_FOR_Test)
{
	//声明变量
	std::string szTemplate, szResultContent, szResultError;
	bool bSuccess = false;
	many_type record0 = { 1, 2, 3 };
	many_type record1 = { 10, 20, 30 };
	many_type record2 = { 100, 200, 300 };
	many_type record_list = { record0, record1, record2 };
	dict_type aDict = { { "name", "张三" }, { "age", 18 }, { "record_list", record_list } };
	dict_type aRenderDict = { { "record_list", record_list }, { "name", "张三" }, { "age", 18 }, { "dict_instance", aDict } };

	//基本测试
	szTemplate = " {%- for record in dict_instance.record_list -%} a {%- endfor -%} zzz";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "aaa zzz");

	//嵌套测试
	szResultContent.clear();
	szTemplate = " {%- for record in dict_instance.record_list -%}{% for x in record %}{{loop.index}}{%endfor%}{%- endfor -%}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "123123123");

	//嵌套测试带外围访问
	szResultContent.clear();
	szTemplate = " {%- for record in dict_instance.record_list %}{% set loop1 = loop %} {% for x in record %}{{loop1.index0}}-{{loop.index0}} {%endfor%}{%- endfor %}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, " 0-0 0-1 0-2 1-0 1-1 1-2 2-0 2-1 2-2");
}

BOOST_AUTO_TEST_CASE(render_Value_Test)
{
	//声明变量
	std::string szTemplate, szResult, szResultError;
	bool bSuccess = false;
	many_type record_list = { 1, 2, 3 };
	dict_type aDict = { { "name", "张三" }, { "age", 18 }, { "record_list", record_list } };
	dict_type aRenderDict = { { "record_list", record_list }, { "name", "张三" }, { "age", 18 }, { "dict_instance", aDict } };

	//简单值替换
	szResult.clear();
	szResultError.clear();
	szTemplate = "My name is {{ name }} and age is {{age }} and sex is {{ sex }}.";
	bSuccess = render(szTemplate, aRenderDict, szResult, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResult, "My name is 张三 and age is 18 and sex is .");

	//列表下标
	szResult.clear();
	szTemplate = "record[2]: {{record_list[2]}} == 3.";
	bSuccess = render(szTemplate, aRenderDict, szResult, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResult, "record[2]: 3 == 3.");

	//对象属性值替换
	szResult.clear();
	szTemplate = "{{dict_instance.name}}";
	bSuccess = render(szTemplate, aRenderDict, szResult, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResult, "张三");

	//综合值替换
	szResult.clear();
	szTemplate = "{{dict_instance.record_list[1]}}";
	bSuccess = render(szTemplate, aRenderDict, szResult, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResult, "2");
}

BOOST_AUTO_TEST_CASE(render_TEXT_Test)
{
	//纯文本
	std::string szTemplate = "SELECT * FROM tbl_person";
	dict_type aRenderDict = {};
	std::string szResultContent, szResultError;
	bool bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, szTemplate);

	//带注释文本
	szResultContent.clear();
	szTemplate = "SELECT * FROM {# aaa #} tbl_person";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "SELECT * FROM  tbl_person");

	//带注释文本(左紧凑)
	szResultContent.clear();
	szTemplate = "SELECT * FROM  {#- aaa #} tbl_person";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "SELECT * FROM tbl_person");

	//带注释文本(右紧凑)
	szResultContent.clear();
	szTemplate = "SELECT * FROM  {# aaa -#} tbl_person";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "SELECT * FROM  tbl_person");

	//带注释文本(两侧紧凑)
	szResultContent.clear();
	szTemplate = "SELECT * FROM  {#- aaa -#} tbl_person";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "SELECT * FROMtbl_person");
}

BOOST_AUTO_TEST_CASE(render_Comment_Test)
{
	//声明变量
	std::string szTemplate, szResult, szResultError;
	bool bSuccess = false;
	std::vector<int> record_list = { 1, 2, 3 };
	dict_type aRenderDict = { { "record_list", record_list }, };

	//全注释
	szResult.clear();
	szResultError.clear();
	szTemplate = "{#  aaa {# {% #}";
	bSuccess = render(szTemplate, aRenderDict, szResult, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResult, "");

	//注释在开头
	szResult.clear();
	szResultError.clear();
	szTemplate = "{#  aaa {# {% #}def";
	bSuccess = render(szTemplate, aRenderDict, szResult, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResult, "def");

	//注释在结尾
	szResult.clear();
	szResultError.clear();
	szTemplate = "abc{#  aaa {# {% #}";
	bSuccess = render(szTemplate, aRenderDict, szResult, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResult, "abc");

	//注释在中间
	szResult.clear();
	szResultError.clear();
	szTemplate = "abc{#  aaa {# {% #}def";
	bSuccess = render(szTemplate, aRenderDict, szResult, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResult, "abcdef");

	//注释不封闭
	szResult.clear();
	szResultError.clear();
	szTemplate = ("abc{# aaa {# {% # }");
	bSuccess = render(szTemplate, aRenderDict, szResult, szResultError);
	BOOST_CHECK_MESSAGE(!bSuccess, szResultError);
	BOOST_CHECK_MESSAGE(szResultError.find("注释无结束标签") != std::string::npos, "");

	//类似注释但非注释
	szResult.clear();
	szResultError.clear();
	szTemplate = "abc{ #  aaa  # }{# {% #}def";
	bSuccess = render(szTemplate, aRenderDict, szResult, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResult, "abc{ #  aaa  # }def");
}

BOOST_AUTO_TEST_CASE(render_IF_Test)
{
	//声明变量
	bool bSuccess;
	std::string szTemplate;
	std::string szResultContent, szResultError;
	many_type record_list = { 1, 2, 3 };
	dict_type aDict = { { "name", "张三" }, { "age", 18 }, { "record_list", record_list } };
	dict_type aRenderDict = { { "record_list", record_list }, { "name", "张三" }, { "age", 18 }, { "dict_instance", aDict }, { "true_value", true }, { "false_value", false }, };

	//真测试
	szResultContent.clear();
	szTemplate = "{% if true_value %}yes{%else%}no{%endif%}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "yes");

	//假测试
	szResultContent.clear();
	szTemplate = "{% if false_value %}yes{%else%}no{%endif%}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "no");

	//缩进测试
	szResultContent.clear();
	szTemplate = " \r\n\t {%- if true_value -%}\r\n\t yes \r\n\t {%- else -%}\r\n no \r\n{%- endif -%}\r\n";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "yes");
	szResultContent.clear();
	szTemplate = " \r\n\t {%- if false_value -%}\r\n\t yes \r\n\t {%- else -%}\r\n no \r\n{%- endif -%}\r\n";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "no");

	//嵌套测试
	szResultContent.clear();
	szTemplate = "{% if false_value %}yes{% else %}{% if false_value %}no no{% else -%}  no yes{% endif %}{% endif %}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "no yes");

	//复杂值测试
	szResultContent.clear();
	szTemplate = "{% if not dict_instance.record_list[1] %}yes{% else -%}  no{% endif %}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "no");
}

BOOST_AUTO_TEST_CASE(EvalVariable_Test)
{
	//声明变量
	std::string szVariable;
	bool bSuccess;
	std::string szResultError;
	any_type aResultValue;
	many_type record_0 = { 1, 2, 3 };
	many_type record_1 = { 10, 20, 30 };
	many_type record_2 = { 100, 200, 300 };
	many_type name_list = { "zs", "ls", "ww" };
	dict_type record_dict = { { "names", name_list }, {"name","zl"} };
	many_type record_list = { record_0, record_1, record_2, record_dict };
	many_type field_names = { "id", "age", "name","note" };
	dict_type aRenderDict = { { "record_list", record_list }, { "field_names", field_names }, { "id", "uuid" }, { "record_dict", record_dict } };

	//bool变量求值
	szVariable = "True";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_bool(aResultValue), true);
	szVariable = "true";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_bool(aResultValue), true);
	szVariable = "False";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_bool(aResultValue), false);
	szVariable = "false";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_bool(aResultValue), false);

	//文本常量
	szVariable = "\'abc\'";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_string(aResultValue), "abc");
	szVariable = "\"abc\"";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_string(aResultValue), "abc");

	//简单变量
	szVariable = "id";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("uuid"));

	//对象方法调用
	szVariable = "field_names.index(\'age\')";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_int(aResultValue),1);
	szVariable = "field_names.length()";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_int(aResultValue), 4);

	//数字下标
	szVariable = "field_names[2]";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("name"));

	//文字下标
	szVariable = "record_dict[\"name\"]";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("zl"));

	//对象属性
	szVariable = "record_dict.name";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("zl"));

	//对象内置属性
	szVariable = "record_dict.length";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_int(aResultValue), 2);

	//综合属性
	szVariable = "record_list[field_names.index(\'note\')].names[2]";
	bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("ww"));
}

}//jinjia2名字空间结束
