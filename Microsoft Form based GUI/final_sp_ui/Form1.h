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
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::CheckBox^  checkBox1;
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
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(55, 192);
			this->radioButton1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(124, 21);
			this->radioButton1->TabIndex = 0;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"Automated test";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton1_CheckedChanged);
			// 
			// radioButton4
			// 
			this->radioButton4->AutoSize = true;
			this->radioButton4->Location = System::Drawing::Point(55, 487);
			this->radioButton4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(51, 21);
			this->radioButton4->TabIndex = 3;
			this->radioButton4->TabStop = true;
			this->radioButton4->Text = L"Exit";
			this->radioButton4->UseVisualStyleBackColor = true;
			this->radioButton4->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton4_CheckedChanged);
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::SystemColors::MenuHighlight;
			this->button1->Location = System::Drawing::Point(304, 550);
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
			this->label2->Location = System::Drawing::Point(238, 194);
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
			this->label3->Location = System::Drawing::Point(207, 247);
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
			this->radioButton2->Location = System::Drawing::Point(55, 243);
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
			this->radioButton3->Location = System::Drawing::Point(55, 346);
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
			this->label4->Location = System::Drawing::Point(207, 348);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(112, 17);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Predicted Word!!";
			// 
			// radioButton5
			// 
			this->radioButton5->AutoSize = true;
			this->radioButton5->Location = System::Drawing::Point(55, 405);
			this->radioButton5->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->radioButton5->Name = L"radioButton5";
			this->radioButton5->Size = System::Drawing::Size(111, 21);
			this->radioButton5->TabIndex = 5;
			this->radioButton5->TabStop = true;
			this->radioButton5->Text = L"Live Training";
			this->radioButton5->UseVisualStyleBackColor = true;
			this->radioButton5->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton5_CheckedChanged);
			// 
			// checkBox3
			// 
			this->checkBox3->Location = System::Drawing::Point(241, 401);
			this->checkBox3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(111, 30);
			this->checkBox3->TabIndex = 16;
			this->checkBox3->Text = L"Save Files";
			this->checkBox3->UseVisualStyleBackColor = true;
			this->checkBox3->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox3_CheckedChanged);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(391, 405);
			this->checkBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(98, 21);
			this->checkBox1->TabIndex = 17;
			this->checkBox1->Text = L"Play Back!!";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged);
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(16) {L"Open", L"Close", L"Exit", L"History", L"Up", 
				L"Down", L"Quora", L"Jobs", L"Search", L"Payment", L"Video", L"Portal", L"Faculties", L"Outlook", L"Github", L"Calender"});
			this->comboBox1->Location = System::Drawing::Point(311, 447);
			this->comboBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(213, 24);
			this->comboBox1->TabIndex = 18;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// label5
			// 
			this->label5->ForeColor = System::Drawing::SystemColors::InactiveCaptionText;
			this->label5->Location = System::Drawing::Point(22, 442);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(238, 30);
			this->label5->TabIndex = 19;
			this->label5->Text = L"Select Word to be Trained!!";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(13, 295);
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
			this->label6->Location = System::Drawing::Point(207, 225);
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
			this->label7->Location = System::Drawing::Point(412, 194);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(46, 17);
			this->label7->TabIndex = 23;
			this->label7->Text = L"label7";
			this->label7->Visible = false;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(207, 562);
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
			this->label9->Location = System::Drawing::Point(405, 594);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(306, 13);
			this->label9->TabIndex = 25;
			this->label9->Text = L"Copyright 2021 Jay Khatri (214101023), V. Abhijith (214101059)";
			this->label9->Click += gcnew System::EventHandler(this, &Form1::label9_Click);
			// 
			// btn_cnf
			// 
			this->btn_cnf->Location = System::Drawing::Point(476, 342);
			this->btn_cnf->Name = L"btn_cnf";
			this->btn_cnf->Size = System::Drawing::Size(75, 23);
			this->btn_cnf->TabIndex = 26;
			this->btn_cnf->Text = L"Perform";
			this->btn_cnf->UseVisualStyleBackColor = true;
			this->btn_cnf->Visible = false;
			this->btn_cnf->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(338, 247);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(54, 17);
			this->label10->TabIndex = 27;
			this->label10->Text = L"label10";
			this->label10->Visible = false;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(103, -29);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(569, 184);
			this->pictureBox1->TabIndex = 28;
			this->pictureBox1->TabStop = false;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(357, 346);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(54, 17);
			this->label11->TabIndex = 29;
			this->label11->Text = L"label11";
			this->label11->Visible = false;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlLight;
			this->ClientSize = System::Drawing::Size(782, 624);
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
			this->Controls->Add(this->checkBox3);
			this->Controls->Add(this->checkBox1);
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
				 this->btn_cnf->Visible = false;
			 }
private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->label3->Visible = false;
			 this->label1->Visible=false;
			 this->label6->Visible=true;
			 this->label10->Visible = false;
			 this->label4->Visible=false;
			 this->label11->Visible = false;
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
			 this->btn_cnf->Visible = false;
			 choice='3';
			
		 }
private: System::Void radioButton4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->label3->Visible = false;
			 this->label11->Visible = false;
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
				 read_codebook();
				 this->button1->Visible = false;
				 this->Refresh();
				 if(environment_known == 0){
						printf("--------------Recording silence--------------\n");
						system("Recording_Module.exe 1 silence.wav silence_file.txt");	
						environment_known = 1;
					}
					/*int choice;
					for(int i=0;i<16;i++)
					{
						cout<<"Enter "<<i+1<<" for training "<<keywords[i]<<endl;
					}
					cin>>choice;
					choice--;*/
					
				this->button1->Visible = true;
			
		 }
		 else{
			 exit(0);
         }
			  
			
		 }
private: System::Void radioButton5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 label1->Visible=false;
			 label6->Visible=false;
			 choice='4';
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
};
}

