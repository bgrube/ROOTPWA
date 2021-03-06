#ifndef EVIDENCECALCULATOR_H
#define EVIDENCECALCULATOR_H

#include<cmath>
#include<complex>
#include<vector>


class TRandom3;

namespace rpwa {

	class ampIntegralMatrix;

	class parameterSpace {

	  public:

		parameterSpace(const ampIntegralMatrix& integralMatrix);

		~parameterSpace();

		void pickUniformAngles();
		void setAngles(const std::vector<double>& phi);

		double getR() const { return std::sqrt( _nEvents / getRho() ); }
		double getDRDPhi(const unsigned int& phiIndex) const { return _DRDPhi[phiIndex]; }

		double getDS(const unsigned int& phiIndex) const;
		double getDSHat(const unsigned int& phiIndex) const { return getDS(phiIndex) / std::sqrt(_nEvents); }
		double getDA() const;
		double getDAHat() const;
		double getDASphereHat() const;
		double getDARatio() const { return getDAHat() / getDASphereHat(); }

		// get the spherical coordinates in the form (r, phi_1, phi_2, ..., phi_(n-1))
		static std::vector<double> convertToSphericalCoordinates(const std::vector<double> x);
		// the reverse transformation, the input vector should be in the same format as above
		static std::vector<double> convertToCartesianCoordinates(const std::vector<double> sphereX);

	  private:

		void initialize() { calculateRho(); calculateAllDSigmaDPhi(); calculateAllDRDPhi(); }

		void calculateAllDRDPhi();
		double calculateDRDPhi(const unsigned int& phiIndex) const;

		void calculateRho();
		const double& getRho() const { return _rho; }

		double calculateDRhoDPhi(const unsigned int& phiIndex) const;

		double calculateSigma(const unsigned int& index) const;
		void calculateAllDSigmaDPhi();
		double calculateDSigmaDPhi(const unsigned int& sigmaIndex, const unsigned int& phiIndex) const;
		inline double getDSigmaDPhi(const unsigned int& sigmaIndex, const unsigned int& phiIndex) const {
			return _DSigmaDPhi[sigmaIndex][phiIndex];
		}

		const unsigned int _nEvents;
		const unsigned int _nDimensions;
		std::vector<std::vector <std::complex<double> > > _integralMatrix;

		std::vector<double> _phi;
		std::vector<double> _cosPhi;
		std::vector<double> _sinPhi;
		std::vector<double> _sigma;

		double _rho;
		std::vector<double> _DRDPhi;
		std::vector<std::vector<double> > _DSigmaDPhi;

		TRandom3* _randomNumberGen;


	};

}

#endif
