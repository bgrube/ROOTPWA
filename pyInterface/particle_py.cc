#include<particle_py.h>

#include<TLorentzRotation.h>
#include<TPython.h>
#include<TVector3.h>

#include<rootConverters_py.h>

namespace bp = boost::python;

namespace {

	struct particleWrapper : public rpwa::particle,
	                                bp::wrapper<rpwa::particle>
	{

		particleWrapper()
			: rpwa::particle(),
			  bp::wrapper<rpwa::particle>() { };

		particleWrapper(const rpwa::particle& part)
			: rpwa::particle(part),
			  bp::wrapper<rpwa::particle>() { };

		particleWrapper(const rpwa::particleProperties& partProp,
		                const int                       index    = -1,
		                const int                       spinProj = 0,
		                const int                       refl     = 0,
		                const bp::object&               momentum = bp::object())
			: rpwa::particle(partProp, index, spinProj, refl),
			  bp::wrapper<rpwa::particle>()
		{
			if(!(momentum.is_none())) {
				rpwa::particle::setMomentum(*(rpwa::py::convertFromPy<TVector3*>(momentum.ptr())));
			}
		};

		particleWrapper(const std::string&        partName,
		                const bool                requirePartInTable = true,
		                const int                 index              = -1,
		                const int                 spinProj           = 0,
		                const int                 refl               = 0,
		                const bp::object&         momentum           = bp::object())
			: rpwa::particle(partName, requirePartInTable, index, spinProj, refl),
			  bp::wrapper<rpwa::particle>()
		{ 
			if(!(momentum.is_none())) {
				rpwa::particle::setMomentum(*(rpwa::py::convertFromPy<TVector3*>(momentum.ptr())));
			}
		};

		particleWrapper(const std::string&        partName,
		                const int                 isospin,
		                const int                 G,
		                const int                 J,
		                const int                 P,
		                const int                 C,
		                const int                 spinProj,
		                const int                 refl  = 0,
		                const int                 index = -1)
			: rpwa::particle(partName, isospin, G, J, P, C, spinProj, refl, index),
			  bp::wrapper<rpwa::particle>() { };

		bool read__(bp::object& pyLine) {
			std::string strLine = bp::extract<std::string>(pyLine);
			std::istringstream sstrLine(strLine, std::istringstream::in);
			return rpwa::particleProperties::read(sstrLine);
		};

		PyObject* momentum() const {
			return rpwa::py::convertToPy<TVector3>(particle::momentum());
		};

		void setMomentum(PyObject* pyMom) {
			TVector3* newMom = rpwa::py::convertFromPy<TVector3*>(pyMom);
			if(newMom == NULL) {
				printErr<<"Got invalid input when executing rpwa::particle::setMomentum()."<<std::endl;
			} else {
				rpwa::particle::setMomentum(*newMom);
			}
		};

		PyObject* transform__(PyObject* pyTrafo) {
			TVector3* trafoTV3 = rpwa::py::convertFromPy<TVector3*>(pyTrafo);
			if(trafoTV3 != NULL) {
				return rpwa::py::convertToPy<TLorentzVector>(rpwa::particle::transform(*trafoTV3));
			}
			TLorentzRotation* trafoTLR = rpwa::py::convertFromPy<TLorentzRotation*>(pyTrafo);
			if(trafoTLR != NULL) {
				return rpwa::py::convertToPy<TLorentzVector>(rpwa::particle::transform(*trafoTLR));
			}
			printErr<<"Got invalid input when executing rpwa::particle::transform()."<<std::endl;
			return NULL;
		};

		std::string qnSummary() const {
			if(bp::override qnSummary = this->get_override("qnSummary")) {
				return qnSummary();
			}
			return rpwa::particle::qnSummary();
		}

		std::string default_qnSummary() const {
			return rpwa::particle::qnSummary();
		}

		std::string label() const {
			if(bp::override label = this->get_override("label")) {
				return label();
			}
			return rpwa::particle::label();
		};

		std::string default_label() const {
			return rpwa::particle::label();
		};

	};

}

void rpwa::py::exportParticle() {

	bp::class_<particleWrapper, bp::bases<rpwa::particleProperties> >("particle")

		.def(bp::init<particleWrapper&>())
		.def(bp::init<rpwa::particleProperties&, bp::optional<int, int, int, const bp::object&> >())
		.def(bp::init<std::string, bp::optional<bool, int, int, int, bp::object&> >())
		.def(bp::init<std::string, int, int, int, int, int, int, bp::optional<int, int> >())

		.def(bp::self_ns::str(bp::self))

		.def(
			"clone"
			, &particleWrapper::clone
		)

		.add_property("spinProj", &particleWrapper::spinProj, &particleWrapper::setSpinProj)
		.add_property("momentum", &particleWrapper::momentum, &particleWrapper::setMomentum)

		.def("transform", &particleWrapper::transform__)

		.def("qnSummary", &particleWrapper::qnSummary, &particleWrapper::default_qnSummary)

		.def("read", &particleWrapper::read__)

		.def("label", &particleWrapper::label, &particleWrapper::default_label);

	bp::register_ptr_to_python< rpwa::particlePtr >();

};

