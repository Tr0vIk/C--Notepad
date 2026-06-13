#pragma once

#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <msclr/marshal_cppstd.h>

namespace CppNotepadCompiler {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Diagnostics;
    using namespace System::IO;
    using namespace System::Text;

    /// <summary>
    /// Main form - Notepad with C++ compiler
    /// </summary>
    public ref class MainForm : public System::Windows::Forms::Form
    {
    public:
        MainForm(void)
        {
            InitializeComponent();
            SetDefaultCode();
        }

    protected:
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        // Interface components
        System::ComponentModel::Container^ components;
        System::Windows::Forms::MenuStrip^ mainMenu;
        System::Windows::Forms::ToolStripMenuItem^ fileMenu;
        System::Windows::Forms::ToolStripMenuItem^ newFileMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ openFileMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ saveFileMenuItem;
        System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
        System::Windows::Forms::ToolStripMenuItem^ exitMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ compileMenu;
        System::Windows::Forms::ToolStripMenuItem^ runMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ compileAndRunMenuItem;
        System::Windows::Forms::ToolStripMenuItem^ helpMenu;
        System::Windows::Forms::ToolStripMenuItem^ aboutMenuItem;

        System::Windows::Forms::SplitContainer^ splitContainer;
        System::Windows::Forms::RichTextBox^ editor;
        System::Windows::Forms::RichTextBox^ output;

        System::Windows::Forms::ToolStrip^ toolBar;
        System::Windows::Forms::ToolStripButton^ compileBtn;
        System::Windows::Forms::ToolStripButton^ runBtn;
        System::Windows::Forms::ToolStripButton^ compileRunBtn;
        System::Windows::Forms::ToolStripSeparator^ toolStripSeparator2;
        System::Windows::Forms::ToolStripButton^ clearBtn;
        System::Windows::Forms::ToolStripComboBox^ compilerCombo;
        System::Windows::Forms::ToolStripLabel^ statusLabel;

        System::Windows::Forms::StatusStrip^ statusStrip;
        System::Windows::Forms::ToolStripStatusLabel^ statusStripLabel;

        System::Windows::Forms::SaveFileDialog^ saveFileDialog;
        System::Windows::Forms::OpenFileDialog^ openFileDialog;

        String^ currentFilePath;
        String^ compilerPath;

        // Helper method for encoding conversion
        String^ ReadFileWithEncoding(String^ filePath)
        {
            array<Byte>^ bytes = File::ReadAllBytes(filePath);

            // Check for UTF-8 with BOM
            if (bytes->Length >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
            {
                return Encoding::UTF8->GetString(bytes, 3, bytes->Length - 3);
            }

            // Try UTF-8 without BOM
            try
            {
                return Encoding::UTF8->GetString(bytes);
            }
            catch (...)
            {
                // If not UTF-8, use ANSI (1251 for Russian, but we'll use UTF8)
                return Encoding::UTF8->GetString(bytes);
            }
        }

        void WriteFileWithEncoding(String^ filePath, String^ content)
        {
            // Save as UTF-8 with BOM for full Unicode support
            array<Byte>^ bytes = Encoding::UTF8->GetBytes(content);
            array<Byte>^ bom = { 0xEF, 0xBB, 0xBF };

            array<Byte>^ finalBytes = gcnew array<Byte>(bom->Length + bytes->Length);
            bom->CopyTo(finalBytes, 0);
            bytes->CopyTo(finalBytes, bom->Length);

            File::WriteAllBytes(filePath, finalBytes);
        }

    private:
        /// <summary>
        /// Initialize all form components
        /// </summary>
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->mainMenu = gcnew System::Windows::Forms::MenuStrip();
            this->fileMenu = gcnew System::Windows::Forms::ToolStripMenuItem();
            this->newFileMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem();
            this->openFileMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem();
            this->saveFileMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem();
            this->toolStripSeparator1 = gcnew System::Windows::Forms::ToolStripSeparator();
            this->exitMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem();
            this->compileMenu = gcnew System::Windows::Forms::ToolStripMenuItem();
            this->runMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem();
            this->compileAndRunMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem();
            this->helpMenu = gcnew System::Windows::Forms::ToolStripMenuItem();
            this->aboutMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem();
            this->toolBar = gcnew System::Windows::Forms::ToolStrip();
            this->compileBtn = gcnew System::Windows::Forms::ToolStripButton();
            this->runBtn = gcnew System::Windows::Forms::ToolStripButton();
            this->compileRunBtn = gcnew System::Windows::Forms::ToolStripButton();
            this->toolStripSeparator2 = gcnew System::Windows::Forms::ToolStripSeparator();
            this->clearBtn = gcnew System::Windows::Forms::ToolStripButton();
            this->compilerCombo = gcnew System::Windows::Forms::ToolStripComboBox();
            this->statusLabel = gcnew System::Windows::Forms::ToolStripLabel();
            this->splitContainer = gcnew System::Windows::Forms::SplitContainer();
            this->editor = gcnew System::Windows::Forms::RichTextBox();
            this->output = gcnew System::Windows::Forms::RichTextBox();
            this->statusStrip = gcnew System::Windows::Forms::StatusStrip();
            this->statusStripLabel = gcnew System::Windows::Forms::ToolStripStatusLabel();
            this->saveFileDialog = gcnew System::Windows::Forms::SaveFileDialog();
            this->openFileDialog = gcnew System::Windows::Forms::OpenFileDialog();
            this->mainMenu->SuspendLayout();
            this->toolBar->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer))->BeginInit();
            this->splitContainer->Panel1->SuspendLayout();
            this->splitContainer->Panel2->SuspendLayout();
            this->splitContainer->SuspendLayout();
            this->statusStrip->SuspendLayout();
            this->SuspendLayout();

            // mainMenu
            this->mainMenu->Items->AddRange(gcnew cli::array<System::Windows::Forms::ToolStripMenuItem^>(3) {
                this->fileMenu, this->compileMenu, this->helpMenu
            });
            this->mainMenu->Location = System::Drawing::Point(0, 0);
            this->mainMenu->Name = L"mainMenu";
            this->mainMenu->Size = System::Drawing::Size(984, 24);
            this->mainMenu->TabIndex = 0;
            this->mainMenu->Text = L"menuStrip1";

            // fileMenu
            this->fileMenu->DropDownItems->AddRange(gcnew cli::array<System::Windows::Forms::ToolStripItem^>(5) {
                this->newFileMenuItem, this->openFileMenuItem, this->saveFileMenuItem,
                    this->toolStripSeparator1, this->exitMenuItem
            });
            this->fileMenu->Name = L"fileMenu";
            this->fileMenu->Size = System::Drawing::Size(48, 20);
            this->fileMenu->Text = L"File";

            // newFileMenuItem
            this->newFileMenuItem->Name = L"newFileMenuItem";
            this->newFileMenuItem->ShortcutKeys = (System::Windows::Forms::Keys)((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
            this->newFileMenuItem->Size = System::Drawing::Size(180, 22);
            this->newFileMenuItem->Text = L"New";
            this->newFileMenuItem->Click += gcnew System::EventHandler(this, &MainForm::NewFile_Click);

            // openFileMenuItem
            this->openFileMenuItem->Name = L"openFileMenuItem";
            this->openFileMenuItem->ShortcutKeys = (System::Windows::Forms::Keys)((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
            this->openFileMenuItem->Size = System::Drawing::Size(180, 22);
            this->openFileMenuItem->Text = L"Open...";
            this->openFileMenuItem->Click += gcnew System::EventHandler(this, &MainForm::OpenFile_Click);

            // saveFileMenuItem
            this->saveFileMenuItem->Name = L"saveFileMenuItem";
            this->saveFileMenuItem->ShortcutKeys = (System::Windows::Forms::Keys)((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
            this->saveFileMenuItem->Size = System::Drawing::Size(180, 22);
            this->saveFileMenuItem->Text = L"Save";
            this->saveFileMenuItem->Click += gcnew System::EventHandler(this, &MainForm::SaveFile_Click);

            // toolStripSeparator1
            this->toolStripSeparator1->Name = L"toolStripSeparator1";
            this->toolStripSeparator1->Size = System::Drawing::Size(177, 6);

            // exitMenuItem
            this->exitMenuItem->Name = L"exitMenuItem";
            this->exitMenuItem->Size = System::Drawing::Size(180, 22);
            this->exitMenuItem->Text = L"Exit";
            this->exitMenuItem->Click += gcnew System::EventHandler(this, &MainForm::Exit_Click);

            // compileMenu
            this->compileMenu->DropDownItems->AddRange(gcnew cli::array<System::Windows::Forms::ToolStripItem^>(2) {
                this->runMenuItem, this->compileAndRunMenuItem
            });
            this->compileMenu->Name = L"compileMenu";
            this->compileMenu->Size = System::Drawing::Size(88, 20);
            this->compileMenu->Text = L"Compile";

            // runMenuItem
            this->runMenuItem->Name = L"runMenuItem";
            this->runMenuItem->ShortcutKeys = (System::Windows::Forms::Keys)((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::R));
            this->runMenuItem->Size = System::Drawing::Size(200, 22);
            this->runMenuItem->Text = L"Run without compilation";
            this->runMenuItem->Click += gcnew System::EventHandler(this, &MainForm::RunOnly_Click);

            // compileAndRunMenuItem
            this->compileAndRunMenuItem->Name = L"compileAndRunMenuItem";
            this->compileAndRunMenuItem->ShortcutKeys = (System::Windows::Forms::Keys)((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::F5));
            this->compileAndRunMenuItem->Size = System::Drawing::Size(200, 22);
            this->compileAndRunMenuItem->Text = L"Compile and run";
            this->compileAndRunMenuItem->Click += gcnew System::EventHandler(this, &MainForm::CompileAndRun_Click);

            // helpMenu
            this->helpMenu->DropDownItems->AddRange(gcnew cli::array<System::Windows::Forms::ToolStripItem^>(1) {
                this->aboutMenuItem
            });
            this->helpMenu->Name = L"helpMenu";
            this->helpMenu->Size = System::Drawing::Size(68, 20);
            this->helpMenu->Text = L"Help";

            // aboutMenuItem
            this->aboutMenuItem->Name = L"aboutMenuItem";
            this->aboutMenuItem->Size = System::Drawing::Size(149, 22);
            this->aboutMenuItem->Text = L"About";
            this->aboutMenuItem->Click += gcnew System::EventHandler(this, &MainForm::About_Click);

            // toolBar
            this->toolBar->Items->AddRange(gcnew cli::array<System::Windows::Forms::ToolStripItem^>(7) {
                this->compileBtn, this->runBtn, this->compileRunBtn,
                    this->toolStripSeparator2, this->clearBtn, this->compilerCombo, this->statusLabel
            });
            this->toolBar->Location = System::Drawing::Point(0, 24);
            this->toolBar->Name = L"toolBar";
            this->toolBar->Size = System::Drawing::Size(984, 25);
            this->toolBar->TabIndex = 1;

            // compileBtn
            this->compileBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->compileBtn->Text = L"Compile";
            this->compileBtn->ToolTipText = L"Compile only (Ctrl+F7)";
            this->compileBtn->Click += gcnew System::EventHandler(this, &MainForm::CompileOnly_Click);

            // runBtn
            this->runBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->runBtn->Text = L"Run";
            this->runBtn->ToolTipText = L"Run last compiled program";
            this->runBtn->Click += gcnew System::EventHandler(this, &MainForm::RunOnly_Click);

            // compileRunBtn
            this->compileRunBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->compileRunBtn->Text = L"Compile and run";
            this->compileRunBtn->ToolTipText = L"Compile and run (Ctrl+F5)";
            this->compileRunBtn->Click += gcnew System::EventHandler(this, &MainForm::CompileAndRun_Click);

            // toolStripSeparator2
            this->toolStripSeparator2->Name = L"toolStripSeparator2";
            this->toolStripSeparator2->Size = System::Drawing::Size(6, 25);

            // clearBtn
            this->clearBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->clearBtn->Text = L"Clear output";
            this->clearBtn->Click += gcnew System::EventHandler(this, &MainForm::ClearOutput_Click);

            // compilerCombo
            this->compilerCombo->Items->AddRange(gcnew cli::array<System::Object^>(3) {
                L"g++ (MinGW)", L"MSVC (cl.exe)", L"Auto detect"
            });
            this->compilerCombo->SelectedIndex = 0;
            this->compilerCombo->Size = System::Drawing::Size(150, 25);
            this->compilerCombo->ToolTipText = L"Select compiler";

            // statusLabel
            this->statusLabel->Name = L"statusLabel";
            this->statusLabel->Size = System::Drawing::Size(200, 22);
            this->statusLabel->Text = L"Ready";

            // splitContainer
            this->splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer->Location = System::Drawing::Point(0, 49);
            this->splitContainer->Name = L"splitContainer";
            this->splitContainer->Orientation = System::Windows::Forms::Orientation::Horizontal;

            // splitContainer.Panel1
            this->splitContainer->Panel1->Controls->Add(this->editor);
            // splitContainer.Panel2
            this->splitContainer->Panel2->Controls->Add(this->output);
            this->splitContainer->Size = System::Drawing::Size(984, 561);
            this->splitContainer->SplitterDistance = 350;
            this->splitContainer->TabIndex = 2;

            // editor
            this->editor->Dock = System::Windows::Forms::DockStyle::Fill;
            this->editor->Font = gcnew System::Drawing::Font(L"Consolas", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point);
            this->editor->Location = System::Drawing::Point(0, 0);
            this->editor->Name = L"editor";
            this->editor->Size = System::Drawing::Size(984, 350);
            this->editor->TabIndex = 0;
            this->editor->Text = L"";

            // output
            this->output->BackColor = System::Drawing::Color::FromArgb(30, 30, 30);
            this->output->Dock = System::Windows::Forms::DockStyle::Fill;
            this->output->Font = gcnew System::Drawing::Font(L"Consolas", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point);
            this->output->ForeColor = System::Drawing::Color::LightGreen;
            this->output->Location = System::Drawing::Point(0, 0);
            this->output->Name = L"output";
            this->output->ReadOnly = true;
            this->output->Size = System::Drawing::Size(984, 207);
            this->output->TabIndex = 0;
            this->output->Text = L"";

            // statusStrip
            this->statusStrip->Items->AddRange(gcnew cli::array<System::Windows::Forms::ToolStripItem^>(1) {
                this->statusStripLabel
            });
            this->statusStrip->Location = System::Drawing::Point(0, 610);
            this->statusStrip->Name = L"statusStrip";
            this->statusStrip->Size = System::Drawing::Size(984, 22);
            this->statusStrip->TabIndex = 3;
            this->statusStrip->Text = L"statusStrip1";

            // statusStripLabel
            this->statusStripLabel->Name = L"statusStripLabel";
            this->statusStripLabel->Size = System::Drawing::Size(120, 17);
            this->statusStripLabel->Text = L"Ready to work";

            // saveFileDialog
            this->saveFileDialog->DefaultExt = L"cpp";
            this->saveFileDialog->Filter = L"C++ files (*.cpp)|*.cpp|Header files (*.h)|*.h|All files (*.*)|*.*";
            this->saveFileDialog->AddExtension = true;

            // openFileDialog
            this->openFileDialog->DefaultExt = L"cpp";
            this->openFileDialog->Filter = L"C++ files (*.cpp)|*.cpp|Header files (*.h)|*.h|All files (*.*)|*.*";

            // MainForm
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(984, 632);
            this->Controls->Add(this->splitContainer);
            this->Controls->Add(this->toolBar);
            this->Controls->Add(this->mainMenu);
            this->Controls->Add(this->statusStrip);
            this->MainMenuStrip = this->mainMenu;
            this->Name = L"MainForm";
            this->Text = L"Notepad with C++ Compiler";
            this->mainMenu->ResumeLayout(false);
            this->mainMenu->PerformLayout();
            this->toolBar->ResumeLayout(false);
            this->toolBar->PerformLayout();
            this->splitContainer->Panel1->ResumeLayout(false);
            this->splitContainer->Panel2->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer))->EndInit();
            this->splitContainer->ResumeLayout(false);
            this->statusStrip->ResumeLayout(false);
            this->statusStrip->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();
        }

        void SetDefaultCode()
        {
            editor->Text =
                "#include <iostream>\r\n" +
                "#include <string>\r\n" +
                "using namespace std;\r\n" +
                "\r\n" +
                "int main() {\r\n" +
                "    cout << \"Hello, C++ Programmer!\" << endl;\r\n" +
                "    cout << \"Enter your name: \";\r\n" +
                "    string name;\r\n" +
                "    getline(cin, name);\r\n" +
                "    cout << \"Welcome, \" << name << \"!\" << endl;\r\n" +
                "    return 0;\r\n" +
                "}\r\n";
        }

        String^ GetCompilerCommand()
        {
            switch (compilerCombo->SelectedIndex)
            {
            case 0: return "g++";
            case 1: return "cl.exe";
            default: return "g++";
            }
        }

        bool CompileCode()
        {
            String^ tempFile = Path::GetTempFileName();
            String^ sourceFile = Path::ChangeExtension(tempFile, ".cpp");
            String^ exeFile = Path::ChangeExtension(tempFile, ".exe");

            File::Delete(tempFile);

            // Save with correct encoding
            WriteFileWithEncoding(sourceFile, editor->Text);

            String^ compiler = GetCompilerCommand();
            String^ compileArgs = String::Format("{0} \"{1}\" -o \"{2}\" 2>&1", compiler, sourceFile, exeFile);

            output->AppendText("Compiling...\r\n");
            output->AppendText(String::Format("Command: {0}\r\n", compileArgs));

            Process^ process = gcnew Process();
            process->StartInfo->FileName = compiler;
            process->StartInfo->Arguments = String::Format("\"{0}\" -o \"{1}\"", sourceFile, exeFile);
            process->StartInfo->UseShellExecute = false;
            process->StartInfo->RedirectStandardOutput = true;
            process->StartInfo->RedirectStandardError = true;
            process->StartInfo->CreateNoWindow = true;
            process->StartInfo->StandardOutputEncoding = Encoding::UTF8;
            process->StartInfo->StandardErrorEncoding = Encoding::UTF8;

            try
            {
                process->Start();
                String^ outputText = process->StandardOutput->ReadToEnd();
                String^ error = process->StandardError->ReadToEnd();
                process->WaitForExit(10000);

                if (process->ExitCode == 0 && error->Length == 0)
                {
                    this->output->AppendText("Compilation successful!\r\n");
                    this->output->AppendText("----------------------------------------\r\n");
                    currentFilePath = exeFile;
                    statusStripLabel->Text = "Compilation successful";
                    return true;
                }
                else
                {
                    this->output->AppendText("Compilation error:\r\n");
                    if (error->Length > 0)
                        this->output->AppendText(error);
                    if (outputText->Length > 0)
                        this->output->AppendText(outputText);
                    statusStripLabel->Text = "Compilation error";
                    return false;
                }
            }
            catch (Exception^ ex)
            {
                this->output->AppendText(String::Format("Error: {0}\r\n", ex->Message));
                this->output->AppendText("Make sure the compiler is installed and available in PATH\r\n");
                statusStripLabel->Text = "Compiler launch error";
                return false;
            }
            finally
            {
                if (File::Exists(sourceFile))
                    File::Delete(sourceFile);
            }
        }

        void RunExecutable(String^ exePath)
        {
            if (!File::Exists(exePath))
            {
                output->AppendText("Executable file not found. Please compile the program first.\r\n");
                return;
            }

            output->AppendText("\r\nRunning program:\r\n");
            output->AppendText("========================================\r\n");

            Process^ process = gcnew Process();
            process->StartInfo->FileName = exePath;
            process->StartInfo->UseShellExecute = false;
            process->StartInfo->RedirectStandardOutput = true;
            process->StartInfo->RedirectStandardError = true;
            process->StartInfo->CreateNoWindow = true;
            process->StartInfo->StandardOutputEncoding = Encoding::UTF8;
            process->StartInfo->StandardErrorEncoding = Encoding::UTF8;

            try
            {
                process->Start();
                String^ result = process->StandardOutput->ReadToEnd();
                String^ error = process->StandardError->ReadToEnd();
                process->WaitForExit(5000);

                output->AppendText(result);
                if (error->Length > 0)
                    output->AppendText("STDERR:\r\n" + error);

                output->AppendText("\r\n========================================\r\n");
                output->AppendText(String::Format("Program exited with code: {0}\r\n", process->ExitCode));
                statusStripLabel->Text = "Program executed";
            }
            catch (Exception^ ex)
            {
                output->AppendText(String::Format("Run error: {0}\r\n", ex->Message));
            }
        }

    private:
        // Event handlers
        void NewFile_Click(Object^ sender, EventArgs^ e)
        {
            if (MessageBox::Show("Save changes?", "New File",
                MessageBoxButtons::YesNo, MessageBoxIcon::Question) == (System::Windows::Forms::DialogResult)6)
            {
                SaveFile_Click(sender, e);
            }
            editor->Clear();
            SetDefaultCode();
            currentFilePath = nullptr;
        }

        void OpenFile_Click(Object^ sender, EventArgs^ e)
        {
            if (openFileDialog->ShowDialog() == (System::Windows::Forms::DialogResult)1)
            {
                try
                {
                    editor->Text = ReadFileWithEncoding(openFileDialog->FileName);
                    currentFilePath = openFileDialog->FileName;
                    statusStripLabel->Text = String::Format("Opened: {0}", Path::GetFileName(currentFilePath));
                }
                catch (Exception^ ex)
                {
                    MessageBox::Show("Error opening file: " + ex->Message);
                }
            }
        }

        void SaveFile_Click(Object^ sender, EventArgs^ e)
        {
            if (currentFilePath == nullptr || !File::Exists(currentFilePath))
            {
                if (saveFileDialog->ShowDialog() == (System::Windows::Forms::DialogResult)1)
                    currentFilePath = saveFileDialog->FileName;
                else
                    return;
            }

            try
            {
                WriteFileWithEncoding(currentFilePath, editor->Text);
                statusStripLabel->Text = String::Format("Saved: {0}", Path::GetFileName(currentFilePath));
            }
            catch (Exception^ ex)
            {
                MessageBox::Show("Error saving file: " + ex->Message);
            }
        }

        void Exit_Click(Object^ sender, EventArgs^ e)
        {
            Application::Exit();
        }

        void CompileOnly_Click(Object^ sender, EventArgs^ e)
        {
            output->Clear();
            CompileCode();
        }

        void RunOnly_Click(Object^ sender, EventArgs^ e)
        {
            if (currentFilePath != nullptr && File::Exists(currentFilePath))
                RunExecutable(currentFilePath);
            else
                output->AppendText("No compiled program available. Use 'Compile and run'.\r\n");
        }

        void CompileAndRun_Click(Object^ sender, EventArgs^ e)
        {
            output->Clear();
            if (CompileCode())
                RunExecutable(currentFilePath);
        }

        void ClearOutput_Click(Object^ sender, EventArgs^ e)
        {
            output->Clear();
        }

        void About_Click(Object^ sender, EventArgs^ e)
        {
            MessageBox::Show(
                "Notepad with C++ Compiler\r\nVersion 1.0\r\n\r\n" +
                "Features:\r\n" +
                "- C++ code editing\r\n" +
                "- Compilation via g++/MSVC\r\n" +
                "- Run programs\r\n" +
                "- stdin input support\r\n\r\n" +
                "Requirements:\r\n" +
                "- C++ compiler installed and in PATH\r\n" +
                "- For g++: MinGW or Cygwin\r\n" +
                "- For MSVC: Visual Studio Build Tools\r\n",
                "About",
                MessageBoxButtons::OK,
                MessageBoxIcon::Information
            );
        }
    };
}