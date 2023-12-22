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

#ifndef PARSING_DRIVER_HPP
#define PARSING_DRIVER_HPP

#include "cat_parser.tab.h"
#include "cat_module.h"
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

yy::parser::symbol_type yylex (cat_parsing_drivert& drv);

class cat_parsing_drivert {

private:
	struct State {
		State(yy::location loc, FILE* in,
		      const std::string &dir)
			: loc(loc), in(in), dir(dir) {}

		yy::location loc;
		FILE* in;
		std::string dir;
	};

public:
	cat_parsing_drivert();

	cat_modulet& get_module() {return *(module.get());}

	yy::location &getLocation() { return location; }

	void registerID(std::string id, cat_relationt re) {
		module->registerID(id, std::move(re));
	}

	// Handle consistency constraint in the input file
	void addConstraint(cat_axiomt c, const std::string &s, const yy::location &loc) {
		module->addConstraint(c);
	}

	// URE getRegisteredID(std::string id, const yy::location &loc) {
	// 	auto e = module->getRegisteredID(getQualifiedName(id));
	// 	if (!e) {
	// 		auto f = module->getRegisteredID(id);
	// 		if (!f) {
	// 			std::cerr << loc << ": ";
	// 			std::cerr << "unknown relation encountered (" << id << ")\n";
	// 			exit(EXIT_FAILURE);
	// 		}
	// 		e = std::move(f);
	// 	}
	// 	return std::move(e);
	// }

	/* Invoke the parser on INPUT. Return 0 on success. */
	int parse(const std::string &input);

private:
	void saveState();
	void restoreState();

	/* Location for lexing/parsing */
	yy::location location;

	/* Current source file directory (used for includes) */
	std::string dir;

	std::unique_ptr<cat_modulet> module;

	std::vector<State> states;
};

#endif /* PARSING_DRIVER_HPP */
