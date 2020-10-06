
/*
 * File name: Math.hpp 
 *
 * Date/Time:
 *		start: 2019年10月03日 18时15分14秒 周4
 *		end:
 *
 * Version: 1.0
 * 
 * Author: Tes Alone
 *
 * Note:
*/

#ifndef LEC_MATH_H_
#define LEC_MATH_H_

#include <cinttypes>

#include "BaseTypes.hpp"
#include "Exception.hpp"

namespace Easy {

/**
	\class Math

	\brief 常用数学函数实现.
	
	@since 1.0
*/
class Math {
	public:		
		/**
			\brief 取较小值.
			
			@param n1 有符号 32 位整数值.
			@param n2 有符号 32 位整数值.
			@return n1>n2 ? n2 : n1.
		*/
		static e_int min(e_int n1, e_int n2);

		/**
			\brief 取较大值.
			
			@param n1 有符号 32 位整数值.
			@param n2 有符号 32 位整数值.
			@return n1<n2 ? n2 : n1.
		*/
		
		static e_int max(e_int n1, e_int n2);
		
		/**
			\brief 取绝对值.
			
			@param num 有符号 32 位整数值.
			@return num 的绝对值.
			@throw 如果 num 等于 Integer::MIN , 抛出 HasNoABSException.
		*/
		static e_int abs(e_int num);
		
		/**
			\brief 取绝对值.
			
			@param num 有符号 64 位整数值.
			@return num 的绝对值.
			@throw 如果 num 等于 Long::MIN , 抛出 HasNoABSException.
		*/
		static e_long abs(e_long num);

		/**
			\brief 取绝对值.
			
			@param f 单精度浮点值.
			@return f 的绝对值.
		*/	
		static e_float abs(e_float f);
		
		/**
			\brief 取绝对值.
			
			@param d 双精度浮点值.
			@return d 的绝对值.
		*/
		static e_double abs(e_double d);

		/**
		 * \brief 幂指函数.
		 * 
		 * @param base 基数.
		 * @param exp 指数.
		 * @return base 的 exp 次幂.
		*/
		static e_float pow(e_float base, e_float exp);
		
		/**
		 * \brief 幂指函数.
		 * 
		 * @param base 基数.
		 * @param exp 指数.
		 * @return base 的 exp 次幂.
		*/
		static e_double pow(e_double base, e_double exp);
		
		/**
		 * \brief e 的指数函数.
		 * 
		 * @param exp 指数.
		 * @return e 的 exp 次幂.
		*/
		static e_float epow(e_float exp);
		
		/**
		 * \brief e 的指数函数.
		 * 
		 * @param exp 指数.
		 * @return e 的 exp 次幂.
		*/
		static e_double epow(e_double exp);
		
		/**
			\brief 计算 log10.
			
			@param f 真数			
			@return 以 10 为底, f 的对数.
			@throw 如果 f <= 0.0, 抛出 IllegalArgumentException;
		*/
		static e_float log10(e_float f);
		
		/**
			\brief 计算 log10.
			
			@param d 真数
			@return 以 10 为底, d 的对数.
			@throw 如果 d <= 0.0, 抛出 IllegalArgumentException;
		*/
		static e_double log10(e_double d);
		
		
		/**
			\brief 计算 ln.
			
			@param f 真数
			@return 以 e 为底, f 的对数.
			@throw 如果 f <= 0.0, 抛出 IllegalArgumentException;
		*/
		static e_float ln(e_float f);
		
		/**
			\brief 计算 ln.
			
			@param d 真数
			@return 以 e 为底, d 的对数.
			@throw 如果 d <= 0.0, 抛出 IllegalArgumentException;
		*/
		static e_double ln(e_double d);
		
		/**
			\brief 计算 log2.
			
			@param f 真数
			@return 以 2 为底, f 的对数.
			@throw 如果 f <= 0.0, 抛出 IllegalArgumentException;
		*/
		static e_float log2(e_float f);
		
		/**
			\brief 计算 log2.
			
			@param d 真数
			@return 以 2 为底, d 的对数.
			@throw 如果 d <= 0.0, 抛出 IllegalArgumentException;
		*/
		static e_double log2(e_double d);
		
		/**
			\brief sin 函数.
			
			@param f 弧度.
			@return sin(f).
		*/
		static e_float sin(e_float f);
		
		/**
			\brief sin 函数.
			
			@param d 弧度.
			@return sin(d).
		*/
		static e_double sin(e_double d);
		
		/**
			\brief cos 函数.
			
			@param f 弧度.
			@return cos(f).
		*/
		static e_float cos(e_float f);
		
		/**
			\brief cos 函数.
			
			@param d 弧度.
			@return cos(d).
		*/
		static e_double cos(e_double d);
		
		/**
			\brief tan 函数.
			
			@param f 弧度.
			@return tan(f).
		*/
		static e_float tan(e_float f);
		
		/**
			\brief tan 函数.
			
			@param d 弧度.
			@return tan(d).
		*/
		static e_double tan(e_double d);
		
		/**
			\brief cot 函数.
			
			@param f 弧度.
			@return cot(f).
		*/
		static e_float cot(e_float f);
		
		/**
			\brief cot 函数.
			
			@param d 弧度.
			@return cot(d).
		*/
		static e_double cot(e_double d);
		
		/**
			\brief asin 函数.
			
			@param f 弧度.
			@return asin(f).
		*/
		static e_float asin(e_float f);
		
		/**
			\brief asin 函数.
			
			@param d 弧度.
			@return asin(d).
		*/
		static e_double asin(e_double d);
		
		/**
			\brief acos 函数.
			
			@param f 弧度.
			@return acos(f).
		*/
		static e_float acos(e_float f);
		
		/**
			\brief acos 函数.
			
			@param d 弧度.
			@return acos(d).
		*/
		static e_double acos(e_double d);
		
		/**
			\brief atan 函数.
			
			@param f 弧度.
			@return atan(f).
		*/
		static e_float atan(e_float f);

		/**
			\brief atan 函数.
			
			@param d 弧度.
			@return atan(d).
		*/
		static e_double atan(e_double d);
		
		/**
			\brief acot 函数.
			
			@param f 弧度.
			@return acot(f).
		*/
		static e_float acot(e_float f);

		/**
			\brief acot 函数.
			
			@param d 弧度.
			@return acot(d).
		*/
		static e_double acot(e_double d);
		
		/**
			\brief 计算平方根.
			
			@param f
			@return f 的平方根.
		*/
		static e_float sqrt(e_float f); 
		
		/**
			\brief 计算平方根.
			
			@param d
			@return d 的平方根.
		*/
		static e_double sqrt(e_double d);

		/**
			\brief 向下取整.
			
			@param f 
			@return floor(f)
		*/
		static e_float floor(e_float f);
		
		/**
			\brief 向下取整.
			
			@param d
			@return floor(d)
		*/
		static e_double floor(e_double d);
		
		/**
			\brief 向上取整.
			
			@param f
			@return ceil(f)
		*/
		static e_float ceil(e_float f);
		
		/**
			\brief 向上取整.
			
			@param d
			@return ceil(d)
		*/
		static e_double ceil(e_double d);
		
		/**
			\brief 浮点取模运算.
			
			@param f 
			@param m 
			@return mod(abs(f), m);
		*/
		static e_float mod(e_float f, e_float m);
		
		/**
			\brief 浮点取模运算.
			
			@param d
			@param m 
			@return mod(abs(d), m);
		*/
		static e_double mod(e_double d, e_double m);
		
		/**
			\brief 获取下一个随机 e_bool 值.
			
			@return 随机 e_bool 值.
		*/
		static e_bool nextBool();
		
		/**
			\brief 获取下一个随机 e_char 值.
			
			@return 随机 e_char 值.
		*/
		static e_char nextChar();
		
		/**
			\brief 获取下一个随机 e_byte 值.
			
			@return 随机 e_byte 值.
		*/
		static e_byte nextByte();
		
		/**
			\brief 获取下一个随机 e_short 值.
			
			@param max 随机值的上限, 默认为 Short::MAX
			@return 小于 max 的随机 e_short 值.
			
			@throw 如果 max <= 0, 抛出 IllegalArgumentException.
		*/
		
		static e_short nextShort(e_short max=INT16_MAX);
		
		/**
			\brief 获取下一个随机 e_int 值.
			
			@param max 随机值的上限, 默认为 Integer::MAX
			@return 小于 max 的随机 e_int 值.
			
			@throw 如果 max <= 0, 抛出 IllegalArgumentException.
		*/
		static e_int nextInt(e_int max=INT32_MAX);
		
		/**
			\brief 获取下一个随机 e_long 值.
			
			@param max 随机值的上限, 默认为 Long::MAX
			@return 小于 max 的随机 e_long 值.
			
			@throw 如果 max <= 0, 抛出 IllegalArgumentException.
		*/
		static e_long nextLong(e_long max=INT64_MAX);
		
		/**
			\brief 获取下一个随机 e_float 值.
			
			@return 随机 e_float 值, 在 0.0 ~ 1.0 之间.
		*/
		static e_float nextFloat();
		
		/**
			\brief 获取下一个随机 e_double 值.
			
			@return 随机 e_double 值, 在 0.0 ~ 1.0 之间.
		*/
		static e_double nextDouble();

	private:
		Math() = delete;
		~Math() = delete;
		Math(Math& copy) = delete;
		Math& operator=(Math& copy) = delete;
		Math(Math&& move) = delete;
		Math& operator=(Math&& move) = delete;
};

} //Easy


#endif // LEC_MATH_H_