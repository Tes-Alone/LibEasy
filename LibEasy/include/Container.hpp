/*
 * File Name:	Container.hpp
 * Date/Time:
 * 	Start: 	2019-12-06 15:30 week:5
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_CONTAINER_H_
#define _EASY_CONTAINER_H_

#include "BaseTypes.hpp"
#include "String.hpp"
#include "System.hpp"
#include "StringBuffer.hpp"
#include "Array.hpp"
#include "Iterator.hpp"
#include "AutoPointer.hpp"

namespace Easy {

/**
 * \class Container
 * 
 * \brief A tool for maintaining containers.
 * 
 * @author Tes Alone
 * @since 1.0
*/

class Container {
public:
	/**
	 * \brief To %Array.
	 * 
	 * Class C needs implements iterator().
	 *
	 * @param c a container.
	 * @return a array composed with c's elements.
	*/
	template<class C, class E>
	static Array<E> toArray(const C& c) {
		Array<E> arr{c.size()};
		AutoPointer<Iterator<E>> iter{c.iterator()};
		for (e_int i=0; i<arr.length(); i++) {
			arr[i] = (*iter).current();
			(*iter).advance();
		}
		return System::move(arr);
	}

	/**
	 * \brief Add %Array.
	 * 
	 * Class C needs implements add().
	 * 
	 * @param c a container.
	 * @param arr the array be added.
	*/
	template<class C, class E>
	static e_void addAll(C& c, const Array<E>& arr) {
		for (e_int i=0; i<arr.length(); i++) {
			c.add(arr[i]);
		}
	}
	
	/**
	 * \brief sort.
	 * 
	 * This method is a simple quicksort implement.
	 * <br/>
	 * Class C needs implement iterator() and add().
	 * Class E needs implement compare().
	 * <br/>
	 * If receiver is empty, the method do nothing.
	 * <br/>
	 * The sorted range include start, exclude end, if start equals end, end will 
	 * be added one.
	 *
	 *  @param c sorted container.
	 *	@param start start of sorted range, default is 0, can be a number between 0 and C::size()-1;
	 *	@param end  end of sorted range, default is -1, indicates the C::size() , can be a number between 0 and C::size().
	 *	@throw if start < 0 or start >= C::size(), throw an IndexOutOfBoundsException.
	 *	@throw if end < 0 or end > C::size(), throw an IndexOutOfBoundsException.
	 *	@throw if start > end, throw an IllegalArgumentException.
	*/
	template<class C, class E>
	static C sort(const C& c, e_int start=0, e_int end=-1) {
		Array<E> arr = Container::toArray<C, E>(c);
		arr.sort(start, end);
		C result{};
		Container::addAll<C, E>(result, arr);
		return System::move(result);
	}
	
	/**
	 * \brief Merge %Container.
	 * 
	 * Class C1 and C2 need implement iterator().
	 * Class C3 needs implement add();
	 *
	 * c1 and c2 must be ordered container.
	 * 
	 * @param c1 a container.
	 * @param c2 another container.
	 * @return a container after merging c1 and c2, and is a ordered container.
	*/
	template<class C1, class C2, class C3, class E>
	static C3 merge(const C1& c1, const C2& c2) {
		AutoPointer<Iterator<E>> it1{c1.iterator()};
		AutoPointer<Iterator<E>> it2{c2.iterator()};
		
		C3 merged{};

		while (!it2->isEnd() && !it2->isEnd()) {
			if (it1->current().compare(it2->current()) == -1) {
				merged.add(it1->current());
				it1->advance();
			} else {
				merged.add(it2->current());
				it2->advance();
			}
		}

		if (it1->isEnd()) {
			while (!it2->isEnd()) {
				merged.add(it2->current());
				it2->advance();
			}
		} else {
			while (!it1->isEnd()) {
				merged.add(it1->current());
				it1->advance();
			}
		}

		return System::move(merged);
	}
	
	/**
	 * \brief Get max element.
	 * 
	 * Class E needs implement compare().
	 * Class C needs implement iterator().
	 * 
	 * @param c a container.
	 * @return c's max element.
	 * @throw if c is empty, throws EmptyContainerException.
	*/
	template<class C, class E>
	static E& max(const C& c) {
		AutoPointer<Iterator<E>> iter{c.iterator()};
		if (!(*iter).isEnd()) {
			E& _max = (*iter).current();
			(*iter).advance();
			while (!(*iter).isEnd()) {
				E& cur = (*iter).current();
				if (_max.compare(cur) == -1) {
					_max = cur;
				}
				(*iter).advance();
			}
			return _max;
		}

		throw  EmptyContainerException{__func__, __LINE__, String{L"Container"}};
	}

	/**
	 * \brief Get min element.
	 * 
	 * Class E needs implement compare().
	 * Class C needs implement iterator().
	 * 
	 * @param c a container.
	 * @return c's min element.
	 * @throw if c is empty, throws EmptyContainerException.
	*/
	template<class C, class E>
	static E& min(const C& c) {
		AutoPointer<Iterator<E>> iter{c.iterator()};
		if (!(*iter).isEnd()) {
			E& _min = (*iter).current();
			(*iter).advance();
			while (!(*iter).isEnd()) {
				E& cur = (*iter).current();
				if (_min.compare(cur) == 1) {
					_min = cur;
				}
				(*iter).advance();
			}
			return _min;
		}

		throw  EmptyContainerException{__func__, __LINE__, String{L"Container"}};
	}

	/**
	 * \brief Check equality.
	 * 
	 * The method compare c1 and c2, if c1's elements equals 
	 * c2's, one by one, return true, otherwise retunr false.
	 * <br/>
	 * Class C1 and C2 need implement iterator().
	 * Class E needs implement equals().
	 * 
	 * @param c1 a container .
	 * @param c2 another container .
	 * @return  compared result.
	 * */
	template<class C1, class C2, class E>
	static e_bool equals(const C1& c1, const C2& c2) {
		AutoPointer<Iterator<E>> it1{c1.iterator()};
		AutoPointer<Iterator<E>> it2{c2.iterator()};
		if (c1.size() != c2.size()) {
			return false;
		}
		while (!(*it1).isEnd()) {
			if (!((*it1).current().equals((*it2).current()))) {
				return false;
			}
			(*it1).advance();
			(*it2).advance();
		}
		return true;
	}

	/**
	 * \brief check order.
	 * 
	 * The method compare c1 and c2, if c1's
	 * elements is bigger than c2's, one by one, return 1, 
	 * equals return 0, otherwise return -1.
	 * <br/>
	 * Class C1 and C2 need implement iterator().
	 * Class E needs implement compare().
	 * 
	 * @param c1 a container .
	 * @param c2 another container .
	 * @return  compared result.
	 * */
	template<class C1, class C2, class E>
	static  e_int compare(const C1& c1, const C2& c2) {
		AutoPointer<Iterator<E>> it1{c1.iterator()};
		AutoPointer<Iterator<E>> it2{c2.iterator()};

	    while (!(*it1).isEnd() && !(*it2).isEnd()) {
			e_int state = (*it1).current().compare((*it2).current());
	        if (state != 0) {
				return state;
			}
			it1->advance();
			it2->advance();
	    }

	    if ((*it1).isEnd() && (*it2).isEnd()) {
			return 0;
		}

		return (!(*it1).isEnd()) ? 1 : -1;
	}

	/**
	 * \brief To string represent．
	 * 
	 * Class C needs implement iterator().
	 * Class E needs implement toString().
	 * 
	 * @returns c`s string represent.
	*/
	template<class C, class E>
	static String toString(const C& c) {
		StringBuffer sb{};
		sb.append(String{L"["});
		AutoPointer<Iterator<E>> iter{c.iterator()};
		while (!(*iter).isEnd()) {
			sb.append((*iter).current().toString());
			sb.append(String{L","});
			(*iter).advance();
		}
		if (sb.length() > 1) {
			return System::move(sb.removeTail(sb.length()-1).append(String{L"]"}).toString());
		}
		return System::move(sb.append(String{L"]"}).toString());
	}	
private:
	Container() =delete;
	~Container() =delete;
	Container(Container& copy)  = delete;
	Container(Container&& move)  = delete;
	Container& operator=(Container& copy)  = delete;
	Container& operator=(Container&& move)  = delete;
}; // Container
} // Easy

#endif // _EASY_CONTAINER_H_

