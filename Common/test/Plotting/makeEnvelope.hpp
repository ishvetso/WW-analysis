/*
* create an envelope for the list of histogram
*
*/
#include <algorithm>

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

TH1D * makeEnvelope(std::vector<TH1D*> hists, std::string variation){
	int Nbins = hists[0]->GetNbinsX();
	std::string name = random_string(10);
	TH1D *hist = new TH1D( (name + variation).c_str(),(name + variation).c_str(), Nbins,hists[0]->GetXaxis()->GetBinLowEdge(1),hists[0]->GetXaxis()->GetBinUpEdge(hists[0]->GetNbinsX()));

	for (int iBin = 1; iBin <= Nbins; iBin++ )
	{
		std::vector<double> ValuesPerBin;
		for (unsigned int iHist = 0; iHist < hists.size(); iHist ++)
		{
			ValuesPerBin.push_back(hists.at(iHist)->GetBinContent(iBin));
		}
		double MaxValue = *std::max_element(ValuesPerBin.begin(), ValuesPerBin.end());
		double MinValue = *std::min_element(ValuesPerBin.begin(), ValuesPerBin.end());
		if(variation == "up")hist -> SetBinContent(iBin,MaxValue);
		else if (variation == "down")hist -> SetBinContent(iBin,MinValue);
		else {
			std::cerr << "Wrong type of variation, please use up or down" << std::endl;
		}
		ValuesPerBin.clear();
	}

	return hist;
}