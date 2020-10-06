/*
 * File Name:	FileWriter.hpp
 * Date/Time:
 * 	Start: 	2019-11-27 18:31 week:3
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_FILEWRITER_H_
#define _EASY_FILEWRITER_H_

#include <cstdio>

#include "BaseTypes.hpp"
#include "BasicArray.hpp"
#include "File.hpp"

#include "SharedPointer.hpp"

namespace Easy {

class String;

/**
 * \class FileWriter
 * 
 * \brief Text files output stream.
 * 
 * Only supports UTF-8 encoding.<br/>
 * This class can be wrapped by SharedPointer.
 * 
 * @author Tes Alone
 * @since 1.0
*/
class FileWriter : public EnableSharedFromThis<FileWriter>  {
private:
	FILE* m_file;
	e_bool m_closed;
	String m_filePath;
public:
	
	/**
	 * \brief ctor.
	 * 
	 * Create a file writer from a file.
	 * 
	 * @param file from which receiver create.
	 * @throw if file's full path is empty, throws IllegalArgumentException.
	 * @throw if writer create failed, throws FileOpenFailedException.
	 * */
	FileWriter(const File& file);
	
	/**
	 * \brief ctor.
	 * 
	 * Create a file writer from a file indicates by filePath.
	 * 
	 * @param file from which receiver create.
	 * @throw if filePath is empty, throws IllegalArgumentException.
	 * @throw if writer create failed, throws FileOpenFailedException.
	 * */
	FileWriter(const String& filePath);
	
	/**
	 * \brief dtor.
	 * 
	 * Method will close receiver.
	 * @throw if close failed, throws FileCloseFailedException.
	*/
	inline ~FileWriter() {close();}

	/**
	 * \brief Write a char.
	 * 
	 * Write a char to receiver, and advance writing pos by one.
	 * 
	 * @param c char to write.
	 * @throw if read failed, throws IOException.
	*/
	e_void write(e_int c);
	
	/**
	 * \brief Write one or more chars.
	 * 
	 * Write a char block to receiver, and advance writing pos by writing length.
	 * 
	 * @param buf content to write.
	 * @param length writing length, default -1, indicates buf.length().
	 * @throw if length<0 or length>buf.length(), throws IllegalArgumentException.
	 * @throw if write failed, throws IOException.
	*/
	e_void write(const CharArray& buf, e_int length=-1);
	
	/**
	 * \brief Write string.
	 * 
	 * Write a String to receiver, and advance writing pos by writing length.
	 * 
	 * @param str string to write.
	 * @param length writing length, default -1, indicates str.length().
	 * @throw if length<0 or length>str.length(), throws IllegalArgumentException.
	 * @throw if write failed, throws IOException.
	*/
	e_void write(const String& str, e_int length=-1);
	
	/**
	 * \brief Flush cache.
	 * 
	 * Flush output cache to disk.
	 * 
	 * @throw if flush failed, throws IOException.
	*/
	e_void flush();

	/**
	 * \brief Close stream.
	 * 
	 * @throw if close failed, throws FileCloseFailedException.
	*/
	e_void close();

	String toString() const override;
private:
	FileWriter(FileWriter& copy)  = delete;
	FileWriter(FileWriter&& move)  = delete;
	FileWriter& operator=(FileWriter& copy)  = delete;	
	FileWriter& operator=(FileWriter&& move)  = delete;
}; // FileWriter
} // Easy

#endif // _EASY_FILEWRITER_H_

