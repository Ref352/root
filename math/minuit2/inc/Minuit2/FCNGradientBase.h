// @(#)root/minuit2:$Id$
// Authors: M. Winkler, F. James, L. Moneta, A. Zsenei, E.G.P. Bos   2003-2017

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2005 LCG ROOT Math team,  CERN/PH-SFT                *
 *                                                                    *
 **********************************************************************/

#ifndef ROOT_Minuit2_FCNGradientBase
#define ROOT_Minuit2_FCNGradientBase

#include "Minuit2/FCNBase.h"

#include <vector>

namespace ROOT {

namespace Minuit2 {

//________________________________________________________________________
/** Extension of the FCNBase for providing the analytical Gradient of the
    function. The user-Gradient is checked at the beginning of the
    minimization against the Minuit internal numerical Gradient in order to
    spot problems in the analytical Gradient calculation. This can be turned
    off by overriding CheckGradient() to make it return "false".
    The size of the output Gradient vector must be equal to the size of the
    input Parameter vector.
    Minuit does a check of the user Gradient at the beginning, if this is not
    wanted the method "CheckGradient()" has to be overridden to return
    "false".
 */

enum class GradientParameterSpace {
  External, Internal
};

class FCNGradientBase : public FCNBase {

public:
   ~FCNGradientBase() override {}

   virtual std::vector<double> Gradient(std::span<const double> ) const = 0;
   virtual std::vector<double> GradientWithPrevResult(std::span<const double> parameters, double * /*previous_grad*/,
                                                      double * /*previous_g2*/, double * /*previous_gstep*/) const
   {
      return Gradient(parameters);
   };

   virtual bool CheckGradient() const { return true; }

   virtual GradientParameterSpace gradParameterSpace() const {
      return GradientParameterSpace::External;
   };

   /// return second derivatives (diagonal of the Hessian matrix)
   virtual std::vector<double> G2(std::span<const double> ) const { return std::vector<double>();}

   /// return Hessian
   virtual std::vector<double> Hessian(std::span<const double> ) const { return std::vector<double>();}

   virtual bool HasHessian() const { return false; }

   virtual bool HasG2() const { return false; }


};

} // namespace Minuit2

} // namespace ROOT

#endif // ROOT_Minuit2_FCNGradientBase
