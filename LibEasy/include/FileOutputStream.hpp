/*
 * File Name:	FileOutputStream.hpp
 * Date/Time:
 * 	Start: 	2019-11-27 18:11 week:3
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_FILEOUTPUTSTREAM_H_
#define _EASY_FILEOUTPUTSTREAM_H_

#include <cstdio>

#include "BaseTypes.hpp"
#include "BasicArray.hpp"
#include "File.hpp"

#include "SharedPointer.hpp"

namespace Easy {

class String;

/**
 * \class FileOutputStream
 * 
 * \brief Binary files output stream.
 * 
 * This class can be wrapped by SharedPointer.
 * 
 * @author Tes Alone
 * @since 1.0
*/
class FileOutputStream: public EnableSharedFromThis<FileOutputStream>  {
private:
	FILE* m_file;
	e_bool m_closed;
	String m_filePath;
public:
	/**
	 * \brief ctor.
	 * 
	 * Create a file output stream from a file.
	 * 
	 * @param file from which receiver create.
	 * @throw if file's full path is empty, throws IllegalArgumentException.
	 * @throw if output create failed, throws FileOpenFailedException.
	 * */
	FileOutputStream(const File& file);
	
	/**
	 * \brief ctor.
	 * 
	 * Create a file output stream from a file indicated by filePath.
	 * 
	 * @param filePath from which receiver create.
	 * @throw if filePath is empty, throws IllegalArgumentException.
	 * @throw if output create failed, throws FileOpenFailedException.
	 * */
	FileOutputStream(const String& filePath);
	
	/**
	 * \brief dtor.
	 * 
	 * Method will close receiver.
	 * 
	 * @throw if close failed, throws FileCloseFailedException.
	*/
	inline ~FileOutputStream() {close();}

	/**
	 * \brief Output a byte.
	 * 
	 * @param c the byte to output.
	 * @throw if write failed, throws IOException.
	*/
	e_void write(e_int c);

	/**
	 * \brief Output a byte block.
	 * 
	 * @param buf content to output.
	 * @param length write length, default -1, indicates buf.length().
	 * @throw if length<0 or length>buf.length(), throws IllegalArgumentException.
	 * @throw if write failed, throws IOException.
	*/
	e_void write(const UByteArray& buf, e_int length=-1);
	
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
	FileOutputStream(FileOutputStream& copy)  = delete;
	FileOutputStream(FileOutputStream&& move)  = delete;
	FileOutputStream& operator=(FileOutputStream& copy)  = delete;
	FileOutputStream& operator=(FileOutputStream&& move)  = delete;
}; // FileOutputStream
} // Easy

#endif // _EASY_FILEOUTPUTSTREAM_H_

