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
    auto dbResult = mWrapper->setDb(cTrainImagePathBox->Text, cTrainLabelPathBox->Text, cTestImagePathBox->Text, cTestLabelPathBox->Text);
    if (dbResult)
    {
        mWrapper->configureNetwork();
        mTimer->Enabled = true;
    }
    else
    {
        String^ message = gcnew String("Unable to load data from the files, please check if the path or the files are correct.");
        String^ caption = gcnew String("Error");

        System::Windows::Forms::MessageBoxButtons buttons = System::Windows::Forms::MessageBoxButtons::OK;

        System::Windows::Forms::MessageBox::Show(message, caption, buttons);
    }
}

inline System::Void MainWindow::cStartTraining_Click(System::Object^ sender, System::EventArgs^ e) {
    cTrainGrid->Rows->Clear();
    cConfMatrix->Rows->Clear();
    mWrapper->startTraining();
}

inline System::Void MainWindow::cTrainSnapshot_Click(System::Object^ sender, System::EventArgs^ e) {
    cTrainGrid->Rows->Clear();
    mWrapper->createSnapshot();
}

inline System::Void MainWindow::cPauseTraining_Click(System::Object^ sender, System::EventArgs^ e) {
    mWrapper->stopTraining();
}

void MainWindow::OnConfusionMatrixArrived(ManagedConfusionMatrix result)
{
    cConfMatrix->Rows->Clear();
    cConfMatrix->Rows->Add(10);
    for(int i = 0; i < result->Length; ++i)
    {
        cConfMatrix->Rows[i]->HeaderCell->Value = System::Convert::ToString(i);
        for (int j = 0; j < result[i]->Length; ++j)
        {
            cConfMatrix->Rows[i]->Cells[j]->Value = System::Convert::ToString(result[i][j]);
            if (i == j)
            {
                cConfMatrix->Rows[i]->Cells[j]->Style->BackColor = System::Drawing::Color::DarkOliveGreen;
            }
            else if (result[i][j] > 0)
            {
                cConfMatrix->Rows[i]->Cells[j]->Style->BackColor = System::Drawing::Color::DarkOrange;
            }
        }
    }
}

void MainWindow::OnTestStatusUpdate(int result)
{
    cTestProgress->Value = result;
}

void MainWindow::TimerEventProcessor(Object^ myObject, EventArgs^ myEventArgs)
{
    mWrapper->checkMessage();
}

void MainWindow::OnTrainSnapshotUpdate(TrainSnapshot^ trainSnapshot)
{
    cTrainGrid->Rows->Clear();
    for (int i = 0; i < trainSnapshot->mImageIndex->Length; ++i)
    {
        cTrainGrid->Rows->Add();
        Int32^ prediction = trainSnapshot->mImagePrediction[i];
        Int32^ label = trainSnapshot->mImageLabel[i];

        auto color = (prediction->CompareTo(label) == 0 ) ? System::Drawing::Color::DarkOliveGreen : System::Drawing::Color::DarkOrange;


        cTrainGrid->Rows[i]->Cells["imageNumber"]->Value = trainSnapshot->mImageIndex[i].ToString();
        cTrainGrid->Rows[i]->Cells["imagePrediction"]->Value = prediction->ToString();
        cTrainGrid->Rows[i]->Cells["imagePrediction"]->Style->BackColor = color;
        cTrainGrid->Rows[i]->Cells["imageLabel"]->Value = label->ToString();
        cTrainGrid->Rows[i]->Cells["imageLabel"]->Style->BackColor = color;
    }
}

inline void MainWindow::TrainGridSelectionChanged(Object^ sender, EventArgs^ e)
{
    if (cTrainGrid->SelectedRows->Count > 0)
    {
        auto number = System::Convert::ToInt32(cTrainGrid->SelectedRows[0]->Cells["imageNumber"]->Value);
        auto bitmap = mWrapper->getBitmap(number);
        cPictureBox->Image = bitmap;
        cPictureBox->Width = bitmap->Width;
        cPictureBox->Height = bitmap->Height;
    }
}

inline System::Void MainWindow::cStopTraining_Click(System::Object^ sender, System::EventArgs^ e) {
    mWrapper->stopTraining();
}

inline System::Void MainWindow::cStartTesting_Click(System::Object^ sender, System::EventArgs^ e) {
    mWrapper->startTest();
}

}