/*
 * Copyright (c) 2008, 2010 The University of Notre Dame. All Rights Reserved.
 *
 * The University of Notre Dame grants you ("Licensee") a
 * non-exclusive, royalty free, license to use, modify and
 * redistribute this software in source and binary code form, provided
 * that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
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
 *
 * SUPPORT OPEN SCIENCE!  If you use OpenMD or its source code in your
 * research, please cite the appropriate papers when you publish your
 * work.  Good starting points are:
 *                                                                      
 * [1]  Meineke, et al., J. Comp. Chem. 26, 252-271 (2005).             
 * [2]  Fennell & Gezelter, J. Chem. Phys. 124, 234104 (2006).          
 * [3]  Sun, Lin & Gezelter, J. Chem. Phys. 128, 234107 (2008).          
 * [4]  Kuang & Gezelter,  J. Chem. Phys. 133, 164101 (2010).
 * [5]  Vardeman, Stocker & Gezelter, J. Chem. Theory Comput. 7, 834 (2011).
 */
 
#ifndef INTEGRATOR_LANGEVINHULLFORCEMANAGER_HPP
#define INTEGRATOR_LANGEVINHULLFORCEMANAGER_HPP

#include "brains/ForceManager.hpp"
#include "brains/Thermo.hpp"
#include "brains/Velocitizer.hpp"
#include "primitives/Molecule.hpp"
#include "math/Hull.hpp"
#include "math/Triangle.hpp"
#include "math/SeqRandNumGen.hpp"

using namespace std;
namespace OpenMD {
   
  /**
   * @class LangevinHullForceManager
   * Force manager for NPT Langevin Hull Dynamics applying friction
   * and random forces as well as torques.  Stochastic force is
   * determined by the area of surface triangles on the convex hull.
   * See: Vardeman, Stocker & Gezelter, J. Chem. Theory Comput. 7, 834 (2011),
   *      and Kohanoff et al. CHEMPHYSCHEM 6, 1848-1852 (2005).
   */
  class LangevinHullForceManager : public ForceManager{
    
  public:
    LangevinHullForceManager(SimInfo * info);
    virtual ~LangevinHullForceManager();
    
  protected:
    virtual void postCalculation();
    
  private:
    vector<Vector3d> genTriangleForces(int nTriangles, RealType variance);
    
    Globals* simParams;
    SeqRandNumGen randNumGen_;    
    Velocitizer* veloMunge;
    
    RealType dt_;
    RealType targetTemp_;
    RealType targetPressure_; 
    RealType viscosity_;
    
    RealType variance_;
    
    enum HullTypeEnum {
      hullConvex,
      hullAlphaShape,
      hullUnknown
    };
    
    map<string, HullTypeEnum> stringToEnumMap_;
    HullTypeEnum hullType_;
    
    bool doThermalCoupling_;
    bool doPressureCoupling_;
    
    Hull* surfaceMesh_;
    vector<StuntDouble*> localSites_;
  };
  
} //end namespace OpenMD
#endif //LANGEVINHULL_FORCEMANAGER

