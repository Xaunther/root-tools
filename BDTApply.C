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
  }
  for (int i = 0; i < N_extravars; i++)
  {
    datatree->SetBranchAddress(extravar_list[i].c_str(), &uservar[N_variables + i]);
    tree->Branch(extravar_list[i].c_str(), &uservar[N_variables + i]);
  }
  for (int i = 0; i < N_extrauint; i++)
  {
    datatree->SetBranchAddress(extrauint_list[i].c_str(), &useruint[i]);
    tree->Branch(extrauint_list[i].c_str(), &useruint[i]);
  }
  for (int i = 0; i < N_extraulong64; i++)
  {
    datatree->SetBranchAddress(extraulong64_list[i].c_str(), &userulong64[i]);
    tree->Branch(extraulong64_list[i].c_str(), &userulong64[i]);
  }
  for (int i = 0; i < N_extraint; i++)
  {
    datatree->SetBranchAddress(extraint_list[i].c_str(), &userint[i]);
    tree->Branch(extraint_list[i].c_str(), &userint[i]);
  }
  for (int i = 0; i < N_extrashort; i++)
  {
    datatree->SetBranchAddress(extrashort_list[i].c_str(), &usershort[i]);
    tree->Branch(extrashort_list[i].c_str(), &usershort[i]);
  }
  for (int i = 0; i < N_extrabool; i++)
  {
    datatree->SetBranchAddress(extrabool_list[i].c_str(), &userbool[i]);
    tree->Branch(extrabool_list[i].c_str(), &userbool[i]);
  }

  //Add branch to store B mass
  double B_M;
  datatree->SetBranchAddress("B_M", &B_M);
  tree->Branch("B_M", &B_M);
  //Add branch to store BDT response
  double BDT_response;
  tree->Branch("BDT_response", &BDT_response);

  //Apply BDT
  for (long k = 0; k < datatree->GetEntries(); k++)
  {
    //Some output to see it's still alive
    if (k % 100000 == 0)
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

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  bool excludeBDTvars = false;
  switch (argc - 1)
  {
  case 2:
    BDTApply(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    if (*(new string(argv[3])) == "true" || *(new string(argv[3])) == "1") {excludeBDTvars = true;}
    BDTApply(*(new string(argv[1])), *(new string(argv[2])), excludeBDTvars);
    break;
  case 4:
    if (*(new string(argv[3])) == "true" || *(new string(argv[3])) == "1") {excludeBDTvars = true;}
    BDTApply(*(new string(argv[1])), *(new string(argv[2])), excludeBDTvars, *(new string(argv[4])));
    break;
  case 5:
    if (*(new string(argv[3])) == "true" || *(new string(argv[3])) == "1") {excludeBDTvars = true;}
    BDTApply(*(new string(argv[1])), *(new string(argv[2])), excludeBDTvars, *(new string(argv[4])), *(new string(argv[5])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
