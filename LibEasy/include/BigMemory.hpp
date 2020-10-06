/*
 * 文件名: BigMemory.hpp 
 *
 * 日期/时间:
 *		开始: 2020年09月02日 16:34:20 周三
 *		结束:
 *
 * 版本: 1.0
 * 
 * 作者: Tes Alone
 *
 * 备注:
*/

#ifndef _EASY_BIGMEMORY_H_
#define _EASY_BIGMEMORY_H_

namespace Easy {

class DataNode {

friend class BigMemory;

	e_bool  _isPacked;
	e_long _start;
	e_long _end;
	UByteArray _data;

public:
	DataNode() {}
	
	DataNode(e_long s, e_long e) {
		_isPacked = true;
		_start    = s;
		_end	  = e;
	}
	
	DataNode(e_long s, e_long e, const UByteArray& d) {
		if ((e_int)(e-s) != d.length()) {
			throw IllegalArgumentException{__func__, __LINE__};
		}
		_isPacked = false;
		_start    = s;
		_end	  = e;
		_data     = d;
	}
	
	DataNode(e_long s, e_long e, UByteArray&& d) {
		if ((e_int)(e-s) != d.length()) {
			throw IllegalArgumentException{__func__, __LINE__};
		}
		_isPacked = false;
		_start    = s;
		_end	  = e;
		_data     = System::move(d);
	}
	
	e_bool isPacked() const {
		return _isPacked;
	}
	
	e_long getStart() const {
		return _start;
	}
	
	e_long getEnd() const {
		return _end;
	}
	
	const UByteArray& getData() const {
		return _data;
	}
	
	e_bool equals(const DataNode& other) const {
		return _start == other._start;
	}
	
	e_int compare(const DataNode& other) const {
		return Long::compare(_start, other._start);
	}
	
	e_int hashCode() const{
		return Math::abs((e_int)
						(Bool::hashCode(_isPacked) 
						+ Long::hashCode(_start)
						+ Long::hashCode(_end)
						+ _data.hashCode())
				);
	}
	
	String toString() const {
		return Long::toString(_start) + L":" + Long{_end};
	}

private:
	e_void replace(const UByteArray& d, e_int start, e_int end) {
		_data.replace(d, start, end);
	}
}; // DataNode

/**
 * \class BigMemory
 * 
 * \brief A packed memory block.
 * */
class BigMemory {

private:
	TreeSet<DataNode> _memory;
	e_long _maxSize;
	
public:
	/**
	 * \brief Cotr.
	 * 
	 * @param s memory size, if s <= 0, s = 0;
	 * */
	BigMemory(e_long s=0) {
		if (s <= 0) {
			_maxSize = 0;
		} else {
			_maxSize = s;
			_memory.add(DataNode{0, s});
		}
	}
	
	/**
	 * \brief Copy ctor.
	 * 
	 * @param copy .
	 * */
	BigMemory(const BigMemory& copy) {
		_memory  = copy._memory;
		_maxSize = copy._maxSize;
	}
	
	/**
	 * \brief Copy assignment.
	 * 
	 * @param copy .
	 * */
	BigMemory& operator=(const BigMemory& copy) {
		_memory  = copy._memory;
		_maxSize = copy._maxSize;
		return *this;
	}
	
	/**
	 * \brief Move ctor.
	 * 
	 * @param move .
	 * */
	BigMemory(BigMemory&& move) {
		_memory  = System::move(move._memory);
		_maxSize = move._maxSize;
		move._maxSize = 0;
	}
	
	/**
	 * \brief Move assignment.
	 * 
	 * @param move .
	 * */	
	BigMemory& operator=(BigMemory&& move) {
		_memory  = System::move(move._memory);
		_maxSize = move._maxSize;
		move._maxSize = 0;
		return *this;
	}
	
	/**
	 * \brief Check equality.
	 * 
	 * The method compare receiver and other, if their status equals,
	 * return true, otherwise retunr false.
	 * 
	 * @param other .
	 * @return  compared result.
	 * */
	e_bool equals(const BigMemory& other) const {
		return _memory.equals(other._memory) 
					&&	_maxSize == other._maxSize;
	}
	
	/**
	 * \brief Check order.
	 * 
	 * Method just compare receiver's max size and other's max size.
	 * If receiver's size is bigger return 1, both equals return 0, otherwise return -1;
	 * 
	 * @param other .
	 * @return compared result.
	 * */
	e_int compare(const BigMemory& other) const {
		return ULong::compare(_maxSize, other._maxSize);
	}
	
	/**
	 * \brief Get hash code.
	 * 
	 * Get receiver`s hash code.
	 * 
	 * @return receiver`s hash code.
	 * */
	e_int hashCode() const {
		return Math::abs((e_int) (_memory.hashCode() + _maxSize));
	}
	
	/**
	 * \brief To string represent．
	 * 
	 * @returns receiver`s string represent.
	*/
	String toString() const {
		String fmt{L"%02s"};
		StringBuffer sb;
		sb.append(String{L"["});
		AutoPointer<Iterator<DataNode>> iter{_memory.iterator()};
		String zero = String::format(fmt, Integer::toHexString(0));
		while (!iter->isEnd()) {
			DataNode& node = iter->current();
			if (node._isPacked) {
				for (e_int i=0; i<(e_int)(node._end-node._start); i++) {
					sb.append(zero + L", ");
				}
			} else {
				for (e_int i=0; i<node._data.length(); i++) {
					sb.append(String::format(fmt, Integer::toHexString(node._data[i])) + L", ");
				}
			}
			iter->advance();
		}
		if (sb.length() > 1) {
			sb.removeTail(sb.length()-2);
		}
		sb.append(String{L"]"});
		return System::move(sb.toString());
	}
	
	/**
	 * \brief Read a subblock.
	 * 
	 * @param start block start index, from 0 ~ maxSize-1.
	 * @param end   block end index, from 0 ~ maxSize-1.
	 * @return a subblock.
	 * @throw if start<0 || end<0 || start<end || end>maxSize, throw IllegalArgumentException.
	 * */
	UByteArray read(e_long start, e_long end) const {
		if (start<0 || end<0 || start>end || end>_maxSize) {
			throw IllegalArgumentException{__func__, __LINE__};
		}
		if (start < end) {
			Vector<DataNode> crossedNodes = _getNodesFromStartToEnd(start, end);
//			Console::outln(_memory);
//			Console::outln(crossedNodes);
			if (crossedNodes.size() == 1) {
				if (crossedNodes[0]._isPacked) {
					return System::move(UByteArray{(e_int)(end-start)});
				} else {
					e_int readStart = start - crossedNodes[0]._start;
					e_int readEnd   = end - crossedNodes[0]._start;
					return System::move(crossedNodes[0]._data.subArray(readStart, readEnd));
				}
			} else {
				UByteBuffer buf;
				DataNode& startNode = crossedNodes[0];
				if (startNode._isPacked) {
					buf.append(UByteArray{(e_int)(startNode._end-start)});
				} else {
					e_int subStart = start - startNode._start;
					e_int subEnd   = startNode._end - startNode._start;
					buf.append(startNode._data.subArray(subStart, subEnd));
				}
				for (e_int i=1; i<crossedNodes.size()-1; i++) {
					DataNode& node = crossedNodes[i];
					if (node._isPacked) {
						buf.append(UByteArray{(e_int)(node._end-node._start)});
					} else {
						buf.append(node._data);
					}
				}
				DataNode& endNode 	= crossedNodes[crossedNodes.size()-1];
				if (endNode._isPacked) {
					buf.append(UByteArray{(e_int)(end-endNode._start)});
				} else {
					e_int subEnd = end - endNode._start;
					buf.append(endNode._data.subArray(0, subEnd));
				}
				
				return System::move(buf.subArray());
			}
		}
		return UByteArray{};
	}
	
	/**
	 * \brief Write data to a subblock.
	 * 
	 * @param start block start index, from 0 ~ maxSize-1.
	 * @param end   block end index, from 0 ~ maxSize-1.
	 * @throw if start<0 || end<0 || start<end || end>maxSize, throw IllegalArgumentException.
	 * */
	e_void write(UByteArray&& data, e_long start, e_long end) {
		if (start<0 || end<0 || start>end || end>_maxSize) {
			throw IllegalArgumentException{__func__, __LINE__};
		}
		if (start < end) {
			if (data.length() != (e_int)(end-start)) {
				throw IllegalArgumentException{__func__, __LINE__};
			}
			Vector<DataNode> crossedNodes = _getNodesFromStartToEnd(start, end);
			DataNode startNode = crossedNodes[0];
			DataNode endNode   = crossedNodes[crossedNodes.size()-1];
			if (startNode.equals(endNode)) { // in a node's internal
				if (startNode._isPacked) {
					if (start==startNode._start && end!=startNode._end) {
						_twoPartSplitForPackedAndEqualsStart(start, end, startNode, data);
//						Console::outln("#1.1:");
//						Console::outln(_memory);
					} else if (start!=startNode._start && end==startNode._end) {
						_twoPartSplitForPackedAndEqualsEnd(start, end, startNode, data);
//						Console::outln("#1.2:");
//						Console::outln(_memory);
					} else if (start==startNode._start && end==startNode._end) {
						_memory.remove(startNode);
						_memory.add(DataNode{start, end, data});
//						Console::outln("#1.3:");
//						Console::outln(_memory);
					} else {
						_memory.remove(startNode);
						_memory.add(DataNode{startNode._start, start});
						_memory.add(DataNode{end, startNode._end});
						_memory.add(DataNode(start, end, data));
						
//						Console::outln("#1.4:");
//						Console::outln(_memory);
					}					
				} else {
					e_int replaceStart = start - startNode._start;
					e_int replaceEnd   = replaceStart + data.length();
					startNode.replace(data, replaceStart, replaceEnd);
					_memory.remove(startNode);
					_memory.add(startNode);
					
//					Console::outln("#2:");
//					Console::outln(_memory);
				}
			} else { // cross more than one node
				_removeCrossedNodes(crossedNodes);
				if (start==startNode._start && end!=endNode._end) {
					_twoPartSplitForCrossAndEqualsStart(start, end, endNode, data);
//					Console::outln("#3:");
//					Console::outln(_memory);
				} else if (start!=startNode._start && end==endNode._end) {
					_twoPartSplitForCrossAndEqualsEnd(start, end, startNode, data);
//					Console::outln("#4:");
//					Console::outln(_memory);
				} else if (start==startNode._start && end==endNode._end) {
					_memory.add(DataNode{start, end, data});
//					Console::outln("#5:");
//					Console::outln(_memory);
				} else {
//					Console::outln("#6:");
//					Console::outln(crossedNodes);
					_threePartSplitForCross(start, end, startNode, endNode, data);
//					Console::outln("#6:");
//					Console::outln(_memory);
				}
			}
		}
	}
	
	/**
	 * \brief Write data to a subblock.
	 * 
	 * @param start block start index, from 0 ~ maxSize-1.
	 * @param end   block end index, from 0 ~ maxSize-1.
	 * @throw if start<0 || end<0 || start<end || end>maxSize, throw IllegalArgumentException.
	 * */
	e_void write(const UByteArray& data, e_long start, e_long end) {
		if (start>end || end>_maxSize) {
			throw IllegalArgumentException{__func__, __LINE__};
		}
		if (start < end) {
			if (data.length() != (e_int)(end-start)) {
				throw IllegalArgumentException{__func__, __LINE__};
			}
			Vector<DataNode> crossedNodes = _getNodesFromStartToEnd(start, end);
			DataNode startNode = crossedNodes[0];
			DataNode endNode   = crossedNodes[crossedNodes.size()-1];
			if (startNode.equals(endNode)) { // in a node's internal
				if (startNode._isPacked) {
					if (start==startNode._start && end!=startNode._end) {
						_twoPartSplitForPackedAndEqualsStart(start, end, startNode, data);
//						Console::outln("#1.1:");
//						Console::outln(_memory);
					} else if (start!=startNode._start && end==startNode._end) {
						_twoPartSplitForPackedAndEqualsEnd(start, end, startNode, data);
//						Console::outln("#1.2:");
//						Console::outln(_memory);
					} else if (start==startNode._start && end==startNode._end) {
						_memory.remove(startNode);
						_memory.add(DataNode{start, end, data});
//						Console::outln("#1.3:");
//						Console::outln(_memory);
					} else {
						_memory.remove(startNode);
						_memory.add(DataNode{startNode._start, start});
						_memory.add(DataNode{end, startNode._end});
						_memory.add(DataNode(start, end, data));
						
//						Console::outln("#1.4:");
//						Console::outln(_memory);
					}					
				} else {
					e_int replaceStart = start - startNode._start;
					e_int replaceEnd   = replaceStart + data.length();
					startNode.replace(data, replaceStart, replaceEnd);
					_memory.remove(startNode);
					_memory.add(startNode);
					
//					Console::outln("#2:");
//					Console::outln(_memory);
				}
			} else { // cross more than one node
				_removeCrossedNodes(crossedNodes);
				if (start==startNode._start && end!=endNode._end) {
					_twoPartSplitForCrossAndEqualsStart(start, end, endNode, data);
//					Console::outln("#3:");
//					Console::outln(_memory);
				} else if (start!=startNode._start && end==endNode._end) {
					_twoPartSplitForCrossAndEqualsEnd(start, end, startNode, data);
//					Console::outln("#4:");
//					Console::outln(_memory);
				} else if (start==startNode._start && end==endNode._end) {
					_memory.add(DataNode{start, end, data});
//					Console::outln("#5:");
//					Console::outln(_memory);
				} else {
//					Console::outln("#6:");
//					Console::outln(crossedNodes);
					_threePartSplitForCross(start, end, startNode, endNode, data);
//					Console::outln("#6:");
//					Console::outln(_memory);
				}
			}
		}
	}
	
	/**
	 * \brief Get max size.
	 * 
	 * Get max size of receiver.
	 * */
	e_long getMaxSize() const {
		return _maxSize;
	}
	
	/**
	 * \brief Get internal data.
	 * 
	 * Get internal data of receiver.
	 * */
	const TreeSet<DataNode>& getDatas() const {
		return _memory;
	}
	
private:
	e_void _twoPartSplitForPackedAndEqualsStart(e_long start, e_long end, 
												DataNode& node, const UByteArray& data) {
		_memory.add(DataNode{end, node._end});
		node._end      = end;
		node._isPacked = false;
		node._data     = data;
		_memory.remove(node);
		_memory.add(node);
	}
	
	e_void _twoPartSplitForPackedAndEqualsEnd(e_long start, e_long end, 
												DataNode& node, const UByteArray& data) {
		_memory.remove(node);
		_memory.add(DataNode{node._start, start});
		node._start    = start;
		node._data     = data;
		node._isPacked = false;
		_memory.add(node);
	}
	
	e_void _twoPartSplitForCrossAndEqualsStart(e_long start, e_long end, 
												DataNode& endNode, const UByteArray& data) {
		_memory.add(DataNode{start, end, data});
		if (endNode._isPacked) {
			_memory.add(DataNode{end, endNode._end});
		} else {
			e_int subStart = end - endNode._start;
			e_int subEnd   = endNode._end - endNode._start;
			_memory.add(DataNode{end, endNode._end, endNode._data.subArray(subStart, subEnd)});
		}
	}
	
	e_void _twoPartSplitForCrossAndEqualsEnd(e_long start, e_long end, 
												DataNode& startNode, const UByteArray& data) {
		if (startNode._isPacked) {
			_memory.add(DataNode{startNode._start, start});
		} else {
			_memory.add(DataNode{startNode._start, start, startNode._data.subArray(0, start-startNode._start)});
		}
		_memory.add(DataNode{start, end, data});										
	}
	
	e_void _threePartSplitForCross(e_long start, e_long end, 
									DataNode& startNode, DataNode& endNode, const UByteArray& data) {
		if (startNode._isPacked) {
			_memory.add(DataNode{startNode._start, start});
		} else {
			_memory.add(DataNode{startNode._start, start, startNode._data.subArray(0, start-startNode._start)});
		}
		if (endNode._isPacked) {
			_memory.add(DataNode{end, endNode._end});
		} else {
			e_int subStart = end - endNode._start;
			e_int subEnd   = endNode._end - endNode._start;
//			Console::outln("end");
//			Console::outln(end);
//			Console::outln(endNode);
			_memory.add(DataNode{end, endNode._end, endNode._data.subArray(subStart, subEnd)});
		}
		_memory.add(DataNode{start, end, data});
	}
	
	e_void _twoPartSplitForPackedAndEqualsStart(e_long start, e_long end, 
												DataNode& node, UByteArray&& data) {
		_memory.add(DataNode{end, node._end});
		node._end      = end;
		node._isPacked = false;
		node._data     = data;
		_memory.remove(node);
		_memory.add(node);
	}
	
	e_void _twoPartSplitForPackedAndEqualsEnd(e_long start, e_long end, 
												DataNode& node, UByteArray&& data) {
		_memory.remove(node);
		_memory.add(DataNode{node._start, start});
		node._start    = start;
		node._data     = data;
		node._isPacked = false;
		_memory.add(node);
	}
	
	e_void _twoPartSplitForCrossAndEqualsStart(e_long start, e_long end, 
												DataNode& endNode, UByteArray&& data) {
		_memory.add(DataNode{start, end, data});
		if (endNode._isPacked) {
			_memory.add(DataNode{end, endNode._end});
		} else {
			e_int subStart = end - endNode._start;
			e_int subEnd   = endNode._end - endNode._start;
			_memory.add(DataNode{end, endNode._end, endNode._data.subArray(subStart, subEnd)});
		}
	}
	
	e_void _twoPartSplitForCrossAndEqualsEnd(e_long start, e_long end, 
												DataNode& startNode, UByteArray&& data) {
		if (startNode._isPacked) {
			_memory.add(DataNode{startNode._start, start});
		} else {
			_memory.add(DataNode{startNode._start, start, startNode._data.subArray(0, start-startNode._start)});
		}
		_memory.add(DataNode{start, end, data});										
	}
	
	e_void _threePartSplitForCross(e_long start, e_long end, 
									DataNode& startNode, DataNode& endNode, UByteArray&& data) {
		if (startNode._isPacked) {
			_memory.add(DataNode{startNode._start, start});
		} else {
			_memory.add(DataNode{startNode._start, start, startNode._data.subArray(0, start-startNode._start)});
		}
		if (endNode._isPacked) {
			_memory.add(DataNode{end, endNode._end});
		} else {
			e_int subStart = end - endNode._start;
			e_int subEnd   = endNode._end - endNode._start;
//			Console::outln("end");
//			Console::outln(end);
//			Console::outln(endNode);
			_memory.add(DataNode{end, endNode._end, endNode._data.subArray(subStart, subEnd)});
		}
		_memory.add(DataNode{start, end, data});
	}
	
	e_void _removeCrossedNodes(const Vector<DataNode>& nodes) {
		for (e_int i=0; i<nodes.size(); i++) {
			_memory.remove(nodes[i]);
		}
	}
	
	Vector<DataNode> _getNodesFromStartToEnd(e_long start, e_long end) const {
		Vector<DataNode> crossedNodes;
		AutoPointer<Iterator<DataNode>> iter{_memory.iterator()};
		DataNode prev;
		while (!iter->isEnd()) {
			DataNode& node = iter->current();
			if (node._start <= start) {
				prev = node;
			} else if (node._start > start) {
				break;
			}
			iter->advance();
		}
		crossedNodes.add(prev);
		if (prev._end >= end) {
			// do nothing.
		} else {
			while (!iter->isEnd()) {
				DataNode& node = iter->current();
				if (node._end >= end) {
					prev = node;
					crossedNodes.add(prev);
					break;
				}
				crossedNodes.add(node);
				iter->advance();
			}
		}
		return System::move(crossedNodes);
	}
};

} // namespace Easy


#endif // _EASY_BIGMEMORY_H_
