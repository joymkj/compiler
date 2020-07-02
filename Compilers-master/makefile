stage2: lexer.o parser.o firstfollow.o ast.o semanticAnalysis.o symbolTable.o codeGen.o driver.o
	@gcc -g lexer.o parser.o firstfollow.o ast.o driver.o semanticAnalysis.o codeGen.o symbolTable.o -o stage2exe 
	@rm *.o 
	@echo Type \"./stage2exe [testcasefile] [OutputASMfile]\" to run.

driver.o: driver.c
	@gcc -g -c driver.c -o driver.o 

codeGen.o: codeGen.c codeGen.h
	@gcc -g -c codeGen.c -o codeGen.o

semanticAnalysis.o: semanticAnalysis.c semanticAnalysis.h
	@gcc -g -c semanticAnalysis.c -o semanticAnalysis.o
	@echo Semantic Analysis done.

symbolTable.o: symbolTable.c symbolTable.h
	@gcc -g -c symbolTable.c -o symbolTable.o
	@echo Symbol Table created.

ast.o: ast.c ast.h astDef.h
	@gcc -g -c ast.c -o ast.o 
	@echo Abstract Syntax Tree created.

parser.o: parser.c parser.h
	@gcc -g -c parser.c -o parser.o 
	@echo Parser Module built.

firstfollow.o: firstfollow.c parser.h
	@gcc -g -c firstfollow.c -o firstfollow.o 

lexer.o: lexer.c lexer.h
	@gcc -g -c lexer.c -o lexer.o 
	@echo Lexer Module built. 

clean:
	@rm *.o

nasm: code.o
	@ld -m elf_i386 code.o -o code
	@echo Running code...
	@./code
	@rm code.o

code.o: stage2 code.asm codeGen.c
	@nasm -f elf code.asm
	@echo Generated Code.
