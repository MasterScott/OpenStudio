/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_WATERHEATERHEATPUMPPUMPEDCONDENSER_HPP
#define MODEL_WATERHEATERHEATPUMPPUMPEDCONDENSER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class WaterHeaterHeatPumpPumpedCondenser_Impl;

} // detail

/** WaterHeaterHeatPumpPumpedCondenser is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:WaterHeater:HeatPump:PumpedCondenser'. */
class MODEL_API WaterHeaterHeatPumpPumpedCondenser : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit WaterHeaterHeatPumpPumpedCondenser(const Model& model);

  explicit WaterHeaterHeatPumpPumpedCondenser(const Model& model,
                                              const ModelObject & dxCoil,
                                              const HVACComponent & tank,
                                              const HVACComponent & fan,
                                              Schedule & compressorSetpointTemperatureSchedule,
                                              Schedule & inletAirMixerSchedule);

  virtual ~WaterHeaterHeatPumpPumpedCondenser() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> inletAirConfigurationValues();

  static std::vector<std::string> compressorLocationValues();

  static std::vector<std::string> fanPlacementValues();

  static std::vector<std::string> parasiticHeatRejectionLocationValues();

  static std::vector<std::string> tankElementControlLogicValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  Schedule compressorSetpointTemperatureSchedule() const;

  double deadBandTemperatureDifference() const;

  double condenserBottomLocation() const;

  double condenserTopLocation() const;

  boost::optional<double> evaporatorAirFlowRate() const;

  bool isEvaporatorAirFlowRateAutocalculated() const;

  std::string inletAirConfiguration() const;

  boost::optional<Schedule> inletAirTemperatureSchedule() const;

  boost::optional<Schedule> inletAirHumiditySchedule() const;

  HVACComponent tank() const;

  ModelObject dXCoil() const;

  double minimumInletAirTemperatureforCompressorOperation() const;

  double maximumInletAirTemperatureforCompressorOperation() const;

  std::string compressorLocation() const;

  boost::optional<Schedule> compressorAmbientTemperatureSchedule() const;

  HVACComponent fan() const;

  std::string fanPlacement() const;

  double onCycleParasiticElectricLoad() const;

  double offCycleParasiticElectricLoad() const;

  std::string parasiticHeatRejectionLocation() const;

  boost::optional<Schedule> inletAirMixerSchedule() const;

  std::string tankElementControlLogic() const;

  boost::optional<double> controlSensor1HeightInStratifiedTank() const;

  double controlSensor1Weight() const;

  boost::optional<double> controlSensor2HeightInStratifiedTank() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setCompressorSetpointTemperatureSchedule(Schedule& schedule);

  bool setDeadBandTemperatureDifference(double deadBandTemperatureDifference);

  bool setCondenserBottomLocation(double condenserBottomLocation);

  bool setCondenserTopLocation(double condenserTopLocation);

  bool setEvaporatorAirFlowRate(double evaporatorAirFlowRate);

  void autocalculateEvaporatorAirFlowRate();

  bool setInletAirConfiguration(std::string inletAirConfiguration);

  bool setInletAirTemperatureSchedule(Schedule& schedule);

  void resetInletAirTemperatureSchedule();

  bool setInletAirHumiditySchedule(Schedule& schedule);

  void resetInletAirHumiditySchedule();

  bool setTank(const HVACComponent& waterHeaterStratified);

  bool setDXCoil(const ModelObject& heatPumpWaterHeaterDXCoilsPumped);

  bool setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation);

  bool setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation);

  bool setCompressorLocation(std::string compressorLocation);

  bool setCompressorAmbientTemperatureSchedule(Schedule& schedule);

  void resetCompressorAmbientTemperatureSchedule();

  bool setFan(const HVACComponent& fansOnOff);

  bool setFanPlacement(std::string fanPlacement);

  bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

  bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

  bool setParasiticHeatRejectionLocation(std::string parasiticHeatRejectionLocation);

  bool setInletAirMixerSchedule(Schedule& schedule);

  void resetInletAirMixerSchedule();

  bool setTankElementControlLogic(std::string tankElementControlLogic);

  bool setControlSensor1HeightInStratifiedTank(double controlSensor1HeightInStratifiedTank);

  void resetControlSensor1HeightInStratifiedTank();

  bool setControlSensor1Weight(double controlSensor1Weight);

  bool setControlSensor2HeightInStratifiedTank(double controlSensor2HeightInStratifiedTank);

  void resetControlSensor2HeightInStratifiedTank();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::WaterHeaterHeatPumpPumpedCondenser_Impl ImplType;

  explicit WaterHeaterHeatPumpPumpedCondenser(std::shared_ptr<detail::WaterHeaterHeatPumpPumpedCondenser_Impl> impl);

  friend class detail::WaterHeaterHeatPumpPumpedCondenser_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.WaterHeaterHeatPumpPumpedCondenser");
};

/** \relates WaterHeaterHeatPumpPumpedCondenser*/
typedef boost::optional<WaterHeaterHeatPumpPumpedCondenser> OptionalWaterHeaterHeatPumpPumpedCondenser;

/** \relates WaterHeaterHeatPumpPumpedCondenser*/
typedef std::vector<WaterHeaterHeatPumpPumpedCondenser> WaterHeaterHeatPumpPumpedCondenserVector;

} // model
} // openstudio

#endif // MODEL_WATERHEATERHEATPUMPPUMPEDCONDENSER_HPP

