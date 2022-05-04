#include "globals.hpp"

//globals for threads
bool t1running = true;
bool t2running = true;
bool t3running = true;

std::vector<sdk::process> processes_hits;

//thread1
std::wstring input;

//thread3
bool kill_game = false;
std::wstring game = L"csgo.exe";


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
                sdk::process object(input);
                processes_hits.push_back(object);
            }
        }

        Sleep(200);
        //system("CLS");
    }
}

void Get_open_window_titles_loop()
{
    Sleep(10000);

    while (t1running == true)
    {
        get_open_window_titles();

        Sleep(10000);
    }
}

void Is_protected_game_open_and_killable()
{
    while (t3running == true)
    {
        auto processid = find_processId(game);

        if (processid)
        {
            std::cout << "[THREAD3] Protected game ID found!" << std::endl;

            if (kill_game == true)
            {
                kill_process_by_ID(processid);
            }
        }

        Sleep(5000);
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
    std::vector<std::string> hard_drives;

    for (const auto& item : HDD_SSD.ResultList)
    {
        std::wcout << item << std::endl;
        hardrives = hardrives.append(sdk::wstring_to_string(item) + " ");
        hard_drives.push_back(sdk::wstring_to_string(item));
    }

    //HWID CHECK HOST
    //open file check for cpu and harddrive IDs
    //if true then terminate and close anything and check other hardwareids if not listed append to blacklist
    //else start threads/looping and add his or her IDs only if a detection happened
    
    sdk::host host_object(create_host_hwid_string(sdk::wstring_to_string(OS.ResultList.at(0)), sdk::wstring_to_string(CPU.ResultList.at(0)), hardrives));

    if (open_file(sdk::wstring_to_string(CPU.ResultList.at(0)), hard_drives) == -2)
    {
        std::cout << "[MAIN] Terminating game..." << std::endl;
        //terminate game
        return 0;
    }

    //threads
    /*std::thread t1(Process_loop);
    std::thread t2(Get_open_window_titles_loop);
    std::thread t3(Is_protected_game_open_and_killable);

    t1.join();
    t2.join();
    t3.join();*/

    //Hackerconsole
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10); //10 is green

    //
    sdk::process object(L"Cheat Engine");

    std::cout << "[MEMORY] Searching for strings in processmemory:" << std::endl;
    auto memresults = 0;
    auto moduleresults = 0;

    for (auto itt : search_file())
    {
        std::cout << "[MEMORY] Searching for \"" << itt << "\"" << std::endl;
        memresults = memory::search_for_string(object.get_id(), itt.data(), itt.length()); 
        std::cout << "[MEMORY] Found " << std::dec << memresults << " references." << std::endl << std::endl;
    }

    if (memresults)
        host_object.set_flag(sdk::flags::Memory);

    std::cout << "[MEMORY] Searching loaded DLL's" << std::endl;
    moduleresults = memory::detect_modules(object.get_id());
    std::cout << "[MEMORY] Found " << std::dec << moduleresults << " references." << std::endl << std::endl;
    
    if (moduleresults)
        host_object.set_flag(sdk::flags::Module);

    ////Can be removed just testing...
    Sleep(3000);

    margins window = find_game_window_and_resolution(L"csgo.exe");

    if (window.get_bottom_height() != 0)
        screen_capture_part(window, "test.bmp");
    else
        std::cout << "[GAME WINDOW] NOT OPEN!" << std::endl;
   
    //OCR TEST
    ocr::execute_ocr();

    //Test for attaching CE
    debug_string_detection();

    //if flag > 2 then write HWID to blacklist ENDSTEP
    for (auto item : sdk::processes_tuple)
    {
        if ((int)host_object.get_flag() > 2)
        {
            write_file(create_flagged_hwid_string(sdk::wstring_to_string(CPU.ResultList.at(0)), hardrives));
            std::cout << "[MAIN] Added Hardware to Blacklist" << std::endl;
            std::cout << "[BAN] Found Memorydetection" << std::endl;
            kill_process_by_ID(std::get<1>(item));
        }
    }

    system("pause");
    
    return 0;
}
