#include "globals.hpp"

//globals for threads
bool t1running = true;
bool t2running = true;

std::vector<sdk::process> processes_hits;

//thread1
std::wstring input;


void Process_loop() 
{
    std::vector<std::wstring> processes;
    processes.push_back(L"cheat");
    processes.push_back(L"ida");
    processes.push_back(L"reclass");

    while (t1running == true) 
    {
        //loop the vector
        for (auto itt : processes)
        {
            std::wcout << "[THREAD1] Processing: " << itt << std::endl;
            input = print_processes(itt);

            if (input.at(0) != '0')
            {
                std::cout << "[THREAD1] Process found!" << std::endl;
                sdk::process object(input, sdk::flags::Process);
                processes_hits.push_back(object);
            }
        }

        Sleep(10000);
    }
}

void Get_open_window_titles_loop()
{
    while (t1running == true)
    {
        get_open_window_titles();

        Sleep(10000);
    }
}

int main()
{
    SetConsoleTitleA("DAPI - ACAR");

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

    std::string hardrives;

    for (const auto& item : HDD_SSD.ResultList)
    {
        std::wcout << item << std::endl;
        hardrives = hardrives.append(sdk::wstring_to_string(item));
    }

    //threads
    std::thread t1(Process_loop);
    std::thread t2(Get_open_window_titles_loop);

    t1.join();
    t2.join();


    write_file(sdk::wstring_to_string(OS.ResultList.at(0)), sdk::wstring_to_string(CPU.ResultList.at(0)), hardrives);


    //Hackerconsole
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10); //10 is green

    //std::cout << "[MEMORY] Searching for strings in processmemory:" << std::endl;
    //auto num_of_strings = 4; //Example
    //auto memresults = 0;
    //auto moduleresults = 0;
    //const char* strings[] = { "Cheat Engine", "ReClass" , "Hex-Rays", "Aimbot"};

    //for (size_t i = 0; i < num_of_strings; i++)
    //{
    //    std::cout << "[MEMORY] Searching for \"" << strings[i] << "\"" << std::endl;
    //    memresults = memory::search_for_string(object.get_id(), strings[i], strlen(strings[i]));
    //    std::cout << "[MEMORY] Found " << std::dec << memresults << " references." << std::endl << std::endl;
    //}

    //std::cout << "[MEMORY] Searching loaded DLL's" << std::endl;
    //moduleresults = memory::detect_modules(object.get_id());
    //std::cout << "[MEMORY] Found " << std::dec << moduleresults << " references." << std::endl << std::endl;

    //if ((memresults || moduleresults) != 0)
    //    object.set_flag(sdk::flags::Memory);

    //for (auto item : sdk::processes_tuple)
    //{
    //    if (object.get_flag() == sdk::flags::Memory)
    //    {
    //        std::cout << "[BAN] Found Memorydetection" << std::endl;
    //        kill_process_by_ID(std::get<1>(item));
    //    }
    //}

    ////Can be removed just testing...
    //Sleep(3000);

    //margins window = find_game_window_and_resolution(L"csgo.exe");

    //if (window.get_bottom_height() != 0)
    //    screen_capture_part(window, "test.bmp");
    //else
    //    std::cout << "[GAME WINDOW] NOT OPEN!" << std::endl;
   
    ////OCR TEST
    //ocr::execute_ocr();

    ////Test for attaching CE
    //debug_string_detection();

    system("pause");
    
    return 0;
}
