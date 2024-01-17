#include "MyForm.h"
using namespace System;
using namespace System::Windows::Forms;
[STAThread]
//void main(array<String^>^ args) {
int main(){
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	kr::MyForm form;
	Application::Run(% form);
	return 0;
}
