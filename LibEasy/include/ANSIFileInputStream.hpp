/*
 * File Name:	ANSIFileInputStream.hpp
 * Date/Time:
 * 	Start: 	2019年12月23日 17:56:27 周一
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_ANSIFILEINPUTSTREAM_H_
#define _EASY_ANSIFILEINPUTSTREAM_H_

#include <cstdio>

#include "BaseTypes.hpp"
#include "String.hpp"
#include "BasicArray.hpp"
#include "BasicArray.tcc"
#include "FileInputStream.hpp"

#include "SharedPointer.hpp"

namespace Easy {

/**
 * \class ANSIFileInputStream
 * 
 * \brief ANSI encoding file input stream.
 * 
 * This class converts a ANSI encoding file to Unicode represent.
 * <br/>
 * <b>There is no ANSIFileOutpuutStream.</b>
 * <br/>
 * 
 * @author Tes Alone
 * @since 1.0
*/

class ANSIFileInputStream : public EnableSharedFromThis<ANSIFileInputStream> {
private:
	FileInputStream* m_in;
public:

	/**
	 * \brief ctor.
	 * 
	 * Create a ANSI encoding input stream from a file.
	 * @param file from which receiver create.
	 * @throw if stream create failed, throw FileOpenFailedException.
	 * */
	inline ANSIFileInputStream(const File& file) {
		m_in = new FileInputStream{file};
	}
	
	/**
	 * \brief ctor.
	 * 
	 * Create a ANSI encoding input stream from file path.
	 * 
	 * @param input the file path.
	 * @throw if input opend failed, throw FileOpenFailedException.
	*/
	inline explicit ANSIFileInputStream(const String& input) {
		m_in = new FileInputStream{input};
	}
	
	/**
	 * \brief dtor.
	 * 
	 * Close receiver, if receiver has been not closed.
	 * */
	inline ~ANSIFileInputStream() {close(); delete m_in;}
	
	/**
	 * \brief read a char.
	 * 
	 * Read a full Unicode character from ANSI byets, and move stream to next pos.
	 * <br/>
	 * If read after receiver closed, value returned make no sense.
	 * 
	 * @return readed char.
	 * @throw if read failed, throw IOException.
	*/
	e_int read();
	
	/**
	 * \brief read a block chars.
	 * 
	 * Read a block chars one time.
	 * <br/>
	 * If read after receiver closed, value returned make no sense.
	 * 
	 * @param buf read buffer.
	 * @return readed chars count.
	 * @throw if read faild, throw IOException.
	*/
	e_int read(CharArray& buf);
	
	/**
	 * \brief close stream.
	 * 
	 * Close stream.
	 * 
	 * @throw if stream close failed, throw FileCloseFailedException.
	*/
	e_void close();
	
	/**
	 * \brief get EOF sign.
	 * 
	 * <b>Note: this method just return value of EOF, it does not indicate receiver reach a EOF.</b>
	 * 
	 * @return the EOF sign.
	*/
	inline constexpr e_int eof() const {return m_in->eof();}

	String toString() const override;
private:
	ANSIFileInputStream(ANSIFileInputStream& copy) =delete;
	ANSIFileInputStream& operator=(ANSIFileInputStream& copy) =delete;
	ANSIFileInputStream(ANSIFileInputStream&& move) =delete;
	ANSIFileInputStream& operator=(ANSIFileInputStream&& move) =delete;
}; // ANSIFileInputStream
} // Easy

#endif // _EASY_ANSIFILEINPUTSTREAM_H_

