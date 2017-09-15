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

    //composit
    szVariable = "record_list[field_names.index(\'note\')].names[2]";
    bSuccess = EvalVariable(szVariable, aRenderDict, aResultValue, szResultError);
    BOOST_CHECK_MESSAGE(bSuccess, szResultError);
    BOOST_CHECK_EQUAL(get_string(aResultValue), std::string("ww"));'''

 # License
 Mozilla Public License  