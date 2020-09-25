/* Custom manipulators.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __IOMANIP_HPP__
#define __IOMANIP_HPP__

#include <iostream>

using namespace std;

//{ endm manipulator
inline ostream& endm(ostream& str) {
	str << "[eol]\n";
	return str;
}
//}

//{ squares manipulator
class SquaresProxy {
public:
	explicit SquaresProxy(ostream & os) :os(os) {}
	ostream & os;
};

template<typename Arg>
	 ostream & operator<<(SquaresProxy const& a, Arg const& arg) {
		a.os << "[" << arg << "]";
		return a.os;
	}

inline class SquaresCreator { } squares;

inline SquaresProxy operator<<(ostream & os, SquaresCreator) {
	return SquaresProxy(os);
}
//}

//{ add manipulator
class Adder {
public:
	struct farg {
	ostream& ostr;
	};

	template<typename T>
	struct sarg {
		ostream& ostr;
		T value;
	};
};

inline Adder::farg operator<< (ostream& ostr, Adder) {
	return {ostr};
}

template<typename T>
Adder::sarg<T> operator<< (Adder::farg fag, T&& fval) {
	return {fag.ostr, fval};
}

template<typename TL, typename TR>
ostream& operator<< (Adder::sarg<TL> sag, TR&& sval) {
	sag.ostr << (sag.value + sval);
	return sag.ostr;
}

inline Adder add;
//}

#endif // __IOMANIP_HPP__
