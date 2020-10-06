/*
 * File Name:	FileInputStream.hpp
 * Date/Time:
 * 	Start: 	2019-11-27 17:56 week:3
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_FILEINPUTSTREAM_H_
#define _EASY_FILEINPUTSTREAM_H_

#include <cstdio>

#include "BaseTypes.hpp"
#include "BasicArray.hpp"
#include "File.hpp"

#include "SharedPointer.hpp"

namespace Easy {

class String;

/**
 * \class FileInputStream
 * 
 * \brief Binary files input stream.
 * 
 * This class can be wrapped by SharedPointer.
 * 
 * @author Tes Alone
 * @since 1.0
*/

class FileInputStream : public EnableSharedFromThis<FileInputStream> {
private:
	FILE*  m_file;
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
	explicit FileInputStream(const File& file);
	
	/**
	 * \brief ctor.
	 * 
	 * Open file indicates by filePath as a input stream.
	 * 
	 * @param filePath .
	 * @throw if filePath is empty, throws IllegalArgumentException.
	 * @throw if open failed, throws FileOpenFailedException.
	*/
	explicit FileInputStream(const String& filePath);

	/**
	 * \brief dtor.
	 * 
	 * Method will close receiver.
	 * @throw if close failed, throws FileCloseFailedException.
	*/
	inline ~FileInputStream() {close();};

	/**
		\brief Set next reading pos.
		
		If pos is bigger than file size, the next reading will reach eof.
		
		@param pos .
		@return if seeking succeed return true, otherwise return false.
		@throw if pos < 0, throws IllegalArgumentException.
	*/
	e_bool seek(e_long pos);
	
	/**
	 * \brief Read a byte.
	 * 
	 * Read current byte in receiver, and advance reading pos by one.
	 * If reading reaches eof, return -1.
	 * 
	 * @return the read byte or -1.
	 * @throw if read failed, throws IOException.
	*/
	e_int read();

	/**
	 * \brief Read one or more bytes.
	 * 
	 * Read a byte block in receiver, and advance reading pos by reading length.
	 * If reading reaches eof, return reading length, maybe 0.
	 * 
	 * @param buf reading buffer.
	 * @return reading length.
	 * @throw if read failed, throws IOException.
	*/
	e_int read(UByteArray& buf);

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
	inline constexpr e_int eof() const {return EOF;}
	
	String toString() const override;
private:
	FileInputStream(FileInputStream& copy)  = delete;
	FileInputStream(FileInputStream&& move)  = delete;
	FileInputStream& operator=(FileInputStream& copy)  = delete;
	FileInputStream& operator=(FileInputStream&& move)  = delete;
}; // FileInputStream
} // Easy

#endif // _EASY_FILEINPUTSTREAM_H_

