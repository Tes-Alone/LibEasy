/*
 * File Name:	ArgsParser.hpp
 * Date/Time:
 * 	Start: 	2020-05-01 19:35:32 5
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note: FIXME
*/

#ifndef _EASY_CMDPARSER_H_
#define _EASY_CMDPARSER_H_


#include "Array.hpp"
#include "String.hpp"
#include "BaseTypes.hpp"
#include "Vector.hpp"
#include "HashMap.hpp"
#include "Exception.hpp"

#include "SharedPointer.hpp"

namespace Easy {

/** 
 * \class ArgsParser
 * 
 * \brief A tool for handling command line arguments.
 * 
 * ArgsParser distinguishes three types argument: <b>flag</b>, <b>option</b> and <b>free option values</b>.
 * <br/>
 * <h4>Flags</h4>
 * <P>
 * Flags are arguments that starts with "--" but not equals "--".
 * Flags can indicate turning something on or off.
 * If a flag appears more than one time, they do not do more thing.
 * </P>
 *
 * <h4>Options</h4>
 * <p>
 * Options are arguments that starts with "-" buf not equals "-", they must followed
 * by one or more option values.
 * Options can supply program some info or data.
 * If a option appears more than one time, the option values will be merged as one 
 * option valus list.
 * </p>
 *
 * <h4>Free option values</h4>
 * <p>
 * Free option values are argument that neither a <b>Flag</b> nor a <b>Option</b>.
 * If a argument follows a option, it is a option value, otherwise it is a free option value.
 * Free option values can also supply program some info or data.
 * A free option value is independent. They do not influence each other.
 * </p>
 * 
 * <p>
 * The order of arguments is not cared by ArgsParser, except OptionValues of a Option, they need
 * follow the Option.
 * <br/>
 * "--" and "-" are treated as option values, if they are not following a option, they are free,
 * as other option values.
 * </p>
 * 
 * <h3>Note:</h3>
 * <p>There are three types error in arguments.
 * 	<ul>
 * 		<li>Undefined Flags.</li>
 * 		<li>Undefined Options.</li>
 * 		<li>A option has no values.</li>
 * 	</ul>
 * They are all raised in ArgsParser::parse(), if happened one.
 * </p>
 *  
 * <h3>Use Case:</h3> 
 * <p>
 * Suppose <b>ecc</b> is a toy C compiler build with Easy, 
 * its command line interface may implement as following code:
 * <p>
 *  
	<pre>
		...
		ArgsParser argz{args}; // args is command line arguments.
		argz.addHelpHeader(String{L"Usage: \n\t"}+argz.getProgramName()+String{L"\t[--help] | [--version]"});
		argz.addHelpHeader(String{L"\t"}+argz.getProgramName()+String{L"\t[--s] -o ouput inputs"});
		argz.addFlag(String{L"help"}, String{L"Print help info"});
		argz.addFlag(String{L"version"}, String{L"Print version info"});
		argz.addFlag(String{L"s"}, String{L"Compile as release"});
		argz.addOption(String{L"o"}, String{L"output"}, String{L"Supply output name"});
		argz.setVersionInfo(argz.getProgramName() + String{L" : ver 1.0"})
		
		try {
			argz.parse();
		} catch (ArgsParseException& e) {
			argz.printUsage();
			System::exit(-1);
		}
		
		e_bool isRelease = false;
		String output;
		
		if (argz.isTheOnlyFlag(String{L"help"})) {
			argz.printUsage();
			System::exit(0);
		}
		
		if (argz.isTheOnlyFlag(String{L"version"})) {
			argz.printVersion();
			System::exit(0);
		}
		
		while (!argz.isDone()) {
			if (argz.isFlag()) {
				if (argz.current().equals(String{L"s"})) {
					isRelease = true;
				}
			} else if (argz.isOption()) {
				if (argz.current().equals(String{L"o"})) {
					output = argz.getOptionValues(String{L"o"}).get(0);
				}
			}
			argz.advance();
		}
		
		//Console::outln(isRelease);
		//Console::outln(output);
	...
	
	Use ecc:
		ecc --help 
		ecc --version
		ecc -o main.o main.c
		ecc -s main.c -o main.exe
	</pre>
 * 
 * @author Tes Alone
 * @since 1.0
 * */

class ArgsParser : public EnableSharedFromThis<ArgsParser> {
private:
	static const e_int errIllegalFlag   = 0;
	static const e_int errIllegalOption = 1;
	static const e_int errOptionHasNoValue =2;
	
	static e_void error(const String& programName, int errCode, const String& sup) {
		String des;
		switch (errCode) {
			case 0:
			des = String{L"Undefined flag: "};
			break;
			case 1:
			des = String{L"Undefined option: "};
			break;
			case 2:
			des = String{L"Option has no values: "};
			break;
		}
		e_int color = Console::getColor();
		Console::setColor(Console::FR_WHITE|Console::BK_BLACK|Console::FR_HIGHLIGHT);
		Console::err(programName + L": ");
		Console::setColor(Console::FR_RED|Console::BK_BLACK|Console::FR_HIGHLIGHT);
		Console::err("error: ");
		Console::setColor(color);
		Console::errln(des + sup);
		Console::outln(String{L"-"} * 79);
		throw ArgsParseException{__func__, __LINE__};
	}
	
class Argument {
public:
	static const e_int FLAG   = 0;
	static const e_int OPTION = 1;
	static const e_int FREE   = 2;
	
	String m_text;
	e_int  m_type;
	
	Argument() {}
	
	Argument(e_int type, String text) {
		m_type = type;
		m_text = text;
	}
	
	e_bool equals(const Argument& other) const {
		return (m_type == other.m_type) 
				&& (m_text.equals(other.m_text));
	}
	
	String toString() const {
		String type;
		switch (m_type) {
			case FLAG:
				type = L"Flag:";
			break;
			case OPTION:
				type = L"Option:";
			break;
			case FREE:
				type = L"Free Option:";
			break;
		}
		return System::move(type + m_text);
	}
}; // Argument

private:	
	Array<String>  m_argz;
	e_int m_index;
	String m_look;
	
	Vector<String> m_flags;
	Vector<String> m_options;
	Vector<String> m_megs;
	String m_versionInfo;
	
	HashMap<String, Vector<String>> m_optionMap;
	Vector<Argument> m_result;
	e_int m_curArg;
	
public:
	/**
	 * \brief ctor.
	 * 
	 * @param args cmd line arguments, the first element of args is treated as program name.
	 * @throw if args` length is less than 1, throw an IllegalArgumentException.
	 * */
	explicit ArgsParser(const Array<String>& args) {
		if (args.length() < 1)
			throw IllegalArgumentException{__func__, __LINE__};
		m_argz   = args;
		m_curArg = 0;
		m_index  = 1;
	}	
	
	/**
	 * \brief Add flag.
	 * 
	 * @param flag a flag.
	 * @param meg info of the flag, used when print helo info.
	 * @throw if flag or meg is empty, throw an IllegalArgumentException.
	 * */
	
	void addFlag(const String& flag, const String& meg) {
		if (flag.empty() || meg.empty())
			throw IllegalArgumentException{__func__, __LINE__};
		
		m_flags.add(String{L"--"} + flag);
		String fixMeg = String::format(L"\t%-10s  %-15s%-s", String{L"--"}+flag+L":", String{}, meg);
		m_megs.add(fixMeg);
	}
	
	
	/**
	 * \brief Add option.
	 * 
	 * @param option a option name.
	 * @param holder option value holder, used when print helo info.
	 * @param meg info of the option, used when print helo info.
	 * 
	 * @throw if optio, holder or meg is empty, throw an IllegalArgumentException.
	 * */
	
	e_void addOption(const String& option, const String& holder, const String& meg) {
		if (option.empty() || holder.empty() || meg.empty())
			throw IllegalArgumentException{__func__, __LINE__};
		
		m_options.add(String{L"-"} + option);
		String fixMeg = String::format(L"\t%-10s <%-15s%-s", String{L"-"}+option, holder+L">:", meg);
		m_megs.add(fixMeg);
	}
	
	/**
	 * \brief Add help info.
	 * 
	 * <b>Note: </b>
	 * <br>
	 * <p>
	 * Although the method name is addHelpHeader, the added info will do not appear
	 * in front, clients need do the thing explicitly, i.e. call this methon in front.
	 * </p>
	 * 
	 * @param header help info
	 * @throw if header is empty, throw an IllegalArgumentException.
	 * */
	
	inline e_void addHelpHeader(const String& header) {
		if (header.empty())
			throw IllegalArgumentException{__func__, __LINE__};
		
		m_megs.add(header);
	}
	
	/**
	 * \brief Add help info.
	 * 
	 * <b>Note: </b>
	 * <br>
	 * <p>
	 * Although the method name is addTailHeader, the added info will do not appear
	 * at tail, clients need do the thing explicitly, i.e. call this methon at tail.
	 * </p>
	 * 
	 * @param tail help info
	 * @throw if tail is empty, throw an IllegalArgumentException.
	 * */
	inline e_void addHelpTail(const String& tail) {
		if (tail.empty())
			throw IllegalArgumentException{__func__, __LINE__};
		
		m_megs.add(tail);
	}
	
	/**
	 * \brief Add help info.
	 * 
	 * This method add a help info to receiver`s help info list.
	 * The first in info, the first on console.
	 * 
	 * @param aidHelp help info, may be empty.
	 * */
	inline e_void addAidHelp(const String& aidHelp) {		
		m_megs.add(aidHelp);
	}
	
	/**
	 * \brief Set version info.
	 * 
	 * Set the version info to receiver.
	 * 
	 * @param versionInfo the version info,  may be empty.
	 * */
	inline e_void setVersionInfo(const String& versionInfo) {
		m_versionInfo = versionInfo;
	}
	
	/**
	 * \brief Parse arguments.
	 * 
	 * @throw if reach a error (described as above), throw an ArgsParseException.
	 * */
	e_void parse() {
		static e_bool parsed = false;
		if (!parsed) {
			_args();
			if (_isEnd()) {
				parsed = true;
			}
		}
	}
	
	/**
	 * \brief Chech current argument whether a flag.
	 * 
	 * @return result of check.
	 * @throw if receiver is done, throw an IndexOutOfBoundsException.
	 * */
	
	inline e_bool isFlag() const {
		return m_result.get(m_curArg).m_type == Argument::FLAG;
	}
	
	
	/**
	 * \brief Chech current argument whether a option.
	 * 
	 * @return result of check.
	 * @throw if receiver is done, throw an IndexOutOfBoundsException.
	 * */
	inline e_bool isOption() const {
		return m_result.get(m_curArg).m_type == Argument::OPTION;
	}
	
	
	/**
	 * \brief Chech current argument whether a free option value.
	 * 
	 * @return result of check.
	 * @throw if receiver is done, throw an IndexOutOfBoundsException.
	 * */
	inline e_bool isFreeOption()  const {
		return !isFlag() && !isOption();
	}
	
	
	/**
	 * \brief Get current argument.
	 * 
	 * @return current argument.
	 * @throw if receiver is done, 抛出 IndexOutOfBoundsException.
	 * */
	inline String& current() const {
		return m_result.get(m_curArg).m_text;
	}
	
	/**
	* \brief Move to next argument.
	* 
	* This method does not check whether has next argument.
	* */
	inline e_void advance() {
		m_curArg++;
	}
	
	/**
	 * \brief Check whether has next argument.
	 * 
	 * @return if has not, return true, otherwise return false.
	 * */
	inline e_bool isDone() const {
		return m_curArg >= m_result.size();
	}
	
	/**
	 * \brief Get option values of a option.
	 * 
	 * @param option the option.
	 * @return values of the option .
	 * @throw if option is empty, throw an IllegalArgumentException.
	 * @throw if option undefined, throw a  HasNoSuchElementException.
	 * */
	
	inline Vector<String>& getOptionValues(const String& option) const {
		if (option.empty())
			throw IllegalArgumentException{__func__, __LINE__};
		return m_optionMap.get(option);
	}
	
	/**
	 * \brief Check whether has no argument supplied.
	 * 
	 * @return if not, return true, otherwise return false.
	 * */
	inline e_bool hasNoArguments() const {
		return m_result.empty();
	}
	
	/**
	 * \brief Print help info.
	 * */
	inline e_void printUsage() const {
		for (e_int i=0; i<m_megs.size(); i++) {
			Console::outln(m_megs[i]);
		}
	}
	
	/**
	 * \brief Print version info.
	 * 
	 * Print the version info set by setVersionInfo();
	 * */
	 inline e_void printVersion() const {
	 	Console::outln(m_versionInfo);
	 }
	
	/**
	 * \brief Get program name.
	 * 
	 * Receiver treats the first element in the ctor`s first argumnet as program name.
	 * 
	 * @return program name.
	 * */
	
	inline String& getProgramName() const {
		return m_argz[0];
	}
	
	/**
	 * \brief Check whether flag is the only argument.
	 * 
	 * @param flag the checked flag.
	 * @return result.
	 * @throw if flag is empty, throw an IllegalArgumentException.
	 * */
	
	e_bool isTheOnlyFlag(const String& flag) const {
		if (flag.empty())
			throw IllegalArgumentException{__func__, __LINE__};
		return m_result.size()==1 && m_result[0].m_text.equals(flag);
	}
	

	inline String toString() const override {
		return System::move(String{L"Args:"} + m_argz + L";" + m_result);
	}
private:	 
	 e_void _args() {
		while (true) {
			if (!_isEnd() && _isFlag(_current())) {
				if (!m_flags.contains(_current())) {
					error(m_argz[0], errIllegalFlag, _current());
				}
				String trimMinus = _current().substring(2);
				Argument arg{Argument::FLAG, trimMinus};
				if (!m_result.contains(arg)) m_result.add(arg);
				_advance();
			} else if (!_isEnd() && _isOption(_current())) {
				if (!m_options.contains(_current())) {
					error(m_argz[0], errIllegalOption, _current());
				}
				String trimMinus = _current().substring(1);
				Argument arg{Argument::OPTION, trimMinus};
				if (!m_result.contains(arg)) m_result.add(arg);
				try {
					Vector<String>& values = m_optionMap.get(trimMinus);
					String option = _current();
					_advance();
					if (!_isEnd() && _isOptionValue(_current())) {
						if (!values.contains(_current())) values.add(_current());
						_advance();
						while (!_isEnd() && _isOptionValue(_current())) {
							if (!values.contains(_current())) values.add(_current());
							_advance();
						}
					} else {
						error(m_argz[0], errOptionHasNoValue, option);
					}
				} catch (HasNoSuchElementException& e) {
					Vector<String> tmpValues;
					m_optionMap.put(trimMinus, tmpValues);
					Vector<String>& values = m_optionMap.get(trimMinus);
					String option = _current();
					_advance();
					if (!_isEnd() && _isOptionValue(_current())) {
						if (!values.contains(_current())) values.add(_current());
						_advance();
						while (!_isEnd() && _isOptionValue(_current())) {
							if (!values.contains(_current())) values.add(_current());
							_advance();
						}
					} else {
						error(m_argz[0], errOptionHasNoValue, option);
					}
				}
			} else if (!_isEnd() && _isFreeOptionValue(_current())) {
				Argument arg{Argument::FREE, _current()};
				if (!m_result.contains(arg)) m_result.add(arg);
				_advance();
			} else {
				break;
			}
		}
	}
	
	String& _current() {
		return m_argz[m_index];
	}
	
	e_void _advance() {
		m_index++;
	}
	
	e_bool _isEnd() {
		return m_index >= m_argz.length();
	}
	
	e_bool _isFlag(String& look) {
		return look.length()>2 && look.startsWith(String{L"--"});
	}
	
	e_bool _isOption(String& look) {
		return look.length()>1 && look.startsWith(String{L"-"}) 
					&& look[1]!='-';
	}
	
	e_bool _isOptionValue(String& look) {
		return !_isFlag(look) && !_isOption(look);
	}
	
	e_bool _isFreeOptionValue(String& look) {
		return _isOptionValue(look);
	}
}; // ArgsParser
} // Easy

#endif // _EASY_CMDPARSER_H_

