/*
 * 文件名: Regex.hpp 
 *
 * 日期/时间:
 *		开始: 2020年05月16日 20:31:32 周六
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#ifndef _EASY_REGEX_H_
#define _EASY_REGEX_H_

#include "../BaseTypes.hpp"
#include "../Character.hpp"
#include "../String.hpp"
#include "../SharedPointer.hpp"
#include "../Exception.hpp"
#include "../HashSet.hpp"
#include "../Vector.hpp"
#include "../Console.hpp"


namespace Easy {

namespace Regex {

class Pattern;

class Matcher {
	
friend class Pattern;
friend class Node;
friend class Start;
friend class Char;
friend class Slice;
friend class Or;
friend class Loop;
friend class Clazz;
friend class Dot;


private:
	SharedPointer<Pattern> _pattern;
	IntArray _groups;
	String   _text;
	e_int    _last;
	e_int    _first;
	
	Matcher(const SharedPointer<Pattern>& pattern, const String& text);
	
public:
	e_bool matches(); // can never be const
}; // Matcher

class Node : public EnableSharedFromThis<Node> {
public:
	static const e_int START = 0;
	static const e_int ENDZ  = 1;
	static const e_int CHARZ = 2;
	static const e_int SLICE = 3;
	static const e_int OR    = 4;
	static const e_int LOOP  = 5;
	static const e_int CLAZZ = 6;
	static const e_int DOT   = 7;
	
	e_int _type;
	
	SharedPointer<Node> _next;
	
	Node() {}
	
	Node(e_int type) {
		_type = type;
	}
	
	virtual ~Node() {
		Console::out("type: "); // FIXME
		Console::outln(_type);
	}
	
	virtual e_bool match(Matcher& matcher, const String& text, e_int offset) const =0;
	
	virtual String toString() const =0;
}; // Node

class Start : public Node {
public:
	Start(const SharedPointer<Node>& next) 
		: Node(Node::START) {
		_next = next;	
	}
	
	e_bool match(Matcher& matcher, const String& text, e_int offset) const override {
		matcher._first = offset;
		e_bool result  = _next->match(matcher, text, offset);
		if (!result) {
			matcher._first = -1;
			matcher._last  = 0;
		} else {
			matcher._groups[0] = matcher._first;
			matcher._groups[1] = matcher._last;
		}
		return result;
	}
	
	String toString() const {
		return L"Start";
	}
}; // Start

class Or : public Node {
public:
	Vector<SharedPointer<Node>> _branches;
	
	Or() : Node(Node::OR) {}
	
	e_void addBran(const SharedPointer<Node>& bran) {
		_branches.add(bran);
	}
	
	e_bool match(Matcher& matcher, const String& text, e_int offset) const override {
		if (_branches.empty()) return true; // for default _next in others.
		
		for (e_int i=0; i<_branches.size(); i++) {
			if (_branches[i]->match(matcher, text, offset)) {
				if (!_next.isNullPtr()) {
					return _next->match(matcher, text, matcher._last);
				}
				return true;
			}
		}
		return false;
	}
	
	String toString() const {
		return L"Branch";
	}
}; // Or


class Char : public Node {
public:
	e_char _c;
	
	Char() {}
	
	Char(e_char c) 
		: Node(Node::CHARZ) {
		_c = c;
		_next = SharedPointer<Node>{new Or};
	}
	
	e_bool match(Matcher& matcher, const String& text, e_int offset) const override {
		e_int len = text.length() - offset;
		if (len >= 1) {
			e_bool result = _c == text[offset];
			if (result) {
				matcher._last = offset + 1;
				return _next->match(matcher, text, matcher._last);
			}
			return result;
		}
		return false;
	}
	
	String toString() const override {
		return String{L"Char "} + Character{_c};
	}
}; // Char

class Slice : public Node {
public:
	String _slice;
	
	Slice() {}
	
	Slice(const String& slice) 
		: Node(Node::SLICE)	{
		_slice = slice;
		_next  = SharedPointer<Node>{new Or};
	}
	
	e_bool match(Matcher& matcher, const String& text, e_int offset) const override {
		e_int len = text.length() - offset;
		if (len >= _slice.length()) {
			e_bool result = true;
			for (e_int i=0; i<_slice.length(); i++) {
				if (_slice[i] != text[i+offset]) {
					result = false;
					break;
				}
			}
			if (result) {
				matcher._last = offset + _slice.length();
				return _next->match(matcher, text, matcher._last);
			}
			return result;
		}
		return false;
	}
	
	String toString() const override {
		return String{L"Slice "} + _slice;
	}
}; // Slice

class Loop : public Node {
public:
	static const e_int GREEDY     = 0;
	static const e_int LAZY       = 1;
	static const e_int POSSESSIVE = 2;
	
	SharedPointer<Node> _body;
	
	e_int _cmax;
	e_int _cmin;
	e_int _loopType;
	
	Loop() {}
	
	Loop(const SharedPointer<Node>& body, e_int cmin, e_int cmax, e_int loopType)
		: Node(Node::LOOP) {
		_body = body;
		_cmin = cmin;
		_cmax = cmax;
		_loopType = loopType;
		_next = SharedPointer<Node>{new Or{}};
	}
	
	e_bool match(Matcher& matcher, const String& text, e_int offset) const override {
		e_int j; // j 这个变量用于记录匹配的次数
        for (j = 0; j < _cmin; j++) {
            if (_body->match(matcher, text, offset)) {
                offset = matcher._last;
                continue;
            }
            return false;
        }
        if (_loopType == GREEDY) {
        	return match0(matcher, text, offset, j);
        } else if (_loopType == LAZY) {
        	return match1(matcher, text, offset, j);
        } else {
        	return match2(matcher, text, offset, j);
        }
    }
    
    // Greedy match.
    // i is the index to start matching at
    // j is the number of atoms that have matched
    e_bool match0(Matcher& matcher, const String& text, e_int i, e_int j) const {
        if (j >= _cmax) {
            return _next->match(matcher, text, i);
        }
        // 如果执行到这里, 说明匹配的次数 (j)小于 _cmax, 而且一定等于 _cmin.
        int backLimit = j; // 记录匹配次数, 也就是 _cmin.
        while (_body->match(matcher, text, i)) {
            e_int k = matcher._last - i; // k 用来记录 _body 匹配的长度.
            if (k == 0) // 如果 k 为 0, (对于某些节点, 匹配不消耗输入字符串, 比如 Loop 节点), 也有防止无限循环的意思.
                break;
            i = matcher._last; // 更新 i, 下次匹配从 i 开始.
            j++;			  // 又一次匹配成功.

            while (j < _cmax) { // 进行贪心匹配, 直到遇到错误, 或到达最大匹配限度 (_cmax)
                if (!_body->match(matcher, text, i)) // 如果遇到错误, 进入回溯步骤.
                    break;
                if (i + k != matcher._last) { // 这里的意思是如果从上次匹配的开始位置 i 加上匹配长度 k
                							  // 不等于 _last, 可能有些节点会有这种行为, 目前 Easy 不 care.
                    if (match0(matcher, text, matcher._last, j+1))
                        return true;
                    break;
                }
                i += k; // 如果匹配成功, 更新 i, 下次匹配从 i 开始.
                j++; // 记录匹配成功次数
            }
            
            // 这里的 j 比较 backLimit 的意思是回退不应该超过最小匹配次数, 也就是 _cmin.
            while (j >= backLimit) { // 到这里, 因为  _body 的匹配失败, _body 失败的因素根据节点的类型而变,
            						 // 不过失败的原因也包括了到达输入尾部.
               if (_next->match(matcher, text, i))
                    return true;
                i -= k;
                j--;
            }
            return false;
        }
        return _next->match(matcher, text, i);
    }
    
    // Reluctant match. At this point, the minimum has been satisfied.
    // i is the index to start matching at
    // j is the number of atoms that have matched
    e_bool match1(Matcher& matcher, const String& text, e_int i, e_int j) const {
        for (;;) {
            // Try finishing match without consuming any more
            if (_next->match(matcher, text, i))
                return true;
            // At the maximum, no match found
            if (j >= _cmax)
                return false;
            // Okay, must try one more atom
            if (!_body->match(matcher, text, i))
                return false;
            // If we haven't moved forward then must break out
            if (i == matcher._last) // 防止无限循环.
                return false;
            // Move up index and number matched
            i = matcher._last;
            j++;
        }
    }
    
    // possessive match
    e_bool match2(Matcher& matcher, const String& text, e_int i, e_int j) const {
        for (; j < _cmax; j++) {
            if (!_body->match(matcher, text, i))
                break;
            if (i == matcher._last) // 防止无限循环.
                break;
            i = matcher._last;
        }
        return _next->match(matcher, text, i);
    }
    
    String toString() const override {
    	return String{L"Loop "} + Integer{_cmin} + L"," 
    			+ Integer{_cmax} + L"," + (_loopType==GREEDY ? L"Greed" 
    					: (_loopType==LAZY? L"Lazy" : L"Possessive"));
    }
}; // Loop

class Clazz : public Node {

class Pair {
public:
	e_char start;
	e_char end;
	
	String toString() const {
		return String{} + Integer{start} + String{L":"} + Integer{end};
	}
}; // Pair

public:	
	e_bool _isNeg;
	HashSet<Character> _singleCharList;
	Vector<Pair> _pairList;
	SharedPointer<Node> _other;
	
	Clazz() 
		: Node(Node::CLAZZ) {
		_isNeg = false;
		_next  = SharedPointer<Node>{new Or};
	}
	
	e_void addSingleChar(e_char c) {
		Character cc{c};
		if (!_singleCharList.contains(cc)) {
			_singleCharList.add(cc);
		}
	}
	
	e_void addPair(e_char start, e_char end) {
		_pairList.add(Pair{start, end});
	}
	
	e_bool contains(e_char c) const {
		Character cc{c};
		if (_singleCharList.contains(cc)) {
			return true;
		} else {
			for (e_int i=0; i<_pairList.size(); i++) {
				Pair pair = _pairList.get(i);
				if (c>=pair.start && c<=pair.end) {
					return true;
				}
			}
		}
		return false;
	}
	
	e_bool empty() const {
		return _singleCharList.empty() && _pairList.empty();
	}
	
	e_bool match(Matcher& matcher, const String& text, e_int offset) const override {
		e_int restLen = text.length() - offset;
		if (restLen >= 1) {
			e_bool inClass = contains(text[offset]);
			e_bool inOtherClass = false;
			e_bool result = false;
			if (!_other.isNullPtr()) {
				inOtherClass = _other->lockTo<Clazz>()->contains(text[offset]);
			}
			
			if (_isNeg) {
				if (!_other.isNullPtr()) {
					if (_other->lockTo<Clazz>()->_isNeg) {
						result = (!inClass && !inOtherClass);
					} else {
						result = (!inClass && inOtherClass);
					}
				} else {
					result = !inClass;
				}
			} else {
				if (!_other.isNullPtr()) {
					if (_other->lockTo<Clazz>()->_isNeg) {
						result = (inClass && !inOtherClass);
					} else {
						result = (inClass && inOtherClass);
					}
				} else {
					result = inClass;
				}
			}
			if (result) {
				matcher._last = offset + 1;
				return _next->match(matcher, text, matcher._last);
			}
			return result;
		}
		return false;
	}
	
	String toString() const override {
		return String{L"Class "} + (_isNeg?L"isNeg ":L" ")
				+  _singleCharList + L" " + _pairList;
	}
}; // Clazz

class Dot : public Node {
public:
	e_bool _dotAll;
	Dot(e_bool dotAll)
		: Node(Node::DOT) {
		_dotAll = dotAll;
		_next = SharedPointer<Node>{new Or};
	}
	
	e_bool match(Matcher& matcher, const String& text, e_int offset) const override {
		e_int len = text.length() - offset;
		if (len >= 1) {
			matcher._last = offset + 1;
			return _next->match(matcher, text, matcher._last); // 暂时忽略 dotall
		} 
		return false;
	}
	
	String toString() const override {
		return L"Dot";
	}
}; //

class Pattern : public EnableSharedFromThis<Pattern> {
	
friend class Matcher;

class Token {
public:	
	String _lexeme;
	e_int  _index;
	e_bool _isMeta;
	e_int  _num;
	
	Token() {
		_num = -1;
	}
	
	Token(const String& lexeme, e_bool isMeta, e_int index) {
		_lexeme = lexeme;
		_isMeta = isMeta;
		_index  = index;
		_num    = -1;
	}
	
	String toString() const {
		return String{L"Lexeme:"} + _lexeme
					+ String{L", Index:"} + Integer{_index}
						+ String{L", IsMeta:"} + Bool{_isMeta};
 	}
}; // Token

private:
	String _pattern;
	e_int  _index;
	Vector<Token> _tokenList;
	HashSet<Character> _metaCharSet;
	
	SharedPointer<Node> _root;
	e_bool _isCompiled;
	e_int  _groupCount;
public:
	static SharedPointer<Pattern> compile(const String& regex) {
		SharedPointer<Pattern> pattern{new Pattern{regex}};
		pattern->_compile();
		return pattern;
	}

	Matcher matcher(const String& text) {
		return Matcher{sharedFromThis(), text};
	}
	
	e_void printPattern() {
		_walk(_root, 0);
	}

	String toString() const override {
		return System::move(_pattern.substring(0, _pattern.length()-1));
	}
private:
	explicit Pattern(const String& pattern) {
		if (pattern.empty()) {
			throw IllegalArgumentException{__func__, __LINE__};
		}
		_pattern = pattern + Character::SENTINEL;
		_index   = 0;
		_isCompiled = false;
		_groupCount = 1;
	}

	e_void _compile() {
		if (!_isCompiled) {
			_tokenization();
			Console::outln(_tokenList);
			_index = 0;
			_root  = _expr();
			_isCompiled = true;
		}
	}

	e_void _print(const String& nodeInfo, e_int depth) {
		Console::outln(String{L" "}*depth + L"<" + nodeInfo + L">");
	}
	
	e_void _walk(SharedPointer<Node> node, e_int depth) {
		depth++;
		while (!node.isNullPtr()) {
			if (node->_type == Node::START) {
				String str = node->toString();
				_print(str, depth);
			} else if (node->_type == Node::CHARZ) {
				String info = node->toString();
				_print(info, depth);
			} else if (node->_type == Node::SLICE) {
				String info = node->toString();
				_print(info, depth);
			} else if (node->_type == Node::OR) {
				Vector<SharedPointer<Node>>& branches = node->lockTo<Or>()->_branches;
				if (branches.empty()) {
					return;
				}
				_print(L"Branch ", depth);
				e_int i;
				for (i=0; i<branches.size()-1; i++) {
					_walk(branches[i], depth+1);
					_print(String{L"-"}*20, depth+1);
				}
				if (!branches.empty()) {
					_walk(branches[i], depth+1);
				}
				_print(L"Branch End", depth);
			} else if (node->_type == Node::LOOP) {
				String info = node->toString();
				_print(info, depth);
				_walk(node->lockTo<Loop>()->_body, depth+1);
				_print(L"Loop End", depth);
			} else if (node->_type == Node::CLAZZ) {
				String info = node->toString();
				_print(info, depth);
				if (!node->lockTo<Clazz>()->_other.isNullPtr()) 
					_walk(node->lockTo<Clazz>()->_other, depth+1);
				_print(L"Class End", depth);
			} else if (node->_type == Node::DOT) {
				_print(node->toString(), depth);
			}
			node = node->_next;
		}
	}
	
	/**
	 * 
	 * 初始文法: 
	 * 	Regex -> Or 
	 * 	Or  -> Or \| Cat | Cat
	 *  Cat -> Cat StarLike | StarLike
	 *  StarLike -> StarLike * | StarLike + | StarLike ? | Atom
	 *  Atom -> CHAR | ( Regex ) | Class | DOT
	 *  Class -> [ Neg SinglePairList ClassOperator [ Neg SinglePairList ] ]
	 *  ClassOperator -> && | EMPTY
	 *  SinglePairList -> CHAR SinglePairList | Pair SinglePairList | CHAR | Pair
	 *  Pair -> CHAR - CHAR
	 *  Neg -> ^ | EMPTY
	 * 
	 * 去除左递归:
	 * 	Regex -> Or
	 *  Or 	-> Cat Or`
	 * 	Or` -> \| Cat Or` | EMPTY
	 * 	Cat  -> StarLike Cat`
	 *  Cat` -> StarLike Cat` | EMPTY
	 *  StarLike -> Atom StarLike`
	 *  StarLike` -> * StarLike` | + StarLike` | ? StarLike` | EMPTY 
	 * 	Atom -> CHAR | ( Regex ) | Class | DOT
	 *  Class -> [ Neg SinglePairList ClassOperator [ Neg SinglePairList ] ]
	 *  ClassOperator -> && | EMPTY
	 *  SinglePairList -> CHAR SinglePairList | Pair SinglePairList | CHAR | Pair
	 *  Pair -> CHAR - CHAR
	 *  Neg -> ^ | EMPTY
	 * */
	SharedPointer<Node> _expr() {
		SharedPointer<Node> start{new Start{_or()}};
		if (!_isDone()) {
			throw Exception(L"parse error.", __func__, __LINE__);
		}
		return start;
	}
	
	SharedPointer<Node> _or() {
		SharedPointer<Node> oR{new Or};
		oR->lockTo<Or>()->addBran(_sequence());
		while (true) {
			if (!_isDone() && _current()._lexeme.equals(L"|")) {
				_move();
				oR->lockTo<Or>()->addBran(_sequence());
			} else {
				break;
			}
		}
		return oR;
	}
	
	SharedPointer<Node> _sequence() {
		SharedPointer<Node> node = _loop();
		SharedPointer<Node> temp = node;
		
		while (true) {
			if (!_isDone() && _current()._isMeta && _current()._lexeme[0]=='(') {
				SharedPointer<Node> group = _group0();
				if (!group.isNullPtr()) {
					temp->_next = group;
					temp = temp->_next;
				}
			} else if (!_isDone() && _isAtomStart(_current())) {
				temp->_next = _loop();
				temp = temp->_next;
			} else {
				break;
			}
		}
		return node;
	}
	
	e_bool _isAtomStart(const Token& t) {
		return !t._isMeta || t._lexeme[0]=='(' || t._lexeme[0]=='[' || t._lexeme[0]=='.';
	}
	
	static const e_int MAX_REP = Integer::MAX;
	
	SharedPointer<Node> _loop() {
		SharedPointer<Node> body = _atom();
		if (!_isDone() && _current()._isMeta &&  _current()._lexeme.equals(L"*")) {
			_move();
			return SharedPointer<Node>{new Loop{body, 0, MAX_REP, Loop::GREEDY}};
		} else if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L"+")) {
			_move();
			return SharedPointer<Node>{new Loop{body, 1, MAX_REP, Loop::GREEDY}};
		} else if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L"?")) {
			_move();
			return SharedPointer<Node>{new Loop{body, 0, 1, Loop::GREEDY}};
		} else if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L"*?")) {
			_move();
			return SharedPointer<Node>{new Loop{body, 0, MAX_REP, Loop::LAZY}};
		} else if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L"+?")) {
			_move();
			return SharedPointer<Node>{new Loop{body, 1, MAX_REP, Loop::LAZY}};
		} else if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L"??")) {
			_move();
			return SharedPointer<Node>{new Loop{body, 0, 1, Loop::LAZY}};
		} else if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L"*+")) {
			_move();
			return SharedPointer<Node>{new Loop{body, 0, MAX_REP, Loop::POSSESSIVE}};
		} else if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L"++")) {
			_move();
			return SharedPointer<Node>{new Loop{body, 1, MAX_REP, Loop::POSSESSIVE}};
		} else if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L"?+")) {
			_move();
			return SharedPointer<Node>{new Loop{body, 0, 1, Loop::POSSESSIVE}};
		} else if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L"{")) {
			return _curlyLoop(body);
		} else {
			return body;
		}
	}
	
	SharedPointer<Node> _curlyLoop(SharedPointer<Node> body) {
		_move();
		if (!_isDone() && _current()._num!=-1) {
            e_int cmin = _current()._num;
            e_int cmax = cmin;
            _move();
            if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L",")) {
                _move();
                cmax = MAX_REP;
                if (!_isDone() && _current()._num!=-1) {
                    cmax = _current()._num;
                    _move();
                }
            }
            if (_isDone() || !_current()._isMeta 
            		|| (_current()._isMeta && !_current()._lexeme.equals(L"}")))
                throw Exception(L"parse error.", __func__, __LINE__);
            if (((cmin) | (cmax) | (cmax - cmin)) < 0)
                throw Exception(L"parse error.", __func__, __LINE__);
            _move();
            SharedPointer<Node> curly;
            if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L"?")) {
                _move();
                curly = SharedPointer<Node>{new Loop(body, cmin, cmax, Loop::LAZY)};
            } else if (!_isDone() && _current()._isMeta &&  _current()._lexeme.equals(L"+")) {
                _move();
                curly = SharedPointer<Node>{new Loop(body, cmin, cmax, Loop::POSSESSIVE)};
            } else {
                curly = SharedPointer<Node>{new Loop(body, cmin, cmax, Loop::GREEDY)};
            }
            return curly;
        } else {
            throw Exception(L"parse error.", __func__, __LINE__);
        }
	}
	
	SharedPointer<Node> _atom() {
		if (!_isDone()) {
			if (_current()._isMeta && _current()._lexeme[0]=='(') {
				return _group0();
			} else if (_current()._isMeta && _current()._lexeme[0]=='[') {
				SharedPointer<Node> clazz = _clazz();
				_move();
				return clazz;
			} else if (_current()._isMeta && _current()._lexeme[0]=='.') {
				_move();
				return SharedPointer<Node>{new Dot{true}};
			} else if (!_current()._isMeta) {
				StringBuffer sb;
				while (!_isDone() && !_current()._isMeta) {
					sb.append(Character{_current()._lexeme[0]});
					_move();
				}
				
				if (sb.length() == 1) { // can not be less 1
					return SharedPointer<Node>{new Char{sb[0]}};
				} else {
					return SharedPointer<Node>{new Slice{sb.toString()}};
				}
			} else {
				Console::outln(_current());
				throw Exception(L"parse error.", __func__, __LINE__);
			}
		} else {
			throw Exception(L"parse error.", __func__, __LINE__); 
		}
	}
	
	SharedPointer<Node> _group0() {
		if (_current()._isMeta && _current()._lexeme.equals(L"(")) {
			_move();
			SharedPointer<Node> re = _or();
			if (!_isDone() && _current()._isMeta && _current()._lexeme.equals(L")")) {
				_move();
				return re;
			} else {
				throw Exception(L"parse error.", __func__, __LINE__);
			}
		}
		throw Exception(L"parse error.", __func__, __LINE__);
	}
	
	SharedPointer<Node> _clazz() {
		_move();
		SharedPointer<Node> clazz{new Clazz};
		e_bool isFirst = true;
		for (;;) {
			if (isFirst && !_isDone() && _current()._lexeme.equals(L"^")) {
				clazz->lockTo<Clazz>()->_isNeg = true;
				_move();
				isFirst = false;
			} else if (!_isDone(2) && _after(1)._lexeme.equals(L"-")) {
				e_char start = _current()._lexeme[0];
				e_char end   = _after(2)._lexeme[0];
				if (start >= end) {
					throw Exception(L"parse error.", __func__, __LINE__);
				} else {
					clazz->lockTo<Clazz>()->addPair(start, end);
					_index += 3;
					isFirst = false;
				}
			} else if (!_isDone() && _current()._lexeme.equals(L"[")) {
				clazz->lockTo<Clazz>()->_other = _clazz();
				_move();
				if (_isDone() || _current()._lexeme[0]!=']') {
					throw Exception(L"parse error.", __func__, __LINE__);
				} else {
					break;
				}
			} else if (!_isDone() && !_current()._lexeme.equals(L"]")) {
				clazz->lockTo<Clazz>()->addSingleChar(_current()._lexeme[0]);
				_move();
				isFirst = false;
			} else if (!_isDone() && _current()._lexeme.equals(L"]")) {
				break;
			} else if (_isDone()) {
				throw Exception(L"parse error.", __func__, __LINE__);
			}
		}
		if (clazz->lockTo<Clazz>()->empty()) {
			throw Exception(L"parse error.", __func__, __LINE__);
		}
		return clazz;		
	}
	
	Token& _after(e_int after) {
		return _tokenList[_index + after];
	}
	
	Token& _current() {
		return _tokenList[_index];
	}
	
	e_void _move() {
		_index++;
	}
	
	e_bool _isDone(e_int need=0) {
		return _index+need >= _tokenList.size();
	}
	
	e_void _tokenization() {
		e_bool passLeftBrace = false;
		while (true) {
			_filterWhiteSpace();
			switch (_pattern[_index]) {
				case '*':
				case '+':
				case '?':
				{
					switch(_pattern[_index+1]) {
						case '?':
						case '+':
							_addToken(_index, _index+2, true);
							_index += 2;
							break;
						case '*': // dangling *
							throw Exception(L"parse error.", __func__, __LINE__);
							break;
						case Character::sentinel:
							_addToken(_index, _index, true);
							goto breakLoop;
						default:
							_addToken(_index, _index, true);
							_index++;
					}
					break;
				}
				case Character::sentinel:
					goto breakLoop;
				case '\\':
				{
					switch (_pattern[++_index]) {
						case Character::sentinel:
							_addToken(_index-1, _index, false);
							goto breakLoop;
						case '*':
						case '+':
						case '?':
						case '|':
						case '(':
						case ')':
						case '[':
						case ']':
						case '-':
						case '.':
						case '^':
						case '$':
						case '{':
						case '}':
						case ',':
						case '\\':
							_addToken(_index, _index, false);
							_index++;
							break;
						default:
							throw Exception(L"parse error.", __func__, __LINE__);
					}
					break;
				}
				case '(':
				{
					_index++;
					if (!_isEnd(1) && _pattern[_index]=='?' 
										&& _pattern[_index+1]=='=') { // lookahead
						_addToken(_index-1, _index+2, true);
						_index += 2;
					} else if (!_isEnd(1) && _pattern[_index]=='?'
										&& _pattern[_index+1]=='!') { // lookahead negative
						_addToken(_index-1, _index+2, true);
						_index += 2;
					} else if (!_isEnd(2) && _pattern[_index]=='?'
											&& _pattern[_index+1]=='<'
												&& _pattern[_index+2]=='=') { // look behind
						_addToken(_index-1, _index+3, true);
						_index += 3;
					} else if (!_isEnd(2) && _pattern[_index]=='?'
											&& _pattern[_index+1]=='<'
												&& _pattern[_index+2]=='!') { // look behind negative
						_addToken(_index-1, _index+3, true);
						_index += 3;
					} else if (!_isEnd(1) && _pattern[_index]=='?'
											&& _pattern[_index+1]=='#') { // comment
						_addToken(_index-1, _index+2, true);
						_index += 2;
					} else if (!_isEnd()) {
						_addToken(_index-1, _index, true);
					} else if (_isEnd()) {
						goto breakLoop;
					}
					break;
				}
				case '|':
				case ')':
				case '[':
				case ']':
				case '-':
				case '.':
				case '^':
				case '$':
				case '{':
					passLeftBrace = true;
					_addToken(_index, _index, true);
					_index++;
					break;
				case '}':
					passLeftBrace = false;
				case ',':
					_addToken(_index, _index, true);
					_index++;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					if (passLeftBrace) {
						_addNumber(_index);
						break;
					}
				default:
					_addToken(_index, _index, false);
					_index++;
			}
		}
		breakLoop:;
	}
	
	e_void _addNumber(e_int index) {
		e_char ch  = _pattern[_index];
		e_int  num = 0;
		do {
        	num = num * 10 + (ch - '0');
        } while (Character::isDigit(ch = _pattern[++_index]));
        Token tk{L"", false, index}; // 这里的 false 不会影响 _sequence() 的解析, 
        							// 这个 token 的解析发生在 { 之后, } 之前, 不会影响其它部分 FIXME
        tk._num = num;
        _tokenList.add(tk);
	}
	
	e_void _addToken(e_int start, e_int end, e_bool isMeta) {
		_tokenList.add(Token{_pattern.substring(start, end), isMeta, start});
	}
	
	e_bool _isEnd(e_int need=0) {
		return _index+need >= _pattern.length();
	}
	
	e_void _filterWhiteSpace() {
		
	}
	
	Pattern(Pattern& copy) =delete;
	Pattern& operator=(Pattern& copy) =delete;
	Pattern(Pattern&& move) =delete;
	Pattern& operator=(Pattern&& move) =delete;
}; // Pattern

} // Regex
} // Easy

#endif // _EASY_REGEX_H_
