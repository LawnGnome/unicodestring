#ifndef TEST_TEST_H_DEFINED
#define TEST_TEST_H_DEFINED

class TestRunner;

class Test {
	public:
		virtual ~Test();

		virtual void run(TestRunner &runner) = 0;
};

#endif
