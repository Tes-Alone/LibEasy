/*
 * File Name:	LXML.cpp
 * Date/Time:
 * 	Start: 	2019年12月20日 18:19:41 周五
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note: FIXME
*/

#include "LXML.hpp"

#include "Iterator.hpp"
#include "StringBuffer.hpp"
#include "FileWriter.hpp"
#include "FileReader.hpp"
#include "System.hpp"
#include "IOUtility.hpp"

#include "Console.hpp"

namespace Easy {

namespace LXML {
	
NodeList::NodeList(const String& parent) {
	m_parent = parent;
}

SharedPointer<Node> NodeList::addNode(SharedPointer<Node>& node) {
	if (node.isNullPtr()) {
		throw NullPointerException{__func__, __LINE__};
	}
	return m_nodes.add(System::move(node));
}

SharedPointer<Node> NodeList::addNodeByTag(const String& tag) {
	if (tag.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	return m_nodes.add(SharedPointer<Node>{new Node{tag}});
}


SharedPointer<Node> NodeList::getNodeAt(e_int index) const {
	return m_nodes.get(index);
}

SharedPointer<Node> NodeList::getNode(const SharedPointer<Node>& node) const {
	if (node.isNullPtr()) {
		throw NullPointerException{__func__, __LINE__};
	}
	try {
		return m_nodes.get(node);
	} catch (HasNoSuchElementException& e){
		throw HasNoSuchNodeException{__func__, __LINE__, m_parent, node->getTag()};
	}
}

SharedPointer<Node> NodeList::removeNodeAt(e_int index) {
	return System::move(m_nodes.remove(index));
}

SharedPointer<Node> NodeList::removeNode(const SharedPointer<Node>& node) {
	if (node.isNullPtr()) {
		throw NullPointerException{__func__, __LINE__};
	}
	try {
		return System::move(m_nodes.remove(node));
	} catch (HasNoSuchElementException& e){
		throw HasNoSuchNodeException{__func__, __LINE__, m_parent, node->getTag()};
	}
}

SharedPointer<Node> NodeList::getNodeByAttr(const String& name, const String& value) const {
	if (name.empty() || value.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	for (e_int i=0; i<m_nodes.size(); i++) {
		SharedPointer<Node>& node = m_nodes.get(i);
		AttrMap& attrs = node->getAttrMap();
		try {
			String attrValue = attrs.getAttr(name);
			if (attrValue.equals(value)) {
				return node;
			}
		} catch (HasNoSuchAttrException& e) {
			throw HasNoSuchNodeException{__func__, __LINE__, m_parent,
						 String{L"which has attribute '"} 
						 	+ name + String{L"' with value '"} + value + String{L"'"}};
		}
	}
	
	throw HasNoSuchNodeException{__func__, __LINE__, m_parent,
						 String{L"which has attribute '"} 
						 	+ name + String{L"' with value '"} + value + String{L"'"}};
}

SharedPointer<Node> NodeList::getNodeByTag(const String& tag) const {
	if (tag.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	for (e_int i=0; i<m_nodes.size(); i++) {
		SharedPointer<Node>& node = m_nodes.get(i);
		if (node->getTag().equals(tag)) {
			return node;
		}
	}
	
	throw HasNoSuchNodeException{__func__, __LINE__, m_parent, tag};
}

e_int NodeList::getLength() const {
	return m_nodes.size();
}

AttrMap::AttrMap(const String& node) {
	m_node = node;
}

e_void AttrMap::addAttr(const String& name, const String& value) {
	if (name.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	m_map.put(name, value);
}

e_void AttrMap::removeAttr(const String& name) {
	if (name.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	try {
		m_map.remove(name);
	} catch (HasNoSuchElementException& e) {
		throw  HasNoSuchAttrException{__func__, __LINE__, m_node, name};
	}
}

String& AttrMap::getAttr(const String& name) const {
	if (name.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	try {
		return m_map.get(name);
	} catch (HasNoSuchElementException& e) {
		throw HasNoSuchAttrException{__func__, __LINE__, m_node, name};
	}
}

e_int AttrMap::getLength()const {
	return m_map.size();
}

HashSet<String> AttrMap::listAttrs() {	
	return System::move(m_map.keySet());
}

Node::Node() {
	m_tag     = System::move(String{L"default"});
	m_nodes   = System::move(NodeList{m_tag});
	m_attrs   = System::move(AttrMap{m_tag});
}

Node::Node(const String& tag) {
	m_tag     = tag;
	m_nodes   = System::move(NodeList{m_tag});
	m_attrs   = System::move(AttrMap{m_tag});
}

e_void Node::setTag(const String& tag) {
	if (tag.empty()) {
		throw IllegalArgumentException{__func__, __LINE__};
	}
	m_tag = tag;
}

e_void Node::setContent(const String& content) {
	m_content = content;
}


NodeList& Node::getNodeList() {
	return m_nodes;
}

AttrMap& Node::getAttrMap() {
	return m_attrs;
}

const String& Node::getTag() const {
	return m_tag;
}

const String& Node::getContent() const {
	return m_content;
}

static e_void save0(SharedPointer<Node> parent, FileWriter& out, e_int depth);

e_void  Node::save(const String& out){
	FileWriter fw{out};
	save0(sharedFromThis(), fw, -1);
	fw.close();
}

e_bool Node::equals(const SharedPointer<Node>& other) const {
	return this == other.lock();
}

e_int  Node::compare(const SharedPointer<Node>& other) const {
	return this==(other.lock()) ? 0 : (this>other.lock()?1:-1);
}

e_int  Node::hashCode() const {
	return m_tag.hashCode();
}
String Node::toString() const {
	return System::move(String{m_tag});
}

static e_void save0(SharedPointer<Node> parent, FileWriter& out, e_int depth) {
	depth++;
	out.write(String{L"\t"} * depth);
	out.write(String{L"<"});
	out.write(parent->getTag());
	out.write(String{L" "});
	HashSet<String> attrNames = parent->getAttrMap().listAttrs();
	AutoPointer<Iterator<String>> iter{attrNames.iterator()};
	while (!iter->isEnd()) {
		String name = iter->current();
		out.write(name);
		out.write(String{L"=\""});
		out.write(parent->getAttrMap().getAttr(name));
		out.write(String{L"\" "});
		iter->advance();
	}
	
	if (parent->getNodeList().getLength()==0 && parent->getContent().empty()) {
		out.write(String{L"/>\n"});
		return;
	} else if (parent->getNodeList().getLength() > 0) {
		out.write(String{L">\n"});
		NodeList& nodeList = parent->getNodeList();
		for (e_int i=0; i<nodeList.getLength(); i++) {
			SharedPointer<Node> node = nodeList.getNodeAt(i);
			save0(node, out, depth);
		}
	} else if (!parent->getContent().empty()) {
		out.write(String{L">\n"});
		out.write(String{L"\t"} * (depth+1));
		out.write(parent->getContent());
		out.write(String{L"\n"});
	}
	
	out.write(String{L"\t"} * depth);
	out.write(String{L"</"});
	out.write(parent->getTag());
	out.write(String{L">\n"});
}

/*
// test used.
static e_char info[10][15] = {
	L"opentag", L"attrname", L"equ", L"attrvalue", L"big",
	L"closetag", L"selfclosesign", L"comment", L"content", L"end"
};

 //test used.	
String Token::toString() const {
	return System::move(String{info[m_type]} + String{L" : "} + *m_word);
}
//*/	

static e_void error(const String& fileName, const String& errInfo) {
	Console::errln(errInfo);
	throw LXMLParseException{__func__, __LINE__, fileName};
}

Token Lexer::_openTag(e_int index) {
	m_index += 2;
	
	while (!_isEnd() && _isOpenTagPart(m_index)) {
		m_index++;
	}
	
	m_startAttrName = true;
	return System::move(Token{System::move(m_text.substring(index+1, m_index)), Token::openTag});
}

Token Lexer::_closeTag(e_int index) {
	m_index += 3;
	while (!_isEnd() && _isCloseTagPart(m_index)) {
		m_index++;
	}
	return System::move(Token{System::move(m_text.substring(index+2, m_index)), Token::closeTag});
}

Token Lexer::_attrName(e_int index) {
	m_index++;
	while (!_isEnd() && _isAttrNamePart(m_index)) {
		m_index++;
	}
	return System::move(Token{System::move(m_text.substring(index, m_index)), Token::attrName});
}

Token Lexer::_attrValue(e_int index, e_char quot) {
	m_index++;
	while (!_isEnd() && m_text[m_index]!=quot) {
		if (m_text[m_index] == '\n') m_line++;
		m_index++;
	}
	
	if (_isEnd()) {
		error(m_fileName, String{L"错误: "} + m_fileName + String{L":"} + Integer{m_line} + String{L": 属性值缺失右引号."});
	}
	
	/*
	if (index+1 == m_index) {
		error(String{L"错误: "} + m_fileName + String{L":"} + Integer{m_line} + String{L": 属性值空."});
	}
	*/
	
	m_index++;
	if (index+1 == m_index-1) return System::move(Token{String{}, Token::attrValue});
	return System::move(Token{System::move(m_text.substring(index+1, m_index-1)), Token::attrValue});
}

Token Lexer::_content(e_int index) {
	StringBuffer sb;
	while (!_isEnd() && !_isContentEnd(m_index)) {			
		if (!_isEnd(0) && Character::isWhiteSpace(m_text[m_index])) {
			if (m_text[m_index] == '\n') {
				m_line++;
			}
			sb.append(Character{' '});
			m_index++;
		}
		while (!_isEnd() && !_isContentEnd(m_index) && 
								Character::isWhiteSpace(m_text[m_index])) {
			if (m_text[m_index]=='\n') m_line++;
			m_index++;
		}
		
		//********************************
		// Content 中允许注释.
		if (!_isEnd(3) && _isCommentStart(m_index, 
							m_index+1, m_index+2, m_index+3)) {
			_comment(m_index);
		}
		//********************************
		// 这里的 else 不能省.
		else if (!_isEnd() && !_isContentEnd(m_index)) {
			sb.append(Character{m_text[m_index]});
			m_index++;
		}
	}
	return System::move(Token{System::move(sb.toString().trim()), Token::content});
}

Token Lexer::_comment(e_int index) {
	m_index += 3;
	while (!_isEnd(2) && !_isCommentEnd(m_index, 
						m_index+1, m_index+2)) {
		if (m_text[m_index] == '\n') m_line++;
		m_index++;
	}
	
	if (_isEnd(2)) {
		error(m_fileName, String{L"错误: "} + m_fileName + String{L":"} + Integer{m_line} + String{L": 注释未关闭."});
	} else {
		m_index += 3;
	}
	
	return System::move(Token{String{}, Token::comment});
}

Token Lexer::nextToken() {
	_filterWhiteSpace();
	
	if (!_isEnd(1) && _isOpenTagStart(m_index, m_index+1)) {
		return _openTag(m_index);
	} else if (!_isEnd(2) && _isCloseTagStart(m_index, m_index+1, m_index+2)) {
		return _closeTag(m_index);
	} else if (!_isEnd() && _isAttrNameStart(m_index)) {
		return _attrName(m_index);
	} else if (!_isEnd(1) && _isSelfCloseSign(m_index, m_index+1)) {
		m_index+=2; m_startAttrName=false; 
		return System::move(Token{String{L"/>"}, Token::selfCloseSign});
	} else if (!_isEnd() && m_text[m_index]=='"') { //is attrValue
		return _attrValue(m_index, '"');
	} else if (!_isEnd() && m_text[m_index]=='\'') { //is attrValue
		return _attrValue(m_index, '\'');
	} else if (!_isEnd() && m_text[m_index]=='>') { // big
		m_index++; m_startAttrName = false;
		return System::move(Token{String{L">"}, Token::big});
	} else if (!_isEnd() && _isContentStart(m_index)) { // 对 content 的判断要放在 big 之后.
		return _content(m_index);
	} else if (!_isEnd() && m_text[m_index]=='=') { // equ
		m_index++;
		return System::move(Token{String{L"="}, Token::equ});
	} else if (!_isEnd(3) && _isCommentStart(m_index, m_index+1, m_index+2, m_index+3)) {
		return _comment(m_index);
	} else if (_isEnd()) {
		return System::move(Token{String{}, Token::end});
	} else {
		error(m_fileName, String{L"错误: "} + m_fileName + 
			String{L":"} + Integer{m_line} + String{L": 无效字符 "} + 
				Character{m_text[m_index]});
		return Token{}; //never arrive here
	}
}

e_void Lexer::_filterWhiteSpace() {
	while (!_isEnd()) {
		switch (m_text[m_index]) {
			case ' ': 
			case '\t':
				m_index++;
				continue;
			case '\n': 
				m_line++;
				m_index++;
				continue;
			default:
				return;
		}
	}
}

String Lexer::_getText(const String& fileName) {
	try {
		return System::move(IOUtility::read(fileName));
	} catch (Exception& e) {
		error(m_fileName, String{L"错误: 文件 "} + fileName + String{L" 读取失败!"});
		return String{}; //never arrive here.
	}
}

Parser::Parser(const String& fileName) {
	m_lex = Lexer(fileName);
}

Parser& Parser::parse() {
	static e_bool parsed = false;
	if (!parsed) {
		_move();
			
		if (_match(Token::end)) {
			m_root = SharedPointer<Node>{new Node{String{L"empty"}}};
			return *this;
		} else 	if (!_match(Token::openTag)) { 					
			error(m_lex.getFileName(), String{L"错误: "} + m_lex.getFileName() + String{L":"} +
				Integer{m_lex.getLine()} +	String{L": 缺失Open Tag."});
		}
		
		_node();

		if (!_match(Token::end)) {
			error(m_lex.getFileName(), String{L"错误: "} + m_lex.getFileName() + String{L":"} + Integer{m_lex.getLine()} +
						String{L": 根节点已关闭, 但之后仍有文本."});
		}
		parsed = true;
	}
	return *this;
}


//代码中使用 //*** 标记的代码行为语法制导转换策略部分. 其它代码为语法分析部分.
e_void Parser::_attr() {
	String name = m_look.getWord(); //***
	_move();
	
	if (!_match(Token::equ)) {
		error(m_lex.getFileName(), String{L"错误: "} + m_lex.getFileName() + String{L":"} + Integer{m_lex.getLine()} +
				String{L": 缺失 '=' ."});
	} else {
		_move();
	}
	
	String value = m_look.getWord(); //***
	
	if (!_match(Token::attrValue)) {
		error(m_lex.getFileName(), String{L"错误: "} + m_lex.getFileName() + String{L":"} + Integer{m_lex.getLine()} +
				String{L": 缺失属性值."});
	} else {
		_move();
	}
	
	m_top->getAttrMap().addAttr(name, value); //***
}

e_void Parser::_attrs() {
	while (true) {
		if (_match(Token::attrName)) {
			_attr();
		} else {
			break;
		}
	}
}

e_void Parser::_nodes() {
	while (true) {
		if (_match(Token::openTag)) {
			_node();
		} else {
			break;
		}
	}
}

e_void Parser::_checkTagPair(const String& tag) {		
	if (!m_topTag.equals(tag)) {
		error(m_lex.getFileName(), String{L"错误: "} + m_lex.getFileName() + String{L":"} + Integer{m_lex.getLine()} + 
				String{L": Open Tag:"} + m_topTag + 
					String{L" 与 Close Tag:"} + tag + String{L" 不匹配."});
	}
}


// 代码中使用 //*** 标记的代码行为语法制导转换策略部分. 其它代码为语法分析部分.

e_void Parser::_node() {
	String savedTag = System::move(m_topTag);  //***
	m_topTag = m_look.getWord(); //***
	
	SharedPointer<Node> saved = m_top; //***
	
	if (saved.isNullPtr()) { //***
		m_root = SharedPointer<Node>{new Node{m_topTag}}; //***
		m_top  = m_root; //***
	} else { //***
		m_top = SharedPointer<Node>{new Node{m_topTag}}; //*** 
		saved->getNodeList().addNode(m_top); //***
	}
	
	_move();
	_attrs();
	if (_match(Token::selfCloseSign)) {
		m_topTag = savedTag;
		m_top    = saved;
		_move();
	} else if (_match(Token::big)) {
		_move();
		if (_match(Token::content)) {
			m_top->setContent(m_look.getWord());
			_move();
		} else {
			_nodes();
		}
		if (!_match(Token::closeTag)) {
			error(m_lex.getFileName(), String{L"错误: "} + m_lex.getFileName() + String{L":"} +
				Integer{m_lex.getLine()} + String{L": Open Tag '"} + m_topTag 
					+ String{L"' 缺失 Close Tag."});
		} else {
			_checkTagPair(m_look.getWord()); //***
			m_topTag = savedTag; //***
			m_top    = saved; //***
			_move();
			if (!_match(Token::big)) {
				error(m_lex.getFileName(), String{L"错误: "} + m_lex.getFileName() + String{L":"} +
					Integer{m_lex.getLine()} + String{L": 缺失 '>'."});
			} else {
				_move();
			}
		}
	} else {
		error(m_lex.getFileName(), String{L"错误: "} + m_lex.getFileName() + String{L":"} +
				Integer{m_lex.getLine()} + String{L": 缺失 '属性名' 或 '>' 或 '/>'."});
	}								
}

} // LXML

} // Easy