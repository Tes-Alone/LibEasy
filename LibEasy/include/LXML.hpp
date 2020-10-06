/*
 * File Name:	LXML.hpp
 * Date/Time:
 * 	Start: 	2019年12月20日 18:19:41 周五
 * 	End:
 * Author:	Tes Alone
 * Version:	1.0
 * Note: FIXME
*/

#ifndef _EASY_LXML_H_
#define _EASY_LXML_H_

#include "BaseTypes.hpp"
#include "Exception.hpp"
#include "Character.hpp"
#include "String.hpp"
#include "Vector.hpp"
#include "HashMap.hpp"
#include "HashSet.hpp"
#include "AutoPointer.hpp"
#include "SharedPointer.hpp"

namespace Easy {

/**
	\namespace LXML
	
	\brief Easy 类库 'Little' XML 文档解析模块.
	<p>
	'Little' XML 是一个小型的 XML 文档解析模块, 虽说是 XML 文档, 但只是在语法上类似于 XML,
	而且只是 XML 规格的子集, LXML 只支持节点和属性这两个概念.
	</p>
	<p>
	LXML 文档由节点和节点属性组成, 节点可以包含子节点或内容, 但不能同时包含子节点和内容, 节点可以有
	一个或多个属性. 一个 LXML 文档只可以有一个根节点. 另外, LXML 支持注释. 注释可以出现在子节点的位置,
	也可以出现在节点内容中.
	</p>
	<p>
	LXML 文档只支持 UTF-8 编码. 节点的标签名可以是字母、数字、'-'、'_'、':' 的组合, 但不能以数字开头.
	属性名可以是字母、数字、'_' 的组合, 但不能以数字开头. LXML 文档目前不支持 &lt、&gt 等转义字符.
	这里的字母、数字需符合 Unicode 标准,但目前　Easy 的 Character 类还不能完整支持 Unicode 标准,
	所以这里的字母只是  ASCII 以及 CJK 字母, 数字只是 ASCII 数字 '0'-'9'.
	</p>
	<br/>
	例子如下:
	<pre>
		<code>
			<root attr0="value0" attr1="value1" attr2="">
				<demo>this is a string.</demo>
				<test name="test"/>
				<test>
					<demo>this is a string.</demo>
				</test>
			</root>
		</code>
	</pre>
	
*/
namespace LXML {

class Node;

/**
	\class NodeList
	
	\brief LXML 节点列表.
*/
class NodeList {

friend class Node;

private:
	/*
		这里的 Node 必须是指针, 这不仅仅是因为 NodeList 的
		构造函数与 Node 的构造函数会导致 无限递归, 还因为 Node 的
		等于函数 只能是比较对象地址, 所以不能使用值语义来存储节点.
	*/
	Vector<SharedPointer<Node>> m_nodes;
	String m_parent;

	/**
	 * \brief ctor.
	 * 
	 * do nothing.
	 * */
	inline NodeList() {}

	/**
		\brief 构造函数.
		
		客户端只能通过 Node::getNodeList() 来获取 NodeList 对象.
	*/
	NodeList(const String& parent);
public:		
	/**
		\brief 添加节点.
		
		@param node 
		@return 添加到接受者的节点.
	*/
	SharedPointer<Node> addNode(SharedPointer<Node>& node);
	
	/**
		\brief 根据 tag 添加节点.
		
		@param tag 新建节点的 tag.
		@return 添加到接受者的节点.
	*/
	SharedPointer<Node> addNodeByTag(const String& tag);
	
	/**
		\brief 获取指定位置的节点.
		
		@param index 指定位置.
		@return index 位置的节点.
		@throw 如果 index<0 或 index>=NodeList::getLength(), 抛出 IndexOutOfBoundsException.
	*/
	SharedPointer<Node> getNodeAt(e_int index) const;
	
	/**
		\brief 获取节点.
		
		@param node 引子.
		@return 接受者中与 node 等价的节点.
		@throw 如果 node 是空指针, 抛出 NullPointerException.
		@throw 如果接受者中不包含于 node 等价的节点, 抛出 HasNoSuchNodeException.
	*/
	SharedPointer<Node> getNode(const SharedPointer<Node>& node) const;
	
	/**
		\brief 移除指定位置的节点.
		
		@param index 指定位置.
		@return index 位置的节点.
	*/
	SharedPointer<Node>  removeNodeAt(e_int index);
	
	/**
		\brief 移除节点.
		
		@param node 引子.
		@return 接受者中与 node 等价的节点.
		@throw 如果 node 是空指针, 抛出 NullPointerException.
		@throw 如果接受者中不包含于 node 等价的节点, 抛出 HasNoSuchNodeException.
	*/
	SharedPointer<Node>  removeNode(const SharedPointer<Node>& node);
	
	/**
		\brief 根据属性获取节点.
		
		如果接受者中的有多个包含  name/value 的节点, 只返回第一个.
		
		@param name 属性名.
		@param value 属性值.
		@return 接受者中包含 name/value 属性对的节点.
		@throw 如果接受者中不包含有 name/value 的节点, 抛出 HasNoSuchNodeException.
	*/
	SharedPointer<Node> getNodeByAttr(const String& name, const String& value) const;
	
	/**
		\brief 根据 tag 获取节点.
		
		如果接受者中的有多个 tag 为 tag 的节点, 只返回第一个.
		
		@param tag 
		@return 接受者中 tag 等于 tag 的节点.
		@throw 如果接受者中不包含 tag 等于 tag 的节点, 抛出 HasNoSuchNodeException.
	*/
	SharedPointer<Node> getNodeByTag(const String& tag) const;
	
	/**
		\brief 获取节点个数.
		
		@return 接受者的节点个数.
	*/
	e_int getLength() const;
}; // NodeList

/**
	\class AttrMap
	
	\brief LXML 节点属性.
*/
class AttrMap {
friend class Node;

private:
	HashMap<String, String> m_map;
	String m_node;
	
	/**
	 * \brief ctor.
	 * 
	 * do nothing.
	 * */
	inline AttrMap() {}
	
	/**
		\brief 构造函数.
		客户端只能通过 Node::getAttrMap() 来获取 AttrMap 对象.
	*/
	AttrMap(const String& node);
public:	
	/**
		\brief 添加属性.
		
		@param name 属性名.
		@param value 属性值.
		@throw 如果 name 为空串, 抛出 IllegalArgumentException.
	*/
	e_void  addAttr(const String& name, const String& value);
	
	/**
		\brief 移除属性.
		
		@param name 要移除的属性名.
		@throw 如果 name 为空串, 抛出 IllegalArgumentException.
		@throw 如果接受者不包含属性 name, 抛出 HasNoSuchAttrException.
	*/
	e_void  removeAttr(const String& name);
	
	/**
		\brief 获取属性值.
		
		@param name 属性名.
		@return name 的值.
		@throw 如果 name 为空串, 抛出 IllegalArgumentException.
		@throw 如果接受者不包含属性 name, 抛出 HasNoSuchAttrException.
	*/
	String& getAttr(const String& name) const;
	
	/**
		\brief 获取属性对个数.
		
		@return 属性对个数.
	*/
	e_int   getLength()const;
	
	/**
		\brief 获取属性名列表.
		
		@return 属性名列表.
	*/
	HashSet<String> listAttrs();
}; // AttrMap

/**
	\class Node
	
	\brief LXML 节点.
*/
class Node : public EnableSharedFromThis<Node> {

private:
	NodeList  m_nodes;
	AttrMap   m_attrs;
	String    m_tag;
	String    m_content;

public:
	/**
		\brief 构造函数.
		
		默认标签名为 "default".
	*/
	Node();
	
	/**
		\brief 构造函数.
		
		@param tag 新建节点 tag.
	*/
	Node(const String& tag);
	
	
	/**
		\brief 设置节点 tag.
		
		@param tag 接受者的新 tag.
		@throw 如果 tag 为空串, 抛出 IllegalArgumentException.
	*/
	e_void setTag(const String& tag);
	
	/**
		\brief 设置内容.
		
		content 可以为空.
		
		@param  content 接受者的新内容.
	*/
	e_void  setContent(const String& content);
	
	/**
		\brief 获取子节点列表.
		
		@return 接受者的子节点列表.
	*/
	NodeList& getNodeList() ; // can not be const
	
	/**
		\brief 获取节点属性.
		
		@return 接受者的属性 map.
	*/
	AttrMap&  getAttrMap() ; // can not be const
	
	/**
		\brief 获取节点 tag.
		
		<b>注意: 不要 move 返回值.</b>
		
		@return 接受者的 tag.
	*/
	const String& getTag() const;
	
	/**
		\brief 获取节点内容.
		
		<b>注意: 不要 move 返回值.</b>
		
		@return 接受者的内容.
	*/
	const String& getContent() const;
	
	/**
		\brief 保存节点所有信息到文件.
		
		@param  out 保存的文件路径.
	*/
	e_void save(const String& out);
	
	/**
		\brief 等于函数.
			
		方法比较的是节点的地址.
			
		@param other 接受者的比较对象.
		@return 如果接受者的值等于 other 的值, 返回真; 否则返回假.
	*/
	e_bool equals(const SharedPointer<Node>& other) const;
	
	/**
		\brief 比较函数.
		
		方法比较的是节点的地址.
		
		@param other
		@return 如果接受者的值大于 item 的值, 返回 1; 等于返回 0; 小于返回 -1.
	*/
	e_int compare(const SharedPointer<Node>& other) const;
	
	/**
	 * \brief 哈希函数.
	 * 
	 * @return 接受者的哈希值.
	*/
	e_int hashCode() const;
	
	/**
	 * \brief 转到字符串．
	 * 
	 * @returns 接受者的字符串表示.
	*/
	String toString() const;
}; // Node

/**
	\class Token
	
	\brief LXML Token 类.
*/
class Token {
public:
	/**
		\brief Open Tag 类型.
	*/
	static const e_int openTag    	 = 0;
	
	/**
		\brief  属性名类型.
	*/
	static const e_int attrName 	 = 1;
	
	/**
		\brief '=' 类型.
	*/
	static const e_int equ 			 = 2;
	
	/**
		\brief  属性值类型.
	*/
	static const e_int attrValue 	 = 3;
	
	/**
		\brief  '>' 类型.
	*/
	static const e_int big 			 = 4;
	
	/**
		\brief  Close Tag 类型.
	*/
	static const e_int closeTag 	 = 5;
	
	/**
		\brief '/>' 类型.
	*/
	static const e_int selfCloseSign = 6;
	
	/**
		\brief  注释类型.
	*/
	static const e_int comment 		 = 7;
	
	/**
		\brief  节点内容类型.
	*/
	static const e_int content 		 = 8;
	
	/**
		\brief  文档结束类型.
	*/
	static const e_int end 			 = 9;
	
	/**
		\brief 构造函数.
	*/
	Token() {
		m_type = -1;
	}
	
	/**
		\brief 构造方法.
		
		@param word 词位.
		@param type 类型.
		@throw 如果 type<0 或 type>Token::end, 抛出 IllegalArgumentException.
	*/
	Token(String&& word, e_int type) {
		if (type<0 || type>Token::end)
			throw IllegalArgumentException{__func__, __LINE__};	
		m_word = System::move(word);
		m_type = type;
	}
	
	/**
		\brief 获取词位.
		
		不要 move 返回值.
		
		@return 接受者的词位.
	*/
	inline const String& getWord() const {
		return m_word;
	}
	
	/**
		\brief 获取类型.
		
		@return 接受者的类型.
	*/
	inline e_int getType() const {
		return m_type;
	}
	
	//String toString() const;
private:
	String  m_word;
	e_int   m_type;
}; // Token


/**
	\class Lexer
	
	\brief LXML 文档词法解析器.
*/
class Lexer {
private:
	String  m_fileName;
	String  m_text;
	e_int   m_index;
	e_int   m_line;
	e_bool  m_startAttrName;
public:
	
	/**
	 * \brief ctor.
	 * 
	 * do nothing.
	 * */
	inline Lexer() {}
	
	/**
		\brief 构造函数.
		
		@param fileName 文档路径.
	*/
	Lexer(const String& fileName) {
		m_fileName = fileName;
		m_index    = 0;
		m_line     = 1;
		m_text     = System::move(_getText(m_fileName));
		m_startAttrName = false;
	}
	
	/**
		\brief 获取 Token.
		
		@return 下一个 Token.
	*/
	Token nextToken();
	
	/**
		\brief 获取当前行数.
		
		@return 当前行数.
	*/
	inline e_int getLine() const {return m_line;}
	
	/**
		\brief 获取文档名.
		
		@return 文档名.
	*/
	inline String getFileName() const {return System::move(String{m_fileName});}
private:
	String _getText(const String& fileName);
	e_void _filterWhiteSpace();
	
	inline e_bool _isOpenTagStart(e_int index1, e_int index2) {
		return m_text[index1]=='<' 
					&& (Character::isLetter(m_text[index2])
						|| m_text[index2]=='_' || m_text[index2]=='-'
							|| m_text[index2]==':');
	}
	
	inline e_bool _isOpenTagPart(e_int index) {
		return m_text[index]=='-' || m_text[index]=='_' 
					|| m_text[index]==':' 
						||	Character::isLetterOrDigit(m_text[index]);
	}
	
	inline e_bool _isCloseTagStart(e_int index1, e_int index2, e_int index3) {
		return m_text[index1]=='<' && m_text[index2]=='/'
					&& (Character::isLetter(m_text[index3])
						|| m_text[index3]=='_');
	}
	
	inline e_bool _isCloseTagPart(e_int index) {
		return _isOpenTagPart(index);
	}
	
	inline e_bool _isContentStart(e_int index) {
		return !m_startAttrName && m_text[index]!='<';
	}
	
	inline e_bool _isContentEnd(e_int index) {
		return m_text[index] == '<';
	}
	
	inline e_bool _isAttrNameStart(e_int index) {
		return m_startAttrName && 
					(Character::isLetter(m_text[index]) 
						|| m_text[index]=='_');
	}
	
	inline e_bool _isAttrNamePart(e_int index) {
		return m_text[index]=='_' ||
				Character::isLetterOrDigit(m_text[index]);
	}
	
	inline e_bool _isSelfCloseSign(e_int index1, e_int index2) {
		return m_text[index1]=='/' && m_text[index2]=='>';
	}
	
	inline e_bool _isCommentStart(e_int index1, e_int index2, e_int index3, e_int index4) {
		return m_text[index1]=='<' && m_text[index2]=='!' && m_text[index3]=='-' && m_text[index4]=='-';
	}
	
	inline e_bool _isCommentEnd(e_int index1, e_int index2, e_int index3) {
		return m_text[index1]=='-' && m_text[index2]=='-' && m_text[index3]=='>';
	}
	
	Token _openTag(e_int index);
	Token _closeTag(e_int index);
	Token _attrName(e_int index);
	Token _attrValue(e_int index, e_char quot);
	Token _content(e_int index);
	Token _comment(e_int index);
	
	inline e_bool _isEnd(e_int need=0) {
		return need+m_index >= m_text.length();
	}
}; // Lexer

/*
 * 文法: LXML ->　node
 * 		node -> OpenTag attrList Big nodes CloseTag
 * 				| OpenTag attrList SelfCloseSign
 * 				| OpenTag attrList Big Content CloseTag
 * 		nodes -> node nodes | empty
 * 		attrList -> attr attrList | empty
 * 		attr -> AttrName Equ AttrValue 
 * */
/**
	\class Parser
	
	\brief  LXML 文档语法解析器.
	
	文法定义如下:
	<pre>
		<code>
			LXML ->　node
	  		node -> OpenTag attrList Big nodes CloseTag
	  				| OpenTag attrList SelfCloseSign
	  				| OpenTag attrList Big Content CloseTag
	  		nodes -> node nodes | empty
	  		attrList -> attr attrList | empty
	  		attr -> AttrName Equ AttrValue 
		</code>
	</pre>
	首字母大写的符号为终端符.
*/
class Parser {

private:
	Token   m_look;
	Lexer   m_lex;
	String  m_topTag;
	SharedPointer<Node>  m_top;
	SharedPointer<Node>  m_root;
public:
	/**
	 * \brief ctor.
	 * 
	 * do nothing.
	 * */
	inline Parser() {}
	
	/**
		\brief 构造函数.
		
		@param fileName 文件路径.
	*/
	Parser(const String& fileName);
	
	/**
		\brief 解析文档.
		
		@return 接受者.
	*/
	Parser& parse();
	
	/**
		\brief 获取根节点.
		
		@return 文档根节点.
	*/
	SharedPointer<Node> getRoot() const {
		return m_root;
	}
	
	/**
		\brief 获取文档名.
		
		@return 文档名.
	*/
	inline String getFileName() const {return System::move(m_lex.getFileName());}
private:
	inline e_bool _match(e_int type) {
		return m_look.getType() == type;
	}
	
	inline e_void _move() {
		m_look = m_lex.nextToken();
		while (m_look.getType() == Token::comment)
			m_look = m_lex.nextToken();
	}
	
	e_void _node();
	e_void _nodes();
	e_void _attrs();
	e_void _attr();
	e_void _checkTagPair(const String& tag);
	
}; // Parser

/**
	\class LXMLDocument
	
	\brief LXML 文档表示.
*/

class LXMLDocument : public EnableSharedFromThis<LXMLDocument> {
private:
	SharedPointer<Node> m_root;
	Parser m_parser;
public:
	/**
	 * \brief ctor.
	 * 
	 * do nothing.
	 * */
	 inline LXMLDocument() {}
	
	/**
		\brief 构造函数.
		
		@param fileName 指定的 XML 文件.
	*/
	LXMLDocument(const String& fileName) {
		m_parser = Parser{fileName};
	}
	
	/**
		\brief 解析 XML 文档.
		
		@return 接受者.
	*/
	LXMLDocument& parse() {
		m_root = m_parser.parse().getRoot();
		return *this;
	}
		
	/**
		\brief 获取文档根节点.
		
		@return 接受者的根节点.
	*/
	SharedPointer<Node> getRoot() {
		return m_root;
	}
	
	/**
		\brief 文档另存为.
		
		@param out 输出文件名.
	*/
	e_void save(const String& out) {
		m_root->save(out);
	}
	
	inline String toString() const override {
		return String{L"LXMLDocument:'"} + m_parser.getFileName() + String{L"';Root:"} + m_root;
	}
}; // LXMLDocument

} // LXML

} // Easy

#endif // _EASY_LXML_H_

