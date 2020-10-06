/*
 * File Name:	FileReader.hpp
 * Date/Time:
 * 	Start: 	2019-11-27 18:21 week:3
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_FILEREADER_H_
#define _EASY_FILEREADER_H_

#include <cstdio>

#include "BaseTypes.hpp"
#include "BasicArray.hpp"
#include "File.hpp"

#include "SharedPointer.hpp"

namespace Easy {

class String;

/**
 * \class FileReader
 * 
 * \brief Text files input stream.
 * 
 * Only supports UTF-8 encoding. <br/>
 * This class can be wrapped by SharedPointer.
 * 
 * @author Tes Alone
 * @since 1.0
*/
class FileReader : public EnableSharedFromThis<FileReader>  {
private:
	FILE* m_file;
	e_bool m_closed;
	String m_filePath;
public:
	
	/**
	 * \brief ctor.
	 * 
	 * Open file as a input stream.
	 * 
	 * @param file .
	 * @throw if file's full path is empty, throws IllegalArgumentException.
	 * @throw if open failed, throws FileOpenFailedException.
	*/
	FileReader(const File& file);

	/**
	 * \brief ctor.
	 * 
	 * Open file indicates by filePath as a input stream.
	 * 
	 * @param filePath .
	 * @throw if filePath is empty, throws IllegalArgumentException.
	 * @throw if open failed, throws FileOpenFailedException.
	*/
	FileReader(const String& filePath);

	/**
	 * \brief dtor.
	 * 
	 * Method will close receiver.
	 * @throw if close failed, throws FileCloseFailedException.
	*/
	inline ~FileReader() {close();}

	/**
	 * \brief Read a char.
	 * 
	 * Read current char in receiver, and advance reading pos by one.
	 * If reading reaches eof, return -1.
	 * 
	 * @return the read char or -1.
	 * @throw if read failed, throws IOException.
	*/
	e_int read();

	/**
	 * \brief Read one or more chars.
	 * 
	 * Read a char block in receiver, and advance reading pos by reading length.
	 * If reading reaches eof, return reading length, maybe 0.
	 * 
	 * @param buf reading buffer.
	 * @return reading length.
	 * @throw if read failed, throws IOException.
	*/
	e_int read(CharArray& buf);
	
	/**
	 * \brief Read one line.
	 * 
	 * @return next line.
	 * @throw if read failed, throws IOException.
	 * @throw if reach eof, throws  FileReachEOFException.
	*/
	String readLine();

	/**
	 * \brief Close stream.
	 * 
	 * @throw if close failed, throws FileCloseFailedException.
	*/
	e_void close();

	/**
	 * \brief Get EOF.
	 * 
	 * <b>Note: The method just get eof, not mean the reading reaches eof.</b>
	 * 
	 * @return constant EOF.
	*/
	constexpr inline e_int eof() const {return WEOF;}

	String toString() const override;
private:
	FileReader(FileReader& copy)  = delete;
	FileReader(FileReader&& move)  = delete;
	FileReader& operator=(FileReader& copy)  = delete;
	FileReader& operator=(FileReader&& move)  = delete;
}; // FileReader
} // Easy

#endif // _EASY_FILEREADER_H_

