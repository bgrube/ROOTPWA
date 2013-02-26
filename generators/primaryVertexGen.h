/** @addtogroup generators
 * @{
 */

#ifndef TPRIMARYVERTEXGEN_HH
#define TPRIMARYVERTEXGEN_HH

#include <string>

#include <TVector3.h>
#include <TLorentzVector.h>

class TFile;
class TTree;


namespace rpwa {

	class beamAndVertexGenerator {

	  public:

		beamAndVertexGenerator(std::string rootFileName,
		                 double massBeamParticle);

		~beamAndVertexGenerator();

		bool check();

		bool event();

		const TVector3& getVertex() { return _vertex; }

		const TLorentzVector& getBeam() { return _beam; }

	  private:

		TFile* _rootFile;
		TTree* _beamTree;
		TVector3 _vertex;
		TLorentzVector _beam;

		double _massBeamParticle;
		bool _sigmasPresent;

	};

}

#endif
/* @} **/
