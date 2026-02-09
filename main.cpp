#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

struct enabled_flags
{
    bool has_flags;
    bool line_number; //-l
    bool case_sensitive; //-i
    bool reverse; // -r
};

bool contains(const std::string& line,
              const std::string& search,
              bool ignoreCase);

void simple_search();
void get_enabled_flags(int argc, char** argv, enabled_flags* flags);
std::vector<std::string> getFileContent(const std::string& file_name);
void file_search(int argc, char** argv);


int main(const int argc, char* argv[])
{
    if (argc > 1)
    {
        file_search(argc, argv);
    }
    else
    {
        simple_search();
    }

    return 0;
}

bool contains(const std::string& line,
              const std::string& search,
              const bool ignoreCase)
{
    if (!ignoreCase)
    {
        return line.find(search) != std::string::npos;
    }

    return std::ranges::search(
        line, search
        ,
        [](const unsigned char a, const unsigned char b)
        {
            return std::tolower(a) == std::tolower(b);
        }
    ).begin() != line.end();
}

void simple_search()
{
    std::string input;
    std::string search;
    std::cout << "Give a string from which to search for:" << std::endl;
    std::getline(std::cin, input);
    std::cout << "Give search string:" << std::endl;
    std::getline(std::cin, search);
    const size_t pos = input.find(search);
    if (pos != std::string::npos)
    {
        std::printf("\"%s\" found in \"%s\" in position %lu\n", search.c_str(), input.c_str(), pos);
    }
    else
    {
        std::printf("\"%s\" NOT found in \"%s\"\n", search.c_str(), input.c_str());
    }
}

void get_enabled_flags(const int argc, char** argv, enabled_flags* flags)
{
    for (int i = 1; i < argc; i++)
    {
        if (std::string flag = argv[i]; flag.find("-o") != std::string::npos)
        {
            flags->has_flags = true;
            if (flag.find('l') != std::string::npos)
            {
                flags->line_number = true;
            }
            if (flag.find('i') != std::string::npos)
            {
                flags->case_sensitive = true;
            }
            if (flag.find('r') != std::string::npos)
            {
                flags->reverse = true;
            }
            return;
        }
    }
}

std::vector<std::string> getFileContent(const std::string& file_name)
{
    std::vector<std::string> lines;

    std::ifstream input{file_name};

    if (input.bad() || input.fail())
    {
        std::cerr << "An exception occurred. Exception Nr. -1" << std::endl;
        std::cerr << "Could not find out the size of file " << "\"" << file_name << "\"" << std::endl;
        return lines;
    }

    if (!input.is_open())
    {
        std::cerr << "An exception occurred. Exception Nr. -1" << std::endl;
        std::cerr << "Could not open file " << "\"" << file_name << "\"" << std::endl;
        return lines;
    }

    for (std::string line; std::getline(input, line);)
    {
        lines.push_back(line);
    }

    if (lines.empty())
    {
        std::cerr << "An exception occurred. Exception Nr. -1" << std::endl;
        std::cerr << "\"" << file_name << "\"" << " is empty" << std::endl;
        return lines;
    }

    return lines;
}

void file_search(const int argc, char** argv)
{
    if (argc == 2)
    {
        std::cerr << "File name required" << std::endl;
        return;
    }
    enabled_flags flags = {false, false, false, false};

    enabled_flags* ef = &flags;
    get_enabled_flags(argc, argv, ef);

    const std::string search = argv[argc - 2];
    const std::string file_name = argv[argc - 1];
    const auto result = getFileContent(file_name);
    if (result.empty())
    {
        return;
    }
    int line_count = 0;
    int result_count = 0;
    for (auto& line : result)
    {
        line_count++;
        const bool contain = contains(line, search, ef->case_sensitive);
        if (ef->reverse ? !contain : contain)
        {
            if (ef->line_number)
            {
                std::printf("%i:     %s\n", line_count, line.c_str());
            }
            else
            {
                std::printf("%s\n", line.c_str());
            }
            result_count++;
        }
    }
    if (ef->line_number)
    {
        printf("\n");
        std::printf("Occurrences of lines containing \"%s\": %i\n", search.c_str(), result_count);
    }
}
