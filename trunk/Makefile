
CPPFLAGS = -I. -Isrc -g

test: tests/tests
	tests/tests

clean:
	rm -f tests/tests.cpp tests/tests
	find -name "*.o" -exec rm -f {} \;

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
	done >.depend
