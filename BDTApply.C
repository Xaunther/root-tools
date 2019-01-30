void BDTApply(string fileapplied)
{
  //Variables. Array con las variables a usar:
  int N = 15;
  string* variable_list = new string[N];
  variable_list[0] = "B_OWNPV_CHI2";
  variable_list[1] = "KS0_M";
  //ETC, hasta variable_list[N-1]

  TFile* data = new TFile(fileapplied.c_str()); //Data_cut.root o MC_cut.root (aprox)
  TTree* datatree = (TTree*)data->Get("DecayTree");

  //BDT Output
  TFile* target = new TFile((fileapplied+"_BDT").c_str(), "RECREATE");
  TTree* tree = new TTree("DecayTree", "treelibrated tree");

  TMVA::Tools::Instance();
  TMVA::Reader* reader =  new TMVA::Reader("V:Color:!Silent");

  Float_t* var = new Float_t[N];
  //Variables used in training
  for(int i=0;i<N;i++)
    {
      reader->TMVA::Reader::AddVariable(variable_list[i].c_str(), &var[i]);
    }
  //TMVA method
  //The Folder is the one used by default
  reader->TMVA::Reader::BookMVA("BDT method", "default/weights/TMVAClassification_BDT.weights.xml");

  //Variables from data
  for(int i=0;i<N;i++)
    {
      datatree->SetBranchAddress(variable_list[i].c_str(), &var[i]);
      tree->Branch(variable_list[i].c_str(), &var[i]);
    }

  //Add branch to store B mass
  double B_M;
  datatree->SetBranchAddress("B_M", &B_M);
  tree->Branch("B_M", &B_M);
  //Add branch to store BDT response
  double BDT_response;
  tree->Branch("BDT_response", &BDT_response);

  //Apply BDT
  for(long k=0; k<datatree->GetEntries(); k++)
    {
      datatree->GetEntry(k);
      BDT_response = reader->TMVA::Reader::EvaluateMVA("BDT method");
      tree->Fill();
    }
  //Save ntuple
  tree->Write();
  
  data->Close();
  target->Close();
}

