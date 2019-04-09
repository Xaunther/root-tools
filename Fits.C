void fit()
{
  //Mass variable. Name between "" must coincide with the name from the tree
  RooRealVar B_M("B_M", "B_M", 4300., 6300.);
  //Initialize all the stuff needed for the fit
  //Gaussian
  RooRealVar mean("mean", "mean", 5300., 4300., 6300.);
  RooRealVar width("width", "width", 100., 10., 200.);
  RooGaussian signal("signal", "signal", B_M, mean, width);
  //Exponential
  RooRealVar tau("tau", "tau", -0.01, -1., 0.);
  RooExponential bkg("bkg", "bkg", B_M, tau);

  //Signal N
  RooRealVar fsig("fsig", "fsig", 120., 0., 1000.);
  //Bkg N
  RooRealVar fbkg("fbkg", "fbkg", 600., 0., 2000.);
  
  //Sum gauss + exp
  RooAddPdf model("model", "model", RooArgList(signal,bkg), RooArgList(fsig,fbkg));
  //Move your original TTree* (in this example called tree) to a dataset
  RooDataSet data("data", "data", tree, B_M);
  model.fitTo(data);
}
