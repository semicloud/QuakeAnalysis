#pragma once

#include <string>
#include <vector>

namespace adsma
{
	class Input_file
	{
	private:
		std::string m_mxd02_file;
		std::string m_mxd03_file;
		std::string m_mxd35_file;
	public:
		Input_file(const std::string& mxd02_file, const std::string& mxd03_file,
			const std::string& mxd35_file);
		~Input_file();
		static std::vector<Input_file> load(const std::string& file_path);
		std::string mxd02_file() const
		{
			return m_mxd02_file;
		}

		std::string mxd03_file() const
		{
			return m_mxd03_file;
		}

		std::string mxd35_file() const
		{
			return m_mxd35_file;
		}
	};

}

