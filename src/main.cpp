#include "main.h"


// Setup socket ID and Timer ID
enum
{
	PulseTimer = 13,
	SOCKET_ID = 69
};

class OnePunch: public wxApp{
public:
	virtual bool OnInit();
};

class OnePunchFrame: public wxFrame{
public:
	OnePunchFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	void addCia(wxArrayString &strs) {ciaBox->InsertItems(strs, ciaBox->GetCount());}
	void rmCia(unsigned int pos){ciaBox->Delete(pos);}
	wxArrayInt getCiaSelection(){wxArrayInt selections; ciaBox->GetSelections(selections); return selections;}
	bool isciaEmpty(){return ciaBox->IsEmpty();}
	unsigned int getCiaCount(){return ciaBox->GetCount();}
	wxString getNextCia(){return ciaBox->GetString(0);}
	void setRange(int value){progress->SetRange(value);}
	void setProgress(int value){progress->SetValue(value);}
	void setPulse(){progress->Pulse();}
	void stopPulse(){}
	bool connected;

private:
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnDropFiles(wxDropFilesEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnSend(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnPulseTimer(wxTimerEvent& event);
	void OnSocketEvent(wxSocketEvent& event);
	wxListBox *ciaBox = NULL;
	wxGauge *progress = NULL;
	wxTextCtrl *ipTextBox = NULL;
	wxTimer *pulseTimer = NULL;
	wxSocketClient *socket = NULL;


	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(OnePunchFrame, wxFrame)
	EVT_MENU(wxID_EXIT, OnePunchFrame::OnExit)
	EVT_MENU(wxID_ABOUT, OnePunchFrame::OnAbout)
	EVT_MENU(wxID_OPEN, OnePunchFrame::OnOpen)
	EVT_TIMER(PulseTimer, OnePunchFrame::OnPulseTimer)
	EVT_SOCKET(SOCKET_ID, OnePunchFrame::OnSocketEvent)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(OnePunch);

// Load and show UI
bool OnePunch::OnInit(){
	OnePunchFrame *frame = new OnePunchFrame("OnePunch", wxPoint(50,50), wxSize(450,500));
	frame->Show(true);
	return true;
}


// Setup UI in wxWidgets
OnePunchFrame::OnePunchFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size){
		wxPanel *panel = new wxPanel(this, -1);
		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

		wxStaticText *ipLabel = new wxStaticText(panel, wxID_ANY, wxT("3DS IP Address"));
		hbox->Add(ipLabel, 0, wxRIGHT, 8);
		ipTextBox = new wxTextCtrl(panel, wxID_ANY);
		hbox->Add(ipTextBox, 1);
		vbox->Add(hbox, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
		vbox->Add(-1, 10);

		wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *ciaLabel = new wxStaticText(panel, wxID_ANY, wxT("CIA Queue"));
		hbox2->Add(ciaLabel, 0);
		vbox->Add(hbox2, 0, wxLEFT | wxTOP, 10);
		vbox->Add(-1, 10);

		wxBoxSizer *hbox3 = new wxBoxSizer(wxHORIZONTAL);
		ciaBox = new wxListBox(panel, wxID_ANY, wxPoint(-1,-1), wxSize(-1, -1), 0, NULL, wxLB_EXTENDED);
		ciaBox->DragAcceptFiles(true);
		ciaBox->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(OnePunchFrame::OnDropFiles), NULL, this);
		ciaBox->Connect(wxEVT_CHAR, wxKeyEventHandler(OnePunchFrame::OnChar), NULL, this);
		hbox3->Add(ciaBox, 1, wxEXPAND);
		vbox->Add(hbox3, 10, wxLEFT | wxRIGHT | wxEXPAND, 10);

		vbox->Add(-1, 10);

		wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);
		progress = new wxGauge(panel, wxID_ANY, 100);
		progress->SetRange(100);
		hbox5->Add(progress, 1, wxLEFT | wxRIGHT);
		vbox->Add(hbox5, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);

		vbox->Add(-1, 10);

		wxBoxSizer *hbox4 = new wxBoxSizer(wxHORIZONTAL);
		wxButton *btnSend = new wxButton(panel, wxID_ANY, wxT("Send"));
		btnSend->Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OnePunchFrame::OnSend), NULL, this);
		wxButton *btnCancel = new wxButton(panel, wxID_ANY, wxT("Cancel"));
		btnCancel->Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OnePunchFrame::OnCancel),NULL, this);
		hbox4->Add(btnCancel, 0);
		hbox4->Add(btnSend, 0, wxLEFT | wxBOTTOM, 5);
		vbox->Add(hbox4, 0, wxALIGN_RIGHT | wxRIGHT, 10);


		wxMenu *menuFile = new wxMenu;
		menuFile->Append(wxID_OPEN, wxT("&Open\tCtrl+O"));
		menuFile->Append(wxID_EXIT);
		wxMenu *menuHelp = new wxMenu;
		menuHelp->Append(wxID_ABOUT);

		wxMenuBar *menuBar = new wxMenuBar;
		menuBar->Append(menuFile, "&File");
		menuBar->Append(menuHelp, "&Help");

		SetMenuBar(menuBar);
		panel->SetSizer(vbox);
		Center();
		CreateStatusBar();
		pulseTimer = new wxTimer(this, PulseTimer);
		SetStatusText("Ready!");

	}

void OnePunchFrame::OnExit(wxCommandEvent& event){
	Close(true);
}

void OnePunchFrame::OnAbout(wxCommandEvent& event){
	wxAboutDialogInfo info;
	info.SetName(_("OnePunch"));
	info.SetVersion(_(version));
	info.SetDescription(_("An application to transmit cia files to a 3DS with FBI.\nhttp://github.com/Flat/OnePunch\nThis program is licensed under the MIT License. Full license text available here: https://github.com/Flat/OnePunch/blob/master/LICENSE"));
	info.SetCopyright("(C) Ken Swenson 2016");
	info.AddDeveloper("Ken Swenson (Flat) <flat@imo.uto.moe>");
	wxAboutBox(info);
}
void OnePunchFrame::OnOpen(wxCommandEvent& event){
	wxFileDialog openFileDialog(this, _("Select CIA file"), "", "", "cia files (*.cia)|*.cia", wxFD_OPEN|wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
	if(openFileDialog.ShowModal() == wxID_CANCEL){
		return;
	}
	wxArrayString selectedFiles;
	openFileDialog.GetPaths(selectedFiles);
	addCia(selectedFiles);
}

// Handle files dragged to UI
void OnePunchFrame::OnDropFiles(wxDropFilesEvent& event){
	if(event.GetNumberOfFiles() > 0){
		wxString* droppedfiles = event.GetFiles();

		wxString filename;
		wxString extension;
		wxString filespec = _T("*.cia");
		wxArrayString files;

		for(int i=0; i< event.GetNumberOfFiles(); i++){
			filename = droppedfiles[i];
			wxFileName::SplitPath(filename, NULL, NULL, NULL, &extension);
			if(wxFileExists(filename) && extension == _T("cia")){
				files.push_back(filename);
			}
			else if(wxDirExists(filename)){
				wxDir::GetAllFiles(filename, &files, filespec);
			}
		}
		if(!files.IsEmpty()){
			addCia(files);
		}

	}	
}

// Handle DEL key to delete currently selected entry
void OnePunchFrame::OnChar(wxKeyEvent& event){
	if(event.GetKeyCode() == WXK_DELETE){
		wxArrayInt selections = getCiaSelection();
		int removed = 0;
		for(size_t i=0; i< selections.size(); i++){
			rmCia(selections[i - removed]);
			removed++;
		}
	}
}

// Send CIA to 3DS
void OnePunchFrame::OnSend(wxCommandEvent& event){

	wxTCPClient *TCPClient = new wxTCPClient();
	wxString host = ipTextBox->GetValue();
	if(host.IsEmpty()){
		wxMessageBox("IP address cannot be left blank.", "No IP", wxOK | wxICON_ERROR, this);
		return;
	}
	wxString port = wxString::Format("%i", FBIport);
	socket = new wxSocketClient();
	wxIPV4address address;
	address.Hostname(host);
	address.Service(wxString::Format("%i", FBIport));
	socket->SetEventHandler(*this, SOCKET_ID);
	socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_LOST_FLAG);
	socket->SetFlags(wxSOCKET_WAITALL);
	socket->Notify(true);
	socket->SetTimeout(4);
	wxSocketOutputStream* sos = new wxSocketOutputStream(*socket);


	if(!TCPClient->ValidHost(host)){
		wxMessageBox("Invalid IP address. Please enter a valid IP.", "Not a Valid Host", wxOK | wxICON_ERROR, this);
		return;
	}
	if(isciaEmpty()){
		wxMessageBox(wxT("You must add cia files to send."), wxT("Empty Queue"));
		return;
	}
	SetStatusText(getNextCia());
	unsigned int cias = getCiaCount();
	for(int i = 0; i < cias; i++){
		wxFileInputStream* cia = new wxFileInputStream(getNextCia());
		if(!cia->IsOk()){
			SetStatusText("Failed to open cia");
			return;
		}
		wxFileOffset ciaSize = cia->GetLength();
		uint64_t cSize = (uint64_t)ciaSize;
		#ifdef __unix__
		cSize = htobe64(cSize);
		#elif defined(_WIN32) || defined(WIN32)
		const uint32_t upper = htonl(static_cast<uint32_t>(cSize >> 32));
		const uint32_t lower = htonl(static_cast<uint32_t>(cSize & 0xFFFFFFFFLL));
		cSize = (static_cast<uint64_t>(lower) << 32) | upper;
		#endif
		socket->Connect(address, false);
		pulseTimer->Start(100);
		SetStatusText("Connecting to 3DS at " + ipTextBox->GetValue() + ":" + wxString::Format("%i", FBIport));
		while(!socket->WaitOnConnect(25, 0)){

		}
		bool success = socket->IsConnected();
		connected = success;
		if(!success){
			SetStatusText("Failed to connect.");
			pulseTimer->Stop();
			setProgress(0);
			return;
		}
		SetStatusText("Connected!");
		SetStatusText("Sending CIA Length");

		char *byteSize = (char*)&cSize;
		sos->Write(byteSize, sizeof(byteSize));
		SetStatusText("Sending CIA");
		pulseTimer->Stop();
		size_t read = 0;
		char* buffer[sendBuffSize];
		size_t count = 0;
		size_t written = 0;
		double readMB;
		double ciaMB = (double)cia->GetLength()/1048576;

		while(count != (size_t)cia->GetLength() && !socket->IsDisconnected()){
			cia->Read(buffer, sizeof(buffer));
			read = cia->LastRead();
			sos->Write(buffer, read);
			if(socket->Error()){
				SetStatusText("Failed while transmitting");
				wxMessageBox(wxString::Format("Socket Error: %i", socket->LastError()), wxT("Error while transferring"));
				sos->Close();
				socket->Close();
				return;
			}
			written = sos->LastWrite();
			count += written;
			readMB = (double)count/1048576;
			setRange(cia->GetLength());
			setProgress((int)count);
			SetStatusText(wxString::Format("Transmitting %s: %.2f/%.2f MB", getNextCia(), readMB, ciaMB));
		}
		setProgress((int)count);
		SetStatusText(wxString::Format("Finished sending %s", getNextCia()));
		rmCia(0);
		sos->Close();
		socket->Close();
	}
	
	
	
	
}

void OnePunchFrame::OnCancel(wxCommandEvent& event){
	if(pulseTimer->IsRunning()){
		pulseTimer->Stop();		
	}
	if(socket != NULL){
		if(socket->IsConnected()){
			socket->Close();
		}
	}
	connected = false;
	setProgress(0);
	SetStatusText("Canceled operation.");
}

void OnePunchFrame::OnPulseTimer(wxTimerEvent& event){
	setPulse();
}

void OnePunchFrame::OnSocketEvent(wxSocketEvent& event){
	//todo implement socket events
	if(event.GetSocketEvent() == wxSOCKET_CONNECTION){
		connected = true;
	}
	if(event.GetSocketEvent() == wxSOCKET_LOST){
		connected = false;
	}
}