/*
 * 文件名: IOUtility.cpp 
 *
 * 日期/时间:
 *		开始: 2020年04月28日 16:13:49 周二
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#include "IOUtility.hpp"
#include "FileReader.hpp"
#include "FileWriter.hpp"
#include "FileInputStream.hpp"
#include "FileOutputStream.hpp"

namespace Easy {

String IOUtility::read(const String& file) {
	FileReader in{file};
	StringBuffer sb;
	CharArray buf{4096};
	e_int count = in.read(buf);

	while (count == buf.length()) {
		sb.append(buf, count);
		count = in.read(buf);
	}
	
	sb.append(buf, count);
	in.close();
	return System::move(sb.toString());
}

String IOUtility::read(const File& file) {
	return System::move(read(file.getFullPath()));
}


UByteArray IOUtility::readBin(const File& file) {
	e_int fileLen = (e_int)file.getSize();
	UByteArray result{fileLen};
	FileInputStream in{file.getFullPath()};
	in.read(result);
	in.close();
	return System::move(result);
}

UByteArray IOUtility::readBin(const String& file) {
	return System::move(readBin(File{file}));
}

e_void IOUtility::write(const String& file, const String& content) {
	FileWriter out{file};
	out.write(content);
	out.close();
}

e_void IOUtility::write(const File& file, const String& content) {
	FileWriter out{file.getFullPath()};
	out.write(content);
	out.close();
}

e_void IOUtility::writeBin(const String& file, const UByteArray& content) {
	FileOutputStream out{file};
	out.write(content);
	out.close();
}

e_void IOUtility::writeBin(const File& file, const UByteArray& content) {
	FileOutputStream out{file.getFullPath()};
	out.write(content);
	out.close();
}
}
