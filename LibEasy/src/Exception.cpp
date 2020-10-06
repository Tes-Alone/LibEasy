#include "Exception.hpp"
#include "String.hpp"
#include "Integer.hpp"

namespace Easy {
	Exception::Exception(const String& msg, const String& func, e_int line) {
		m_msg  = new String{msg};
		m_func = new String{func};
		m_line = line;
	}
	
	Exception::~Exception() {
		delete m_msg;
		delete m_func;
	}
	
	String Exception::what() const {
		return String{L"Exception"};
	}
	
	String Exception::getMessage() const {
		return System::move(String{*m_msg});
	}
	
	String Exception::getFunction() const {
		return System::move(String{*m_func});
	}
	 
	NullPointerException::NullPointerException(const String& func, e_int line) 
		: Exception{String{L"Reach a nullptr."}, func, line}  {}
	
	String NullPointerException::what() const {
		return String{L"NullPointerException"};
	}
	
	IllegalArgumentException::IllegalArgumentException(const String& func, e_int line)
		: Exception{String{L"Argument illegal."}, func, line}  {}
	
	String IllegalArgumentException::what() const {
		return String{L"IllegalArgumentException"};
	}
	
	IndexOutOfBoundsException::IndexOutOfBoundsException(
				const String& func, e_int line,
					const String& container, e_int len, e_int index) 
		: Exception{container + String{L"'s length is "} + Integer{len}
			 + String{L", index is "} + Integer{index}, func, line} {}
	
	String IndexOutOfBoundsException::what() const {
		return String{L"IndexOutOfBoundsException"};
	}
		
	
	String NotInFunctionDomainException::what() const {
		return String{L"NotInFunctionDomainException"};
	}
	
	NumberFormatException::NumberFormatException(const String& func, e_int line, const String& num)
		: Exception(String{L"Number '"} + num + String{L"' format illegal."}, func, line) {}
	
	String NumberFormatException::what() const {
		return String{L"NumberFormatException"};
	}
	
	NumberOverflowException::NumberOverflowException(const String& func, e_int line, const String& num)
		: Exception(String{L"Number '"} + num + String(L"' overflow."), func, line) {}
	
	String NumberOverflowException::what() const {
		return String{L"NumberOverflowException"};
	}
	
	FloatExpException::FloatExpException(const String& func, e_int line, const String& num)
		: Exception(String{L"Float number '"} + num + String{L"' exp overflow."}, func, line) {}
	
	String FloatExpException::what() const {
		return String{L"FloatExpException"};
	}
	
	FormatedStringFormatException::FormatedStringFormatException(const String& func, e_int line, const String& str)
		: Exception(String{L"Format string '"} + str + String{L"' format illegal."}, func, line) {}
	
	String FormatedStringFormatException::what() const {
		return String{L"FormatedStringFormatException"};
	}
	
	FileOpenFailedException::FileOpenFailedException(const String& func, e_int line, const String& file)
		: Exception(String{L"File '"} + file + String{L"' open failed."}, func, line) {}
	
	String FileOpenFailedException::what() const {
		return String{L"FileOpenFailedException"};
	}
	
	FileCloseFailedException::FileCloseFailedException(const String& func, e_int line, const String& file)
		: Exception(String{L"File '"} + file + String{L"' close failed."}, func, line) {}
	
	String FileCloseFailedException::what() const {
		return String{L"FileCloseFailedException"};
	}
	
	FileReachEOFException::FileReachEOFException(const String& func, e_int line, const String& file)
		: Exception(String{L"File '"} + file + String{L"' reach EOF."}, func, line) {}
	
	String FileReachEOFException::what() const {
		return String{L"FileReachEOFException"};
	}
	
//	IllegalUTF8CodeUnitException::IllegalUTF8CodeUnitException(const String& func, e_int line, const String& file)
//		: Exception(String{L"File '"} + file + String{L"' reach a ill-formed code unit."}, func, line) {}
//		
//	String IllegalUTF8CodeUnitException::what() const {
//		return String{L"IllegalUTF8CodeUnitException"};
//	}
	
	IOException::IOException(const String& func, e_int line, const String& file)
		: Exception(String{L"File '"} + file + String{L"' IO failed."}, func, line) {}
	
	String IOException::what() const {
		return String{L"IOException"};
	}
	
	FileHasNoParentException::FileHasNoParentException(const String& func, e_int line, const String& file)
		: Exception(String{L"File '"} + file + String{L"' has no parent."}, func, line) {}
		
	String FileHasNoParentException::what() const {
		return String{L"FileHasNoParentException"};
	}
	
	EmptyContainerException::EmptyContainerException(const String& func, e_int line, const String& container)
		: Exception(container + String{L" is empty."}, func, line) {}
	
	String EmptyContainerException::what() const {
		return String{L"EmptyContainerException"};
	}
	
	IllegalHashCodeException::IllegalHashCodeException(const String& func, e_int line, e_int hashCode)
		: Exception(String{L"Negative hash code -> "} + Integer{hashCode}, func, line) {}
	
	String IllegalHashCodeException::what() const {
		return String{L"IllegalHashCodeException"};
	}
	
	IteratorIsEndException::IteratorIsEndException(const String& func, e_int line)
		: Exception(String{L"Iterator is end."}, func, line) {}
	
	String IteratorIsEndException::what() const {
		return String{L"IteratorIsEndException"};
	}
	
	HasNoSuchElementException::HasNoSuchElementException(const String& func, e_int line, const String& container)
		: Exception(container + String{L" has no such element."}, func, line) {}
	
	String HasNoSuchElementException::what() const {
		return String{L"HasNoSuchElementException"};
	}
	
	ConcurrentModificationException::ConcurrentModificationException(const String& func, e_int line, const String& container)
		: Exception(container + L" modified after iterator() called.", func, line) {}
	
	String ConcurrentModificationException::what() const {
		return L"ConcurrentModificationException";
	}
	
	HasNoSuchAttrException::HasNoSuchAttrException(const String& func, e_int line, const String& node, const String& attr)
		: Exception(node + String{L" has no attribute -> "} + attr, func, line) {}
	
	String HasNoSuchAttrException::what() const {
		return String{L"HasNoSuchAttrException"};
	}
	
	HasNoSuchNodeException::HasNoSuchNodeException(const String& func, e_int line, const String& parent, const String& node)
		: Exception(parent + String{L" has no note -> "} + node, func, line) {} // FIXME
	
	String HasNoSuchNodeException::what() const {
		return String{L"HasNoSuchNodeException"};
	}
	
	UnsupportedOperationException::UnsupportedOperationException(const String& func, e_int line, const String& operation)
		: Exception(operation + String{L" not supported."}, func, line) {}
	
	String UnsupportedOperationException::what() const {
		return String{L"UnsupportedOperationException"};
	}
	
	LXMLParseException::LXMLParseException(const String& func, e_int line, const String& doc)
		: Exception(doc + String{L" parse failed."}, func, line) {}
	
	String LXMLParseException::what() const {
		return String{L"LXMLParseException"};
	}
	
	ProcessCreateException::ProcessCreateException(const String& func, e_int line, const String& process)
		: Exception(process + String{L" create failed."}, func, line) {}
	
	String ProcessCreateException::what() const {
		return String{L"ProcessCreateException"};
	}
	
	ArgsParseException::ArgsParseException(const String& func, e_int line)
		: Exception(String{L"Command arguments parse failed."}, func, line) {}
	
	String ArgsParseException::what() const {
		return String{L"ArgsParseException"};
	}
	
	BadWeakPointerException::BadWeakPointerException(const String& func, e_int line) 
		: Exception(String{L"Reach a bad weak pointer."}, func, line) {}
		
	String BadWeakPointerException::what() const {
		return String{L"BadWeakPointerException"};
	}

	RegexParseException::RegexParseException(const String& func, e_int line)
		: Exception(String{L"Regex parse failed."}, func, line) {}
	
	String RegexParseException::what() const {
		return System::move(String{L"RegexParseException"});
	}

} // Easy