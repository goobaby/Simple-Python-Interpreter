.SUFFIXES: .o .cpp .x

CFLAGS = -ggdb -std=c++20
objects =  Types.o Range.o Token.o Tokenizer.o Parser.o ExprNode.o SymTab.o Statements.o main.o

statement.x: $(objects)
	g++ $(CFLAGS) -o statement.x $(objects)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@

Types.o: Types.cpp Types.hpp
Token.o:  Token.cpp Token.hpp
Tokenizer.o: Tokenizer.cpp Tokenizer.hpp
ExprNode.o: ExprNode.cpp ExprNode.hpp Token.hpp SymTab.hpp
SymTab.o: SymTab.cpp SymTab.hpp Types.cpp Types.hpp
Range.o: Range.cpp Range.hpp SymTab.hpp
Parser.o: Parser.cpp Token.hpp Parser.hpp Range.hpp Tokenizer.hpp SymTab.hpp ExprNode.hpp Statements.hpp
Statements.o: Statements.cpp Statements.hpp ExprNode.hpp Range.hpp Token.hpp SymTab.hpp
main.o: main.cpp Token.hpp Tokenizer.hpp Parser.hpp SymTab.hpp ExprNode.hpp Statements.hpp

clean:
	rm -fr *.o *~ *.x
