export module common;

import std.core;

export using uint64 = uint64_t;
export using int64 = int64_t;
export using uint32 = uint32_t;
export using int32 = int32_t;
export using uint16 = uint16_t;
export using int16 = int16_t;
export using uint8 = uint8_t;
export using int8 = int8_t;
export using usize = size_t;
export using isize = ptrdiff_t;

namespace aoc
{
	export class input
	{
		std::string m_filename;
		usize m_filesize;
		std::ifstream m_file;

		usize get_filesize()
		{
			std::ifstream f(m_filename);
			f.seekg(0, std::ios::end);
			return f.tellg();
		}

		std::string get_filename_from_day(int32 _day, bool _test)
		{
			std::string const dayStr = std::to_string(_day);
			return std::string{ "inputs/" } + (dayStr.size() < 2 ? "0" : "") + dayStr + (_test ? "_test" : "") + ".txt";
		}

	public:
		input(int32 _day, bool _test = false)
			: m_filename{ get_filename_from_day(_day, _test) }
			, m_filesize{ get_filesize() }
			, m_file{ m_filename }
		{}

		std::string to_string()
		{
			m_file.seekg(0);

			std::string buffer(m_filesize, ' ');
			m_file.read(&buffer[0], m_filesize);
			return buffer;
		}

		template<std::integral T>
		std::vector<T> to_numbers()
		{
			m_file.seekg(0);

			std::vector<T> nums;
			nums.reserve(std::min<usize>(m_filesize / 2, 50000)); // reserve but set a reasonable upper limit
			while (!m_file.eof())
			{
				T i;
				if (m_file >> i)
				{
					nums.push_back(i);
				}
			}
			return nums;
		}

		std::vector<std::string> to_lines()
		{
			m_file.seekg(0);

			std::vector<std::string> lines;
			lines.reserve(std::min<usize>(m_filesize / 2, 50000)); // reserve but set a reasonable upper limit
			std::string s;
			while (std::getline(m_file, s))
			{
				lines.push_back(s);
			}
			return lines;
		}

		std::vector<std::string> to_blank_separated()
		{
			m_file.seekg(0);

			std::vector<std::string> blocks;
			blocks.reserve(std::min<usize>(m_filesize / 2, 50000)); // reserve but set a reasonable upper limit
			std::string s;
			blocks.emplace_back();
			std::string* next_line = &blocks.back();
			while (std::getline(m_file, s))
			{
				if (s.empty())
				{
					blocks.emplace_back();
					next_line = &blocks.back();
				}
				if (!next_line->empty())
				{
					*next_line += '\n';
				}
				*next_line += s;
			}
			if (next_line->empty())
			{
				blocks.pop_back();
			}
			return blocks;
		}
	};

	export std::string nice_output(int32 _day, std::string const& _partA, std::string const& _partB)
	{
		return std::string{ "Day " } + std::to_string(_day) + "\n--- \nPart A : " + _partA + "\nPart B : " + _partB + "\n\n";
	}

	export struct timer
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
		timer()
			: start{ std::chrono::high_resolution_clock::now() }
		{}

		void end(char const* _sectionName)
		{
			auto const end = std::chrono::high_resolution_clock::now();
			auto const durationNS = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			std::cout.imbue(std::locale(""));
			if (durationNS > 1000)
			{
				auto const durationUS = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
				std::cout << "[" << durationUS << " us to complete " << _sectionName << "]\n";
			}
			else
			{
				std::cout << "[" << durationNS << " ns to complete " << _sectionName << "]\n";
			}

			start = std::chrono::high_resolution_clock::now();
		}
	};

	export struct multi_timer
	{
		char const* m_name;
		usize m_num_runs;
		multi_timer(char const* _name, usize _num_runs = 1000)
			: m_name(_name)
			, m_num_runs(_num_runs)
		{}

		static std::pair<double, char const*> get_timeframe(int64 _ns)
		{
			if (_ns > 1000000000)
			{
				return { _ns / 1000000000.0, "s" };
			}
			else if (_ns > 1000000)
			{
				return { _ns / 1000000.0, "ms" };
			}
			else if (_ns > 1000)
			{
				return { _ns / 1000.0, "us" };
			}
			else
			{
				return { _ns, "ns" };
			}
		}

		template<typename F, typename ...Args>
		std::invoke_result_t<F, Args...> run(F&& _fn, Args... _args)
		{
			std::vector<int64> times(m_num_runs, 0);
			for (usize i = 0; i < times.size(); ++i)
			{
				auto const start = std::chrono::high_resolution_clock::now();
				_fn(_args...);
				auto const end = std::chrono::high_resolution_clock::now();
				times[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			}

			std::sort(times.begin(), times.end());
			int64 const mean = std::accumulate(times.begin(), times.end(), static_cast<int64>(0)) / static_cast<int64>(times.size());

			std::cout.imbue(std::locale(""));
			std::cout << std::setprecision(3);
			std::cout << m_num_runs << " runs - " << m_name << ":\n";
			auto const medLeftS = get_timeframe(times[times.size() / 2 - 1]);
			auto const medMiddleS = get_timeframe(times[times.size() / 2]);
			auto const medRightS = get_timeframe(times[times.size() / 2 + 1]);
			std::cout << "Median: ["
				<< medLeftS.first << medLeftS.second << ", "
				<< medMiddleS.first << medMiddleS.second << ", "
				<< medRightS.first << medRightS.second << "]"
				<< std::endl;
			auto const meanS = get_timeframe(mean);
			std::cout << "Mean: [" << meanS.first << meanS.second << "]\n\n";

			return _fn(_args...); // one last time to get answer
		}
	};

	export template<typename F, typename ...Args>
	auto timed(F&& _fn, Args... _args)
	{
		auto const start = std::chrono::high_resolution_clock::now();
		_fn(_args...);
		auto const end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	}
}

namespace util
{
	export template <std::integral T>
	T svtoi(std::string_view const& _sv)
	{
		T output;
		std::from_chars_result res = std::from_chars(_sv.data(), _sv.data() + _sv.size(), output);

		if (res.ec == std::errc::invalid_argument)
		{
			throw std::invalid_argument{ "invalid_argument" };
		}
		else if (res.ec == std::errc::result_out_of_range)
		{
			throw std::out_of_range{ "out_of_range" };
		}

		return output;
	}

	export template<typename T>
	class grid
	{
		std::vector<T> m_data;
		usize m_width{ 0 };
		usize m_height{ 0 };
	public:
		grid(usize _width, usize _height, T const& _init = T())
			: m_data(_width* _height, _init)
			, m_width(_width)
			, m_height(_height)
		{}

		T& at(usize _x, usize _y) { return m_data[_x + (m_width * _y)]; }
		T const& at(usize _x, usize _y) const { return m_data[_x + (m_width * _y)]; }
		T& operator[](usize _index) { return m_data[_index]; }
		T const& operator[](usize _index) const { return m_data[_index]; }

		usize const& width() const { return m_width; }
		usize const& height() const { return m_height; }
	};

	struct bool_wrapper
	{
		bool m_b;
	};

	export template<>
	class grid<bool>
	{
		std::vector<bool_wrapper> m_data;
		usize m_width{ 0 };
		usize m_height{ 0 };
	public:
		grid(usize _width, usize _height, bool const& _init = false)
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

namespace std
{
	struct bool_wrapper
	{
		bool m_b;
		void operator=(bool _o) { m_b = _o; }
		operator bool() const { return m_b; }
	};

	export using vector_bool = vector<bool_wrapper>;
}