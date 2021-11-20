#include "hmm_backend.h"
#pragma once

namespace final_sp_ui {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for test
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		int test_ans;
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::RadioButton^  radioButton1;
	protected: 


	private: System::Windows::Forms::RadioButton^  radioButton4;
	private: System::Windows::Forms::Button^  button1;


	private: System::Windows::Forms::Label^  label2;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::RadioButton^  radioButton2;

	private: System::Windows::Forms::RadioButton^  radioButton3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::RadioButton^  radioButton5;


	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Label^  label5;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Button^  btn_cnf;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->radioButton5 = (gcnew System::Windows::Forms::RadioButton());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->btn_cnf = (gcnew System::Windows::Forms::Button());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->BackColor = System::Drawing::Color::Transparent;
			this->radioButton1->Location = System::Drawing::Point(51, 199);
			this->radioButton1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(124, 21);
			this->radioButton1->TabIndex = 0;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"Automated test";
			this->radioButton1->UseVisualStyleBackColor = false;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton1_CheckedChanged);
			// 
			// radioButton4
			// 
			this->radioButton4->AutoSize = true;
			this->radioButton4->BackColor = System::Drawing::Color::Transparent;
			this->radioButton4->Location = System::Drawing::Point(50, 532);
			this->radioButton4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(51, 21);
			this->radioButton4->TabIndex = 3;
			this->radioButton4->TabStop = true;
			this->radioButton4->Text = L"Exit";
			this->radioButton4->UseVisualStyleBackColor = false;
			this->radioButton4->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton4_CheckedChanged);
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button1->Location = System::Drawing::Point(315, 558);
			this->button1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(119, 41);
			this->button1->TabIndex = 4;
			this->button1->Text = L"Let\'s Go!!";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::Transparent;
			this->label2->Location = System::Drawing::Point(224, 199);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(210, 17);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Overall Accuarcy achieved (%): ";
			this->label2->Visible = false;
			this->label2->Click += gcnew System::EventHandler(this, &Form1::label2_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(188, 260);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(112, 17);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Predicted Word!!";
			this->label3->Visible = false;
			this->label3->Click += gcnew System::EventHandler(this, &Form1::label3_Click);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(47, 260);
			this->radioButton2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(102, 21);
			this->radioButton2->TabIndex = 1;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"Manual test";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton2_CheckedChanged);
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(47, 352);
			this->radioButton3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(101, 21);
			this->radioButton3->TabIndex = 2;
			this->radioButton3->TabStop = true;
			this->radioButton3->Text = L"Live testing";
			this->radioButton3->UseVisualStyleBackColor = true;
			this->radioButton3->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton3_CheckedChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(188, 352);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(112, 17);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Predicted Word!!";
			// 
			// radioButton5
			// 
			this->radioButton5->AutoSize = true;
			this->radioButton5->BackColor = System::Drawing::Color::Transparent;
			this->radioButton5->Location = System::Drawing::Point(40, 439);
			this->radioButton5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->radioButton5->Name = L"radioButton5";
			this->radioButton5->Size = System::Drawing::Size(111, 21);
			this->radioButton5->TabIndex = 5;
			this->radioButton5->TabStop = true;
			this->radioButton5->Text = L"Live Training";
			this->radioButton5->UseVisualStyleBackColor = false;
			this->radioButton5->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton5_CheckedChanged);
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(16) {L"Open", L"Close", L"Exit", L"History", L"Up", 
				L"Down", L"Quora", L"Jobs", L"Search", L"Payment", L"Video", L"Portal", L"Faculties", L"Outlook", L"Github", L"Calender"});
			this->comboBox1->Location = System::Drawing::Point(279, 485);
			this->comboBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(213, 24);
			this->comboBox1->TabIndex = 18;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// label5
			// 
			this->label5->BackColor = System::Drawing::Color::Transparent;
			this->label5->ForeColor = System::Drawing::SystemColors::InactiveCaptionText;
			this->label5->Location = System::Drawing::Point(8, 485);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(238, 30);
			this->label5->TabIndex = 19;
			this->label5->Text = L"Select Word to be Trained!!";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label5->Click += gcnew System::EventHandler(this, &Form1::label5_Click);
			// 
			// label1
			// 
			this->label1->BackColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(10, 294);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(743, 38);
			this->label1->TabIndex = 21;
			this->label1->Text = L"Will take 2 recordings for the first call of live testing, in first recording it " 
				L"will take silence as input and in second recording it will take user input!!";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label1->Visible = false;
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(107, 227);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(371, 17);
			this->label6->TabIndex = 22;
			this->label6->Text = L"Store your input file in input folder with input_file.txt name.";
			this->label6->Visible = false;
			this->label6->Click += gcnew System::EventHandler(this, &Form1::label6_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->BackColor = System::Drawing::Color::Transparent;
			this->label7->Location = System::Drawing::Point(460, 199);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(46, 17);
			this->label7->TabIndex = 23;
			this->label7->Text = L"label7";
			this->label7->Visible = false;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(170, 570);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(91, 17);
			this->label8->TabIndex = 24;
			this->label8->Text = L"Please wait...";
			this->label8->Visible = false;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.5F));
			this->label9->Location = System::Drawing::Point(548, 671);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(306, 13);
			this->label9->TabIndex = 25;
			this->label9->Text = L"Copyright 2021 Jay Khatri (214101023), V. Abhijith (214101059)";
			this->label9->Click += gcnew System::EventHandler(this, &Form1::label9_Click);
			// 
			// btn_cnf
			// 
			this->btn_cnf->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->btn_cnf->Location = System::Drawing::Point(384, 342);
			this->btn_cnf->Name = L"btn_cnf";
			this->btn_cnf->Size = System::Drawing::Size(75, 37);
			this->btn_cnf->TabIndex = 26;
			this->btn_cnf->Text = L"Perform";
			this->btn_cnf->UseVisualStyleBackColor = false;
			this->btn_cnf->Visible = false;
			this->btn_cnf->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::Transparent;
			this->label10->Location = System::Drawing::Point(365, 263);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(54, 17);
			this->label10->TabIndex = 27;
			this->label10->Text = L"label10";
			this->label10->Visible = false;
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Transparent;
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(110, -40);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(539, 183);
			this->pictureBox1->TabIndex = 28;
			this->pictureBox1->TabStop = false;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(312, 352);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(54, 17);
			this->label11->TabIndex = 29;
			this->label11->Text = L"label11";
			this->label11->Visible = false;
			// 
			// listBox1
			// 
			this->listBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left));
			this->listBox1->BackColor = System::Drawing::SystemColors::MenuBar;
			this->listBox1->FormattingEnabled = true;
			this->listBox1->ItemHeight = 16;
			this->listBox1->Items->AddRange(gcnew cli::array< System::Object^  >(11) {L"Available Words:-", L"Calender", L"Exit", L"Faculties", 
				L"Github", L"Jobs", L"Open", L"Outlook", L"Payments", L"Portal", L"Quora"});
			this->listBox1->Location = System::Drawing::Point(703, 11);
			this->listBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(176, 196);
			this->listBox1->TabIndex = 30;
			this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listBox1_SelectedIndexChanged);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::SystemColors::ControlDark;
			this->button2->Location = System::Drawing::Point(509, 420);
			this->button2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(158, 41);
			this->button2->TabIndex = 31;
			this->button2->Text = L"Go to next recording!!";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Visible = false;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click_1);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->BackColor = System::Drawing::Color::Transparent;
			this->checkBox1->Location = System::Drawing::Point(227, 440);
			this->checkBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(207, 21);
			this->checkBox1->TabIndex = 32;
			this->checkBox1->Text = L"Update live training models!!";
			this->checkBox1->UseVisualStyleBackColor = false;
			this->checkBox1->Visible = false;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged_1);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->BackColor = System::Drawing::Color::White;
			this->label12->Location = System::Drawing::Point(10, 391);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(785, 17);
			this->label12->TabIndex = 33;
			this->label12->Text = L"Will take totalof 10 recordings, in first recording it will take silene as input " 
				L"and in susequent recordings it will take user input!!";
			this->label12->Visible = false;
			this->label12->Click += gcnew System::EventHandler(this, &Form1::label12_Click);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->BackColor = System::Drawing::Color::Transparent;
			this->label13->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->label13->Location = System::Drawing::Point(587, 582);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(250, 17);
			this->label13->TabIndex = 34;
			this->label13->Text = L"Copyright 2021  Jay Khatri, V. Abhijith ";
			this->label13->Click += gcnew System::EventHandler(this, &Form1::label13_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlLight;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(891, 607);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->listBox1);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->btn_cnf);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->radioButton2);
			this->Controls->Add(this->radioButton5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->radioButton3);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->radioButton4);
			this->Controls->Add(this->radioButton1);
			this->Controls->Add(this->pictureBox1);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"Form1";
			this->Text = L"Speech Based Browser";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 choice='1';
				 this->label1->Visible=false;
				 this->label6->Visible=false;
				 this->label3->Visible = false;
				 this->label10->Visible = false;
				 this->label4->Visible=false;
				 this->label12->Visible = false;
				 this->btn_cnf->Visible = false;
			 }
private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->label3->Visible = false;
			 this->label1->Visible=false;
			 this->label6->Visible=true;
			 this->label10->Visible = false;
			 this->label4->Visible=false;
			 this->label11->Visible = false;
			 this->label12->Visible = false;
			 this->btn_cnf->Visible = false;
			 choice='2';
		 }
private: System::Void radioButton3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->label3->Visible = false;
			 this->label1->Visible=true;
			 this->label4->Visible=false;
			 this->label6->Visible=false;
			 this->label11->Visible = false;
			 this->label10->Visible = false;
			 this->label12->Visible = false;
			 this->btn_cnf->Visible = false;
			 choice='3';
			
		 }
private: System::Void radioButton4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->label3->Visible = false;
			 this->label11->Visible = false;
			 this->label12->Visible = false;
			 this->label4->Visible=false;
			 choice='0';
			 this->label10->Visible = false;
			 this->label1->Visible=false;
			 this->btn_cnf->Visible = false;
		 }

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Refresh();
			 if(choice=='1'){
				 this->button1->Visible = false;
				 this->label8->Visible = true;
				read_codebook();
				ans = testing();
				this->label7->Text= ans.ToString();
				this->label8->Visible = false;
				this->label2->Visible = true;
				this->label7->Visible = true;
				this->button1->Visible = true;
				  //this->Refresh();
			 }
			 else if(choice=='2'){
				 read_codebook();
				 this->label3->Visible = false;
				 this->button1->Visible = false;
				  int man_ans = test_file_helper();
				  string str = keywords[man_ans];
				  String^ str3 = gcnew String(str.c_str());
				  this->label10->Text=str3;
				  this->label10->Visible = true;
				  this->button1->Visible = true;
				  this->label3->Visible = true;
				  this->Refresh();
			 }
			 else if(choice=='3'){
				 read_codebook();
				 this->button1->Visible = false;
				  this->Refresh();
					String ^s1;
					int live = live_testing_helper();
					string str = keywords[live];
					String^ str2 = gcnew String(str.c_str());
					this->label11->Text=str2;
					this->button1->Visible = true;
					this->btn_cnf->Visible = true;
					this->label4->Visible=true;
					this->label11->Visible = true;
			 }
			 else if(choice=='4'){
				 this->button1->Visible = true;
				 label12->Visible=true;
				 read_codebook();
				 this->Refresh();
				 itr_count=1;
			String ^gettext=comboBox1->Text;
			String ^word1,^word2,^word3,^word4,^word5,^word6,^word7,^word8,^word9,^word10,^word11,^word12,^word13,^word14,^word15,^word16;
			word1="Calender";
			word2="Close";
			word3="Down";
			word4="Exit";
			word5="Faculties";
			word6="Github";
			word7="History";
			word8="Jobs";
			word9="Open";
			word10="Outlook";
			word11="Payment";
			word12="Portal";
			word13="Quora";
			word14="Search";
			word15="Up";
			word16="Video";

			//Will set the index of the word used in live training.
			if(word1==gettext)
				train_index=0;
			else if(word2==gettext)
				train_index=1;
			else if(word3==gettext)
				train_index=2;
			else if(word4==gettext)
				train_index=3;
			else if(word5==gettext)
				train_index=4;
			else if(word6==gettext)
				train_index=5;
			else if(word7==gettext)
				train_index=6;
			else if(word8==gettext)
				train_index=7;
			else if(word9==gettext)
				train_index=8;
			else if(word10==gettext)
				train_index=9;
			else if(word11==gettext)
				train_index=10;
			else if(word12==gettext)
				train_index=11;
			else if(word13==gettext)
				train_index=12;
			else if(word14==gettext)
				train_index=13;
			else if(word15==gettext)
				train_index=14;
			else if(word16==gettext)
				train_index=15;

				 if(environment_known == 0){
						printf("--------------Recording silence--------------\n");
						system("Recording_Module.exe 1 silence.wav silence_file.txt");	
						environment_known = 1;
					}
					    button1->Visible=false;
						button2->Visible=true;
						checkBox1->Visible=true;
			
		 }
		 else{
			 exit(0);
         }
			  
			
		 }
private: System::Void radioButton5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 label1->Visible=false;
			 label6->Visible=false;
			 this->label12->Visible = true;
			 choice='4';
			 //textBox6->Text=" ";
			// textBox4->Text=" ";
			 checkBox1->Visible=false;
			 //checkBox2->Visible=false;
	
		 }
private: System::Void checkedListBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			
		 }
private: System::Void textBox4_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox5_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void textBox6_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label6_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void checkBox3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			
		 }
private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 String ^lt;
			// lt=comboBox1.SelectedValue.ToString();
		 }
private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label9_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 perform(manual_test_result);
		 }
private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label3_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button2_Click_1(System::Object^  sender, System::EventArgs^  e) {
			  if(itr_count<=10)
			 live_training(train_index);
			  // this-> textBox4->Text= itr_count.ToString();
			 itr_count++;
			 if(itr_count>10)
			 {
				 //button1->Visible=true;
				 label11->Visible=false;
				  label12->Visible=false;
				 button2->Visible=false;
				 button1->Visible=true;
				 checkBox1->Visible=false;

			 }
		 }
private: System::Void checkBox1_CheckedChanged_1(System::Object^  sender, System::EventArgs^  e) {
			  if(live_update==0)
			 live_update=1;
			 else
			 live_update=0;
		 }
private: System::Void label12_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label5_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label13_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

