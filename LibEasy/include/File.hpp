/*
 * File Name:	File.hpp
 * Date/Time:
 * 	Start: 	2019-11-29 16:58 week:5
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note:
*/

#ifndef _EASY_FILE_H_
#define _EASY_FILE_H_

#include "BaseTypes.hpp"
#include "String.hpp"
#include "System.hpp"
#include "Date.hpp"
#include "Array.hpp"
#include "Vector.hpp"

namespace Easy {

/**
 * \class File
 * 
 * \brief Files and directories maintainer.
 * 
 * This class deal with file system, supports files/directories creating,
 * deleting, removing, renaming  and so on.
 * 
 * @author Tes Alone
 * @since 1.0
*/

class File {
private:
	String m_fullPath;
public:
	/**
	 * \brief ctor.
	 * 
	 * A zero-argument ctor, do nothing.
	*/
	File() {}

	/**
	 * \brief ctor.
	 * 
	 * @param filePath .
	 * @throw if filePath is empty, throw IllegalArgumentException.
	*/
	explicit File(const String& filePath);
	
	/**
	 * \brief Check equality.
	 * 
	 * @param other .
	 * @return if receiver's full path equals other's return true, otherwise return false.
	*/
	e_bool equals(const File& other) const ;

	/**
	 * \brief Check order.
	 * 
	 * If one of receiver and other is a directory, and one is file, the file is bigger.
	 * If both are files or directories, compare the full path.
	 * 
	 * @param other .
	 * @return if receiver is bigger, return 1; both equals return 0; otherwise return -1.
	 * @sa String::compare()
	*/
	e_int compare(const File& other) const ;

	/**
	 * \brief Get hash code.
	 * 
	 * Get receiver`s hash code.
	 * 
	 * @return receiver`s hash code.
	 * */
	inline e_int hashCode() const {return m_fullPath.hashCode();}

	/**
	 * \brief To string represent.
	 * 
	 * @return receiver's full path.
	*/
	inline String toString() const {return System::move(String{m_fullPath});}

	/**
	 * \brief Check whether exist.
	 * 
	 * @return result.
	*/
	e_bool exists() const;

	/**
	 * \brief Check whether hidder.
	 * 
	 * @return result.
	*/
	e_bool isHidden() const;

	/**
	 * \brief Check whether read only.
	 * 
	 * @return result.
	*/
	e_bool isReadOnly() const;

	/**
	 * \brief Check whether is a directory.
	 * 
	 * @return result.
	*/
	e_bool isDirectory() const;

	/**
	 * \brief Get full path.
	 * 
	 * @return receiver's full path.
	*/
	String getFullPath() const;
	
	/**
	 * \brief Get name.
	 * 
	 * @return receiver's name.
	*/
	String getName() const;
	
	/**
		\brief Get parent.
		
		@return receiver's parent directory.
	*/
	File getParent() const;
	
	/**
	 * \brief Get file size.
	 * 
	 * If file not exist, return 0.
	 * 
	 * @return receiver's size, by byte.
	*/
	e_long getSize() const;

	/** 
	 * \brief Get creating date.
	 * 
	 * If file not exist, return Date::Date().
	 * 
	 *  @return receiver's creating date.
	 */
	Date getCreateDate() const;
	
	/**
	 * \brief Get last modifying date.
	 * 
	 * If file not exist, return Date::Date().
	 * 
	 * @return receiver's last modifying date. 
	*/
	Date getLastModifyDate() const;
	
	/**
	 * \brief Get last accessing date.
	 * 
	 * If file not exist, return Date::Date().
	 * 
	 * @return receiver's last accessing date. 
	*/
	Date getLastAccessDate() const;
	
	/**
	 * \brief Create a new file.
	 * 
	 * @return if creating succeed return true, not return false.
	*/
	e_bool createNewFile() const;
	
	/**
	 * \brief Create a new directory.
	 * 
	 * @return if creating succeed return true, not return false.
	*/
	e_bool mkdir() const;
	
	/**
	 * \brief Create a new directory.
	 * 
	 * @return if creating succeed return true, not return false.
	*/
	e_bool mkdirs() const;

	/**
	 * \brief Delete a file.
	 * 
	 * @return if deleting succeed return true, not return false.
	*/
	e_bool deleteFile() const;

	/**
	 * \brief Delete a empty directory.
	 * 
	 * @return if deleting succeed return true, not return false.
	*/
	e_bool remove() const;
	
	/**
	 * \brief %List all child files.
	 * 
	 * 
	 * @return receiver's all children.
	*/
	Vector<File> listAll() const;

	/**
	 * \brief Rename a file.
	 * 
	 * Method will change receiver's status.
	 *
	 * @param newName new name.
	 * @return if renaming succeed return true, not return false.
	 * @throw if newName is empty, throw IllegalArgumentException.
	*/
	e_bool rename(const String& newName);

	/**
	 * \brief Touch a file.
	 * 
	 * If receiver does not exist, create a file; otherwise update receiver's
	 * last modifying date to method called date/time. 
	 * 
	 * @return if touching succeed return true, otherwise return false.
	*/
	e_bool touch() const;
	
	/**
	 * \brief Check whether can read.
	 * 
	 * @return result.
	*/
	e_bool readable() const;

	/**
	 * \brief Check whether can write.
	 * 
	 * @return result.
	*/
	e_bool writable() const;

	/**
	 * \brief Check whether can execute.
	 * 
	 * @return result.
	*/
	e_bool executable() const;

	/**
	 * \brief Get file owner.
	 * 
	 * @return receiver's owner.
	*/
	String getOwner() const;
private:
	Date _createDate(SYSTEMTIME* pst) const; 
}; // File
} // Easy

#endif // _EASY_FILE_H_

