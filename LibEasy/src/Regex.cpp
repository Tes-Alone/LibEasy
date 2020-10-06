/*
 * 文件名: Regex.cpp 
 *
 * 日期/时间:
 *		开始: 2020年05月20日 16:45:25 周三
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#include "Regex/Regex.hpp"
#include "Console.hpp"

namespace Easy {

namespace Regex {

Matcher::Matcher(const SharedPointer<Pattern>& pattern, const String& text) {
	_pattern = pattern;
	_text    = text;
	_groups  = IntArray{pattern->_groupCount * 2};
	_last    = 0;
	_first   = -1;
}

e_bool Matcher::matches() {
	e_bool result = _pattern->_root->match(*this, _text, 0);
	if (result) {
		Console::out("last:");
		Console::outln(_last);
		return _last == _text.length();
	}
	return false;
}

} // Regex
} // Easy
