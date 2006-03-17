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

#include "applications/hydrodynamics/Spheric.hpp"
#include "applications/hydrodynamics/HydrodynamicsModel.hpp"
namespace oopse {
Spheric::Spheric(Vector3d origin, double radius) : origin_(origin), radius_(radius){

}

bool Spheric::isInterior(Vector3d pos) {
    Vector3d r = pos - origin_;

    bool result;
    if (r.length() < radius_)
        result = true;
    else
        result = false;
    
    return result;
}

std::pair<Vector3d, Vector3d> Spheric::getBox() {
    std::pair<Vector3d, Vector3d>  boundary;
    Vector3d r(radius_, radius_, radius_);
    boundary.first = origin_ - r;
    boundary.second = origin_ + r;
    return boundary;
}

bool Spheric::calcHydroProps(HydrodynamicsModel* model, double viscosity, double temperature) {
    return model->calcHydroProps(this, viscosity, temperature);
}

}
