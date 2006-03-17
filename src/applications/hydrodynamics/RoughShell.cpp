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

#include "applications/hydrodynamics/RoughShell.hpp"
#include "applications/hydrodynamics/ShapeBuilder.hpp"
#include "brains/SimInfo.hpp"
namespace oopse {

RoughShell::RoughShell(StuntDouble* sd, SimInfo* info) : ApproximationModel(sd, info){
    shape_=ShapeBuilder::createShape(sd);
    Globals* simParams = info->getSimParams();
    if (simParams->haveBeadSize()) {
        sigma_ = simParams->getBeadSize();
    }else {

    }
}

struct BeadLattice {
    Vector3d origin;
    double radius;
    bool interior;
};

struct ExteriorFunctor : public std::unary_function<BeadLattice, bool>{

    bool operator() (const BeadLattice& bead) {
        return !bead.interior;
    }

};

struct InteriorFunctor  : public std::unary_function<BeadLattice, bool>{

    bool operator() (const BeadLattice& bead) {
        return bead.interior;
    }

};
bool RoughShell::createBeads(std::vector<BeadParam>& beads) {
    std::pair<Vector3d, Vector3d> boxBoundary = shape_->getBox();
    double len = boxBoundary.second[0] - boxBoundary.first[0];
    int numLattices = static_cast<int>(len/sigma_) + 1;
    Grid3D<BeadLattice>  grid(numLattices, numLattices, numLattices);

    //fill beads
    for (int i = 0; i < numLattices; ++i) {
        for (int j = 0; j < numLattices; ++j) {
            for (int k = 0; k < numLattices; ++k) {
                BeadLattice& currentBead = grid(i, j, k);
                currentBead.origin = Vector3d(i*sigma_ + boxBoundary.first[0], j *sigma_ + boxBoundary.first[1], k*sigma_+ boxBoundary.first[2]);
                currentBead.radius = sigma_;
                currentBead.interior = shape_->isInterior(grid(i, j, k).origin);                
            }
        }
    }

    //remove embedded beads 
    for (int i = 0; i < numLattices; ++i) {
        for (int j = 0; j < numLattices; ++j) {
            for (int k = 0; k < numLattices; ++k) {
                 std::vector<BeadLattice> neighborCells = grid.getAllNeighbors(i, j, k);
                 //if one of its neighbor cells is exterior, current cell is on the surface

                 std::vector<BeadLattice>::iterator ei = std::find_if(neighborCells.begin(), neighborCells.end(), ExteriorFunctor());                
                 std::vector<BeadLattice>::iterator ii = std::find_if(neighborCells.begin(), neighborCells.end(), InteriorFunctor());                
                 
                  if (ei != neighborCells.end() && ii != neighborCells.end()) {
                      BeadParam surfaceBead;
                      surfaceBead.atomName = "Bead";
                      surfaceBead.pos = grid(i, j, k).origin;
                      surfaceBead.radius = grid(i, j, k).radius;
                      beads.push_back(surfaceBead);
                  }

            }
        }
    }
     
    return true;
}

}
