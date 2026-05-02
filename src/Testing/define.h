#ifndef TESTING_DEFINE_H_INCLUDED
#define TESTING_DEFINE_H_INCLUDED

#include <Engine/define.h>


// MACRO for adding quotes
#define STRINGIFY(S) STRINGIFY2(S)
#define STRINGIFY2(S) #S

// MACRO for concatenating
#define CAT(A,B) CAT2(A,B)
#define CAT2(A,B) A##B
#define CAT_2 CAT
#define CAT_3(A,B,C) CAT(A,CAT_2(B,C))
#define CAT_4(A,B,C,D) CAT(A,CAT_3(B,C,D))


#define BEGIN_TEST( testName ) struct CAT(Test, testName) { public: static void Run() {
#define END_TEST_MAIN_FUNCTION( testName ) } private:
#define END_TEST( testName ) };
#define BEGIN_SUBTEST( testName, subtestName ) struct CAT_4(Test, testName, _, subtestName) { public: static void Run() {
#define END_SUBTEST( subtestName ) END_TEST_MAIN_FUNCTION( subtestName ) END_TEST( subtestName )


#endif