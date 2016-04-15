/*
* scale factors for HLT_Mu50_v*
* values are taken from AN-2015-223, v8, page 16, count method, without mass window, Run D, 74X
*/
namespace Mu50{
	
	double scaleFactor(double pt, double eta)
	{
		double SF;
		if ( pt >= 53. && pt < 140.)
		{	
			if (std::abs(eta) < 0.9) SF = 0.9706;
			else if (std::abs(eta) >= 0.9 && std::abs(eta) < 1.2) SF = 0.964;
			else if (std::abs(eta) >= 1.2 && std::abs(eta) < 2.1) SF = 0.9608;
			else if (std::abs(eta) >= 2.1 && std::abs(eta) < 2.4) SF = 0.9512;
			else throw cms::Exception("InvalidValue") <<  " not supported eta for HLT_Mu50_v* !!! eta : " << eta << std::endl;	

		}
		else if ( pt >= 140.)
		{
			if (std::abs(eta) < 0.9) SF = 0.9708;
			else if (std::abs(eta) >= 0.9 && std::abs(eta) < 1.2) SF = 0.9986;
			else if (std::abs(eta) >= 1.2 && std::abs(eta) < 2.1) SF = 0.9989;
			else if (std::abs(eta) >= 2.1 && std::abs(eta) < 2.4) SF = 1.039;
			else throw cms::Exception("InvalidValue") <<  " not supported eta for HLT_Mu50_v* !!! eta : " << eta << std::endl;				
		}
		else throw cms::Exception("InvalidValue") <<  " not supported pt for HLT_Mu50_v* !!! pt : " << pt << std::endl;	
		return SF;
	}

}