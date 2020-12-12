export module Util;

import std.core;

import Common;

namespace Util
{
	export template<std::signed_integral I>
	I QstoiR(std::string_view const& _str)
	{
		I n = 1;
		I res = 0;
		for (isize i = _str.size() - 1; i >= 0; --i)
		{
			if (_str[i] >= '0' && _str[i] <= '9')
			{
				res += n * (_str[i] - '0');
				n *= 10;
			}
			else if (_str[i] == '-')
			{
				res = -res;
				break;
			}
			else
			{
				break;
			}
		}
		return res;
	}

	export template<std::unsigned_integral I>
	I QstoiR(std::string_view const& _str)
	{
		I n = 1;
		I res = 0;
		for (isize i = _str.size() - 1; i >= 0; --i)
		{
			if (_str[i] >= '0' && _str[i] <= '9')
			{
				res += n * (_str[i] - '0');
				n *= 10;
			}
			else
			{
				break;
			}
		}
		return res;
	}

	export template<std::integral I>
	I QstoiR(std::string const& _str)
	{
		return QstoiR<I>(std::string_view(_str));
	}

	export template<std::signed_integral I>
	I QstoiL(std::string_view const& _str)
	{
		I negative = 1;
		I res = 0;
		for (char const c : _str)
		{
			if (c >= '0' && c <= '9')
			{
				res *= 10;
				res += (c - '0');
			}
			else if (c == '-')
			{
				negative = -1;
			}
			else if (c != '+')
			{
				break;
			}
		}
		return negative * res;
	}

	export template<std::unsigned_integral I>
	I QstoiL(std::string_view const& _str)
	{
		I res = 0;
		for (char const c : _str)
		{
			if (c >= '0' && c <= '9')
			{
				res *= 10;
				res += (c - '0');
			}
			else if (c != '+')
			{
				break;
			}
		}
		return res;
	}

	export template<std::integral I>
	I QstoiL(std::string const& _str)
	{
		return QstoiL<I>(std::string_view(_str));
	}

	export template<typename T>
	class Grid
	{
		std::vector<T> m_data;
		usize m_width{ 0 };
		usize m_height{ 0 };
	public:
		Grid(usize _width, usize _height, T const& _init = T())
			: m_data(_width* _height, _init)
			, m_width(_width)
			, m_height(_height)
		{}

		Grid(Grid const& _oth)
			: m_data(_oth.m_data)
			, m_width(_oth.m_width)
			, m_height(_oth.m_height)
		{}
		Grid(Grid&& _oth)
			: m_data(std::move(_oth.m_data))
			, m_width(_oth.m_width)
			, m_height(_oth.m_height)
		{}
		Grid& operator=(Grid&& _oth)
		{
			m_data = std::move(_oth.m_data);
			m_width = _oth.m_width;
			m_height = _oth.m_height;
			_oth.m_width = 0;
			_oth.m_height = 0;
			return *this;
		}
		Grid& operator=(Grid const& _oth)
		{
			m_data = _oth.m_data;
			m_width = _oth.m_width;
			m_height = _oth.m_height;
			return *this;
		}

		T& at(usize _x, usize _y) { return m_data[_x + (m_width * _y)]; }
		T const& at(usize _x, usize _y) const { return m_data[_x + (m_width * _y)]; }
		T& operator[](usize _index) { return m_data[_index]; }
		T const& operator[](usize _index) const { return m_data[_index]; }

		usize const& width() const { return m_width; }
		usize const& height() const { return m_height; }

		constexpr typename std::vector<T>::iterator begin() noexcept { return m_data.begin(); }
		constexpr typename std::vector<T>::const_iterator begin() const noexcept { return m_data.begin(); }
		constexpr typename std::vector<T>::const_iterator cbegin() const noexcept { return m_data.cbegin(); }
		constexpr typename std::vector<T>::iterator end() noexcept { return m_data.end(); }
		constexpr typename std::vector<T>::const_iterator end() const noexcept { return m_data.end(); }
		constexpr typename std::vector<T>::const_iterator cend() const noexcept { return m_data.cend(); }
	};

	export template<typename T>
	class Grid2
	{
		std::vector<std::vector<T>> m_data;
		usize m_width{ 0 };
		usize m_height{ 0 };
	public:
		Grid2(usize _width, usize _height, T const& _init = T())
			: m_data(_height, std::vector<T>(_width, _init))
			, m_width(_width)
			, m_height(_height)
		{}

		T& at(usize _x, usize _y) { return m_data[_y][_x]; }
		T const& at(usize _x, usize _y) const { return m_data[_y][_x]; }
		T& operator[](usize _index) { return m_data[_index / m_width][_index % m_width]; }
		T const& operator[](usize _index) const { return m_data[_index / m_width][_index % m_width]; }

		usize const& width() const { return m_width; }
		usize const& height() const { return m_height; }

		constexpr typename std::vector<std::vector<T>>::iterator begin() noexcept { return m_data.begin(); }
		constexpr typename std::vector<std::vector<T>>::const_iterator begin() const noexcept { return m_data.begin(); }
		constexpr typename std::vector<std::vector<T>>::const_iterator cbegin() const noexcept { return m_data.cbegin(); }
		constexpr typename std::vector<std::vector<T>>::iterator end() noexcept { return m_data.end(); }
		constexpr typename std::vector<std::vector<T>>::const_iterator end() const noexcept { return m_data.end(); }
		constexpr typename std::vector<std::vector<T>>::const_iterator cend() const noexcept { return m_data.cend(); }
	};

	struct bool_wrapper
	{
		bool m_b;
	};

	export template<>
	class Grid<bool>
	{
		std::vector<bool_wrapper> m_data;
		usize m_width{ 0 };
		usize m_height{ 0 };
	public:
		Grid(usize _width, usize _height, bool const& _init = false)
			: m_data(_width* _height, { _init })
			, m_width(_width)
			, m_height(_height)
		{}

		bool& at(usize _x, usize _y) { return m_data[_x + (m_width * _y)].m_b; }
		bool const& at(usize _x, usize _y) const { return m_data[_x + (m_width * _y)].m_b; }
		bool& operator[](usize _index) { return m_data[_index].m_b; }
		bool const& operator[](usize _index) const { return m_data[_index].m_b; }

		usize const& width() const { return m_width; }
		usize const& height() const { return m_height; }
	};
}