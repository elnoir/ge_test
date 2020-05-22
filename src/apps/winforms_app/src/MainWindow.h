#pragma once

namespace annWinForm {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			InitializeComponent();
		}

	protected:
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		System::ComponentModel::Container ^components;

	private: System::Windows::Forms::Button^ cTrainImageBrowse;
	private: System::Windows::Forms::Button^ cTrainLabelBrowse;
	private: System::Windows::Forms::Button^ cTestLabelBrowse;
	private: System::Windows::Forms::Button^ cTestImageBrowse;

	private: System::Windows::Forms::TextBox^ cTrainImagePathBox;
	private: System::Windows::Forms::TextBox^ cTrainLabelPathBox;
	private: System::Windows::Forms::TextBox^ cTestImagePathBox;
	private: System::Windows::Forms::TextBox^ cTestLabelPathBox;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Button^ cConfigureNetwork;

	private: System::Windows::Forms::Button^ cStartTraining;
	private: System::Windows::Forms::Button^ cPauseTraining;
	private: System::Windows::Forms::Button^ cStopTraining;
	private: System::Windows::Forms::Button^ cStartTesting;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->cTrainImageBrowse = (gcnew System::Windows::Forms::Button());
			this->cTrainLabelBrowse = (gcnew System::Windows::Forms::Button());
			this->cTestLabelBrowse = (gcnew System::Windows::Forms::Button());
			this->cTestImageBrowse = (gcnew System::Windows::Forms::Button());
			this->cTrainImagePathBox = (gcnew System::Windows::Forms::TextBox());
			this->cTrainLabelPathBox = (gcnew System::Windows::Forms::TextBox());
			this->cTestImagePathBox = (gcnew System::Windows::Forms::TextBox());
			this->cTestLabelPathBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->cConfigureNetwork = (gcnew System::Windows::Forms::Button());
			this->cStartTraining = (gcnew System::Windows::Forms::Button());
			this->cPauseTraining = (gcnew System::Windows::Forms::Button());
			this->cStopTraining = (gcnew System::Windows::Forms::Button());
			this->cStartTesting = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			//
			// cTrainImageBrowse
			//
			this->cTrainImageBrowse->Location = System::Drawing::Point(943, 13);
			this->cTrainImageBrowse->Name = L"cTrainImageBrowse";
			this->cTrainImageBrowse->Size = System::Drawing::Size(177, 26);
			this->cTrainImageBrowse->TabIndex = 0;
			this->cTrainImageBrowse->Text = L"browse...";
			this->cTrainImageBrowse->UseVisualStyleBackColor = true;
			this->cTrainImageBrowse->Click += gcnew System::EventHandler(this, &MainWindow::cTrainImageBrowse_Click);
			//
			// cTrainLabelBrowse
			//
			this->cTrainLabelBrowse->Location = System::Drawing::Point(943, 46);
			this->cTrainLabelBrowse->Name = L"cTrainLabelBrowse";
			this->cTrainLabelBrowse->Size = System::Drawing::Size(177, 26);
			this->cTrainLabelBrowse->TabIndex = 1;
			this->cTrainLabelBrowse->Text = L"browse...";
			this->cTrainLabelBrowse->UseVisualStyleBackColor = true;
			this->cTrainLabelBrowse->Click += gcnew System::EventHandler(this, &MainWindow::cTrainLabelBrowse_Click);
			//
			// cTestLabelBrowse
			//
			this->cTestLabelBrowse->Location = System::Drawing::Point(943, 111);
			this->cTestLabelBrowse->Name = L"cTestLabelBrowse";
			this->cTestLabelBrowse->Size = System::Drawing::Size(177, 26);
			this->cTestLabelBrowse->TabIndex = 3;
			this->cTestLabelBrowse->Text = L"browse...";
			this->cTestLabelBrowse->UseVisualStyleBackColor = true;
			this->cTestLabelBrowse->Click += gcnew System::EventHandler(this, &MainWindow::cTestLabelBrowse_Click);
			//
			// cTestImageBrowse
			//
			this->cTestImageBrowse->Location = System::Drawing::Point(943, 78);
			this->cTestImageBrowse->Name = L"cTestImageBrowse";
			this->cTestImageBrowse->Size = System::Drawing::Size(177, 26);
			this->cTestImageBrowse->TabIndex = 2;
			this->cTestImageBrowse->Text = L"browse...";
			this->cTestImageBrowse->UseVisualStyleBackColor = true;
			this->cTestImageBrowse->Click += gcnew System::EventHandler(this, &MainWindow::cTestImageBrowse_Click);
			//
			// cTrainImagePathBox
			//
			this->cTrainImagePathBox->Location = System::Drawing::Point(173, 14);
			this->cTrainImagePathBox->Name = L"cTrainImagePathBox";
			this->cTrainImagePathBox->Size = System::Drawing::Size(764, 22);
			this->cTrainImagePathBox->TabIndex = 4;
			//
			// cTrainLabelPathBox
			//
			this->cTrainLabelPathBox->Location = System::Drawing::Point(173, 48);
			this->cTrainLabelPathBox->Name = L"cTrainLabelPathBox";
			this->cTrainLabelPathBox->Size = System::Drawing::Size(764, 22);
			this->cTrainLabelPathBox->TabIndex = 5;
			//
			// cTestImagePathBox
			//
			this->cTestImagePathBox->Location = System::Drawing::Point(173, 82);
			this->cTestImagePathBox->Name = L"cTestImagePathBox";
			this->cTestImagePathBox->Size = System::Drawing::Size(764, 22);
			this->cTestImagePathBox->TabIndex = 6;
			//
			// cTestLabelPathBox
			//
			this->cTestLabelPathBox->Location = System::Drawing::Point(173, 115);
			this->cTestLabelPathBox->Name = L"cTestLabelPathBox";
			this->cTestLabelPathBox->Size = System::Drawing::Size(764, 22);
			this->cTestLabelPathBox->TabIndex = 7;
			//
			// label1
			//
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(15, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(109, 17);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Train image file:";
			//
			// label2
			//
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(15, 51);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(101, 17);
			this->label2->TabIndex = 9;
			this->label2->Text = L"Train label file:";
			//
			// label3
			//
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(18, 86);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(100, 17);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Test image file";
			//
			// label4
			//
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(21, 119);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(92, 17);
			this->label4->TabIndex = 11;
			this->label4->Text = L"Test label file";
			//
			// cConfigureNetwork
			//
			this->cConfigureNetwork->Location = System::Drawing::Point(173, 154);
			this->cConfigureNetwork->Name = L"cConfigureNetwork";
			this->cConfigureNetwork->Size = System::Drawing::Size(312, 46);
			this->cConfigureNetwork->TabIndex = 12;
			this->cConfigureNetwork->Text = L"Configure Network";
			this->cConfigureNetwork->UseVisualStyleBackColor = true;
			this->cConfigureNetwork->Click += gcnew System::EventHandler(this, &MainWindow::cConfigureNetwork_Click);
			//
			// cStartTraining
			//
			this->cStartTraining->Location = System::Drawing::Point(24, 223);
			this->cStartTraining->Name = L"cStartTraining";
			this->cStartTraining->Size = System::Drawing::Size(243, 52);
			this->cStartTraining->TabIndex = 13;
			this->cStartTraining->Text = L"Start Training";
			this->cStartTraining->UseVisualStyleBackColor = true;
			this->cStartTraining->Click += gcnew System::EventHandler(this, &MainWindow::cStartTraining_Click);
			//
			// cPauseTraining
			//
			this->cPauseTraining->Location = System::Drawing::Point(273, 223);
			this->cPauseTraining->Name = L"cPauseTraining";
			this->cPauseTraining->Size = System::Drawing::Size(243, 52);
			this->cPauseTraining->TabIndex = 14;
			this->cPauseTraining->Text = L"Pause";
			this->cPauseTraining->UseVisualStyleBackColor = true;
			this->cPauseTraining->Click += gcnew System::EventHandler(this, &MainWindow::cPauseTraining_Click);
			//
			// cStopTraining
			//
			this->cStopTraining->Location = System::Drawing::Point(522, 223);
			this->cStopTraining->Name = L"cStopTraining";
			this->cStopTraining->Size = System::Drawing::Size(243, 52);
			this->cStopTraining->TabIndex = 15;
			this->cStopTraining->Text = L"Stop";
			this->cStopTraining->UseVisualStyleBackColor = true;
			this->cStopTraining->Click += gcnew System::EventHandler(this, &MainWindow::cStopTraining_Click);
			//
			// cStartTesting
			//
			this->cStartTesting->Location = System::Drawing::Point(24, 281);
			this->cStartTesting->Name = L"cStartTesting";
			this->cStartTesting->Size = System::Drawing::Size(243, 52);
			this->cStartTesting->TabIndex = 16;
			this->cStartTesting->Text = L"Start Testing";
			this->cStartTesting->UseVisualStyleBackColor = true;
			this->cStartTesting->Click += gcnew System::EventHandler(this, &MainWindow::cStartTesting_Click);
			//
			// MainWindow
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1132, 382);
			this->Controls->Add(this->cStartTesting);
			this->Controls->Add(this->cStopTraining);
			this->Controls->Add(this->cPauseTraining);
			this->Controls->Add(this->cStartTraining);
			this->Controls->Add(this->cConfigureNetwork);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->cTestLabelPathBox);
			this->Controls->Add(this->cTestImagePathBox);
			this->Controls->Add(this->cTrainLabelPathBox);
			this->Controls->Add(this->cTrainImagePathBox);
			this->Controls->Add(this->cTestLabelBrowse);
			this->Controls->Add(this->cTestImageBrowse);
			this->Controls->Add(this->cTrainLabelBrowse);
			this->Controls->Add(this->cTrainImageBrowse);
			this->ResumeLayout(false);
			this->PerformLayout();

			this->components = gcnew System::ComponentModel::Container();
			this->Text = L"Main Window";
			this->Padding = System::Windows::Forms::Padding(0);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		}
#pragma endregion

	private: System::Void cTrainImageBrowse_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void cTrainLabelBrowse_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void cTestImageBrowse_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void cTestLabelBrowse_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void cStartTraining_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void cStopTraining_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void cStartTesting_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void cConfigureNetwork_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void cPauseTraining_Click(System::Object^ sender, System::EventArgs^ e);

	};
}
