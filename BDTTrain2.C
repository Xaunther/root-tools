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
  
  //Weights for signal and background
  double signalW = 1;
  double backgroundW = 1;

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
