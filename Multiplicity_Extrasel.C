//This script is designed for the HLT2 upgrade, where we want to know the multiplicity (how many particles are selected per selected event) of the extra selections
//For that, we take a list of ntuples (which we'll join in a TChain) and compute the multiplicity as m/e
//Where m is the number of entries in such extra container
//And e is the number of events the line fire
//In principle, we might need 2 different decay trees since as there can be an event that fired a trigger line but didn't get any entry in the extra selection
//And we want to take that into account as well

//EXAMPLE:
//2 events fired trigger line (e=2), the first recorded 1 extra particle and the second none
//If we just look at the extrasel tree, we'll find only 1 particle (m=1), but 1 event (e=1); so multiplicity = 1, but actually 2 events fired, so the multiplicity IS 1/2

//In order to calculate the "Number of events" it assumes the reference ntuple was not cut after being created (it wouldn't make sense to run this if it was, right?)

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "TChain.h"
#include "TTreeFormula.h"
#include "Functions/StringTools.h"
#include "Functions/TreeTools.h"
#include "Functions/Filereading.h"
#include "Functions/ArrayTools.h"

//Get Number of Events from a vector of chains
//Each chain is assumed to have been made from the same initial (m)DST
//Furthermore, is chain is made of a list of trees, for each internal tree we must determine the number of events
//For that, we use the "EventInSequence" variable. Its maximum usually corresponds to the maximum event of the (m)DST processed
//However, it might be the case that that chain did not use the last event, and so the maximum EventInSequence seen does not correspond to the number of events
//That's why we loop over all the DecayTrees produced by 1 line, because the maximum among all of them ALWAYS is the number of events
//We are also assuming no cuts have been applied after ntuple production
int GetNEvents(std::vector<TChain *> &chains, const int NTrees)
{
    int NEvents = 0;
    //Initialize vector with events in each tree
    std::vector<int> NEventsTree(NTrees, 0);
    for (auto &chain : chains) //Loop over the DecayTrees of the same line
    {
        if (!chain->GetEntries())
            continue; //Skip if chain is empty
        //Now we must determine the maximum "EventInSequence" in each tree
        auto EventInSequence = new TTreeFormula("EventInSequence", "EventInSequence", chain);
        auto totCandidate = new TTreeFormula("totCandidates", "totCandidates", chain);
        int i = 0;
        while (i < chain->GetEntries())
        {
            chain->GetEntry(i);
            if (EventInSequence->EvalInstance() > NEventsTree[chain->GetTreeNumber()]) //If this entry has a higher value than the max recorded so far for this tree in the chain
                NEventsTree[chain->GetTreeNumber()] = EventInSequence->EvalInstance(); //Keep it as new maximum
            //Go to next entry, we can skip candidates from the same event
            i = i + totCandidate->EvalInstance();
        }
    }
    //After all of this, sum the events from each tree and return it
    for (const auto &N : NEventsTree)
        NEvents += N;
    std::cout << NEvents << std::endl;
    return NEvents;
}

void Multiplicity_Extrasel(std::string filelist, std::string cutfile, std::string outfile, std::string ref_dectreelist = "DecayTree", std::string extra_dectreename = "DecayTree")
{
    //First, take the filelist and get an array of names
    int N_files = 0;
    std::string *filenames = SplitString(N_files, filelist, " ");
    //Second, the list of decaytrees used in each file
    //Because One line spawns several decay trees
    int N_decays = 0;
    std::string *ref_dectreenames = SplitString(N_decays, ref_dectreelist, " ");

    //Now let's load all these reference chains
    std::vector<TChain *> ref_chain;
    for (int i = 0; i < N_decays; i++)
        ref_chain.push_back(GetChain(filenames, N_files, ref_dectreenames[i]));
    TChain *extra_chain = GetChain(filenames, N_files, extra_dectreename);
    //We also need to read the list of cuts provided in the file
    int N_cuts = 0;
    std::string *cuts = ReadVariables(N_cuts, cutfile);

    //Everything loaded, now the idea is to go cut by cut and dump the result in the output file
    //The difficult task here is to use all the TChains in ref_chain to get a single number, the number of events (not candidates)
    //If we had just one decaytree for each line (as in the early stage of HLT2 lines) we could just use:
    //int N_E = ref_chain->GetEntries("nCandidate==0");
    //However, with several decays we must add up all these numbers and substract the intersections between them
    //Or, keep track of all evt numbers (or whatever good ID number) and count how many we have at the end
    //The number of events is always the same, and we assume no other cuts were done since the ntuple was created

    int N_E = GetNEvents(ref_chain, N_files);
    //The denominator must be >0, otherwise just print an error message on the output
    bool denom_OK = (bool)N_E;
    //Make some decent output
    std::ofstream fout;
    fout.open(outfile.c_str());
    fout << "Multiplicity applied on " << filelist << ", using: " << std::endl;
    fout << "Reference Tree(s): ";
    for (int i = 0; i < N_decays; i++)
        fout << ref_dectreenames[i] << ", ";
    fout << std::endl;
    fout << "Extra Tree: " << extra_dectreename << std::endl;
    fout << endl;
    fout << "               Multiplicity for each cut               " << endl;
    fout << "-------------------------------------------------------" << endl;
    int maxL = GetMaxLength(cuts, N_cuts);
    if (!denom_OK)
        fout << "Error: 0 events in the denominator!" << std::endl;
    for (int i = 0; i < N_cuts; i++)
    {
        if (denom_OK)
            fout << cuts[i] << std::setw(maxL + 5 - int(cuts[i].size())) << "  |  " << double(extra_chain->GetEntries(cuts[i].c_str())) / N_E << std::endl;
    }
    //Close files and clean memory
    fout.close();
    if (denom_OK)
    {
        for (auto &chain : ref_chain)
            CloseChain(chain);
        CloseChain(extra_chain);
    }
    delete[] filenames;
    delete[] cuts;
    return;
}

#if !defined(__CLING__)
int main(int argc, char **argv)
{
    switch (argc - 1)
    {
    case 3:
        Multiplicity_Extrasel(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])));
        break;
    case 4:
        Multiplicity_Extrasel(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])));
        break;
    case 5:
        Multiplicity_Extrasel(*(new std::string(argv[1])), *(new std::string(argv[2])), *(new std::string(argv[3])), *(new std::string(argv[4])), *(new std::string(argv[5])));
        break;
    default:
        std::cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << std::endl;
        return (1);
        break;
    }
    return 0;
}
#endif