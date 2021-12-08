/*
 * preprocess.cpp
 * Author: ylz86
 *  Date : 2016-12-10
 */

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cassert>

//=====================================================================
// Simplify pthread_create and pthread_join
//=====================================================================
std::string trim(const std::string &s)
{
	int i = 0, j = s.size() - 1;
	while(s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')
		i++;
	while(s[j] == ' ' || s[j] == '\t' || s[j] == '\r' || s[j] == '\n')
		j--;
	return s.substr(i, j + 1 - i);
}

void pthread_create_parse(std::string line, std::string& thread_id, std::string& function, std::string& params)
{
	int pos, newpos;
	std::string str_create("pthread_create");
	assert(line.find(str_create) != std::string::npos);

	// get thread_id
	pos = line.find('(', 0) + 1;
	newpos = line.find(',', pos);
	thread_id = line.substr(pos, newpos - pos);
	thread_id = trim(thread_id);
	thread_id = thread_id.substr(1, thread_id.size() - 1);

	// get function
	pos = line.find(',', newpos + 1) + 1;
	newpos = line.find(',', pos);
	function = line.substr(pos, newpos - pos);
	function = trim(function);

	// get params
	pos = newpos + 1;
	newpos = line.find(')', pos);
	params = line.substr(pos, newpos - pos);
	params = trim(params);
}

void pthread_join_parse(std::string line, std::string& thread_id)
{
	int pos, newpos;
	std::string str_join("pthread_join");
	assert(line.find(str_join) != std::string::npos);

	// get thread_id
	pos = line.find('(', 0) + 1;
	newpos = line.find(',', pos);
	thread_id = line.substr(pos, newpos - pos);
	thread_id = trim(thread_id);
}


void pthread_simplify(std::string file_name, std::string& newfile)
{
	std::ifstream input(file_name.data(), std::ios::in);

	std::string pjoin_thr_id;
	std::string pcreate_thr_id, pcreate_function, pcreate_params;
	bool pre_is_create = false;
	std::string pcreate;

	char line[10000];

	while(!input.eof()) {
		input.getline(line, 10000);
		std::string str_line(line);
		std::string str_line_trim = str_line;
		str_line_trim = trim(str_line_trim);

		// find a pthread_join
		if (pre_is_create && str_line_trim.find("pthread_join") == 0) {
			pthread_join_parse(str_line_trim, pjoin_thr_id);
			if (pjoin_thr_id == pcreate_thr_id) {
				std::string fun_invoke = pcreate_function + std::string("(") + pcreate_params + std::string(");\n");
				newfile += fun_invoke;
			}
			else {
				newfile += pcreate + std::string("\n");
				newfile += str_line + std::string("\n");
			}
			pre_is_create = false;
		}
		else {
			if (pre_is_create)
				newfile += pcreate + std::string("\n");

			// find a pthread_create
			if (str_line_trim.find("pthread_create") == 0) {
				pre_is_create = true;
				pcreate = str_line;
				pthread_create_parse(str_line_trim, pcreate_thr_id, pcreate_function, pcreate_params);
			}
			else {
				newfile += str_line + std::string("\n");
				pre_is_create = false;
			}
		}
	}
	input.close();
}

//=====================================================================
// Replace All struct variables
//=====================================================================
struct MemVar {
	std::string type;
	std::string var;     // array
	std::string org_var; // array[3]
};
struct StructType {
	std::string name;
	std::vector<MemVar> var_list;
	int end_pos;
};

struct StructVar{
	int type_id;
	std::string name;
	int end_pos;
};

std::vector<StructType> struct_types;
std::vector<StructVar> struct_vars;
std::vector<int> valid_struct_vars;

bool IsVarSymbol(char c)
{
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c >= '0' && c <= '9') ||
		c == '_';
}

bool IsFirstVarSymbol(char c)
{
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}


bool IsVar(std::string str)
{
	if (!IsFirstVarSymbol(str[0]))
		return false;
	for (size_t i = 1; i < str.size(); i++) {
		if (!IsVarSymbol(str[i]))
			return false;
	}
	return true;
}

void PrintStructType(StructType struct_type)
{
	std::cout << "typedef struct " << struct_type.name << "{\n";
	for (int j = 0; j < struct_type.var_list.size(); j++) {
		MemVar& var = struct_type.var_list[j];
		std::cout << "\t" << var.type << " " << var.var << ";\n";
	}
	std::cout << "}" << struct_type.name << ";\n\n";
}

int StructTypeParse(const std::string& str, int pos)
{
	StructType type;
	int ls = str.find('{', pos);
	int rs = str.find('}', pos);
	int end = str.find(';', rs);
	std::string name = str.substr(rs + 1, end - 1 - rs);
	type.name = trim(name);
	if (!IsVar(type.name))
		return end;

	int i = ls + 1;
	while (i < rs) {
		while(str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n')
			i++;
		if (i >= rs)
			break;

		MemVar mem_var;
		int j = str.find(' ', i);
		mem_var.type = trim(str.substr(i, j + 1 - i));
		i = j + 1;
		j = str.find(';', i);
		mem_var.org_var = trim(str.substr(i, j - i));
		i = j + 1;

		mem_var.var = mem_var.org_var;
		if (mem_var.var.find('[', 0) != std::string::npos)
			mem_var.var = mem_var.var.substr(0, mem_var.var.find('[', 0));

		type.var_list.push_back(mem_var);
	}
	type.end_pos = end;

	struct_types.push_back(type);

//	PrintStructType(type);

	return end;
}

void ComputeStructDefs(const std::string& str)
{
	int i = str.rfind("extern", str.size() - 1);
	while(str.find("typedef struct", i) != std::string::npos) {
		i = StructTypeParse(str, str.find("typedef struct", i));
	}
}

int StructVarParse(const std::string& str, int pos, int type_id)
{
	StructVar var;
	var.type_id = type_id;
	int i = pos + struct_types[type_id].name.size();
	while(str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n')
		i++;
	int end = str.find(';', i);
	std::string name = trim(str.substr(i, end - i));
	if (!IsVar(name))
		return end;
	var.name = name;
	var.end_pos = end;
	struct_vars.push_back(var);
	return end;
}

void ComputeStructVars(const std::string& str)
{
	for (size_t i = 0; i < struct_types.size(); i++) {
		int start = struct_types[i].end_pos;
		while(str.find(struct_types[i].name, start) != std::string::npos) {
			start = StructVarParse(str, str.find(struct_types[i].name, start), i);
		}
	}
}

void ComputeValidStructVars(const std::string &str)
{
	for (size_t i = 0; i < struct_vars.size(); i++) {
		StructVar& var = struct_vars[i];
		int start = var.end_pos;
		bool valid = true;
		while(str.find(var.name, start) != std::string::npos) {
			int k = str.find(var.name, start);
			start = k + var.name.size();
			if (IsVarSymbol(str[k-1]) || IsVarSymbol(str[start]))
				continue;
			if (str[start] != '.') {
				valid = false;
				break;
			}
		}
		if (valid)
			valid_struct_vars.push_back(i);
	}
}

void ReplaceStructVar(std::string& str, int var_id)
{
	StructVar& var = struct_vars[var_id];

	std::map<std::string, std::string> replace_list;
	std::string var_declare = "\n\n";
	for (size_t i = 0; i < struct_types[var.type_id].var_list.size(); i++) {
		MemVar& mem_var = struct_types[var.type_id].var_list[i];
		std::string old_name = var.name + std::string(".") + mem_var.var;
		std::string new_name = var.name + std::string("_") + mem_var.var;
		replace_list[old_name] = new_name;

		std::string mem_var_decl = mem_var.type + std::string(" ") + var.name + std::string("_") + mem_var.org_var + std::string(";\n");
		var_declare += mem_var_decl;
	}
	var_declare += "\n";

	std::string first = str.substr(0, var.end_pos + 1);
	std::string last = str.substr(var.end_pos + 1, str.size() - var.end_pos - 1);
	str = first + var_declare + last;

	for (std::map<std::string, std::string>::iterator it = replace_list.begin(); it != replace_list.end(); ++it) {
		while (str.find(it->first) != std::string::npos)
			str = str.replace(str.find(it->first), it->first.length(), it->second);
	}
}

void ReplaceAllStructVars(std::string& str)
{
	for (size_t i = 0; i < valid_struct_vars.size(); i++) {
		ReplaceStructVar(str, valid_struct_vars[i]);
	}
}

void UnrollStructVars(std::string& str)
{
	ComputeStructDefs(str);
	ComputeStructVars(str);
	ComputeValidStructVars(str);
	ReplaceAllStructVars(str);
}

//===================================================================================

int find_number_of(std::string& str, std::string substr)
{
	int ret = 0;
	for(int start = 0;;)
	{
		int pos = str.find(substr, start);
		if(pos == std::string::npos)
			return ret;
		ret++;
		start = pos + 1;
	}
}

bool preprocess(std::string file_name, std::string& new_file_name) {
	std::string str;

	pthread_simplify(file_name, str);
	UnrollStructVars(str);

	std::ofstream output(new_file_name.data(), std::ios::out);
	output<< str << "\n";
	output.close();

	return find_number_of(str, "pthread_mutex_lock") > 2;

	//return valid_struct_vars.size() > 0;
}
