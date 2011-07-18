APP:=mytest.so
SHELL:=sh
CXX:=g++ -Wall -O2 -fPIC
LD:=g++ -shared
MAKEDEP:=g++ $(CXX) -M

SOURCES:=$(wildcard *.c)
OBJECTS:=$(SOURCES:.c=.o)
DEPS:=$(SOURCES:.c=.d)

.PHONY: all clean

all: $(APP)

$(APP): $(OBJECTS)
	$(LD) $^ -o $@

$(OBJECTS): %.o: %.c Makefile
	$(CXX) -c $< -o $@

$(DEPS): %.d: %.c Makefile
	@$(SHELL) -ec '$(CXX) -M $< | sed "s/$*.o/& $@/g" > $@'

clean:
	@rm -rf $(APP) $(OBJECTS) $(DEPS) *~

-include $(SOURCES:.c=.d)

