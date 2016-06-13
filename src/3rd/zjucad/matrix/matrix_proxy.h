/*
	State Key Lab of CAD&CG Zhejiang Unv.

	Author: Jin Huang (hj@cad.zju.edu.cn)

	Copyright (c) 2004-2011 <Jin Huang>
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	1. Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	derived from this software without specific prior written permission.
*/

#pragma once

#include "matrix_expression.h"
#include "colon.h"
#include <algorithm>

namespace zjucad { namespace matrix {

#define KEEP_SIZE_RESIZE \
	void resize(size_type size) {assert(size == size());} \
	void resize(size_type row, size_type col) {assert(row == size(1) && col == size(2));}

#define SCALAR_FILL\
	const expression_type& operator=(const_reference a) {\
		std::fill(begin(), end(), a);\
		return *this;\
	}

#define PROXY_ASSIGN \
	template <typename E9> \
	const expression_type& operator=(const matrix_expression<E9> &e) { \
		e().assign(e(), *this); \
		return *this; \
	} \
	const expression_type& operator=(const expression_type &e) { \
		e.assign(e, *this); \
		return *this; \
	}

#define PROXY_OPS \
	PROXY_ACCESS; \
	SCALAR_FILL; \
	MATRIX_SELF_OP; \
	KEEP_SIZE_RESIZE; \
	PROXY_ASSIGN;

template <typename E>
class idx_cv_i : public matrix_expression<idx_cv_i<E> > {
public:
    typedef idx_cv_i<E> expression_type;
	typedef typename E::size_type size_type;
	typedef typename E::const_reference const_reference;
	typedef typename E::reference reference;
	typedef typename E::value_type value_type;

	idx_cv_i(E &m, size_type col):m_m(m), m_col(col) {assert(m_col >= 0 && m_col < m_m.size(2));}

	// basic operation
	// matrix operation
	size_type size(int dim) const {return (1==dim)?m_m.size(1):1;}
	const_reference operator()(idx_type i, idx_type j) const {assert(j == 0); return m_m(i, m_col);}
	reference operator()(idx_type i, idx_type j) {assert(j == 0); return m_m(i, m_col);}

	// vector operation
	size_type size(void) const {return m_m.size(1);}

	const_reference operator()(idx_type i) const {return m_m(i, m_col);}
	reference operator()(idx_type i) {return m_m(i, m_col);}
	const_reference operator[](idx_type i) const {return m_m(i, m_col);}
	reference operator[](idx_type i) {return m_m(i, m_col);}

	typedef typename E::const_iterator const_iterator;
	typedef typename E::iterator iterator;

	const_iterator begin(void) const {
		assert(m_col>=0 && m_col<m_m.size(2));
		return const_iterator(m_m.begin()+m_col*m_m.size(1));
	}
	const_iterator end(void) const {return const_iterator(m_m.begin()+(m_col+1)*m_m.size(1));}

	iterator begin(void) {
		assert(m_col>=0 && m_col<m_m.size(2));
		return iterator(m_m.begin()+m_col*m_m.size(1));
	}
	iterator end(void) {return iterator(m_m.begin()+(m_col+1)*m_m.size(1));}

	PROXY_OPS;

	E &m_m;
	size_type m_col;
};

template <typename E1, typename E2>
class idx_cv_J : public matrix_expression<idx_cv_J<E1, E2> > {
public:
    typedef idx_cv_J<E1, E2> expression_type;
	typedef typename E1::size_type size_type;
	typedef typename E1::const_reference const_reference;
	typedef typename E1::reference reference;
	typedef typename E1::value_type value_type;

	idx_cv_J(E1 &m, const E2 &J):m_m(m), m_J(J) {}

	// basic operation
	// matrix operation
	size_type size(int dim) const {return (1==dim)?m_m.size(1):m_J.size();}
	const_reference operator()(idx_type i, idx_type j) const {return m_m(i, m_J[j]);}
	reference operator()(idx_type i, idx_type j) {return m_m(i, m_J[j]);}

	// vector operation
	size_type size(void) const {return m_m.size(1)*m_J.size();}

	const_reference operator()(idx_type i) const {return m_m(i%m_m.size(1), i/m_m.size(1));}
	reference operator()(idx_type i) {return m_m(i%m_m.size(1), i/m_m.size(1));}
	const_reference operator[](idx_type i) const {return m_m(i%m_m.size(1), i/m_m.size(1));}
	reference operator[](idx_type i) {return m_m(i%m_m.size(1), i/m_m.size(1));}

	template <typename M, typename I>
	struct iterator_base : public default_matrix_iterator<value_type> {
		iterator_base(){}
		iterator_base(M *m, size_type row, size_type col):m_m(m), m_row(row), m_col(col){
			if(m_col < m_m->m_J.size())
				m_i = m_m->m_m.begin()+
					m_row+m_m->m_J[m_col]*m_m->m_m.size(1);
		}
		iterator_base operator++(void) {
			if(m_m->m_m.size(1)-1 == m_row) {
				++m_col;
				m_row = 0;
				if(m_col < m_m->m_J.size())
					m_i=m_m->m_m.begin()+m_m->m_m.size(1)*m_m->m_J[m_col];
			}
			else {
				++m_i; ++m_row;
			}
			return *this;
		}
		iterator_base operator+=(offset_type n) {
			const offset_type rn = m_m->m_m.size(1);
			const offset_type n2 = m_row+m_col*rn+n;
			m_row = n2%rn;
			m_col = n2/rn;
			if(m_col < m_m->m_J.size())
				m_i = m_m->m_m.begin()+
					m_row+m_m->m_J[m_col]*m_m->m_m.size(1);
			return *this;
		}
		iterator_base operator+(offset_type n) const {
			iterator_base i = *this;
			i+=n;
			return i;
		}
		const_reference operator*(void) const {return *m_i;}
		template <typename I1>
		bool operator != (const I1 &i) const {return m_row !=i.m_row || m_col != i.m_col;}
		template <typename I1>
		bool operator == (const I1 &i) const {return !this->operator!=(i);}
		M *m_m;
		I m_i;
		size_type m_row, m_col;
	};
	typedef iterator_base<const expression_type, typename E1::const_iterator> const_iterator;
	const_iterator begin(void) const {return const_iterator(this, 0, 0);}
	const_iterator end(void) const {return const_iterator(this, 0, size(2));}

	struct iterator : public iterator_base<expression_type, typename E1::iterator> {
		iterator(){}
		iterator(expression_type *m, size_type row, size_type col)
			:iterator_base<expression_type, typename E1::iterator>(m, row, col){}
		operator const_iterator() const {return const_iterator(this->m_m, this->m_row, this->m_i);}
		reference operator*(void) {return *this->m_i;}
		iterator operator+(size_type n) const {
			iterator i = *this;
			i+=n;
			return i;
		}
	};
	iterator begin(void) {return iterator(this, 0, 0);}
	iterator end(void) {return iterator(this, 0, size(2));}

	PROXY_OPS;

	E1 &m_m;
	const E2 &m_J;
};

template <typename E>
class idx_i_cv : public matrix_expression<idx_i_cv<E> > {
public:
    typedef idx_i_cv<E> expression_type;
	typedef typename E::size_type size_type;
	typedef typename E::const_reference const_reference;
	typedef typename E::reference reference;
	typedef typename E::value_type value_type;

	idx_i_cv(E &m, size_type row):m_m(m), m_row(row) {assert(m_row >= 0 && m_row < m_m.size(1));}

	// basic operation
	// vector operation
	size_type size(void) const {return m_m.size(2);}

	// 2D matrix operation
	size_type size(int dim) const {return (1==dim)?1:m_m.size(2);}
	const_reference operator()(idx_type i, idx_type j) const {assert(j == 0); return m_m(i, this->m_col);}
	reference operator()(idx_type i, idx_type j) {assert(j == 0); return m_m(i, this->m_col);}

	const_reference operator()(idx_type i) const {return m_m(m_row, i);}
	reference operator()(idx_type i) {return m_m(m_row, i);}
	const_reference operator[](idx_type i) const {return m_m(m_row, i);}
	reference operator[](idx_type i) {return m_m(m_row, i);}

	template <typename I>
	struct iterator_base : public default_matrix_iterator<value_type> {
		iterator_base(){}
		iterator_base(I i, size_type rows):m_i(i), m_rows(rows) {}
		const_reference operator *(void) const {return *m_i;}
		iterator_base operator++(void) {m_i+=m_rows; return *this;}
		iterator_base operator+=(size_type n) {m_i+=n*m_rows; return *this;}
		iterator_base operator+(size_type n) const {iterator_base i=*this; i+=n; return i;}
		template <typename I1>
		bool operator != (const I1 &i) const {return m_i != i.m_i;}
		template <typename I1>
		bool operator == (const I1 &i) const {return !this->operator!=(i);}
		I m_i;
		size_type m_rows;
	};
	typedef iterator_base<typename E::const_iterator> const_iterator;
	struct iterator : public iterator_base<typename E::iterator> {
		iterator(){}
		iterator(typename E::iterator i, size_type rows):iterator_base<typename E::iterator>(i, rows) {}
		operator const_iterator() const {return const_iterator(this->m_i, this->m_rows);}
		iterator operator+(size_type n) const {iterator i=*this; i+=n; return i;}
		reference operator *(void) {return *this->m_i;}
	};

	const_iterator begin(void) const {return const_iterator(m_m.begin()+m_row, m_m.size(1));}
	const_iterator end(void) const {return const_iterator(m_m.begin()+m_row+m_m.size(2)*m_m.size(1), m_m.size(1));}
	iterator begin(void) {return iterator(m_m.begin()+m_row, m_m.size(1));}
	iterator end(void) {return iterator(m_m.begin()+m_row+m_m.size(2)*m_m.size(1), m_m.size(1));}

	PROXY_OPS;

	E &m_m;
	size_type m_row;
};

template <typename E, typename E1>
class idx_I : public matrix_expression<idx_I<E, E1> > {
public:
    typedef idx_I<E, E1> expression_type;
	typedef typename E::size_type size_type;
	typedef typename E::const_reference const_reference;
	typedef typename E::reference reference;
	typedef typename E::value_type value_type;

	idx_I(E &m, const E1 &I):m_m(m), m_I(I) {}

	// basic operation
	// vector operation
	size_type size(void) const {return m_I.size();}
	const_reference operator[](idx_type i) const {return m_m(m_I(i));}
	const_reference operator()(idx_type i) const {return m_m(m_I(i));}
	reference operator[](idx_type i) {return m_m(m_I(i));}
	reference operator()(idx_type i) {return m_m(m_I(i));}

	// 2D matrix operation
	size_type size(int dim) const {return (1==dim)?m_I.size():1;}
	const_reference operator()(idx_type row, idx_type col) const {assert(col == 0);return m_m(m_I(row));}
	reference operator()(idx_type row, idx_type col) {assert(col == 0);return m_m(m_I(row));}

	template <typename E2>
	struct iterator_base : public default_matrix_iterator<value_type> {
		iterator_base(){}
		iterator_base(E2 *m, typename E1::const_iterator iter_I):m_m(m), m_iter_I(iter_I) {}
		const_reference operator *(void) const {return (*m_m)(*m_iter_I);}
		iterator_base operator++(void) {++m_iter_I; return *this;}
		iterator_base operator+=(size_type n) {m_iter_I+=n; return *this;}
		iterator_base operator+(size_type n) const {iterator_base i=*this; i+=n; return i;}
		template <typename I1>
		bool operator != (const I1 &i) const {assert(m_m == i.m_m); return m_iter_I != i.m_iter_I;}
		template <typename I1>
		bool operator == (const I1 &i) const {return !this->operator!=(i);}
		E2 *m_m;
		typename E1::const_iterator m_iter_I;
	};
	typedef iterator_base<const E> const_iterator;
	struct iterator : public iterator_base<E> {
		iterator(){}
		iterator(E *m, typename E1::const_iterator iter_I):iterator_base<E>(m, iter_I) {}
		operator const_iterator() const {return const_iterator(m_m, this->m_i);}
		iterator operator+(size_type n) const {iterator i=*this; i+=n; return i;}
		reference operator *(void) {return (*this->m_m)(*this->m_iter_I);}
	};

	const_iterator begin(void) const {return const_iterator(&m_m, m_I.begin());}
	const_iterator end(void) const {return const_iterator(&m_m, m_I.end());}
	iterator begin(void) {return iterator(&m_m, m_I.begin());}
	iterator end(void) {return iterator(&m_m, m_I.end());}

	PROXY_OPS;

	E &m_m;
	const E1 &m_I;
};

template <typename E, typename T>
class idx_range : public matrix_expression<idx_range<E, T> > {
public:
    typedef idx_range<E, T> expression_type;
	typedef typename E::size_type size_type;
	typedef typename E::const_reference const_reference;
	typedef typename E::reference reference;
	typedef typename E::value_type value_type;

	idx_range(E &m, const range<T> &range):m_m(m), m_range(range) {}

	// basic operation
	// vector operation
	size_type size(void) const {return m_range.size();}
	const_reference operator()(size_type i) const {return m_m(m_range(i));}
	reference operator()(size_type i) {return m_m(m_range(i));}
	const_reference operator[](idx_type i) const {return m_m(m_range(i));}
	reference operator[](idx_type i) {return m_m(m_range(i));}

	// 2D matrix operation
	size_type size(int dim) const {return (dim==1)?m_range.size():1;}
	const_reference operator()(size_type i, size_type j) const {
		assert(j == 0);
		return m_m(m_range(i));
	}
	reference operator()(size_type i, size_type j) {
		assert(j == 0);
		return m_m(m_range(i));
	}

	typedef typename E::const_iterator const_iterator;
	typedef typename E::iterator iterator;

	const_iterator begin(void) const {return m_m.begin()+m_range.m_front;}
	const_iterator end(void) const {return m_m.begin()+m_range.m_end;}
	iterator begin(void) {return m_m.begin()+m_range.m_front;}
	iterator end(void) {return m_m.begin()+m_range.m_end;}

	PROXY_OPS;

	E &m_m;
	range<T> m_range;
};

template <typename E>
class idx_cv : public matrix_expression<idx_cv<E> > {
public:
    typedef idx_cv<E> expression_type;
	typedef typename E::size_type size_type;
	typedef typename E::const_reference const_reference;
	typedef typename E::reference reference;
	typedef typename E::value_type value_type;

	idx_cv(E &m):m_m(m) {}

	// basic operation
	// vector operation
	size_type size(void) const {return m_m.size();}
	const_reference operator()(size_type i) const {return m_m(i);}
	reference operator()(size_type i) {return m_m(i);}
	const_reference operator[](idx_type i) const {return m_m[i];}
	reference operator[](idx_type i) {return m_m[i];}

	// 2D matrix operation
	size_type size(int dim) const {return (dim==1)?m_m.size():1;}
	const_reference operator()(size_type i, size_type j) const {
		assert(j == 0);
		return m_m(i);
	}
	reference operator()(size_type i, size_type j) {
		assert(j == 0);
		return m_m(i);
	}

	typedef typename E::const_iterator const_iterator;
	typedef typename E::iterator iterator;

	const_iterator begin(void) const {return m_m.begin();}
	const_iterator end(void) const {return m_m.end();}
	iterator begin(void) {return m_m.begin();}
	iterator end(void) {return m_m.end();}

	PROXY_OPS;

	E &m_m;
};

template <typename E, typename R1, typename R2>
class idx_range_range : public matrix_expression<idx_range_range<E, R1, R2> > {
public:
    typedef idx_range_range<E, R1, R2> expression_type;
	typedef typename E::size_type size_type;
	typedef typename E::const_reference const_reference;
	typedef typename E::reference reference;
	typedef typename E::value_type value_type;

	idx_range_range(E &m, R1 &range1, R2 &range2):m_m(m), m_range1(range1), m_range2(range2) {}

	// basic operation
	// vector operation
	size_type size(void) const {return m_range1.size()*m_range2.size();}
	const_reference operator()(size_type i) const {
		return (*this)(i%m_range1.size(), i/m_range1.size());
	}
	reference operator()(size_type i) {
		return (*this)(i%m_range1.size(), i/m_range1.size());
	}
	const_reference operator[](size_type i) const {
		return (*this)(i%m_range1.size(), i/m_range1.size());
	}
	reference operator[](size_type i) {
		return (*this)(i%m_range1.size(), i/m_range1.size());
	}

	// 2D matrix operation
	size_type size(int dim) const {return (1==dim)?m_range1.size():m_range2.size();}
	const_reference operator()(size_type i, size_type j) const {
		return m_m(m_range1[i], m_range2[j]);
	}
	reference operator()(size_type i, size_type j) {
		return m_m(m_range1[i], m_range2[j]);
	}

	template <typename M, typename I>
	struct iterator_base : public default_matrix_iterator<value_type> {
		iterator_base(){}
		iterator_base(M *m, size_type row, size_type col):m_m(m), m_row(row){
			m_i = m_m->m_m.begin()+
				(m_m->m_range1.m_front+m_row)+
				(m_m->m_range2.m_front+col)*m_m->m_m.size(1);
		}
//		template <typename I1>
//		iterator_base(M *m, size_type row, I1 i):m_m(m), m_row(row), m_i(i){}
		iterator_base operator++(void) {
			if(m_m->m_range1.size()-1 == m_row) {
				m_i+=(m_m->m_m.size(1)-m_m->size(1)+1); m_row = 0;
			}
			else {
				++m_i; ++m_row;
			}
			return *this;
		}
		iterator_base operator+=(offset_type n) {
			if(!n) return *this;
			if(n == m_m->size(1)) {
				m_i+=m_m->m_m.size(1);
				return *this;
			}
			offset_type row_adv = m_row+n;
			offset_type col_adv = row_adv/m_m->size(1);
			row_adv = (row_adv%m_m->size(1))-m_row;
			m_i+=(col_adv*m_m->m_m.size(1)+row_adv);
			m_row += row_adv;
			return *this;
		}
		iterator_base operator+(offset_type n) const {
			iterator_base i = *this;
			i+=n;
			return i;
		}
		const_reference operator*(void) const {return *m_i;}
		template <typename I1>
		bool operator != (const I1 &i) const {return m_i !=i.m_i;}
		template <typename I1>
		bool operator == (const I1 &i) const {return !this->operator!=(i);}
		M *m_m;
		I m_i;
		size_type m_row;
	};

	typedef iterator_base<const expression_type, typename E::const_iterator> const_iterator;
	const_iterator begin(void) const {return const_iterator(this, 0, 0);}
	const_iterator end(void) const {return const_iterator(this, 0, size(2));}

	struct iterator : public iterator_base<expression_type, typename E::iterator> {
		iterator(){}
		iterator(expression_type *m, size_type row, size_type col)
			:iterator_base<expression_type, typename E::iterator>(m, row, col){}
		operator const_iterator() const {return const_iterator(this->m_m, this->m_row, this->m_i);}
		reference operator*(void) {return *this->m_i;}
		iterator operator+(size_type n) const {
			iterator i = *this;
			i+=n;
			return i;
		}
	};

	iterator begin(void) {return iterator(this, 0, 0);}
	iterator end(void) {return iterator(this, 0, size(2));}

	PROXY_OPS;

	E &m_m;
	R1 m_range1;
	R2 m_range2;
};

template <typename E, typename R>
class idx_cv_range : public matrix_expression<idx_cv_range<E, R> > {
public:
    typedef idx_cv_range<E, R> expression_type;
	typedef typename E::size_type size_type;
	typedef typename E::const_reference const_reference;
	typedef typename E::reference reference;
	typedef typename E::value_type value_type;

	idx_cv_range(E &m, R &range):m_m(m), m_range(range) {}

	// basic operation
	// vector operation
	size_type size(void) const {return m_m.size(1)*m_range.size();}
	const_reference operator()(size_type i) const {
		return m_m(i%m_m.size(1), i/m_m.size(1));
	}
	reference operator()(size_type i) {
		return m_m(i%m_m.size(1), i/m_m.size(1));
	}
	const_reference operator[](size_type i) const {
		return m_m(i%m_m.size(1), i/m_m.size(1));
	}
	reference operator[](size_type i) {
		return m_m(i%m_m.size(1), i/m_m.size(1));
	}

	// 2D matrix operation
	size_type size(int dim) const {return (1==dim)?m_m.size(1):m_range.size();}
	const_reference operator()(size_type i, size_type j) const {
		return m_m(i, m_range[j]);
	}
	reference operator()(size_type i, size_type j) {
		return m_m(i, m_range[j]);
	}

	typedef typename E::const_iterator const_iterator;
	typedef typename E::iterator iterator;
	const_iterator begin(void) const {return m_m.begin()+m_range.m_front*m_m.size(1);}
	const_iterator end(void) const {return m_m.begin()+m_range.m_end*m_m.size(1);}

	iterator begin(void) {return m_m.begin()+m_range.m_front*m_m.size(1);}
	iterator end(void) {return m_m.begin()+m_range.m_end*m_m.size(1);}

	PROXY_OPS;

	E &m_m;
	R m_range;
};

template <typename E, typename R>
class idx_range_i : public matrix_expression<idx_range_i<E, R> > {
public:
    typedef idx_range_i<E, R> expression_type;
	typedef typename E::size_type size_type;
	typedef typename E::const_reference const_reference;
	typedef typename E::reference reference;
	typedef typename E::value_type value_type;

	idx_range_i(E &m, R &range_row, size_type col):m_m(m), m_range(range_row), m_col(col) {}

	// basic operation
	// vector operation
	size_type size(void) const {return m_range.size();}
	const_reference operator()(size_type i) const {
		return m_m(m_range(i), m_col);
	}
	reference operator()(size_type i) {
		return m_m(m_range(i), m_col);
	}
	const_reference operator[](size_type i) const {
		return m_m(m_range(i), m_col);
	}
	reference operator[](size_type i) {
		return m_m(m_range(i), m_col);
	}

	// 2D matrix operation
	size_type size(int dim) const {return (1==dim)?m_range.size():1;}
	const_reference operator()(size_type i, size_type j) const {
		assert(j == 0);
		return m_m(m_range[i], m_col);
	}
	reference operator()(size_type i, size_type j) {
		assert(j == 0);
		return m_m(m_range[i], m_col);
	}

	typedef typename E::const_iterator const_iterator;
	typedef typename E::iterator iterator;
	const_iterator begin(void) const {return m_m.begin()+m_range.m_front+m_col*m_m.size(1);}
	const_iterator end(void) const {return m_m.begin()+m_range.m_end+m_col*m_m.size(1);}

	iterator begin(void) {return m_m.begin()+m_range.m_front+m_col*m_m.size(1);}
	iterator end(void) {return m_m.begin()+m_range.m_end+m_col*m_m.size(1);}

	PROXY_OPS;

	E &m_m;
	R m_range;
	size_type m_col;
};

}	// namespace matrix
}	// namespace zjucad
