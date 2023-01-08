#include "NNLC.h"

#include <cmath>
#include <string>
#include <vector>

using std::string;

using std::vector;

using IncludesVector = vector<string>;

namespace constants
{

	// For Debug
	static constexpr bool
		DELETE_CPP_FILE{
			true};

	// Includes
	static vector<
		string>
		IO_INCLUDES{
			"#include <iostream>\n"};

	static vector<
		string>
		STRING_INCLUDES{
			"#include <string>"};

	static vector<
		string>
		CONV_INCLUDES{
			"#include <string>\n",
			"#include <cstdlib>\n"};

	static vector<
		string>
		MATH_INCLUDES{
			"#include <cmath>\n"};

	static vector<
		string>
		TYPE_INCLUDES{
			"#include <type_traits>\n",
			"#include <typeinfo>\n",
			"#include <memory>\n",
			"#include <string>\n",
			"#include <cstdlib>\n",
			"#include <typeinfo>\n"};

	static vector<
		string>
		NONE_INCLUDE{
			""};

	// Transpiler
	static const string
		FIRST_MAIN{
			"int main()\n{\n"};

	static const string
		INDENTATION{
			"	"};

	static const string
		NEWLINE{
			"\n"};

	static const string
		LAST_MAIN{
			INDENTATION +
			"return 0;\n}\n"};

	static const string
		DEFAULT{
			"DEF"}; // Placeholder variable

	// C Replacement -- WIP
	static const string
		SPACE{
              "\" \""};
	static const string
		NEWLINE_CHARACTER{
			"\"\\n\""};

	static const string
		TRUE{
			"true"};

	static const string
		FALSE{
			"false"};

	static const string
		NONE{
			""};

	// Messages
	static const string
		HELP_MESSAGE{

			"Incorrect arguments.\n"
			"NNLC Usage:\n"
			"[NNLC Executable] [anyfile.nnl] [c/nc - compile/nocompile]\n"
			"Warning: You need g++ to compile.\n"};

	// Functions
	static const string
		TYPE_NAME{

			"#ifndef _MSC_VER\n"
			"#include <cxxabi.h>\n"
			"#endif\n"
			"\n"
			"template <class T> std::string type_name()\n"
			"{\n"
			"	typedef typename std::remove_reference<T>::type TR;\n"
			"	std::unique_ptr<char, void(*)(void *)> own\n"
			"	(\n"
			"#ifndef _MSC_VER\n"
			"		abi::__cxa_demangle(typeid(TR).name(), nullptr,\n"
			"			nullptr, nullptr),\n"
			"#else\n"
			"		nullptr,\n"
			"#endif\n"
			"		std::free\n"
			"	);\n"
			"	std::string r = own != nullptr ? own.get() : typeid(TR).name();\n"
			"	if (std::is_const<TR>::value) { r += \" const\"; }\n"
			"	if (std::is_volatile<TR>::value) { r += \" volatile\"; }\n"
			"	if (std::is_lvalue_reference<T>::value) { r += \"&\"; }\n"
			"	else if (std::is_rvalue_reference<T>::value) { r += \"&&\"; }\n"
			"	return r;\n"
			"}\n"};

}

namespace vars
{

	static string
		def{
			constants::DEFAULT};

	static bool
		def_reference{
			false};

	static bool
		ipow_dec{
			false};

	static bool
		fpow_dec{
			false};

	static bool
		dpow_dec{
			false};

	static bool
		type_dec{
			false};

}

string prefixCheck(string prefix = "")
{

	if (vars::def_reference)

	{

		return "";
	}

	else

	{

		return prefix;
	}
}

// Variables
string NNLC::dec(string var_name, string var_value)
{
	return "auto " + var_name + "{ " + var_value + " };\n";
}

string NNLC::dec(string var_name, string var_value, string type)
{
	return type + " " + var_name + "{ " + var_value + " };\n";
}

string NNLC::mov(string l_var, string r_var)
{
	return "auto " + r_var + "{ " + l_var + " };\n";
}

string NNLC::anydef(string var_name, bool reference)
{

	vars::def = var_name;

	vars::def_reference = reference;

	return string("");
}

string NNLC::conv(string var_name, string type)
{
	return prefixCheck(type + " ") + vars::def + "{ static_cast<" + type +
		   ">(" + var_name + ") };\n";
}

string NNLC::iconv(string old_var, string new_var)
{
	return "int " + new_var + "{ std::stoi(" + old_var + ") };\n";
}

string NNLC::fconv(string old_var, string new_var)
{
	return "float " + new_var + "{ strtof(" + old_var + ") };\n";
}

string NNLC::dconv(string old_var, string new_var)
{
	return "double " + new_var + "{ atof(" + old_var + ".c_str()) };\n";
}

string NNLC::type(string var_name)
{

	// TODO
	return "";
}

string NNLC::round(string l_var, string r_var)
{
	return "auto " + r_var + "{ round(" + l_var + ") };\n";
}

// Math
string NNLC::math(string l_var, string r_var, string mode, bool def)
{

	if (mode == "pow")

	{

		return prefixCheck("int ") + vars::def + " = pow(" + l_var + ", " +
			   r_var + ");\n";
	}

	else if (mode == "rpow")

	{

		return l_var + " = pow(" + l_var + ", " + r_var + ");\n";
	}

	if (def)

	{

		return prefixCheck("auto ") + vars::def + " {" + l_var + " " + mode +
			   " " + r_var + "};\n";
	}

	else

	{

		return l_var + " " + mode + "= " + r_var + ";\n";
	}
}

string NNLC::cat(string l_var, string r_var)
{
	return l_var + " += " + "std::string(" + r_var + ");\n";
}

string NNLC::mathpow(string base, string exp, string mode)
{
	return prefixCheck("int ") + vars::def + " = " + mode + "pow(" + base +
		   ", " + exp + ");\n";
} // mode == i, d, etc

// Input/Output
string NNLC::io(string mode, string var)
{

	if (mode == "out")

	{

		return "std::cout << " + var + ";\n";
	}

	else if (mode == "nlout")

	{

		return "std::cout << " + var + " << '\\n';\n";
	}

	else if (mode == "in")

	{

		return "std::string " + var + "{};\n" + constants::INDENTATION +
			   "std::cin >> " + var + ";\n";
	}

	else if (mode == "input")

	{

		if (vars::def_reference)

		{

			return "std::cout << " + var + ";\n"
										   "" +
				   constants::INDENTATION + "std::cin >> " + vars::def + ";\n";
		}

		else

		{

			return "std::cout << " + var + ";\n"
										   "std::string " +
				   vars::def + " {};\n"
							   "" +
				   constants::INDENTATION + "std::cin >> " + vars::def + ";\n";
		}
	}

	else if (mode == "defin")

	{

		if (vars::def_reference)

		{

			return "std::cin >> " + vars::def + ";\n";
		}

		else

		{

			return "std::string " + vars::def + "{};\n" +
				   constants::INDENTATION + "std::cin >> " + vars::def + ";\n";
		}
	}

	else

	{

		return string("");
	}
}

string NNLC::assert(string l_var, string r_var, string mode, bool format)
{

	if (format)

	{

		return "if (" + l_var + " " + mode + " " + r_var + ")"
														   " { std::cout << \"" +
			   l_var + " " + mode + " " + r_var +
			   " = true\\n\"; }\n"
			   "else { std::cout << \"" +
			   l_var + " " + mode +
			   " " + r_var + " = false\\n\"; }\n";
	}

	else

	{

		return "if (" + l_var + " " + mode + " " + r_var +
			   ") { std::cout << \"true\\n\"; }\n"
			   "else { std::cout << \"false\\n\";\n";
	}

} // mode == "==", <, >, etc

string NNLC::cpp(string b, string c)
{
	return "" + b + c + '\n';
} // Use with include

vector<string> getIncludes(string to_include)
{

	if (to_include == "io")

	{

		return constants::IO_INCLUDES;
	}

	else if (to_include == "conv")

	{

		return constants::CONV_INCLUDES;
	}

	else if (to_include == "math")

	{

		return constants::MATH_INCLUDES;
	}

	else if (to_include == "string")

	{

		return constants::STRING_INCLUDES;
	}

	else if (to_include == "type")

	{

		return constants::TYPE_INCLUDES;
	}

	else

	{

		return constants::NONE_INCLUDE;
	}
}

string NNLC::process_c(string c)
{

	// Removes " ;" and anything past it
	string cut_c{
		c.substr(0, c.find(" ;", 0))};

	if (cut_c == "NEWLINE")

	{

		return constants::NEWLINE_CHARACTER;
	}

	else if (cut_c == "SPACE")

	{

		return constants::SPACE;
	}

	else if (cut_c == "TRUE")

	{

		return constants::TRUE;
	}

	else if (cut_c == "FALSE")

	{

		return constants::FALSE;
	}

	else if (cut_c == "NONE")

	{

		return constants::NONE;
	}

	else

	{

		return cut_c;
	};
}

string NNLC::process_line(string a, string b, string c,
						 IncludesVector *include_vector)
{

	if (a == "dec")

	{

		return NNLC::dec(b, c);
	}

	else if (a == "sdec") // string
	{

		include_vector->push_back("#include <string>\n");

		return NNLC::dec(b, c, "std::string");
	}

	else if (a == "idec")

	{

		return NNLC::dec(b, c, "int");

	} // int
	else if (a == "cdec")

	{

		return NNLC::dec(b, c, "char");

	} // char
	else if (a == "fdec")

	{

		return NNLC::dec(b, c, "float");

	} // float
	else if (a == "ddec")

	{

		return NNLC::dec(b, c, "double");

	} // double
	else if (a == "bdec")

	{

		return NNLC::dec(b, c, "bool");

	} // bool
	else if (a == "cat")

	{

		include_vector->push_back("#include <string>\n");

		return NNLC::cat(b, c);
	}

	else if (a == "mov")

	{

		return NNLC::mov(b, c);
	}

	else if (a == "def")

	{

		return NNLC::anydef(b, false);
	}

	else if (a == "rdef")

	{

		return NNLC::anydef(b, true);
	}

	else if (a == "conv")

	{

		return NNLC::conv(b, c);
	}

	else if (a == "iconv")

	{

		include_vector->push_back("#include <string>\n");

		return NNLC::iconv(b, c);
	}

	else if (a == "fconv")

	{

		include_vector->push_back("#include <cstdlib>\n");

		return NNLC::fconv(b, c);
	}

	else if (a == "dconv")

	{

		include_vector->push_back("#include <cstdlib>\n");

		return NNLC::dconv(b, c);
	}

	else if (a == "add")

	{

		return NNLC::math(b, c, "+", false);
	}

	else if (a == "sub")

	{

		return NNLC::math(b, c, "-", false);
	}

	else if (a == "mul")

	{

		return NNLC::math(b, c, "*", false);
	}

	else if (a == "div")

	{

		return NNLC::math(b, c, "/", false);
	}

	else if (a == "mod")

	{

		return NNLC::math(b, c, "%", false);
	}

	else if (a == "pow")

	{

		include_vector->push_back("#include <cmath>\n");

		return NNLC::math(b, c, "pow", false);
	}

	else if (a == "rpow")

	{

		include_vector->push_back("#include <cmath>\n");

		return NNLC::math(b, c, "rpow", false);
	}

	else if (a == "defadd")

	{

		return NNLC::math(b, c, "+", true);
	}

	else if (a == "defsub")

	{

		return NNLC::math(b, c, "-", true);
	}

	else if (a == "defmul")

	{

		return NNLC::math(b, c, "*", true);
	}

	else if (a == "defdiv")

	{

		return NNLC::math(b, c, "/", true);
	}

	else if (a == "defmod")

	{

		return NNLC::math(b, c, "%", true);
	}

	else if (a == "defpow")

	{

		include_vector->push_back("#include <cmath>\n");

		return NNLC::math(b, c, "pow", true);
	}

	else if (a == "ipow")

	{

		return NNLC::mathpow(b, c, "i");
	}

	else if (a == "fpow")

	{

		return NNLC::mathpow(b, c, "f");
	}

	else if (a == "dpow")

	{

		return NNLC::mathpow(b, c, "d");
	}

	else if (a == "round")

	{

		include_vector->push_back("#include <cmath>\n");

		return NNLC::round(b, c);
	}

	else if (a == "io")

	{

		include_vector->push_back("#include <iostream>\n");

		return NNLC::io(b, c);
	}

	else if (a == "asserteq")

	{

		return NNLC::assert(b, c, "==", false);
	}

	else if (a == "fasserteq")

	{

		return NNLC::assert(b, c, "==", true);
	}

	else if (a == "assertlt")

	{

		return NNLC::assert(b, c, "<", false);
	}

	else if (a == "fassertlt")

	{

		return NNLC::assert(b, c, "<", true);
	}

	else if (a == "assertgt")

	{

		return NNLC::assert(b, c, ">", false);
	}

	else if (a == "fassertgt")

	{

		return NNLC::assert(b, c, ">", true);
	}

	else if (a == "assertgteq")

	{

		return NNLC::assert(b, c, ">=", false);
	}

	else if (a == "fassertgteq")

	{

		return NNLC::assert(b, c, ">=", true);
	}

	else if (a == "assertlteq")

	{

		return NNLC::assert(b, c, "<=", false);
	}

	else if (a == "fassertlteq")

	{

		return NNLC::assert(b, c, "<=", true);
	}

	else if (a == "cpp")

	{

		return NNLC::cpp(b, c);
	}

	else if (a == "include")

	{

		vector<string> includes{
			getIncludes(b)};

		for (string include : includes)

		{

			include_vector->push_back(include);
		}

		return string();
	}

	else if (a == "type" || a == "ptype" || a == "fptype")

	{

		vector<string> includes{
			getIncludes("type")};

		for (string include : includes)

		{

			include_vector->push_back(include);
		}

		if (a == "type")

		{

			return prefixCheck("auto ") + vars::def +
				   "{ type_name<decltype(" + b + ")>() };\n";
		}

		else if (a == "ptype")

		{

			include_vector->push_back("#include <iostream>\n");

			return "std::cout << " + string() + " type_name<decltype(" + b +
				   ")>() << '\\n';\n";
		}

		else if (a == "fptype")

		{

			include_vector->push_back("#include <iostream>\n");

			return "std::cout << \"variable '" + b +
				   "' has type '\" << type_name<decltype(" + b +
				   ")>() << \"'\\n\";\n";
		}

		else

		{

			return "";
		}
	}

	else

	{

		return "";
	}
}

string NNLC::declare_function(string function_name)
{

	if (function_name == "ipow" && !(vars::ipow_dec))

	{

		vars::ipow_dec = true;

		return "int ipow(int base, int exp) { return exp == 0 ? 1 : base * ipow(base, exp - 1); }\n";
	}

	else if (function_name == "fpow" && !(vars::fpow_dec))

	{

		vars::fpow_dec = true;

		return "float fpow(float base, float exp) { return exp == 0 ? 1 : base * fpow(base, exp - 1); }\n";
	}

	else if (function_name == "dpow" && !(vars::dpow_dec))

	{

		vars::dpow_dec = true;

		return "double dpow(double base, double exp) { return exp == 0 ? 1 : base * dpow(base, exp - 1); }\n";
	}

	else if ((function_name == "type" || function_name == "ptype" || function_name == "fptype") && !(vars::type_dec))

	{

		vars::type_dec = true;

		return constants::TYPE_NAME;
	}

	else

	{

		return "";
	}
}
