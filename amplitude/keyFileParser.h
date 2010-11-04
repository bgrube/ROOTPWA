///////////////////////////////////////////////////////////////////////////
//
//    Copyright 2010
//
//    This file is part of rootpwa
//
//    rootpwa is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    rootpwa is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with rootpwa. If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// File and Version Information:
// $Rev::                             $: revision of last commit
// $Author::                          $: author of last commit
// $Date::                            $: date of last commit
//
// Description:
//      singleton class that constructs decay topologies according to key files
//
//
// Author List:
//      Boris Grube          TUM            (original author)
//
//
//-------------------------------------------------------------------------


#ifndef KEYFILEPARSER_H
#define KEYFILEPARSER_H


#include <string>
#include <vector>

#include "libconfig.h++"

#include "isobarDecayTopology.h"
#include "isobarAmplitude.h"


namespace rpwa {


  class keyFileParser {

  public:

    static keyFileParser& instance() { return _instance; }  ///< get singleton instance
  
    static bool parse(const std::string& keyFileName);  ///< parses key file and constructs decay topology

	  static bool constructDecayTopology(isobarDecayTopologyPtr& topo,
	                                     const bool              requireXQnKey = true);  ///< construct isobar decay topology from keyfile

	  static bool constructAmplitude(isobarAmplitudePtr&           amplitude);   ///< construct isobar decay amplitude from keyfile
	  static bool constructAmplitude(isobarAmplitudePtr&           amplitude,
	                                 const isobarDecayTopologyPtr& topo);  ///< construct isobar amplitude using existing decay topology

	  static bool writeKeyFile(const std::string&         keyFileName,
	                           const isobarAmplitude&     amplitude,
	                           const bool                 writeProdVert = true);  ///< creates key file from amplitude
	  static bool writeKeyFile(const std::string&         keyFileName,
	                           const isobarDecayTopology& topo,
	                           const bool                 writeProdVert = true);  ///< creates key file from decay topology

    static const libconfig::Setting* findGroup(const libconfig::Setting& parent,
                                               const std::string&        groupName,
                                               const bool                mustExist = true);  ///< finds field in keyfile and makes sure it is a group

    static const libconfig::Setting* findList(const libconfig::Setting& parent,
                                              const std::string&        listName,
                                              const bool                mustExist = true);  ///< finds field in keyfile and makes sure it is a non-empty list

    static const libconfig::Setting* findArray(const libconfig::Setting& parent,
                                               const std::string&        arrayName,
                                               const bool                mustExist = true);  ///< finds field in keyfile and makes sure it is a non-empty array

	  static std::string keyFileNameFromTopology
	  (const isobarDecayTopology&  topo,
	   const isobarDecayVertexPtr& currentVertex = isobarDecayVertexPtr());  ///< recursive funstion that generates unique key file name for wave from decay topology
	  static std::string oldKeyFileNameFromTopology
	  (isobarDecayTopology&        topo,
	   const isobarDecayVertexPtr& currentVertex = isobarDecayVertexPtr());  ///< recursive funstion that generates unique key file name (old convention) for wave from decay topology

    static bool debug() { return _debug; }                             ///< returns debug flag
    static void setDebug(const bool debug = true) { _debug = debug; }  ///< sets debug flag


  private:

    keyFileParser () { }
    ~keyFileParser() { }
    keyFileParser (const keyFileParser&);
    keyFileParser& operator =(const keyFileParser&);

	  static isobarAmplitudePtr mapAmplitudeType(const std::string&            formalismType,
	                                             const isobarDecayTopologyPtr& topo);  ///< creates amplitude for specified formalism
	  static bool setAmplitude(libconfig::Setting&    amplitudeKey,
	                           const isobarAmplitude& amplitude);  ///< puts amplitude specification into key

    static bool constructXParticle(const libconfig::Setting& XQnKey,
                                   particlePtr&              X);  ///< creates X particle with quantum numbers defined in X key
	  static bool setXQuantumNumbersKeys(libconfig::Setting& XQnKey,
	                                     const particle&     X);  ///< puts X quantum numbers into keys

    static bool constructParticle(const libconfig::Setting& particleKey,
                                  particlePtr&              particle);  ///< creates particle using name in particle key
    
    static bool constructDecayVertex(const libconfig::Setting&          parentKey,
                                     const particlePtr&                 parentParticle,
                                     std::vector<isobarDecayVertexPtr>& decayVertices,
                                     std::vector<particlePtr>&          fsParticles);  ///< recursively traverses decay chain and creates decay vertices and final state particles
    static massDependencePtr mapMassDependenceType(const std::string& massDepType);  ///< creates mass dependence functor of specified type
	  static bool setMassDependence(libconfig::Setting&   isobarMassDepKey,
	                                const massDependence& massDep);  ///< puts mass dependence into key


    static bool constructProductionVertex(const libconfig::Setting& rootKey,
                                          const particlePtr&        X,
                                          productionVertexPtr&      prodVert);  ///< creates production vertex
    static bool mapProductionVertexType(const libconfig::Setting& prodVertKey,
                                        const std::string&        vertType,
                                        const particlePtr&        X,
                                        productionVertexPtr&      prodVert);  ///< creates production vertex according to type and list of production kinematics particles
    static bool setProductionVertexKeys(libconfig::Setting&        prodVertKey,
                                        const productionVertexPtr& prodVert);  ///< puts production vertex info into keys

	  static bool setXDecayKeys(libconfig::Setting&        parentDecayKey,
	                            const isobarDecayTopology& topo,
	                            const isobarDecayVertex&   vert);  ///< recursive function that puts X decay chain into keys

	  static bool writeKeyFile(libconfig::Setting&        rootKey,
	                           const isobarAmplitude&     amplitude);  ///< writes amplitude parameters to key file
	  static bool writeKeyFile(libconfig::Setting&        rootKey,
	                           const isobarDecayTopology& topo,
	                           const bool                 writeProdVert = true);  ///< writes decay topology to key file

    static keyFileParser _instance;  ///< singleton instance

	  static libconfig::Config         _key;            ///< key file
	  static const libconfig::Setting* _amplitudeKey;   ///< pointer to amplitude options

    static bool _debug;  ///< if set to true, debug messages are printed

  };


}  // namespace rpwa


#endif  // KEYFILEPARSER_H
