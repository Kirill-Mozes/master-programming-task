/* Json object.
 * @file
 * @date 2018-08-09
 * @author Anonymous
 */

#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <map>
#include <string>
#include <vector>

#include <boost/spirit/home/x3.hpp>
//? Why I need ``std_pair`` header? Do I need one for std::map?
#include <boost/fusion/adapted/std_pair.hpp>

#include "parser.hpp"
#include "quoted_string.hpp"
#include "variant_decorator.hpp"

/* Json EBNF specification (https://www.json.org)
 *      string := \" ([^\"] | \\\")* \"
 *      number := [0-9]+(\.[0-9]*)?
 *      boolean := "true" | "false"
 *      nullable := "null"
 *      value := number | string | boolean | nullable | array | object
 *      array := '[' value (',' value)* ']'
 *      key_value := string ':' value
 *      object := '{' key_value (',' key_value)* '}'
 *      json := array | object
 */

namespace types
{
    namespace json
    {
        //{ describe json data types

        using value = variant_decorator<int, float, std::string, bool, nullptr_t, boost::recursive_wrapper<class array>, boost::recursive_wrapper<class object>>;
            
        class array : public std::vector<types::json::value>
        {
            using vector::vector;
            using vector::operator=;
        };
        class object : public std::map<std::string, types::json::value>
        {
            using map::map;
            using map::operator=;
        };

        using json = variant_decorator<types::json::array, types::json::object>;
        //}
    }
}

namespace parser
{
    namespace json
    {
        //? Why I need ``sfloat_`` instead of just ``x3::float_``?
        //? What is the syntax ``class array_``? Is it wrong?
        const auto sfloat_ = x3::real_parser<float, x3::strict_real_policies<float>>();

        //{ describe json grammar
        const auto number = sfloat_ | x3::int_;
        struct boolean_val : x3::symbols<bool>
        {
            boolean_val()
            {
                add("true", true)("false", false);
            }
        };
        struct nullabl_symbol : x3::symbols<nullptr_t>
        {
            nullabl_symbol()
            {
                add("null", nullptr);
            }
        };



        const auto boolean = x3::rule<class boolean, bool> {} = boolean_val();
        const auto nullable = x3::rule<class nullable, nullptr_t>{} = nullabl_symbol();
        x3::rule<class array, types::json::array> array;
        x3::rule<class object, types::json::object> object;
        x3::rule<class json, types::json::json> json;
        const auto value = x3::rule<class value, types::json::value>{} = 
            number | quoted_string | boolean | nullable | array | object;
        
        const auto key_value = quoted_string >> ':' >> value; 

        const auto array_def = x3::rule<class array_def, std::vector<types::json::value>>{} = '[' >> (value % ',') >> ']';
        
        const auto object_def = '{' >> (key_value % ',') >> '}';
        const auto json_def = array | object;
        //}

        BOOST_SPIRIT_DEFINE(array, object, json)
    }
}

namespace literals
{
    namespace json
    {
        //{ describe ``_json`` literal
        types::json::json operator"" _json(const char* str, size_t)
        {
            return parser::load_from_string<types::json::json>(str, parser::json::json);
        }
        //}*/
    }
}

#endif // __JSON_HPP__
