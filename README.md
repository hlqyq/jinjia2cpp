# jinjia2cpp
Lightweight implements of jinja2 for c++  

templates follow Jinja2 syntax
supports:
   1. variable substitution
   2. for loops
   3. if statements
   4. object subscript and attribute support

# dependance
   
   1. boost 1.63 or later(boost.any)

# Example

   * variable substitution:   {{ somevar }} will be replace by the value of somevar::

```c++
     	
    std::string szVariable;
    bool bSuccess;
    std::string szResultError;
    many_type record_0 = { 1, 2, 3 };
    many_type record_1 = { 10, 20, 30 };
    many_type record_2 = { 100, 200, 300 };
    many_type name_list = { "zs", "ls", "ww" };
    dict_type record_dict = { { "names", name_list }, {"name","zl"} };
    many_type record_list = { record_0, record_1, record_2, record_dict };
    many_type field_names = { "id", "age", "name","note" };
    dict_type aRenderDict = { { "record_list", record_list }, 
                              { "field_names", field_names },
                              { "id", "uuid" },
                              { "record_dict", record_dict } };
       
 
    //bool
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

    //text
    szVariable = "\'abc\'";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_string(aResultValue), "abc");
    szVariable = "\"abc\"";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_string(aResultValue), "abc");

    //simple
    szVariable = "id";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("uuid"));

    //object call
    szVariable = "field_names.index(\'age\')";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_int(aResultValue),1);
    szVariable = "field_names.length()";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_int(aResultValue), 4);

    //subscript
    szVariable = "field_names[2]";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("name"));

    //text subscript
    szVariable = "record_dict[\"name\"]";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("zl"));

    //object attribute
    szVariable = "record_dict.name";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("zl"));

    //object inner attribute
    szVariable = "record_dict.length";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_int(aResultValue), 2);

    //composite
    szVariable = "record_list[field_names.index(\'note\')].names[2]";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("ww"));'''
```

   * if statment
```
	bool bSuccess;
	std::string szTemplate;
	std::string szResultContent, szResultError;
	many_type record_list = { 1, 2, 3 };
	dict_type aDict = { { "name", "张三" }, { "age", 18 },
                        { "record_list", record_list } };
	dict_type aRenderDict = { { "record_list", record_list },
                              { "name", "张三" },
                              { "age", 18 },
                              { "dict_instance", aDict }, 
                              { "true_value", true },
                              { "false_value", false }, };

	//true
	szResultContent.clear();
	szTemplate = "{% if true_value %}yes{%else%}no{%endif%}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "yes");

	//false
	szResultContent.clear();
	szTemplate = "{% if false_value %}yes{%else%}no{%endif%}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "no");

	//compact
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

	//nested 
	szResultContent.clear();
	szTemplate = "{% if false_value %}yes{% else %}{% if false_value %}no no{% else -%}  no yes{% endif %}{% endif %}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "no yes");

	//composite
	szResultContent.clear();
	szTemplate = "{% if not dict_instance.record_list[1] %}yes{% else -%}  no{% endif %}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "no");
```   
   * For statement

```
	std::string szTemplate, szResultContent, szResultError;
	bool bSuccess = false;
	many_type record0 = { 1, 2, 3 };
	many_type record1 = { 10, 20, 30 };
	many_type record2 = { 100, 200, 300 };
	many_type record_list = { record0, record1, record2 };
	dict_type aDict = { { "name", "张三" }, { "age", 18 }, { "record_list", record_list } };
	dict_type aRenderDict = { { "record_list", record_list },
                              { "name", "张三" }, 
                              { "age", 18 },
                              { "dict_instance", aDict } };

	//simple
	szTemplate = " {%- for record in dict_instance.record_list -%} a {%- endfor -%} zzz";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "aaa zzz");

	//nested
	szResultContent.clear();
	szTemplate = " {%- for record in dict_instance.record_list -%}{% for x in record %}{{loop.index}}{%endfor%}{%- endfor -%}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, "123123123");

	//set
	szResultContent.clear();
	szTemplate = " {%- for record in dict_instance.record_list %}{% set loop1 = loop %} {% for x in record %}{{loop1.index0}}-{{loop.index0}} {%endfor%}{%- endfor %}";
	bSuccess = render(szTemplate, aRenderDict, szResultContent, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultContent, " 0-0 0-1 0-2 1-0 1-1 1-2 2-0 2-1 2-2");     
```
   * sql
  ```
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
	dict_type aRenderDict = { { "record_list", record_list },
                              { "field_names", field_names }, };
	szTemplate = "SELECT * FROM tbl_person 
                  WHERE {% for record in record_list -%}
                           {%- if loop.index0 %} OR {% endif %}
                           (name={{record[field_names.index(\'name\')]}} AND 
                            age={{record[field_names.index(\'age\')]}})
                        {%- endfor %}";
	bSuccess = render_sql(szTemplate, aRenderDict, "?", szResultSQL, aResultParameters, szResultError);
	BOOST_CHECK_MESSAGE(bSuccess, szResultError);
	BOOST_CHECK_EQUAL(szResultSQL, "SELECT * FROM tbl_person WHERE (name=? AND age=?) OR (name=? AND age=?) OR (name=? AND age=?)");
	BOOST_CHECK_EQUAL(aResultParameters.size(), 6);
	BOOST_CHECK_EQUAL(get_string(aResultParameters[2]), "10");
	BOOST_CHECK_EQUAL(get_string(aResultParameters[5]), "200");
  ``` 

 # License
 Mozilla Public License  