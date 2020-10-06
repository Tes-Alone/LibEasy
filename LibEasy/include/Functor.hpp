/*
 * File Name:	Functor.hpp
 * Date/Time:
 * 	Start: 	2020-03-22 12:25:24 0
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_FUNCTOR_H_
#define _EASY_FUNCTOR_H_

namespace Easy {

/**
	\class Functor
	
	\brief A base interface for functors.
	
	@author Tes Alone
	@since 1.0
*/

template<class T, class ...Args>
class Functor {
public:
	/**
		\brief ctor.
		
		Do nothing.
	*/
	Functor() {}
	
	/**
		\brief dtor.
		
		Do nothing.
	*/
	virtual ~Functor() {}
	
	/**
		\brief Function definition.
		
		Child classes need implement this method.
	*/
	virtual T operator() (Args... args) =0;

private:
	Functor(Functor& copy)  = delete;
	Functor(Functor&& move)  = delete;
	Functor& operator=(Functor& copy)  = delete;
	Functor& operator=(Functor&& move)  = delete;
}; // Functor
} // Easy

#endif // _EASY_FUNCTOR_H_

