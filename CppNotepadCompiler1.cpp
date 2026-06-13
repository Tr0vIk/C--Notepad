#include "pch.h"
#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args)
{
    // Включение визуальных стилей Windows
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Создание и запуск главной формы
    CppNotepadCompiler::MainForm form;
    Application::Run(% form);

    return 0;
}