SRC = cat_parsing_driver.cpp \
      cat_module.cpp \
      cat_parser.tab.cpp \
      cat_lexer.yy.cpp \
      # Empty last line
INCLUDES= -I ..

include ../config.inc
include ../common

CLEANFILES = cat$(LIBEXT)

all: cat$(LIBEXT)

###############################################################################

cat$(LIBEXT): $(OBJ)
	$(LINKLIB)

cat_parser.tab.cpp: cat_parser.ypp
	$(YACC) -v cat_parser.ypp --defines=cat_parser.tab.h -o $@

cat_parser.tab.h: cat_parser.tab.cpp

cat_lexer.yy.cpp: cat_lexer.lpp
	$(LEX) -o$@ cat_lexer.lpp

generated_files: cat_lexer.yy.cpp cat_parser.tab.cpp cat_parser.tab.h

# extra dependencies
cat_parser.tab$(OBJEXT): cat_parser.tab.cpp cat_parser.tab.h
cat_lexer.yy$(OBJEXT): cat_parser.tab.cpp cat_lexer.yy.cpp cat_parser.tab.h
cat_parsing_driver$(OBJEXT): cat_parser.tab.h