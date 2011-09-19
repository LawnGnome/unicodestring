#ifndef TEST_RUNNER_H_DEFINED
#define TEST_RUNNER_H_DEFINED

#include <ctime>
#include <iostream>
#include <string>

#include "ustring.h"

class Test;

class TestRunner {
	public:
		TestRunner();

		void assert(bool condition, const char *message);
		void assert(int expected, size_t value, const char *message) {
			assert((size_t) expected, value, message);
		}
		void assert(const char *expected, const std::string &value, const char *message) {
			std::string expectedStr(expected);
			assert(expectedStr, value, message);
		}
		void assert(const UString &expected, const UString &value, const char *message) {
			std::string exp(expected.toUTF8()), val(value.toUTF8());
			assert(exp, val, message);
		}
		template<typename T> void assert(const T &expected, const T &value, const char *message) {
			doAssert(expected == value);
			std::cout << message;

			if (expected != value) {
				std::cout << "; expected " << expected << "; got " << value;
			}

			std::cout << std::endl;
		}

		std::string report() const;
		void run(Test &test);

	private:
		unsigned long assertions;
		unsigned long failures;
		time_t start;

		bool doAssert(bool condition);
};

#endif
