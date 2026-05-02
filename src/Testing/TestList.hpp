#ifndef TEST_LIST_HPP_INCLUDED
#define TEST_LIST_HPP_INCLUDED

#include <Containers/List.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( List )

	// List<int> mainList;
	// std::cout << "--------------PushBack--------------" << std::endl;
	// // Test PushBack
	// mainList.PushBack(1);
	// mainList.PushBack(2);
	// mainList.PushBack(3);
	// mainList.PushBack(1);
	// mainList.PushBack(2);
	// mainList.PushBack(3);
	// std::cout << "MainList : ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 1 2 3 1 2 3
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------PushFront--------------" << std::endl;
	// // Test PushFront
	// mainList.PushFront(0);
	// std::cout << "MainList push front 0 : ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 0 1 2 3 1 2 3
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Pop Front & Back--------------" << std::endl;
	// // Test pop_back and pop_front/
	// std::cout << "MainList before pop front & back: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " ";  // Should print 0 1 2 3 1 2 3
	// }
	// std::cout << std::endl;
	// mainList.PopBack();
	// mainList.PopFront();
	// std::cout << "MainList after pop front & back: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // 1 2 3 1 2 
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Insert--------------" << std::endl;
	// // Test insert
	// List<int>::Iterator pos = mainList.Begin();
	// Advance(pos, 1); // Move iterator to position 1
	// mainList.Insert(pos, 4); // Insert 4 at index 
	// std::cout << "MainList after insert 4 at index 1: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 1 4 2 3 1 2 
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Insert (index , first, last)--------------" << std::endl;
	// // Test insert with range
	// List<int> insertList;
	// insertList.PushBack(5);
	// insertList.PushBack(6);
	// insertList.PushBack(7);
	// List<int>::Iterator possec = mainList.Begin();
	// Advance(possec, 1); // Move iterator to position 1
	// mainList.Insert(possec, insertList); // insert 5, 6, 7 at index 1
	// std::cout << "MainList after insert range at index 1: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; //Should print 1 5 6 7 4 2 3 1 2
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Resize--------------" << std::endl;
	// // Test 
	// mainList.Resize(5, 0);
	// std::cout << "MainList after resize to 5: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 1 5 6 7 4
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Remove--------------" << std::endl;
	// // Test remove
	// mainList.Remove(4); // Remove 4
	// std::cout << "MainList after remove 4: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 1 5 6 7
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Remove if--------------" << std::endl;
	// // Test remove_if
	// mainList.RemoveIf([](const int& value) { return value < 2; }); // Remove values less than 2
	// std::cout << "MainList after remove_if (remove values < 2): ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 6 7
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Reverse--------------" << std::endl;
	// // Test reverse
	// mainList.Reverse();
	// std::cout << "MainList after reverse: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 7 6 5
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Unique--------------" << std::endl;
	// // Test unique
	// mainList.PushBack(1);
	// mainList.PushBack(2);
	// mainList.PushBack(2);
	// mainList.PushBack(3);
	// std::cout << "MainList before unique: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print  7 6 5 1 2 2 3
	// }
	// std::cout << std::endl;
	// mainList.Unique();
	// std::cout << "MainList after unique: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 7 6 5 1 2 3
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Merge--------------" << std::endl;
	// // Test merge
	// List<int> SecondList;
	// SecondList.PushBack(5);
	// SecondList.PushBack(6);
	// SecondList.PushBack(7);
	// SecondList.Sort();
	// std::cout << "SecondList before merge: ";
	// for (auto it = SecondList.Begin(); it != SecondList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 6 7
	// }
	// std::cout << std::endl;
	// mainList.Sort();
	// mainList.Unique();
	// std::cout << "mainList before merge: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // 1 2 3 5 6 7
	// }
	// std::cout << std::endl;
	// mainList.Merge(SecondList);
	// std::cout << "MainList after merge: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 1 2 3 5 5 6 6 7 7
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Erase--------------" << std::endl;
	// //test erase
	// mainList.Erase(mainList.Begin()); // Erase first element
	// std::cout << "MainList after erase first element: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 2 3 5 5 6 6 7 7
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Erase From x to y--------------" << std::endl;
	// //test erase from one pos to another
	// List<int>::Iterator posera = mainList.Begin();
	// Advance(posera, 2); // Move iterator to position 1
	// mainList.Erase(mainList.Begin(), posera); // Erase first two elements
	// std::cout << "MainList after erase from Begin to 2: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 6 6 7 7
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Test Swap--------------" << std::endl;
	// // Test Swap
	// std::cout << "MainList before Swap :";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 6 6 7 7
	// }
	// std::cout << std::endl;
	//
	// std::cout << "SecondList before Swap :";
	// for (auto it = SecondList.Begin(); it != SecondList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------Swaped--------" << std::endl;
	// std::cout << std::endl;
	// mainList.Swap(SecondList);
	// std::cout << "MainList after Swap :";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 
	// }
	// std::cout << std::endl;
	// std::cout << "SecondList after Swap :";
	// for (auto it = SecondList.Begin(); it != SecondList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 6 6 7 7
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Empty--------------" << std::endl;
	// // Test Empty
	// std::cout << "Is mainList Empty after : " << (mainList.Empty() ? "Yes" : "No") << std::endl; // Should be Yes
	// std::cout << "Is SecondList Empty after : " << (SecondList.Empty() ? "Yes" : "No") << std::endl; // Should be No
	//
	// std::cout << std::endl << "--------------Test Splice--------------" << std::endl;
	// // Test splice
	// mainList.PushBack(5);
	// mainList.PushBack(10);
	//
	// std::cout << "MainList before splice: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 10
	// }
	// std::cout << std::endl;
	// std::cout << "SecondList :";
	// for (auto it = SecondList.Begin(); it != SecondList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 6 6 7 7
	// }
	// pos = mainList.Begin();
	// Advance(pos, 1); // Move iterator to position 2
	// mainList.Splice(pos, SecondList); // Splice  into mainList at position 2
	// std::cout << std::endl;
	// std::cout << std::endl << "--------Spliced--------" << std::endl;
	// std::cout << std::endl;
	// std::cout << "MainList after splice SecondList: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 5 6 6 7 7 10
	// }
	// std::cout << std::endl;
	//
	// std::cout << "SecondList after splice SecondList: ";
	// for (auto it = SecondList.Begin(); it != SecondList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print ""
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Emplace--------------" << std::endl;
	// // Test Emplace
	// List<int>::Iterator it = mainList.Begin();
	// Advance(it, 2); // Move iterator to position 2
	// mainList.Emplace(it, 8); // Emplace 8 at position 2
	// std::cout << "MainList after emplace 8 at position 2: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 8 5 6 6 7 7 10
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Test AppendRange--------------" << std::endl;
	// // Test AppendRange
	// List<int> appendList;
	// appendList.PushBack(11);
	// appendList.PushBack(12);
	// appendList.PushBack(13);
	// mainList.AppendRange(appendList); // Append appendList to mainList
	// std::cout << "MainList after append range: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 8 5 6 6 7 7 10 11 12 13
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Test Clear--------------" << std::endl;
	// // Test Clear
	// mainList.Clear();
	// std::cout << "MainList after clear: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print nothing
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Test EmplaceFront / EmplaceBack--------------" << std::endl;
	// List<int> emplaceList;
	// emplaceList.EmplaceFront(2);
	// emplaceList.EmplaceFront(1);
	// emplaceList.EmplaceBack(3);
	// emplaceList.EmplaceBack(4);
	// std::cout << "EmplaceList (should be 1 2 3 4): ";
	// for (auto it = emplaceList.Begin(); it != emplaceList.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test PrependRange--------------" << std::endl;
	// List<int> prependList;
	// prependList.PushBack(10);
	// prependList.PushBack(20);
	// emplaceList.PrependRange(prependList);
	// std::cout << "EmplaceList after prepend (should be 10 20 1 2 3 4): ";
	// for (auto it = emplaceList.Begin(); it != emplaceList.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Insert(index, count, value)--------------" << std::endl;
	// auto itInsert = emplaceList.Begin();
	// Advance(itInsert, 2); // position after 20
	// emplaceList.Insert(itInsert, 3, 99); // insert 99 three times
	// std::cout << "EmplaceList after Insert(3, 99): ";
	// for (auto it = emplaceList.Begin(); it != emplaceList.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Insert(index, InputIt first, InputIt last)--------------" << std::endl;
	// Vector<int> fromVec = { 7, 8, 9 };
	// emplaceList.Insert(emplaceList.CEnd(), fromVec.begin(), fromVec.end());
	// std::cout << "EmplaceList after Insert(fromVec): ";
	// for (auto it = emplaceList.Begin(); it != emplaceList.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Unique with Predicate--------------" << std::endl;
	// List<int> uniqTest;
	// uniqTest.PushBack(1);
	// uniqTest.PushBack(2);
	// uniqTest.PushBack(2);
	// uniqTest.PushBack(3);
	// uniqTest.PushBack(4);
	// uniqTest.PushBack(5);
	// uniqTest.Unique([](int a, int b) { return (a + b) % 2 == 0; }); // Remove if sum is even
	// std::cout << "uniqTest after Unique(predicate): ";
	// for (auto it = uniqTest.Begin(); it != uniqTest.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Merge with Predicate--------------" << std::endl;
	// List<int> m1, m2;
	// m1.PushBack(1); m1.PushBack(3); m1.PushBack(5);
	// m2.PushBack(2); m2.PushBack(4); m2.PushBack(6);
	// m1.Merge(m2, [](int a, int b) { return a < b; });
	// std::cout << "m1 after Merge with predicate: ";
	// for (auto it = m1.Begin(); it != m1.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Splice(single element)--------------" << std::endl;
	// List<int> a, b;
	// a.PushBack(1); a.PushBack(2);
	// b.PushBack(10); b.PushBack(11);
	// auto itSingle = b.Begin();
	// Advance(itSingle, 1); // points to 11
	// a.Splice(a.Begin(), b, itSingle);
	// std::cout << "a after splice single element: ";
	// for (auto it = a.Begin(); it != a.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	// std::cout << "b after removing element: ";
	// for (auto it = b.Begin(); it != b.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Splice(range)--------------" << std::endl;
	// List<int> source;
	// source.PushBack(7); source.PushBack(8); source.PushBack(9);
	// auto first = source.Begin();
	// auto last = source.End();
	// a.Splice(a.End(), source, first, last);
	// std::cout << "a after splice range: ";
	// for (auto it = a.Begin(); it != a.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	// }
	//
	// std::cout << std::endl;
	// mainList.Sort();
	// mainList.Unique();
	// std::cout << "mainList before merge: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // 1 2 3 5 6 7
	// }
	// std::cout << std::endl;
	// mainList.Merge(SecondList);
	// std::cout << "MainList after merge: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 1 2 3 5 5 6 6 7 7
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Erase--------------" << std::endl;
	// //test erase
	// mainList.Erase(mainList.Begin()); // Erase first element
	// std::cout << "MainList after erase first element: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 2 3 5 5 6 6 7 7
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Erase From x to y--------------" << std::endl;
	// //test erase from one pos to another
	// List<int>::Iterator posera = mainList.Begin();
	// Advance(posera, 2); // Move iterator to position 1
	// mainList.Erase(mainList.Begin(), posera); // Erase first two elements
	// std::cout << "MainList after erase from Begin to 2: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 6 6 7 7
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Test Swap--------------" << std::endl;
	// // Test Swap
	// std::cout << "MainList before Swap :";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 6 6 7 7
	// }
	// std::cout << std::endl;
	//
	// std::cout << "SecondList before Swap :";
	// for (auto it = SecondList.Begin(); it != SecondList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------Swaped--------" << std::endl;
	// std::cout << std::endl;
	// mainList.Swap(SecondList);
	// std::cout << "MainList after Swap :";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 
	// }
	// std::cout << std::endl;
	// std::cout << "SecondList after Swap :";
	// for (auto it = SecondList.Begin(); it != SecondList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 6 6 7 7
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Empty--------------" << std::endl;
	// // Test Empty
	// std::cout << "Is mainList Empty after : " << (mainList.Empty() ? "Yes" : "No") << std::endl; // Should be Yes
	// std::cout << "Is SecondList Empty after : " << (SecondList.Empty() ? "Yes" : "No") << std::endl; // Should be No
	//
	// std::cout << std::endl << "--------------Test Splice--------------" << std::endl;
	// // Test splice
	// mainList.PushBack(5);
	// mainList.PushBack(10);
	//
	// std::cout << "MainList before splice: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 10
	// }
	// std::cout << std::endl;
	// std::cout << "SecondList :";
	// for (auto it = SecondList.Begin(); it != SecondList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 6 6 7 7
	// }
	// pos = mainList.Begin();
	// Advance(pos, 1); // Move iterator to position 2
	// mainList.Splice(pos, SecondList); // Splice  into mainList at position 2
	// std::cout << std::endl;
	// std::cout << std::endl << "--------Spliced--------" << std::endl;
	// std::cout << std::endl;
	// std::cout << "MainList after splice SecondList: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 5 6 6 7 7 10
	// }
	// std::cout << std::endl;
	//
	// std::cout << "SecondList after splice SecondList: ";
	// for (auto it = SecondList.Begin(); it != SecondList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print ""
	// }
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Emplace--------------" << std::endl;
	// // Test Emplace
	// List<int>::Iterator it = mainList.Begin();
	// Advance(it, 2); // Move iterator to position 2
	// mainList.Emplace(it, 8); // Emplace 8 at position 2
	// std::cout << "MainList after emplace 8 at position 2: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 8 5 6 6 7 7 10
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Test AppendRange--------------" << std::endl;
	// // Test AppendRange
	// List<int> appendList;
	// appendList.PushBack(11);
	// appendList.PushBack(12);
	// appendList.PushBack(13);
	// mainList.AppendRange(appendList); // Append appendList to mainList
	// std::cout << "MainList after append range: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print 5 5 8 5 6 6 7 7 10 11 12 13
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Test Clear--------------" << std::endl;
	// // Test Clear
	// mainList.Clear();
	// std::cout << "MainList after clear: ";
	// for (auto it = mainList.Begin(); it != mainList.End(); ++it) {
	// 	std::cout << *it << " "; // Should print nothing
	// }
	// std::cout << std::endl;
	// std::cout << std::endl << "--------------Test EmplaceFront / EmplaceBack--------------" << std::endl;
	// List<int> emplaceList;
	// emplaceList.EmplaceFront(2);
	// emplaceList.EmplaceFront(1);
	// emplaceList.EmplaceBack(3);
	// emplaceList.EmplaceBack(4);
	// std::cout << "EmplaceList (should be 1 2 3 4): ";
	// for (auto it = emplaceList.Begin(); it != emplaceList.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test PrependRange--------------" << std::endl;
	// List<int> prependList;
	// prependList.PushBack(10);
	// prependList.PushBack(20);
	// emplaceList.PrependRange(prependList);
	// std::cout << "EmplaceList after prepend (should be 10 20 1 2 3 4): ";
	// for (auto it = emplaceList.Begin(); it != emplaceList.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Insert(index, count, value)--------------" << std::endl;
	// auto itInsert = emplaceList.Begin();
	// Advance(itInsert, 2); // position after 20
	// emplaceList.Insert(itInsert, 3, 99); // insert 99 three times
	// std::cout << "EmplaceList after Insert(3, 99): ";
	// for (auto it = emplaceList.Begin(); it != emplaceList.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Insert(index, InputIt first, InputIt last)--------------" << std::endl;
	// Vector<int> fromVec = { 7, 8, 9 };
	// emplaceList.Insert(emplaceList.CEnd(), fromVec.begin(), fromVec.end());
	// std::cout << "EmplaceList after Insert(fromVec): ";
	// for (auto it = emplaceList.Begin(); it != emplaceList.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Unique with Predicate--------------" << std::endl;
	// List<int> uniqTest;
	// uniqTest.PushBack(1);
	// uniqTest.PushBack(2);
	// uniqTest.PushBack(2);
	// uniqTest.PushBack(3);
	// uniqTest.PushBack(4);
	// uniqTest.PushBack(5);
	// uniqTest.Unique([](int a, int b) { return (a + b) % 2 == 0; }); // Remove if sum is even
	// std::cout << "uniqTest after Unique(predicate): ";
	// for (auto it = uniqTest.Begin(); it != uniqTest.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Merge with Predicate--------------" << std::endl;
	// List<int> m1, m2;
	// m1.PushBack(1); m1.PushBack(3); m1.PushBack(5);
	// m2.PushBack(2); m2.PushBack(4); m2.PushBack(6);
	// m1.Merge(m2, [](int a, int b) { return a < b; });
	// std::cout << "m1 after Merge with predicate: ";
	// for (auto it = m1.Begin(); it != m1.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Splice(single element)--------------" << std::endl;
	// List<int> a, b;
	// a.PushBack(1); a.PushBack(2);
	// b.PushBack(10); b.PushBack(11);
	// auto itSingle = b.Begin();
	// Advance(itSingle, 1); // points to 11
	// a.Splice(a.Begin(), b, itSingle);
	// std::cout << "a after splice single element: ";
	// for (auto it = a.Begin(); it != a.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	// std::cout << "b after removing element: ";
	// for (auto it = b.Begin(); it != b.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;
	//
	// std::cout << std::endl << "--------------Test Splice(range)--------------" << std::endl;
	// List<int> source;
	// source.PushBack(7); source.PushBack(8); source.PushBack(9);
	// auto first = source.Begin();
	// auto last = source.End();
	// a.Splice(a.End(), source, first, last);
	// std::cout << "a after splice range: ";
	// for (auto it = a.Begin(); it != a.End(); ++it)
	// 	std::cout << *it << " ";
	// std::cout << std::endl;

END_TEST_MAIN_FUNCTION( List )
END_TEST( List )


#endif
