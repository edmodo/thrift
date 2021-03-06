/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*
 * This file is programmatically sanitized for style:
 * astyle --style=1tbs -f -p -H -j -U t_go_generator.cc
 *
 * The output of astyle should not be taken unquestioningly, but it is a good
 * guide for ensuring uniformity and readability.
 */

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>
#include <algorithm>
#include "t_generator.h"
#include "platform.h"
#include "version.h"

using namespace std;

/**
 * A helper for automatically formatting the emitted Go code from the Thrift
 * IDL per the Go style guide.
 *
 * Returns:
 *  - true, if the formatting process succeeded.
 *  - false, if the formatting process failed, which means the basic output was
 *           still generated.
 */
bool format_go_output(const string &file_path);

const string default_thrift_import = "git.apache.org/thrift.git/lib/go/thrift";

/**
 * Go code generator.
 */
class t_go_generator : public t_generator
{
public:
    t_go_generator(
        t_program* program,
        const std::map<std::string, std::string>& parsed_options,
        const std::string& option_string)
        : t_generator(program) {
        std::map<std::string, std::string>::const_iterator iter;
        out_dir_base_ = "gen-go";
        gen_thrift_import_ = default_thrift_import;

        iter = parsed_options.find("package_prefix");

        if (iter != parsed_options.end()) {
            gen_package_prefix_ = (iter->second);
        }

        iter = parsed_options.find("thrift_import");

        if (iter != parsed_options.end()) {
            gen_thrift_import_ = (iter->second);
        }
    }

    /**
     * Init and close methods
     */

    void init_generator();
    void close_generator();

    /**
     * Program-level generation functions
     */

    void generate_typedef(t_typedef*  ttypedef);
    void generate_enum(t_enum*     tenum);
    void generate_const(t_const*    tconst);
    void generate_struct(t_struct*   tstruct);
    void generate_xception(t_struct*   txception);
    void generate_service(t_service*  tservice);

    std::string render_const_value(t_type* type, t_const_value* value, const string& name);

    /**
     * Struct generation code
     */

    void generate_go_struct(t_struct* tstruct, bool is_exception);
    void generate_go_struct_definition(std::ofstream& out, t_struct* tstruct, bool is_xception = false, bool is_result = false);
    void generate_isset_helpers(std::ofstream& out, t_struct* tstruct, const string& tstruct_name, bool is_result = false);
    void generate_go_struct_reader(std::ofstream& out, t_struct* tstruct, const string& tstruct_name, bool is_result = false);
    void generate_go_struct_writer(std::ofstream& out, t_struct* tstruct, const string& tstruct_name, bool is_result = false);
    void generate_go_function_helpers(t_function* tfunction);

    /**
     * Service-level generation functions
     */

    void generate_service_helpers(t_service*  tservice);
    void generate_service_interface(t_service* tservice);
    void generate_service_client(t_service* tservice);
    void generate_service_remote(t_service* tservice);
    void generate_service_server(t_service* tservice);
    void generate_process_function(t_service* tservice, t_function* tfunction);

    /**
     * Serialization constructs
     */

    void generate_deserialize_field(std::ofstream &out,
                                    t_field*    tfield,
                                    bool        declare,
                                    std::string prefix = "",
                                    bool inclass = false,
                                    bool coerceData = false);

    void generate_deserialize_struct(std::ofstream &out,
                                     t_struct*   tstruct,
                                     bool        declare,
                                     std::string prefix = "");

    void generate_deserialize_container(std::ofstream &out,
                                        t_type*     ttype,
                                        bool        declare,
                                        std::string prefix = "");

    void generate_deserialize_set_element(std::ofstream &out,
                                          t_set*      tset,
                                          bool        declare,
                                          std::string prefix = "");

    void generate_deserialize_map_element(std::ofstream &out,
                                          t_map*      tmap,
                                          bool        declare,
                                          std::string prefix = "");

    void generate_deserialize_list_element(std::ofstream &out,
                                           t_list*     tlist,
                                           bool        declare,
                                           std::string prefix = "");

    void generate_serialize_field(std::ofstream &out,
                                  t_field*    tfield,
                                  std::string prefix = "");

    void generate_serialize_struct(std::ofstream &out,
                                   t_struct*   tstruct,
                                   std::string prefix = "");

    void generate_serialize_container(std::ofstream &out,
                                      t_type*     ttype,
                                      std::string prefix = "");

    void generate_serialize_map_element(std::ofstream &out,
                                        t_map*      tmap,
                                        std::string kiter,
                                        std::string viter);

    void generate_serialize_set_element(std::ofstream &out,
                                        t_set*      tmap,
                                        std::string iter);

    void generate_serialize_list_element(std::ofstream &out,
                                         t_list*     tlist,
                                         std::string iter);

    void generate_go_docstring(std::ofstream& out,
                               t_struct* tstruct);

    void generate_go_docstring(std::ofstream& out,
                               t_function* tfunction);

    void generate_go_docstring(std::ofstream& out,
                               t_doc*    tdoc,
                               t_struct* tstruct,
                               const char* subheader);

    void generate_go_docstring(std::ofstream& out,
                               t_doc* tdoc);

    /**
     * Helper rendering functions
     */

    std::string go_autogen_comment();
    std::string go_package();
    std::string go_imports_begin(bool include_error_package);
    std::string go_imports_end();
    std::string render_includes(bool include_error_package);
    std::string render_import_protection();
    std::string render_fastbinary_includes();
    std::string declare_argument(t_field* tfield);
    std::string render_field_default_value(t_field* tfield, const string& name);
    std::string type_name(t_type* ttype);
    std::string function_signature(t_function* tfunction, std::string prefix = "");
    std::string function_signature_if(t_function* tfunction, std::string prefix = "", bool addError = false);
    std::string argument_list(t_struct* tstruct);
    std::string type_to_enum(t_type* ttype);
    std::string type_to_go_type(t_type* ttype);
    std::string type_to_go_key_type(t_type* ttype);
    std::string type_to_spec_args(t_type* ttype);

    static std::string get_real_go_module(const t_program* program) {
        std::string real_module = program->get_namespace("go");

        if (real_module.empty()) {
            return program->get_name();
        }

        return real_module;
    }

private:

    std::string gen_package_prefix_;
    std::string gen_thrift_import_;

    /**
     * File streams
     */

    std::ofstream f_types_;
    std::string f_types_name_;
    std::ofstream f_consts_;
    std::string f_consts_name_;
    std::stringstream f_const_values_;
    std::ofstream f_service_;

    std::string package_name_;
    std::string package_dir_;

    static std::string publicize(const std::string& value);
    static std::string new_prefix(const std::string& value);
    static std::string privatize(const std::string& value);
    static std::string variable_name_to_go_name(const std::string& value);
    static bool can_be_nil(t_type* value);

};


std::string t_go_generator::publicize(const std::string& value)
{
    if (value.size() <= 0) {
        return value;
    }

    std::string value2(value), prefix;

    string::size_type dot_pos = value.rfind('.');
    if (dot_pos != string::npos) {
      prefix = value.substr(0, dot_pos + 1) + prefix;
      value2 = value.substr(dot_pos + 1);
    }

    if (!isupper(value2[0])) {
        value2[0] = toupper(value2[0]);
    }

    // as long as we are changing things, let's change _ followed by lowercase to capital
    for (string::size_type i = 1; i < value2.size() - 1; ++i) {
        if (value2[i] == '_' && islower(value2[i + 1])) {
            value2.replace(i, 2, 1, toupper(value2[i + 1]));
        }
    }

    return prefix + value2;
}

std::string t_go_generator::new_prefix(const std::string& value)
{
    if (value.size() <= 0) {
        return value;
    }

    string::size_type dot_pos = value.rfind('.');
    if (dot_pos != string::npos) {
      return value.substr(0, dot_pos + 1) + "New" + publicize(value.substr(dot_pos + 1));
    }
    return "New" + publicize(value);
}

std::string t_go_generator::privatize(const std::string& value)
{
    if (value.size() <= 0) {
        return value;
    }

    std::string value2(value);

    if (!islower(value2[0])) {
        value2[0] = tolower(value2[0]);
    }

    // as long as we are changing things, let's change _ followed by lowercase to capital
    for (string::size_type i = 1; i < value2.size() - 1; ++i) {
        if (value2[i] == '_' && isalpha(value2[i + 1])) {
            value2.replace(i, 2, 1, toupper(value2[i + 1]));
        }
    }

    return value2;
}

std::string t_go_generator::variable_name_to_go_name(const std::string& value)
{
    if (value.size() <= 0) {
        return value;
    }

    std::string value2(value);
    std::transform(value2.begin(), value2.end(), value2.begin(), ::tolower);

    switch (value[0]) {
    case 'b':
    case 'B':
        if (value2 != "break") {
            return value;
        }

        break;

    case 'c':
    case 'C':
        if (value2 != "case" && value2 != "chan" && value2 != "const" && value2 != "continue") {
            return value;
        }

        break;

    case 'd':
    case 'D':
        if (value2 != "default" && value2 != "defer") {
            return value;
        }

        break;

    case 'e':
    case 'E':
        if (value2 != "else" && value2 != "error") {
            return value;
        }

        break;

    case 'f':
    case 'F':
        if (value2 != "fallthrough" && value2 != "for" && value2 != "func") {
            return value;
        }

        break;

    case 'g':
    case 'G':
        if (value2 != "go" && value2 != "goto") {
            return value;
        }

        break;

    case 'i':
    case 'I':
        if (value2 != "if" && value2 != "import" && value2 != "interface") {
            return value;
        }

        break;

    case 'm':
    case 'M':
        if (value2 != "map") {
            return value;
        }

        break;

    case 'p':
    case 'P':
        if (value2 != "package") {
            return value;
        }

        break;

    case 'r':
    case 'R':
        if (value2 != "range" && value2 != "return") {
            return value;
        }

        break;

    case 's':
    case 'S':
        if (value2 != "select" && value2 != "struct" && value2 != "switch") {
            return value;
        }

        break;

    case 't':
    case 'T':
        if (value2 != "type") {
            return value;
        }

        break;

    case 'v':
    case 'V':
        if (value2 != "var") {
            return value;
        }

        break;

    default:
        return value;
    }

    return value2 + "_a1";
}


/**
 * Prepares for file generation by opening up the necessary file output
 * streams.
 *
 * @param tprogram The program to generate
 */
void t_go_generator::init_generator()
{
    // Make output directory
    string module = get_real_go_module(program_);
    string target = module;
    package_dir_ = get_out_dir();

    while (true) {
        // TODO: Do better error checking here.
        MKDIR(package_dir_.c_str());

        if (module.empty()) {
            break;
        }

        string::size_type pos = module.find('.');

        if (pos == string::npos) {
            package_dir_ += "/";
            package_dir_ += module;
            package_name_ = module;
            module.clear();
        } else {
            package_dir_ += "/";
            package_dir_ += module.substr(0, pos);
            module.erase(0, pos + 1);
        }
    }

    string::size_type loc;

    while ((loc = target.find(".")) != string::npos) {
        target.replace(loc, 1, 1, '/');
    }

    // Make output files
    f_types_name_ = package_dir_ + "/" + "ttypes.go";
    f_types_.open(f_types_name_.c_str());

    f_consts_name_ = package_dir_ + "/" + "constants.go";
    f_consts_.open(f_consts_name_.c_str());

    vector<t_service*> services = program_->get_services();
    vector<t_service*>::iterator sv_iter;

    for (sv_iter = services.begin(); sv_iter != services.end(); ++sv_iter) {
        string service_dir = package_dir_ + "/" + underscore((*sv_iter)->get_name()) + "-remote";
        MKDIR(service_dir.c_str());
    }

    // Print header
    f_types_ <<
             go_autogen_comment() <<
             go_package() <<
             render_includes(false) <<
             render_import_protection();

    f_consts_ <<
              go_autogen_comment() <<
              go_package() <<
              render_includes(false);

    f_const_values_ << endl << "func init() {" << endl;

}

/**
 * Renders all the imports necessary for including another Thrift program
 */
string t_go_generator::render_includes(bool include_error_package)
{
    const vector<t_program*>& includes = program_->get_includes();
    string result = "";
    string unused_prot = "";

    for (size_t i = 0; i < includes.size(); ++i) {
        string go_module = get_real_go_module(includes[i]);
        size_t found = 0;
        for (size_t j = 0; j < go_module.size(); j++) {
            // Import statement uses slashes ('/') in namespace
            if (go_module[j] == '.') {
                go_module[j] = '/';
                found = j + 1;
            }
        }

        result += "\t\"" + gen_package_prefix_ + go_module + "\"\n";
        unused_prot += "var _ = " + go_module.substr(found) + ".GoUnusedProtection__\n";
    }

    if (includes.size() > 0) {
        result += "\n";
    }

    return go_imports_begin(include_error_package) + result + go_imports_end() + unused_prot;
}

string t_go_generator::render_import_protection() {
  return string("var GoUnusedProtection__ int;\n\n");
}

/**
 * Renders all the imports necessary to use the accelerated TBinaryProtocol
 */
string t_go_generator::render_fastbinary_includes()
{
    return "";
}

/**
 * Autogen'd comment
 */
string t_go_generator::go_autogen_comment()
{
    return
        std::string() +
        "// Autogenerated by Thrift Compiler (" + THRIFT_VERSION + ")\n"
        "// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING\n\n";
}

/**
 * Prints standard thrift package
 */
string t_go_generator::go_package()
{
    return string("package ") + package_name_ + "\n\n";
}

/**
 * Render the beginning of the import statement
 */
string t_go_generator::go_imports_begin(bool include_error_package)
{
    string to_return = 
        string("import (\n"
               "\t\"fmt\"\n"
               "\t\"math\"\n"
               "\t\"" + gen_thrift_import_ + "\"\n");

    if(include_error_package) {
        to_return += "\tgoerr \"github.com/bugsnag/bugsnag-go/errors\"\n";
    }

    return to_return;
}

/**
 * End the import statement, include undscore-assignments
 *
 * These "_ =" prevent the go compiler complaining about used imports.
 * This will have to do in lieu of more intelligent import statement construction
 */
string t_go_generator::go_imports_end()
{
    return
        string(
            ")\n\n"
            "// (needed to ensure safety because of naive import list construction.)\n"
            "var _ = math.MinInt32\n"
            "var _ = thrift.ZERO\n"
            "var _ = fmt.Printf\n\n");
}

/**
 * Closes the type files
 */
void t_go_generator::close_generator()
{
    f_const_values_ << "}" << endl << endl;
    f_consts_ << f_const_values_.str();

    // Close types and constants files
    f_consts_.close();
    f_types_.close();
    format_go_output(f_types_name_);
    format_go_output(f_consts_name_);
}

/**
 * Generates a typedef. This is not done in go, types are all implicit.
 *
 * @param ttypedef The type definition
 */
void t_go_generator::generate_typedef(t_typedef* ttypedef)
{
    generate_go_docstring(f_types_, ttypedef);
    string newTypeDef(publicize(ttypedef->get_symbolic()));
    string baseType(type_to_go_type(ttypedef->get_type()));

    if (baseType == newTypeDef) {
        return;
    }

    f_types_ <<
             "type " << newTypeDef << " " << baseType << endl << endl;
}

/**
 * Generates code for an enumerated type. Done using a class to scope
 * the values.
 *
 * @param tenum The enumeration
 */
void t_go_generator::generate_enum(t_enum* tenum)
{
    std::ostringstream to_string_mapping, from_string_mapping;
    std::string tenum_name(publicize(tenum->get_name()));
    generate_go_docstring(f_types_, tenum);
    f_types_ <<
             "type " << tenum_name << " int64" << endl <<
             "const (" << endl;
    to_string_mapping <<
                      indent() << "func (p " << tenum_name << ") String() string {" << endl <<
                      indent() << "  switch p {" << endl;
    from_string_mapping <<
                        indent() << "func " << tenum_name << "FromString(s string) (" << tenum_name << ", error) {" << endl <<
                        indent() << "  switch s {" << endl;
    vector<t_enum_value*> constants = tenum->get_constants();
    vector<t_enum_value*>::iterator c_iter;
    int value = -1;

    for (c_iter = constants.begin(); c_iter != constants.end(); ++c_iter) {
        if ((*c_iter)->has_value()) {
            value = (*c_iter)->get_value();
        } else {
            ++value;
        }

        string iter_std_name(escape_string((*c_iter)->get_name()));
        string iter_name((*c_iter)->get_name());
        f_types_ <<
                 indent() << "  " << tenum_name << "_" << iter_name << ' ' << tenum_name << " = " << value << endl;
        // Dictionaries to/from string names of enums
        to_string_mapping <<
                          indent() << "  case " << tenum_name << "_" << iter_name << ": return \"" << tenum_name << "_" << iter_std_name << "\"" << endl;

        if (iter_std_name != escape_string(iter_name)) {
            from_string_mapping <<
                                indent() << "  case \"" << tenum_name << "_" << iter_std_name << "\", \"" << escape_string(iter_name) << "\": return " <<
                                tenum_name << "_" << iter_name << ", nil " << endl;
        } else {
            from_string_mapping <<
                                indent() << "  case \"" << tenum_name << "_" << iter_std_name << "\": return " <<
                                tenum_name << "_" << iter_name << ", nil "  << endl;
        }
    }

    to_string_mapping <<
                      indent() << "  }" << endl <<
                      indent() << "  return \"<UNSET>\"" << endl <<
                      indent() << "}" << endl;
    from_string_mapping <<
                        indent() << "  }" << endl <<
                        indent() << "  return " << tenum_name << "(math.MinInt32 - 1)," <<
                        " fmt.Errorf(\"not a valid " << tenum_name << " string\")" << endl <<
                        indent() << "}" << endl;

    f_types_ << ")" << endl << endl
             << to_string_mapping.str() << endl
             << from_string_mapping.str() << endl << endl;

}


/**
 * Generate a constant value
 */
void t_go_generator::generate_const(t_const* tconst)
{
    t_type* type = tconst->get_type();
    string name = publicize(tconst->get_name());
    t_const_value* value = tconst->get_value();

    if (type->is_base_type() || type->is_enum()) {
        indent(f_consts_) << "const " << name << " = " << render_const_value(type, value, name) << endl;
    } else {
        f_const_values_ <<
                        indent() << name << " = " << render_const_value(type, value, name) << endl << endl;

        f_consts_ <<
                  indent() << "var " << name << " " << type_to_go_type(type) << endl;
    }
}

/**
 * Prints the value of a constant with the given type. Note that type checking
 * is NOT performed in this function as it is always run beforehand using the
 * validate_types method in main.cc
 */
string t_go_generator::render_const_value(t_type* type, t_const_value* value, const string& name)
{
    type = get_true_type(type);
    std::ostringstream out;

    if (type->is_base_type()) {
        t_base_type::t_base tbase = ((t_base_type*)type)->get_base();

        switch (tbase) {
        case t_base_type::TYPE_STRING:
            if (((t_base_type*)type)->is_binary()) {
                out << "[]byte(\"" << get_escaped_string(value) << "\")";
            } else {
                out << '"' << get_escaped_string(value) << '"';
            }

            break;

        case t_base_type::TYPE_BOOL:
            out << (value->get_integer() > 0 ? "true" : "false");
            break;

        case t_base_type::TYPE_BYTE:
        case t_base_type::TYPE_I16:
        case t_base_type::TYPE_I32:
        case t_base_type::TYPE_I64:
            out << value->get_integer();
            break;

        case t_base_type::TYPE_DOUBLE:
            if (value->get_type() == t_const_value::CV_INTEGER) {
                out << value->get_integer();
            } else {
                out << value->get_double();
            }

            break;

        default:
            throw "compiler error: no const of base type " + t_base_type::t_base_name(tbase);
        }
    } else if (type->is_enum()) {
        indent(out) << value->get_integer();
    } else if (type->is_struct() || type->is_xception()) {
        out <<
            "&" << publicize(type_name(type)) << "{";
        indent_up();
        const vector<t_field*>& fields = ((t_struct*)type)->get_members();
        vector<t_field*>::const_iterator f_iter;
        const map<t_const_value*, t_const_value*>& val = value->get_map();
        map<t_const_value*, t_const_value*>::const_iterator v_iter;

        for (v_iter = val.begin(); v_iter != val.end(); ++v_iter) {
            t_type* field_type = NULL;

            for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
                if ((*f_iter)->get_name() == v_iter->first->get_string()) {
                    field_type = (*f_iter)->get_type();
                }
            }

            if (field_type == NULL) {
                throw "type error: " + type->get_name() + " has no field " + v_iter->first->get_string();
            }

            if (field_type->is_base_type() || field_type->is_enum()) {
                out << endl <<
                    indent() << publicize(v_iter->first->get_string()) << ": " << render_const_value(field_type, v_iter->second, name) << ",";
            } else {
                string k(tmp("k"));
                string v(tmp("v"));
                out << endl <<
                    indent() << v << " := " << render_const_value(field_type, v_iter->second, v) << endl <<
                    indent() << name << "." << publicize(v_iter->first->get_string()) << " = " << v;
            }
        }

        out << "}";

        indent_down();
    } else if (type->is_map()) {
        t_type* ktype = ((t_map*)type)->get_key_type();
        t_type* vtype = ((t_map*)type)->get_val_type();
        const map<t_const_value*, t_const_value*>& val = value->get_map();
        out <<
            "map[" << type_to_go_type(ktype) << "]" << type_to_go_type(vtype) << "{" << endl;
        indent_up();
        map<t_const_value*, t_const_value*>::const_iterator v_iter;

        for (v_iter = val.begin(); v_iter != val.end(); ++v_iter) {
            out <<
                indent() << render_const_value(ktype, v_iter->first, name) << ": " <<
                render_const_value(vtype, v_iter->second, name) << "," << endl;
        }

        indent_down();
        out <<
            indent() << "}";
    } else if (type->is_list()) {
        t_type* etype = ((t_list*)type)->get_elem_type();
        const vector<t_const_value*>& val = value->get_list();
        out <<
            "[]" << type_to_go_type(etype) << "{" << endl;
        indent_up();
        vector<t_const_value*>::const_iterator v_iter;

        for (v_iter = val.begin(); v_iter != val.end(); ++v_iter) {
            out <<
                indent() << render_const_value(etype, *v_iter, name) << ", ";
        }

        indent_down();
        out <<
            indent() << "}";
    } else if (type->is_set()) {
        t_type* etype = ((t_set*)type)->get_elem_type();
        const vector<t_const_value*>& val = value->get_list();
        out <<
            "map[" << type_to_go_key_type(etype) << "]bool{" << endl;
        indent_up();
        vector<t_const_value*>::const_iterator v_iter;

        for (v_iter = val.begin(); v_iter != val.end(); ++v_iter) {
            out <<
                indent() << render_const_value(etype, *v_iter, name) << ": true," << endl;

        }

        indent_down();
        out <<
            indent() << "}";
    } else {
        throw "CANNOT GENERATE CONSTANT FOR TYPE: " + type->get_name();
    }

    return out.str();
}

/**
 * Generates a go struct
 */
void t_go_generator::generate_struct(t_struct* tstruct)
{
    generate_go_struct(tstruct, false);
}

/**
 * Generates a struct definition for a thrift exception. Basically the same
 * as a struct but extends the Exception class.
 *
 * @param txception The struct definition
 */
void t_go_generator::generate_xception(t_struct* txception)
{
    generate_go_struct(txception, true);
}

/**
 * Generates a go struct
 */
void t_go_generator::generate_go_struct(t_struct* tstruct,
                                        bool is_exception)
{
    generate_go_struct_definition(f_types_, tstruct, is_exception);
}

/**
 * Generates a struct definition for a thrift data type.
 *
 * @param tstruct The struct definition
 */
void t_go_generator::generate_go_struct_definition(ofstream& out,
        t_struct* tstruct,
        bool is_exception,
        bool is_result)
{
    const vector<t_field*>& members = tstruct->get_members();
    const vector<t_field*>& sorted_members = tstruct->get_sorted_members();
    vector<t_field*>::const_iterator m_iter;

    std::string tstruct_name(publicize(tstruct->get_name()));
    out <<
        indent() << "type " << tstruct_name << " struct {" << endl;
    /*
       Here we generate the structure specification for the fastbinary codec.
       These specifications have the following structure:
       thrift_spec -> tuple of item_spec
       item_spec -> nil | (tag, type_enum, name, spec_args, default)
       tag -> integer
       type_enum -> TType.I32 | TType.STRING | TType.STRUCT | ...
       name -> string_literal
       default -> nil  # Handled by __init__
       spec_args -> nil  # For simple types
                  | (type_enum, spec_args)  # Value type for list/set
                  | (type_enum, spec_args, type_enum, spec_args)
                    # Key and value for map
                  | (class_name, spec_args_ptr) # For struct/exception
       class_name -> identifier  # Basically a pointer to the class
       spec_args_ptr -> expression  # just class_name.spec_args

       TODO(dreiss): Consider making this work for structs with negative tags.
    */
    // TODO(dreiss): Look into generating an empty tuple instead of nil
    // for structures with no members.
    // TODO(dreiss): Test encoding of structs where some inner structs
    // don't have thrift_spec.
    indent_up();

    if (sorted_members.empty() || (sorted_members[0]->get_key() >= 0)) {
        int sorted_keys_pos = 0;

        for (m_iter = sorted_members.begin(); m_iter != sorted_members.end(); ++m_iter) {
            for (; sorted_keys_pos != (*m_iter)->get_key(); sorted_keys_pos++) {
                if (sorted_keys_pos != 0) {
                    indent(out) << "// unused field # " << sorted_keys_pos << endl;
                }
            }

            t_type* fieldType = (*m_iter)->get_type();
            string goType(type_to_go_type(fieldType));

            indent(out) << publicize(variable_name_to_go_name((*m_iter)->get_name())) << " "
                        << goType << " `thrift:\""
                        << escape_string((*m_iter)->get_name())
                        << "," << sorted_keys_pos;

            if ((*m_iter)->get_req() == t_field::T_REQUIRED) {
                out << ",required";
            }

            out << "\"`" << endl;
            sorted_keys_pos ++;
        }
    } else {
        for (m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
            // This fills in default values, as opposed to nulls
            out <<
                indent() << publicize((*m_iter)->get_name()) << " " <<
                type_to_go_type((*m_iter)->get_type()) << endl;
        }
    }

    indent_down();
    out <<
        indent() << "}" << endl << endl <<
        indent() << "func New" << tstruct_name << "() *" << tstruct_name << " {" << endl <<
        indent() << "  return &" << tstruct_name << "{";

    for (m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
        // Initialize fields
        const string base_field_name = (*m_iter)->get_name();
        const string escaped_field_name = escape_string(base_field_name);
        const string go_safe_name = variable_name_to_go_name(escaped_field_name);
        const string publicized_name = publicize(go_safe_name);
        const t_type* type = get_true_type((*m_iter)->get_type());
        const bool has_default_value = (*m_iter)->get_value() != NULL;
        const bool type_is_enum = type->is_enum();

        if (has_default_value) {
            out << endl << indent() << publicized_name << ": " << render_field_default_value(*m_iter, base_field_name) << "," << endl;
        } else if (type_is_enum) {
            out << endl << indent() << publicized_name << ": math.MinInt32 - 1, // unset sentinal value" << endl;
        }
    }

    out << "}" << endl;
    out << "}" << endl << endl;
    generate_isset_helpers(out, tstruct, tstruct_name, is_result);
    generate_go_struct_reader(out, tstruct, tstruct_name, is_result);
    generate_go_struct_writer(out, tstruct, tstruct_name, is_result);

    out <<
        indent() << "func (p *" << tstruct_name << ") String() string {" << endl <<
        indent() << "  if p == nil {" << endl <<
        indent() << "    return \"<nil>\"" << endl <<
        indent() << "  }" << endl <<
        indent() << "  return fmt.Sprintf(\"" << escape_string(tstruct_name) << "(%+v)\", *p)" << endl <<
        indent() << "}" << endl << endl;

}

/**
 * Generates the IsSet helper methods for a struct
 */
void t_go_generator::generate_isset_helpers(ofstream& out,
        t_struct* tstruct,
        const string& tstruct_name,
        bool is_result)
{
    const vector<t_field*>& fields = tstruct->get_members();
    vector<t_field*>::const_iterator f_iter;
    const string escaped_tstruct_name(escape_string(tstruct->get_name()));

    for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
        t_type* type = get_true_type((*f_iter)->get_type());

        if ((*f_iter)->get_req() == t_field::T_OPTIONAL || type->is_enum()) {
            const string field_name(publicize(variable_name_to_go_name(escape_string((*f_iter)->get_name()))));
            t_const_value* field_default_value = (*f_iter)->get_value();
            out <<
                indent() << "func (p *" << tstruct_name << ") IsSet" << field_name << "() bool {" << endl;
            indent_up();
            string s_check_value;
            int64_t i_check_value;
            double d_check_value;

            if (type->is_base_type()) {
                t_base_type::t_base tbase = ((t_base_type*)type)->get_base();

                switch (tbase) {
                case t_base_type::TYPE_STRING:
                    if (((t_base_type*)type)->is_binary()) {
                        // ignore default value for binary
                        out <<
                            indent() << "return p." << field_name << " != nil" << endl;
                    } else {
                        s_check_value = (field_default_value == NULL) ? "\"\"" : render_const_value(type, field_default_value, tstruct_name);
                        out <<
                            indent() << "return p." << field_name << " != " << s_check_value << endl;
                    }

                    break;

                case t_base_type::TYPE_BOOL:
                    s_check_value = (field_default_value != NULL && field_default_value->get_integer() > 0) ? "true" : "false";
                    out <<
                        indent() << "return p." << field_name << " != " << s_check_value << endl;
                    break;

                case t_base_type::TYPE_BYTE:
                case t_base_type::TYPE_I16:
                case t_base_type::TYPE_I32:
                case t_base_type::TYPE_I64:
                    i_check_value = (field_default_value == NULL) ? 0 : field_default_value->get_integer();
                    out <<
                        indent() << "return p." << field_name << " != " << i_check_value << endl;
                    break;

                case t_base_type::TYPE_DOUBLE:
                    d_check_value = (field_default_value == NULL) ? 0.0 : field_default_value->get_double();
                    out <<
                        indent() << "return p." << field_name << " != " << d_check_value << endl;
                    break;

                default:
                    throw "compiler error: no const of base type " + t_base_type::t_base_name(tbase);
                }
            } else if (type->is_enum()) {
                out << indent() << "return int64(p." << field_name << ") != "
                    << "math.MinInt32 - 1" << endl;
            } else if (type->is_struct() || type->is_xception()) {
                out <<
                    indent() << "return p." << field_name << " != nil" << endl;
            } else if (type->is_list() || type->is_set()) {
                if (field_default_value != NULL && field_default_value->get_list().size() > 0) {
                    out <<
                        indent() << "return p." << field_name << " != nil" << endl;
                } else {
                    out <<
                        indent() << "return p." << field_name << " != nil && len(p." << field_name << ") > 0" << endl;
                }
            } else if (type->is_map()) {
                if (field_default_value != NULL && field_default_value->get_map().size() > 0) {
                    out <<
                        indent() << "return p." << field_name << " != nil" << endl;
                } else {
                    out <<
                        indent() << "return p." << field_name << " != nil && len(p." << field_name << ") > 0" << endl;
                }
            } else {
                throw "CANNOT GENERATE ISSET HELPERS FOR TYPE: " + type->get_name();
            }

            indent_down();
            out <<
                indent() << "}" << endl << endl;
        }
    }
}

/**
 * Generates the read method for a struct
 */
void t_go_generator::generate_go_struct_reader(ofstream& out,
        t_struct* tstruct,
        const string& tstruct_name,
        bool is_result)
{
    const vector<t_field*>& fields = tstruct->get_members();
    vector<t_field*>::const_iterator f_iter;
    string escaped_tstruct_name(escape_string(tstruct->get_name()));
    out <<
        indent() << "func (p *" << tstruct_name << ") Read(iprot thrift.TProtocol) error {" << endl;
    indent_up();
    out <<
        indent() << "if _, err := iprot.ReadStructBegin(); err != nil {" << endl <<
        indent() << "  return fmt.Errorf(\"%T read error\", p)" << endl <<
        indent() << "}" << endl;
    // Loop over reading in fields
    indent(out) << "for {" << endl;
    indent_up();
    // Read beginning field marker
    out <<
        indent() << "_, fieldTypeId, fieldId, err := iprot.ReadFieldBegin()" << endl <<
        indent() << "if err != nil {" << endl <<
        indent() << "  return fmt.Errorf(\"%T field %d read error: %s\", p, fieldId, err)" << endl <<
        indent() << "}" << endl;
    // Check for field STOP marker and break
    out <<
        indent() << "if fieldTypeId == thrift.STOP { break; }" << endl;
    // Switch statement on the field we are reading
    bool first = true;
    string thriftFieldTypeId;
    // Generate deserialization code for known cases
    int32_t field_id = -1;

    for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
        field_id = (*f_iter)->get_key();

        if (first) {
            first = false;
            indent(out) << "switch fieldId {" << endl;
        }

        // if negative id, ensure we generate a valid method name
        string field_method_prefix("readField");

        if (field_id < 0) {
            field_method_prefix += "_";
            field_id *= -1;
        }

        indent_up();
        out << "case " << field_id << ":" << endl;
        thriftFieldTypeId = type_to_enum((*f_iter)->get_type());

        if (thriftFieldTypeId == "thrift.BINARY") {
            thriftFieldTypeId = "thrift.STRING";
        }

        out <<
            indent() << "if err := p." << field_method_prefix << field_id << "(iprot); err != nil {" << endl <<
            indent() << "  return err" << endl <<
            indent() << "}" << endl;
        indent_down();
    }

    // In the default case we skip the field
    if (!first) {
        out <<
            indent() << "default:" << endl <<
            indent() << "  if err := iprot.Skip(fieldTypeId); err != nil {" << endl <<
            indent() << "    return err" << endl <<
            indent() << "  }" << endl <<
            indent() << "}" << endl;
    }

    // Read field end marker
    out <<
        indent() << "if err := iprot.ReadFieldEnd(); err != nil {" << endl <<
        indent() << "  return err" << endl <<
        indent() << "}" << endl;
    indent_down();
    out <<
        indent() << "}" << endl <<
        indent() << "if err := iprot.ReadStructEnd(); err != nil {" << endl <<
        indent() << "  return fmt.Errorf(\"%T read struct end error: %s\", p, err)" << endl <<
        indent() << "}" << endl <<
        indent() << "return nil" << endl;
    indent_down();
    out <<
        indent() << "}" << endl << endl;

    for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
        string field_type_name(publicize((*f_iter)->get_type()->get_name()));
        string field_name(publicize((*f_iter)->get_name()));
        string field_method_prefix("readField");
        int32_t field_id = (*f_iter)->get_key();

        if (field_id < 0) {
            field_method_prefix += "_";
            field_id *= -1;
        }

        out <<
            indent() << "func (p *" << tstruct_name << ")  " << field_method_prefix << field_id << "(iprot thrift.TProtocol) error {" << endl;
        indent_up();
        generate_deserialize_field(out, *f_iter, false, "p.");
        indent_down();
        out <<
            indent() << "  return nil" << endl <<
            indent() << "}" << endl << endl;
    }
}

void t_go_generator::generate_go_struct_writer(ofstream& out,
        t_struct* tstruct,
        const string& tstruct_name,
        bool is_result)
{
    string name(tstruct->get_name());
    const vector<t_field*>& fields = tstruct->get_sorted_members();
    vector<t_field*>::const_iterator f_iter;
    indent(out) <<
                "func (p *" << tstruct_name << ") Write(oprot thrift.TProtocol) error {" << endl;
    indent_up();
    out <<
        indent() << "if err := oprot.WriteStructBegin(\"" << name << "\"); err != nil {" << endl <<
        indent() << "  return fmt.Errorf(\"%T write struct begin error: %s\", p, err) }" << endl;

    string field_name;
    string escape_field_name;
    //t_const_value* field_default_value;
    t_field::e_req field_required;
    bool field_can_be_nil = false;
    int32_t field_id = -1;

    if (is_result && fields.size()) {
        out <<
            indent() << "switch {" << endl;
        vector<t_field*>::const_reverse_iterator fr_iter;

        for (fr_iter = fields.rbegin(); fr_iter != fields.rend(); ++fr_iter) {
            string field_method_prefix("writeField");
            field_name = (*fr_iter)->get_name();
            field_id = (*fr_iter)->get_key();

            if (field_id < 0) {
                field_method_prefix += "_";
                field_id *= -1;
            }

            if (can_be_nil((*fr_iter)->get_type()) && field_id != 0) {
                out <<
                    indent() << "case p." << publicize(variable_name_to_go_name(field_name)) << " != nil:" << endl <<
                    indent() << "  if err := p." << field_method_prefix << field_id << "(oprot); err != nil { return err }" << endl;
            } else {
                out <<
                    indent() << "default:" << endl <<
                    indent() << "  if err := p." << field_method_prefix << field_id << "(oprot); err != nil { return err }" << endl;
            }
        }

        out <<
            indent() << "}" << endl;
    } else {
        for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
            string field_method_prefix("writeField");
            field_name = (*f_iter)->get_name();
            escape_field_name = escape_string(field_name);
            field_id = (*f_iter)->get_key();

            if (field_id < 0) {
                field_method_prefix += "_";
                field_id *= -1;
            }

            out <<
                indent() << "if err := p." << field_method_prefix << field_id << "(oprot); err != nil { return err }" << endl;

        }
    }

    // Write the struct map
    out <<
        indent() << "if err := oprot.WriteFieldStop(); err != nil {" << endl <<
        indent() << "  return fmt.Errorf(\"%T write field stop error: %s\", err) }" << endl <<
        indent() << "if err := oprot.WriteStructEnd(); err != nil {" << endl <<
        indent() << "  return fmt.Errorf(\"%T write struct stop error: %s\", err) }" << endl <<
        indent() << "return nil" << endl;
    indent_down();
    out <<
        indent() << "}" << endl << endl;

    for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
        string field_method_prefix("writeField");
        field_id = (*f_iter)->get_key();
        field_name = (*f_iter)->get_name();
        escape_field_name = escape_string(field_name);
        //field_default_value = (*f_iter)->get_value();
        field_required = (*f_iter)->get_req();
        field_can_be_nil = can_be_nil((*f_iter)->get_type());

        if (field_id < 0) {
            field_method_prefix += "_";
            field_id *= -1;
        }

        out <<
            indent() << "func (p *" << tstruct_name << ") " << field_method_prefix << field_id << "(oprot thrift.TProtocol) (err error) {" << endl;
        indent_up();

        // Write field header
        if (field_can_be_nil) {
            out <<
                indent() << "if p." << publicize(variable_name_to_go_name(field_name)) << " != nil {" << endl;
            indent_up();
        }

        if (field_required == t_field::T_OPTIONAL || (*f_iter)->get_type()->is_enum()) {
            out <<
                indent() << "if p.IsSet" << publicize(variable_name_to_go_name(field_name)) << "() {" << endl;
            indent_up();
        }

        out <<
            indent() << "if err := oprot.WriteFieldBegin(\"" <<
            escape_field_name << "\", " <<
            type_to_enum((*f_iter)->get_type()) << ", " <<
            field_id << "); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"%T write field begin error " << field_id << ":" << escape_field_name << ": %s\", p, err); }" << endl;

        // Write field contents
        generate_serialize_field(out, *f_iter, "p.");


        // Write field closer
        out <<
            indent() << "if err := oprot.WriteFieldEnd(); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"%T write field end error " << field_id << ":" << escape_field_name << ": %s\", p, err); }" << endl;

        if (field_required == t_field::T_OPTIONAL || (*f_iter)->get_type()->is_enum()) {
            indent_down();
            out <<
                indent() << "}" << endl;
        }

        if (field_can_be_nil) {
            indent_down();
            out <<
                indent() << "}" << endl;
        }

        indent_down();
        out <<
            indent() << "  return err" << endl <<
            indent() << "}" << endl << endl;
    }
}

/**
 * Generates a thrift service.
 *
 * @param tservice The service definition
 */
void t_go_generator::generate_service(t_service* tservice)
{
    string f_service_name = package_dir_ + "/" + underscore(service_name_) + ".go";
    f_service_.open(f_service_name.c_str());
    f_service_ <<
               go_autogen_comment() <<
               go_package() <<
               render_includes(true);

    generate_service_interface(tservice);
    generate_service_client(tservice);
    generate_service_server(tservice);
    generate_service_helpers(tservice);
    generate_service_remote(tservice);
    // Close service file
    f_service_ << endl;
    f_service_.close();
    format_go_output(f_service_name);
}

/**
 * Generates helper functions for a service.
 *
 * @param tservice The service to generate a header definition for
 */
void t_go_generator::generate_service_helpers(t_service* tservice)
{
    vector<t_function*> functions = tservice->get_functions();
    vector<t_function*>::iterator f_iter;
    f_service_ <<
               "// HELPER FUNCTIONS AND STRUCTURES" << endl << endl;

    for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
        t_struct* ts = (*f_iter)->get_arglist();
        generate_go_struct_definition(f_service_, ts, false);
        generate_go_function_helpers(*f_iter);
    }
}

/**
 * Generates a struct and helpers for a function.
 *
 * @param tfunction The function
 */
void t_go_generator::generate_go_function_helpers(t_function* tfunction)
{
    if (true || !tfunction->is_oneway()) {
        t_struct result(program_, tfunction->get_name() + "_result");
        t_field success(tfunction->get_returntype(), "success", 0);

        if (!tfunction->get_returntype()->is_void()) {
            result.append(&success);
        }

        t_struct* xs = tfunction->get_xceptions();
        const vector<t_field*>& fields = xs->get_members();
        vector<t_field*>::const_iterator f_iter;

        for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
            result.append(*f_iter);
        }

        generate_go_struct_definition(f_service_, &result, false, true);
    }
}

/**
 * Generates a service interface definition.
 *
 * @param tservice The service to generate a header definition for
 */
void t_go_generator::generate_service_interface(t_service* tservice)
{
    string extends = "";
    string extends_if = "";
    string serviceName(publicize(tservice->get_name()));
    string interfaceName = serviceName;

    if (tservice->get_extends() != NULL) {
        extends = type_name(tservice->get_extends());
        size_t index = extends.rfind(".");

        if (index != string::npos) {
            extends_if = "\n" + indent() + "  " + extends.substr(0, index + 1) + publicize(extends.substr(index + 1)) + "\n";
        } else {
            extends_if = "\n" + indent() + publicize(extends) + "\n";
        }
    }

    f_service_ <<
               indent() << "type " << interfaceName << " interface {" << extends_if;
    indent_up();
    generate_go_docstring(f_service_, tservice);
    vector<t_function*> functions = tservice->get_functions();

    if (!functions.empty()) {
        f_service_ << endl;
        vector<t_function*>::iterator f_iter;

        for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
            generate_go_docstring(f_service_, (*f_iter));
            f_service_ <<
                       indent() << function_signature_if(*f_iter, "", true) << endl;
        }
    }

    indent_down();
    f_service_ <<
               indent() << "}" << endl << endl;
}

/**
 * Generates a service client definition.
 *
 * @param tservice The service to generate a server for.
 */
void t_go_generator::generate_service_client(t_service* tservice)
{
    string extends = "";
    string extends_field = "";
    string extends_client = "";
    string extends_client_new = "";
    string serviceName(publicize(tservice->get_name()));

    if (tservice->get_extends() != NULL) {
        extends = type_name(tservice->get_extends());
        size_t index = extends.rfind(".");

        if (index != string::npos) {
            extends_client = extends.substr(0, index + 1) + publicize(extends.substr(index + 1)) + "Client";
            extends_client_new = extends.substr(0, index + 1) + "New" + publicize(extends.substr(index + 1)) + "Client";
        } else {
            extends_client = publicize(extends) + "Client";
            extends_client_new = "New" + extends_client;
        }
    }

    extends_field = extends_client.substr(extends_client.find(".") + 1);

    generate_go_docstring(f_service_, tservice);
    f_service_ <<
               indent() << "type " << serviceName << "Client struct {" << endl;
    indent_up();

    if (!extends_client.empty()) {
        f_service_ <<
                   indent() << "*" << extends_client << endl;
    } else {
        f_service_ <<
                   indent() << "Transport thrift.TTransport" << endl <<
                   indent() << "ProtocolFactory thrift.TProtocolFactory" << endl <<
                   indent() << "InputProtocol thrift.TProtocol" << endl <<
                   indent() << "OutputProtocol thrift.TProtocol" << endl <<
                   indent() << "SeqId int32" << endl /*<<
      indent() << "reqs map[int32]Deferred" << endl*/;
    }

    indent_down();
    f_service_ <<
               indent() << "}" << endl << endl;
    // Constructor function
    f_service_ <<
               indent() << "func New" << serviceName << "ClientFactory(t thrift.TTransport, f thrift.TProtocolFactory) *" << serviceName << "Client {" << endl;
    indent_up();
    f_service_ <<
               indent() << "return &" << serviceName << "Client";

    if (!extends.empty()) {
        f_service_ <<
                   "{" << extends_field << ": " << extends_client_new << "Factory(t, f)}";
    } else {
        indent_up();
        f_service_ << "{Transport: t," << endl <<
                   indent() << "ProtocolFactory: f," << endl <<
                   indent() << "InputProtocol: f.GetProtocol(t)," << endl <<
                   indent() << "OutputProtocol: f.GetProtocol(t)," << endl <<
                   indent() << "SeqId: 0," << endl /*<<
      indent() << "Reqs: make(map[int32]Deferred)" << endl*/;
        indent_down();
        f_service_ <<
                   indent() << "}" << endl;
    }

    indent_down();
    f_service_ <<
               indent() << "}" << endl << endl;
    // Constructor function
    f_service_ <<
               indent() << "func New" << serviceName << "ClientProtocol(t thrift.TTransport, iprot thrift.TProtocol, oprot thrift.TProtocol) *" << serviceName << "Client {" << endl;
    indent_up();
    f_service_ <<
               indent() << "return &" << serviceName << "Client";

    if (!extends.empty()) {
        f_service_ <<
                   "{" << extends_field << ": " << extends_client_new << "Protocol(t, iprot, oprot)}" << endl;
    } else {
        indent_up();
        f_service_ << "{Transport: t," << endl <<
                   indent() << "ProtocolFactory: nil," << endl <<
                   indent() << "InputProtocol: iprot," << endl <<
                   indent() << "OutputProtocol: oprot," << endl <<
                   indent() << "SeqId: 0," << endl /*<<
      indent() << "Reqs: make(map[int32]interface{})" << endl*/;
        indent_down();
        f_service_ <<
                   indent() << "}" << endl;
    }

    indent_down();
    f_service_ <<
               indent() << "}" << endl << endl;
    // Generate client method implementations
    vector<t_function*> functions = tservice->get_functions();
    vector<t_function*>::const_iterator f_iter;

    for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
        t_struct* arg_struct = (*f_iter)->get_arglist();
        const vector<t_field*>& fields = arg_struct->get_members();
        vector<t_field*>::const_iterator fld_iter;
        string funname = publicize((*f_iter)->get_name());
        // Open function
        generate_go_docstring(f_service_, (*f_iter));
        f_service_ <<
                   indent() << "func (p *" << serviceName << "Client) " << function_signature_if(*f_iter, "", true) << " {" << endl;
        indent_up();
        /*
        f_service_ <<
          indent() << "p.SeqId += 1" << endl;
        if (!(*f_iter)->is_oneway()) {
          f_service_ <<
            indent() << "d := defer.Deferred()" << endl <<
            indent() << "p.Reqs[p.SeqId] = d" << endl;
        }
        */
        f_service_ <<
                   indent() << "if err = p.send" << funname << "(";
        bool first = true;

        for (fld_iter = fields.begin(); fld_iter != fields.end(); ++fld_iter) {
            if (first) {
                first = false;
            } else {
                f_service_ << ", ";
            }

            f_service_ << variable_name_to_go_name((*fld_iter)->get_name());
        }

        f_service_ << "); err != nil { return }" << endl;

        if (!(*f_iter)->is_oneway()) {
            f_service_ <<
                       indent() << "return p.recv" << funname << "()" << endl;
        } else {
            f_service_ <<
                       indent() << "return" << endl;
        }

        indent_down();
        f_service_ <<
                   indent() << "}" << endl << endl <<
                   indent() << "func (p *" << serviceName << "Client) send" << function_signature(*f_iter) << "(err error) {" << endl;
        indent_up();
        std::string argsname = publicize((*f_iter)->get_name()) + "Args";
        // Serialize the request header
        string args(tmp("args"));
        f_service_ <<
                   indent() << "oprot := p.OutputProtocol" << endl <<
                   indent() << "if oprot == nil {" << endl <<
                   indent() << "  oprot = p.ProtocolFactory.GetProtocol(p.Transport)" << endl <<
                   indent() << "  p.OutputProtocol = oprot" << endl <<
                   indent() << "}" << endl <<
                   indent() << "p.SeqId++" << endl <<
                   indent() << "oprot.WriteMessageBegin(\"" << (*f_iter)->get_name() << "\", thrift.CALL, p.SeqId)" << endl <<
                   indent() << args << " := New" << publicize(argsname) << "()" << endl;

        for (fld_iter = fields.begin(); fld_iter != fields.end(); ++fld_iter) {
            f_service_ <<
                       indent() << args << "." << publicize(variable_name_to_go_name((*fld_iter)->get_name())) << " = " << variable_name_to_go_name((*fld_iter)->get_name()) << endl;
        }

        // Write to the stream
        f_service_ <<
                   indent() << "err = " << args << ".Write(oprot)" << endl <<
                   indent() << "oprot.WriteMessageEnd()" << endl <<
                   indent() << "oprot.Flush()" << endl <<
                   indent() << "return" << endl;
        indent_down();
        f_service_ <<
                   indent() << "}" << endl << endl;

        if (true) { //!(*f_iter)->is_oneway() || true) {}
            std::string resultname = publicize((*f_iter)->get_name()) + "Result";
            // Open function
            f_service_ << endl <<
                       indent() << "func (p *" << serviceName << "Client) recv" << publicize((*f_iter)->get_name()) <<
                       "() (";

            if (!(*f_iter)->get_returntype()->is_void()) {
                f_service_ <<
                           "value " << type_to_go_type((*f_iter)->get_returntype()) << ", ";
            }

            t_struct* exceptions = (*f_iter)->get_xceptions();
            string errs = argument_list(exceptions);

            if (errs.size()) {
                f_service_ << errs << ", ";
            }

            f_service_ <<
                       "err error) {" << endl;
            indent_up();
            // TODO(mcslee): Validate message reply here, seq ids etc.
            string result(tmp("result"));
            string error(tmp("error"));
            string error2(tmp("error"));
            f_service_ <<
                       indent() << "iprot := p.InputProtocol" << endl <<
                       indent() << "if iprot == nil {" << endl <<
                       indent() << "  iprot = p.ProtocolFactory.GetProtocol(p.Transport)" << endl <<
                       indent() << "  p.InputProtocol = iprot" << endl <<
                       indent() << "}" << endl <<
                       indent() << "_, mTypeId, seqId, err := iprot.ReadMessageBegin()" << endl <<
                       indent() << "if err != nil {" << endl <<
                       indent() << "  return" << endl <<
                       indent() << "}" << endl <<
                       indent() << "if mTypeId == thrift.EXCEPTION {" << endl <<
                       indent() << "  " << error << " := thrift.NewTApplicationException(thrift.UNKNOWN_APPLICATION_EXCEPTION, \"Unknown Exception\")" << endl <<
                       indent() << "  var " << error2 << " error" << endl <<
                       indent() << "  " << error2 << ", err = " << error << ".Read(iprot)" << endl <<
                       indent() << "  if err != nil {" << endl <<
                       indent() << "    return" << endl <<
                       indent() << "  }" << endl <<
                       indent() << "  if err = iprot.ReadMessageEnd(); err != nil {" << endl <<
                       indent() << "    return" << endl <<
                       indent() << "  }" << endl <<
                       indent() << "  err = " << error2 << endl <<
                       indent() << "  return" << endl <<
                       indent() << "}" << endl <<
                       indent() << "if p.SeqId != seqId {" << endl <<
                       indent() << "  err = thrift.NewTApplicationException(thrift.BAD_SEQUENCE_ID, \"ping failed: out of sequence response\")" << endl <<
                       indent() << "  return" << endl <<
                       indent() << "}" << endl <<
                       indent() << result << " := New" << publicize(resultname) << "()" << endl <<
                       indent() << "err = " << result << ".Read(iprot)" << endl <<
                       indent() << "iprot.ReadMessageEnd()" << endl;

            // Careful, only return _result if not a void function
            if (!(*f_iter)->get_returntype()->is_void()) {
                f_service_ <<
                           indent() << "value = " << result << ".Success" << endl;
            }

            t_struct* xs = (*f_iter)->get_xceptions();
            const std::vector<t_field*>& xceptions = xs->get_members();
            vector<t_field*>::const_iterator x_iter;

            for (x_iter = xceptions.begin(); x_iter != xceptions.end(); ++x_iter) {
                f_service_ <<
                           indent() << "if " << result << "." << publicize((*x_iter)->get_name()) << " != nil {" << endl <<
                           indent() << "  " << (*x_iter)->get_name() << " = " << result << "." << publicize((*x_iter)->get_name()) << endl <<
                           indent() << "}" << endl;
            }

            f_service_ <<
                       indent() << "return" << endl;
            // Close function
            indent_down();
            f_service_ <<
                       indent() << "}" << endl << endl;
        }
    }

    //indent_down();
    f_service_ <<
               endl;
}

/**
 * Generates a command line tool for making remote requests
 *
 * @param tservice The service to generate a remote for.
 */
void t_go_generator::generate_service_remote(t_service* tservice)
{
    vector<t_function*> functions = tservice->get_functions();
    t_service* parent = tservice->get_extends();

    // collect inherited functions
    while (parent != NULL) {
        vector<t_function*> p_functions = parent->get_functions();
        functions.insert(functions.end(), p_functions.begin(), p_functions.end());
        parent = parent->get_extends();
    }

    vector<t_function*>::iterator f_iter;
    string f_remote_name = package_dir_ + "/" + underscore(service_name_) + "-remote/" + underscore(service_name_) + "-remote.go";
    ofstream f_remote;
    f_remote.open(f_remote_name.c_str());
    string service_module = get_real_go_module(program_);
    string::size_type loc;

    while ((loc = service_module.find(".")) != string::npos) {
        service_module.replace(loc, 1, 1, '/');
    }

    f_remote <<
             go_autogen_comment() <<
             indent() << "package main" << endl << endl <<
             indent() << "import (" << endl <<
             indent() << "        \"flag\"" << endl <<
             indent() << "        \"fmt\"" << endl <<
             indent() << "        \"math\"" << endl <<
             indent() << "        \"net\"" << endl <<
             indent() << "        \"net/url\"" << endl <<
             indent() << "        \"os\"" << endl <<
             indent() << "        \"strconv\"" << endl <<
             indent() << "        \"strings\"" << endl <<
             indent() << "        \"" + gen_thrift_import_ + "\"" << endl <<
             indent() << "        \"" << service_module << "\"" << endl <<
             indent() << ")" << endl <<
             indent() << endl <<
             indent() << "func Usage() {" << endl <<
             indent() << "  fmt.Fprintln(os.Stderr, \"Usage of \", os.Args[0], \" [-h host:port] [-u url] [-f[ramed]] function [arg1 [arg2...]]:\")" << endl <<
             indent() << "  flag.PrintDefaults()" << endl <<
             indent() << "  fmt.Fprintln(os.Stderr, \"\\nFunctions:\")" << endl;

    for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
        f_remote <<
                 "  fmt.Fprintln(os.Stderr, \"  " << (*f_iter)->get_returntype()->get_name() << " " << (*f_iter)->get_name() << "(";
        t_struct* arg_struct = (*f_iter)->get_arglist();
        const std::vector<t_field*>& args = arg_struct->get_members();
        vector<t_field*>::const_iterator a_iter;
        int num_args = args.size();
        bool first = true;

        for (int i = 0; i < num_args; ++i) {
            if (first) {
                first = false;
            } else {
                f_remote << ", ";
            }

            f_remote <<
                     args[i]->get_type()->get_name() << " " << args[i]->get_name();
        }

        f_remote << ")\")" << endl;
    }

    f_remote <<
             indent() << "  fmt.Fprintln(os.Stderr)" << endl <<
             indent() << "  os.Exit(0)" << endl <<
             indent() << "}" << endl <<
             indent() << endl <<
             indent() << "func main() {" << endl;
    indent_up();
    f_remote <<
             indent() << "flag.Usage = Usage" << endl <<
             indent() << "var host string" << endl <<
             indent() << "var port int" << endl <<
             indent() << "var protocol string" << endl <<
             indent() << "var urlString string" << endl <<
             indent() << "var framed bool" << endl <<
             indent() << "var useHttp bool" << endl <<
             indent() << "var parsedUrl url.URL" << endl <<
             indent() << "var trans thrift.TTransport" << endl <<
             indent() << "_ = math.MinInt32 // will become unneeded eventually" << endl <<
             indent() << "_ = strconv.Atoi" << endl <<
             indent() << "flag.Usage = Usage" << endl <<
             indent() << "flag.StringVar(&host, \"h\", \"localhost\", \"Specify host and port\")" << endl <<
             indent() << "flag.IntVar(&port, \"p\", 9090, \"Specify port\")" << endl <<
             indent() << "flag.StringVar(&protocol, \"P\", \"binary\", \"Specify the protocol (binary, compact, simplejson, json)\")" << endl <<
             indent() << "flag.StringVar(&urlString, \"u\", \"\", \"Specify the url\")" << endl <<
             indent() << "flag.BoolVar(&framed, \"framed\", false, \"Use framed transport\")" << endl <<
             indent() << "flag.BoolVar(&useHttp, \"http\", false, \"Use http\")" << endl <<
             indent() << "flag.Parse()" << endl <<
             indent() << endl <<
             indent() << "if len(urlString) > 0 {" << endl <<
             indent() << "  parsedUrl, err := url.Parse(urlString)" << endl <<
             indent() << "  if err != nil {" << endl <<
             indent() << "    fmt.Fprintln(os.Stderr, \"Error parsing URL: \", err)" << endl <<
             indent() << "    flag.Usage()" << endl <<
             indent() << "  }" << endl <<
             indent() << "  host = parsedUrl.Host" << endl <<
             indent() << "  useHttp = len(parsedUrl.Scheme) <= 0 || parsedUrl.Scheme == \"http\"" << endl <<
             indent() << "} else if useHttp {" << endl <<
             indent() << "  _, err := url.Parse(fmt.Sprint(\"http://\", host, \":\", port))" << endl <<
             indent() << "  if err != nil {" << endl <<
             indent() << "    fmt.Fprintln(os.Stderr, \"Error parsing URL: \", err)" << endl <<
             indent() << "    flag.Usage()" << endl <<
             indent() << "  }" << endl <<
             indent() << "}" << endl <<
             indent() << endl <<
             indent() << "cmd := flag.Arg(0)" << endl <<
             indent() << "var err error" << endl <<
             indent() << "if useHttp {" << endl <<
             indent() << "  trans, err = thrift.NewTHttpClient(parsedUrl.String())" << endl <<
             indent() << "} else {" << endl <<
             indent() << "  portStr := fmt.Sprint(port)" << endl <<
             indent() << "  if strings.Contains(host, \":\") {" << endl <<
             indent() << "         host, portStr, err = net.SplitHostPort(host)" << endl <<
             indent() << "         if err != nil {" << endl <<
             indent() << "                 fmt.Fprintln(os.Stderr, \"error with host:\", err)" << endl <<
             indent() << "                 os.Exit(1)" << endl <<
             indent() << "         }" << endl <<
             indent() << "  }" << endl <<
             indent() << "  trans, err = thrift.NewTSocket(net.JoinHostPort(host, portStr))" << endl <<
             indent() << "  if err != nil {" << endl <<
             indent() << "    fmt.Fprintln(os.Stderr, \"error resolving address:\", err)" << endl <<
             indent() << "    os.Exit(1)" << endl <<
             indent() << "  }" << endl <<
             indent() << "  if framed {" << endl <<
             indent() << "    trans = thrift.NewTFramedTransport(trans)" << endl <<
             indent() << "  }" << endl <<
             indent() << "}" << endl <<
             indent() << "if err != nil {" << endl <<
             indent() << "  fmt.Fprintln(os.Stderr, \"Error creating transport\", err)" << endl <<
             indent() << "  os.Exit(1)" << endl <<
             indent() << "}" << endl <<
             indent() << "defer trans.Close()" << endl <<
             indent() << "var protocolFactory thrift.TProtocolFactory" << endl <<
             indent() << "switch protocol {" << endl <<
             indent() << "case \"compact\":" << endl <<
             indent() << "  protocolFactory = thrift.NewTCompactProtocolFactory()" << endl <<
             indent() << "  break" << endl <<
             indent() << "case \"simplejson\":" << endl <<
             indent() << "  protocolFactory = thrift.NewTSimpleJSONProtocolFactory()" << endl <<
             indent() << "  break" << endl <<
             indent() << "case \"json\":" << endl <<
             indent() << "  protocolFactory = thrift.NewTJSONProtocolFactory()" << endl <<
             indent() << "  break" << endl <<
             indent() << "case \"binary\", \"\":" << endl <<
             indent() << "  protocolFactory = thrift.NewTBinaryProtocolFactoryDefault()" << endl <<
             indent() << "  break" << endl <<
             indent() << "default:" << endl <<
             indent() << "  fmt.Fprintln(os.Stderr, \"Invalid protocol specified: \", protocol)" << endl <<
             indent() << "  Usage()" << endl <<
             indent() << "  os.Exit(1)" << endl <<
             indent() << "}" << endl <<
             indent() << "client := " << package_name_ << ".New" << publicize(service_name_) << "ClientFactory(trans, protocolFactory)" << endl <<
             indent() << "if err := trans.Open(); err != nil {" << endl <<
             indent() << "  fmt.Fprintln(os.Stderr, \"Error opening socket to \", host, \":\", port, \" \", err)" << endl <<
             indent() << "  os.Exit(1)" << endl <<
             indent() << "}" << endl <<
             indent() << endl <<
             indent() << "switch cmd {" << endl;

    for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
        t_struct* arg_struct = (*f_iter)->get_arglist();
        const std::vector<t_field*>& args = arg_struct->get_members();
        vector<t_field*>::const_iterator a_iter;
        int num_args = args.size();
        string funcName((*f_iter)->get_name());
        string pubName(publicize(funcName));
        f_remote <<
                 indent() << "case \"" << escape_string(funcName) << "\":" << endl;
        indent_up();
        f_remote <<
                 indent() << "if flag.NArg() - 1 != " << num_args << " {" << endl <<
                 indent() << "  fmt.Fprintln(os.Stderr, \"" << escape_string(pubName) << " requires " << num_args << " args\")" << endl <<
                 indent() << "  flag.Usage()" << endl <<
                 indent() << "}" << endl;

        for (int i = 0; i < num_args; ++i) {
            int flagArg = i + 1;
            t_type* the_type(args[i]->get_type());
            t_type* the_type2(get_true_type(the_type));

            if (the_type2->is_enum()) {
                f_remote <<
                         indent() << "tmp" << i << ", err := (strconv.Atoi(flag.Arg(" << flagArg << ")))" << endl <<
                         indent() << "if err != nil {" << endl <<
                         indent() << "  Usage()" << endl <<
                         indent() << " return" << endl <<
                         indent() << "}" << endl <<
                         indent() << "argvalue" << i << " := " << package_name_ << "." << publicize(the_type->get_name()) << "(tmp" << i << ")" << endl;
            } else if (the_type2->is_base_type()) {
                t_base_type::t_base e = ((t_base_type*)the_type2)->get_base();
                string err(tmp("err"));

                switch (e) {
                case t_base_type::TYPE_VOID:
                    break;

                case t_base_type::TYPE_STRING:
                    f_remote <<
                             indent() << "argvalue" << i << " := flag.Arg(" << flagArg << ")" << endl;
                    break;

                case t_base_type::TYPE_BOOL:
                    f_remote <<
                             indent() << "argvalue" << i << " := flag.Arg(" << flagArg << ") == \"true\"" << endl;
                    break;

                case t_base_type::TYPE_BYTE:
                    f_remote <<
                             indent() << "tmp" << i << ", " << err << " := (strconv.Atoi(flag.Arg(" << flagArg << ")))" << endl <<
                             indent() << "if " << err << " != nil {" << endl <<
                             indent() << "  Usage()" << endl <<
                             indent() << "  return" << endl <<
                             indent() << "}" << endl <<
                             indent() << "argvalue" << i << " := byte(tmp" << i << ")" << endl;
                    break;

                case t_base_type::TYPE_I16:
                    f_remote <<
                             indent() << "tmp" << i << ", " << err << " := (strconv.Atoi(flag.Arg(" << flagArg << ")))" << endl <<
                             indent() << "if " << err << " != nil {" << endl <<
                             indent() << "  Usage()" << endl <<
                             indent() << "  return" << endl <<
                             indent() << "}" << endl <<
                             indent() << "argvalue" << i << " := byte(tmp" << i << ")" << endl;
                    break;

                case t_base_type::TYPE_I32:
                    f_remote <<
                             indent() << "tmp" << i << ", " << err << " := (strconv.Atoi(flag.Arg(" << flagArg << ")))" << endl <<
                             indent() << "if " << err << " != nil {" << endl <<
                             indent() << "  Usage()" << endl <<
                             indent() << "  return" << endl <<
                             indent() << "}" << endl <<
                             indent() << "argvalue" << i << " := int32(tmp" << i << ")" << endl;
                    break;

                case t_base_type::TYPE_I64:
                    f_remote <<
                             indent() << "argvalue" << i << ", " << err << " := (strconv.ParseInt(flag.Arg(" << flagArg << "), 10, 64))" << endl <<
                             indent() << "if " << err << " != nil {" << endl <<
                             indent() << "  Usage()" << endl <<
                             indent() << "  return" << endl <<
                             indent() << "}" << endl;
                    break;

                case t_base_type::TYPE_DOUBLE:
                    f_remote <<
                             indent() << "argvalue" << i << ", " << err << " := (strconv.ParseFloat(flag.Arg(" << flagArg << "), 64))" << endl <<
                             indent() << "if " << err << " != nil {" << endl <<
                             indent() << "  Usage()" << endl <<
                             indent() << "  return" << endl <<
                             indent() << "}" << endl;
                    break;

                default:
                    throw ("Invalid base type in generate_service_remote");
                }

                //f_remote << publicize(args[i]->get_name()) << "(strconv.Atoi(flag.Arg(" << flagArg << ")))";
            } else if (the_type2->is_struct()) {
                string arg(tmp("arg"));
                string mbTrans(tmp("mbTrans"));
                string err1(tmp("err"));
                string factory(tmp("factory"));
                string jsProt(tmp("jsProt"));
                string err2(tmp("err"));
                std::string tstruct_name(publicize(the_type->get_name()));
                f_remote <<
                         indent() << arg << " := flag.Arg(" << flagArg << ")" << endl <<
                         indent() << mbTrans << " := thrift.NewTMemoryBufferLen(len(" << arg << "))" << endl <<
                         indent() << "defer " << mbTrans << ".Close()" << endl <<
                         indent() << "_, " << err1 << " := " << mbTrans << ".WriteString(" << arg << ")" << endl <<
                         indent() << "if " << err1 << " != nil {" << endl <<
                         indent() << "  Usage()" << endl <<
                         indent() << "  return" << endl <<
                         indent() << "}" << endl <<
                         indent()  << factory << " := thrift.NewTSimpleJSONProtocolFactory()" << endl <<
                         indent() << jsProt << " := " << factory << ".GetProtocol(" << mbTrans << ")" << endl <<
                         indent() << "argvalue" << i << " := " << package_name_ << ".New" << tstruct_name << "()" << endl <<
                         indent() << err2 << " := argvalue" << i << ".Read(" << jsProt << ")" << endl <<
                         indent() << "if " << err2 << " != nil {" << endl <<
                         indent() << "  Usage()" << endl <<
                         indent() << "  return" << endl <<
                         indent() << "}" << endl;
            } else if (the_type2->is_container() || the_type2->is_xception()) {
                string arg(tmp("arg"));
                string mbTrans(tmp("mbTrans"));
                string err1(tmp("err"));
                string factory(tmp("factory"));
                string jsProt(tmp("jsProt"));
                string err2(tmp("err"));
                std::string argName(publicize(args[i]->get_name()));
                f_remote <<
                         indent() << arg << " := flag.Arg(" << flagArg << ")" << endl <<
                         indent() << mbTrans << " := thrift.NewTMemoryBufferLen(len(" << arg << "))" << endl <<
                         indent() << "defer " << mbTrans << ".Close()" << endl <<
                         indent() << "_, " << err1 << " := " << mbTrans << ".WriteString(" << arg << ")" << endl <<
                         indent() << "if " << err1 << " != nil { " << endl <<
                         indent() << "  Usage()" << endl <<
                         indent() << "  return" << endl <<
                         indent() << "}" << endl <<
                         indent() << factory << " := thrift.NewTSimpleJSONProtocolFactory()" << endl <<
                         indent() << jsProt << " := " << factory << ".GetProtocol(" << mbTrans << ")" << endl <<
                         indent() << "containerStruct" << i << " := " << package_name_ << ".New" << pubName << "Args()" << endl <<
                         indent() << err2 << " := containerStruct" << i << ".ReadField" << (i + 1) << "(" << jsProt << ")" << endl <<
                         indent() << "if " << err2 << " != nil {" << endl <<
                         indent() << "  Usage()" << endl <<
                         indent() << "  return" << endl <<
                         indent() << "}" << endl <<
                         indent() << "argvalue" << i << " := containerStruct" << i << "." << argName << endl;
            } else {
                throw ("Invalid argument type in generate_service_remote");
                string err1(tmp("err"));
                f_remote <<
                         indent() << "argvalue" << i << ", " << err1 << " := eval(flag.Arg(" << flagArg << "))" << endl <<
                         indent() << "if " << err1 << " != nil {" << endl <<
                         indent() << "  Usage()" << endl <<
                         indent() << "  return" << endl <<
                         indent() << "}" << endl;
            }

            if (the_type->is_typedef()) {
                f_remote <<
                         indent() << "value" << i << " := " << package_name_ << "." << publicize(the_type->get_name()) << "(argvalue" << i << ")" << endl;
            } else {
                f_remote <<
                         indent() << "value" << i << " := argvalue" << i << endl;
            }
        }

        f_remote <<
                 indent() << "fmt.Print(client." << pubName << "(";
        bool argFirst = true;

        for (int i = 0; i < num_args; ++i) {
            if (argFirst) {
                argFirst = false;
            } else {
                f_remote << ", ";
            }

            if (args[i]->get_type()->is_enum()) {
                f_remote << "value" << i;
            } else if (args[i]->get_type()->is_base_type()) {
                t_base_type::t_base e = ((t_base_type*)(args[i]->get_type()))->get_base();

                switch (e) {
                case t_base_type::TYPE_VOID:
                    break;

                case t_base_type::TYPE_STRING:
                case t_base_type::TYPE_BOOL:
                case t_base_type::TYPE_BYTE:
                case t_base_type::TYPE_I16:
                case t_base_type::TYPE_I32:
                case t_base_type::TYPE_I64:
                case t_base_type::TYPE_DOUBLE:
                    f_remote << "value" << i;
                    break;

                default:
                    throw ("Invalid base type in generate_service_remote");
                }

                //f_remote << publicize(args[i]->get_name()) << "(strconv.Atoi(flag.Arg(" << flagArg << ")))";
            } else {
                f_remote << "value" << i;
            }
        }

        f_remote <<
                 "))" << endl <<
                 indent() << "fmt.Print(\"\\n\")" << endl <<
                 indent() << "break" << endl;
        indent_down();
    }

    f_remote <<
             indent() << "case \"\":" << endl <<
             indent() << "  Usage()" << endl <<
             indent() << "  break" << endl <<
             indent() << "default:" << endl <<
             indent() << "  fmt.Fprintln(os.Stderr, \"Invalid function \", cmd)" << endl <<
             indent() << "}" << endl;
    indent_down();
    f_remote <<
             indent() << "}" << endl;
    // Close service file
    f_remote.close();
    format_go_output(f_remote_name);
#ifndef _MSC_VER
    // Make file executable, love that bitwise OR action
    chmod(f_remote_name.c_str(),
          S_IRUSR
          | S_IWUSR
          | S_IXUSR
#ifndef MINGW
          | S_IRGRP
          | S_IXGRP
          | S_IROTH
          | S_IXOTH
#endif
         );
#endif
}

/**
 * Generates a service server definition.
 *
 * @param tservice The service to generate a server for.
 */
void t_go_generator::generate_service_server(t_service* tservice)
{
    // Generate the dispatch methods
    vector<t_function*> functions = tservice->get_functions();
    vector<t_function*>::iterator f_iter;
    string extends = "";
    string extends_processor = "";
    string extends_processor_new = "";
    string serviceName(publicize(tservice->get_name()));

    if (tservice->get_extends() != NULL) {
        extends = type_name(tservice->get_extends());
        size_t index = extends.rfind(".");

        if (index != string::npos) {
            extends_processor = extends.substr(0, index + 1) + publicize(extends.substr(index + 1)) + "Processor";
            extends_processor_new = extends.substr(0, index + 1) + "New" + publicize(extends.substr(index + 1)) + "Processor";
        } else {
            extends_processor = publicize(extends) + "Processor";
            extends_processor_new = "New" + extends_processor;
        }
    }

    string pServiceName(privatize(serviceName));
    // Generate the header portion
    string self(tmp("self"));

    if (extends_processor.empty()) {
        f_service_ <<
                   indent() << "type " << serviceName << "Processor struct {" << endl <<
                   indent() << "  processorMap map[string]thrift.TProcessorFunction" << endl <<
                   indent() << "  handler " << serviceName << endl <<
                   indent() << "  listener thrift.THandlerListener" << endl <<
                   indent() << "}" << endl << endl <<
                   indent() << "func (p *" << serviceName << "Processor) AddToProcessorMap(key string, processor thrift.TProcessorFunction) {" << endl <<
                   indent() << "  p.processorMap[key] = processor" << endl <<
                   indent() << "}" << endl << endl <<
                   indent() << "func (p *" << serviceName << "Processor) GetProcessorFunction(key string) (processor thrift.TProcessorFunction, ok bool) {" << endl <<
                   indent() << "  processor, ok = p.processorMap[key]" << endl <<
                   indent() << "  return processor, ok" << endl <<
                   indent() << "}" << endl << endl <<
                   indent() << "func (p *" << serviceName << "Processor) ProcessorMap() map[string]thrift.TProcessorFunction {" << endl <<
                   indent() << "  return p.processorMap" << endl <<
                   indent() << "}" << endl << endl <<
                   indent() << "func New" << serviceName << "Processor(handler " << serviceName << ", listener thrift.THandlerListener) *" << serviceName << "Processor {" << endl <<
                   indent() << "  " << self << " := &" << serviceName << "Processor{handler:handler, listener:listener, processorMap:make(map[string]thrift.TProcessorFunction)}" << endl;

        for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
            string escapedFuncName(escape_string((*f_iter)->get_name()));
            f_service_ <<
                       indent() << "  " << self << ".processorMap[\"" << escapedFuncName << "\"] = &" << pServiceName << "Processor" << publicize((*f_iter)->get_name()) << "{handler:handler, listener:listener}" << endl;
        }

        string x(tmp("x"));
        f_service_ <<
                   indent() << "return " << self << endl <<
                   indent() << "}" << endl << endl <<
                   indent() << "func (p *" << serviceName << "Processor) Receive(request thrift.Request) (success bool, err thrift.TException) {" << endl <<
                   indent() << "  name := request.Name()" << endl <<
                   indent() << "  seqId := request.SeqId()" << endl <<
                   indent() << "  iprot := request.In()" << endl <<
                   indent() << "  oprot := request.Out()" << endl <<
                   indent() << "  if processor, ok := p.GetProcessorFunction(name); ok {" << endl <<
                   indent() << "    return processor.Process(request)" << endl <<
                   indent() << "  }" << endl <<
                   indent() << "  iprot.Skip(thrift.STRUCT)" << endl <<
                   indent() << "  iprot.ReadMessageEnd()" << endl <<
                   indent() << "  " << x << " := thrift.NewTApplicationException(thrift.UNKNOWN_METHOD, \"Unknown function \" + name)" << endl <<
                   indent() << "  oprot.WriteMessageBegin(name, thrift.EXCEPTION, seqId)" << endl <<
                   indent() << "  " << x << ".Write(oprot)" << endl <<
                   indent() << "  oprot.WriteMessageEnd()" << endl <<
                   indent() << "  oprot.Flush()" << endl <<
                   indent() << "  return false, " << x << endl <<
                   indent() << "" << endl <<
                   indent() << "}" << endl << endl;
    } else {
        f_service_ <<
                   indent() << "type " << serviceName << "Processor struct {" << endl <<
                   indent() << "  *" << extends_processor << endl <<
                   indent() << "}" << endl << endl <<
                   indent() << "func New" << serviceName << "Processor(handler " << serviceName << ", listener thrift.THandlerListener) *" << serviceName << "Processor {" << endl <<
                   indent() << "  " << self << " := &" << serviceName << "Processor{" << extends_processor_new << "(handler, listener)}" << endl;

        for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
            string escapedFuncName(escape_string((*f_iter)->get_name()));
            f_service_ <<
                       indent() << "  " << self << ".AddToProcessorMap(\"" << escapedFuncName << "\", &" << pServiceName << "Processor" << publicize((*f_iter)->get_name()) << "{handler:handler, listener:listener})" << endl;
        }

        f_service_ <<
                   indent() << "  return " << self << endl <<
                   indent() << "}" << endl << endl;
    }

    // Generate the process subfunctions
    for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
        generate_process_function(tservice, *f_iter);
    }

    f_service_ << endl;
}

/**
 * Generates a process function definition.
 *
 * @param tfunction The function to write a dispatcher for
 */
void t_go_generator::generate_process_function(t_service* tservice,
        t_function* tfunction)
{
    // Open function
    string processorName = privatize(tservice->get_name()) + "Processor" + publicize(tfunction->get_name());
    string argsname = publicize(tfunction->get_name()) + "Args";
    string resultname = publicize(tfunction->get_name()) + "Result";
    //t_struct* xs = tfunction->get_xceptions();
    //const std::vector<t_field*>& xceptions = xs->get_members();
    vector<t_field*>::const_iterator x_iter;
    f_service_ <<
               indent() << "type " << processorName << " struct {" << endl <<
               indent() << "  handler " << publicize(tservice->get_name()) << endl <<
               indent() << "  listener thrift.THandlerListener" << endl <<
               indent() << "}" << endl << endl <<
               indent() << "func (p *" << processorName << ") Process(request thrift.Request) (success bool, err thrift.TException) {" << endl;
    indent_up();
    f_service_ <<
               indent() << "seqId := request.SeqId()" << endl <<
               indent() << "iprot := request.In()" << endl <<
               indent() << "oprot := request.Out()" << endl <<
               indent() << "args := New" << argsname << "()" << endl <<
               indent() << "if err = args.Read(iprot); err != nil {" << endl <<
               indent() << "  iprot.ReadMessageEnd()" << endl <<
               indent() << "  x := thrift.NewTApplicationException(thrift.PROTOCOL_ERROR, err.Error())" << endl <<
               indent() << "  oprot.WriteMessageBegin(\"" << escape_string(tfunction->get_name()) << "\", thrift.EXCEPTION, seqId)" << endl <<
               indent() << "  x.Write(oprot)" << endl <<
               indent() << "  oprot.WriteMessageEnd()" << endl <<
               indent() << "  oprot.Flush()" << endl <<
               indent() << "  return" << endl <<
               indent() << "}" << endl <<
               indent() << "iprot.ReadMessageEnd()" << endl <<
               indent() << "if p.listener != nil {" << endl <<
               indent() << "  p.listener.PreHandle(request";

    {
        const vector<t_field*>& fields = tfunction->get_arglist()->get_members();
        for (vector<t_field*>::const_iterator f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
            f_service_ << ", args." << publicize(variable_name_to_go_name((*f_iter)->get_name()));
        }
    }
    
    f_service_ << ")" << endl <<
               indent() << "}" << endl <<
               indent() << "result := New" << resultname << "()" << endl;

    std::string result_args;

    f_service_ << indent() << "var callbackError error" << endl;
    f_service_ << indent() << "(func() {" << endl;
    indent_up();

    f_service_ << indent() << "defer (func() {" << endl;
    {
        Indent _(this);
        f_service_ << indent() << "if r := recover(); r != nil {" << endl;
        {
            Indent _(this);
            f_service_ << indent() << "callbackError = goerr.New(r, 0)" << endl;
        }
        f_service_ << indent() << "}" << endl;
    }
    f_service_ << indent() << "})()" << endl;

    if (!tfunction->is_oneway()) {
        if (!tfunction->get_returntype()->is_void()) {
            result_args += "result.Success, ";
        }

        const vector<t_field*>& fields = tfunction->get_xceptions()->get_members();
        for (vector<t_field*>::const_iterator f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
            result_args += "result.";
            result_args += publicize(variable_name_to_go_name((*f_iter)->get_name()));
            result_args += ", ";
        }
    }

    // Generate the function call
    t_struct* arg_struct = tfunction->get_arglist();
    const std::vector<t_field*>& fields = arg_struct->get_members();
    vector<t_field*>::const_iterator f_iter;
    f_service_ << indent() << result_args <<
               "callbackError = p.handler." << publicize(tfunction->get_name()) << "(";
    bool first = true;

    for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
        if (first) {
            first = false;
        } else {
            f_service_ << ", ";
        }

        f_service_ << "args." << publicize(variable_name_to_go_name((*f_iter)->get_name()));
    }
    f_service_ << ")" << endl;

    indent_down();
    f_service_ << indent() << "})()" << endl;

    f_service_ <<
               indent() << "if p.listener != nil {" << endl <<
               indent() << "  p.listener.PostHandle(request, " << result_args << "callbackError)" << endl <<
               indent() << "  defer p.listener.Completed(request, callbackError)" << endl <<
               indent() << "}" << endl <<
               indent() << "if callbackError != nil {" << endl <<
               indent() << "  x := thrift.NewTApplicationException(thrift.INTERNAL_ERROR, \"Internal error processing " << escape_string(tfunction->get_name()) << ": \" + callbackError.Error())" << endl <<
               indent() << "  oprot.WriteMessageBegin(\"" << escape_string(tfunction->get_name()) << "\", thrift.EXCEPTION, seqId)" << endl <<
               indent() << "  x.Write(oprot)" << endl <<
               indent() << "  oprot.WriteMessageEnd()" << endl <<
               indent() << "  oprot.Flush()" << endl <<
               indent() << "  return" << endl <<
               indent() << "}" << endl <<
               indent() << "if err2 := oprot.WriteMessageBegin(\"" << escape_string(tfunction->get_name()) << "\", thrift.REPLY, seqId); err2 != nil {" << endl <<
               indent() << "  err = err2" << endl <<
               indent() << "}" << endl <<
               indent() << "if err2 := result.Write(oprot); err == nil && err2 != nil {" << endl <<
               indent() << "  err = err2" << endl <<
               indent() << "}" << endl <<
               indent() << "if err2 := oprot.WriteMessageEnd(); err == nil && err2 != nil {" << endl <<
               indent() << "  err = err2" << endl <<
               indent() << "}" << endl <<
               indent() << "if err2 := oprot.Flush(); err == nil && err2 != nil {" << endl <<
               indent() << "  err = err2" << endl <<
               indent() << "}" << endl <<
               indent() << "if err != nil {" << endl <<
               indent() << "  return" << endl <<
               indent() << "}" << endl <<
               indent() << "return true, err" << endl;
    indent_down();
    f_service_ <<
               indent() << "}" << endl << endl;
    /*
    indent(f_service_) <<
        "func (p *" << publicize(tservice->get_name()) << "Client) WriteResultsSuccess" << publicize(tfunction->get_name()) <<
        "(success bool, result " << publicize(tfunction->get_name()) << "Result, seqid int32, oprot thrift.TProtocol) (err error) {" << endl;
    indent_up();
    f_service_ <<
      indent() << "result.Success = success" << endl <<
      indent() << "oprot.WriteMessageBegin(\"" << escape_string(tfunction->get_name()) << "\", thrift.REPLY, seqid)" << endl <<
      indent() << "result.Write(oprot)" << endl <<
      indent() << "oprot.WriteMessageEnd()" << endl <<
      indent() << "oprot.Flush()" << endl <<
      indent() << "return" << endl;
    indent_down();
    f_service_ <<
      indent() << "}" << endl << endl;
    */
    // Try block for a function with exceptions
    /*
    if (!tfunction->is_oneway() && xceptions.size() > 0) {
      indent(f_service_) <<
        "func (p *" << publicize(tservice->get_name()) << "Client) WriteResultsException" << publicize(tfunction->get_name()) <<
        "(error *" << publicize(tfunction->get_name()) << ", result *, seqid, oprot) (err error) {" << endl;
      indent_up();

      // Kinda absurd
      for (x_iter = xceptions.begin(); x_iter != xceptions.end(); ++x_iter) {
        f_service_ <<
          indent() << "except " << type_name((*x_iter)->get_type()) << ", " << (*x_iter)->get_name() << ":" << endl;
        if (!tfunction->is_oneway()) {
          indent_up();
          f_service_ <<
            indent() << "result." << (*x_iter)->get_name() << " = " << (*x_iter)->get_name() << endl;
          indent_down();
        } else {
          f_service_ <<
            indent() << "pass" << endl;
        }
      }
      f_service_ <<
        indent() << "err = oprot.WriteMessageBegin(\"" << escape_string(tfunction->get_name()) << "\", thrift.REPLY, seqid)" << endl <<
        indent() << "if err != nil { return err }" << endl <<
        indent() << "err = result.Write(oprot)" << endl <<
        indent() << "if err != nil { return err }" << endl <<
        indent() << "err = oprot.WriteMessageEnd()" << endl <<
        indent() << "if err != nil { return err }" << endl <<
        indent() << "err = oprot.Flush()" << endl <<
        indent() << "if err != nil { return err }" << endl;
      indent_down();
      f_service_ << "}" << endl << endl;
    }
    */
}

/**
 * Deserializes a field of any type.
 */
void t_go_generator::generate_deserialize_field(ofstream &out,
        t_field* tfield,
        bool declare,
        string prefix,
        bool inclass,
        bool coerceData)
{
    t_type* orig_type = tfield->get_type();
    t_type* type = get_true_type(orig_type);
    string name(prefix + publicize(variable_name_to_go_name(tfield->get_name())));

    if (type->is_void()) {
        throw "CANNOT GENERATE DESERIALIZE CODE FOR void TYPE: " + name;
    }

    if (type->is_struct() || type->is_xception()) {
        generate_deserialize_struct(out,
                                    (t_struct*)type,
                                    declare,
                                    name);
    } else if (type->is_container()) {
        generate_deserialize_container(out, type, declare, name);
    } else if (type->is_base_type() || type->is_enum()) {

        if (declare) {
            out << "var " << tfield->get_name() << " " << type_to_go_type(tfield->get_type()) << endl;
        }

        indent(out) <<
                    "if v, err := iprot.";

        if (type->is_base_type()) {
            t_base_type::t_base tbase = ((t_base_type*)type)->get_base();

            switch (tbase) {
            case t_base_type::TYPE_VOID:
                throw "compiler error: cannot serialize void field in a struct: " +
                name;
                break;

            case t_base_type::TYPE_STRING:
                if (((t_base_type*)type)->is_binary()) {
                    out << "ReadBinary()";
                } else {
                    out << "ReadString()";
                }

                break;

            case t_base_type::TYPE_BOOL:
                out << "ReadBool()";
                break;

            case t_base_type::TYPE_BYTE:
                out << "ReadByte()";
                break;

            case t_base_type::TYPE_I16:
                out << "ReadI16()";
                break;

            case t_base_type::TYPE_I32:
                out << "ReadI32()";
                break;

            case t_base_type::TYPE_I64:
                out << "ReadI64()";
                break;

            case t_base_type::TYPE_DOUBLE:
                out << "ReadDouble()";
                break;

            default:
                throw "compiler error: no Go name for base type " + t_base_type::t_base_name(tbase);
            }
        } else if (type->is_enum()) {
            out << "ReadI32()";
        }

        out << "; err != nil {" << endl <<
            indent() << "return fmt.Errorf(\"error reading field " <<
            tfield->get_key()  << ": %s\")" << endl;

        out << "} else {" << endl;
        string wrap;

        if (type->is_enum() || orig_type->is_typedef()) {
            wrap = publicize(type_name(orig_type));
        } else if (((t_base_type*)type)->get_base() == t_base_type::TYPE_BYTE) {
            wrap = "int8";
        }

        if (wrap == "") {
            indent(out) << name << " = v" << endl;
        } else {
            indent(out) << name << " = " << wrap << "(v)" << endl;
        }

        out << "}" << endl;
    } else {
        throw "INVALID TYPE IN generate_deserialize_field '" + type->get_name() + "' for field '" + tfield->get_name() + "'";
    }
}

/**
 * Generates an unserializer for a struct, calling read()
 */
void t_go_generator::generate_deserialize_struct(ofstream &out,
        t_struct* tstruct,
        bool declare,
        string prefix)
{
    string eq(" := ");

    if (!declare) {
        eq = " = ";
    }

    out <<
        indent() << prefix << eq << new_prefix(type_name(tstruct)) << "()" << endl <<
        indent() << "if err := " << prefix << ".Read(iprot); err != nil {" << endl <<
        indent() << "  return fmt.Errorf(\"%T error reading struct: %s\", " << prefix << ")" << endl <<
        indent() << "}" << endl;
}

/**
 * Serialize a container by writing out the header followed by
 * data and then a footer.
 */
void t_go_generator::generate_deserialize_container(ofstream &out,
        t_type* ttype,
        bool   declare,
        string prefix)
{
    string eq(" = ");

    if (declare) {
        eq = " := ";
    }

    // Declare variables, read header
    if (ttype->is_map()) {
        t_map* t = (t_map*)ttype;
        out <<
            indent() << "_, _, size, err := iprot.ReadMapBegin()" << endl <<
            indent() << "if err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error reading map begin: %s\")" << endl <<
            indent() << "}" << endl <<
            indent() << prefix << eq << "make(map[" << type_to_go_type(t->get_key_type()) << "]" <<  type_to_go_type(t->get_val_type()) << ", size)" << endl;
    } else if (ttype->is_set()) {
        t_set* t = (t_set*)ttype;
        out <<
            indent() << "_, size, err := iprot.ReadSetBegin()" << endl <<
            indent() << "if err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error reading set being: %s\")" << endl <<
            indent() << "}" << endl <<
            indent() << prefix << eq << "make(map[" << type_to_go_type(t->get_elem_type()) << "]bool, size)" << endl;
    } else if (ttype->is_list()) {
        t_list* t = (t_list*)ttype;
        out <<
            indent() << "_, size, err := iprot.ReadListBegin()" << endl <<
            indent() << "if err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error reading list being: %s\")" << endl <<
            indent() << "}" << endl <<
            indent() << prefix << eq << "make(" << type_to_go_type(t) << ", 0, size)" << endl;
    } else {
        throw "INVALID TYPE IN generate_deserialize_container '" + ttype->get_name() + "' for prefix '" + prefix + "'";
    }

    // For loop iterates over elements
    out <<
        indent() << "for i := 0; i < size; i ++ {" << endl;
    indent_up();

    if (ttype->is_map()) {
        generate_deserialize_map_element(out, (t_map*)ttype, declare, prefix);
    } else if (ttype->is_set()) {
        generate_deserialize_set_element(out, (t_set*)ttype, declare, prefix);
    } else if (ttype->is_list()) {
        generate_deserialize_list_element(out, (t_list*)ttype, declare, prefix);
    }

    indent_down();
    out <<
        indent() << "}" << endl;

    // Read container end
    if (ttype->is_map()) {
        out <<
            indent() << "if err := iprot.ReadMapEnd(); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error reading map end: %s\")" << endl <<
            indent() << "}" << endl;
    } else if (ttype->is_set()) {
        out <<
            indent() << "if err := iprot.ReadSetEnd(); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error reading set end: %s\")" << endl <<
            indent() << "}" << endl;
    } else if (ttype->is_list()) {
        out <<
            indent() << "if err := iprot.ReadListEnd(); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error reading list end: %s\")" << endl <<
            indent() << "}" << endl;
    }
}


/**
 * Generates code to deserialize a map
 */
void t_go_generator::generate_deserialize_map_element(ofstream &out,
        t_map* tmap,
        bool   declare,
        string prefix)
{
    string key = tmp("_key");
    string val = tmp("_val");
    t_field fkey(tmap->get_key_type(), key);
    t_field fval(tmap->get_val_type(), val);
    generate_deserialize_field(out, &fkey, true);
    generate_deserialize_field(out, &fval, true);
    indent(out) <<
                prefix << "[" << key << "] = " << val << endl;

}

/**
 * Write a set element
 */
void t_go_generator::generate_deserialize_set_element(ofstream &out,
        t_set* tset,
        bool   declare,
        string prefix)
{
    string elem = tmp("_elem");
    t_field felem(tset->get_elem_type(), elem);
    generate_deserialize_field(out, &felem, true, "");
    indent(out) <<
                prefix << "[" << elem << "] = true" << endl;
}

/**
 * Write a list element
 */
void t_go_generator::generate_deserialize_list_element(ofstream &out,
        t_list* tlist,
        bool   declare,
        string prefix)
{
    string elem = tmp("_elem");
    t_field felem(tlist->get_elem_type(), elem);
    generate_deserialize_field(out, &felem, true, "");
    indent(out) <<
                prefix << " = append(" << prefix << ", " << elem << ")" << endl;
}


/**
 * Serializes a field of any type.
 *
 * @param tfield The field to serialize
 * @param prefix Name to prepend to field name
 */
void t_go_generator::generate_serialize_field(ofstream &out,
        t_field* tfield,
        string prefix)
{
    t_type* type = get_true_type(tfield->get_type());
    string name(prefix + publicize(variable_name_to_go_name(tfield->get_name())));

    // Do nothing for void types
    if (type->is_void()) {
        throw "compiler error: cannot generate serialize for void type: " + name;
    }

    if (type->is_struct() || type->is_xception()) {
        generate_serialize_struct(out,
                                  (t_struct*)type,
                                  name);
    } else if (type->is_container()) {
        generate_serialize_container(out,
                                     type,
                                     name);
    } else if (type->is_base_type() || type->is_enum()) {
        indent(out) <<
                    "if err := oprot.";

        if (type->is_base_type()) {
            t_base_type::t_base tbase = ((t_base_type*)type)->get_base();

            switch (tbase) {
            case t_base_type::TYPE_VOID:
                throw
                "compiler error: cannot serialize void field in a struct: " + name;
                break;

            case t_base_type::TYPE_STRING:
                if (((t_base_type*)type)->is_binary()) {
                    out << "WriteBinary(" << name << ")";
                } else {
                    out << "WriteString(string(" << name << "))";
                }

                break;

            case t_base_type::TYPE_BOOL:
                out << "WriteBool(bool(" << name << "))";
                break;

            case t_base_type::TYPE_BYTE:
                out << "WriteByte(byte(" << name << "))";
                break;

            case t_base_type::TYPE_I16:
                out << "WriteI16(int16(" << name << "))";
                break;

            case t_base_type::TYPE_I32:
                out << "WriteI32(int32(" << name << "))";
                break;

            case t_base_type::TYPE_I64:
                out << "WriteI64(int64(" << name << "))";
                break;

            case t_base_type::TYPE_DOUBLE:
                out << "WriteDouble(float64(" << name << "))";
                break;

            default:
                throw "compiler error: no Go name for base type " + t_base_type::t_base_name(tbase);
            }
        } else if (type->is_enum()) {
            out << "WriteI32(int32(" << name << "))";
        }

        out << "; err != nil {" << endl
            << indent() << "return fmt.Errorf(\"%T." << escape_string(tfield->get_name())
            << " (" << tfield->get_key() << ") field write error: %s\", p) }" << endl;
    } else {
        throw "compiler error: Invalid type in generate_serialize_field '" + type->get_name() + "' for field '" + name + "'";
    }
}

/**
 * Serializes all the members of a struct.
 *
 * @param tstruct The struct to serialize
 * @param prefix  String prefix to attach to all fields
 */
void t_go_generator::generate_serialize_struct(ofstream &out,
        t_struct* tstruct,
        string prefix)
{
    out <<
        indent() << "if err := " << prefix << ".Write(oprot); err != nil {" << endl <<
        indent() << "  return fmt.Errorf(\"%T error writing struct: %s\", " << prefix << ")" << endl <<
        indent() << "}" << endl;
}

void t_go_generator::generate_serialize_container(ofstream &out,
        t_type* ttype,
        string prefix)
{
    if (ttype->is_map()) {
        out <<
            indent() << "if err := oprot.WriteMapBegin(" <<
            type_to_enum(((t_map*)ttype)->get_key_type()) << ", " <<
            type_to_enum(((t_map*)ttype)->get_val_type()) << ", " <<
            "len(" << prefix << ")); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error writing map begin: %s\")" << endl <<
            indent() << "}" << endl;
    } else if (ttype->is_set()) {
        out <<
            indent() << "if err := oprot.WriteSetBegin(" <<
            type_to_enum(((t_set*)ttype)->get_elem_type()) << ", " <<
            "len(" << prefix << ")); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error writing set begin: %s\")" << endl <<
            indent() << "}" << endl;
    } else if (ttype->is_list()) {
        out <<
            indent() << "if err := oprot.WriteListBegin(" <<
            type_to_enum(((t_list*)ttype)->get_elem_type()) << ", " <<
            "len(" << prefix << ")); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error writing list begin: %s\")" << endl <<
            indent() << "}" << endl;
    } else {
        throw "compiler error: Invalid type in generate_serialize_container '" + ttype->get_name() + "' for prefix '" + prefix + "'";
    }

    if (ttype->is_map()) {
        t_map* tmap = (t_map*)ttype;
        out <<
            indent() << "for k,v := range " << prefix << " {" << endl;
        indent_up();
        generate_serialize_map_element(out, tmap, "k", "v");
        indent_down();
        indent(out) << "}" << endl;
    } else if (ttype->is_set()) {
        t_set* tset = (t_set*)ttype;
        out <<
            indent() << "for v, _ := range " << prefix << " {" << endl;
        indent_up();
        generate_serialize_set_element(out, tset, "v");
        indent_down();
        indent(out) << "}" << endl;
    } else if (ttype->is_list()) {
        t_list* tlist = (t_list*)ttype;
        out <<
            indent() << "for _, v := range " << prefix << " {" << endl;

        indent_up();
        generate_serialize_list_element(out, tlist, "v");
        indent_down();
        indent(out) << "}" << endl;
    }

    if (ttype->is_map()) {
        out <<
            indent() << "if err := oprot.WriteMapEnd(); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error writing map end: %s\")" << endl <<
            indent() << "}" << endl;
    } else if (ttype->is_set()) {
        out <<
            indent() << "if err := oprot.WriteSetEnd(); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error writing set end: %s\")" << endl <<
            indent() << "}" << endl;
    } else if (ttype->is_list()) {
        out <<
            indent() << "if err := oprot.WriteListEnd(); err != nil {" << endl <<
            indent() << "  return fmt.Errorf(\"error writing list end: %s\")" << endl <<
            indent() << "}" << endl;
    }
}

/**
 * Serializes the members of a map.
 *
 */
void t_go_generator::generate_serialize_map_element(ofstream &out,
        t_map* tmap,
        string kiter,
        string viter)
{
    t_field kfield(tmap->get_key_type(), "");
    generate_serialize_field(out, &kfield, kiter);
    t_field vfield(tmap->get_val_type(), "");
    generate_serialize_field(out, &vfield, viter);
}

/**
 * Serializes the members of a set.
 */
void t_go_generator::generate_serialize_set_element(ofstream &out,
        t_set* tset,
        string prefix)
{
    t_field efield(tset->get_elem_type(), "");
    generate_serialize_field(out, &efield, prefix);
}

/**
 * Serializes the members of a list.
 */
void t_go_generator::generate_serialize_list_element(ofstream &out,
        t_list* tlist,
        string prefix)
{
    t_field efield(tlist->get_elem_type(), "");
    generate_serialize_field(out, &efield, prefix);
}

/**
 * Generates the docstring for a given struct.
 */
void t_go_generator::generate_go_docstring(ofstream& out,
        t_struct* tstruct)
{
    generate_go_docstring(out, tstruct, tstruct, "Attributes");
}

/**
 * Generates the docstring for a given function.
 */
void t_go_generator::generate_go_docstring(ofstream& out,
        t_function* tfunction)
{
    generate_go_docstring(out, tfunction, tfunction->get_arglist(), "Parameters");
}

/**
 * Generates the docstring for a struct or function.
 */
void t_go_generator::generate_go_docstring(ofstream& out,
        t_doc*    tdoc,
        t_struct* tstruct,
        const char* subheader)
{
    bool has_doc = false;
    stringstream ss;

    if (tdoc->has_doc()) {
        has_doc = true;
        ss << tdoc->get_doc();
    }

    const vector<t_field*>& fields = tstruct->get_members();

    if (fields.size() > 0) {
        if (has_doc) {
            ss << endl;
        }

        has_doc = true;
        ss << subheader << ":\n";
        vector<t_field*>::const_iterator p_iter;

        for (p_iter = fields.begin(); p_iter != fields.end(); ++p_iter) {
            t_field* p = *p_iter;
            ss << " - " << publicize(variable_name_to_go_name(p->get_name()));

            if (p->has_doc()) {
                ss << ": " << p->get_doc();
            } else {
                ss << endl;
            }
        }
    }

    if (has_doc) {
        generate_docstring_comment(out,
                                   "",
                                   "// ", ss.str(),
                                   "");
    }
}

/**
 * Generates the docstring for a generic object.
 */
void t_go_generator::generate_go_docstring(ofstream& out,
        t_doc* tdoc)
{
    if (tdoc->has_doc()) {
        generate_docstring_comment(out,
                                   "",
                                   "//", tdoc->get_doc(),
                                   "");
    }
}

/**
 * Declares an argument, which may include initialization as necessary.
 *
 * @param tfield The field
 */
string t_go_generator::declare_argument(t_field* tfield)
{
    std::ostringstream result;
    result << publicize(tfield->get_name()) << "=";

    if (tfield->get_value() != NULL) {
        result << "thrift_spec[" <<
               tfield->get_key() << "][4]";
    } else {
        result << "nil";
    }

    return result.str();
}

/**
 * Renders a field default value, returns nil otherwise.
 *
 * @param tfield The field
 */
string t_go_generator::render_field_default_value(t_field* tfield, const string& name)
{
    t_type* type = get_true_type(tfield->get_type());

    if (tfield->get_value() != NULL) {
        return render_const_value(type, tfield->get_value(), name);
    } else {
        return "nil";
    }
}

/**
 * Renders a function signature of the form 'type name(args)'
 *
 * @param tfunction Function definition
 * @return String of rendered function definition
 */
string t_go_generator::function_signature(t_function* tfunction,
        string prefix)
{
    // TODO(mcslee): Nitpicky, no ',' if argument_list is empty
    return
        publicize(prefix + tfunction->get_name()) +
        "(" + argument_list(tfunction->get_arglist()) + ")";
}

/**
 * Renders an interface function signature of the form 'type name(args)'
 *
 * @param tfunction Function definition
 * @return String of rendered function definition
 */
string t_go_generator::function_signature_if(t_function* tfunction,
        string prefix,
        bool addError)
{
    // TODO(mcslee): Nitpicky, no ',' if argument_list is empty
    string signature = publicize(prefix + tfunction->get_name()) + "(";
    signature += argument_list(tfunction->get_arglist()) + ") (";
    t_type* ret = tfunction->get_returntype();
    t_struct* exceptions = tfunction->get_xceptions();
    string errs = argument_list(exceptions);

    if (!ret->is_void()) {
        signature += "r " + type_to_go_type(ret);

        if (addError || errs.size() == 0) {
            signature += ", ";
        }
    }

    if (errs.size() > 0) {
        signature += errs;

        if (addError) {
            signature += ", ";
        }
    }

    if (addError) {
        signature += "err error";
    }

    signature += ")";
    return signature;
}


/**
 * Renders a field list
 */
string t_go_generator::argument_list(t_struct* tstruct)
{
    string result = "";
    const vector<t_field*>& fields = tstruct->get_members();
    vector<t_field*>::const_iterator f_iter;
    bool first = true;

    for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
        if (first) {
            first = false;
        } else {
            result += ", ";
        }

        result += variable_name_to_go_name((*f_iter)->get_name()) + " " + type_to_go_type((*f_iter)->get_type());
    }

    return result;
}

string t_go_generator::type_name(t_type* ttype)
{
    t_program* program = ttype->get_program();

    if (program != NULL && program != program_) {
        string module(get_real_go_module(program));
        // for namespaced includes, only keep part after dot.
        size_t dot = module.rfind('.');
        if (dot != string::npos) {
            module = module.substr(dot + 1);
        }
        return module + "." + ttype->get_name();
    }

    return ttype->get_name();
}

/**
 * Converts the parse type to a go tyoe
 */
string t_go_generator::type_to_enum(t_type* type)
{
    type = get_true_type(type);

    if (type->is_base_type()) {
        t_base_type::t_base tbase = ((t_base_type*)type)->get_base();

        switch (tbase) {
        case t_base_type::TYPE_VOID:
            throw "NO T_VOID CONSTRUCT";

        case t_base_type::TYPE_STRING:
            if (((t_base_type*)type)->is_binary()) {
                return "thrift.BINARY";
            }

            return "thrift.STRING";

        case t_base_type::TYPE_BOOL:
            return "thrift.BOOL";

        case t_base_type::TYPE_BYTE:
            return "thrift.BYTE";

        case t_base_type::TYPE_I16:
            return "thrift.I16";

        case t_base_type::TYPE_I32:
            return "thrift.I32";

        case t_base_type::TYPE_I64:
            return "thrift.I64";

        case t_base_type::TYPE_DOUBLE:
            return "thrift.DOUBLE";
        }
    } else if (type->is_enum()) {
        return "thrift.I32";
    } else if (type->is_struct() || type->is_xception()) {
        return "thrift.STRUCT";
    } else if (type->is_map()) {
        return "thrift.MAP";
    } else if (type->is_set()) {
        return "thrift.SET";
    } else if (type->is_list()) {
        return "thrift.LIST";
    }

    throw "INVALID TYPE IN type_to_enum: " + type->get_name();
}


/**
 * Converts the parse type to a go map type, will throw an exception if it will
 * not produce a valid go map type.
 */
string t_go_generator::type_to_go_key_type(t_type* type)
{
    t_type* resolved_type = type;

    while (resolved_type->is_typedef()) {
        resolved_type = ((t_typedef*)resolved_type)->get_type();
    }

    if (resolved_type->is_map() || resolved_type->is_list() || resolved_type->is_set()) {
        throw "Cannot produce a valid type for a Go map key: "  + type_to_go_type(type) + " - aborting.";
    }

    return type_to_go_type(type);
}

/**
 * Converts the parse type to a go tyoe
 */
string t_go_generator::type_to_go_type(t_type* type)
{
    //type = get_true_type(type);
    if (type->is_base_type()) {
        t_base_type::t_base tbase = ((t_base_type*)type)->get_base();

        switch (tbase) {
        case t_base_type::TYPE_VOID:
            throw "";

        case t_base_type::TYPE_STRING:
            if (((t_base_type*)type)->is_binary()) {
                return "[]byte";
            }

            return "string";

        case t_base_type::TYPE_BOOL:
            return "bool";

        case t_base_type::TYPE_BYTE:
            return "int8";

        case t_base_type::TYPE_I16:
            return "int16";

        case t_base_type::TYPE_I32:
            return "int32";

        case t_base_type::TYPE_I64:
            return "int64";

        case t_base_type::TYPE_DOUBLE:
            return "float64";
        }
    } else if (type->is_enum()) {
        return publicize(type_name(type));
    } else if (type->is_struct() || type->is_xception()) {
        return string("*") + publicize(type_name(type));
    } else if (type->is_map()) {
        t_map* t = (t_map*)type;
        string keyType = type_to_go_key_type(t->get_key_type());
        string valueType = type_to_go_type(t->get_val_type());
        return string("map[") + keyType + "]" + valueType;
    } else if (type->is_set()) {
        t_set* t = (t_set*)type;
        string elemType = type_to_go_key_type(t->get_elem_type());
        return string("map[") + elemType + string("]bool");
    } else if (type->is_list()) {
        t_list* t = (t_list*)type;
        string elemType = type_to_go_type(t->get_elem_type());
        return string("[]") + elemType;
    } else if (type->is_typedef()) {
        return publicize(type_name(type));
    }

    throw "INVALID TYPE IN type_to_go_type: " + type->get_name();
}


/**
 * Converts the parse type to a go tyoe
 */
bool t_go_generator::can_be_nil(t_type* type)
{
    type = get_true_type(type);

    if (type->is_base_type()) {
        t_base_type::t_base tbase = ((t_base_type*)type)->get_base();

        switch (tbase) {
        case t_base_type::TYPE_VOID:
            throw "Invalid Type for can_be_nil";

        case t_base_type::TYPE_BOOL:
        case t_base_type::TYPE_BYTE:
        case t_base_type::TYPE_I16:
        case t_base_type::TYPE_I32:
        case t_base_type::TYPE_I64:
        case t_base_type::TYPE_DOUBLE:
            return false;

        case t_base_type::TYPE_STRING:
            return (((t_base_type*)type)->is_binary());
        }
    } else if (type->is_enum()) {
        return false;
    } else if (type->is_struct() || type->is_xception()) {
        return true;
    } else if (type->is_map()) {
        return true;
    } else if (type->is_set()) {
        return true;
    } else if (type->is_list()) {
        return true;
    }

    throw "INVALID TYPE IN can_be_nil: " + type->get_name();
}



/** See the comment inside generate_go_struct_definition for what this is. */
string t_go_generator::type_to_spec_args(t_type* ttype)
{
    while (ttype->is_typedef()) {
        ttype = ((t_typedef*)ttype)->get_type();
    }

    if (ttype->is_base_type() || ttype->is_enum()) {
        return "nil";
    } else if (ttype->is_struct() || ttype->is_xception()) {
        return "(" + type_name(ttype) + ", " + type_name(ttype) + ".thrift_spec)";
    } else if (ttype->is_map()) {
        return "(" +
               type_to_enum(((t_map*)ttype)->get_key_type()) + "," +
               type_to_spec_args(((t_map*)ttype)->get_key_type()) + "," +
               type_to_enum(((t_map*)ttype)->get_val_type()) + "," +
               type_to_spec_args(((t_map*)ttype)->get_val_type()) +
               ")";
    } else if (ttype->is_set()) {
        return "(" +
               type_to_enum(((t_set*)ttype)->get_elem_type()) + "," +
               type_to_spec_args(((t_set*)ttype)->get_elem_type()) +
               ")";
    } else if (ttype->is_list()) {
        return "(" +
               type_to_enum(((t_list*)ttype)->get_elem_type()) + "," +
               type_to_spec_args(((t_list*)ttype)->get_elem_type()) +
               ")";
    }

    throw "INVALID TYPE IN type_to_spec_args: " + ttype->get_name();
}

bool format_go_output(const string &file_path)
{
    const string command = "gofmt -w " + file_path;

    if (system(command.c_str()) == 0) {
        return true;
    }

    fprintf(stderr, "WARNING - Running '%s' failed.\n", command.c_str());
    return false;
}


THRIFT_REGISTER_GENERATOR(go, "Go",
                          "    package_prefix= Package prefix for generated files.\n" \
                          "    thrift_import=  Override thrift package import path (default:" + default_thrift_import + ")\n")
