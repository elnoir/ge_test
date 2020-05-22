#include "pch.h"

using namespace System;

#include "MainWindow.h"

using namespace System::Windows::Forms;

[STAThread]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew annWinForm::MainWindow());
	return 0;
}