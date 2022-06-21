#include "globals.hpp"


//globals for threads
bool t1running = true;
bool t2running = false;
bool t3running = false;
bool t4running = false;
bool t5running = false;
bool t6running = false;
std::mutex m_lock;
bool canceloutput = false;

std::vector<sdk::process> processes_hits;

//thread1
std::wstring input;

//thread3
bool kill_game = false;
std::wstring game = L"csgo.exe";

//Thread 1
void Process_loop()
{
    std::vector<std::wstring> processes;
    processes.push_back(L"cheat");
    processes.push_back(L"ida");
    processes.push_back(L"reclass");

    while (t1running == true)
    {
        //loop the vectorc
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
        m_lock.lock();
        t1running = false;
        t2running = true;
        m_lock.unlock();   
    }
}

//Thread 2
void Is_protected_game_open_and_killable()
{
    while (t2running == true)
    {
        auto processid = find_processId(game);

        if (processid)
        {
            std::cout << "[THREAD2] Protected game ID found!" << std::endl;

            if (kill_game == true)
            {
                kill_process_by_ID(processid);
            }
        }
        m_lock.lock();
        t2running = false;
        t3running = true;
        m_lock.unlock();
    }
}


//Thread 3
void String_Detection(sdk::host& host_object)
{
    while (t3running == true)
    {
        //EINKOMMENTIEREN
        //if (debug_string_detection())
         //   host_object.set_flag(sdk::flags::Driveroutput);

        m_lock.lock();
        t3running = false;
        t4running = true;
        m_lock.unlock();
    }
}



//Thread 4
void Get_open_window_titles_loop(sdk::host& host_object)
{
    while (t4running == true)
    {
        if (get_open_window_titles())
        {
            host_object.set_flag(sdk::flags::Windowname);
            canceloutput = true;
        }

        m_lock.lock();
        t4running = false;
        t5running = true;
        m_lock.unlock();
    }
}


//THread 5
void Search_window(sdk::host& host_object)
{
    while (t5running == true)
    {
        margins window = find_game_window_and_resolution(L"csgo.exe");

        if (window.get_bottom_height() != 0)
            screen_capture_part(window, "test.bmp");
        else
            std::cout << "[GAME WINDOW] NOT OPEN!" << std::endl;

        ///OCR TEST
        if (!ocr::execute_ocr()) 
        {
            host_object.set_flag(sdk::flags::OCR);
            canceloutput = true;
        }

        m_lock.lock();
        t5running = false;
        t6running = true;
        m_lock.unlock(); 
    }
}

//Thread 6
void MemMod_Check(sdk::host& host_object) {

    while (t6running == true)
    {
        //cheatengine-x86_64.exe
        //elo.exe
        //ida64.exe
        //ReClass.NET.exe
        sdk::process object(L"ida64.exe");

        std::cout << "[MEMORY] Searching for strings in processmemory:" << std::endl;
        auto memresults = 0;
        auto moduleresults = 0;

        for (auto itt : search_file())
        {
            std::cout << "[MEMORY] Searching for \"" << itt << "\"" << std::endl;
            memresults = memory::search_for_string(object.get_id(), itt.data(), itt.length());
            std::cout << "[MEMORY] Found " << std::dec << memresults << " references." << std::endl << std::endl;
        }

        if (memresults) {
            host_object.set_flag(sdk::flags::Memory);
            canceloutput = true;
        }

        std::cout << "[MEMORY] Searching loaded DLL's" << std::endl;
        moduleresults = memory::detect_modules(object.get_id());
        std::cout << "[MEMORY] Found " << std::dec << moduleresults << " references." << std::endl << std::endl;

        if (moduleresults) {
            host_object.set_flag(sdk::flags::Module);
            canceloutput = true;
        }

        m_lock.lock();
        t6running = false;
        t1running = true;
        m_lock.unlock();   
    }
}


int main()
{
    SetConsoleTitleA("DAPI - ACAR");

    std::cout << "DAPI - ACAR" << std::endl;
    std::cout << "  _      _      _" << std::endl;
    std::cout << ">(.)__ <(.)__ =(.)__" << std::endl;
    std::cout << " (___/  (___/  (___/  \n\n" << std::endl;

    //Fabian Folger
    // Get OS and partition
    std::cout << "[HWID] OS/PARTITION: " << std::endl;
    WmiQueryResult OS = queryAndPrintResult(L"SELECT * FROM Win32_OperatingSystem", L"Name");

    //Anton Halder
    for (const auto& item : OS.ResultList)
        std::wcout << item << std::endl;

    //Fabian Folger
    //Get OS buildver
    DWORD minver = 0;
    DWORD buildver = GetRealOSVersion(minver);
    std::wcout << "[HWID] Build: " << buildver << "." << minver << std::endl;

    //Fabian Folger
    // Get id of CPU
    std::cout << "[HWID] CPU ID: " << std::endl;
    WmiQueryResult CPU = queryAndPrintResult(L"SELECT ProcessorId FROM Win32_Processor", L"ProcessorId");

    //Anton Halder
    for (const auto& item : CPU.ResultList)
        std::wcout << item << std::endl;

    //Fabian Folger
    // Get serial number of Hard Drive
    std::cout << "[HWID] HDD's/SSD's: " << std::endl;
    WmiQueryResult HDD_SSD = queryAndPrintResult(L"SELECT SerialNumber FROM Win32_PhysicalMedia", L"SerialNumber");

    //Anton Halder
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

    //write_file(create_flagged_hwid_string(sdk::wstring_to_string(CPU.ResultList.at(0)), hardrives)); //TEMP to write hwdi into file for showcase
    
    /*if (open_file(sdk::wstring_to_string(CPU.ResultList.at(0)), hard_drives) == -2)
    {
        std::cout << "[MAIN] Terminating game..." << std::endl;
        kill_process_by_name(game);
        return 0;
    }*/
   
    //Christoph Sommer
    //Threads
   while (true)
   {
        //Hackerconsole
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 10); //10 is green
        
        std::thread t1(Process_loop);
        std::thread t2(Is_protected_game_open_and_killable);
        std::thread t3(String_Detection, std::ref(host_object));
        std::thread t4(Get_open_window_titles_loop, std::ref(host_object));
        std::thread t5(Search_window, std::ref(host_object));
        std::thread t6(MemMod_Check, std::ref(host_object));

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();

        /*write_file(create_flagged_hwid_string(sdk::wstring_to_string(CPU.ResultList.at(0)), hardrives));
        kill_process_by_name(game);*/
    }

    system("pause");

    return 0;
}