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
	module->addBase("RMW", 1); // dummy
	module->addBase("STRONG", 1); // todo: for imm, not yet modeled
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

int cat_parsing_drivert::parse(const std::string &name)
{
	saveState();

	if (name.empty()) {
		std::cerr << "no input file provided" << std::endl;
		return 1;
	}

	auto path = dir + name;
	if (!(yyin = fopen(path.c_str(), "r"))) {
		std::cerr << "cannot open " << path
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
