#include "MyForm.h"

//#include <Windows.h>
//#include <iostream>

using namespace System;
using namespace System::Windows::Forms;

void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	ACARGUI::MyForm form;
	Application::Run(% form);
}