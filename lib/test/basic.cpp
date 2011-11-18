#include "ustring.h"

#include "test/runner.h"
#include "test/test.h"

#include <iostream>
#include <sstream>

#define ASSERT(runner, expected, value, message) \
	{ \
		std::ostringstream ss; \
		ss << (message) << " at " __FILE__ ":" << __LINE__ \
			<< " in " << __PRETTY_FUNCTION__; \
		(runner).assert((expected), (value), ss.str().c_str()); \
	}

#define ASSERT_EXCEPTION(runner, exception, call, message) \
	{ \
		try { \
			call; \
			ASSERT(runner, "exception", "no exception", message); \
		} catch (exception e) { \
			ASSERT(runner, true, true, message); \
		} \
	}

#define ASSERT_USTRING(runner, expected, value, message) \
	{ \
		UString expect((expected), sizeof(expected) - 1, "ISO-8859-1"); \
		ASSERT(runner, expect, value, message); \
	}

class BasicConstructorTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString us;
			ASSERT(runner, 0, us.length(), "UString length");
		}
};

class BufferConstructorTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString us("foo", 3, "ISO-8859-1");
			ASSERT(runner, 3, us.length(), "UString length");
			ASSERT(runner, "foo", us.toUTF8(), "UString content");

			ASSERT_EXCEPTION(runner, MalformedInput, UString("foo", 3, "bad"), "Character set exception");
		}
};

class CopyConstructorTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString original("foo", 3, "ISO-8859-1");
			UString copy(original);

			ASSERT(runner, true, original == copy, "Copy equality");
		}
};

class OperatorAssignmentTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString original("foo", 3, "ISO-8859-1");
			UString copy;

			copy = original;
			ASSERT(runner, true, original == copy, "Assigned copy equality");
		}
};

class OperatorComparisonTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString a("a", 1, "ISO-8859-1");
			UString b("b", 1, "ISO-8859-1");
			UString another("a", 1, "ISO-8859-1");

			ASSERT(runner, true, another == a, "a == another");
			ASSERT(runner, true, a != b, "a != b");
			ASSERT(runner, true, a < b, "a < b");
			ASSERT(runner, false, a > b, "a > b");
			ASSERT(runner, true, a >= another, "a >= another");
			ASSERT(runner, true, a <= b, "a <= b");
		}
};

class CharAtTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString us("foo", 3, "ISO-8859-1");

			ASSERT(runner, (UChar32) 'o', us.charAt(1), "charAt");
			ASSERT_EXCEPTION(runner, std::out_of_range, us.charAt(3), "charAt out of range");
		}
};

class CompareTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString a("a", 1, "ISO-8859-1");
			UString b("b", 1, "ISO-8859-1");
			UString another("a", 1, "ISO-8859-1");

			ASSERT(runner, 0, a.compare(another), "a cmp another");
			ASSERT(runner, -1, a.compare(b), "a cmp b");
			ASSERT(runner, 1, b.compare(a), "b cmp a");
		}
};

class CodepointCountTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString foo("foo", 3, "ISO-8859-1");
			UString::CodepointCount count(foo.countCodepoints());

			ASSERT(runner, 2, count.size(), "Count size");
			ASSERT(runner, 1, count['f'], "Count: f");
			ASSERT(runner, 2, count['o'], "Count: o");
			ASSERT(runner, 0, count['z'], "Count: z");
		}
};

class FindTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString haystack("quick", 5, "ISO-8859-1");
			UString needle("ui", 2, "ISO-8859-1");
			UString notFound("foo", 3, "ISO-8859-1");

			ASSERT(runner, 1, haystack.find(needle), "Find");
			ASSERT_EXCEPTION(runner, NotFoundError, haystack.find(notFound), "Find non-existent value");
		}
};

class LengthTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString foo("foo", 3, "ISO-8859-1");
			ASSERT(runner, 3, foo.length(), "Length");
		}
};

class RemoveTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString us("foo", 3, "ISO-8859-1");

			us.remove(0);
			ASSERT_USTRING(runner, "oo", us, "Remove first character");
			ASSERT_EXCEPTION(runner, std::out_of_range, us.remove(2), "Remove out of range");
		}
};

class ReplaceTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString haystack("quick", 5, "ISO-8859-1");
			UString needle("ui", 2, "ISO-8859-1");
			UString notFound("foo", 3, "ISO-8859-1");
			UString multiple("quickquick", 10, "ISO-8859-1");

			ASSERT_USTRING(runner, "qfoock", haystack.replace(needle, notFound), "Existent replacement");
			ASSERT(runner, haystack, haystack.replace(notFound, needle), "Non-existent replacement");
			ASSERT_USTRING(runner, "qfoockqfoock", multiple.replace(needle, notFound), "Multiple replacements");
		}
};

class ReverseTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString empty, us("foo", 3, "ISO-8859-1");

			ASSERT(runner, empty, empty.reverse(), "Empty reversal");
			ASSERT_USTRING(runner, "oof", us.reverse(), "Foo reversal");
		}
};

class RFindTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString haystack("quick quick", 11, "ISO-8859-1");
			UString needle("ui", 2, "ISO-8859-1");
			UString notFound("foo", 3, "ISO-8859-1");

			ASSERT(runner, 7, haystack.rfind(needle), "RFind");
			ASSERT_EXCEPTION(runner, NotFoundError, haystack.find(notFound), "RFind non-existent value");
		}
};

class SetCharAtTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString empty;
			UString q("q", 1, "ISO-8859-1");
			UString us("foo", 3, "ISO-8859-1");

			us.setCharAt(1, q);
			ASSERT_USTRING(runner, "fqo", us, "setCharAt");

			ASSERT_EXCEPTION(runner, std::out_of_range, us.setCharAt(3, q), "Out of range offset");
			ASSERT_EXCEPTION(runner, std::invalid_argument, us.setCharAt(0, empty), "Empty string");
		}
};

class SubstringTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString quick("quick", 5, "ISO-8859-1");
			
			ASSERT_USTRING(runner, "ui", quick.substring(1, 2), "Substring with offset and length");
			ASSERT_USTRING(runner, "uick", quick.substring(1), "Substring with offset only");

			ASSERT_EXCEPTION(runner, std::out_of_range, quick.substring(5), "Substring offset out of range");
			ASSERT_EXCEPTION(runner, std::out_of_range, quick.substring(4, 2), "Substring length out of range");
		}
};

class ToEncodingTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString us("f\xf5o", 3, "ISO-8859-1");

			ASSERT(runner, "f\xf5o", us.toEncoding("ISO-8859-1"), "Basic toEncoding: ISO-8859-1");
			ASSERT(runner, "f\xc3\xb5o", us.toEncoding("UTF-8"), "Basic toEncoding: UTF-8");
		}
};

class ToLowerUpperTest : public Test {
	public:
		void run(TestRunner &runner) {
			UString us("Foo", 3, "ISO-8859-1");

			ASSERT_USTRING(runner, "foo", us.toLower(), "toLower");
			ASSERT_USTRING(runner, "FOO", us.toUpper(), "toUpper");
		}
};

class ToUTF8Test : public Test {
	public:
		void run(TestRunner &runner) {
			UString us("foo", 3, "ISO-8859-1");

			ASSERT(runner, "foo", us.toUTF8(), "Basic toUTF8");
		}
};

int main() {
	TestRunner runner;

	BasicConstructorTest().run(runner);
	BufferConstructorTest().run(runner);
	CopyConstructorTest().run(runner);
	OperatorAssignmentTest().run(runner);
	OperatorComparisonTest().run(runner);
	CharAtTest().run(runner);
	CompareTest().run(runner);
	CodepointCountTest().run(runner);
	FindTest().run(runner);
	LengthTest().run(runner);
	RemoveTest().run(runner);
	ReplaceTest().run(runner);
	ReverseTest().run(runner);
	RFindTest().run(runner);
	SetCharAtTest().run(runner);
	SubstringTest().run(runner);
	ToEncodingTest().run(runner);
	ToLowerUpperTest().run(runner);
	ToUTF8Test().run(runner);

	std::cout << std::endl << runner.report() << std::endl;

	return 0;
}
