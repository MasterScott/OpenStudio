/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_CURVEQUADRATICLINEAR_IMPL_HPP
#define MODEL_CURVEQUADRATICLINEAR_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CurveQuadraticLinear_Impl is a Curve_Impl that is the implementation class for CurveQuadraticLinear.*/
  class MODEL_API CurveQuadraticLinear_Impl : public Curve_Impl {
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
   public:

    /** @name Constructors and Destructors */
    //@{

    CurveQuadraticLinear_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CurveQuadraticLinear_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                              Model_Impl* model,
                              bool keepHandle);

    CurveQuadraticLinear_Impl(const CurveQuadraticLinear_Impl& other,
                              Model_Impl* model,
                              bool keepHandle);

    virtual ~CurveQuadraticLinear_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual int numVariables() const override;

    virtual double evaluate(const std::vector<double>& x) const override;

    //@}
    /** @name Getters */
    //@{

    double coefficient1Constant() const;

    double coefficient2x() const;

    double coefficient3xPOW2() const;

    double coefficient4y() const;

    double coefficient5xTIMESY() const;

    double coefficient6xPOW2TIMESY() const;

    double minimumValueofx() const;

    double maximumValueofx() const;

    double minimumValueofy() const;

    double maximumValueofy() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    std::string inputUnitTypeforX() const;

    bool isInputUnitTypeforXDefaulted() const;

    std::string inputUnitTypeforY() const;

    bool isInputUnitTypeforYDefaulted() const;

    std::string outputUnitType() const;

    bool isOutputUnitTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setCoefficient1Constant(double coefficient1Constant);

    void setCoefficient2x(double coefficient2x);

    void setCoefficient3xPOW2(double coefficient3xPOW2);

    void setCoefficient4y(double coefficient4y);

    void setCoefficient5xTIMESY(double coefficient5xTIMESY);

    void setCoefficient6xPOW2TIMESY(double coefficient6xPOW2TIMESY);

    void setMinimumValueofx(double minimumValueofx);

    void setMaximumValueofx(double maximumValueofx);

    void setMinimumValueofy(double minimumValueofy);

    void setMaximumValueofy(double maximumValueofy);

    void setMinimumCurveOutput(boost::optional<double> minimumCurveOutput);

    void resetMinimumCurveOutput();

    void setMaximumCurveOutput(boost::optional<double> maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforX(std::string inputUnitTypeforX);

    void resetInputUnitTypeforX();

    bool setInputUnitTypeforY(std::string inputUnitTypeforY);

    void resetInputUnitTypeforY();

    bool setOutputUnitType(std::string outputUnitType);

    void resetOutputUnitType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CurveQuadraticLinear");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CURVEQUADRATICLINEAR_IMPL_HPP

