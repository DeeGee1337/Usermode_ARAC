#include "globals.hpp"

int main()
{
    std::cout << "DAPI - ACAR" << std::endl;
    std::cout << "  _      _      _" << std::endl;
    std::cout << ">(.)__ <(.)__ =(.)__" << std::endl;
    std::cout << " (___/  (___/  (___/  \n\n" << std::endl;

    // Get OS and partition
    std::cout << "[HWID] OS/PARTITION: " << std::endl;
    WmiQueryResult OS = queryAndPrintResult(L"SELECT * FROM Win32_OperatingSystem", L"Name");

    for (const auto& item : OS.ResultList)
        std::wcout << item << std::endl;

    //Get OS buildver
    DWORD minver = 0;
    DWORD buildver = GetRealOSVersion(minver);
    std::wcout << "[HWID] Build: " << buildver << "." <<minver << std::endl;

    // Get id of CPU
    std::cout << "[HWID] CPU ID: " << std::endl;
    WmiQueryResult CPU = queryAndPrintResult(L"SELECT ProcessorId FROM Win32_Processor", L"ProcessorId");

    for (const auto& item : CPU.ResultList)
        std::wcout << item << std::endl;

    // Get serial number of Hard Drive
    std::cout << "[HWID] HDD's/SSD's: " << std::endl;
    WmiQueryResult HDD_SSD = queryAndPrintResult(L"SELECT SerialNumber FROM Win32_PhysicalMedia", L"SerialNumber");

    for (const auto& item : HDD_SSD.ResultList)
        std::wcout << item << std::endl;

    //processlist
    std::wstring input = print_processes(L"cheat");
    sdk::process object(input, sdk::flags::NONE);

    std::cout << "[MEMORY] Searching for strings in processmemory:" << std::endl;
    auto num_of_strings = 2; //Example
    auto memresults = 0;
    auto moduleresults = 0;
    const char* strings[] = { "Cheat Engine", "Dark Byte" };

    for (size_t i = 0; i < num_of_strings; i++)
    {
        std::cout << "[MEMORY] Searching for \"" << strings[i] << "\"" << std::endl;
        memresults = memory::search_for_string(object.get_id(), strings[i], strlen(strings[i]));
        std::cout << "[MEMORY] Found " << std::dec << memresults << " references." << std::endl << std::endl;
    }

    std::cout << "[MEMORY] Searching loaded CE DLL's" << std::endl;
    moduleresults = memory::detect_modules(object.get_id());
    std::cout << "[MEMORY] Found " << std::dec << moduleresults << " references." << std::endl << std::endl;

    if ((memresults || moduleresults) != 0)
        object.set_flag(sdk::flags::Memory);


    for (auto item : sdk::processes_tuple)
    {
        if (std::get<2>(item) == sdk::flags::Memory)
        {
            std::cout << "[BAN] Found Memorydetection" << std::endl;
            kill_process_by_ID(std::get<1>(item));
        }
    }

    system("pause");
    
    return 0;
}