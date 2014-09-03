
#include "eventMetadata.h"
#include "reportingUtils.hpp"


using namespace std;
using namespace rpwa;


rpwa::eventMetadata::eventMetadata()
	: _userString(""),
	  _contentHash(""),
	  _productionKinematicsParticleNames(),
	  _decayKinematicsParticleNames(),
	  _binningMap()
{ }


rpwa::eventMetadata::~eventMetadata() { };


ostream& rpwa::eventMetadata::print(ostream& out) const
{
	out << "dataMetadata: " << endl
	    << "    userString ...................... '" << _userString << "'"         << endl
	    << "    contentHash ..................... '" << _contentHash << "'"        << endl
	    << "    initial state particle names: ... "  << _productionKinematicsParticleNames << endl
        << "    final state particle names: ..... "  << _decayKinematicsParticleNames   << endl
	    << "    binning map";
	if(_binningMap.empty()) {
		out << " ..................... " << "<empty>" << endl;
	} else {
		out << ": " << endl;
		for(binningMapType::const_iterator it = _binningMap.begin(); it != _binningMap.end(); ++it) {
			out << "        variable '" << it->first << "' range " << it->second << endl;
		}
	}
	return out;
}


void rpwa::eventMetadata::appendToUserString(const string& userString,
                                             const string& delimiter)
{
	if(_userString == "") {
		_userString = userString;
	} else {
		stringstream strStr;
		strStr << _userString << delimiter << userString;
		_userString = strStr.str();
	}
}


void rpwa::eventMetadata::setProductionKinematicsParticleNames(const vector<string>& productionKinematicsNames)
{
	_productionKinematicsParticleNames = productionKinematicsNames;
}


void rpwa::eventMetadata::setDecayKinematicsParticleNames(const vector<string>& decayKinematicsParticleNames)
{
	_decayKinematicsParticleNames = decayKinematicsParticleNames;
}


void rpwa::eventMetadata::setBinningVariableLabels(const vector<string>& labels)
{
	for(unsigned int i = 0; i < labels.size(); ++i) {
		_binningMap[labels[i]] = rangePairType(0., 0.);
	}
}


void rpwa::eventMetadata::setBinningVariableRange(const string& label, const rangePairType& range)
{
	_binningMap[label] = range;
}


void rpwa::eventMetadata::setBinningMap(const binningMapType& binningMap)
{
	_binningMap = binningMap;
}