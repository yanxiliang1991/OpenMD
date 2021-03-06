/*
 * Copyright (c) 2005 The University of Notre Dame. All Rights Reserved.
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



#include "applications/dynamicProps/SystemDipoleCorrFunc.hpp"
#include "utils/Revision.hpp"
#include "brains/ForceManager.hpp"
#include "brains/Thermo.hpp"

namespace OpenMD {

  // Just need the dipole of the system for each frame
  SystemDipoleCorrFunc::SystemDipoleCorrFunc(SimInfo* info,
                                             const std::string& filename, 
                                             const std::string& sele1, 
                                             const std::string& sele2,
                                             long long int memSize)
    : FrameTimeCorrFunc(info, filename, sele1, sele2, 
			DataStorage::dslPosition | 
			DataStorage::dslVelocity |
			DataStorage::dslParticlePot,
			memSize){
    
      setCorrFuncType("SystemDipoleCorrFunc");
      setOutputName(getPrefix(dumpFilename_) + ".sysdipcorr");
      histogram_.resize(nTimeBins_); 
      count_.resize(nTimeBins_);
    }

  void SystemDipoleCorrFunc::correlateFrames(int frame1, int frame2) {
    Snapshot* snapshot1 = bsMan_->getSnapshot(frame1);
    assert(snapshot1);
    Vector3d dipoleMoment1 = thermo_->getSystemDipole();

    Snapshot* snapshot2 = bsMan_->getSnapshot(frame2);
    assert(snapshot2);
    Vector3d dipoleMoment2 = thermo_->getSystemDipole();
    assert(snapshot1 && snapshot2);

    RealType time1 = snapshot1->getTime();
    RealType time2 = snapshot2->getTime();
       
    int timeBin = int ((time2 - time1) /deltaTime_ + 0.5);

    histogram_[timeBin] += dot(dipoleMoment1, dipoleMoment2);
    
    count_[timeBin]++;
    
  }

  void SystemDipoleCorrFunc::postCorrelate() {
    for (unsigned int i =0 ; i < nTimeBins_; ++i) {
      if (count_[i] > 0) {
        histogram_[i] /= count_[i];
      }
    }
  }

  void SystemDipoleCorrFunc::preCorrelate() {
    // Fill the histogram with zeroes, 0.0
    std::fill(histogram_.begin(), histogram_.end(), 0.0);
    // count array set to zero
    std::fill(count_.begin(), count_.end(), 0);
    thermo_ = new Thermo(info_);
  }   


  void SystemDipoleCorrFunc::writeCorrelate() {
    std::ofstream ofs(getOutputFileName().c_str());

    if (ofs.is_open()) {
      Revision r;
      
      ofs << "# " << getCorrFuncType() << "\n";
      ofs << "# OpenMD " << r.getFullRevision() << "\n";
      ofs << "# " << r.getBuildDate() << "\n";
      ofs << "# selection script1: \"" << selectionScript1_ ;
      ofs << "\"\tselection script2: \"" << selectionScript2_ << "\"\n";
      if (!paramString_.empty())
        ofs << "# parameters: " << paramString_ << "\n";

      ofs << "#time\tnormalizedTau\n";

      for (unsigned int i = 0; i < nTimeBins_; ++i) {
        ofs << time_[i] << "\t" << 
	      histogram_[i] << "\n";
      }
            
    } else {
      sprintf(painCave.errMsg,
              "SystemDipoleCorrFunc::writeCorrelate Error: fail to open %s\n",
              getOutputFileName().c_str());
      painCave.isFatal = 1;
      simError();        
    }
    ofs.close();    
  }
}
