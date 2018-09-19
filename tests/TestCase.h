/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#ifndef REPOSTBOT_TESTCASE_H
#define REPOSTBOT_TESTCASE_H

class TestCase {
public:
    virtual void runTest() = 0;
};

// Add this helper to class, if you need access to private fields
#define TEST_HELPER public:\
    friend class TestCase;\
    friend class TestQueue1;\
    friend class TestQueue2;\

#endif //REPOSTBOT_TESTCASE_H
