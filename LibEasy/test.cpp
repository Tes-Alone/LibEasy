
/*
 * File name: test.cpp
 * 
 * Description:
 *		This is a c++ implement file.
 *
 * Start Date: 2019年10月03日 14时45分18秒 周4
 * End   Date:
 *
 * Version: 1.0
 *
 * Author: Tes Alone.
*/

#include <windows.h>

#include "Easy.hpp"

using namespace Easy;

#include <pthread.h>

e_void test0(); 
e_void test1();
e_void test2();
e_void test3();
e_void test4();
e_void test5();
e_void test6();
e_void test7();
e_void test8();
e_void test9();
e_void test10();
e_void test11();
e_void test12();
e_void test13();
e_void test14();
e_void test15();
e_void test16();
e_void test17();
e_void test18();
e_void test19();
e_void test20();
e_void test21();
e_void test22();
e_void test23();
e_void test24();

using namespace Easy::LXML;

e_void test25() {
	ArgsParser argsParser{Array<String>{}};
}

e_void test24() {
	LXMLDocument doc{String{L"F:\\EasyTest\\demo.xml"}};
	SharedPointer<Node> root = doc.parse().getRoot();
	//root->getNodeList().getNodeByAttr(String{L"demo"}, String{L"test"});
	//root->getAttrMap().getAttr(String{L"demo"});
	root->save(String{L"F:\\EasyTest\\demo1.xml"});
	//root->getNodeList().getNodeByAttr(String{L"demo"}, String{L"test"});
	Console::outln(String{L"#"} + root->getAttrMap().getAttr(String{L"demo"}) + String{L"#"});
	Console::outln(doc);
}

e_void test0() {
	TreeMap<String, String> map;
	map.put(String{L"Demo"}, String{L"Test"});
	map.put(String{L"Demo1"}, String{L"Test1"});
	map.put(String{L"Demo2"}, String{L"Test2"});
	map.put(String{L"Demo3"}, String{L"Test3"});
	Console::outln(map);
	TreeSet<String> set = map.keySet();
	Console::outln(set);
	Console::outln(map);
}

e_void test1() {
	Console::setColor(Console::FR_RED | Console::BK_BLACK);
	Console::outln(L"RED!");
	Console::errln(L"RED?");
	Console::setColor(Console::FR_WHITE | Console::BK_BLACK);
}

e_void test2() {
	e_int oc = Console::getColor();
	Console::out(L"gcc: ");
	Console::setColor(Console::FR_RED | Console::BK_BLACK | Console::FR_HIGHLIGHT);
	Console::err(L"fatal error");
	Console::setColor(oc);
	Console::outln(L": file demo.c read failed!");
}

e_void test3() {
	Test::equals(1, Character::isNonCharacter(0x10ffff), true);
	Test::equals(2, Character::isNonCharacter(0x1fffe), true);
	Test::equals(3, Character::isNonCharacter(0x1ffff), true);
	Test::equals(4, Character::isNonCharacter(0x2ffff), true);
	Test::equals(5, Character::isNonCharacter(0x2fffe), true);
	Test::equals(6, Character::isNonCharacter(0x5fffe), true);
	Test::equals(7, Character::isNonCharacter(0x9ffff), true);
	Test::equals(8, Character::isNonCharacter(0x9fffe), true);
	Test::equals(9, Character::isNonCharacter(0xffff), true);
	Test::equals(10, Character::isNonCharacter(0xfffe), true);
	Test::equals(11, Character::isNonCharacter(0x7ffe), false);
}

e_void test3(Array<String>& args) {
	ArgsParser argsParser{args};
	argsParser.addHelpHeader(String{L"用法:\n\t"} + args[0] 
				+ String{L" -in fileOrFolder -out output [-filter extList]"});
	argsParser.addAidHelp(String{L"-"} * 60);
	argsParser.addAidHelp(String{L"选项:\n"});
	argsParser.addFlag(L"help", L"输出帮助信息");
	argsParser.addFlag(L"version", L"输出版本信息");
	argsParser.addOption(L"in", L"fileOrFolder", L"指定处理的文件或目录");
	argsParser.addOption(L"out", L"output", L"指定输出文件");
	argsParser.addOption(L"filter", L"extList", L"指定要过滤的文件类型, 以冒号分隔");
	argsParser.addAidHelp(String{L"-"} * 60);
	argsParser.addHelpTail(String{L"用例:\n\t"} + args[0] + String{L" -in demo.txt -out 1.txt"});
	argsParser.addHelpTail(String{L"\t"} + args[0] + String{L" -in .\\src -filter exe:o:a -out 1.txt"});
	
	try {
		argsParser.parse();
	} catch (ArgsParseException& e) {
		argsParser.printUsage();
		System::exit(-1);
	}
	
	if (argsParser.hasNoArguments()) {
		argsParser.printUsage();
		System::exit(0);
	}
	
	Console::outln(argsParser);
}

e_void test10() {
	File f1{".\\System.o\\"};
	File f2{"..\\LibEasy\\System.o\\"};
	Test::equals(1, f1.exists(), false);
	Test::equals(2, f2.exists(), false);
	Console::outln(f1);
	Console::outln(f2);
	Test::equals(3, f1, f2);
}


// 昨天修改了 LXML 的输出方式.
// 昨天新增了 TimeCounter 类.
// 今天给 Array 添加了 equals(), compare(), hashCode() 方法.
// 今天修改了 ArgsParser 的 -- 和 - bug.

e_void test13(Array<String>& args) {
	Console::outln(args.indexOf(String{L"demo"}));
}

e_void test14() {
	Array<Integer> arr1{11};
	Array<Integer> arr2{10};
	for (e_int i=0; i<10; i++) {
		arr1[i] = i;
		arr2[i] = i;
	}
	arr1[10] = 100;
	Console::outln(arr1.equals(arr2));
	Console::outln(arr2.compare(arr1));
}

e_void test12() {
	TimeCounter tc;
	tc.start();
	test11();
	tc.stop();
	Console::out("Time Used: ");
	Console::out(tc.getTimeMillis());
	Console::outln(" mills.");
}

using namespace Easy::Regex;

e_void test11() {
	SharedPointer<Pattern> re = Pattern::compile(L"(.*(abb))abcdef");
	Console::outln("compiled");
	re->printPattern();
	Matcher matcher = re->matcher(L"efgabbabbabbabbabcdef");
	Console::outln(matcher.matches());
	Console::outln(re);
}

e_void test16() {
	Vector<Integer> v;
	for (int i=0; i<10000000; i++) {
		v.add(Math::nextInt());
	}
	TimeCounter tc;
	tc.start();
	v.sort();
	tc.stop();
	Console::out("Time Used: ");
	Console::outln(tc.getTimeMillis());
	Console::outln(v[0]);
	Console::outln(v[1]);
}

e_void test17() {
	List<Integer> l;
	for (e_int i=0; i<100; i++) {
		l.insert(i);
		l.remove();
	}
	Console::outln(l);
	AutoPointer<Iterator<Integer>> iter{l.iterator()};
	Console::outln(iter);
}

e_void test18() {
	TimeCounter tc;
	tc.start();
	//std::stack<Integer> s;
	Stack<Integer> s;
	for (int i=0; i<10000000; i++) {
		s.push(i);
	}
	tc.stop();
	Console::out("Time Used: ");
	Console::outln(tc.getTimeMillis());
}

e_void test19() {
	TimeCounter tc;
	tc.start();
	//std::stack<Integer> s;
	Array<Integer> arr{10000000};
	for (int i=0; i<arr.length(); i++) {
		arr[i] = Math::nextInt();
	}
	Heap<Integer> hp{arr};
	for (e_int i=0; i<arr.length(); i++) {
		arr[i] = hp.removeFirst();
	}
	/*
	for (e_int i=0; i<hp.size(); i++) {
		Console::outln(hp.getFirst());
	}
	Integer t = hp.remove(5);
	while (!hp.empty()) {
		Console::outln(hp.removeFirst());
	}*/
	tc.stop();
	Console::outln(hp[0]);
	Console::out("Time Used: ");
	Console::outln(tc.getTimeMillis());
}

e_void test20() {
	List<Integer> l;
	Console::outln(l);
	for (e_int i=0; i<1000; i++) {
		l.add(i);
	}
	for (e_int i=0; i<500; i++) {
		l.remove();
	}
	for (e_int i=0; i<10; i++) {
		l.insert(i);
	}
	TimeCounter tc;
	tc.start();
	for (e_int i=0; i<l.size(); i++) {
		l.get(i);
	}
	tc.stop();
	Console::out("Time Used: ");
	Console::outln(tc.getTimeMillis());
	Console::outln(l);
}

e_void test21() {
	HashSet<Integer> set;
	set.add(100);
	set.add(101);
	set.add(102);
	set.remove(100);
	set.remove(101);
	set.remove(102);
	Console::outln(set);
}

e_void test23() {
	Vector<Integer> v;
	for (e_int i=0; i<3; i++) {
		v.add(i);
	}
	Array<Integer> a = Container::toArray<Vector<Integer>, Integer>(v);
	v[0] = 99;
	Console::outln(v);
	Console::outln(a);
}


// 明天修改一下 Exception 类, 可以提示出错函数.
e_void testBitSet() {
	BitSet b{5};
	BitSet c{5};
	c.setBit(4);
	c.setBit(1);
	b.setBit(2);
	b.setBit(3);
	Console::outln(String{L"b:"} + b);
	Console::outln(String{L"c:"} + c);
	b.complements();
	Console::outln(String{L"b:"} + b);
	b.unions(c);
	Console::outln(String{L"b:"} + b);
	b.symDiffers(c);
	Console::outln(String{L"b:"} + b);
	b.joins(c);
	Console::outln(String{L"b:"} + b);
	Test::equals(1, b, c);
	Test::equals(2, b.isSet(3), true);
}

e_void testBitSet2() {
	BitSet b{10};
	BitSet c{10};
	for (e_int i=0; i<6; i++) {
		e_int m = Math::abs(Math::nextInt()) % 10;
		e_int n = Math::abs(Math::nextInt()) % 10;
		b.setBit(m);
		c.setBit(n);
	}
	Console::outln(b);
	Console::outln(c);
	b.unions(c);
	Console::outln(b);
	Test::equals(1, c.isSuperOf(c), true);	
	Test::equals(2, b.isSuperOf(c), true);	
	Test::equals(3, b.isProperSuperOf(c), true);	
	Test::equals(4, c.isSubOf(c), true);	
	Test::equals(5, c.isProperSubOf(b), true);
	Console::outln(c.toUBytes());
}

e_void testCM() {
	HashSet<Integer> set;
	set.add(100);
	set.add(1000);
	Iterator<Integer>* iter = set.iterator();
	Console::outln(iter->current());
	delete iter;
}

class E : public EnableSharedFromThis<E> {
public:
	E() {}
	~E() {
		Console::outln(1000);
	}
}; // E

e_void testBitSet3() {
		BitSet bitset{320};
	for (e_int i=0; i<251; i++) {
		bitset.setBit(i);
		/*Console::out(i);
		Console::out(":");
		Console::outln(bitset.isSet(i));
		*/
	}
	
	Console::outln(bitset.isSet(219));
	for (e_int i=251; i<bitset.length(); i++) {
		bitset.cleanBit(i);
//		Console::out(i);
//		Console::out(":");
//		Console::out(bitset.isSet(219));
//		Console::out(":");
//		Console::outln(bitset.isSet(i));
	}
	bitset.setBit(310);
	bitset.setBit(311);
	bitset.setBit(312);
	bitset.setBit(313);
	for (e_int i=0; i<bitset.length(); i++) {
//		Console::out(i);
//		Console::out(":");
//		Console::outln(bitset.isSet(i));
	}
	Console::outln(bitset);
	Console::outln(bitset.toUBytes());
}

e_void testBigMemory() {
	BigMemory bm{256};
	UByteArray buf{8};
	for (e_int i=0; i<8; i++) {
		buf[i] = i;
	}
	bm.write(buf, 100, 108);
	bm.write(System::move(buf), 92, 100);
	bm.write(buf, 98, 98+8);
	UByteArray arr = bm.read(0, bm.getMaxSize());
	Console::outln(arr);
}

e_void testBigMemory2() {
	BigMemory bm{8};
	UByteArray buf{4};
	for (e_int i=0; i<buf.length(); i++) {
		buf[i] = i;
	}
	bm.write(buf, 0, 4);
	bm.write(buf, 4, 8);
	bm.write(buf, 2, 6);
	bm.write(System::move(UByteArray{2}), 5, 7);
	bm.write(System::move(UByteArray{8}), 0, 8);
	bm.write(buf, 2,6);
	Console::outln(bm);
}

//Vector<Integer> vec;
//SharedPointer<Easy::Lock> lk;
e_int c = 0;

class Worker : public Thread {
	e_int _i;
public:
	Worker(e_int i) {
		_i = i;
	}
	void run() override {
//		Thread::lock();
//		for (e_int i=0; i<50; i++) {
//			//vec.add(Integer{_i + i});
//			c++;
//		}
//		Thread::unlock();
	}
	
	~Worker() {
		Console::outln("dtor.");
	}

}; // Worker

e_void testThread() {
	SharedPointer<Easy::Lock> lk = SharedPointer<Lock>{new Lock()};
	SharedPointer<Thread> t1{new Worker(1)};
	SharedPointer<Thread> t2{new Worker(2)};
	SharedPointer<Thread> t3{new Worker(3)};
	t1->setPriority(Thread::PRIORITY_HIGHEST);
	t1->setLock(lk);
	t2->setLock(lk);
	//t3->setLock(lk);
	t1->start();
	t2->start();
	//t3->start();
	t2->join();
	t1->join();
	t3->join();
	Console::outln(c);
	//throw NullPointerException(__func__, __LINE__);
}

#define LOOP 1000
IntArray buffer{LOOP};
e_int ic = 0;

class Producer : public Thread {	
public:
	void run() override {
		for (e_int i=1; true; i++) {
			lock();
			while (ic != 0) wait();
			ic = i;
			wakeup();
			unlock();
		}
	}
}; // Producer

class Consumer : public Thread {
public:
	void run() override {
		for (e_int i=0; true; i++) {
			lock();
			while (ic == 0) {wait();}
			Console::outln(ic);
			ic = 0;
			wakeup();
			unlock();
		}
	}	
}; // Consumer

template<typename T>
e_void testInteger() {
	Console::outln(T::toString(T::MAX, 2));
	Console::outln(T::toString(T::MIN, 2));
	Console::outln(T::toString(T::MAX, 8));
	Console::outln(T::toString(T::MIN, 8));
	Console::outln(T::toString(T::MAX, 10));
	Console::outln(T::toString(T::MIN, 10));
	Console::outln(T::toString(T::MAX, 16));
	Console::outln(T::toString(T::MIN, 16));
}

template<typename T>
e_void testInteger2() {
	String str = T::toString(T::MAX, 2);
	Console::outln(T::parse(str, 2));
	
	str = T::toString(T::MAX, 8);
	Console::outln(T::parse(str, 8));
	
	str = T::toString(T::MAX, 10);
	Console::outln(T::parse(str, 10));
	
	str = T::toString(T::MAX, 16);
	Console::outln(T::parse(str, 16));
}

template<typename T>
e_void testInteger3() {
	String str = T::toString(T::MIN, 2);
	Console::outln(T::parse(str, 2));
	
	str = T::toString(T::MIN, 8);
	Console::outln(T::parse(str, 8));
	
	str = T::toString(T::MIN, 10);
	Console::outln(T::parse(str, 10));
	
	str = T::toString(T::MIN, 16);
	Console::outln(T::parse(str, 16));
}

e_int e_main(Array<String>& args) {
	Console::outln("Hello World.");
	return 0;
}