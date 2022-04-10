#include "globals.hpp"

int main()
{
    std::wstring input = print_processes(L"cheat");

    std::cout << "\n\nDAPI - ACAR" << std::endl;
    std::cout << "  _      _      _" << std::endl;
    std::cout << ">(.)__ <(.)__ =(.)__" << std::endl;
    std::cout << " (___/  (___/  (___/  \n\n" << std::endl;

    sdk::process object(input,sdk::flags::NONE);

    for (auto item : sdk::processes_tuple)
        kill_process_by_ID(std::get<1>(item));

    system("pause");
    
    return 0;
}
