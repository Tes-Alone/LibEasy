
/*
 * File name: Easy.hpp
 *
 * Date/Time:
 *		start: 2019年10月03日 14时48分27秒 周4
 *		end:
 *
 * Version: 1.0
 *
 * Author: Tes Alone
 *
 * Note:
*/

#ifndef LEC_EASY_H_
#define LEC_EASY_H_

/**	
	\mainpage Easy API.
	
	<h3>Introduction</h3>
	<p>
	Easy API is a very tiny C++ class library. It includes some very very basic classes,
	like Integer, the signed 32 bits integer wrapper, but it made some sense for me.
	It help me built a lot web pages and wrapped JSoso, which code editor for edit this document,
	in the past.
	</p>
	
	<p>
	The naming convention of Easy`s API mimics Java SE API, like StringBuffer,
	HashSet, etc. But Easy can not be powerful as Java, now. (of course.)
	And for the moment, it is just for me filling my daily time, both by coding 
	it and using it.
	</p>
	
	<p>
	Easy can only run on Windows, (or more restricted, on Windows 7, 
	I have no enough device to test.)
	</p>
	
	<p>
	As above, Easy mimics Java. But it use few inheritance (except Exceptions.)
	Because the polymorphism of C++ need reference and pointer variables, and the 
	Containers of Easy use value semantics, so Easy can not use polymorphism.
	But Easy use the other polymorphism supplied by C++ --- the templates.
	</p>
	<p>
	In Easy, if an object of some class is used as an element of a Container, like HashSet,
	the some class needs implement the equals() and the hashCode() method, but there is no
	inheritance, it needs do the full thing. For the specific methods a such class needs 
	provide, see the document of the specific Container.
	</p>
	
	<hr/>
	
	<h3>Class Type</h3>
	<h4>Basic Classes</h4>
	<ul>
		<li>Easy::Bool</li>
		<li>Easy::UByte</li>
		<li>Easy::Byte</li>
		<li>Easy::UShort</li>
		<li>Easy::Short</li>
		<li>Easy::UInteger</li>
		<li>Easy::Integer</li>
		<li>Easy::ULong</li>
		<li>Easy::Long</li>
		<li>Easy::Character</li>
		<li>Easy::Float</li>
		<li>Easy::Double</li>
		<li>Easy::String</li>
		<li>Easy::File</li>
		<li>Easy::Date</li>
	</ul>
	<h4>Containers</h4>
	<ul>
		<li>Easy::String</li>
		<li>Easy::StringBuffer</li>
		<li>Easy::BasicArray</li>
		<li>Easy::BasicBuffer</li>
		<li>Easy::Array</li>
		<li>Easy::Vector</li>
		<li>Easy::List</li>
		<li>Easy::Stack</li>
		<li>Easy::Queue</li>
		<li>Easy::Heap</li>
		<li>Easy::HashSet</li>
		<li>Easy::HashMap</li>
		<li>Easy::TreeSet</li>
		<li>Easy::TreeMap</li>
		<li>Easy::BitSet</li>
		<li>Easy::Container</li>
	</ul>
	<h4>IO Classes</h4>
	<ul>
		<li>Easy::ANSIFileInputStream</li>
		<li>Easy::FileInputStream</li>
		<li>Easy::FileOutputStream</li>
		<li>Easy::FileReader</li>
		<li>Easy::FileWriter</li>
		<li>Easy::IOUtility</li>
	</ul>
	<h4>Pure Tool</h4>
	<ul>
		<li>Easy::Bytes</li>
		<li>Easy::Math</li>
		<li>Easy::Console</li>
		<li>Easy::System</li>
		<li>Easy::Test</li>
		<li>Easy::Container</li>
		<li>Easy::IOUtility</li>
	</ul>
	<h4>Utility</h4>
	<ul>
		<li>Easy::LXML::LXMLDocument</li>
		<li>Easy::Regex::Pattern / Easy::Regex::Matcher</li>
		<li>Easy::Functor</li>
		<li>Easy::SharedPointer / Easy::WeakPointer / Easy::EnableSharedFromThis</li>
		<li>Easy::AutoPointer</li>
		<li>Easy::ArgsParser</li>
		<li>Easy::StringBuffer</li>
		<li>Easy::Iterator</li>
		<li>Easy::BigMemory</li>
		<li>Easy::Thread</li>
	</ul>
	
	<h4>Exceptions</h4>
	<p>
	When Easy reach a null pointer, it will throw a Easy::NullPointerException.<br/>
	
	When an Easy Container (include String) fed a out of bounds index, 
	it will throw an Easy::IndexOutOfBoundsException.<br/>
	
	When an Easy function fed a illegal argument, it will throw an 
	Easy::IllegalArgumentException.<br/>
	
	Ans so on.
	</p>
	
	<hr/>
	
	<h3>Installation and Usage</h3>
	<p>
	Easy includes a static library, named <b>libeasystatic.a</b>; a dynamic library,
	the index file named <b>libeasy.a</b>, the dll file named <b>lieasy.dll</b>;
	and a suit header files. You need setup it as other libraries.
	</p>
	
	<h4>Enter Point</h4>
	<p>
	For make sure the api`s look and feel consistency, Easy hides the native main() function,
	and provides a new <b>Enter Point</b>, the prototype is:
	<pre>
		<code>
			<b>e_int e_main(Array<String>& args);</b>
		</code>
	</pre>
	A program build with Easy needs implement the method.
	</p>
	
	<h4>Some Convention</h4>
	In the docs for Easy, 
	<ol>
		<li>
			if a class has docs for dtor, copies and moves, it can use as a
			field of another class.
		</li>
		<li>
			if a class has no docs for dtor, copies and moves, it can use as
			a field of another class.
		</li>
		<li>
			if a class has equals(), compare(), hashCode(), toString(), it can use 
			as a element type of containers.
		</li>
		<li>
			if a class has docs for dtor, but copies and moves, and extends 
			EnableSharedFromThis, it can wrapped by a shared pointer.
		</li>
		<li>
			if a class extends EnableSharedFromThis, it can wrapped by 
			a shared pointer.
		</li>
	</ol>
	it is not strict, just a guidance.
	
	<hr/>
	
	@author Tes Alone
	@version 1.0
*/

/**
 * \namespace Easy
 * 
 * \brief The main name space of Easy.
 * */
namespace Easy {
 	// empty for doc.
} // Easy

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "Exception.tcc"
#include "Bool.hpp"
#include "UInteger.hpp"
#include "Integer.hpp"
#include "Float.hpp"
#include "Character.hpp"
#include "String.hpp"
#include "String.tcc"
#include "Bytes.hpp"

#include "StringBuffer.hpp"

#include "Math.hpp"
#include "Date.hpp"

#include "AutoPointer.hpp"
#include "SharedPointer.hpp"
#include "Console.hpp"
#include "System.hpp"
#include "Functor.hpp"

#include "ANSIFileInputStream.hpp"
#include "FileInputStream.hpp"
#include "FileReader.hpp"
#include "FileOutputStream.hpp"
#include "FileWriter.hpp"
#include "File.hpp"
#include "IOUtility.hpp"

#include "BasicArray.hpp"
#include "BasicArray.tcc"
#include "BasicBuffer.hpp"
#include "BasicBuffer.tcc"
#include "Container.hpp"
#include "Iterator.hpp"
#include "Array.hpp"
#include "Vector.hpp"
#include "List.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "Heap.hpp"
#include "TreeSet.hpp"
#include "TreeMap.hpp"
#include "HashSet.hpp"
#include "HashMap.hpp"
#include "MapEntry.hpp"
#include "BitSet.hpp"

#include "PtrVector.hpp"

#include "Test.hpp"

#include "LXML.hpp"

#include "ArgsParser.hpp"
#include "TimeCounter.hpp"

#include "Regex/Regex.hpp"

#include "BigMemory.hpp"

#include "Thread.hpp"

#endif // LEC_EASY_H_
