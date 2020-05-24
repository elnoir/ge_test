#include "pch.h"
#include "MainWindow.h"

namespace annWinForm {

bool browseForFileName(String^ dialogName, System::Windows::Forms::TextBox^% textBox)
{
    System::Windows::Forms::OpenFileDialog^ dialog = gcnew System::Windows::Forms::OpenFileDialog();
    dialog->Title = dialogName;
    dialog->Filter = "All Files (*.*)|*.*";

    DialogResult result = dialog->ShowDialog();

    if (result == DialogResult::OK)
    {
        textBox->Text = dialog->FileName;
    }

    return result == DialogResult::OK;
}

inline System::Void MainWindow::cTrainImageBrowse_Click(System::Object^ sender, System::EventArgs^ e) {
    browseForFileName("Select training image file...", this->cTrainImagePathBox);
}

inline System::Void MainWindow::cTrainLabelBrowse_Click(System::Object^ sender, System::EventArgs^ e) {
    browseForFileName("Select training label file...", this->cTrainLabelPathBox);
}

inline System::Void MainWindow::cTestImageBrowse_Click(System::Object^ sender, System::EventArgs^ e) {
    browseForFileName("Select test image file...", this->cTestImagePathBox);
}

inline System::Void MainWindow::cTestLabelBrowse_Click(System::Object^ sender, System::EventArgs^ e) {
    browseForFileName("Select test label file...", this->cTestLabelPathBox);
}


inline System::Void MainWindow::cConfigureNetwork_Click(System::Object^ sender, System::EventArgs^ e) {
    mWrapper->setDb(cTrainImagePathBox->Text, cTrainLabelPathBox->Text, cTestImagePathBox->Text, cTestLabelPathBox->Text);
    mWrapper->configureNetwork();
}

inline System::Void MainWindow::cStartTraining_Click(System::Object^ sender, System::EventArgs^ e) {
    mWrapper->startTraining();
}

inline System::Void MainWindow::cPauseTraining_Click(System::Object^ sender, System::EventArgs^ e) {
    mWrapper->stopTraining();
}

inline System::Void MainWindow::cStopTraining_Click(System::Object^ sender, System::EventArgs^ e) {
    mWrapper->stopTraining();
}

inline System::Void MainWindow::cStartTesting_Click(System::Object^ sender, System::EventArgs^ e) {
    mWrapper->startTest();
}

}