
/*
 * File name: Error.hpp 
 *
 * Date/Time:
 *		start: 2019年10月03日 14时55分45秒 周4
 *		end:
 *
 * Version: 1.0
 * 
 * Author: Tes Alone
 *
 * Note:
*/

#ifndef LEC_EXCEPTION_H_
#define LEC_EXCEPTION_H_

#include "BaseTypes.hpp"

namespace Easy {

class String;

/**
	\class Exception
	
	\brief Base for all other excption classes in Easy.
	
	@author Tes Alone
   	@since  1.0
*/

class ExitException {
public:
	ExitException(e_int code) {
		_code = code;
	}

	e_int getExitCode() {
		return _code;
	}
private:
	e_int _code;
}; // ExitException



class Exception {
private:
	String* m_msg;
	String* m_func;
	e_int   m_line;
public:
	/**
	 * \brief ctor.
	 * 
	 * @param msg exception's message.
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * */
	Exception(const String& msg, const String& func, e_int line);
	
	/**
	 * \brief dtor.
	 * 
	 * Do nothing but for inheritance.
	 * */
	virtual ~Exception();
	
	/**
	 * \brief Get exception type.
	 * 
	 * @return type info for exception.
	 * */
	virtual String what() const;
	
	/**
	 * \brief Get message of exception.
	 * 
	 * The message of a exception describes the exception's details.
	 * But because of C++'s native characteristic, the details may be not 
	 * very enough, for example, there is no details about the location in
	 * source file where the exception threw.
	 * 
	 * @return message of a exception.
	 * */
	String getMessage() const;
	
	/**
	 * \brief Get function which thrown the object.
	 * 
	 * @return function.
	 * */
	String getFunction() const;
	
	/**
	 * \brief Get line where thrown the object.
	 * 
	 * @return line.
	 * */
	inline e_int getLine() const {
		return m_line;
	}
	
}; // Exception

/**
 * \class NullPointerException
 * 
 * \brief nullptr pointer exception.
 * 
 * Be threw when program reach a nullptr.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class NullPointerException : public Exception {
public:
	/**
	 * \brief ctor.
	 * */
	NullPointerException(const String& func, e_int line);
	
	String what() const override;
	
}; // NullPointerException

/**
 * \class IllegalArgumentException.
 * 
 * \brief illegal argument exception.
 * 
 * Be threw when a method is fed by a illegal argument, like a empty string.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class IllegalArgumentException : public Exception {
public:
	/**
	 * \brief ctor.
	 * */
	IllegalArgumentException(const String& func, e_int line);
	
	String what() const override;
	
}; // IllegalArgumentException


/**
 * \class IndexOutOfBoundsException
 * 
 * \brief index out of bounds exception.
 * 
 * Be threw when a container's method is fed a bad index.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class IndexOutOfBoundsException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param container container name.
	 * @param len container length.
	 * @param index the bad index.
	 * */
	IndexOutOfBoundsException(const String& func, e_int line, 
								const String& container, e_int len, e_int index);
	
	String what() const override;
	
}; // IndexOutOfBoundsException


/**
 * \class NotInFunctionDomainException
 * 
 * \brief argument not in function domain exception.
 * 
 * Be threw when a math function is fed a bad argument.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class NotInFunctionDomainException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * X needs implement toString().
	 * 
	 * @param func function name.
	 * @param line code line where thrown the exception.
	 * @param x bad argument. 
	 * */
	template<class X>
	NotInFunctionDomainException(const String& func, e_int line, X x);
		
	String what() const override;
	
}; //NotInFunctionDomainException


/**
 * \class NumberFormatException
 * 
 * \brief number string format illegal exception.
 * 
 * Be threw when converting string to number and the string is illegal.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class NumberFormatException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param num ill-formed string.
	 * */
	NumberFormatException(const String& func, e_int line, const String& num);
		
	String what() const override;
	
}; // NumberFormatException


class NumberOverflowException : public Exception {

public:
	NumberOverflowException(const String& func, e_int line, const String& num);
	
	String what() const override;
}; // NumberOverflowException

class FloatExpException : public Exception {

public:
	FloatExpException(const String& func, e_int line, const String& num);
	
	String what() const override;
}; // FloatExpException

/**
 * \class FormatedStringFormatException
 * 
 * \brief formated string format exception.
 * 
 * Throw when String::format() fed a illegal formatted string.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class FormatedStringFormatException  : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param str the wrong string.
	 * */
	FormatedStringFormatException(const String& func, e_int line, const String& str);
		
	String what() const override;
	
}; // FormatedStringFormatException


/**
 * \class IOException
 * 
 * \brief IO exception.
 * 
 * Throw when read from or write to a file failed.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class IOException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param file the wrong file.
	 * */
	IOException(const String& func, e_int line, const String& file);
		
	String what() const override;
	
}; // IOException

/**
 * \class FileOpenFailedException
 * 
 * \brief %File open failed exception.
 * 
 * Throw when a file open failed.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class FileOpenFailedException  : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param file the wrong file.
	 * */
	FileOpenFailedException(const String& func, e_int line, const String& file);
		
	String what() const override;
	
}; //FileOpenFailedException


/**
 * \class FileCloseFailedException
 * 
 * \brief %File close failed exception.
 * 
 * Throw when a file close failed.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class FileCloseFailedException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param file the wrong file.
	 * */
	FileCloseFailedException(const String& func, e_int line, const String& file);
		
	String what() const override;
	
}; // FileCloseFailedException


/**
 * \class FileReachEOFException
 * 
 * \brief %File reach EOF exception.
 * 
 * Throw when FileReader::readLine() reach EOF.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class FileReachEOFException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param file the wrong file.
	 * */
	FileReachEOFException(const String& func, e_int line, const String& file);
		
	String what() const override;
	
}; //FileReachEOFException


///*
// * \class IllegalUTF8CodeUnitException
// * 
// * \brief 无效 UTF-8 编码单元异常.
// * 
// * 遇到无效 UTF-8 编码单元时抛出.
// * */
//class IllegalUTF8CodeUnitException : public Exception {
//public:
//	/**
//	 * \brief 构造函数.
//	 * 
//	 * @param file 出错文件名.
//	 * */
//	IllegalUTF8CodeUnitException(const String& func, e_int line, const String& file);
//	String what() const override;
//}; // IllegalUTF8CodeUnitException


/**
 * \class FileHasNoParentException
 * 
 * \brief %File has no parent exception.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class FileHasNoParentException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param file the file path.
	 * */
	 FileHasNoParentException(const String& func, e_int line, const String& file);
	 
	 String what() const override;
}; // FileHasNoParentException

/**
 * \class EmptyContainerException
 * 
 * \brief Empty container exception.
 * 
 * Throw when reach a empty container.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class EmptyContainerException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param container the wrong container.
	 * */
	EmptyContainerException(const String& func, e_int line, const String& container);
	
	String what() const override;
	
}; // EmptyContainerException

/**
 * \class IllegalHashCodeException
 * 
 * \brief Illegal hash code exception.
 * 
 * Throws when HashSet or HashMap reach a negative hash code.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class IllegalHashCodeException : public Exception {
public:
	/**
 	 * \brief ctor.
 	 * 
 	 * @param hashCode illegal hash code.
	 * */
	IllegalHashCodeException(const String& func, e_int line, e_int hashCode);
	
	String what() const override;
}; // IllegalHashCodeException

/**
 * \class IteratorIsEndException
 * 
 * \brief Iterator is end exception.
 * 
 * Throw when Iterator::current() reach end.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class IteratorIsEndException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * */
	IteratorIsEndException(const String& func, e_int line);
	
	String what() const override;
	
}; // IteratorIsEndException


/**
 * \class HasNoSuchElementException
 * 
 * \brief Has no such element exception.
 * 
 * Throw when a container has no a element.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class HasNoSuchElementException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param container the wrong container.
	 * */
	HasNoSuchElementException(const String& func, e_int line, const String& container);
	
	String what() const override;
	
}; // HasNoSuchElementException


class ConcurrentModificationException : public Exception {
public:
	ConcurrentModificationException(const String& func, e_int line, const String& container);
	
	String what() const override;
}; // ConcurrentModificationException

/**
 * \class HasNoSuchAttrException
 * 
 * \brief Has no such attribute exception.
 * 
 * Throw when a LXML::Node has no a attribute.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class HasNoSuchAttrException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param node the wrong LXML::Node tag.
	 * @param attr the wrong attribut.
	 * */
	HasNoSuchAttrException(const String& func, e_int line, const String& node, const String& attr);
		
	String what() const override;
	
}; // HasNoSuchAttrException


/**
 * \class HasNoSuchNodeException
 * 
 * \brief Has no such node exception.
 * 
 * Throw when a LXML doc has no a node.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class HasNoSuchNodeException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param parent the wrong parent node tag.
	 * @param node the wrong node tag.
	 * */
	HasNoSuchNodeException(const String& func, e_int line, 
								const String& parent, const String& node);
		
	String what() const override;
	
}; // HasNoSuchNodeException


/**
 * \class UnsupportedOperationException
 * 
 * \brief Unsupported operation exception.
 * 
 * Throw when a function is an unsupported function but must be exist.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class UnsupportedOperationException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param operation the wrong function name.
	 * */
	UnsupportedOperationException(const String& func, e_int line, 
									const String& operation);
	
	String what() const override ;
	
}; //


/**
 * \class LXMLParseException
 * 
 * \brief LXML doc parse exception.
 * 
 * Throw when parsing a wrong format LXML doc.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class LXMLParseException  : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param doc the wrong doc name.
	 * */
	LXMLParseException(const String& func, e_int line, const String& doc);
		
	String what() const override;
	
}; //LXMLParseException


/**
 * \class ProcessCreateException
 * 
 * \brief Process create exception.
 * 
 * Throw when a process create failed.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class ProcessCreateException  : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * @param process the wring process name.
	 * */
	ProcessCreateException(const String& func, e_int line, const String& process);
	
	String what() const override;
	
}; // ProcessCreateException


/**
 * \class ArgsParseException
 * 
 * \brief Arguments parse exception.
 * 
 * Be threw when ArgsParser parsing, and reach a error.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class ArgsParseException : public Exception {
public:
	/**
	 * \brief ctor.
	 * */
	ArgsParseException(const String& func, e_int line);
		
	String what() const override;
	
}; //ArgsParseException

/**
 * \class BadWeakPointerException
 * 
 * \brief Bad weak pointer exception.
 * 
 * Be threw when program reach a dangling weak pointer.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class BadWeakPointerException : public Exception {
public:
	/**
	 * \brief ctor.
	 * */
	BadWeakPointerException(const String& func, e_int line);
	String what() const override;
}; //


/**
 * \class RegexParseException
 * 
 * \brief Regex parse exception.
 * 
 * Throw when parsing a illegal regex.
 * 
 * @author Tes Alone
 * @since  1.0
 * */
class RegexParseException : public Exception {
public:
	/**
	 * \brief ctor.
	 * 
	 * @param func function thrown the exception.
	 * @param line code line where thrown the exception.
	 * */
	RegexParseException(const String& func, e_int line);
	
	String what() const override;

}; // RegexParseException

} // Easy
#endif // LEC_EXCEPTION_H_￿