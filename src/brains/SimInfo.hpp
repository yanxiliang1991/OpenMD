 /*
 * Copyright (c) 2005 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Acknowledgement of the program authors must be made in any
 *    publication of scientific results based in part on use of the
 *    program.  An acceptable form of acknowledgement is citation of
 *    the article in which the program was described (Matthew
 *    A. Meineke, Charles F. Vardeman II, Teng Lin, Christopher
 *    J. Fennell and J. Daniel Gezelter, "OOPSE: An Object-Oriented
 *    Parallel Simulation Engine for Molecular Dynamics,"
 *    J. Comput. Chem. 26, pp. 252-271 (2005))
 *
 * 2. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. All express or implied conditions, representations and
 * warranties, including any implied warranty of merchantability,
 * fitness for a particular purpose or non-infringement, are hereby
 * excluded.  The University of Notre Dame and its licensors shall not
 * be liable for any damages suffered by licensee as a result of
 * using, modifying or distributing the software or its
 * derivatives. In no event will the University of Notre Dame or its
 * licensors be liable for any lost revenue, profit or data, or for
 * direct, indirect, special, consequential, incidental or punitive
 * damages, however caused and regardless of the theory of liability,
 * arising out of the use of or inability to use software, even if the
 * University of Notre Dame has been advised of the possibility of
 * such damages.
 */
 
/**
 * @file SimInfo.hpp
 * @author    tlin
 * @date  11/02/2004
 * @version 1.0
 */ 

#ifndef BRAINS_SIMMODEL_HPP
#define BRAINS_SIMMODEL_HPP

#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include "brains/Exclude.hpp"
#include "io/Globals.hpp"
#include "math/Vector3.hpp"
#include "types/MoleculeStamp.hpp"
#include "UseTheForce/ForceField.hpp"
#include "utils/PropertyMap.hpp"
#include "utils/LocalIndexManager.hpp"

//another nonsense macro declaration
#define __C
#include "brains/fSimulation.h"

namespace oopse{

//forward decalration 
class SnapshotManager;
class Molecule;
class SelectionManager;
/**
 * @class SimInfo SimInfo.hpp "brains/SimInfo.hpp" 
 * @brief As one of the heavy weight class of OOPSE, SimInfo
 * One of the major changes in SimInfo class is the data struct. It only maintains a list of molecules.
 * And the Molecule class will maintain all of the concrete objects (atoms, bond, bend, torsions, rigid bodies,
 * cutoff groups, constrains).
 * Another major change is the index. No matter single version or parallel version,  atoms and
 * rigid bodies have both global index and local index. Local index is not important to molecule as well as
 * cutoff group. 
 */
class SimInfo {
    public:
        typedef std::map<int, Molecule*>::iterator  MoleculeIterator;

        /**
         * Constructor of SimInfo
         * @param molStampPairs MoleculeStamp Array. The first element of the pair is molecule stamp, the
         * second element is the total number of molecules with the same molecule stamp in the system
         * @param ff pointer of a concrete ForceField instance
         * @param simParams 
         * @note
         */
        SimInfo(std::vector<std::pair<MoleculeStamp*, int> >& molStampPairs, ForceField* ff, Globals* simParams);
        virtual ~SimInfo();

        /**
         * Adds a molecule
         * @return return true if adding successfully, return false if the molecule is already in SimInfo
         * @param mol molecule to be added
         */
        bool addMolecule(Molecule* mol);

        /**
         * Removes a molecule from SimInfo
         * @return true if removing successfully, return false if molecule is not in this SimInfo
         */
        bool removeMolecule(Molecule* mol);

        /** Returns the total number of molecules in the system. */
        int getNGlobalMolecules() {
            return nGlobalMols_;
        }

        /** Returns the total number of atoms in the system. */
        int getNGlobalAtoms() {
            return nGlobalAtoms_;
        }

        /** Returns the total number of cutoff groups in the system. */
        int getNGlobalCutoffGroups() {
            return nGlobalCutoffGroups_;
        }

        /**
         * Returns the total number of integrable objects (total number of rigid bodies plus the total number
         * of atoms which do not belong to the rigid bodies) in the system
         */
        int getNGlobalIntegrableObjects() {
            return nGlobalIntegrableObjects_;
        }

        /**
         * Returns the total number of integrable objects (total number of rigid bodies plus the total number
         * of atoms which do not belong to the rigid bodies) in the system
         */
        int getNGlobalRigidBodies() {
            return nGlobalRigidBodies_;
        }

        int getNGlobalConstraints();
        /** 
         * Returns the number of local molecules.
         * @return the number of local molecules 
         */
        int getNMolecules() {
            return molecules_.size();
        }

        /** Returns the number of local atoms */
        unsigned int getNAtoms() {
            return nAtoms_;
        }

        /** Returns the number of local bonds */        
        unsigned int getNBonds(){
            return nBonds_;
        }

        /** Returns the number of local bends */        
        unsigned int getNBends() {
            return nBends_;
        }

        /** Returns the number of local torsions */        
        unsigned int getNTorsions() {
            return nTorsions_;
        }

        /** Returns the number of local rigid bodies */        
        unsigned int getNRigidBodies() {
            return nRigidBodies_;
        }

        /** Returns the number of local integrable objects */
        unsigned int getNIntegrableObjects() {
            return nIntegrableObjects_;
        }

        /** Returns the number of local cutoff groups */
        unsigned int getNCutoffGroups() {
            return nCutoffGroups_;
        }

        /** Returns the total number of constraints in this SimInfo */
        unsigned int getNConstraints() {
            return nConstraints_;
        }
        
        /**
         * Returns the first molecule in this SimInfo and intialize the iterator.
         * @return the first molecule, return NULL if there is not molecule in this SimInfo
         * @param i the iterator of molecule array (user shouldn't change it)
         */
        Molecule* beginMolecule(MoleculeIterator& i);

        /** 
          * Returns the next avaliable Molecule based on the iterator.
          * @return the next avaliable molecule, return NULL if reaching the end of the array 
          * @param i the iterator of molecule array
          */
        Molecule* nextMolecule(MoleculeIterator& i);

        /** Returns the number of degrees of freedom */
        int getNdf() {
            return ndf_;
        }

        /** Returns the number of raw degrees of freedom */
        int getNdfRaw() {
            return ndfRaw_;
        }

        /** Returns the number of translational degrees of freedom */
        int getNdfTrans() {
            return ndfTrans_;
        }

        //getNZconstraint and setNZconstraint ruin the coherent of SimInfo class, need refactorying
        
        /** Returns the total number of z-constraint molecules in the system */
        int getNZconstraint() {
            return nZconstraint_;
        }

        /** 
         * Sets the number of z-constraint molecules in the system.
         */
        void setNZconstraint(int nZconstraint) {
            nZconstraint_ = nZconstraint;
        }
        
        /** Returns the snapshot manager. */
        SnapshotManager* getSnapshotManager() {
            return sman_;
        }

        /** Sets the snapshot manager. */
        void setSnapshotManager(SnapshotManager* sman);
        
        /** Returns the force field */
        ForceField* getForceField() {
            return forceField_;
        }

        Globals* getSimParams() {
            return simParams_;
        }

        /** Returns the velocity of center of mass of the whole system.*/
        Vector3d getComVel();

        /** Returns the center of the mass of the whole system.*/
        Vector3d getCom();

        /** Returns the seed (used for random number generator) */
        int getSeed() {
            return seed_;
        }

        /** Sets the seed*/
        void setSeed(int seed) {
            seed_ = seed;
        }

        /** main driver function to interact with fortran during the initialization and molecule migration */
        void update();

        /** Returns the local index manager */
        LocalIndexManager* getLocalIndexManager() {
            return &localIndexMan_;
        }

        int getMoleculeStampId(int globalIndex) {
            //assert(globalIndex < molStampIds_.size())
            return molStampIds_[globalIndex];
        }

        /** Returns the molecule stamp */
        MoleculeStamp* getMoleculeStamp(int id) {
            return moleculeStamps_[id];
        }

        /** Return the total number of the molecule stamps */
        int getNMoleculeStamp() {
            return moleculeStamps_.size();
        }
        /**
         * Finds a molecule with a specified global index
         * @return a pointer point to found molecule
         * @param index
         */
        Molecule* getMoleculeByGlobalIndex(int index) {
            MoleculeIterator i;
            i = molecules_.find(index);

            return i != molecules_.end() ? i->second : NULL;
        }

        /** Calculate the maximum cutoff radius based on the atom types */
        double calcMaxCutoffRadius();

        double getRcut() {
            return rcut_;
        }

        double getRsw() {
            return rsw_;
        }
        
        std::string getFinalConfigFileName() {
            return finalConfigFileName_;
        }
        
        void setFinalConfigFileName(const std::string& fileName) {
            finalConfigFileName_ = fileName;
        }

        std::string getDumpFileName() {
            return dumpFileName_;
        }
        
        void setDumpFileName(const std::string& fileName) {
            dumpFileName_ = fileName;
        }

        std::string getStatFileName() {
            return statFileName_;
        }
        
        void setStatFileName(const std::string& fileName) {
            statFileName_ = fileName;
        }

        /** 
         * Sets GlobalGroupMembership
         * @see #SimCreator::setGlobalIndex
         */  
        void setGlobalGroupMembership(const std::vector<int>& globalGroupMembership) {
            assert(globalGroupMembership.size() == nGlobalAtoms_);
            globalGroupMembership_ = globalGroupMembership;
        }

        /** 
         * Sets GlobalMolMembership
         * @see #SimCreator::setGlobalIndex
         */        
        void setGlobalMolMembership(const std::vector<int>& globalMolMembership) {
            assert(globalMolMembership.size() == nGlobalAtoms_);
            globalMolMembership_ = globalMolMembership;
        }


        bool isFortranInitialized() {
            return fortranInitialized_;
        }
        
        //below functions are just forward functions
        //To compose or to inherit is always a hot debate. In general, is-a relation need subclassing, in the
        //the other hand, has-a relation need composing.
        /**
         * Adds property into property map
         * @param genData GenericData to be added into PropertyMap
         */
        void addProperty(GenericData* genData);

        /**
         * Removes property from PropertyMap by name
         * @param propName the name of property to be removed
         */
        void removeProperty(const std::string& propName);

        /**
         * clear all of the properties
         */
        void clearProperties();

        /**
         * Returns all names of properties
         * @return all names of properties
         */
        std::vector<std::string> getPropertyNames();

        /**
         * Returns all of the properties in PropertyMap
         * @return all of the properties in PropertyMap
         */      
        std::vector<GenericData*> getProperties();

        /**
         * Returns property 
         * @param propName name of property
         * @return a pointer point to property with propName. If no property named propName
         * exists, return NULL
         */      
        GenericData* getPropertyByName(const std::string& propName);

        /**
         * add all exclude pairs of a molecule into exclude list.
         */
        void addExcludePairs(Molecule* mol);

        /**
         * remove all exclude pairs which belong to a molecule from exclude list
         */

        void removeExcludePairs(Molecule* mol);


        SelectionManager* getSelectionManager() {
            return selectMan_;
        }
        
        friend std::ostream& operator <<(std::ostream& o, SimInfo& info);
        
    private:

        
        /** Returns the unique atom types of local processor in an array */
        std::set<AtomType*> getUniqueAtomTypes();

        /** fill up the simtype struct*/
        void setupSimType();

        /**
         * Setup Fortran Simulation
         * @see #setupFortranParallel
         */
        void setupFortranSim();

        /** Figure out the radius of cutoff, radius of switching function and pass them to fortran */
        void setupCutoff();

        /** Calculates the number of degress of freedom in the whole system */
        void calcNdf();
        void calcNdfRaw();
        void calcNdfTrans();

        /**
         * Adds molecule stamp and the total number of the molecule with same molecule stamp in the whole
         * system.
         */
        void addMoleculeStamp(MoleculeStamp* molStamp, int nmol);

        ForceField* forceField_;      
        Globals* simParams_;

        std::map<int, Molecule*>  molecules_; /**< Molecule array */
        
        //degress of freedom
        int ndf_;           /**< number of degress of freedom (excludes constraints),  ndf_ is local */
        int ndfRaw_;    /**< number of degress of freedom (includes constraints),  ndfRaw_ is local */
        int ndfTrans_; /**< number of translation degress of freedom, ndfTrans_ is local */
        int nZconstraint_; /** number of  z-constraint molecules, nZconstraint_ is global */
        
        //number of global objects
        int nGlobalMols_;       /**< number of molecules in the system */
        int nGlobalAtoms_;   /**< number of atoms in the system */
        int nGlobalCutoffGroups_; /**< number of cutoff groups in this system */
        int nGlobalIntegrableObjects_; /**< number of integrable objects in this system */
        int nGlobalRigidBodies_; /**< number of rigid bodies in this system */
        /**
         * the size of globalGroupMembership_  is nGlobalAtoms. Its index is  global index of an atom, and the
         * corresponding content is the global index of cutoff group this atom belong to. 
         * It is filled by SimCreator once and only once, since it never changed during the simulation.
         */
        std::vector<int> globalGroupMembership_; 

        /**
         * the size of globalGroupMembership_  is nGlobalAtoms. Its index is  global index of an atom, and the
         * corresponding content is the global index of molecule this atom belong to. 
         * It is filled by SimCreator once and only once, since it is never changed during the simulation.
         */
        std::vector<int> globalMolMembership_;        

        
        std::vector<int> molStampIds_;                                /**< stamp id array of all molecules in the system */
        std::vector<MoleculeStamp*> moleculeStamps_;      /**< molecule stamps array */        
        
        //number of local objects
        int nAtoms_;                        /**< number of atoms in local processor */
        int nBonds_;                        /**< number of bonds in local processor */
        int nBends_;                        /**< number of bends in local processor */
        int nTorsions_;                    /**< number of torsions in local processor */
        int nRigidBodies_;              /**< number of rigid bodies in local processor */
        int nIntegrableObjects_;    /**< number of integrable objects in local processor */
        int nCutoffGroups_;             /**< number of cutoff groups in local processor */
        int nConstraints_;              /**< number of constraints in local processors */

        simtype fInfo_; /**< A dual struct shared by c++/fortran which indicates the atom types in simulation*/
        Exclude exclude_;      
        PropertyMap properties_;                  /**< Generic Property */
        SnapshotManager* sman_;               /**< SnapshotManager */

        int seed_; /**< seed for random number generator */

        /** 
         * The reason to have a local index manager is that when molecule is migrating to other processors, 
         * the atoms and the rigid-bodies will release their local indices to LocalIndexManager. Combining the
         * information of molecule migrating to current processor, Migrator class can query  the LocalIndexManager
         * to make a efficient data moving plan.
         */        
        LocalIndexManager localIndexMan_;

        //file names
        std::string finalConfigFileName_;
        std::string dumpFileName_;
        std::string statFileName_;

        double rcut_;       /**< cutoff radius*/
        double rsw_;        /**< radius of switching function*/

        bool fortranInitialized_; /**< flag indicate whether fortran side is initialized */

        SelectionManager* selectMan_;
#ifdef IS_MPI
    //in Parallel version, we need MolToProc
    public:
                
        /**
         * Finds the processor where a molecule resides
         * @return the id of the processor which contains the molecule
         * @param globalIndex global Index of the molecule
         */
        int getMolToProc(int globalIndex) {
            //assert(globalIndex < molToProcMap_.size());
            return molToProcMap_[globalIndex];
        }

        /** 
         * Set MolToProcMap array
         * @see #SimCreator::divideMolecules
         */
        void setMolToProcMap(const std::vector<int>& molToProcMap) {
            molToProcMap_ = molToProcMap;
        }
        
    private:

        void setupFortranParallel();
        
        /** 
         * The size of molToProcMap_ is equal to total number of molecules in the system.
         *  It maps a molecule to the processor on which it resides. it is filled by SimCreator once and only
         * once.
         */        
        std::vector<int> molToProcMap_; 

#endif

};

} //namespace oopse
#endif //BRAINS_SIMMODEL_HPP

