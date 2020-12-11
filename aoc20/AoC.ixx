export module AoC;

import std.core;

import Common;

namespace AoC
{
	export template<usize N>
	std::string Day()
	{
		//static_assert(false, "Day not yet implemented!");
		return "Day not yet implemented!";
	}

	export class Input
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

		std::string get_filename_from_day(int32 _day, char const* _ext)
		{
			std::string const dayStr = std::to_string(_day);
			return std::string{ "inputs/" } + (dayStr.size() < 2 ? "0" : "") + dayStr + (_ext ? (std::string("_") + _ext) : "") + ".txt";
		}

	public:
		Input(int32 _day, bool _test = false)
			: m_filename{ get_filename_from_day(_day, _test ? "test" : nullptr) }
			, m_filesize{ get_filesize() }
			, m_file{ m_filename }
		{}

		Input(int32 _day, char const* _ext)
			: m_filename{ get_filename_from_day(_day, _ext) }
			, m_filesize{ get_filesize() }
			, m_file{ m_filename }
		{}

		std::string ToString()
		{
			m_file.seekg(0);

			std::string buffer(m_filesize, ' ');
			m_file.read(&buffer[0], m_filesize);
			return buffer;
		}

		template<std::integral T>
		std::vector<T> ToNumbers()
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

		std::vector<std::string> ToLines()
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

		std::vector<std::string> ToBlankSeparated()
		{
			m_file.seekg(0);

			std::vector<std::string> blocks;
			blocks.reserve(std::min<usize>(m_filesize / 2, 50000)); // reserve but set a reasonable upper limit
			std::string s;
			blocks.emplace_back();
			std::string* nextLine = &blocks.back();
			while (std::getline(m_file, s))
			{
				if (s.empty())
				{
					blocks.emplace_back();
					nextLine = &blocks.back();
				}
				if (!nextLine->empty())
				{
					*nextLine += '\n';
				}
				*nextLine += s;
			}
			if (nextLine->empty())
			{
				blocks.pop_back();
			}
			return blocks;
		}
	};

	export std::string NiceOutput(int32 _day, std::string const& _partA, std::string const& _partB)
	{
		return std::string{ "Day " } + std::to_string(_day) + "\n--- \nPart A : " + _partA + "\nPart B : " + _partB + "\n\n----------------------------\n";
	}

	export struct Benchmark
	{
		char const* m_name;
		usize m_numRuns;
		Benchmark(char const* _name, usize _numRuns = 1000)
			: m_name(_name)
			, m_numRuns(_numRuns)
		{}

		static std::pair<double, char const*> GetTimeframe(int64 _ns)
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
				return { static_cast<double>(_ns), "ns" };
			}
		}

		template<typename F, typename ...Args>
		std::invoke_result_t<F, Args...> Run(F&& _fn, Args&&... _args) const
		{
			std::vector<int64> times(m_numRuns, 0);
			for (usize i = 0; i < times.size(); ++i)
			{
				auto const start = std::chrono::high_resolution_clock::now();
				_fn(std::forward<Args>(_args)...);
				auto const end = std::chrono::high_resolution_clock::now();
				times[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			}

			std::sort(times.begin(), times.end());
			int64 const mean = std::accumulate(times.begin(), times.end(), static_cast<int64>(0)) / static_cast<int64>(times.size());

			std::cout.imbue(std::locale(""));
			std::cout << std::setprecision(3);
			std::cout << m_numRuns << " runs - " << m_name << ":\n";
			std::cout << "Median: [";
			if (times.size() >= 2)
			{
				auto const medLeftS = GetTimeframe(times[times.size() / 2 - 1]);
				std::cout << medLeftS.first << medLeftS.second << ", ";
			}
			auto const medMiddleS = GetTimeframe(times[times.size() / 2]);
			std::cout << medMiddleS.first << medMiddleS.second << (times.size() >= 3 ? ", " : "]");
			if (times.size() >= 3)
			{
				auto const medRightS = GetTimeframe(times[times.size() / 2 + 1]);
				std::cout << medRightS.first << medRightS.second << "]";
			}

			std::cout << std::endl;
			auto const meanS = GetTimeframe(mean);
			std::cout << "Mean: [" << meanS.first << meanS.second << "]\n\n";

			return _fn(std::forward<Args>(_args)...); // one last time to get answer
		}
	};
}