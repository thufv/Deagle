/*
 * KATER -- Automating Weak Memory Model Metatheory
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-3.0.html.
 */

#include "cat_parsing_driver.h"
#include "cat_parser.tab.h"
#include <cstring>
#include <iostream>
#include <fstream>

#define DEBUG_TYPE "parser"

extern FILE* yyin;
extern void yyrestart(FILE *);

cat_parsing_drivert::cat_parsing_drivert() : module(new cat_modulet) 
{
	module->addBase("po", 2);
	module->addBase("rf", 2);
	module->addBase("co", 2);
	module->addBase("int", 2);
	module->addBase("ext", 2);
	module->addBase("addr", 2);
	module->addBase("data", 2);
	module->addBase("ctrl", 2);
	module->addBase("casdep", 2); // todo: for imm, not yet modeled
	module->addBase("rmw", 2);
	module->addBase("loc", 2);
	module->addBase("id", 2);
	module->addBase("_", 1);
	module->addBase("R", 1);
	module->addBase("W", 1);
	module->addBase("M", 1);
	module->addBase("~M", 1);
	module->addBase("F", 1);
	module->addBase("Fsync", 1);
	module->addBase("Flwsync", 1);
	module->addBase("Fisync", 1);
	module->addBase("A", 1);
	module->addBase("I", 1);
	module->addBase("IR", 1);
	module->addBase("IW", 1);
	module->addBase("IM", 1);
	module->addBase("RLX", 1);
	module->addBase("CON", 1);
	module->addBase("ACQ", 1);
	module->addBase("REL", 1);
	module->addBase("ACQ_REL", 1);
	module->addBase("SC", 1);
	module->addBase("RMW", 1);
	module->addBase("STRONG", 1);

	// lkmm
	module->addBase("Once", 1);
	module->addBase("Acquire", 1);
	module->addBase("Release", 1);
	module->addBase("Noreturn", 1);
	module->addBase("~Noreturn", 1);
	module->addBase("LKR", 1);
	module->addBase("LKW", 1);
	module->addBase("UL", 1);
	module->addBase("LF", 1);
	module->addBase("RL", 1);
	module->addBase("RU", 1);
	module->addBase("Rmb", 1);
	module->addBase("Wmb", 1);
	module->addBase("Mb", 1);
	module->addBase("Barrier", 1);
	module->addBase("Before-atomic", 1);
	module->addBase("After-atomic", 1);
	module->addBase("After-spinlock", 1);
	module->addBase("After-unlock-lock", 1);
	module->addBase("Sync-rcu", 1);
	module->addBase("Sync-srcu", 1);
	module->addBase("Rcu-lock", 1);
	module->addBase("Rcu-unlock", 1);
	module->addBase("Srcu-lock", 1);
	module->addBase("Srcu-unlock", 1);
	module->addBase("rcu-rscs", 2);
}

void cat_parsing_drivert::saveState()
{
	states.push_back(State(getLocation(), yyin, dir));
}

void cat_parsing_drivert::restoreState()
{
	if (!states.empty()) {
		auto &s = states.back();
		yyrestart(s.in);
		location = s.loc;
		dir = s.dir;
		states.pop_back();
	}
}

#include <fstream>
#include <regex>

void remove_block_comments(std::string& str)
{
	std::regex block_comment("\\(\\*(.|\\r|\\n)*?\\*\\)");
	str = std::regex_replace(str, block_comment, "");
}

void add_internal(std::string& str)
{
	std::string internal =
		R"(let rfi = rf & int
		let rfe = rf & ext
		let coi = co & int
		let coe = co & ext
		let fr = rf^-1 ; co
		let fri = fr & int
		let fre = fr & ext
		let po-loc = po & loc
		let poloc = po & loc
		)";
	str = internal + str;
}

void preprocess(const std::string& path, const std::string& new_path)
{
	std::ifstream in(path);
	std::istreambuf_iterator<char> beg(in), end;
	std::string str(beg, end);
	in.close();

	remove_block_comments(str);
	add_internal(str);

	std::ofstream out(new_path);
	out << str;
	out.close();
}

int cat_parsing_drivert::parse(const std::string &name)
{
	saveState();

	if (name.empty()) {
		std::cerr << "no input file provided" << std::endl;
		return 1;
	}

	auto path = dir + name;
	auto new_path = path + ".new";
	std::cout << "old path " << path << ", new path " << new_path << "\n";
	preprocess(path, new_path);

	if (!(yyin = fopen(new_path.c_str(), "r"))) {
		std::cerr << "cannot open " << new_path
			  << ": " << strerror(errno) << std::endl;
		return 1;
	}

	yyrestart(yyin);
	location.initialize(&path);

	yy::parser parser(*this);

	// KATER_DEBUG(
	// 	if (config.debug)
	// 		parser.set_debug_level(2);
	// );

	auto res = parser.parse(); // 0: OK; 1: error

	fclose(yyin);

	restoreState();

	return res;
}
