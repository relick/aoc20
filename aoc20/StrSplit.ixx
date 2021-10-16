export module StrSplit;

import <concepts>;
import <string>;

import Common;

namespace util
{
	export template<typename str_T, std::predicate<char> DelimFunction>
	class str_split_iter
	{
		str_T const& m_s;
		DelimFunction const& m_fn;

		using str_iter = typename str_T::const_iterator; // for now

		str_iter m_currentBegin;
		str_iter m_currentEnd;

		void next_nondelim(str_iter& io_iter) const
		{
			while (io_iter != m_s.end() && m_fn(*io_iter))
			{
				++io_iter;
			}
		}

		str_iter find_next() const
		{
			auto current = m_currentEnd;
			next_nondelim(current);

			while (current != m_s.end() && !m_fn(*current))
			{
				++current;
			}

			return current;
		}

	public:
		str_split_iter(str_T const& _s, DelimFunction const& _fn, str_iter const& _start)
			: m_s{ _s }
			, m_fn{ _fn }
			, m_currentBegin{ _start }
			, m_currentEnd{ _start }
		{
			++(*this);
		}

		str_split_iter& operator++()
		{
			auto oldEnd = m_currentEnd;
			next_nondelim(oldEnd);
			m_currentEnd = find_next();
			m_currentBegin = oldEnd;

			return *this;
		}

		str_split_iter operator++(int)
		{
			str_split_iter clone{ *this };
			++(*this);
			return clone;
		}

		std::string_view operator*() const
		{
			return std::string_view(m_currentBegin, m_currentEnd);
		}

		bool operator!=(str_split_iter const& _b) const
		{
			return m_currentBegin != _b.m_currentBegin; // assumed ends are equal if begins are equal.
		}

		std::string_view next()
		{
			return *((*this)++);
		}
	};

	export template<typename str_T, std::predicate<char> DelimFunction>
		class str_split_range
	{
		str_T const& m_s;
		DelimFunction m_fn;

	public:
		using iterator = str_split_iter<str_T, DelimFunction>;
		using difference_type = isize;
		using size_type = usize;
		using value_type = std::string_view;
		using pointer = std::string_view*;
		using reference = std::string_view&;

		str_split_range(str_T const& _s, DelimFunction&& _fn)
			: m_s{ _s }
			, m_fn{ _fn }
		{}

		iterator begin() const
		{
			return iterator(m_s, m_fn, m_s.begin());
		}

		iterator end() const
		{
			return iterator(m_s, m_fn, m_s.end());
		}
	};

	export template<typename str_T, std::predicate<char> DelimFunction>
	str_split_range<str_T, DelimFunction> str_split(str_T const& _s, DelimFunction&& _fn)
	{
		return { _s, std::forward<DelimFunction>(_fn) };
	}

	namespace detail
	{
		export struct char_is_delim
		{
			char m_delim;
			char_is_delim(char _delim) : m_delim{ _delim } {}
			bool operator()(char const& _c) const { return _c == m_delim; }
		};
	}
	export template<typename str_T>
	str_split_range<str_T, detail::char_is_delim> str_split(str_T const& _s, char _delim)
	{
		return { _s, detail::char_is_delim(_delim) };
	}
}