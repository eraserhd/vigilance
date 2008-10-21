
CPPFLAGS = -I. -Isrc -g

test: tests/tests
	tests/tests

clean:
	rm -f tests/tests.cpp tests/tests
	find -name "*.o" -exec rm -f {} \;

src/identifier_characters.h: src/identifier_characters.txt util/make_identifier_characters
	util/make_identifier_characters <src/identifier_characters.txt >src/identifier_characters.h

tests/tests.cpp: tests/test_*.h
	./cxxtestgen.py --error-printer -o $@ tests/test_*.h

ALL_SOURCES = $(wildcard src/*.cpp) tests/tests.cpp
ALL_OBJECTS = $(addsuffix .o,$(basename $(ALL_SOURCES)))

tests/tests: $(ALL_OBJECTS)
	$(CXX) -o $@ $(ALL_OBJECTS)


include .depend
.depend: $(ALL_SOURCES)
	for file in $(ALL_SOURCES); do \
	  g++ $(CPPFLAGS) -MM -MQ $${file%%.cpp}.o $$file; \
	done |sed 's/ identifier_characters\.h/ src\/identifier_characters\.h/' \
	  >.depend
