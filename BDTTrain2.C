//#include "TMVA/Tools.h"
//#include "TMVA/DataLoader.h"
//#include "TMVA/Factory.h"
//using namespace std;

//Tutorial
void BDTTrain2()
{
  //Variables. Array con las variables a usar:
  int N = 15;
  string* variable_list = new string[N];
  variable_list[0] = "B_OWNPV_CHI2";
  variable_list[1] = "KS0_M";
  //ETC, hasta variable_list[N-1]

  //Instantiate class
  TMVA::Tools::Instance();

  //Output file to store the results
  TFile* outfile = TFile::Open("BDT-results.root", "RECREATE");

  //Declare factory :mgalletas:
  TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", outfile, "V:!Silent:Color:Transformations=I:DrawProgressBar:AnalysisType=Classification");
  TMVA::DataLoader *dl = new TMVA::DataLoader();

  //Get list of files and weights. Space separated
  int Nfiles = 0, Nw = 0;
  string* bkg_file = SplitString(Nfiles, bkg_filelist, " ");
  string* bkg_w_str = SplitString(Nw, bkgweightlist, " ");
  //Weights for signal and background
  double signalW = 1;
  double* bkg_w = new double[Nfiles];
  for (int i = 0; i < Nw; i++) {bkg_w[i] = stod(bkg_w_str[i]);}
  for (int i = Nw; i < Nfiles; i++) {bkg_w[i] = 1.;}

  //Open input MC and signal files
  TFile* sigfile = new TFile("Archivo_MC_cut.root");
  TTree* sigtree  = (TTree*)sigfile->Get("DecayTree");
  TFile* datafile = new TFile("Archivo_data_cut.root");
  TTree* datatree = (TTree*)datafile->Get("DecayTree");

  cout << "----------------------------------------------------------------------------" << endl;
  cout << "Starting training with " << sigtree->GetEntries() << " signal events and " << datatree->GetEntries() << " background events" << endl;
  cout << "----------------------------------------------------------------------------" << endl;

  //Add trees to dataloader
  dl->AddTree(sigtree, "Signal", signalW, "", TMVA::Types::kMaxTreeType);
  dl->AddTree(datatree, "Background", backgroundW, "", TMVA::Types::kMaxTreeType);

  //Add Variables used in BDT
  for(int i=0;i<N;i++)
    {
      dl->AddVariable(variable_list[i].c_str(), 'D');
    }
  
  //Prepare training. Here you tell the BDT to take 50% of each sample to train (by default) and randomly selected events.
  dl->PrepareTrainingAndTestTree("", "", "SplitMode=Random:SplitSeed=100");

  //TMVA method. Parameters of the BDT. Use these as default
  factory->BookMethod(dl, TMVA::Types::kBDT, "BDT", "NTrees=400:MaxDepth=2:CreateMVAPdfs=True");
  //Train and test
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
}

#if !defined(__CLING__)
int main(int argc, char** argv)
{
  switch (argc - 1)
  {
  case 2:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])));
    break;
  case 3:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])));
    break;
  case 4:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])));
    break;
  case 5:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])));
    break;
  case 6:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])));
    break;
  case 7:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])));
    break;
  case 8:
    BDTTrain2(*(new string(argv[1])), *(new string(argv[2])), *(new string(argv[3])), *(new string(argv[4])), *(new string(argv[5])), *(new string(argv[6])), *(new string(argv[7])),
              *(new string(argv[8])));
    break;
  default:
    cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
    return (1);
    break;
  }
  return 0;
}
#endif
