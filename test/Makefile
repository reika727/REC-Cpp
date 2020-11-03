BUILTIN=../builtin

.PHONY: test
test:
	../$(TARGET) test.c test.s
	as test.s $(BUILTIN)/*.s start.s -o test.o
	ld test.o
	./a.out

.PHONY: clean
clean:
	$(RM) test.s test.o a.out
