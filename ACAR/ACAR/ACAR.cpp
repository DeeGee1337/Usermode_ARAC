#include "globals.hpp"

int main()
{
    std::cout << "DAPI - ACAR" << std::endl;
    std::cout << "  _      _      _" << std::endl;
    std::cout << ">(.)__ <(.)__ =(.)__" << std::endl;
    std::cout << " (___/  (___/  (___/  \n\n" << std::endl;

    // Get OS and partition
    std::cout << "[DEBUG] OS/PARTITION: " << std::endl;
    WmiQueryResult OS = queryAndPrintResult(L"SELECT * FROM Win32_OperatingSystem", L"Name");

    for (const auto& item : OS.ResultList)
        std::wcout << item << std::endl;

    // Get id of CPU
    std::cout << "[DEBUG] CPU ID: " << std::endl;
    WmiQueryResult CPU = queryAndPrintResult(L"SELECT ProcessorId FROM Win32_Processor", L"ProcessorId");

    for (const auto& item : CPU.ResultList)
        std::wcout << item << std::endl;

    // Get serial number of Hard Drive
    std::cout << "[DEBUG] HDD's/SSD's: " << std::endl;
    WmiQueryResult HDD_SSD = queryAndPrintResult(L"SELECT SerialNumber FROM Win32_PhysicalMedia", L"SerialNumber");

    for (const auto& item : HDD_SSD.ResultList)
        std::wcout << item << std::endl;

    std::wstring input = print_processes(L"cheat");

    sdk::process object(input,sdk::flags::NONE);

    for (auto item : sdk::processes_tuple)
        kill_process_by_ID(std::get<1>(item));

    system("pause");
    
    return 0;
}
