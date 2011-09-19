#include "test/runner.h"
#include "test/test.h"

#include <iostream>
#include <sstream>

TestRunner::TestRunner() : assertions(0), failures(0) {
	start = time(NULL);
}

void TestRunner::assert(bool condition, const char *message) {
	doAssert(condition);
	std::cout << message << std::endl;
}

std::string TestRunner::report() const {
	std::ostringstream ss;

	ss << "TEST REPORT" << std::endl
		<< "Assertions made:   " << assertions << std::endl
		<< "Assertions passed: " << (assertions - failures) << std::endl
		<< "Assertions failed: " << failures << std::endl
		<< "Seconds taken:     " << (time(NULL) - start) << std::endl;

	return ss.str();
}

void TestRunner::run(Test &test) {
	test.run(*this);
}

bool TestRunner::doAssert(bool condition) {
	++assertions;

	if (!condition) {
		++failures;
		std::cout << "FAIL ";
	} else {
		std::cout << "PASS ";
	}

	return condition;
}
