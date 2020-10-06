/*
 * File Name:	IOUtility.hpp
 * Date/Time:
 * 	Start: 	2020-04-28 16:09:26 2
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_IOUTILITY_H_
#define _EASY_IOUTILITY_H_

#include "String.hpp"
#include "BasicArray.hpp"
#include "File.hpp"

namespace Easy {

/**
 * \class IOUtility.
 * 
 * \brief %File IO utility.
 * 
 * This pure tool class provides some high level %file read/write functions.
 * 
 * @author Tes Alone
 * @since 1.0
 * */

class IOUtility {
public:
	
	/**
	 * \brief Read text file.
	 * 
	 * %File encoding must be UTF-8.
	 * 
	 * @param file file path.
	 * @return content of file.
	 * @throw if file is empty, throws IllegalArgumentException.
	 * @throw if file open failed, throws FileOpenFailedException.
	 * @throw if reach a IO error, throws IOException.
	 * @throw if file close failed, throws FileCloseFailedException.
	 * */
	static String read(const String& file);
	
	/**
	 * \brief Read text file.
	 * 
	 * %File encoding must be UTF-8.
	 * 
	 * @param file .
	 * @return content of file.
	 * @throw if file's path is empty, throws IllegalArgumentException.
	 * @throw if file open failed, throws FileOpenFailedException.
	 * @throw if reach a IO error, throws IOException.
	 * @throw if file close failed, throws FileCloseFailedException.
	 * */
	static String read(const File& file);
	
	/**
	 * \brief Read binary file.
	 * 
	 * @param file file path.
	 * @return content of file.
	 * @throw if file is empty, throws IllegalArgumentException.
	 * @throw if file open failed, throws FileOpenFailedException.
	 * @throw if reach a IO error, throws IOException.
	 * @throw if file close failed, throws FileCloseFailedException.
	 * */
	static UByteArray readBin(const String& file);
	
	/**
	 * \brief Read binary file.
	 * 
	 * @param file .
	 * @return content of file.
	 * @throw if file's path is empty, throws IllegalArgumentException.
	 * @throw if file open failed, throws FileOpenFailedException.
	 * @throw if reach a IO error, throws IOException.
	 * @throw if file close failed, throws FileCloseFailedException.
	 * */
	static UByteArray readBin(const File& file);
	
	/**
	 * \brief Save string to text file.
	 * 
	 * Saved file's encoding is UTF-8 with BOM.
	 * <br/>
	 * If file already exists, the content of the file will be erased.
	 * 
	 * @param file save path.
	 * @param content .
	 * @throw if file is empty, throws IllegalArgumentException.
	 * @throw if file open failed, throws FileOpenFailedException.
	 * @throw if reach a IO error, throws IOException.
	 * @throw if file close failed, throws FileCloseFailedException.
	 * */
	static e_void write(const String& file, const String& content);
	
	/**
	 * \brief Save string to text file.
	 * 
	 * Saved file's encoding is UTF-8 with BOM.
	 * <br/>
	 * If file already exists, the content of the file will be erased.
	 * 
	 * @param file .
	 * @param content .
	 * @throw if file's path is empty, throws IllegalArgumentException.
	 * @throw if file open failed, throws FileOpenFailedException.
	 * @throw if reach a IO error, throws IOException.
	 * @throw if file close failed, throws FileCloseFailedException.
	 * */
	static e_void write(const File& file, const String& content);
	
	/**
	 * \brief Save binary data to file.
	 * 
	 * If file already exists, the content of the file will be erased.
	 * 
	 * @param file save path.
	 * @param content .
	 * @throw if file is empty, throws IllegalArgumentException.
	 * @throw if file open failed, throws FileOpenFailedException.
	 * @throw if reach a IO error, throws IOException.
	 * @throw if file close failed, throws FileCloseFailedException.
	 * */
	static e_void writeBin(const String& file, const UByteArray& content);
	
	/**
	 * \brief Save binary data to file.
	 * 
	 * If file already exists, the content of the file will be erased.
	 * 
	 * @param file .
	 * @param content .
	 * @throw if file's path is empty, throws IllegalArgumentException.
	 * @throw if file open failed, throws FileOpenFailedException.
	 * @throw if reach a IO error, throws IOException.
	 * @throw if file close failed, throws FileCloseFailedException.
	 * */
	static e_void writeBin(const File& file, const UByteArray& content);

private:
	IOUtility() =delete;
	~IOUtility() =delete;
	IOUtility(IOUtility& copy)  = delete;
	IOUtility(IOUtility&& move)  = delete;
	IOUtility& operator=(IOUtility& copy)  = delete;
	IOUtility& operator=(IOUtility&& move)  = delete;
}; // IOUtility

} // Easy

#endif // _EASY_IOUTILITY_H_

