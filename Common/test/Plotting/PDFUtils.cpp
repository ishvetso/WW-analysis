/*
* create an envelope for the list of histogram
*
*/
#include <PDFUtils.hpp>



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
	hist -> Sumw2();

	for (int iBin = 1; iBin <= Nbins; iBin++ )
	{
		std::vector<double> ValuesPerBin;
		std::vector<double> ErrorsPerBin;
		for (unsigned int iHist = 0; iHist < hists.size(); iHist ++)
		{
			ValuesPerBin.push_back(hists.at(iHist)->GetBinContent(iBin));
			ErrorsPerBin.push_back(hists.at(iHist)->GetBinError(iBin));
		}
		double MaxValue = *std::max_element(ValuesPerBin.begin(), ValuesPerBin.end());
		double MinValue = *std::min_element(ValuesPerBin.begin(), ValuesPerBin.end());

		//get the index of the max and min value in the vector
		int indexOfMaxValue = std::distance(ValuesPerBin.begin(), std::max_element(ValuesPerBin.begin(), ValuesPerBin.end()));
		int indexOfMinValue = std::distance(ValuesPerBin.begin(), std::min_element(ValuesPerBin.begin(), ValuesPerBin.end()));

		
		if(variation == "up"){
			hist -> SetBinContent(iBin,MaxValue);
			hist -> SetBinError(iBin,ErrorsPerBin.at(indexOfMaxValue));
		}
		else if (variation == "down"){
			hist -> SetBinContent(iBin,MinValue);
			hist -> SetBinError(iBin,ErrorsPerBin.at(indexOfMinValue));
		}
		else {
			std::cerr << "Wrong type of variation, please use up or down" << std::endl;
		}
		ValuesPerBin.clear();
		ErrorsPerBin.clear();
	}

	return hist;
}

 struct  {
        bool operator()(std::pair<double, double> a, std::pair<double, double> b )
        {   
            return a.first < b.first;
        }   
    } ComparePairs;

TH1D * makePDF4LHC(std::vector<TH1D*> hists, std::string variation){
	if (hists.size()!= 100)	throw std::runtime_error("you should have exactly 100 variations according to PDF4LHC recommendations");
		
	int Nbins = hists[0]->GetNbinsX();
	std::string name = random_string(10);
	TH1D *hist = new TH1D( (name + variation).c_str(),(name + variation).c_str(), Nbins,hists[0]->GetXaxis()->GetBinLowEdge(1),hists[0]->GetXaxis()->GetBinUpEdge(hists[0]->GetNbinsX()));
	hist -> Sumw2();

	for (int iBin = 1; iBin <= Nbins; iBin++ )
	{
		std::vector<std::pair<double,double>> ValuesAndErrorsPerBin;
	
		for (unsigned int iHist = 0; iHist < hists.size(); iHist ++)
		{
			std::pair <double,double> ValueAndError;
			ValueAndError = std::make_pair(hists.at(iHist)->GetBinContent(iBin),hists.at(iHist)->GetBinError(iBin));
			ValuesAndErrorsPerBin.push_back(ValueAndError);
		}

		std::sort(ValuesAndErrorsPerBin.begin(), ValuesAndErrorsPerBin.end(),ComparePairs);		
		
		if(variation == "up"){
			hist -> SetBinContent(iBin,ValuesAndErrorsPerBin.at(83).first);
			hist -> SetBinError(iBin,ValuesAndErrorsPerBin.at(83).second);
		}
		else if (variation == "down"){
			hist -> SetBinContent(iBin,ValuesAndErrorsPerBin.at(15).first);
			hist -> SetBinError(iBin,ValuesAndErrorsPerBin.at(15).second);
		}
		else {
			std::cerr << "Wrong type of variation, please use up or down" << std::endl;
		}
		ValuesAndErrorsPerBin.clear();
		
	}

	return hist;
}
